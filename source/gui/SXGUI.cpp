#include "GUIbase.h"
#include "IHTMLparser.h"
#include "IDOMdocument.h"
#include "HTMLels.h"

#include "IHTML.h"

#include "IRenderFrame.h"

#include "IFont.h"

#include "IRenderBorder.h"

#include "IVideoRenderer.h"

#include "CDesktop.h"
#include "IVIDEO.h"
#include "ITEXT.h"


namespace GUI
{
	CPITexture def_w;
	CSHADER def_sh;
	CGUI * g_pGUI;

	CGUI::CGUI(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd):
		pDevice(pDev),
		szResourceDir(NULL),
		iScreenWidth(0),
		iScreenHeight(0),
		hWnd(hWnd),
		pOldDepthStencilSurface(NULL),
		pDepthStencilSurface(NULL),
		pActiveDesktop(NULL),
		m_bShowCursor(TRUE)
	{
		g_pGUI = this;
		UpdateScreenSize();

		StringW srp = String(szResPath);

		szResourceDir = new WCHAR[srp.length() + 1];
		memcpy(szResourceDir, srp.c_str(), sizeof(WCHAR) * (srp.length() + 1));

		def_w = ITextureManager::GetTexture(L"/dev/white.png");
		def_sh = ITextureManager::LoadShader(L"text");
	}

	BOOL CGUI::PutMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(!pActiveDesktop)
		{
			return(TRUE);
		}
		IEvent ev;
		const DOM::IDOMnodeCollection * els;
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
			UpdateScreenSize();
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
				ScreenToClient(hWnd, &mc);
				ev.clientX = mc.x;
				ev.clientY = mc.y;
				for(; wheelCount != 0; wheelCount += (wheelCount > 0 ? -1 : 1))
				{
					pActiveDesktop->DispatchEvent(ev);
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
				pActiveDesktop->DispatchEvent(ev);
				break;
			}
			if(KeyMap::KeyDown(wParam))
			{
				ev.type = GUI_EVENT_TYPE_KEYDOWN;
				ev.clientX = ev.clientY = 0;
				ev.key = wParam;
				pActiveDesktop->DispatchEvent(ev);
			}
			if(wParam == KEY_ESCAPE)
			{
				PopDesktop();
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			if(KeyMap::KeyUp(wParam))
			{
				ev.type = GUI_EVENT_TYPE_KEYUP;
				ev.clientX = ev.clientY = 0;
				ev.key = wParam;
				pActiveDesktop->DispatchEvent(ev);
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

			pActiveDesktop->DispatchEvent(ev);

			break;

		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_LBUTTON;
			KeyMap::KeyDown(KEY_LBUTTON);
			pActiveDesktop->DispatchEvent(ev);

			break;

		case WM_LBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_LBUTTON;
			KeyMap::KeyUp(KEY_LBUTTON);
			pActiveDesktop->DispatchEvent(ev);
			break;

		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_RBUTTON;
			KeyMap::KeyDown(KEY_RBUTTON);
			pActiveDesktop->DispatchEvent(ev); break;

		case WM_RBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_RBUTTON;
			KeyMap::KeyUp(KEY_RBUTTON);
			pActiveDesktop->DispatchEvent(ev);
			break;



		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_MBUTTON;
			KeyMap::KeyDown(KEY_MBUTTON);
			pActiveDesktop->DispatchEvent(ev); break;

		case WM_MBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = KEY_MBUTTON;
			KeyMap::KeyUp(KEY_MBUTTON);
			pActiveDesktop->DispatchEvent(ev);
			break;



		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
			ev.type = GUI_EVENT_TYPE_MOUSEDOWN;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = GET_XBUTTON_WPARAM(wParam);
			KeyMap::KeyDown(GET_XBUTTON_WPARAM(wParam));
			pActiveDesktop->DispatchEvent(ev); break;

		case WM_XBUTTONUP:
			ev.type = GUI_EVENT_TYPE_MOUSEUP;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			ev.key = GET_XBUTTON_WPARAM(wParam);
			KeyMap::KeyUp(GET_XBUTTON_WPARAM(wParam));
			pActiveDesktop->DispatchEvent(ev);
			break;

		case WM_MOUSEMOVE:
			ev.type = GUI_EVENT_TYPE_MOUSEMOVE;
			ev.clientX = lParam & 0xFFFF;
			ev.clientY = (lParam >> 16) & 0xFFFF;
			pActiveDesktop->DispatchEvent(ev);
			break;
		}
		//OutputDebugStringA("PutMessage():return(true);\n");
		return(TRUE);
	}

	void CGUI::Update()
	{
		//@TODO: Implement me
	}

	void CGUI::Syncronize()
	{
		//@TODO: Implement me
	}

	void CGUI::Render()
	{
		if(!pActiveDesktop)
		{
			return;
		}
		//pDevice->GetS

		//		pDevice->GetDepthStencilSurface(&pOldDepthStencilSurface);
		//		pDevice->SetDepthStencilSurface(pDepthStencilSurface);

		//	g_pVid.Update();
		IVideoUpdateManager::Update();

		pDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);

		//pDevice->SetRenderState(D3DRS_STENCILENABLE, true);

		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xFF);
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		//		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
		//		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		//		pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		pDevice->BeginScene();
		pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		ITextureManager::BindTexture(def_w);

		SMMATRIX mi = SMMatrixIdentity();
		pDevice->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&mi));
		pDevice->SetTransform(D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>(&mi));

		pDevice->SetVertexShader(NULL);
		pDevice->SetPixelShader(NULL);

		SMMATRIX m(
			2.0f / (float)iScreenWidth, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / (float)iScreenHeight, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			-1.0f, 1.0f, 0.5f, 1.0f);
		m = SMMatrixTranslation(-0.5f, -0.5f, 0.0f) * m;
		pDevice->SetTransform(D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>(&m));
		//doc->Render();
		for(UINT i = 0, l = mDesktopStack.size(); i < l; ++i)
		{
			if(mDesktopStack[i])
			{
				mDesktopStack[i]->Render();
			}
		}
		pActiveDesktop->Render();
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


		pDevice->EndScene();

		//		pDevice->SetDepthStencilSurface(pOldDepthStencilSurface);
	}

	void CGUI::OnResetDevice()
	{
		//@TODO: Implement me
	}

	void CGUI::OnLostDevice()
	{
		//@TODO: Implement me
	}

	void CGUI::Release()
	{
		//@TODO: Release desired objects
		delete this;
	}

	IDesktop * CGUI::CreateDesktopA(const char * name, const char * file)
	{
		IDesktop * d = new CDesktop();
		d->SetWidth(iScreenWidth);
		d->SetHeight(iScreenHeight);
		d->UpdateSize();
		d->LoadFromFile(StringW(String(file)).c_str());
		mDesktops[String(name)] = d;
		return(d);
	}

	IDesktop * CGUI::CreateDesktopW(const wchar_t * name, const wchar_t * file)
	{
		IDesktop * d = new CDesktop();
		d->SetWidth(iScreenWidth);
		d->SetHeight(iScreenHeight);
		d->UpdateSize();
		d->LoadFromFile(file);
		((DOM::CDOMdocument*)(d->GetDocument()))->Reflow();
		mDesktops[name] = d;
		return(d);
	}

	void CGUI::SetActiveDesktop(IDesktop * d, BOOL clearStack)
	{
		pActiveDesktop = d;
		KeyMap::ReleaseKeys();
		if(clearStack)
		{
			mDesktopStack.clear();
		}
	}

	void CGUI::SetActiveDesktopA(const char * name)
	{
		SetActiveDesktop(GetDesktopA(name));
	}
	void CGUI::SetActiveDesktopW(const WCHAR * name)
	{
		SetActiveDesktop(GetDesktopW(name));
	}

	IDesktop * CGUI::GetActiveDesktop()
	{
		return(pActiveDesktop);
	}

	IDesktop * CGUI::GetDesktopA(const char * _name)
	{
		StringW name = String(_name);
		if(mDesktops.KeyExists(name))
		{
			return(mDesktops[name]);
		}
		return(NULL);
	}
	IDesktop * CGUI::GetDesktopW(const wchar_t * name)
	{
		if(mDesktops.KeyExists(name))
		{
			return(mDesktops[name]);
		}
		return(NULL);
	}

	void CGUI::RegisterCallback(const char * cbName, GUI_CALLBACK cb)
	{
		if(cb)
		{
			//mCallbacks[StringW(String(cbName))].push_back(cb);
			mCallbacks[StringW(String(cbName))] = cb;
		}
	}

	void CGUI::RegisterCallbackDefault(GUI_CALLBACK_WC cb, BOOL greedy)
	{
		if(cb)
		{
			(greedy ? mCallbacksDefaults : mCallbacksDefaultsWC).push_back(cb);
		}
	}

	void CGUI::UpdateScreenSize()
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		iScreenHeight = rect.bottom - rect.top;
		iScreenWidth = rect.right - rect.left;
	}

	void CGUI::ShowCursor(BOOL bShow)
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

	WCHAR * CGUI::GetResourceDir()
	{
		return(szResourceDir);
	}

	IDirect3DDevice9 * CGUI::GetDevice()
	{
		return(pDevice);
	}

	GUI_CALLBACK CGUI::GetCallbackByName(const char * _cbName)
	{
		StringW cbName = String(_cbName);
		if(mCallbacks.KeyExists(cbName))
		{
			return(mCallbacks[cbName]);
		}
		return(NULL);
	}

	GUI_CALLBACK CGUI::GetCallbackByName(const StringW & cbName)
	{
		if(mCallbacks.KeyExists(cbName))
		{
			return(mCallbacks[cbName]);
		}
		return(NULL);
	}

	UINT CGUI::GetScreenWidth()
	{
		return(iScreenWidth);
	}
	UINT CGUI::GetScreenHeight()
	{
		return(iScreenHeight);
	}

	CGUI * GetGUI()
	{
		return(g_pGUI);
	}

	void CGUI::DestroyDesktop(IDesktop * dp)
	{
		if(!dp)
		{
			return;
		}
		for(AssotiativeArray<StringW, IDesktop*>::Iterator i = mDesktops.begin(); i; i++)
		{
			if(dp == *(i.second))
			{
				*(i.second) = NULL;
			}
		}
		if(GetActiveDesktop() == dp)
		{
			SetActiveDesktop(NULL);
		}
		delete dp;
	}

	void CGUI::MessageBox(const WCHAR * title, const WCHAR * text, ...)
	{
		IDesktop * dp = GetDesktopW(L"MessageBox");
		/*if(dp)
		{
			PopDesktop();
			DestroyDesktop(dp);
			dp = NULL;
		}*/
		if(!dp)
		{
			dp = CreateDesktopW(L"MessageBox", L"sys/messagebox.html");
		}
		DOM::IDOMdocument * doc = dp->GetDocument();
		doc->GetElementById(L"mb_title")->SetText(title, TRUE);
		doc->GetElementById(L"mb_text")->SetText(text, TRUE);
		DOM::IDOMnode * pBtns = doc->GetElementById(L"mb_buttons");

		while(pBtns->GetChilds()->size())
		{
			pBtns->RemoveChild((*(pBtns->GetChilds()))[0]);
		}

		va_list ap;
		va_start(ap, text);

		WCHAR * cap;
		WCHAR * cmd;
		DOM::IDOMnode * pFirst = NULL;
		while((cap = va_arg(ap, WCHAR*)))
		{
			cmd = va_arg(ap, WCHAR*);
			DOM::IDOMnode * pNewNode = dp->CreateFromText(StringW(L"<button onclick=\"") + cmd + L"\">" + cap + L"</button>")[0];
			if(!pFirst)
			{
				pFirst = pNewNode;
			}
			pBtns->AppendChild(pNewNode);
		}
		if(pFirst)
		{
			dp->RequestFocus(pFirst);
		}
		va_end(ap);
		

		PushDesktop(dp);
	}

	void CGUI::PushDesktop(IDesktop * dp)
	{
		mDesktopStack.push_back(pActiveDesktop);
		SetActiveDesktop(dp, FALSE);
	}
	IDesktop * CGUI::PopDesktop()
	{
		UINT s = mDesktopStack.size();
		if(s == 0)
		{
			return(NULL);
		}
		IDesktop * ret = pActiveDesktop;
		SetActiveDesktop(mDesktopStack[s - 1], FALSE);
		mDesktopStack.erase(s - 1);
		return(ret);
	}

	void CGUI::ExecCallback(const StringW cmd, IEvent * ev)
	{
		GUI_CALLBACK cb = GetCallbackByName(cmd);
		if(cb)
		{
			cb(ev);
		}
		else
		{
			for(UINT i = 0, l = mCallbacksDefaults.size(); i < l; ++i)
			{
				mCallbacksDefaults[i](cmd.c_str(), ev);
			}
		}
		for(UINT i = 0, l = mCallbacksDefaultsWC.size(); i < l; ++i)
		{
			mCallbacksDefaultsWC[i](cmd.c_str(), ev);
		}
	}
};

GUI::IGUI * InitInstance(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd)
{
	GUI::CGUI * pGui = new GUI::CGUI(pDev, szResPath, hWnd);

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
		
		GUI::KeyMap::Init();

		GUI::DOM::HTMLelsInit();

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