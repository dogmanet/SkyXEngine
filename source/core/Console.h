#ifndef __CONSOLE_H
#define __CONSOLE_H

#include <xcommon/IXConsole.h>

class CConsole final: public IXUnknownImplementation<IXConsole>
{
public:
	void XMETHODCALLTYPE registerCommand(const char *szName, XCONCMD cmd, const char *szDesc) override;
	void XMETHODCALLTYPE registerCommand(const char *szName, XCONCMDARG cmd, const char *szDesc) override;
	void XMETHODCALLTYPE registerCommand(const char *szName, IXConsoleCommand *pCommand, const char *szDesc) override;
	void XMETHODCALLTYPE removeCommand(const char *szName) override;

	void XMETHODCALLTYPE registerCVar(const char *szName, const char *szValue, const char *szDesc, int flags = 0) override;
	void XMETHODCALLTYPE registerCVar(const char *szName, int iValue, const char *szDesc, int flags = 0) override;
	void XMETHODCALLTYPE registerCVar(const char *szName, float fValue, const char *szDesc, int flags = 0) override;
	void XMETHODCALLTYPE registerCVar(const char *szName, bool bValue, const char *szDesc, int flags = 0) override;

	void XMETHODCALLTYPE execCommand(const char *szCommand) override;
	void execCommand2(const char *szFormat, ...) override;

	const char** XMETHODCALLTYPE getPCVarString(const char *szName) override;
	const int* XMETHODCALLTYPE getPCVarInt(const char *szName) override;
	const float* XMETHODCALLTYPE getPCVarFloat(const char *szName) override;
	const bool* XMETHODCALLTYPE getPCVarBool(const char *szName) override;
};

#endif
