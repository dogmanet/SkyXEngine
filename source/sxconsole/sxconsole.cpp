#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <conio.h>
#include <memory>
#include <process.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <common/Array.h>
#include <math.h>

#include "ColorPrint.h"

#include <mutex>


typedef std::mutex Mutex;
typedef std::unique_lock<Mutex> ScopedLock;

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "59705"
//#define COMMAND_PORT "59706"

char * g_szUserInp = NULL;
HANDLE g_hStdOut = NULL;
COORD g_iOldOutPos;
ColorPrint * g_pColor;

BOOL g_bRunning = 1;
SOCKET ClientSocket = INVALID_SOCKET;

AnsiColor g_iCurColorFG;
AnsiColor g_iCurColorBG;

int g_iServerPort = 59705;
char g_szServerPort[8];
char g_szClientPort[8];

bool g_bExitOnDisconnect = false;

#define DEFAULT_PORT g_szServerPort
#define COMMAND_PORT g_szClientPort

Mutex mx;

struct HistoryItem
{
	const char * szOrigCmd;
	char * szEditCmd;
};

Array<HistoryItem> g_vHistory;
int g_iHistoryPointer;

#pragma warning(disable:4996)

char * GetHistItemPtr()
{
	HistoryItem * hi = &g_vHistory[g_iHistoryPointer];
	if(!hi->szEditCmd)
	{
		hi->szEditCmd = new char[DEFAULT_BUFLEN];
		hi->szEditCmd[0] = 0;
		if(hi->szOrigCmd)
		{
			strcpy(hi->szEditCmd, hi->szOrigCmd);
		}
	}
	return(hi->szEditCmd);
}

void PutHistory()
{
	HistoryItem * hi = &g_vHistory[g_iHistoryPointer];
	char * cmd = hi->szEditCmd;
	hi->szEditCmd = NULL;
	if(!strlen(cmd))
	{
		delete[] cmd;
		g_iHistoryPointer = g_vHistory.size() - 1;
		return;
	}
	if(hi->szOrigCmd)
	{
		//put new item
		g_iHistoryPointer = g_vHistory.size() - 2;
		if(strcmp(g_vHistory[g_iHistoryPointer].szOrigCmd, cmd)) // last cmd not equals current
		{
			hi = &g_vHistory[g_vHistory.size() - 1];
			hi->szOrigCmd = cmd;
			cmd = hi->szEditCmd;
			hi->szEditCmd = NULL;
			g_vHistory.push_back({NULL, cmd});
			g_iHistoryPointer = g_vHistory.size() - 1;
		}
		else
		{
			g_iHistoryPointer = g_vHistory.size() - 1;
			delete[] g_vHistory[g_iHistoryPointer].szEditCmd;
			g_vHistory[g_iHistoryPointer].szEditCmd = cmd;
			cmd[0] = 0;
		}
	}
	else
	{
		g_iHistoryPointer = g_vHistory.size() - 2;
		if(g_iHistoryPointer < 0 || strcmp(g_vHistory[g_iHistoryPointer].szOrigCmd, cmd)) // last cmd not equals current
		{
			hi->szOrigCmd = cmd;
			++g_iHistoryPointer;
			g_vHistory.push_back({NULL, NULL});
		}
		else
		{
			hi->szEditCmd = cmd;
			cmd[0] = 0;
		}
		++g_iHistoryPointer;
	}
}

void SetColor(AnsiColor clr)
{
	g_iCurColorFG = clr;
	g_pColor->setColor(clr);
}

void SetColorBG(AnsiColor clr)
{
	g_iCurColorBG = clr;
	g_pColor->setBG(clr);
}

void SyncOutput(bool begin)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(g_hStdOut, &csbi);
	if(csbi.dwSize.Y - csbi.dwCursorPosition.Y < 100)
	{
		csbi.dwSize.Y += 100;
		SetConsoleScreenBufferSize(g_hStdOut, csbi.dwSize);
	}
	if(begin)
	{
		//if(csbi.dwCursorPosition.X != 0)
		{
			char * str = (char*)alloca(sizeof(char)* (csbi.dwMaximumWindowSize.X + 1));
			memset(str, ' ', csbi.dwMaximumWindowSize.X);
			str[csbi.dwMaximumWindowSize.X] = 0;
			csbi.dwCursorPosition.X = 0;
			SetConsoleCursorPosition(g_hStdOut, csbi.dwCursorPosition);
			printf(str);
		}
		SetConsoleCursorPosition(g_hStdOut, g_iOldOutPos);
		g_pColor->setColor(g_iCurColorFG);
		g_pColor->setBG(g_iCurColorBG);
	}
	else
	{
		g_iOldOutPos = csbi.dwCursorPosition;

		if(csbi.dwCursorPosition.X != 0)
		{
			csbi.dwCursorPosition.X = 0;
			++csbi.dwCursorPosition.Y;
			SetConsoleCursorPosition(g_hStdOut, csbi.dwCursorPosition);
		}
		g_pColor->setColor(ANSI_LGREEN);
		if(g_szUserInp)
		{
			char * s;
			if((s = strstr(g_szUserInp, "\n")))
			{
				*s = 0;
			}
			printf("> %s", g_szUserInp);
		}
		else
		{
			printf("> ");
		}
		//g_pColor->setDefault();
	}
}

void ClearAll()
{
	mx.lock();
	SyncOutput(1);

	COORD tl = {0, 0};
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacterA(console, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);

	SyncOutput(0);
	mx.unlock();
}

void WriteOutput(const char * buf, ...)
{
	mx.lock();
	va_list va;
	va_start(va, buf);
	SyncOutput(1);
	vprintf(buf, va);
	SyncOutput(0);
	va_end(va);
	mx.unlock();
}

void WriteColored(char ** _buf)
{
	mx.lock();
	//mx.lock();
	SyncOutput(1);
	char * esc = NULL;
	char * buf = *_buf;

	int clr;
	bool done = false;

	while(!done)
	{
		esc = strstr(buf, "\033");
		if(esc)
		{
			*esc = 0;
			printf("%s", buf);
			*esc = '\033';
			++esc;
			if(!strstr(esc, "m") && strlen(esc) < 20)
			{
				*_buf = esc - 1;
				goto end;
			}
			if(*esc != '[')
			{
				continue;
			}
			++esc;
			if(!memcmp(esc, "38;5;", 5))
			{
				esc += 5;
				if(!sscanf(esc, "%d", &clr))
				{
					continue;
				}
				esc += (int)(log10(clr) + 1);
				if(*esc != 'm')
				{
					continue;
				}
				++esc;
				//swap 0 - 2 bits
				int r = clr & 1;
				int b = (clr >> 2) & 1;
				if(r)
				{
					clr |= 4;
				}
				else
				{
					clr &= ~4;
				}
				if(b)
				{
					clr |= 1;
				}
				else
				{
					clr &= ~1;
				}
				SetColor((AnsiColor)clr);
			}
			else if(!memcmp(esc, "48;5;", 5))
			{
				esc += 5;
				if(!sscanf(esc, "%d", &clr))
				{
					continue;
				}
				esc += (int)(floor(log10(clr)) + 1);
				if(*esc != 'm')
				{
					continue;
				}
				++esc;
				//swap 0 - 2 bits
				int r = clr & 1;
				int b = (clr >> 2) & 1;
				if(r)
				{
					clr |= 4;
				}
				else
				{
					clr &= ~4;
				}
				if(b)
				{
					clr |= 1;
				}
				else
				{
					clr &= ~1;
				}
				SetColorBG((AnsiColor)clr);
			}
			else if(!memcmp(esc, "0m", 2))
			{
				//reset color
				SetColor(g_pColor->getDefaultFG());
				SetColorBG(g_pColor->getDefaultBG());
				esc += 2;
			}
			buf = esc;
		}
		else
		{
			done = true;
		}
	}
	printf("%s", buf);
	*_buf = NULL;

end:
	SyncOutput(0);
	mx.unlock();
}

bool g_bConnected = true;

SOCKET ListenSocket = INVALID_SOCKET;

void threadServer(void*)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET _ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		SetColor(ANSI_RED);
		WriteOutput("WSAStartup failed with error: %d\n", iResult);
		SetColor(g_pColor->getDefaultFG());
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if(iResult != 0)
	{
		SetColor(ANSI_RED);
		WriteOutput("getaddrinfo failed with error: %d\n", iResult);
		SetColor(g_pColor->getDefaultFG());
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(ListenSocket == INVALID_SOCKET)
	{
		SetColor(ANSI_RED);
		WriteOutput("socket failed with error: %ld\n", WSAGetLastError());
		SetColor(g_pColor->getDefaultFG());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		SetColor(ANSI_RED);
		WriteOutput("bind failed with error: %d\n", WSAGetLastError());
		SetColor(g_pColor->getDefaultFG());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if(iResult == SOCKET_ERROR)
	{
		SetColor(ANSI_RED);
		WriteOutput("listen failed with error: %d\n", WSAGetLastError());
		SetColor(g_pColor->getDefaultFG());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	SetColor(ANSI_LCYAN);
	WriteOutput("Waiting for connection...\n");
	SetColor(g_pColor->getDefaultFG());
	// Accept a client socket

	int offset = 0;
	while(g_bRunning)
	{
		_ClientSocket = accept(ListenSocket, NULL, NULL);
		if(_ClientSocket == INVALID_SOCKET)
		{
			SetColor(ANSI_RED);
			WriteOutput("accept failed with error: %d\n", WSAGetLastError());
			SetColor(g_pColor->getDefaultFG());
			continue;
		}


		// No longer need server socket
		closesocket(ListenSocket);

		ClearAll();
		SetColor(ANSI_LGREEN);
		WriteOutput("Connected!\n");
		SetColor(g_pColor->getDefaultFG());

		do
		{

			iResult = recv(_ClientSocket, recvbuf + offset, recvbuflen - 1 - offset, 0);
			if(iResult > 0)
			{
				recvbuf[iResult + offset] = 0;
				char * buf = recvbuf;
				WriteColored(&buf);
				offset = 0;
				if(buf) // not fully parsed
				{
					memmove(recvbuf, buf, (offset = strlen(buf)) + 1);
				}
			}
			else if(iResult == 0)
			{
				SetColor(ANSI_RED);
				WriteOutput("Connection closed.\n");
				SetColor(g_pColor->getDefaultFG());
			}
			else
			{
				SetColor(ANSI_RED);
				WriteOutput("recv failed with error: %d\n", WSAGetLastError());
				SetColor(g_pColor->getDefaultFG());
				closesocket(_ClientSocket);
				_ClientSocket = INVALID_SOCKET;
				continue;
			}

		}
		while(iResult > 0);
		g_bConnected = false;
		// shutdown the connection since we're done
		iResult = shutdown(_ClientSocket, SD_SEND);
		if(iResult == SOCKET_ERROR)
		{
			SetColor(ANSI_RED);
			WriteOutput("shutdown failed with error: %d\n", WSAGetLastError());
			SetColor(g_pColor->getDefaultFG());
			closesocket(_ClientSocket);
			_ClientSocket = INVALID_SOCKET;
			break;
			continue;
		}
		_ClientSocket = INVALID_SOCKET;
		iResult = shutdown(ClientSocket, SD_SEND);
		if(iResult == SOCKET_ERROR)
		{
			SetColor(ANSI_RED);
			WriteOutput("shutdown failed with error: %d\n", WSAGetLastError());
			SetColor(g_pColor->getDefaultFG());
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			break;
			continue;
		}
		ClientSocket = INVALID_SOCKET;
		break;
	}


	closesocket(_ClientSocket);
	WSACleanup();

	if(g_bExitOnDisconnect)
	{
		ExitProcess(0);
	}
	else
	{
		_beginthread(threadServer, 0, 0);
	}
}

void InitCommandChannel(void*)
{
	WSADATA wsaData;
	int iResult;


	struct addrinfo *result = NULL;
	struct addrinfo hints;

//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		SetColor(ANSI_RED);
		WriteOutput("WSAStartup failed with error: %d\n", iResult);
		SetColor(g_pColor->getDefaultFG());
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, COMMAND_PORT, &hints, &result);
	if(iResult != 0)
	{
		SetColor(ANSI_RED);
		WriteOutput("getaddrinfo failed with error: %d\n", iResult);
		SetColor(g_pColor->getDefaultFG());
		WSACleanup();
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(ListenSocket == INVALID_SOCKET)
	{
		SetColor(ANSI_RED);
		WriteOutput("socket failed with error: %ld\n", WSAGetLastError());
		SetColor(g_pColor->getDefaultFG());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if(iResult == SOCKET_ERROR)
	{
		SetColor(ANSI_RED);
		WriteOutput("bind failed with error: %d\n", WSAGetLastError());
		SetColor(g_pColor->getDefaultFG());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if(iResult == SOCKET_ERROR)
	{
		SetColor(ANSI_RED);
		WriteOutput("listen failed with error: %d\n", WSAGetLastError());
		SetColor(g_pColor->getDefaultFG());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	//SetColor(ANSI_LCYAN);
	//WriteOutput("\n");
	//SetColor(g_pColor->getDefaultFG());
	// Accept a client socket

	int offset = 0;
	while(g_bRunning)
	{
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if(ClientSocket == INVALID_SOCKET)
		{
			SetColor(ANSI_RED);
			//WriteOutput("accept failed with error: %d\n", WSAGetLastError());
			//SetColor(g_pColor->getDefaultFG());
			continue;
		}
		g_bConnected = true;
		//SetColor(ANSI_LGREEN);
		//WriteOutput("Connected!\n");
		//SetColor(g_pColor->getDefaultFG());

		while(g_bConnected)
		{
			Sleep(1000);
		}

		/*do
		{

		iResult = recv(ClientSocket, recvbuf + offset, recvbuflen - 1 - offset, 0);
		if(iResult > 0)
		{
		recvbuf[iResult + offset] = 0;
		char * buf = recvbuf;
		WriteColored(&buf);
		offset = 0;
		if(buf) // not fully parsed
		{
		memmove(recvbuf, buf, (offset = strlen(buf)) + 1);
		}
		}
		else if(iResult == 0)
		{
		//	SetColor(ANSI_RED);
		//	WriteOutput("Connection closed.\n");
		//	SetColor(g_pColor->getDefaultFG());
		}
		else
		{
		//	SetColor(ANSI_RED);
		//	WriteOutput("recv failed with error: %d\n", WSAGetLastError());
		//	SetColor(g_pColor->getDefaultFG());
		closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
		continue;
		}

		}
		while(iResult > 0);*/

		// shutdown the connection since we're done
		iResult = shutdown(ClientSocket, SD_SEND);
		if(iResult == SOCKET_ERROR)
		{
			SetColor(ANSI_RED);
			//WriteOutput("shutdown failed with error: %d\n", WSAGetLastError());
			SetColor(g_pColor->getDefaultFG());
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			continue;
		}
		ClientSocket = INVALID_SOCKET;
	}
}
void CloseCommandChannel()
{
	// No longer need server socket
	closesocket(ListenSocket);


	closesocket(ClientSocket);
	WSACleanup();
}

BOOL WINAPI HandlerRoutine(
	_In_ DWORD dwCtrlType
	)
{
	if(CTRL_CLOSE_EVENT == dwCtrlType)
	{
		if(ClientSocket != INVALID_SOCKET)
		{
			send(ClientSocket, "exit\n", strlen("exit\n"), 0);
		}
	}
	return(FALSE);
}

int main(int argc, char ** argv)
{
	g_pColor = new ColorPrint();
	g_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(g_hStdOut, &csbi);

	g_iCurColorBG = g_pColor->getDefaultBG();
	g_iCurColorFG = g_pColor->getDefaultFG();

	csbi.dwMaximumWindowSize.X;
	csbi.dwMaximumWindowSize.Y;
	SetColor(ANSI_LCYAN);
	WriteOutput("SkyXEngine Console\n");
	SetColor(g_pColor->getDefaultFG());

	g_iHistoryPointer = 0;
	g_vHistory.push_back({NULL, NULL});

	if(argc == 2)
	{
		g_bExitOnDisconnect = true;
		sscanf(argv[1], "%d", &g_iServerPort);
	}
	sprintf(g_szServerPort, "%d", g_iServerPort);
	sprintf(g_szClientPort, "%d", g_iServerPort + 1);
	
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);

	_beginthread(threadServer, 0, 0);
	_beginthread(InitCommandChannel, 0, 0);

	int i = 0;

	char ch;

	//char str[64];
	//strcpy(str, "Some " COLOR_RED "red " COLOR_RESET "text\n");
	//WriteColored(str);

	SetConsoleTitle(L"sxconsole");

	while(1)
	{
		ch = (char)getch();
		if(ch == '\b')
		{
			if(!g_szUserInp)
			{
				g_szUserInp = GetHistItemPtr();
				i = strlen(g_szUserInp);
			}
			putch(g_szUserInp[i] = ch);
			i -= 2;
			if(i < 0)
			{
				i = -1;
			}
			putch(' ');
			putch('\b');
			g_szUserInp[++i] = 0;
		}
		else if(ch == '\r')
		{
			if(!g_szUserInp)
			{
				g_szUserInp = GetHistItemPtr();
			}
			putch(g_szUserInp[i] = ch);
			g_szUserInp[i] = '\n';
			g_szUserInp[++i] = 0;
			if(ClientSocket != INVALID_SOCKET)
			{
				send(ClientSocket, g_szUserInp, strlen(g_szUserInp), 0);
				WriteOutput("] %s", g_szUserInp);
				PutHistory();
			}
			g_szUserInp = NULL;
			i = 0;
			//g_szUserInp[i = 0] = 0;
			WriteOutput("");
		}
		else if(ch == -32) // Up
		{
			int c = getch();
			switch(c)
			{
			case 72: // up
				--g_iHistoryPointer;
				if(g_iHistoryPointer < 0)
				{
					g_iHistoryPointer = 0;
				}
				g_szUserInp = GetHistItemPtr();
				i = strlen(g_szUserInp);
				WriteOutput("");
				break;
			case 80: // down
				++g_iHistoryPointer;
				if(g_iHistoryPointer >= (int)g_vHistory.size())
				{
					g_iHistoryPointer = g_vHistory.size() - 1;
				}
				g_szUserInp = GetHistItemPtr();
				i = strlen(g_szUserInp);
				WriteOutput("");
				break;
			case 75: // left
			case 77: // right
			case 83: // del
				break;
			}
		}
		else
		{
			if(!g_szUserInp)
			{
				g_szUserInp = GetHistItemPtr();
			}
			putch(g_szUserInp[i] = ch);
			g_szUserInp[++i] = 0;
		}
		
		//g_szUserInp
	}

	CloseCommandChannel();

	delete g_pColor;
	return(0);
}
