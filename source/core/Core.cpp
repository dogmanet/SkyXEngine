#include "Core.h"
#include <common/file_utils.h>
#include "AsyncFileReader.h"
#include "FileSystem.h"
#include "ModelProvider.h"

CCore::CCore()
{
	m_pPluginManager = new CPluginManager();

	m_pFileSystem = new CFileSystem();
	m_pFileSystem->addRoot("gamesource");

	m_pResourceManager = new CResourceManager(this);

	m_pModelProvider = new CModelProvider(this);
	m_pPluginManager->registerInterface(IXMODELPROVIDER_GUID, m_pModelProvider);

}
CCore::~CCore()
{
	shutdownUpdatable();

	mem_delete(m_pModelProvider);
	mem_delete(m_pResourceManager);
	mem_delete(m_pFileSystem);
	mem_delete(m_pPluginManager);
	for(AssotiativeArray<XGUID, IBaseEventChannel*>::Iterator i = m_mEventChannels.begin(); i; i++)
	{
		mem_delete(*i.second);
	}
}

void CCore::Release()
{
	--m_uRefCount;
	assert(m_uRefCount && "You MUST NOT explicitly call Release on IXCore interface");
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
	float fDeltaTime = 0.16f;
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
