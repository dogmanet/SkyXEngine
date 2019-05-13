#ifndef __CORE_H
#define __CORE_H

#include "sxcore.h"
#include <xcommon/IXCore.h>
#include "PluginManager.h"
#include "FileSystem.h"
#include <xcommon/IXUpdatable.h>

class CCore: public IXCore
{
public:
	CCore();
	~CCore();

	void XMETHODCALLTYPE Release() override;

	IPluginManager *getPluginManager() override;
	IFileSystem *getFileSystem() override;

	IAsyncFileReader *getAsyncFileReader() override;
	IAsyncTaskRunner *getAsyncTaskRunner() override;

	void getRenderPipeline(IXRenderPipeline **ppRenderPipeline) override;
	void setRenderPipeline(IXRenderPipeline *pRenderPipeline) override;

	void loadPlugins();

	void initUpdatable() override;
	void shutdownUpdatable();
	void runUpdate() override;

protected:
	IBaseEventChannel *getEventChannelInternal(const XGUID &guid) override;

	CPluginManager *m_pPluginManager = NULL;
	CFileSystem *m_pFileSystem = NULL;
	AssotiativeArray<XGUID, IBaseEventChannel*> m_mEventChannels;

	IXRenderPipeline *m_pRenderPipeline = NULL;

	struct _update_sys
	{
		IXUpdatable *pUpdatable;
		UINT uPriority;
	};

	Array<_update_sys> m_aUpdatables;
};

#endif
