#include "Console.h"
#include "concmd.h"

#include <common/ConcurrentQueue.h>

void XMETHODCALLTYPE CConsole::registerCommand(const char *szName, XCONCMD cmd, const char *szDesc)
{
	Core_0RegisterConcmd(szName, cmd, szDesc);
}
void XMETHODCALLTYPE CConsole::registerCommand(const char *szName, XCONCMDARG cmd, const char *szDesc)
{
	Core_0RegisterConcmdArg(szName, cmd, szDesc);
}
void XMETHODCALLTYPE CConsole::registerCommand(const char *szName, IXConsoleCommand *pCommand, const char *szDesc)
{
	Core_0RegisterConcmdClsArg(szName, pCommand, (SXCONCMDCLSARG)&IXConsoleCommand::execute, szDesc);
}

void XMETHODCALLTYPE CConsole::removeCommand(const char *szName)
{
	//! @todo implement me!
}

void XMETHODCALLTYPE CConsole::registerCVar(const char *szName, const char *szValue, const char *szDesc, int flags)
{
	Core_0RegisterCVarString(szName, szValue, szDesc, flags);
}
void XMETHODCALLTYPE CConsole::registerCVar(const char *szName, int iValue, const char *szDesc, int flags)
{
	Core_0RegisterCVarInt(szName, iValue, szDesc, flags);
}
void XMETHODCALLTYPE CConsole::registerCVar(const char *szName, float fValue, const char *szDesc, int flags)
{
	Core_0RegisterCVarFloat(szName, fValue, szDesc, flags);
}
void XMETHODCALLTYPE CConsole::registerCVar(const char *szName, bool bValue, const char *szDesc, int flags)
{
	Core_0RegisterCVarBool(szName, bValue, szDesc, flags);
}

//! @FIXME Remove that!
extern std::mutex g_conUpdMtx;
extern CConcurrentQueue<char*> g_vCommandBuffer;
void XMETHODCALLTYPE CConsole::execCommand(const char *szCommand)
{
	execCommand2("%s", szCommand);
}
void CConsole::execCommand2(const char *szFormat, ...)
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

const char** XMETHODCALLTYPE CConsole::getPCVarString(const char *szName)
{
	return(GET_PCVAR_STRING(szName));
}
const int* XMETHODCALLTYPE CConsole::getPCVarInt(const char *szName)
{
	return(GET_PCVAR_INT(szName));
}
const float* XMETHODCALLTYPE CConsole::getPCVarFloat(const char *szName)
{
	return(GET_PCVAR_FLOAT(szName));
}
const bool* XMETHODCALLTYPE CConsole::getPCVarBool(const char *szName)
{
	return(GET_PCVAR_BOOL(szName));
}
