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

namespace GUI
{
	class CGUI: public IGUI
	{
	public:
		CGUI(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd);

		BOOL PutMessage(UINT msg, WPARAM wParam, LPARAM lParam);
		void Update();
		void Syncronize();

		void Render();

		void OnResetDevice();
		void OnLostDevice();

		void Release();


		IDesktop * CreateDesktopA(const char * name, const char * file);
		IDesktop * CreateDesktopW(const WCHAR * name, const WCHAR * file);

		void SetActiveDesktop(IDesktop * d, BOOL clearStack = TRUE);
		void SetActiveDesktopA(const char * name);
		void SetActiveDesktopW(const WCHAR * name);

		IDesktop * GetActiveDesktop();

		IDesktop * GetDesktopA(const char * name);
		IDesktop * GetDesktopW(const WCHAR * name);

		void RegisterCallback(const char * cbName, GUI_CALLBACK cb);

		void RegisterCallbackDefault(GUI_CALLBACK_WC cb, BOOL greedy = FALSE);

		GUI_CALLBACK GetCallbackByName(const char * cbName);
		GUI_CALLBACK GetCallbackByName(const StringW & cbName);

		void UpdateScreenSize();
		void ShowCursor(BOOL bShow);

		WCHAR * GetResourceDir();
		IDirect3DDevice9 * GetDevice();

		UINT GetScreenWidth();
		UINT GetScreenHeight();

		void MessageBox(const WCHAR * title, const WCHAR * text, ...);

		void PushDesktop(IDesktop *);
		IDesktop * PopDesktop();

		void DestroyDesktop(IDesktop * dp);

		void ExecCallback(const StringW cmd, IEvent * ev);
	protected:

		IDirect3DDevice9 * pDevice;
		WCHAR * szResourceDir;

		UINT iScreenWidth;
		UINT iScreenHeight;

		HWND hWnd;


		IDirect3DSurface9 * pOldDepthStencilSurface;
		IDirect3DSurface9 * pDepthStencilSurface;

		IDesktop * pActiveDesktop;
		Array<IDesktop*> mDesktopStack;

		bool m_bShowCursor;
		AssotiativeArray<StringW, IDesktop*> mDesktops;

		AssotiativeArray<StringW, /* Array< */GUI_CALLBACK/* > */> mCallbacks;
		Array<GUI_CALLBACK_WC> mCallbacksDefaults;
		Array<GUI_CALLBACK_WC> mCallbacksDefaultsWC;
	};

	CGUI * GetGUI();
};

EXTERN_C __declspec(dllexport) GUI::IGUI * InitInstance(IDirect3DDevice9 * pDev, const char * szResPath, HWND hWnd);



#endif
