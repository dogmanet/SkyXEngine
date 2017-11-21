
extern LRESULT MinimuzeWinInsteadClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern void OutputInFile(const char* path,const char* name_space,bool in_ns,bool constructors,bool destructors,bool comments,bool include);


namespace SXNameSapce
{
	ISXGUIBaseWnd* OutputGUIInFile;
	ISXGUIStatic* NameSpace;
	ISXGUIEdit* SXNameSpace;
	ISXGUICheckBox* Constructors;
	ISXGUICheckBox* Destructors;
	ISXGUICheckBox* Comments;
	ISXGUICheckBox* Inlude;
	ISXGUICheckBox* DetectedNameSpace;
	ISXGUIStatic* SaveFile;
	ISXGUIEdit* PathForFile;
	ISXGUIButton* ButtonSelectFile;
	ISXGUIButton* StartOutput;

	void InitAllElements();

	void DeleteAllElements();

};

LRESULT StartButClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char PathForFile[1024];
	SXNameSapce::PathForFile->GetText(PathForFile,1024);
		if(strstr(PathForFile,":\\"))
		{
			char name_space[64];
			bool in_ns,constructors,destructors,comments,include;
			SXNameSapce::SXNameSpace->GetText(name_space,64);
			in_ns = SXNameSapce::DetectedNameSpace->GetCheck() == 1 ? true : false;
			constructors = SXNameSapce::Constructors->GetCheck() == 1 ? true : false;
			destructors = SXNameSapce::Destructors->GetCheck() == 1 ? true : false;
			comments = SXNameSapce::Comments->GetCheck() == 1 ? true : false;
			include = SXNameSapce::Inlude->GetCheck() == 1 ? true : false;

			OutputInFile(PathForFile,name_space,in_ns,constructors,destructors,comments,include);
			MessageBox(0,"Output in file is completed! Check result","Completed",MB_TASKMODAL);
			SXNameSapce::OutputGUIInFile->Visible(false);
			SXMainWndElem::MainWnd->Enable(true);
			SXMainWndElem::JobMainWnd->Enable(true);
			SXMainWndElem::ParamWnd->Enable(true);
			SXMainWndElem::WndLog->Enable(true);
			SetActiveWindow(SXMainWndElem::MainWnd->GetHWND());
		}
		else
			MessageBox(0,"Make sure the path to the output file!",0,MB_TASKMODAL);
		
	return 0;
}

LRESULT SelectButClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OPENFILENAME ofn;
	char szFileName[1024];
	szFileName[0] = szFileName[1] = 0;

	char szFileTitle[256];
	szFileTitle[0] = szFileTitle[1] = 0;

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.hInstance	= GetModuleHandle(0);
	ofn.hwndOwner	= 0;         
	ofn.lpstrFilter	= "Все файлы\0*.*\0\0";
	ofn.lpstrFile	= szFileName;     
	ofn.nMaxFile	= sizeof(szFileName);

	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);

	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	//InLog("%s","Дизактивация окон и вызов диалогового окна сохранения файла ...");
	/*SXMainWndElem::MainWnd->Enable(false);
	SXMainWndElem::JobMainWnd->Enable(false);
	SXMainWndElem::ParamWnd->Enable(false);
	SXMainWndElem::WndLog->Enable(false);*/
		if(GetSaveFileName(&ofn) == TRUE)
		{
			SXNameSapce::PathForFile->SetText(szFileName);
		}
	/*SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->GetHWND());*/
	//InLog("%s","Вызов диалогового окна сохранения файла успешно завершен, активация окон");
		
	return 0;
}

LRESULT HideSettingsWndClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ShowWindow(hwnd,SW_HIDE);
	SXMainWndElem::MainWnd->Enable(true);
	SXMainWndElem::JobMainWnd->Enable(true);
	SXMainWndElem::ParamWnd->Enable(true);
	SXMainWndElem::WndLog->Enable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->GetHWND());
	return 0;
}

void SXNameSapce::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	
	SXNameSapce::OutputGUIInFile = SXGUICrBaseWnd("OutputGUIInFile", "OutputGUIInFile", 0, 0, ((wrect.right / 2) - 170), ((wrect.bottom / 2) - 115), 340, 230, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED, 0, WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXNameSapce::OutputGUIInFile);
	SXNameSapce::OutputGUIInFile->AddHandler(HideSettingsWndClose,WM_CLOSE,0,0,0,0,true);
	SXNameSapce::NameSpace = SXGUICrStatic("NameSpace", 13, 13, 100, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0);
	SXNameSapce::NameSpace->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::NameSpace->SetColorText(0,0,0);
	SXNameSapce::NameSpace->SetColorTextBk(255,255,255);
	SXNameSapce::NameSpace->SetTransparentTextBk(true);
	SXNameSapce::NameSpace->SetColorBrush(220,220,220);
	SXNameSapce::SXNameSpace = SXGUICrEdit("SXNameSpace", 119, 11, 199, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0);
	SXNameSapce::SXNameSpace->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::SXNameSpace->SetColorText(0,0,0);
	SXNameSapce::SXNameSpace->SetColorTextBk(255,255,255);
	SXNameSapce::SXNameSpace->SetTransparentTextBk(true);
	SXNameSapce::SXNameSpace->SetColorBrush(255,255,255);
	SXNameSapce::Constructors = SXGUICrCheckBox("Constructors", 15, 60, 100, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0, false);
	SXNameSapce::Constructors->SetCheck(SXGUI_CHECKBOX_CHECKED);
	SXNameSapce::Constructors->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Constructors->SetColorText(0,0,0);
	SXNameSapce::Constructors->SetColorTextBk(255,255,255);
	SXNameSapce::Constructors->SetTransparentTextBk(true);
	SXNameSapce::Constructors->SetColorBrush(220,220,220);
	SXNameSapce::Destructors = SXGUICrCheckBox("Destructors", 15, 80, 100, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0, false);
	SXNameSapce::Destructors->SetCheck(SXGUI_CHECKBOX_CHECKED);
	SXNameSapce::Destructors->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Destructors->SetColorText(0,0,0);
	SXNameSapce::Destructors->SetColorTextBk(255,255,255);
	SXNameSapce::Destructors->SetTransparentTextBk(true);
	SXNameSapce::Destructors->SetColorBrush(220,220,220);
	SXNameSapce::Comments = SXGUICrCheckBox("Comments", 15, 100, 100, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0, false);
	SXNameSapce::Comments->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Comments->SetColorText(0,0,0);
	SXNameSapce::Comments->SetColorTextBk(255,255,255);
	SXNameSapce::Comments->SetTransparentTextBk(true);
	SXNameSapce::Comments->SetColorBrush(220,220,220);
	SXNameSapce::Inlude = SXGUICrCheckBox("Inlude", 15, 120, 125, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0, false);
	SXNameSapce::Inlude->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Inlude->SetColorText(0,0,0);
	SXNameSapce::Inlude->SetColorTextBk(255,255,255);
	SXNameSapce::Inlude->SetTransparentTextBk(true);
	SXNameSapce::Inlude->SetColorBrush(220,220,220);
	SXNameSapce::DetectedNameSpace = SXGUICrCheckBox("In Name Space", 15, 40, 100, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0, false);
	SXNameSapce::DetectedNameSpace->SetCheck(SXGUI_CHECKBOX_CHECKED);
	SXNameSapce::DetectedNameSpace->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::DetectedNameSpace->SetColorText(0,0,0);
	SXNameSapce::DetectedNameSpace->SetColorTextBk(255,255,255);
	SXNameSapce::DetectedNameSpace->SetTransparentTextBk(true);
	SXNameSapce::DetectedNameSpace->SetColorBrush(220,220,220);
	SXNameSapce::SaveFile = SXGUICrStatic("SaveFile", 7, 161, 58, 16, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0);
	SXNameSapce::SaveFile->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::SaveFile->SetColorText(0,0,0);
	SXNameSapce::SaveFile->SetColorTextBk(255,255,255);
	SXNameSapce::SaveFile->SetTransparentTextBk(true);
	SXNameSapce::SaveFile->SetColorBrush(220,220,220);
	SXNameSapce::PathForFile = SXGUICrEdit("PathForFile", 61, 159, 233, 20, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0);
	SXNameSapce::PathForFile->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::PathForFile->SetColorText(0,0,0);
	SXNameSapce::PathForFile->SetColorTextBk(255,255,255);
	SXNameSapce::PathForFile->SetTransparentTextBk(true);
	SXNameSapce::PathForFile->SetColorBrush(255,255,255);
	SXNameSapce::ButtonSelectFile = SXGUICrButton("...", 298, 159, 23, 19, SXGUI_BUTTON_IMAGE_NONE, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0);
	SXNameSapce::ButtonSelectFile->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::ButtonSelectFile->AddHandler(SelectButClick,WM_LBUTTONUP);
	SXNameSapce::StartOutput = SXGUICrButton("Start output", 160, 51, 134, 77, SXGUI_BUTTON_IMAGE_NONE, SXNameSapce::OutputGUIInFile->GetHWND(), 0, 0);
	SXNameSapce::StartOutput->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::StartOutput->AddHandler(StartButClick,WM_LBUTTONUP);
}
void SXNameSapce::DeleteAllElements()
{
	delete SXNameSapce::OutputGUIInFile;
	SXNameSapce::OutputGUIInFile = 0;
	delete SXNameSapce::NameSpace;
	SXNameSapce::NameSpace = 0;
	delete SXNameSapce::SXNameSpace;
	SXNameSapce::SXNameSpace = 0;
	delete SXNameSapce::Constructors;
	SXNameSapce::Constructors = 0;
	delete SXNameSapce::Destructors;
	SXNameSapce::Destructors = 0;
	delete SXNameSapce::Comments;
	SXNameSapce::Comments = 0;
	delete SXNameSapce::Inlude;
	SXNameSapce::Inlude = 0;
	delete SXNameSapce::DetectedNameSpace;
	SXNameSapce::DetectedNameSpace = 0;
	delete SXNameSapce::SaveFile;
	SXNameSapce::SaveFile = 0;
	delete SXNameSapce::PathForFile;
	SXNameSapce::PathForFile = 0;
	delete SXNameSapce::ButtonSelectFile;
	SXNameSapce::ButtonSelectFile = 0;
	delete SXNameSapce::StartOutput;
	SXNameSapce::StartOutput = 0;
}
