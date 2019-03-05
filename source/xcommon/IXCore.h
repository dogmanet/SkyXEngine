#ifndef __IXCORE_H
#define __IXCORE_H

#include <gdefines.h>
#include "IFileSystem.h"
#include "IAsyncFileReader.h"
#include "IAsyncTaskRunner.h"
#include "XEvents.h"

class IPluginManager;

class IXCore: public IXUnknown
{
public:
	virtual IPluginManager *getPluginManager() = 0;
	virtual IFileSystem *getFileSystem() = 0;

	virtual IAsyncFileReader *getAsyncFileReader() = 0;
	virtual IAsyncTaskRunner *getAsyncTaskRunner() = 0;

	template<typename T> IEventChannel<T> *getEventChannel(const XGUID &guid)
	{
		return((IEventChannel<T>*)getEventChannelInternal(guid));
	}

protected:
	virtual IBaseEventChannel *getEventChannelInternal(const XGUID &guid) = 0;
};

#endif
