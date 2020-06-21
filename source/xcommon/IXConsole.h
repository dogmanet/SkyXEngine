#ifndef __IXCONSOLE_H
#define __IXCONSOLE_H

#include <gdefines.h>

typedef void(*XCONCMD)(); /*!< Тип функции для регистрации команды без аргументов */
typedef void(*XCONCMDARG)(int argc, const char **argv); /*!< Тип функции для регистрации команды с аргументами */

//! Флаги кваров
enum CVAR_FLAG
{
	FCVAR_NONE = 0x00, //!< нет
	FCVAR_CHEAT = 0x01, //!< Изменение этой переменной с дефолтного значения разрешено только в режиме разработки
	FCVAR_READONLY = 0x02,  //!< Только для чтения
	FCVAR_NOTIFY_OLD = 0x04, //!< Оповещать об изменениях
	FCVAR_NOTIFY = 0x08  //!< Оповещать об изменениях
};

//##########################################################################

class IXConsoleCommand
{
public:
	virtual void XMETHODCALLTYPE execute(int argc, const char **argv) = 0;
};

//##########################################################################

class IXConsole: public IXUnknown
{
public:
	//! Регистрация консольной функции без аргументов
	virtual void XMETHODCALLTYPE registerCommand(const char *szName, XCONCMD cmd, const char *szDesc) = 0;
	//! Регистрация консольной функции с аргументами
	virtual void XMETHODCALLTYPE registerCommand(const char *szName, XCONCMDARG cmd, const char *szDesc) = 0;
	//! Регистрация консольной функции с аргументами
	virtual void XMETHODCALLTYPE registerCommand(const char *szName, IXConsoleCommand *pCommand, const char *szDesc) = 0;

	virtual void XMETHODCALLTYPE removeCommand(const char *szName) = 0;

	//! Регистрирует строковую переменную
	virtual void XMETHODCALLTYPE registerCVar(const char *szName, const char *szValue, const char *szDesc, int flags = 0) = 0;
	//! Регистрирует целочисленную переменную
	virtual void XMETHODCALLTYPE registerCVar(const char *szName, int iValue, const char *szDesc, int flags = 0) = 0;
	//! Регистрирует вещественную переменную
	virtual void XMETHODCALLTYPE registerCVar(const char *szName, float fValue, const char *szDesc, int flags = 0) = 0;
	//! Регистрирует логическую переменную
	virtual void XMETHODCALLTYPE registerCVar(const char *szName, bool bValue, const char *szDesc, int flags = 0) = 0;

	virtual void XMETHODCALLTYPE execCommand(const char *szCommand) = 0;
	virtual void execCommand2(const char *szFormat, ...) = 0;

	virtual const char** XMETHODCALLTYPE getPCVarString(const char *szName) = 0;
	virtual const int* XMETHODCALLTYPE getPCVarInt(const char *szName) = 0;
	virtual const float* XMETHODCALLTYPE getPCVarFloat(const char *szName) = 0;
	virtual const bool* XMETHODCALLTYPE getPCVarBool(const char *szName) = 0;
};

#endif
