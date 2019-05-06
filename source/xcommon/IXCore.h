#ifndef __IXCORE_H
#define __IXCORE_H

#include <gdefines.h>
#include "IFileSystem.h"
#include "IAsyncFileReader.h"
#include "IAsyncTaskRunner.h"
#include "XEvents.h"
#include "IXRenderPipeline.h"

class IPluginManager;

class IXCore: public IXUnknown
{
public:
	virtual IPluginManager *getPluginManager() = 0;
	virtual IFileSystem *getFileSystem() = 0;

	virtual IAsyncFileReader *getAsyncFileReader() = 0;
	virtual IAsyncTaskRunner *getAsyncTaskRunner() = 0;

	virtual void getRenderPipeline(IXRenderPipeline **ppRenderPipeline) = 0;
	virtual void setRenderPipeline(IXRenderPipeline *pRenderPipeline) = 0;

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
