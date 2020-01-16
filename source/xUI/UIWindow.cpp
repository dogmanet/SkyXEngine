#include "UIWindow.h"
#include "XUI.h"

INT_PTR XMETHODCALLTYPE CWindowCallback::onMessage(UINT msg, WPARAM wParam, LPARAM lParam, IXWindow *pWindow)
{
	if(!m_pDesktopStack->putMessage(msg, wParam, lParam))
	{
		return(TRUE);
	}

	switch(msg)
	{
	case WM_SIZE:
		m_uNewWidth = LOWORD(lParam);
		m_uNewHeight = HIWORD(lParam);
		m_isScreenSizeChanged = true;
		if(!m_isResizing)
		{
			m_pUIWindow->releaseSwapChain();
			m_pUIWindow->createSwapChain(m_uNewWidth, m_uNewHeight);
		}
		break;

	case WM_ENTERSIZEMOVE:
		m_isResizing = true;
		m_isScreenSizeChanged = false;
		break;

	case WM_EXITSIZEMOVE:
		m_isResizing = false;
		if(m_isScreenSizeChanged)
		{
			m_isScreenSizeChanged = false;
			m_pUIWindow->releaseSwapChain();
			m_pUIWindow->createSwapChain(m_uNewWidth, m_uNewHeight);
		}
		break;

	default:
		return(pWindow->runDefaultCallback(msg, wParam, lParam));
	}

	return(0);
}

//##########################################################################

CUIWindow::CUIWindow(CXUI *pXUI, const XWINDOW_DESC *pWindowDesc, IUIWindow *pParent):
	m_pXUI(pXUI)
{
	IXWindow *pXParent = NULL;
	if(pParent)
	{
		pXParent = ((CUIWindow*)pParent)->getXWindow();
	}
	m_pDesktopStack = pXUI->getGUI()->newDesktopStack("editor_gui/", (UINT)pWindowDesc->iSizeX, (UINT)pWindowDesc->iSizeY);
	m_pDefaultDesktop = m_pDesktopStack->createDesktopW(L"default", L"main.html");
	m_pDesktopStack->setActiveDesktop(m_pDefaultDesktop);

	m_pXWindowCallback = new CWindowCallback(this, m_pDesktopStack);
	m_pXWindow = pXUI->getWindowSystem()->createWindow(pWindowDesc, m_pXWindowCallback, pXParent);

	createSwapChain((UINT)pWindowDesc->iSizeX, (UINT)pWindowDesc->iSizeY);
}
CUIWindow::~CUIWindow()
{
	releaseSwapChain();
	mem_release(m_pXWindow);
	mem_delete(m_pXWindowCallback);
	mem_release(m_pDefaultDesktop);
	mem_release(m_pDesktopStack);

	m_pXUI->onDestroyWindow(this);
}


void CUIWindow::releaseSwapChain()
{
	mem_release(m_pGuiDepthStencilSurface);
	mem_release(m_pGuiSwapChain);
}
void CUIWindow::createSwapChain(UINT uWidth, UINT uHeight)
{
	if(!m_pXWindow)
	{
		return;
	}
	m_pGuiSwapChain = m_pXUI->getGXDevice()->createSwapChain(uWidth, uHeight, m_pXWindow->getOSHandle());
	m_pGuiDepthStencilSurface = m_pXUI->getGXDevice()->createDepthStencilSurface(uWidth, uHeight, GXFMT_D24S8, GXMULTISAMPLE_NONE);
}

void XMETHODCALLTYPE CUIWindow::hide()
{
	m_pXWindow->hide();
}

void XMETHODCALLTYPE CUIWindow::close()
{
	m_pXWindow->close();
}

void XMETHODCALLTYPE CUIWindow::show()
{
	m_pXWindow->show();
}

bool XMETHODCALLTYPE CUIWindow::isVisible()
{
	return(m_pXWindow->isVisible());
}

void XMETHODCALLTYPE CUIWindow::setTitle(const char *szTitle)
{
	m_pXWindow->setTitle(szTitle);
}

void XMETHODCALLTYPE CUIWindow::update(const XWINDOW_DESC *pWindowDesc)
{
	m_pXWindow->update(pWindowDesc);
}

const XWINDOW_DESC* XMETHODCALLTYPE CUIWindow::getDesc()
{
	return(m_pXWindow->getDesc());
}

void XMETHODCALLTYPE CUIWindow::addControl(IUIControl *pControl)
{
	m_ChildControls.push_back(pControl);
}

IXWindow* CUIWindow::getXWindow()
{
	return(m_pXWindow);
}

void CUIWindow::render(IGXContext *pContext)
{
	IGXSurface *pTarget = m_pGuiSwapChain->getColorTarget();
	pContext->setColorTarget(pTarget);
	mem_release(pTarget);

	pContext->setDepthStencilSurface(m_pGuiDepthStencilSurface);

	m_pDesktopStack->render();
}

void CUIWindow::present()
{
	m_pGuiSwapChain->swapBuffers();
}
