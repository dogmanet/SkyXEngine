
#include "about_sxwincreator.h"

namespace AboutSXWinCreator
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIStatic* StaticVerWinCreatorText;
	ISXGUIStatic* StaticThisIs;
	ISXGUIStatic* StaticVerGuiWinApiText;
	ISXGUIStatic* DevelodepBy;
	ISXGUIStatic* StaticVerWinCreatorVer;
	ISXGUIStatic* StaticVerGUIWinApiVer;

	void InitAllElements();

	void DeleteAllElements();

};

LRESULT HideAboutWndClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ShowWindow(hwnd,SW_HIDE);
	SXMainWndElem::MainWnd->setEnable(true);
	SXMainWndElem::JobMainWnd->setEnable(true);
	SXMainWndElem::ParamWnd->setEnable(true);
	SXMainWndElem::WndLog->setEnable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->getHWND());
	return 0;
}

void AboutSXWinCreator::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);

	AboutSXWinCreator::JobWindow = SXGUICrBaseWndEx("About_SXWinCreator","About_SXWinCreator",((wrect.right/2) - 160),((wrect.bottom/2) - 100),320,200,0,0,CreateSolidBrush(RGB(220,220,220)),0,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,0,WndProcAllDefault);
	gui_func::base_handlers::InitHandlerMsg(AboutSXWinCreator::JobWindow);
	AboutSXWinCreator::JobWindow->addHandler(HideAboutWndClose,WM_CLOSE,0,0,0,0,true);
	AboutSXWinCreator::StaticVerWinCreatorText = SXGUICrStatic("Current version of SXWinCreator: ", 20, 54, 163, 15, AboutSXWinCreator::JobWindow->getHWND(), 0, 0);
	AboutSXWinCreator::StaticVerWinCreatorText->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerWinCreatorText->setColorText(RGB(0, 0, 0));
	AboutSXWinCreator::StaticVerWinCreatorText->setColorTextBk(RGB(255, 255, 255));
	AboutSXWinCreator::StaticVerWinCreatorText->setTransparentTextBk(true);
	AboutSXWinCreator::StaticVerWinCreatorText->setColorBrush(RGB(220, 220, 220));
	AboutSXWinCreator::StaticThisIs = SXGUICrStatic("SXWinCreator - program for the creation of elements WinAPI with library SXGUIWinApi", 48, 10, 210, 30, AboutSXWinCreator::JobWindow->getHWND(), 0, 0);
	AboutSXWinCreator::StaticThisIs->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticThisIs->setColorText(RGB(0, 0, 0));
	AboutSXWinCreator::StaticThisIs->setColorTextBk(RGB(255, 255, 255));
	AboutSXWinCreator::StaticThisIs->setTransparentTextBk(true);
	AboutSXWinCreator::StaticThisIs->setColorBrush(RGB(220, 220, 220));
	AboutSXWinCreator::StaticVerGuiWinApiText = SXGUICrStatic("Current version of SXGUIWinapi:", 20, 74, 162, 15, AboutSXWinCreator::JobWindow->getHWND(), 0, 0);
	AboutSXWinCreator::StaticVerGuiWinApiText->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerGuiWinApiText->setColorText(RGB(0, 0, 0));
	AboutSXWinCreator::StaticVerGuiWinApiText->setColorTextBk(RGB(255, 255, 255));
	AboutSXWinCreator::StaticVerGuiWinApiText->setTransparentTextBk(true);
	AboutSXWinCreator::StaticVerGuiWinApiText->setColorBrush(RGB(220, 220, 220));
	AboutSXWinCreator::DevelodepBy = SXGUICrStatic("Developed by the Project S.I.P", 46, 121, 222, 26, AboutSXWinCreator::JobWindow->getHWND(), 0, 0);
	AboutSXWinCreator::DevelodepBy->setFont("Microsoft Sans Serif",-16,0,400,0,0,0);
	AboutSXWinCreator::DevelodepBy->setColorText(RGB(0, 0, 0));
	AboutSXWinCreator::DevelodepBy->setColorTextBk(RGB(220, 220, 220));
	AboutSXWinCreator::DevelodepBy->setTransparentTextBk(true);
	AboutSXWinCreator::DevelodepBy->setColorBrush(RGB(220, 220, 220));
	AboutSXWinCreator::StaticVerWinCreatorVer = SXGUICrStatic("0.9.0", 185, 54, 100, 15, AboutSXWinCreator::JobWindow->getHWND(), 0, 0);
	AboutSXWinCreator::StaticVerWinCreatorVer->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerWinCreatorVer->setColorText(RGB(0, 0, 0));
	AboutSXWinCreator::StaticVerWinCreatorVer->setColorTextBk(RGB(255, 255, 255));
	AboutSXWinCreator::StaticVerWinCreatorVer->setTransparentTextBk(true);
	AboutSXWinCreator::StaticVerWinCreatorVer->setColorBrush(RGB(220, 220, 220));
	AboutSXWinCreator::StaticVerGUIWinApiVer = SXGUICrStatic("0.9.81", 185, 74, 100, 15, AboutSXWinCreator::JobWindow->getHWND(), 0, 0);
	AboutSXWinCreator::StaticVerGUIWinApiVer->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerGUIWinApiVer->setColorText(RGB(0, 0, 0));
	AboutSXWinCreator::StaticVerGUIWinApiVer->setColorTextBk(RGB(255, 255, 255));
	AboutSXWinCreator::StaticVerGUIWinApiVer->setTransparentTextBk(true);
	AboutSXWinCreator::StaticVerGUIWinApiVer->setColorBrush(RGB(220,220,220));
}
void AboutSXWinCreator::DeleteAllElements()
{
	mem_delete(AboutSXWinCreator::JobWindow);
	mem_delete(AboutSXWinCreator::StaticVerWinCreatorText);
	mem_delete(AboutSXWinCreator::StaticThisIs);
	mem_delete(AboutSXWinCreator::StaticVerGuiWinApiText);
	mem_delete(AboutSXWinCreator::DevelodepBy);
	mem_delete(AboutSXWinCreator::StaticVerWinCreatorVer);
	mem_delete(AboutSXWinCreator::StaticVerGUIWinApiVer);
}
