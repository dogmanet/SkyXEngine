#ifndef __IXCORE_H
#define __IXCORE_H

#include <gdefines.h>
#include "IFileSystem.h"
#include "IAsyncFileReader.h"
#include "IAsyncTaskRunner.h"
#include "XEvents.h"
#include "IXRenderPipeline.h"
#include "IXConfig.h"

#include <fcntl.h>
#include <io.h>

class IPluginManager;
class IXResourceManager;

class IXCore: public IXUnknown
{
public:
	virtual IPluginManager* XMETHODCALLTYPE getPluginManager() = 0;
	virtual IFileSystem* XMETHODCALLTYPE getFileSystem() = 0;
	virtual IXResourceManager* XMETHODCALLTYPE getResourceManager() = 0;

	virtual IAsyncFileReader* XMETHODCALLTYPE getAsyncFileReader() = 0;
	virtual IAsyncTaskRunner* XMETHODCALLTYPE getAsyncTaskRunner() = 0;

	virtual void XMETHODCALLTYPE getRenderPipeline(IXRenderPipeline **ppRenderPipeline) = 0;
	virtual void XMETHODCALLTYPE setRenderPipeline(IXRenderPipeline *pRenderPipeline) = 0;

	virtual UINT_PTR XMETHODCALLTYPE getCrtOutputHandler() = 0;

	virtual void XMETHODCALLTYPE execCmd(const char *szCommand) = 0;
	virtual void execCmd2(const char *szFormat, ...) = 0;

	//@FIXME: Remove that!
	virtual void initUpdatable() = 0;
	virtual void runUpdate() = 0;

	virtual ID XMETHODCALLTYPE getThreadId() = 0;
	virtual bool XMETHODCALLTYPE isOnMainThread() = 0;

	virtual IXConfig* XMETHODCALLTYPE newConfig() = 0;

	virtual const char** XMETHODCALLTYPE getPCVarString(const char *szName) = 0;
	virtual const int* XMETHODCALLTYPE getPCVarInt(const char *szName) = 0;
	virtual const float* XMETHODCALLTYPE getPCVarFloat(const char *szName) = 0;
	virtual const bool* XMETHODCALLTYPE getPCVarBool(const char *szName) = 0;

	template<typename T> IEventChannel<T> *getEventChannel(const XGUID &guid)
	{
		return((IEventChannel<T>*)getEventChannelInternal(guid));
	}
protected:
	virtual IBaseEventChannel *getEventChannelInternal(const XGUID &guid) = 0;
};

class CCoreOutPtr
{
	friend void Core_SetOutPtr(IXCore *pCore);
	CCoreOutPtr(IXCore *pCore)
	{
		UINT_PTR sock = pCore->getCrtOutputHandler();
		if(sock == ~0)
		{
			return;
		}
		int hOut = _open_osfhandle(sock, O_RDONLY | O_RDWR | O_WRONLY | _O_APPEND);
		m_fOut = ::_fdopen(_dup(hOut), "a+");
		::setvbuf(m_fOut, NULL, _IONBF, 0);

#ifdef _MSC_VER
		if(_fileno(stdout) < 0)
		{
			char szPipename[255];
			HANDLE hPipe = NULL;
			{
				sprintf(szPipename, "\\\\.\\pipe\\SkyXEngineConsoleStdout-%u-%u", GetCurrentProcessId(), GetCurrentThreadId());
				hPipe = CreateNamedPipe(szPipename, PIPE_ACCESS_DUPLEX, PIPE_NOWAIT | PIPE_READMODE_BYTE, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);
				freopen(szPipename, "w", stdout);
				_dup2(_fileno(m_fOut), _fileno(stdout));
				CloseHandle(hPipe);
			}

			{
				sprintf(szPipename, "\\\\.\\pipe\\SkyXEngineConsoleStderr-%u-%u", GetCurrentProcessId(), GetCurrentThreadId());
				hPipe = CreateNamedPipe(szPipename, PIPE_ACCESS_DUPLEX, PIPE_NOWAIT | PIPE_READMODE_BYTE, PIPE_UNLIMITED_INSTANCES, 0, 0, 0, NULL);
				freopen(szPipename, "w", stderr);
				_dup2(_fileno(m_fOut), _fileno(stderr));
				CloseHandle(hPipe);
			}
		}
		else
		{
#endif
			_dup2(_fileno(m_fOut), _fileno(stdout));
			_dup2(_fileno(m_fOut), _fileno(stderr));
#ifdef _MSC_VER
		}
#endif
		::setvbuf(stdout, NULL, _IONBF, 0);
		::setvbuf(stderr, NULL, _IONBF, 0);
	}
	~CCoreOutPtr()
	{
		fclose(m_fOut);
	}

	FILE *m_fOut;
};

/*! Устанавливает поток вывода. Для работы консоли
\warning Должна быть инлайнова, чтобы выполняться в контексте вызывающей библиотеки
*/
__inline void Core_SetOutPtr(IXCore *pCore)
{
	static CCoreOutPtr s_optr(pCore);
}

#define INIT_OUTPUT_STREAM(icore) Core_SetOutPtr(icore)

#endif
