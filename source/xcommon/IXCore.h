#ifndef __IXCORE_H
#define __IXCORE_H

#include <gdefines.h>
#include "IFileSystem.h"
#include "IAsyncFileReader.h"
#include "IAsyncTaskRunner.h"
#include "XEvents.h"
#include "IXRenderPipeline.h"

#include <fcntl.h>
#include <io.h>

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

	virtual UINT_PTR XMETHODCALLTYPE getCrtOutputHandler() = 0;

	virtual void XMETHODCALLTYPE execCmd(const char *szCommand) = 0;
	virtual void execCmd2(const char * szFormat, ...) = 0;



	//@FIXME: Remove that!
	virtual void initUpdatable() = 0;
	virtual void runUpdate() = 0;

	template<typename T> IEventChannel<T> *getEventChannel(const XGUID &guid)
	{
		return((IEventChannel<T>*)getEventChannelInternal(guid));
	}
protected:
	virtual IBaseEventChannel *getEventChannelInternal(const XGUID &guid) = 0;
};

/*! Устанавливает поток вывода. Для работы консоли
\warning Должна быть инлайнова, чтобы выполняться в контексте вызывающей библиотеки
*/
__inline void Core_SetOutPtr(IXCore *pCore)
{
	UINT_PTR sock = pCore->getCrtOutputHandler();
	if(sock == ~0)
	{
		return;
	}
	int hOut = _open_osfhandle(sock, O_RDONLY | O_RDWR | O_WRONLY | _O_APPEND);
	FILE * fOut = ::_fdopen(hOut, "a+");
	::setvbuf(fOut, NULL, _IONBF, 0);

	*stdout = *fOut;
	*stderr = *fOut;

	fOut->_file = 1;
}

#define INIT_OUTPUT_STREAM(icore) Core_SetOutPtr(icore)

#endif
