#ifndef __IXDESKTOPSTACK_H
#define __IXDESKTOPSTACK_H

#include <gdefines.h>
#include "IXDesktop.h"

class IXDesktopStack: public IXUnknown
{
public:
	/** Обработка ввода
	Вызывается из цикла обработки сообщений окна
	*/
	virtual BOOL XMETHODCALLTYPE putMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	/**	Отрисовка
	Должно быть вызвано только в контексте потока рендера
	*/
	virtual void XMETHODCALLTYPE render() = 0;
		
	//Создание Desktop'а
	virtual bool XMETHODCALLTYPE newDesktop(const char *szName, const char *szFile, IXDesktop **ppOut) = 0;

	//Установка активного Desktop'а
	virtual void XMETHODCALLTYPE setActiveDesktop(IXDesktop *pDesktop, bool clearStack = true) = 0;
	virtual void XMETHODCALLTYPE setActiveDesktop(const char *name) = 0;

	//Получить текущий активный Desktop
	virtual void XMETHODCALLTYPE getActiveDesktop(IXDesktop **ppOut) = 0;

	//Получить Desktop по имени
	virtual void XMETHODCALLTYPE getDesktop(const char *szName, IXDesktop **ppOut) = 0;

	virtual void XMETHODCALLTYPE messageBox(const char *szTitle, const char *szText, ...) = 0;
	virtual void XMETHODCALLTYPE pushDesktop(IXDesktop *pDesktop) = 0;
	virtual void XMETHODCALLTYPE popDesktop(IXDesktop **ppOut = NULL) = 0;
};

#endif
