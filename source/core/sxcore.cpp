
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define CORE_VERSION 1

#include"sxcore.h"

#include "Config.h"

#include "File.h"

#include "Task.h"
#include "TaskManager.h"

#include "concmd.h"
#include "cvars.h"

#include "Time.h"

//##########################################################################

char g_szCoreName[CORE_NAME_MAX_LEN];

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

//**************************************************************************

CTaskManager* g_pTaskManager = 0;

#define SXCORE_PRECOND(retval) if(!g_pTaskManager){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxcore is not init", GEN_MSG_LOCATION); return retval;}

//**************************************************************************

//РЕГИСТРЫ
bool g_aGRegistersBool[CORE_REGISTRY_SIZE];
int32_t g_aGRegistersInt[CORE_REGISTRY_SIZE];
float32_t g_aGRegistersFloat[CORE_REGISTRY_SIZE];
float4x4 g_aGRegistersMatrix[CORE_REGISTRY_SIZE];
float3 g_aGRegistersFloat3[CORE_REGISTRY_SIZE];
String g_aGRegistersString[CORE_REGISTRY_SIZE];

#define CORE_REGUSTRY_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < CORE_REGISTRY_SIZE))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s: %s - unresolved index '%d' of access for registry", SX_LIB_NAME, GEN_MSG_LOCATION, id); return stdval; }

//**************************************************************************

CTimeManager* g_pTimers = 0;
#define CORE_TIME_PRECOND(retval) if(!g_pTimers){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxcore is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

//функции обертки
long Core_0GetVersion()
{
	return CORE_VERSION;
}

void Core_Dbg_Set(report_func fnReportFunc)
{
	g_fnReportf = fnReportFunc;
}

bool Core_0FileExists(const char *szNameFunc)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind = ::FindFirstFile(szNameFunc, &wfd);
		if (INVALID_HANDLE_VALUE != hFind)
		{
			::FindClose(hFind);
			return true;
		}
	return false;
}

SX_LIB_API UINT Core_0GetTimeLastModify(const char *szPath)
{
	HANDLE hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	
	if (hFile == INVALID_HANDLE_VALUE)
		return 0;

	SYSTEMTIME stUTC, stLocal;
	FILETIME ftCreate, ftAccess, ftWrite;
	GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
	FileTimeToSystemTime(&ftWrite, &stUTC);

	tm tmObj;
	ZeroMemory(&tmObj, sizeof(tm));
	tmObj.tm_year = stUTC.wYear - 1900;
	tmObj.tm_mon = stUTC.wMonth;
	tmObj.tm_mday = stUTC.wDay;
	tmObj.tm_hour = stUTC.wHour;
	tmObj.tm_min = stUTC.wMinute;
	tmObj.tm_sec = stUTC.wSecond;

	uint32_t tLastModify = mktime(&tmObj);

	return tLastModify;
}

bool Core_0ClipBoardCopy(const char *szStr)
{
    HGLOBAL hglb;
    char *s;
	int len = strlen(szStr) + 1;
 
	if(!(hglb = GlobalAlloc(GHND, len)))
		return false;

	if(!(s = (char *)GlobalLock(hglb)))
		return false;

	strcpy(s, szStr);
	GlobalUnlock(hglb);

	if(!OpenClipboard(NULL) || !EmptyClipboard()) 
	{
		GlobalFree(hglb);
		return false;
	}
	SetClipboardData(CF_TEXT, hglb);
	CloseClipboard();
	return true;
}

bool Core_0IsProcessRun(const char* process)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe);

	while (1) {
		if (stricmp(pe.szExeFile, process) == 0) return true;
		if (!Process32Next(hSnapshot, &pe)) return false;
	}
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
							LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
							return;
						}
				}
			strcpy(g_szCoreName, name);
			ConsoleConnect();
			ConsoleRegisterCmds();
			g_pTaskManager = new CTaskManager();
			g_pTimers = new CTimeManager();

			//LibReport(REPORT_MSG_LEVEL_NOTICE, "is init\n");
		}
		else
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

void Core_AKill()
{
	SXCORE_PRECOND(_VOID);

	mem_delete(g_pTaskManager);
	mem_delete(g_pTimers);
	ConsoleDisconnect();
}

void Core_AGetName(char* name)
{
	SXCORE_PRECOND(_VOID);

	if(name)
		strcpy(name, g_szCoreName);
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - invalid argument", GEN_MSG_LOCATION);
}

//##########################################################################

IFile* Core_CrFile()
{
	return new CFile();
}

IFile* Core_OpFile(const char *szPath, int iType)
{
	CFile* pFile = new CFile();
	pFile->open(szPath, iType);
	return pFile;
}


ISXConfig*  Core_CrConfig()
{
	return new CConfig();
}

ISXConfig*  Core_OpConfig(const char* path)
{
	CConfig* pConfig = new CConfig();
	pConfig->open(path);
	return pConfig;
}

//##########################################################################

void Core_MTaskAdd(THREAD_UPDATE_FUNCTION func, DWORD flag)
{
	SXCORE_PRECOND(_VOID);
	g_pTaskManager->add(func, flag);
}

void Core_MTaskStart()
{
	SXCORE_PRECOND(_VOID);
	g_pTaskManager->start();
}

void Core_MTaskStop()
{
	SXCORE_PRECOND(_VOID);
	g_pTaskManager->stop();
}

//##########################################################################

void Core_RBoolSet(int id, bool val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	g_aGRegistersBool[id] = val;
}

bool Core_RBoolGet(int id)
{
	CORE_REGUSTRY_PRE_COND_ID(id, 0);
	return g_aGRegistersBool[id];
}

void Core_RIntSet(int id, int32_t val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	g_aGRegistersInt[id] = val;
}

int32_t Core_RIntGet(int id)
{
	CORE_REGUSTRY_PRE_COND_ID(id,0);
	return g_aGRegistersInt[id];
}

void Core_RFloatSet(int id, float32_t val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	g_aGRegistersFloat[id] = val;
}

float32_t Core_RFloatGet(int id)
{
	CORE_REGUSTRY_PRE_COND_ID(id,0);
	return g_aGRegistersFloat[id];
}

void Core_RMatrixSet(int id, float4x4* val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	if (val)
		g_aGRegistersMatrix[id] = *val;
	else
		g_aGRegistersMatrix[id] = SMMatrixIdentity();
}

void Core_RMatrixGet(int id, float4x4* val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);

	if (val)
		memcpy(val, &g_aGRegistersMatrix[id], sizeof(float4x4));
}


void Core_RFloat3Set(int id, float3* val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	if (val)
		g_aGRegistersFloat3[id] = *val;
}

void Core_RFloat3Get(int id, float3* val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);

	if (val)
		memcpy(val, &g_aGRegistersFloat3[id], sizeof(float3));
}

void Core_RStringSet(int id, const char *val)
{
	CORE_REGUSTRY_PRE_COND_ID(id, _VOID);
	if (val)
		g_aGRegistersString[id] = val;
}

const char* Core_RStringGet(int id)
{
	CORE_REGUSTRY_PRE_COND_ID(id, 0);
	return g_aGRegistersString[id].c_str();
}

//##########################################################################

ID Core_TimeAdd()
{
	CORE_TIME_PRECOND(-1);

	return g_pTimers->timeAdd();
}

void Core_TimesUpdate()
{
	CORE_TIME_PRECOND(_VOID);

	g_pTimers->update();
}


void Core_TimeSpeedSet(ID id, float speed)
{
	CORE_TIME_PRECOND(_VOID);

	g_pTimers->timeSpeedSet(id, speed);
}

float Core_TimeSpeedGet(ID id)
{
	CORE_TIME_PRECOND(0);

	return g_pTimers->timeSpeedGet(id);
}


void Core_TimeWorkingSet(ID id, bool working)
{
	CORE_TIME_PRECOND(_VOID);

	g_pTimers->timeWorkingSet(id, working);
}

bool Core_TimeWorkingGet(ID id)
{
	CORE_TIME_PRECOND(false);

	return g_pTimers->timeWorkingGet(id);
}


void Core_TimeUnixStartSet(ID id, int64_t start_time)
{
	CORE_TIME_PRECOND(_VOID);

	g_pTimers->timeUnixStartSet(id, start_time);
}

int64_t Core_TimeUnixStartGet(ID id)
{
	CORE_TIME_PRECOND(0);

	return g_pTimers->timeUnixStartGet(id);
}

int64_t Core_TimeUnixCurrGet(ID id)
{
	CORE_TIME_PRECOND(0);

	return g_pTimers->timeUnixCurrGet(id);
}


int64_t Core_TimeTotalMcsGet(ID id)
{
	CORE_TIME_PRECOND(0);

	return g_pTimers->timeTotalMcsGet(id);
}

int64_t Core_TimeTotalMcsGetU(ID id)
{
	CORE_TIME_PRECOND(0);

	return g_pTimers->timeTotalMcsGetU(id);
}