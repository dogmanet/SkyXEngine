#ifndef __CORE_H
#define __CORE_H

#include "sxcore.h"
#include <xcommon/IXCore.h>
#include "PluginManager.h"

class CCore: public IXCore
{
public:
	CCore();
	~CCore();

	void Release();

	IPluginManager *getPluginManager();
	IFileSystem *getFileSystem();

	IAsyncFileReader *getAsyncFileReader();
	IAsyncTaskRunner *getAsyncTaskRunner();

	void getRenderPipeline(IXRenderPipeline **ppRenderPipeline);
	void setRenderPipeline(IXRenderPipeline *pRenderPipeline);

	void loadPlugins();

protected:
	IBaseEventChannel *getEventChannelInternal(const XGUID &guid);

	CPluginManager *m_pPluginManager = NULL;
	AssotiativeArray<XGUID, IBaseEventChannel*> m_mEventChannels;

	IXRenderPipeline *m_pRenderPipeline = NULL;
};

#endif
