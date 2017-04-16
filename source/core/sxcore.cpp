
#pragma once

#define CORE_VERSION 1

#include <core\\sxcore.h>

#include <common\\Array.h>
#include <core\loaderconfig.cpp>

#include <core\\file.cpp>

#include <core\Task.cpp>
#include <core\TaskManager.cpp>

#pragma comment(lib, "winmm.lib")

char CoreName[CORE_NAME_MAX_LEN];

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif
SXTaskManager* TaskManager = 0;

#define SXCORE_PRECOND(retval) if(!TaskManager){reportf(-1, "%s - sxcore is not init", gen_msg_location); return retval;}

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

char** Core_0CommandLineToArgvA(char* CmdLine, int* _argc)
{
	char** argv;
	char* _argv;
	UINT len;
	UINT argc;
	char a;
	UINT i, j;

	bool is_qm = false;
	bool is_text = false;
	bool is_space = true;

	len = strlen(CmdLine);
	i = ((len + 2) / 2)*sizeof(void*)+sizeof(void*);

	argv = (char**)GlobalAlloc(GMEM_FIXED, i + (len + 2)*sizeof(char));

	_argv = (char*)(((unsigned char*)argv) + i);

	argc = 0;
	argv[argc] = _argv;
	i = 0;
	j = 0;

	while (a = CmdLine[i])
	{
		if (is_qm)
		{
			if (a == '\"')
			{
				is_qm = false;
			}
			else
			{
				_argv[j] = a;
				j++;
			}
		}
		else
		{
			switch (a)
			{
			case '\"':
				is_qm = true;
				is_text = true;
				if (is_space)
				{
					argv[argc] = _argv + j;
					argc++;
				}
				is_space = false;
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if (is_text)
				{
					_argv[j] = '\0';
					j++;
				}
				is_text = false;
				is_space = true;
				break;
			default:
				is_text = true;
				if (is_space)
				{
					argv[argc] = _argv + j;
					argc++;
				}
				_argv[j] = a;
				j++;
				is_space = false;
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
							TaskManager = new SXTaskManager();
						}
				}
				else
				{
					strcpy(CoreName, name);
					TaskManager = new SXTaskManager();
				}
		}
		else
			reportf(-1, "%s - not init argument [name], sgcore", gen_msg_location);
}

void Core_AKill()
{
	SXCORE_PRECOND(_VOID);

	mem_delete(TaskManager);
}

void Core_AGetName(char* name)
{
	SXCORE_PRECOND(_VOID);

	if(name)
		strcpy(name, CoreName);
	else
		reportf(-1, "%s - invalid argument", gen_msg_location);
}

////

ISXFile* Core_CrFile()
{
	return new SXFile();
}

ISXFile* Core_OpFile(const char* path, int type)
{
	SXFile* tmpsxfile = new SXFile();
	tmpsxfile->Open(path, type);
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

////

void Core_MTaskAdd(THREAD_UPDATE_FUNCTION func, DWORD flag)
{
	SXCORE_PRECOND(_VOID);
	TaskManager->add(func, flag);
}

void Core_MTaskStart()
{
	SXCORE_PRECOND(_VOID);
	TaskManager->start();
}

void Core_MTaskStop()
{
	SXCORE_PRECOND(_VOID);
	TaskManager->stop();
}

//////////////

int32_t GRegistersInt[CORE_REGISTRY_SIZE];
float32_t GRegistersFloat[CORE_REGISTRY_SIZE];
float4x4 GRegistersMatrix[CORE_REGISTRY_SIZE];

#define CORE_REGUSTRY_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < CORE_REGISTRY_SIZE))\
{reportf(REPORT_MSG_LEVEL_ERROR, "[CORE] %s - unresolved index '%d' of access for registry", gen_msg_location, id); return stdval; }

void Core_RIntSet(int id, int32_t val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	GRegistersInt[id] = val;
}

int32_t Core_RIntGet(int id)
{
	CORE_REGUSTRY_PRE_COND_ID(id,0);
	return GRegistersInt[id];
}

void Core_RFloatSet(int id, float32_t val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	GRegistersFloat[id] = val;
}

float32_t Core_RFloatGet(int id)
{
	CORE_REGUSTRY_PRE_COND_ID(id,0);
	return GRegistersFloat[id];
}

void Core_RMatrixSet(int id, float4x4* val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	if (val)
		GRegistersMatrix[id] = *val;
	else
		GRegistersMatrix[id] = SMMatrixIdentity();
}

void Core_RMatrixGet(int id, float4x4* val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);

	if (val)
		memcpy(val, &GRegistersMatrix[id], sizeof(float4x4));
}