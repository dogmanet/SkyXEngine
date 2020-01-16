#ifndef __UIWINDOW_H 
#define __UIWINDOW_H

#include "IUIWindow.h"
#include <gui/guimain.h>

class CUIWindow;
class CWindowCallback: public IXWindowCallback
{
public:
	CWindowCallback(CUIWindow *pUIWindow, gui::IDesktopStack *pDesktopStack):
		m_pUIWindow(pUIWindow),
		m_pDesktopStack(pDesktopStack)
	{
	}

	INT_PTR XMETHODCALLTYPE onMessage(UINT msg, WPARAM wParam, LPARAM lParam, IXWindow *pWindow) override;
protected:
	CUIWindow *m_pUIWindow = NULL;
	gui::IDesktopStack *m_pDesktopStack = NULL;
	bool m_isScreenSizeChanged = false;
	bool m_isResizing = false;
	UINT m_uNewWidth = 0;
	UINT m_uNewHeight = 0;
};

//##########################################################################

class CXUI;
class CUIWindow: public IXUnknownImplementation<IUIWindow>
{
public:
	friend class CWindowCallback;

	CUIWindow(CXUI *pXUI, const XWINDOW_DESC *pWindowDesc, IUIWindow *pParent = NULL);
	~CUIWindow();

	void XMETHODCALLTYPE hide() override;

	void XMETHODCALLTYPE close() override;

	void XMETHODCALLTYPE show() override;

	bool XMETHODCALLTYPE isVisible() override;

	void XMETHODCALLTYPE setTitle(const char *szTitle) override;

	void XMETHODCALLTYPE update(const XWINDOW_DESC *pWindowDesc) override;

	const XWINDOW_DESC* XMETHODCALLTYPE getDesc() override;

	void XMETHODCALLTYPE addControl(IUIControl *pControl) override;

	IXWindow* getXWindow();
	void render(IGXContext *pContext);
	void present();

private:
	void releaseSwapChain();
	void createSwapChain(UINT uWidth, UINT uHeight);

	CXUI *m_pXUI = NULL;
	IXWindow *m_pXWindow = NULL;
	Array<IUIControl*> m_ChildControls;
	gui::IDesktopStack *m_pDesktopStack = NULL;
	gui::IDesktop *m_pDefaultDesktop = NULL;
	IGXSwapChain *m_pGuiSwapChain = NULL;
	IGXDepthStencilSurface *m_pGuiDepthStencilSurface = NULL;
	CWindowCallback *m_pXWindowCallback = NULL;
};

#endif
