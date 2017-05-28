
#include "concmd.h"

//#define WIN32_LEAN_AND_MEAN

//#include <Windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <io.h>
#include <fcntl.h>

#include <process.h>

#include "concurrent_queue.h"

#include "cvars.h"

#pragma comment (lib, "Ws2_32.lib")

AssotiativeArray<String, ConCmd> g_mCmds;

SOCKET ConnectSocket = INVALID_SOCKET;
SOCKET CommandSocket = INVALID_SOCKET;

bool g_bRunning = false;
bool g_bRunningCmd = false;

typedef ConcurrentQueue<char *> CommandBuffer;
typedef std::mutex Mutex;
CommandBuffer g_vCommandBuffer;
Stack<CommandBuffer> g_cbufStack;

SX_LIB_API void Core_0RegisterConcmd(char * name, SXCONCMD cmd, const char * desc)
{
	ConCmd c;
	c.type = CMD_DEFAULT;
	c.cls = NULL;
	c.cmd.cmd = cmd;
	c.szDesc = NULL;
	if(desc)
	{
		c.szDesc = new char[strlen(desc) + 1];
		strcpy(c.szDesc, desc);
	}
	g_mCmds[name] = c;
}
SX_LIB_API void Core_0RegisterConcmdArg(char * name, SXCONCMDARG cmd, const char * desc)
{
	ConCmd c;
	c.type = CMD_ARG;
	c.cls = NULL;
	c.cmd.cmd = cmd;
	c.szDesc = NULL;
	if(desc)
	{
		c.szDesc = new char[strlen(desc) + 1];
		strcpy(c.szDesc, desc);
	}
	g_mCmds[name] = c;
}
SX_LIB_API void Core_0RegisterConcmdCls(char * name, void * pObject, SXCONCMDCLS cmd, const char * desc)
{
	ConCmd c;
	c.type = CMD_CLS;
	c.cls = (ConCmdStub*)pObject;
	c.cmd.clscmd = cmd;
	c.szDesc = NULL;
	if(desc)
	{
		c.szDesc = new char[strlen(desc) + 1];
		strcpy(c.szDesc, desc);
	}
	g_mCmds[name] = c;
}
SX_LIB_API void Core_0RegisterConcmdClsArg(char * name, void * pObject, SXCONCMDCLSARG cmd, const char * desc)
{
	ConCmd c;
	c.type = CMD_CLS_ARG;
	c.cls = (ConCmdStub*)pObject;
	c.cmd.clscmdarg = cmd;
	c.szDesc = NULL;
	if(desc)
	{
		c.szDesc = new char[strlen(desc) + 1];
		strcpy(c.szDesc, desc);
	}
	g_mCmds[name] = c;
}

Mutex g_conUpdMtx;

void ConsoleExecInternal(char * cmd, char * args)
{
	const AssotiativeArray<String, ConCmd>::Node * pNode;
	bool bCVarHandle = false;
	if(!g_mCmds.KeyExists(cmd, &pNode))
	{
		bCVarHandle = true;
		if(!g_mCmds.KeyExists("cvar_handle", &pNode))
		{
			return;
		}
	}
	ConCmd * _cmd = pNode->Val;
	char * arglist[64];
	int argc = 0;


	if(bCVarHandle)
	{
		arglist[argc++] = "cvar_handle";
		arglist[argc++] = cmd;
	}
	else
	{
		arglist[argc++] = cmd;
	}

	if(_cmd)
	{
		switch(_cmd->type)
		{
		case CMD_DEFAULT:
			((SXCONCMD)_cmd->cmd.cmd)();
			return;
		case CMD_CLS:
			(_cmd->cls->*(_cmd->cmd.clscmd))();
			return;
		}

		char * arg = args;

		bool IsInQuote = false;
		bool IsReading = false;
		for(; *args; ++args)
		{
			if(isspace(*args))
			{
				if(IsReading)
				{
					if(IsInQuote)
					{
						*arg++ = *args;
					}
					else
					{
						IsReading = false;
						*arg++ = 0;
					}
				}
			}
			else if(*args == '"')
			{
				if(!IsInQuote)
				{
					if(!IsReading)
					{
						arglist[argc++] = arg;
					}
					IsInQuote = true;
					IsReading = true;
				}
				else
				{
					IsInQuote = false;
					IsReading = false;
					*arg++ = 0;
				}
			}
			else if(*args == L'\\' && *(args + 1))
			{
				if(!IsReading)
				{
					arglist[argc++] = arg;
				}
				++args;
				switch(*args)
				{
				case L'\\':
					*arg++ = L'\\';
					break;
				case L'r':
					*arg++ = L'\r';
					break;
				case L'n':
					*arg++ = L'\n';
					break;
				case L'"':
					*arg++ = L'"';
					break;
				case L't':
					*arg++ = L'\t';
					break;
				case L' ':
					*arg++ = L' ';
					break;
				default:
					*arg++ = *args;
				}
				IsReading = true;
			}
			else
			{
				if(!IsReading)
				{
					arglist[argc++] = arg;
				}
				*arg++ = *args;
				IsReading = true;
			}
		}
		*arg++ = 0;
		switch(_cmd->type)
		{
		case CMD_ARG:
			((SXCONCMDARG)_cmd->cmd.cmd)(argc, (const char**)arglist);
			return;
		case CMD_CLS_ARG:
			(_cmd->cls->*(_cmd->cmd.clscmdarg))(argc, (const char**)arglist);
			return;
		}
	}
}
void ConsoleExecInternal(char * cmd)
{
	char * space = cmd;
	while((*++space) && !isspace(*space));
	if(*space)
	{
		*space = 0;
		++space;
	}
	else
	{
		space = cmd;
		while(*++space);
	}
	ConsoleExecInternal(cmd, space);
}

void ConsolePushBuffer()
{
	g_conUpdMtx.lock();
	g_cbufStack.push(g_vCommandBuffer);
	g_vCommandBuffer.clear();
	g_conUpdMtx.unlock();
}

SX_LIB_API void Core_0ConsoleUpdate()
{
	char * buf;
	while(g_vCommandBuffer.try_pop(buf))
	{
		ConsoleExecInternal(buf);
		mem_delete_a(buf);
	}
	if(!g_cbufStack.IsEmpty())
	{
		g_vCommandBuffer = g_cbufStack.pop();
	}

	//execute command buffer
}
SX_LIB_API void Core_0ConsoleExecCmd(const char * format, ...)
{
	va_list va;
	va_start(va, format);
	int len = _vscprintf(format, va) + 1;
	char * buf, *cbuf = NULL;
	if(len < 4096)
	{
		buf = (char*)alloca(len * sizeof(char));
	}
	else
	{
		cbuf = buf = new char[len];
	}
	vsprintf(buf, format, va);
	va_end(va);

	//g_vCommandBuffer
	
	char * nl;
	do
	{
		nl = strstr(buf, "\n");
		if(nl)
		{
			*nl = 0;
			++nl;

			while(isspace(*buf))
			{
				++buf;
			}

			if(!(*buf == '/' && *(buf + 1) == '/') && (len = strlen(buf)))
			{
				char * str = new char[len + 1];
				memcpy(str, buf, len + 1);
				g_conUpdMtx.lock();
				g_vCommandBuffer.push(str);
				g_conUpdMtx.unlock();
			}
			buf = nl;
		}
	}
	while(nl);

	while(isspace(*buf))
	{
		++buf;
	}

	if(!(*buf == '/' && *(buf + 1) == '/') && (len = strlen(buf)))
	{
		char * str = new char[len + 1];
		memcpy(str, buf, len + 1);
		g_conUpdMtx.lock();
		g_vCommandBuffer.push(str);
		g_conUpdMtx.unlock();
	}

	mem_delete_a(cbuf);
}

void echo(int argc, const char ** argv)
{
	for(int i = 1; i < argc; ++i)
	{
		printf("%s ", argv[i]);
	}
	printf("\n");
}

void exec(int argc, const char ** argv)
{
	if(argc != 2)
	{
		puts(COLOR_LRED "[exec]: Expected filename" COLOR_RESET);
		return;
	}
	ISXFile * f = Core_CrFile();
	if(f->Open(argv[1], CORE_FILE_BIN) < 0)
	{
		printf(COLOR_LRED "Couldn't exec '%s'\n" COLOR_RESET, argv[1]);
		return;
	}
	int len = f->GetSize() + 1;
	char * buf, *cbuf = NULL;
	if(len <= 4096)
	{
		buf = (char*)alloca(sizeof(char) * len);
	}
	else
	{
		buf = cbuf = new char[len];
	}

	f->ReadB(buf, len - 1);
	buf[len - 1] = 0;

	ConsolePushBuffer();
	Core_0ConsoleExecCmd("%s", buf);
	f->Release();

	mem_delete_a(cbuf);
}

void cmd_exit()
{
	PostQuitMessage(0);
}

void cvar_handle(int argc, const char ** argv)
{
	if(argc == 2)
	{
		PrintCVar(argv[1]);
	}
	else if(argc > 2)
	{
		SetCVar(argv[1], argv[2]);
	}
}

void cmd_help(int argc, const char ** argv)
{
	if(argc == 2)
	{
		const AssotiativeArray<String, ConCmd>::Node * pNode;
		if(!g_mCmds.KeyExists(argv[1], &pNode))
		{
			printf(COLOR_LRED "Unknown command '%s'\n" COLOR_RESET, argv[1]);
			return;
		}
		if(pNode->Val->szDesc)
		{
			printf(COLOR_GREEN "%s\n" COLOR_RESET, pNode->Val->szDesc);
		}
		else
		{
			printf(COLOR_RED "No description for '%s'\n" COLOR_RESET, argv[1]);
		}
	}
	else
	{
		printf(COLOR_GREEN "Usage:\n    " COLOR_LGREEN "help <command>" COLOR_GREEN " - Show command description\n" COLOR_RESET);
	}
}

void ConsoleRegisterCmds()
{
	Core_0RegisterConcmdArg("echo", echo, "Echoes all parameters to console");
	Core_0RegisterConcmdArg("exec", exec, "Executes given cfg file");
	Core_0RegisterConcmd("exit", cmd_exit, "Shuts down the engine");
	Core_0RegisterConcmdArg("cvar_handle", cvar_handle, "Handle to show or set cvar value");
	Core_0RegisterConcmdArg("help", cmd_help, "Handle to show command description");
}

SX_LIB_API UINT_PTR Core_ConsoleGetOutHandler()
{
	return(ConnectSocket);
}

/*!
@TODO: Handle this with TaskManager
*/
void ConsoleRecv(void*)
{
	while(g_bRunningCmd)
	{
		char recvbuf[2048];
		int iResult;
		int recvbuflen = sizeof(recvbuf);

		iResult = recv(CommandSocket, recvbuf, recvbuflen - 1, 0);
		if(iResult > 0)
		{
			recvbuf[iResult] = 0;
			Core_0ConsoleExecCmd("%s", recvbuf);
		}
		else if(iResult == 0)
		{
			//printf("Connection closed\n");
			return;
		}
		//else
			//printf("recv failed with error: %d\n", WSAGetLastError());
	}
}


bool CommandConnect();
void CommandDisconnect();

int hOut;
FILE * fOut = NULL;

bool ConsoleConnect()
{

	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "Console initialized...\n";
	char recvbuf[2048];
	int iResult;
	int recvbuflen = sizeof(recvbuf);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return(false);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", CONSOLE_PORT, &hints, &result);
	if(iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return(false);
	}

	// Attempt to connect to an address until one succeeds
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		ConnectSocket = WSASocket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol, NULL, 0, 0);
		if(ConnectSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return(false);
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if(iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if(ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to console!\n");
		WSACleanup();
		return(false);
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if(iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		WSACleanup();
		return(false);
	}

	//connected? reopen streams

	FreeConsole();

	hOut = _open_osfhandle(ConnectSocket, O_RDONLY | O_RDWR | O_WRONLY | _O_APPEND);
	fOut = ::fdopen(hOut, "a+");
	::setvbuf(fOut, NULL, _IONBF, 0);

	*stdout = *fOut;
	*stderr = *fOut;

	g_bRunning = true;
	//_beginthread(ConsoleRecv, 0, 0);

	CommandConnect();

	return(true);
}
void ConsoleDisconnect()
{

	g_bRunning = false; 

	
	if(fOut)
	{
	fclose(fOut);
	}
	/*int iResult = shutdown(ConnectSocket, SD_SEND);
	if(iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		goto end;
	}*/
//end:
	//closesocket(ConnectSocket);
	WSACleanup();
	//Sleep(1000);
	CommandDisconnect();
}

bool CommandConnect()
{
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char recvbuf[2048];
	int iResult;
	int recvbuflen = sizeof(recvbuf);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iResult != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return(false);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", COMMAND_PORT, &hints, &result);
	if(iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return(false);
	}

	// Attempt to connect to an address until one succeeds
	for(ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		CommandSocket = WSASocket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol, NULL, 0, 0);
		if(CommandSocket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return(false);
		}

		// Connect to server.
		iResult = connect(CommandSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if(iResult == SOCKET_ERROR)
		{
			closesocket(CommandSocket);
			CommandSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if(CommandSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to console!\n");
		WSACleanup();
		return(false);
	}

	g_bRunningCmd = true;

	_beginthread(ConsoleRecv, 0, 0);

	return(true);
}
void CommandDisconnect()
{
	g_bRunningCmd = false;

	int iResult = shutdown(CommandSocket, SD_BOTH);
	if(iResult == SOCKET_ERROR)
	{
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(CommandSocket);
		WSACleanup();
		return;
	}
	// cleanup
	closesocket(CommandSocket);
	//WSACleanup();
}

