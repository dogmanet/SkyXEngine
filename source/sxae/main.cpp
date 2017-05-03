
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Editor.h"

#if defined(_DEBUG)
#	pragma comment(lib, "sxguiwinapi_d.lib")
#else
#	pragma comment(lib, "sxguiwinapi.lib")
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SXGUIRegClass::RegGroupBox();

	Editor edt;

	MSG msg = {0};
	
	while(WM_QUIT != msg.message)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && WM_QUIT != msg.message)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		edt.Update();
	}

	return(msg.wParam);
}
