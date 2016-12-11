#define mem_delete(p) if(p) {delete (p);(p) = NULL;}

namespace SXNameSpace
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIButton* Button1;
	ISXGUIButtonImg* ImgButton1;
	ISXGUIEdit* Edit1;
	ISXGUICheckBox* CheckBox1;
	ISXGUIComboBox* ComboBox1;
	ISXGUIGroupBox* qwrwre;
	ISXGUIListBox* ListBox1;
	ISXGUIMemo* Memo1;
	ISXGUIProgressBar* ProgressBar1;
	ISXGUIRadioButton* RadioButton1;
	ISXGUIStatic* Static1;
	ISXGUITrackBar* TrackBar1;
	ISXGUIStatusBar* StatusBar1;
	ISXGUIToolBar* ToolBar1;
	ISXGUIUpDown* UpDown1;
	ISXGUIBaseWnd* Window1;

	void InitAllElements();

	void DeleteAllElements();

};

void SXNameSpace::InitAllElements()
{
	SXNameSpace::JobWindow = SXGUICrBaseWnd("JobWindow","JobWindow",0,0,100,250,910,378,0,0,CreateSolidBrush(RGB(220,220,220)),0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,0,WndProcAllDefault);
SXGUIBaseHandlers::InitHandlerMsg(SXNameSpace::JobWindow);

	SXNameSpace::Button1 = SXGUICrButton("Button1",51,37,100,20,0,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::Button1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::ImgButton1 = SXGUICrButtonImg("",210,45,24,24,RGB(0,0,0),RGB(100,100,100),SXNameSpace::JobWindow->GetHWND(),0);
	SXNameSpace::ImgButton1->FrameColor = RGB(100,100,100);
	SXNameSpace::ImgButton1->EnableBf = true;
	SXNameSpace::ImgButton1->EnableState = true;
	SXNameSpace::Edit1 = SXGUICrEdit("Edit1",260,29,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::Edit1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::Edit1->SetColorText(0,0,0);
	SXNameSpace::Edit1->SetColorTextBk(255,255,255);
	SXNameSpace::Edit1->SetTransparentTextBk(true);
	SXNameSpace::Edit1->SetColorBrush(255,255,255);
	SXNameSpace::CheckBox1 = SXGUICrCheckBox("CheckBox1",440,54,100,20,SXNameSpace::JobWindow->GetHWND(),0,0,true);
	SXNameSpace::CheckBox1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::CheckBox1->SetColorText(0,0,0);
	SXNameSpace::CheckBox1->SetColorTextBk(255,255,255);
	SXNameSpace::CheckBox1->SetTransparentTextBk(true);
	SXNameSpace::CheckBox1->SetColorBrush(255,255,255);
	SXNameSpace::ComboBox1 = SXGUICrComboBox("",605,45,100,21,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::ComboBox1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::ComboBox1->SetColorText(0,0,0);
	SXNameSpace::ComboBox1->SetColorTextBk(255,255,255);
	SXNameSpace::ComboBox1->SetTransparentTextBk(true);
	SXNameSpace::ComboBox1->SetColorBrush(255,255,255);
	SXNameSpace::qwrwre = SXGUICrGroupBox("GroupBox1",46,116,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::qwrwre->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::qwrwre->SetColorText(0,0,0);
	SXNameSpace::qwrwre->SetColorTextBk(255,255,255);
	SXNameSpace::qwrwre->SetTransparentTextBk(true);
	SXNameSpace::qwrwre->SetColorBrush(255,255,255);
	SXNameSpace::ListBox1 = SXGUICrListBox("",186,121,100,80,SXNameSpace::JobWindow->GetHWND(),0,0,true);
	SXNameSpace::ListBox1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::ListBox1->SetColorText(0,0,0);
	SXNameSpace::ListBox1->SetColorTextBk(255,255,255);
	SXNameSpace::ListBox1->SetTransparentTextBk(true);
	SXNameSpace::ListBox1->SetColorBrush(255,255,255);
	SXNameSpace::Memo1 = SXGUICrMemo("Memo1",359,154,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::Memo1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::Memo1->SetColorText(0,0,0);
	SXNameSpace::Memo1->SetColorTextBk(255,255,255);
	SXNameSpace::Memo1->SetTransparentTextBk(true);
	SXNameSpace::Memo1->SetColorBrush(255,255,255);
	SXNameSpace::ProgressBar1 = SXGUICrProgressBar("",510,147,100,20,SXNameSpace::JobWindow->GetHWND(),0,0,true,true);
	SXNameSpace::RadioButton1 = SXGUICrRadioButton("RadioButton1",661,138,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::RadioButton1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::RadioButton1->SetColorText(0,0,0);
	SXNameSpace::RadioButton1->SetColorTextBk(255,255,255);
	SXNameSpace::RadioButton1->SetTransparentTextBk(true);
	SXNameSpace::RadioButton1->SetColorBrush(255,255,255);
	SXNameSpace::Static1 = SXGUICrStatic("Static1",62,250,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::Static1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::Static1->SetColorText(0,0,0);
	SXNameSpace::Static1->SetColorTextBk(255,255,255);
	SXNameSpace::Static1->SetTransparentTextBk(true);
	SXNameSpace::Static1->SetColorBrush(255,255,255);
	SXNameSpace::TrackBar1 = SXGUICrTrackBar("",207,260,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::StatusBar1 = SXGUICrStatusBar("",SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::StatusBar1->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXNameSpace::ToolBar1 = SXGUICrToolBar("",492,234,100,20,100,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::UpDown1 = SXGUICrUpDown(346,241,15,20,SXNameSpace::JobWindow->GetHWND(),0,0);
	SXNameSpace::Window1 = SXGUICrBaseWnd("Window1","Window1",0,0,641,107,200,200,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXNameSpace::JobWindow->GetHWND(),0);
}
void SXNameSpace::DeleteAllElements()
{
	mem_delete(SXNameSpace::Button1);
	mem_delete(SXNameSpace::ImgButton1);
	mem_delete(SXNameSpace::Edit1);
	mem_delete(SXNameSpace::CheckBox1);
	mem_delete(SXNameSpace::ComboBox1);
	mem_delete(SXNameSpace::qwrwre);
	mem_delete(SXNameSpace::ListBox1);
	mem_delete(SXNameSpace::Memo1);
	mem_delete(SXNameSpace::ProgressBar1);
	mem_delete(SXNameSpace::RadioButton1);
	mem_delete(SXNameSpace::Static1);
	mem_delete(SXNameSpace::TrackBar1);
	mem_delete(SXNameSpace::StatusBar1);
	mem_delete(SXNameSpace::ToolBar1);
	mem_delete(SXNameSpace::UpDown1);
	mem_delete(SXNameSpace::Window1);
	mem_delete(SXNameSpace::JobWindow);
}
