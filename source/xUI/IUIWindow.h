#ifndef __IUIWINDOW_H 
#define __IUIWINDOW_H

#include <xWindow/IXWindowSystem.h>
#include "IUIControl.h"
#include <gui/guimain.h>

class IUIWindow: public IXUnknown
{
public:

	//! Скрывает окно (остается в таскбаре)
	virtual void XMETHODCALLTYPE hide() = 0;

	//! Закрывает окно (исчезает из таскбара)
	virtual void XMETHODCALLTYPE close() = 0;

	//! Открывает окно
	virtual void XMETHODCALLTYPE show() = 0;

	//! Видимо ли окно (открыто и не свернуто)
	virtual bool XMETHODCALLTYPE isVisible() = 0;

	//! 
	virtual void XMETHODCALLTYPE setTitle(const char *szTitle) = 0;

	//! 
	virtual void XMETHODCALLTYPE update(const XWINDOW_DESC *pWindowDesc) = 0;

	//! 
	virtual const XWINDOW_DESC* XMETHODCALLTYPE getDesc() = 0;

	//!
	virtual void XMETHODCALLTYPE addControl(IUIControl *pControl) = 0;
};

#endif
