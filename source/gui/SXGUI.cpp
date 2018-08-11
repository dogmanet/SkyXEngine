#include "GUIbase.h"
#include "IHTMLparser.h"
#include "IDOMdocument.h"
#include "HTMLels.h"

#include "IHTML.h"

#include "IRenderFrame.h"

#include "Font.h"

#include "IRenderBorder.h"

#include "VideoRenderer.h"

#include "Desktop.h"
#include "IVIDEO.h"
#include "ITEXT.h"

#include <core/sxcore.h>
#include <chrono>
typedef std::chrono::system_clock::time_point time_point;

namespace gui
{
	CPITexture def_w;
	CSHADER def_sh;
	CGUI * g_pGUI;

	CGUI::CGUI(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd):
		m_pDevice(pDev),
		m_szResourceDir(NULL),
		m_iScreenWidth(0),
		m_iScreenHeight(0),
		m_hWnd(hWnd),
		m_pOldDepthStencilSurface(NULL),
		m_pDepthStencilSurface(NULL),
		m_pActiveDesktop(NULL),
		m_bShowCursor(TRUE),
		m_bDeviceLost(false)
	{
		g_pGUI = this;
		updateScreenSize();

		StringW srp = String(szResPath);

		m_szResourceDir = new WCHAR[srp.length() + 1];
		memcpy(m_szResourceDir, srp.c_str(), sizeof(WCHAR)* (srp.length() + 1));

		def_w = CTextureManager::getTexture(L"/dev/white.png");
		def_sh = CTextureManager::loadShader(L"text");
	}

	BOOL CGUI::putMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(!m_pActiveDesktop)
		{
			return(TRUE);
		}
		IEvent ev;
		//const dom::IDOMnodeCollection * els;
#ifndef _UNICODE
		char src[] = {(char)wParam, 0};
		wchar_t dst[] = {0, 0};
#endif

		static int wheelDelta = 0;
		static int wheelCount = 0;
		static POINT mc;
		switch(message)
		{
		case WM_SIZE:
			updateScreenSize();
			break;

		case WM_MOUSEWHEEL:
			wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);
			wheelCount = 0;
			for(; wheelDelta > WHEEL_DELTA; wheelDelta -= WHEEL_DELTA)
			{
				wheelCount++;
			}
			for(; wheelDelta < 0; wheelDelta += WHEEL_DELTA)
			{
				wheelCount--;
				ev.type = GUI_EVENT_TYPE_MOUSEWHEELDOWN;
			}
			if(wheelCount)
			{
				ev.type = wheelCount > 0 ? GUI_EVENT_TYPE_MOUSEWHEELUP : GUI_EVENT_TYPE_MOUSEWHEELDOWN;
				mc.x = lParam & 0xFFFF;
				mc.y = (lParam >> 16) & 0xFFFF;
				ScreenToClient(m_hWnd, &mc);
				ev.clientX = mc.x;
				ev.clientY = mc.y;
				for(; wheelCount != 0; wheelCount += (wheelCount > 0 ? -1 : 1))
				{
					m_pActiveDesktop->dispatchEvent(ev);
				}
			}
			break;

		case WM_SETCURSOR:
			return(FALSE);

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			switch(wParam)
			{
			case KEY_LEFT:
			case KEY_UP:
			case KEY_DOWN:
			case KEY_RIGHT:
			case KEY_HOME:
			case KEY_END:
			case KEY_DELETE:
			case KEY_PGUP:
			case KEY_PGDN:
				ev.type = GUI_EVENT_TYPE_KEYPRESS;
				ev.clientX = ev.clientY = 0;
				ev.key = wParam;
				ev.syskey = true;
				m_pActiveDesktop->dispatchEvent(ev);
				break;
			}
			if(CKeyMap::keyDown(wParam))
			{
				ev.type = GUI_EVENT_TYPE_KEYDOWN;
				ev.clientX = ev.clientY = 0;
				ev.key = wParam;
				m_pActiveDesktop->dispatchEvent(ev);
			}
			if(wParam == KEY_ESCAPE)
			{
				popDesktop();
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			if(CKeyMap::keyUp(wParam))
			{
				ev.type = GUI_EVENT_TYPE_KEYUP;
				ev.clientX = ev.clientY = 0;
				ev.key = wParam;
				m_pActiveDesktop->dispatchEvent(ev);
			}
			break;

		case WM_CHAR:

			ev.type = GUI_EVENT_TYPE_KEYPRESS;
			ev.clientX = ev.clientY = 0;

#ifdef _UNICODE
			ev.key = wParam;
#else
			MultiByteToWideChar(1251, CP_ACP, src, 1, dst, 1);
			ev.key = dst[0];
#endif
			ev.syskey = false;

			m_pActiveDesktop->dispatchEvent(ev);

			break;

		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_LBUTTON;
			CKeyMap::keyDown(KEY_LBUTTON);
			m_pActiveDesktop->dispatchEvent(ev);

			break;

		case WM_LBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_LBUTTON;
			CKeyMap::keyUp(KEY_LBUTTON);
			m_pActiveDesktop->dispatchEvent(ev);
			break;

		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_RBUTTON;
			CKeyMap::keyDown(KEY_RBUTTON);
			m_pActiveDesktop->dispatchEvent(ev); break;

		case WM_RBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_RBUTTON;
			CKeyMap::keyUp(KEY_RBUTTON);
			m_pActiveDesktop->dispatchEvent(ev);
			break;



		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_MBUTTON;
			CKeyMap::keyDown(KEY_MBUTTON);
			m_pActiveDesktop->dispatchEvent(ev); break;

		case WM_MBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_MBUTTON;
			CKeyMap::keyUp(KEY_MBUTTON);
			m_pActiveDesktop->dispatchEvent(ev);
			break;



		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = GET_XBUTTON_WPARAM(wParam);
			CKeyMap::keyDown(GET_XBUTTON_WPARAM(wParam));
			m_pActiveDesktop->dispatchEvent(ev); break;

		case WM_XBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = GET_XBUTTON_WPARAM(wParam);
			CKeyMap::keyUp(GET_XBUTTON_WPARAM(wParam));
			m_pActiveDesktop->dispatchEvent(ev);
			break;

		case WM_MOUSEMOVE:
			ev.type = GUI_EVENT_TYPE_MOUSEMOVE;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			m_pActiveDesktop->dispatchEvent(ev);
			break;
		}
		//OutputDebugStringA("putMessage():return(true);\n");
		return(TRUE);
	}

	void CGUI::update()
	{
		//@TODO: Implement me
	}

	void CGUI::syncronize()
	{
		//@TODO: Implement me
	}

	void CGUI::render()
	{
		if(!m_pActiveDesktop)
		{
			return;
		}

		static time_point tPrev = std::chrono::high_resolution_clock::now();
		time_point tNow = std::chrono::high_resolution_clock::now();
		long long mksdt = std::chrono::duration_cast<std::chrono::microseconds>(tNow - tPrev).count();
		tPrev = tNow;

		float fTimeDelta = (float)mksdt / 1000000.0f;

		//pDevice->GetS

		//		pDevice->GetDepthStencilSurface(&pOldDepthStencilSurface);
		//		pDevice->SetDepthStencilSurface(pDepthStencilSurface);

		//	g_pVid.Update();
		CVideoUpdateManager::update();

		m_pDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);

		//pDevice->SetRenderState(D3DRS_STENCILENABLE, true);

		m_pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		m_pDevice->SetRenderState(D3DRS_STENCILMASK, 0xFF);
		m_pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		//		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		//		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		//		pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_pDevice->BeginScene();
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CTextureManager::bindTexture(def_w);

		SMMATRIX mi = SMMatrixIdentity();
		m_pDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mi));
		m_pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mi));

		m_pDevice->SetVertexShader(NULL);
		m_pDevice->SetPixelShader(NULL);

		SMMATRIX m(
			2.0f / (float)m_iScreenWidth, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / (float)m_iScreenHeight, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			-1.0f, 1.0f, 0.5f, 1.0f);
		m = SMMatrixTranslation(-0.5f, -0.5f, 0.0f) * m;


		////

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_hWnd, &pt);
		if(pt.x < 0)
		{
			pt.x = 0;
		}
		if(pt.x > (int)m_iScreenWidth)
		{
			pt.x = m_iScreenWidth;
		}
		if(pt.y < 0)
		{
			pt.y = 0;
		}
		if(pt.y > (int)m_iScreenHeight)
		{
			pt.y = m_iScreenHeight;
		}
		pt.x -= m_iScreenWidth / 2;
		pt.y -= m_iScreenHeight / 2;

		float fAngleFOV = SMToRadian(90.0f);

		float fParallaxFactor = m_pActiveDesktop->getParallaxFactor();

		m = SMMatrixPerspectiveFovLH(fAngleFOV, (float)m_iScreenWidth / (float)m_iScreenHeight, -1.0f, 1.0f);
		mi = SMMatrixLookAtLH(
			float3(fParallaxFactor * ((float)pt.x / (float)m_iScreenWidth), fParallaxFactor * ((float)pt.y / (float)m_iScreenHeight), 0.0f) +
			float3((float)m_iScreenWidth * 0.5f + 0.5f, (float)m_iScreenHeight * 0.5f + 0.5f, (float)m_iScreenHeight * tanf(fAngleFOV * 0.5f) * 0.5f),

			float3((float)m_iScreenWidth * 0.5f + 0.5f, (float)m_iScreenHeight * 0.5f + 0.5f, 0.0f),
			float3(0.0f, -1.0f, 0.0f)
		);
		m_pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mi));
		////

		m_pDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&m));
		//doc->Render();
		for(UINT i = 0, l = m_mDesktopStack.size(); i < l; ++i)
		{
			if(m_mDesktopStack[i])
			{
				m_mDesktopStack[i]->render(fTimeDelta);
			}
		}
		m_pActiveDesktop->render(fTimeDelta);
#if 0
		struct point
		{
			float x;
			float y;
			float z;
			float tx;
			float ty;
		};


		point a[6] = {
			{TestRect.left, TestRect.top, 0, 0.0f, 1.0f},
			{TestRect.right, TestRect.top, 0, 1.0f, 1.0f},
			{TestRect.left, TestRect.bottom, 0, 0.0f, 0.0f},
			{TestRect.left, TestRect.bottom, 0, 0.0f, 0.0f},
			{TestRect.right, TestRect.top, 0, 1.0f, 1.0f},
			{TestRect.right, TestRect.bottom, 0, 1.0f, 0.0f}
		};




		ITextureManager::BindTexture(w);
		ITextureManager::BindShader(sh);
		pDevice->SetPixelShaderConstantF(0, (float*)&float4_t(1.0f, 1.0f, 1.0f, 0.5f), 1);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
#endif
		//	brd.Render();


		m_pDevice->EndScene();

		//		pDevice->SetDepthStencilSurface(pOldDepthStencilSurface);
	}

	void CGUI::onResetDevice()
	{
		//@TODO: Implement me

		CTextureManager::onResetDevice();

		for(auto i = m_mDesktops.begin(); i; i++)
		{
			((CDesktop*)(*i.second))->createRenderTarget();
			((CDesktop*)(*i.second))->setDirty();
		}

		m_bDeviceLost = false;
	}

	void CGUI::onLostDevice()
	{
		//@TODO: Implement me

		m_bDeviceLost = true;

		CTextureManager::onLostDevice();

		for(auto i = m_mDesktops.begin(); i; i++)
		{
			((CDesktop*)(*i.second))->releaseRenderTarget();
		}
	}

	void CGUI::release()
	{
		//@TODO: Release desired objects
		delete this;
	}

	IDesktop * CGUI::createDesktopA(const char * name, const char * file)
	{
		IDesktop * d = new CDesktop(StringW(String(name)));
		d->setWidth(m_iScreenWidth);
		d->setHeight(m_iScreenHeight);
		d->updateSize();
		d->loadFromFile(StringW(String(file)).c_str());
		m_mDesktops[String(name)] = d;
		return(d);
	}

	IDesktop * CGUI::createDesktopW(const wchar_t * name, const wchar_t * file)
	{
		IDesktop * d = new CDesktop(StringW(name));
		d->setWidth(m_iScreenWidth);
		d->setHeight(m_iScreenHeight);
		d->updateSize();
		d->loadFromFile(file);
		((dom::CDOMdocument*)(d->getDocument()))->reflow();
		m_mDesktops[name] = d;
		return(d);
	}

	void CGUI::setActiveDesktop(IDesktop * d, BOOL clearStack)
	{
		m_pActiveDesktop = d;
		CKeyMap::releaseKeys();
		if(d)
		{
			d->getDocument()->getElementsByTag(L"body")[0][0]->removePseudoclass(css::ICSSrule::PSEUDOCLASS_DISABLED);
		}
		if(clearStack)
		{
			m_mDesktopStack.clear();
		}
	}

	void CGUI::setActiveDesktopA(const char * name)
	{
		setActiveDesktop(getDesktopA(name));
	}
	void CGUI::setActiveDesktopW(const WCHAR * name)
	{
		setActiveDesktop(getDesktopW(name));
	}

	IDesktop * CGUI::getActiveDesktop()
	{
		return(m_pActiveDesktop);
	}

	IDesktop * CGUI::getDesktopA(const char * _name)
	{
		StringW name = String(_name);
		if(m_mDesktops.KeyExists(name))
		{
			return(m_mDesktops[name]);
		}
		return(NULL);
	}
	IDesktop * CGUI::getDesktopW(const wchar_t * name)
	{
		if(m_mDesktops.KeyExists(name))
		{
			return(m_mDesktops[name]);
		}
		return(NULL);
	}

	void CGUI::registerCallback(const char * cbName, GUI_CALLBACK cb)
	{
		if(cb)
		{
			//mCallbacks[StringW(String(cbName))].push_back(cb);
			m_mCallbacks[StringW(String(cbName))] = cb;
		}
	}

	void CGUI::registerCallbackDefault(GUI_CALLBACK_WC cb, BOOL greedy)
	{
		if(cb)
		{
			(greedy ? m_mCallbacksDefaults : m_mCallbacksDefaultsWC).push_back(cb);
		}
	}

	void CGUI::updateScreenSize()
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		m_iScreenHeight = rect.bottom - rect.top;
		m_iScreenWidth = rect.right - rect.left;

		for(auto i = m_mDesktops.begin(); i; i++)
		{
			CDesktop * pDsk = ((CDesktop*)(*i.second));
			pDsk->setWidth(m_iScreenWidth);
			pDsk->setHeight(m_iScreenHeight);
			if(!m_bDeviceLost)
			{
				pDsk->updateSize();
			}
			((dom::CDOMnode*)((dom::CDOMdocument*)pDsk->getDocument())->getRootNode())->updateLayout(true);
		}
	}

	void CGUI::showCursor(BOOL bShow)
	{
		if(m_bShowCursor && !bShow)
		{
			m_bShowCursor = false;
			::ShowCursor(FALSE);
		}
		if(!m_bShowCursor && bShow)
		{
			m_bShowCursor = true;
			::ShowCursor(TRUE);
		}
	}

	WCHAR * CGUI::getResourceDir()
	{
		return(m_szResourceDir);
	}

	IDirect3DDevice9 * CGUI::getDevice()
	{
		return(m_pDevice);
	}

	GUI_CALLBACK CGUI::getCallbackByName(const char * _cbName)
	{
		StringW cbName = String(_cbName);
		if(m_mCallbacks.KeyExists(cbName))
		{
			return(m_mCallbacks[cbName]);
		}
		return(NULL);
	}

	GUI_CALLBACK CGUI::getCallbackByName(const StringW & cbName)
	{
		if(m_mCallbacks.KeyExists(cbName))
		{
			return(m_mCallbacks[cbName]);
		}
		return(NULL);
	}

	UINT CGUI::getScreenWidth()
	{
		return(m_iScreenWidth);
	}
	UINT CGUI::getScreenHeight()
	{
		return(m_iScreenHeight);
	}

	CGUI * GetGUI()
	{
		return(g_pGUI);
	}

	void CGUI::destroyDesktop(IDesktop * dp)
	{
		if(!dp)
		{
			return;
		}
		for(AssotiativeArray<StringW, IDesktop*>::Iterator i = m_mDesktops.begin(); i; i++)
		{
			if(dp == *(i.second))
			{
				*(i.second) = NULL;
			}
		}
		if(getActiveDesktop() == dp)
		{
			setActiveDesktop(NULL);
		}
		delete dp;
	}

	void CGUI::messageBox(const WCHAR * title, const WCHAR * text, ...)
	{
		IDesktop * dp = getDesktopW(L"MessageBox");
		/*if(dp)
		{
			PopDesktop();
			DestroyDesktop(dp);
			dp = NULL;
		}*/
		if(!dp)
		{
			dp = createDesktopW(L"MessageBox", L"sys/messagebox.html");
		}
		dom::IDOMdocument * doc = dp->getDocument();
		doc->getElementById(L"mb_title")->setText(title, TRUE);
		dom::IDOMnode * pText = doc->getElementById(L"mb_text");// ->SetText(text, TRUE);

		while(pText->getChilds()->size())
		{
			pText->removeChild((*(pText->getChilds()))[0]);
		}

		dom::IDOMnodeCollection newText = dp->createFromText(text);
		for(UINT i = 0, l = newText.size(); i < l; i++)
		{
			pText->appendChild(newText[i]);
		}

		dom::IDOMnode * pBtns = doc->getElementById(L"mb_buttons");

		while(pBtns->getChilds()->size())
		{
			pBtns->removeChild((*(pBtns->getChilds()))[0]);
		}

		va_list ap;
		va_start(ap, text);

		WCHAR * cap;
		WCHAR * cmd;
		dom::IDOMnode * pFirst = NULL;
		while((cap = va_arg(ap, WCHAR*)))
		{
			cmd = va_arg(ap, WCHAR*);
			dom::IDOMnode * pNewNode = dp->createFromText(StringW(L"<button onclick=\"") + cmd + L"\">" + cap + L"</button>")[0];
			if(!pFirst)
			{
				pFirst = pNewNode;
			}
			pBtns->appendChild(pNewNode);
		}
		if(pFirst)
		{
			dp->requestFocus(pFirst);
		}
		va_end(ap);
		

		pushDesktop(dp);
	}

	void CGUI::pushDesktop(IDesktop * dp)
	{
		m_mDesktopStack.push_back(m_pActiveDesktop);
		m_pActiveDesktop->getDocument()->getElementsByTag(L"body")[0][0]->addPseudoclass(css::ICSSrule::PSEUDOCLASS_DISABLED);
		setActiveDesktop(dp, FALSE);
	}
	IDesktop * CGUI::popDesktop()
	{
		UINT s = m_mDesktopStack.size();
		if(s == 0)
		{
			return(NULL);
		}
		IDesktop * ret = m_pActiveDesktop;
		setActiveDesktop(m_mDesktopStack[s - 1], FALSE);
		m_pActiveDesktop->getDocument()->getElementsByTag(L"body")[0][0]->removePseudoclass(css::ICSSrule::PSEUDOCLASS_DISABLED);
		m_mDesktopStack.erase(s - 1);
		return(ret);
	}

	void CGUI::execCallback(const StringW cmd, IEvent * ev)
	{
		GUI_CALLBACK cb = getCallbackByName(cmd);
		if(cb)
		{
			cb(ev);
		}
		else
		{
			for(UINT i = 0, l = m_mCallbacksDefaults.size(); i < l; ++i)
			{
				m_mCallbacksDefaults[i](cmd.c_str(), ev);
			}
		}
		for(UINT i = 0, l = m_mCallbacksDefaultsWC.size(); i < l; ++i)
		{
			m_mCallbacksDefaultsWC[i](cmd.c_str(), ev);
		}
	}
};


gui::IGUI * InitInstance(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd)
{
	Core_SetOutPtr();

	gui::CGUI * pGui = new gui::CGUI(pDev, szResPath, hWnd);

	return(pGui);
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{

	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		
		gui::CKeyMap::init();

		gui::dom::HTMLelsInit();

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}