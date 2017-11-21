
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
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->GetHWND());
	return 0;
}

void AboutSXWinCreator::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);

	AboutSXWinCreator::JobWindow = SXGUICrBaseWnd("About_SXWinCreator","About_SXWinCreator",0,0,((wrect.right/2) - 160),((wrect.bottom/2) - 100),320,200,0,0,CreateSolidBrush(RGB(220,220,220)),0,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,0,WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(AboutSXWinCreator::JobWindow);
	AboutSXWinCreator::JobWindow->AddHandler(HideAboutWndClose,WM_CLOSE,0,0,0,0,true);
	AboutSXWinCreator::StaticVerWinCreatorText = SXGUICrStatic("Current version of SXWinCreator: ", 20, 54, 163, 15, AboutSXWinCreator::JobWindow->GetHWND(), 0, 0);
	AboutSXWinCreator::StaticVerWinCreatorText->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerWinCreatorText->SetColorText(0,0,0);
	AboutSXWinCreator::StaticVerWinCreatorText->SetColorTextBk(255,255,255);
	AboutSXWinCreator::StaticVerWinCreatorText->SetTransparentTextBk(true);
	AboutSXWinCreator::StaticVerWinCreatorText->SetColorBrush(220,220,220);
	AboutSXWinCreator::StaticThisIs = SXGUICrStatic("SXWinCreator - program for the creation of elements WinAPI with library SXGUIWinApi", 48, 10, 210, 30, AboutSXWinCreator::JobWindow->GetHWND(), 0, 0);
	AboutSXWinCreator::StaticThisIs->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticThisIs->SetColorText(0,0,0);
	AboutSXWinCreator::StaticThisIs->SetColorTextBk(255,255,255);
	AboutSXWinCreator::StaticThisIs->SetTransparentTextBk(true);
	AboutSXWinCreator::StaticThisIs->SetColorBrush(220,220,220);
	AboutSXWinCreator::StaticVerGuiWinApiText = SXGUICrStatic("Current version of SXGUIWinapi:", 20, 74, 162, 15, AboutSXWinCreator::JobWindow->GetHWND(), 0, 0);
	AboutSXWinCreator::StaticVerGuiWinApiText->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerGuiWinApiText->SetColorText(0,0,0);
	AboutSXWinCreator::StaticVerGuiWinApiText->SetColorTextBk(255,255,255);
	AboutSXWinCreator::StaticVerGuiWinApiText->SetTransparentTextBk(true);
	AboutSXWinCreator::StaticVerGuiWinApiText->SetColorBrush(220,220,220);
	AboutSXWinCreator::DevelodepBy = SXGUICrStatic("Developed by the Project S.I.P", 46, 121, 222, 26, AboutSXWinCreator::JobWindow->GetHWND(), 0, 0);
	AboutSXWinCreator::DevelodepBy->SetFont("Microsoft Sans Serif",-16,0,400,0,0,0);
	AboutSXWinCreator::DevelodepBy->SetColorText(0,0,0);
	AboutSXWinCreator::DevelodepBy->SetColorTextBk(220,220,220);
	AboutSXWinCreator::DevelodepBy->SetTransparentTextBk(true);
	AboutSXWinCreator::DevelodepBy->SetColorBrush(220,220,220);
	AboutSXWinCreator::StaticVerWinCreatorVer = SXGUICrStatic("0.9.0", 185, 54, 100, 15, AboutSXWinCreator::JobWindow->GetHWND(), 0, 0);
	AboutSXWinCreator::StaticVerWinCreatorVer->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerWinCreatorVer->SetColorText(0,0,0);
	AboutSXWinCreator::StaticVerWinCreatorVer->SetColorTextBk(255,255,255);
	AboutSXWinCreator::StaticVerWinCreatorVer->SetTransparentTextBk(true);
	AboutSXWinCreator::StaticVerWinCreatorVer->SetColorBrush(220,220,220);
	AboutSXWinCreator::StaticVerGUIWinApiVer = SXGUICrStatic("0.9.81", 185, 74, 100, 15, AboutSXWinCreator::JobWindow->GetHWND(), 0, 0);
	AboutSXWinCreator::StaticVerGUIWinApiVer->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	AboutSXWinCreator::StaticVerGUIWinApiVer->SetColorText(0,0,0);
	AboutSXWinCreator::StaticVerGUIWinApiVer->SetColorTextBk(255,255,255);
	AboutSXWinCreator::StaticVerGUIWinApiVer->SetTransparentTextBk(true);
	AboutSXWinCreator::StaticVerGUIWinApiVer->SetColorBrush(220,220,220);
}
void AboutSXWinCreator::DeleteAllElements()
{
	SX_SAFE_DELETE(AboutSXWinCreator::JobWindow);
	SX_SAFE_DELETE(AboutSXWinCreator::StaticVerWinCreatorText);
	SX_SAFE_DELETE(AboutSXWinCreator::StaticThisIs);
	SX_SAFE_DELETE(AboutSXWinCreator::StaticVerGuiWinApiText);
	SX_SAFE_DELETE(AboutSXWinCreator::DevelodepBy);
	SX_SAFE_DELETE(AboutSXWinCreator::StaticVerWinCreatorVer);
	SX_SAFE_DELETE(AboutSXWinCreator::StaticVerGUIWinApiVer);
}
