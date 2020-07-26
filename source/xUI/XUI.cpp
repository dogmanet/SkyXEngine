#include "XUI.h"
#include "UIWindow.h"

CXUI::CXUI(IGXDevice *pDev, IXWindowSystem *pWindowSystem, gui::IGUI *pGUI):
	m_pDev(pDev),
	m_pWindowSystem(pWindowSystem),
	m_pGUI(pGUI)
{
}

IUIWindow* XMETHODCALLTYPE CXUI::createWindow(const XWINDOW_DESC *pWindowDesc, IUIWindow *pParent)
{
	CUIWindow *pWindow = new CUIWindow(this, pWindowDesc, pParent);
	m_pWindows.push_back(pWindow);
	return(pWindow);
}

IUIButton* XMETHODCALLTYPE CXUI::createButton(IUIWindow *pParent)
{
	CUIButton *pButton = new CUIButton(++m_elemendID);
	pParent->addControl(pButton);
	return pButton;
}

IUITextBox* XMETHODCALLTYPE CXUI::createTextBox(IUIWindow *pParent)
{
	CUITextBox *pInput = new CUITextBox(++m_elemendID);
	pParent->addControl(pInput);
	return pInput;
}

IUICheckbox* XMETHODCALLTYPE CXUI::createCheckBox(IUIWindow *pParent)
{
	IUICheckbox *pCheckBox = new CUICheckBox(++m_elemendID);
	pParent->addControl(pCheckBox);
	return pCheckBox;
}

void CXUI::onDestroyWindow(CUIWindow *pWindow)
{
	for(UINT i = 0, l = m_pWindows.size(); i < l; ++i)
	{
		if(m_pWindows[i] == pWindow)
		{
			m_pWindows.erase(i);
			break;
		}
	}
}

IXWindowSystem* CXUI::getWindowSystem()
{
	return(m_pWindowSystem);
}
gui::IGUI* CXUI::getGUI()
{
	return(m_pGUI);
}
IGXDevice* CXUI::getGXDevice()
{
	return(m_pDev);
}

void XMETHODCALLTYPE CXUI::render()
{
	IGXContext *pCtx = m_pDev->getThreadContext();

	IGXSurface *pOldSurface = pCtx->getColorTarget();
	IGXDepthStencilSurface *pOldDS = pCtx->getDepthStencilSurface();

	pCtx->unsetDepthStencilSurface();

	for(UINT i = 0, l = m_pWindows.size(); i < l; ++i)
	{
		if(m_pWindows[i]->isVisible())
		{
			m_pWindows[i]->render(pCtx);
		}
	}

	pCtx->setColorTarget(pOldSurface);
	pCtx->setDepthStencilSurface(pOldDS);
	mem_release(pOldSurface);
	mem_release(pOldDS);
}
void XMETHODCALLTYPE CXUI::present()
{
	for(UINT i = 0, l = m_pWindows.size(); i < l; ++i)
	{
		if(m_pWindows[i]->isVisible())
		{
			m_pWindows[i]->present();
		}
	}
}

//##########################################################################

EXTERN_C __declspec(dllexport) IXUI* InitInstance(IGXDevice *pDev, IXWindowSystem *pWindowSystem, gui::IGUI *pGUI)
{
	return(new CXUI(pDev, pWindowSystem, pGUI));
}
