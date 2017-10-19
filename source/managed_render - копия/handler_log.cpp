
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#include "handler_log.h"

FILE * FileOutLog = 0;	//!< поток ведения лога

BOOL CALLBACK HE_EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == lParam && IsWindow(hwnd))
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return TRUE;
}

void HE_HandlerError(const char* format, ...)
{
	va_list va;
	char buf[4096];
	va_start(va, format);
	vsprintf_s(buf, 4096, format, va);
	va_end(va);

	DWORD PID = GetCurrentProcessId();
	EnumWindows(HE_EnumWindowsProc, PID);

	MessageBox(0, buf, "SkyXEngine error", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL);
	exit(0);
}

void InitOutLog()
{
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	freopen("CONOUT$", "wt", stderr);
	freopen("CONIN$", "rt", stdin);

	char path[256];
	char PathForExe[1024];
	GetModuleFileName(NULL, path, 256);
	int len = strlen(path);
		while (path[len--] != '\\')
		{
				if (path[len - 1] == '\\')
				{
					len--;
					memcpy(PathForExe, path, len);
					PathForExe[len] = 0;
				}
		}
	char FileLogPath[1024];
	sprintf(FileLogPath, "%s%s", PathForExe, "\\log.txt");
	FileOutLog = fopen(FileLogPath, "w");

		if (!FileOutLog)
		{
			HE_HandlerError("Debug system is not init, this is big problem!");
			exit(0);
		}
}

void PrintfLog(int level, const char* format, ...)
{
	va_list va;
	char buf[REPORT_MSG_MAX_LEN];
	va_start(va, format);
	vsprintf_s(buf, REPORT_MSG_MAX_LEN, format, va);
	va_end(va);

		if(FileOutLog)
		{
			if (level == REPORT_MSG_LEVEL_ERROR)
			{
				printf(COLOR_LRED "! ");
				fwrite("! ", 1, 2, FileOutLog);
			}
			else if (level == REPORT_MSG_LEVEL_WARNING)
			{
				printf(COLOR_YELLOW "* ");
				fwrite("* ", 1, 2, FileOutLog);
			}

			printf(buf);
			if(level == REPORT_MSG_LEVEL_ERROR || level == REPORT_MSG_LEVEL_WARNING)
			{
				printf(COLOR_RESET);
			}
			fwrite(buf, 1, strlen(buf), FileOutLog);
			//fprintf(FileOutLog, "\n");
			fflush(FileOutLog);

			if (level == REPORT_MSG_LEVEL_ERROR)
			{
				HE_HandlerError(buf);
			}
		}
}
