
#pragma once

#define CORE_VERSION 1

#include <core\\sxcore.h>

#include <core\\Array.h>
#include <core\sxloaderconfig_class.cpp>

#include <core\\sxfile.cpp>

#include <core\Task.cpp>
#include <core\TaskManager.cpp>

ISXFile* Core_CrFile()
{
	return new SXFile();
}

ISXFile* Core_OpFile(const char* path, int type)
{
	 SXFile* tmpsxfile = new SXFile();
	 tmpsxfile->Open(path,type);
	 return tmpsxfile;
}


ISXLConfig*  Core_CrLConfig()
{
	return new SXLoaderConfig();
}

ISXLConfig*  Core_OpLConfig(const char* path)
{
	 SXLoaderConfig* tmplconfig = new SXLoaderConfig();
	 tmplconfig->Open(path);
	 return tmplconfig;
}

#pragma comment(lib, "winmm.lib")

char CoreName[CORE_NAME_MAX_LEN];
//CoreName[1] = 0;
bool IsCoreInit = false;

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif
SXTaskManager* TaskManager = 0;

//функции обертки
long Core_0GetVersion()
{
	return CORE_VERSION;
}

void Core_Dbg_Set(report_func rf)
{
	reportf = rf;
}

int Core_0FileExists(const char* fname)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind = ::FindFirstFile(fname, &wfd);
		if (INVALID_HANDLE_VALUE != hFind)
		{
			::FindClose(hFind);
			return 1;
		}
	return 0;
}

char** Core_0CommandLineToArgvA(char* CmdLine,int* _argc)
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = strlen(CmdLine);
	i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,i + (len+2)*sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv)+i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

		while( a = CmdLine[i] )
		{
				if(in_QM) 
				{
						if(a == '\"') 
						{
							in_QM = FALSE;
						} 
						else
						{
							_argv[j] = a;
							j++;
						}
				} 
				else
				{
						switch(a) 
						{
							case '\"':
								in_QM = TRUE;
								in_TEXT = TRUE;
									if(in_SPACE)
									{
										argv[argc] = _argv+j;
										argc++;
									}
								in_SPACE = FALSE;
								break;
							case ' ':
							case '\t':
							case '\n':
							case '\r':
									if(in_TEXT)
									{
										_argv[j] = '\0';
										j++;
									}
								in_TEXT = FALSE;
								in_SPACE = TRUE;
								break;
							default:
								in_TEXT = TRUE;
									if(in_SPACE)
									{
										argv[argc] = _argv+j;
										argc++;
									}
								_argv[j] = a;
								j++;
								in_SPACE = FALSE;
								break;
						}
				}
			i++;
		}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}

//---------------------------------

int Core_0ClipBoardCopy(const char *str)
{
    HGLOBAL hglb;
    char *s;
    int len = strlen(str) + 1;
 
		if(!(hglb = GlobalAlloc(GHND, len)))
			return 0;

		if(!(s = (char *)GlobalLock(hglb)))
			return 0;

	strcpy(s, str);
	GlobalUnlock(hglb);

		if(!OpenClipboard(NULL) || !EmptyClipboard()) 
		{
			GlobalFree(hglb);
			return 0;
		}
	SetClipboardData(CF_TEXT, hglb);
	CloseClipboard();
	return 1;
}

void Core_0Create(const char* name, bool is_unic)
{
		if(name && strlen(name) > 1)
		{
				if(is_unic)
				{
					HANDLE hMutex = CreateMutex(NULL, FALSE, name);
						if(GetLastError() == ERROR_ALREADY_EXISTS)
						{
							CloseHandle(hMutex);
							reportf(-1, "%s - none unic name, sgcore", gen_msg_location);
						}
						else
						{
							strcpy(CoreName,name);
							IsCoreInit = true;
							TaskManager = new SXTaskManager();
						}
				}
				else
				{
					strcpy(CoreName, name);
					IsCoreInit = true;
					TaskManager = new SXTaskManager();
				}
		}
		else
			reportf(-1, "%s - not init argument [name], sgcore", gen_msg_location);
}

void Core_AKill()
{
	if (!IsCoreInit)
		reportf(-1, "%s - score is not init", gen_msg_location);

	if (TaskManager)
	{
		mem_delete(TaskManager);
		IsCoreInit = false;
	}
}

void Core_AGetName(char* name)
{
	if (!IsCoreInit)
		reportf(-1, "%s - score is not init", gen_msg_location);

		if(name)
		{
			strcpy(name, CoreName);
		}
		else
			reportf(-1, "%s - invalid argument", gen_msg_location);
}

////

void Core_MTaskAdd(THREAD_UPDATE_FUNCTION func, DWORD flag)
{
	if (!TaskManager)
		reportf(-1, "%s - score is not init", gen_msg_location);

	TaskManager->add(func, flag);
}

void Core_MTaskStart()
{
	if (!TaskManager)
		reportf(-1, "%s - score is not init", gen_msg_location);

	TaskManager->start();
}

void Core_MTaskStop()
{
	if (!TaskManager)
		reportf(-1, "%s - score is not init", gen_msg_location);

	TaskManager->stop();
}