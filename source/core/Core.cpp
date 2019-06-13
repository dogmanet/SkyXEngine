#include "Core.h"
#include <common/file_utils.h>
#include "AsyncFileReader.h"
#include "FileSystem.h"
#include "ModelProvider.h"

#include "concmd.h"
#include "TaskManager.h"

#include "PerfMon.h"
#include "time.h"

#include <GRegisterIndex.h>

extern CTimeManager *g_pTimers;
extern CPerfMon *g_pPerfMon;
extern CCore *g_pCore;
extern CTaskManager *g_pTaskManager;

CCore::CCore(const char *szName)
{
	ConsoleConnect(szName);
	ConsoleRegisterCmds();

	Core_0RegisterCVarBool("g_time_run", true, "Запущено ли игрвоое время?", FCVAR_NOTIFY);
	Core_0RegisterCVarFloat("g_time_speed", 1.f, "Скорость/соотношение течения игрового времени", FCVAR_NOTIFY);
	Core_0RegisterCVarBool("dbg_config_save", false, "Отладочный вывод процесса сохранения конфига");
	Core_0RegisterCVarInt("r_stats", 1, "Показывать ли статистику? 0 - нет, 1 - fps и игровое время, 2 - показать полностью");

	Core_0RegisterConcmd("on_g_time_run_change", []()
	{
		static const bool * g_time_run = GET_PCVAR_BOOL("g_time_run");
		Core_TimeWorkingSet(Core_RIntGet(G_RI_INT_TIMER_GAME), *g_time_run);
	});
	Core_0RegisterConcmd("on_g_time_speed_change", []()
	{
		static const float * g_time_speed = GET_PCVAR_FLOAT("g_time_speed");
		Core_TimeSpeedSet(Core_RIntGet(G_RI_INT_TIMER_GAME), *g_time_speed);
	});

	m_pPluginManager = new CPluginManager();

	m_pFileSystem = new CFileSystem();
	m_pFileSystem->addRoot("gamesource");

	m_pModelProvider = new CModelProvider(this);
	m_pPluginManager->registerInterface(IXMODELPROVIDER_GUID, m_pModelProvider);
	
	g_pPerfMon = m_pPerfMon = new CPerfMon();
	g_pTimers = m_pTimers = new CTimeManager();

	ID idTimerRender = Core_TimeAdd();
	ID idTimerGame = Core_TimeAdd();
	Core_RIntSet(G_RI_INT_TIMER_RENDER, idTimerRender);
	Core_RIntSet(G_RI_INT_TIMER_GAME, idTimerGame);

	tm ct = {0, 0, 10, 27, 5, 2030 - 1900, 0, 0, 0};
	Core_TimeUnixStartSet(idTimerGame, mktime(&ct));

	Core_TimeWorkingSet(idTimerRender, true);
	Core_TimeWorkingSet(idTimerGame, true);

	Core_TimeSpeedSet(idTimerGame, 10);

	int iThreadNum = 0;
	if(!sscanf(Core_0GetCommandLineArg("threads", "0"), "%d", &iThreadNum) || iThreadNum < 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Invalid -threads value! Defaulting to 0\n");
	}

	g_pTaskManager = m_pTaskManager = new CTaskManager(iThreadNum);
	if(strcasecmp(Core_0GetCommandLineArg("no-threads", "no"), "no"))
	{
		m_pTaskManager->forceSinglethreaded();
	}

	loadPlugins();
}
CCore::~CCore()
{
	shutdownUpdatable();

	mem_delete(m_pTaskManager);

	mem_delete(m_pTimers);
	mem_delete(m_pPerfMon);

	mem_delete(m_pModelProvider);
	mem_delete(m_pResourceManager);
	mem_delete(m_pFileSystem);
	mem_delete(m_pPluginManager);
	for(AssotiativeArray<XGUID, IBaseEventChannel*>::Iterator i = m_mEventChannels.begin(); i; i++)
	{
		mem_delete(*i.second);
	}

	ConsoleDisconnect();
}

IPluginManager *CCore::getPluginManager()
{
	return(m_pPluginManager);
}
IFileSystem *CCore::getFileSystem()
{
	return(m_pFileSystem);
}
IXResourceManager *CCore::getResourceManager()
{
	return(m_pResourceManager);
}

IAsyncFileReader *CCore::getAsyncFileReader()
{
	return(new CAsyncFileReader(this));
}
IAsyncTaskRunner *CCore::getAsyncTaskRunner()
{
	return(NULL);
}

IBaseEventChannel *CCore::getEventChannelInternal(const XGUID &guid)
{
	const AssotiativeArray<XGUID, IBaseEventChannel*>::Node *pNode;
	if(m_mEventChannels.KeyExists(guid, &pNode))
	{
		return(*pNode->Val);
	}
	//@HACK
	IBaseEventChannel *pOut = new IEventChannel<int>();
	m_mEventChannels[guid] = pOut;
	return(pOut);
}

void CCore::loadPlugins()
{
#ifdef WIN64
	auto list = FileGetListFiles("../bin64/plugins/*.dll");
#else
	auto list = FileGetListFiles("../bin/plugins/*.dll");
#endif
		
	IXPlugin *pPlugin = NULL;
	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		printf("Loading plugin '%s'... ", list[i].c_str());
		pPlugin = m_pPluginManager->loadPlugin((String("../bin"
#ifdef WIN64
			"64"
#endif
			"/plugins/") + list[i]).c_str());
		if(pPlugin)
		{
			printf(COLOR_GREEN "DONE!" COLOR_RESET "\n", list[i].c_str());
		}
		else
		{
			printf(COLOR_LRED "ERROR!" COLOR_RESET "\n", list[i].c_str());
		}
	}
	m_pPluginManager->invokeStartup(this);

	m_pResourceManager = new CResourceManager(this);
}

void CCore::getRenderPipeline(IXRenderPipeline **ppRenderPipeline)
{
	if(m_pRenderPipeline)
	{
		m_pRenderPipeline->AddRef();
	}

	*ppRenderPipeline = m_pRenderPipeline;
}

void CCore::setRenderPipeline(IXRenderPipeline *pRenderPipeline)
{
	mem_release(m_pRenderPipeline);
	if(pRenderPipeline)
	{
		pRenderPipeline->AddRef();
	}
	m_pRenderPipeline = pRenderPipeline;
}

void CCore::initUpdatable()
{
	IXCore *pCore = Core_GetIXCore();
	IPluginManager *pPluginManager = pCore->getPluginManager();

	IXUpdatable *pUpdatable;
	UINT ic = 0;
	while((pUpdatable = (IXUpdatable*)pPluginManager->getInterface(IXUPDATABLE_GUID, ic++)))
	{
		// if(pUpdatable->getVersion() == IXRENDERABLE_VERSION)
		{
			_update_sys rs;
			rs.pUpdatable = pUpdatable;
			rs.uPriority = pUpdatable->startup();

			m_aUpdatables.push_back(rs);
		}
	}

	m_aUpdatables.quickSort([](const _update_sys &a, const _update_sys &b)
	{
		return(a.uPriority < b.uPriority);
	});
}

void CCore::runUpdate()
{
	static Array<ID> s_aIdToWait;
	ID idTask;
	//@FIXME: Change to actual value!
	float fDeltaTime = 0.016f;
	for(UINT i = 0, l = m_aUpdatables.size(); i < l; ++i)
	{
		idTask = m_aUpdatables[i].pUpdatable->run(fDeltaTime);
		if(ID_VALID(idTask))
		{
			s_aIdToWait.push_back(idTask);
		}
	}

	for(UINT i = 0, l = s_aIdToWait.size(); i < l; ++i)
	{
		Core_MWaitFor(s_aIdToWait[i]);
	}

	s_aIdToWait.clearFast();

	for(UINT i = 0, l = m_aUpdatables.size(); i < l; ++i)
	{
		m_aUpdatables[i].pUpdatable->sync();
	}
}

void CCore::shutdownUpdatable()
{
	for(UINT i = 0, l = m_aUpdatables.size(); i < l; ++i)
	{
		m_aUpdatables[i].pUpdatable->shutdown();
	}
}

UINT_PTR XMETHODCALLTYPE CCore::getCrtOutputHandler()
{
	return(Core_ConsoleGetOutHandler());
}

//! @FIXME Remove that!
extern std::mutex g_conUpdMtx;
extern CConcurrentQueue<char*> g_vCommandBuffer;
void XMETHODCALLTYPE CCore::execCmd(const char *szCommand)
{
	execCmd2("%s", szCommand);
}
void CCore::execCmd2(const char * szFormat, ...)
{
	va_list va;
	va_start(va, szFormat);
	size_t len = _vscprintf(szFormat, va) + 1;
	char * buf, *cbuf = NULL;
	if(len < 4096)
	{
		buf = (char*)alloca(len * sizeof(char));
	}
	else
	{
		cbuf = buf = new char[len];
	}
	vsprintf(buf, szFormat, va);
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

//##########################################################################

C SXCORE_API IXCore* XCoreInit(const char *szName)
{
	return(g_pCore = new CCore(szName));
}

C SXCORE_API void XCoreStart()
{
	g_pTaskManager->start();
}
C SXCORE_API void XCoreStop()
{
	g_pTaskManager->stop();
}
C SXCORE_API void XCoreAddTask(ITask *pTask)
{
	g_pTaskManager->addTask(pTask);
}
