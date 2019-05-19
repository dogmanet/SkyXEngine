#ifndef __IXCORE_H
#define __IXCORE_H

#include <gdefines.h>
#include "IFileSystem.h"
#include "IAsyncFileReader.h"
#include "IAsyncTaskRunner.h"
#include "XEvents.h"
#include "IXRenderPipeline.h"

class IPluginManager;
class IXResourceManager;

class IXCore: public IXUnknown
{
public:
	virtual IPluginManager * XMETHODCALLTYPE getPluginManager() = 0;
	virtual IFileSystem * XMETHODCALLTYPE getFileSystem() = 0;
	virtual IXResourceManager * XMETHODCALLTYPE getResourceManager() = 0;

	virtual IAsyncFileReader * XMETHODCALLTYPE getAsyncFileReader() = 0;
	virtual IAsyncTaskRunner * XMETHODCALLTYPE getAsyncTaskRunner() = 0;

	virtual void XMETHODCALLTYPE getRenderPipeline(IXRenderPipeline **ppRenderPipeline) = 0;
	virtual void XMETHODCALLTYPE setRenderPipeline(IXRenderPipeline *pRenderPipeline) = 0;

	template<typename T> IEventChannel<T> *getEventChannel(const XGUID &guid)
	{
		return((IEventChannel<T>*)getEventChannelInternal(guid));
	}

	//@FIXME: Remove that!
	virtual void initUpdatable() = 0;
	virtual void runUpdate() = 0;
protected:
	virtual IBaseEventChannel *getEventChannelInternal(const XGUID &guid) = 0;
};

#endif
