#ifndef __CORE_H
#define __CORE_H

#include "sxcore.h"
#include <xcommon/IXCore.h>
#include "PluginManager.h"
#include "FileSystem.h"
#include <xcommon/IXUpdatable.h>
#include "ResourceManager.h"
#include "Console.h"

class CModelProvider;
class CPerfMon;
class CTimeManager;
class CTaskManager;

class CCore final: public IXUnknownImplementation<IXCore>
{
public:
	CCore(const char *szName);
	~CCore();

	IPluginManager* XMETHODCALLTYPE getPluginManager() override;
	IFileSystem* XMETHODCALLTYPE getFileSystem() override;
	IXResourceManager* XMETHODCALLTYPE getResourceManager() override;

	IAsyncFileReader* XMETHODCALLTYPE newAsyncFileReader() override;
	IAsyncTaskRunner* XMETHODCALLTYPE newAsyncTaskRunner() override;

	void XMETHODCALLTYPE getRenderPipeline(IXRenderPipeline **ppRenderPipeline) override;
	void XMETHODCALLTYPE setRenderPipeline(IXRenderPipeline *pRenderPipeline) override;

	void loadPlugins();

	void initUpdatable() override;
	void shutdownUpdatable();
	void runUpdate() override;

	UINT_PTR XMETHODCALLTYPE getCrtOutputHandler() override; 

	IXConsole* XMETHODCALLTYPE getConsole() override;
	//void XMETHODCALLTYPE execCmd(const char *szCommand) override;
	//void execCmd2(const char *szFormat, ...) override;

	ID XMETHODCALLTYPE getThreadId() override; 
	bool XMETHODCALLTYPE isOnMainThread() override;
	
	IXConfig* XMETHODCALLTYPE newConfig() override;
	IXBuffer* XMETHODCALLTYPE newBuffer() override;

	//const char** XMETHODCALLTYPE getPCVarString(const char *szName) override;
	//const int* XMETHODCALLTYPE getPCVarInt(const char *szName) override;
	//const float* XMETHODCALLTYPE getPCVarFloat(const char *szName) override;
	//const bool* XMETHODCALLTYPE getPCVarBool(const char *szName) override;

	ID XMETHODCALLTYPE forLoop(int iStart, int iEnd, const IParallelForBody *pBody, int iMaxChunkSize = 0) override;
	void XMETHODCALLTYPE waitForLoop(ID id) override;
	
	IBaseEventChannel* getEventChannelInternal(const XGUID &guid) override;
protected:

	CPluginManager *m_pPluginManager = NULL;
	CFileSystem *m_pFileSystem = NULL;
	CResourceManager *m_pResourceManager = NULL;
	CModelProvider *m_pModelProvider = NULL;
	AssotiativeArray<XGUID, IBaseEventChannel*> m_mEventChannels;

	IXRenderPipeline *m_pRenderPipeline = NULL;

	struct _update_sys
	{
		IXUpdatable *pUpdatable;
		UINT uPriority;
	};

	Array<_update_sys> m_aUpdatables;


	CPerfMon *m_pPerfMon = NULL;
	CTimeManager *m_pTimers = NULL;
	CTaskManager *m_pTaskManager = NULL;

	CConsole *m_pConsole = NULL;

	std::chrono::high_resolution_clock::time_point m_tLastUpdateTime;
};

#endif
