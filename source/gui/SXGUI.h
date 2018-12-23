#ifndef _SXGUI_H_
#define _SXGUI_H_

#include <Windows.h>
#include <d3d9.h>

#if 0
#ifndef SXGUI_LIB
#	ifdef _DEBUG
#		pragma comment(lib,"SXGUId.lib")
#	else
#		pragma comment(lib,"SXGUI.lib")
#	endif
#endif
#endif

#include "guimain.h"

namespace gui
{
	class CGUI: public IGUI
	{
	public:
		CGUI(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd);

		BOOL putMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		void update();
		void syncronize();

		void render();

		void onResetDevice();
		void onLostDevice();

		void release();


		IDesktop * createDesktopA(const char * name, const char * file);
		IDesktop * createDesktopW(const WCHAR * name, const WCHAR * file);

		void setActiveDesktop(IDesktop * d, BOOL clearStack = TRUE);
		void setActiveDesktopA(const char * name);
		void setActiveDesktopW(const WCHAR * name);

		IDesktop * getActiveDesktop();

		IDesktop * getDesktopA(const char * name);
		IDesktop * getDesktopW(const WCHAR * name);

		void registerCallback(const char * cbName, GUI_CALLBACK cb);

		void registerCallbackDefault(GUI_CALLBACK_WC cb, BOOL greedy = FALSE);

		GUI_CALLBACK getCallbackByName(const char * cbName);
		GUI_CALLBACK getCallbackByName(const StringW & cbName);

		void updateScreenSize();
		void showCursor(BOOL bShow);

		WCHAR * getResourceDir();
		IDirect3DDevice9 * getDevice();

		UINT getScreenWidth();
		UINT getScreenHeight();

		void messageBox(const WCHAR * title, const WCHAR * text, ...);

		void pushDesktop(IDesktop *);
		IDesktop * popDesktop();

		void destroyDesktop(IDesktop * dp);

		void execCallback(const StringW cmd, IEvent * ev);
	protected:

		IDirect3DDevice9 * m_pDevice;
		WCHAR * m_szResourceDir;

		UINT m_iScreenWidth;
		UINT m_iScreenHeight;

		HWND m_hWnd;


		IDirect3DSurface9 * m_pOldDepthStencilSurface;
		IDirect3DSurface9 * m_pDepthStencilSurface;

		IDesktop * m_pActiveDesktop;
		Array<IDesktop*> m_mDesktopStack;

		bool m_bShowCursor;
		AssotiativeArray<StringW, IDesktop*> m_mDesktops;

		AssotiativeArray<StringW, /* Array< */GUI_CALLBACK/* > */> m_mCallbacks;
		Array<GUI_CALLBACK_WC> m_mCallbacksDefaults;
		Array<GUI_CALLBACK_WC> m_mCallbacksDefaultsWC;

		bool m_bDeviceLost;
	};

	CGUI * GetGUI();
};

EXTERN_C __declspec(dllexport) gui::IGUI * InitInstance(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd);



#endif
