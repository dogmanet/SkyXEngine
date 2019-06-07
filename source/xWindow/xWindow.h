#ifndef __XWINDOW_H
#define __XWINDOW_H

#include <gdefines.h>

enum XWINDOW_FLAG
{
	XWF_DEFAULT = 0x0000,
	XWF_NOBORDER = 0x0001,
	XWF_BUTTON_CLOSE = 0x0002,
	XWF_BUTTON_MINMAX = 0x0004,
	XWF_TRANSPARENT = 0x0008,
	XWF_NORESIZE = 0x0010
};
DEFINE_ENUM_FLAG_OPERATORS(XWINDOW_FLAG);

#define XCW_USEDEFAULT ((int)0x80000000)

struct XWINDOW_DESC
{
	UINT uSizeX;
	UINT uSizeY;
	UINT uPosX = XCW_USEDEFAULT;
	UINT uPosY = XCW_USEDEFAULT;
	XWINDOW_FLAG flags = XWF_DEFAULT;
	const char *szTitle;
};

//#############################################################################

class IXWindow: public IXUnknown
{
public:
	//! 
	virtual XWINDOW_OS_HANDLE* XMETHODCALLTYPE getOSHandle() = 0;

	//! Скрывает окно (остается в таскбаре)
	virtual void XMETHODCALLTYPE hide() = 0;

	//! Закрывает окно (исчезает из таскбара)
	virtual void XMETHODCALLTYPE close() = 0;

	//! Открывает окно
	virtual void XMETHODCALLTYPE show() = 0;

	//! Видимо ли окно (открыто и не свернуто)
	virtual void XMETHODCALLTYPE isVisible() = 0;

	//! 
	virtual void XMETHODCALLTYPE setTitle(const char *szTitle) = 0;

	//! 
	virtual void XMETHODCALLTYPE update(const XWINDOW_DESC *pWindowDesc) = 0;

	//! 
	virtual INT_PTR XMETHODCALLTYPE runDefaultCallback(UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

class IXWindowCallback
{
public:
	virtual INT_PTR XMETHODCALLTYPE onMessage(UINT msg, WPARAM wParam, LPARAM lParam, IXWindow *pWindow) = 0;
};

class IXWindowSystem: public IXUnknown
{
	//! 
	virtual IXWindow* XMETHODCALLTYPE createWindow(const XWINDOW_DESC *pWindowDesc, IXWindowCallback *pCallback = NULL, IXWindow *pParent = NULL) = 0;

	//! 
	virtual bool XMETHODCALLTYPE processMessages(UINT uMaxMessages = 0) = 0;
};

#endif
