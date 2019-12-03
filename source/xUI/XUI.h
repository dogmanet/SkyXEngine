#ifndef __XUI_H 
#define __XUI_H

#include "IXUI.h"
#include <graphix/graphix.h>
#include <gui/guimain.h>

class CUIWindow;
class CXUI: public IXUI
{
public:
	CXUI(IGXDevice *pDev, IXWindowSystem *pWindowSystem, gui::IGUI *pGUI);

	IUIWindow* XMETHODCALLTYPE createWindow(const XWINDOW_DESC *pWindowDesc, IUIWindow *pParent = NULL) override;

	void onDestroyWindow(CUIWindow *pWindow);

	IXWindowSystem* getWindowSystem();
	gui::IGUI* getGUI();
	IGXDevice* getGXDevice();

	void XMETHODCALLTYPE render() override;
	void XMETHODCALLTYPE present() override;

private:
	Array<CUIWindow*> m_pWindows;

	IGXDevice *m_pDev = nullptr;
    IXWindowSystem *m_pWindowSystem = nullptr;
    gui::IGUI *m_pGUI = nullptr;
};

#endif
