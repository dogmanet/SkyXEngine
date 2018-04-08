
#include "WindowOutput.h"

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
	SXNameSapce::PathForFile->getText(PathForFile,1024);
		if(strstr(PathForFile,":\\"))
		{
			char name_space[64];
			bool in_ns,constructors,destructors,comments,include;
			SXNameSapce::SXNameSpace->getText(name_space,64);
			in_ns = SXNameSapce::DetectedNameSpace->getCheck() == 1 ? true : false;
			constructors = SXNameSapce::Constructors->getCheck() == 1 ? true : false;
			destructors = SXNameSapce::Destructors->getCheck() == 1 ? true : false;
			comments = SXNameSapce::Comments->getCheck() == 1 ? true : false;
			include = SXNameSapce::Inlude->getCheck() == 1 ? true : false;

			OutputInFile(PathForFile,name_space,in_ns,constructors,destructors,comments,include);
			MessageBox(0,"Output in file is completed! Check result","Completed",MB_TASKMODAL);
			SXNameSapce::OutputGUIInFile->setVisible(false);
			SXMainWndElem::MainWnd->setEnable(true);
			SXMainWndElem::JobMainWnd->setEnable(true);
			SXMainWndElem::ParamWnd->setEnable(true);
			SXMainWndElem::WndLog->setEnable(true);
			SetActiveWindow(SXMainWndElem::MainWnd->getHWND());
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
	/*SXMainWndElem::MainWnd->setEnable(false);
	SXMainWndElem::JobMainWnd->setEnable(false);
	SXMainWndElem::ParamWnd->setEnable(false);
	SXMainWndElem::WndLog->setEnable(false);*/
		if(GetSaveFileName(&ofn) == TRUE)
		{
			SXNameSapce::PathForFile->setText(szFileName);
		}
	/*SXMainWndElem::MainWnd->setEnable(true);
	SXMainWndElem::JobMainWnd->setEnable(true);
	SXMainWndElem::ParamWnd->setEnable(true);
	SXMainWndElem::WndLog->setEnable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->getHWND());*/
	//InLog("%s","Вызов диалогового окна сохранения файла успешно завершен, активация окон");
		
	return 0;
}

LRESULT HideSettingsWndClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ShowWindow(hwnd,SW_HIDE);
	SXMainWndElem::MainWnd->setEnable(true);
	SXMainWndElem::JobMainWnd->setEnable(true);
	SXMainWndElem::ParamWnd->setEnable(true);
	SXMainWndElem::WndLog->setEnable(true);
	SetActiveWindow(SXMainWndElem::MainWnd->getHWND());
	return 0;
}

void SXNameSapce::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	
	SXNameSapce::OutputGUIInFile = SXGUICrBaseWndEx("OutputGUIInFile", "OutputGUIInFile", ((wrect.right / 2) - 170), ((wrect.bottom / 2) - 115), 340, 230, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED, 0, WndProcAllDefault);
	gui_func::base_handlers::InitHandlerMsg(SXNameSapce::OutputGUIInFile);
	SXNameSapce::OutputGUIInFile->addHandler(HideSettingsWndClose,WM_CLOSE,0,0,0,0,true);
	SXNameSapce::NameSpace = SXGUICrStatic("NameSpace", 13, 13, 100, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0);
	SXNameSapce::NameSpace->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::NameSpace->setColorText(RGB(0, 0, 0));
	SXNameSapce::NameSpace->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::NameSpace->setTransparentTextBk(true);
	SXNameSapce::NameSpace->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::SXNameSpace = SXGUICrEdit("SXNameSpace", 119, 11, 199, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0);
	SXNameSapce::SXNameSpace->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::SXNameSpace->setColorText(RGB(0, 0, 0));
	SXNameSapce::SXNameSpace->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::SXNameSpace->setTransparentTextBk(true);
	SXNameSapce::SXNameSpace->setColorBrush(RGB(255, 255, 255));
	SXNameSapce::Constructors = SXGUICrCheckBox("Constructors", 15, 60, 100, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0, false);
	SXNameSapce::Constructors->setCheck(SXGUI_CHECKBOX_STATE_CHECKED);
	SXNameSapce::Constructors->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Constructors->setColorText(RGB(0, 0, 0));
	SXNameSapce::Constructors->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::Constructors->setTransparentTextBk(true);
	SXNameSapce::Constructors->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::Destructors = SXGUICrCheckBox("Destructors", 15, 80, 100, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0, false);
	SXNameSapce::Destructors->setCheck(SXGUI_CHECKBOX_STATE_CHECKED);
	SXNameSapce::Destructors->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Destructors->setColorText(RGB(0, 0, 0));
	SXNameSapce::Destructors->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::Destructors->setTransparentTextBk(true);
	SXNameSapce::Destructors->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::Comments = SXGUICrCheckBox("Comments", 15, 100, 100, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0, false);
	SXNameSapce::Comments->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Comments->setColorText(RGB(0, 0, 0));
	SXNameSapce::Comments->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::Comments->setTransparentTextBk(true);
	SXNameSapce::Comments->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::Inlude = SXGUICrCheckBox("Inlude", 15, 120, 125, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0, false);
	SXNameSapce::Inlude->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::Inlude->setColorText(RGB(0, 0, 0));
	SXNameSapce::Inlude->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::Inlude->setTransparentTextBk(true);
	SXNameSapce::Inlude->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::DetectedNameSpace = SXGUICrCheckBox("In Name Space", 15, 40, 100, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0, false);
	SXNameSapce::DetectedNameSpace->setCheck(SXGUI_CHECKBOX_STATE_CHECKED);
	SXNameSapce::DetectedNameSpace->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::DetectedNameSpace->setColorText(RGB(0, 0, 0));
	SXNameSapce::DetectedNameSpace->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::DetectedNameSpace->setTransparentTextBk(true);
	SXNameSapce::DetectedNameSpace->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::SaveFile = SXGUICrStatic("SaveFile", 7, 161, 58, 16, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0);
	SXNameSapce::SaveFile->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::SaveFile->setColorText(RGB(0, 0, 0));
	SXNameSapce::SaveFile->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::SaveFile->setTransparentTextBk(true);
	SXNameSapce::SaveFile->setColorBrush(RGB(220, 220, 220));
	SXNameSapce::PathForFile = SXGUICrEdit("PathForFile", 61, 159, 233, 20, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0);
	SXNameSapce::PathForFile->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::PathForFile->setColorText(RGB(0, 0, 0));
	SXNameSapce::PathForFile->setColorTextBk(RGB(255, 255, 255));
	SXNameSapce::PathForFile->setTransparentTextBk(true);
	SXNameSapce::PathForFile->setColorBrush(RGB(255,255,255));
	SXNameSapce::ButtonSelectFile = SXGUICrButton("...", 298, 159, 23, 19, SXGUI_BUTTON_IMAGE_NONE, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0);
	SXNameSapce::ButtonSelectFile->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::ButtonSelectFile->addHandler(SelectButClick,WM_LBUTTONUP);
	SXNameSapce::StartOutput = SXGUICrButton("Start output", 160, 51, 134, 77, SXGUI_BUTTON_IMAGE_NONE, SXNameSapce::OutputGUIInFile->getHWND(), 0, 0);
	SXNameSapce::StartOutput->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSapce::StartOutput->addHandler(StartButClick,WM_LBUTTONUP);
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
