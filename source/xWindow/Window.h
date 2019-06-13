#ifndef __WINDOW_H
#define __WINDOW_H

#include "IXWindowSystem.h"

class CWindow: public IXWindow
{
public:
	CWindow(HINSTANCE hInst, UINT uId, const XWINDOW_DESC *pWindowDesc, IXWindowCallback *pCallback = NULL, IXWindow *pParent = NULL);
	~CWindow();

	XWINDOW_OS_HANDLE XMETHODCALLTYPE getOSHandle() override;

	void XMETHODCALLTYPE hide() override;
	void XMETHODCALLTYPE close() override;
	void XMETHODCALLTYPE show() override;
	bool XMETHODCALLTYPE isVisible() override;
	void XMETHODCALLTYPE setTitle(const char *szTitle) override;
	void XMETHODCALLTYPE update(const XWINDOW_DESC *pWindowDesc) override;

	INT_PTR XMETHODCALLTYPE runDefaultCallback(UINT msg, WPARAM wParam, LPARAM lParam) override;

	const XWINDOW_DESC* XMETHODCALLTYPE getDesc() override;
	
	INT_PTR runCallback(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd = NULL;
	HINSTANCE m_hInst;
	XWINDOW_DESC m_windowDesc;
	UINT m_uId;
	IXWindowCallback *m_pCallback;
};

#endif
