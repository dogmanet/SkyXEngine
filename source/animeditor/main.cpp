
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Editor.h"

#if defined(_DEBUG)
#	pragma comment(lib, "sxguiwinapi_d.lib")
#else
#	pragma comment(lib, "sxguiwinapi.lib")
#endif


//function for initialize all the elements
void InitAllElements();

//function for delete all the elements
void DeleteAllElements();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	Editor edt;

	//InitAllElements();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, 0, 0, 0) != 0)
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//Render();
	}

	//DeleteAllElements();

	return(msg.wParam);
}
