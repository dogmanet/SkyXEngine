#define mem_delete(p) if(p) {delete (p);(p) = NULL;}

namespace SXMaterialEditor
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIMenu* MainMenu;
	ISXGUIBaseWnd* WindowRender;
	ISXGUIToolBar* ToolBar;
	ISXGUIButton* ButtonSkyBox;
	ISXGUIEdit* EditSkyBox;
	ISXGUICheckBox* CheckBoxModelRot;
	ISXGUIStatic* StaticTypeModel;
	ISXGUIComboBox* ComboBoxTypeModel;
	ISXGUIStatic* StaticTestModel;
	ISXGUIComboBox* ComboBoxTestModel;
	ISXGUIStatic* StaticTex;
	ISXGUIEdit* EditTex;
	ISXGUIStatic* StaticVS;
	ISXGUIEdit* EditVS;
	ISXGUIStatic* StaticPS;
	ISXGUIEdit* EditPS;
	ISXGUIButton* ButtonTex;
	ISXGUIButton* ButtonVS;
	ISXGUIButton* ButtonPS;
	ISXGUICheckBox* CheckBoxAlphaTest;
	ISXGUIStatic* StaticMR;
	ISXGUIStatic* StaticRoughness;
	ISXGUIStatic* StaticF0;
	ISXGUIStatic* StaticMG;
	ISXGUIEdit* EditRoughness;
	ISXGUIEdit* EditF0;
	ISXGUIEdit* EditMR;
	ISXGUICheckBox* CheckBoxLighting;
	ISXGUITrackBar* TrackBarRoughness;
	ISXGUITrackBar* TrackBarF0;
	ISXGUITrackBar* TrackBarUDVSX;
	ISXGUIStatic* StaticMaD;
	ISXGUIEdit* EditTexLighting;
	ISXGUIButton* ButtonTexLighting;
	ISXGUICheckBox* CheckBoxTexLighting;
	ISXGUIComboBox* ComboBoxTypeRefract;
	ISXGUICheckBox* CheckBoxDoSVVS;
	ISXGUIComboBox* ComboBoxTypeReflect;
	ISXGUIStatic* StaticPhysic;
	ISXGUIComboBox* ComboBoxPhysic;
	ISXGUIStatic* StaticMask;
	ISXGUIEdit* EditMask;
	ISXGUIButton* ButtonMask;
	ISXGUIButton* ButtonMR;
	ISXGUIStatic* StaticMB;
	ISXGUIStatic* StaticMA;
	ISXGUIStatic* StaticDR;
	ISXGUIEdit* EditMG;
	ISXGUIEdit* EditMB;
	ISXGUIEdit* EditMA;
	ISXGUIEdit* EditDR;
	ISXGUIButton* ButtonMG;
	ISXGUIButton* ButtonMB;
	ISXGUIButton* ButtonMA;
	ISXGUIButton* ButtonDR;
	ISXGUIStatic* StaticDG;
	ISXGUIStatic* StaticDB;
	ISXGUIStatic* StaticDA;
	ISXGUIEdit* EditDG;
	ISXGUIEdit* EditDB;
	ISXGUIEdit* EditDA;
	ISXGUIButton* ButtonDG;
	ISXGUIButton* ButtonDB;
	ISXGUIButton* ButtonDA;
	ISXGUIStatic* StaticDoS;
	ISXGUIStatic* StaticDoSW;
	ISXGUICheckBox* CheckBoxDoSWVS;
	ISXGUICheckBox* CheckBoxDoSWPS;
	ISXGUIStatic* StaticDoSV;
	ISXGUICheckBox* CheckBoxDoSVPS;
	ISXGUIStatic* StaticDosP;
	ISXGUICheckBox* CheckBoxDoSPVS;
	ISXGUICheckBox* CheckBoxDoSPPS;
	ISXGUIStatic* StaticBoxDoSWV;
	ISXGUIStatic* StaticDoSWVP;
	ISXGUIStatic* StaticDoSCampos;
	ISXGUICheckBox* CheckBoxDoSWVVS;
	ISXGUICheckBox* CheckBoxDoSWVPS;
	ISXGUICheckBox* CheckBoxDoSCamposVS;
	ISXGUICheckBox* CheckBoxDoSWVPVS;
	ISXGUICheckBox* CheckBoxDoSWVPPS;
	ISXGUICheckBox* CheckBoxDoSCamposPS;
	ISXGUIStatic* StaticDoSWinSize;
	ISXGUICheckBox* CheckBoxDoSWinSizeVS;
	ISXGUICheckBox* CheckBoxDoSWinSizePS;

	ISXGUICheckBox* CheckBoxUDVS;

	ISXGUICheckBox* CheckBoxUDVSInPS;
	ISXGUIEdit* EditUDVSX;
	ISXGUIEdit* EditUDVSY;
	ISXGUITrackBar* TrackBarUDVSY;
	ISXGUIStatic* StaticUDVSX;
	ISXGUIStatic* StaticUDVSY;
	ISXGUIStatic* StaticUDVSZ;
	ISXGUIEdit* EditUDVSZ;
	ISXGUITrackBar* TrackBarUDVSZ;
	ISXGUIStatic* StaticUDVSW;
	ISXGUIEdit* EditUDVSW;
	ISXGUITrackBar* TrackBarUDVSW;
	ISXGUIStatic* StaticDoSTimeDelta;
	ISXGUICheckBox* CheckBoxDoSTimeDeltaVS;
	ISXGUICheckBox* CheckBoxDoSTimeDeltaPS;

	ISXGUICheckBox* CheckBoxUDPS;

	ISXGUICheckBox* CheckBoxUDPSInVS;
	ISXGUIStatic* StaticUDPSX;
	ISXGUIStatic* StaticUDPSY;
	ISXGUIStatic* StaticUDPSZ;
	ISXGUIStatic* StaticUDPSW;
	ISXGUIEdit* EditUDPSX;
	ISXGUIEdit* EditUDPSY;
	ISXGUITrackBar* TrackBarUDPSX;
	ISXGUITrackBar* TrackBarUDPSY;
	ISXGUIEdit* EditUDPSZ;
	ISXGUITrackBar* TrackBarUDPSZ;
	ISXGUIEdit* EditUDPSW;
	ISXGUITrackBar* TrackBarUDPSW;

	void InitAllElements();

	void DeleteAllElements();

	void Nulling();
	void InitMtl(ID id);

	ID IDMat = -1;
};

#include <sxmaterialeditor\\common_callback.cpp>
#include <sxmaterialeditor\\buttons_callback.cpp>
#include <sxmaterialeditor\\trackbars_callback.cpp>
#include <sxmaterialeditor\\edits_callback.cpp>

void SXMaterialEditor::InitAllElements()
{
	SXMaterialEditor::JobWindow = SXGUICrBaseWnd("SXMaterialEditor", "SXMaterialEditor", 0, 0, 366, 18, 645, 700, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXMaterialEditor::JobWindow);
	SXMaterialEditor::JobWindow->AddHandler(SXMaterialEditor_JobWindow_CallWmCommand, WM_COMMAND);

	SXMaterialEditor::WindowRender = SXGUICrBaseWnd("WindowRender","WindowRender",0,0,0,25,300,300,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXMaterialEditor::JobWindow->GetHWND(),0);
	SXMaterialEditor::ToolBar = SXGUICrToolBar(0,0,645,25,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	
	SXMaterialEditor::ButtonSkyBox = SXGUICrButton("...", 275, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonSkyBox->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonSkyBox->AddHandler(SXMaterialEditor_ButtonSkyBox_Click, WM_LBUTTONUP);

	SXMaterialEditor::EditSkyBox = SXGUICrEdit("0", 100, 330, 170, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::EditSkyBox->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditSkyBox->SetColorText(0, 0, 0);
	SXMaterialEditor::EditSkyBox->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::EditSkyBox->SetTransparentTextBk(true);
	SXMaterialEditor::EditSkyBox->SetColorBrush(255, 255, 255);
	SXMaterialEditor::EditSkyBox->AddHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::CheckBoxModelRot = SXGUICrCheckBox("rotate", 5, 330, 50, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxModelRot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxModelRot->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxModelRot->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxModelRot->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxModelRot->SetColorBrush(220, 220, 220);
	
	SXMaterialEditor::StaticTypeModel = SXGUICrStatic("Type model:",5,350,70,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticTypeModel->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticTypeModel->SetColorText(0,0,0);
	SXMaterialEditor::StaticTypeModel->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticTypeModel->SetTransparentTextBk(true);
	SXMaterialEditor::StaticTypeModel->SetColorBrush(220,220,220);

	SXMaterialEditor::ComboBoxTypeModel = SXGUICrComboBox("",100,347,200,100,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::ComboBoxTypeModel->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxTypeModel->SetColorText(0,0,0);
	SXMaterialEditor::ComboBoxTypeModel->SetColorTextBk(255,255,255);
	SXMaterialEditor::ComboBoxTypeModel->SetTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTypeModel->SetColorBrush(255,255,255);
	SXMaterialEditor::ComboBoxTypeModel->AddItem("geometry");
	SXMaterialEditor::ComboBoxTypeModel->AddItem("grass");
	SXMaterialEditor::ComboBoxTypeModel->AddItem("tree");
	SXMaterialEditor::ComboBoxTypeModel->AddItem("skin");
	SXMaterialEditor::ComboBoxTypeModel->SetSel(0);

	SXMaterialEditor::StaticTestModel = SXGUICrStatic("Test model:",5,372,70,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticTestModel->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticTestModel->SetColorText(0,0,0);
	SXMaterialEditor::StaticTestModel->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticTestModel->SetTransparentTextBk(true);
	SXMaterialEditor::StaticTestModel->SetColorBrush(220,220,220);
	SXMaterialEditor::ComboBoxTestModel = SXGUICrComboBox("",100,370,200,70,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::ComboBoxTestModel->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxTestModel->SetColorText(0,0,0);
	SXMaterialEditor::ComboBoxTestModel->SetColorTextBk(255,255,255);
	SXMaterialEditor::ComboBoxTestModel->SetTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTestModel->SetColorBrush(255,255,255);
	SXMaterialEditor::ComboBoxTestModel->AddItem("sphere");
	SXMaterialEditor::ComboBoxTestModel->AddItem("cube");
	SXMaterialEditor::ComboBoxTestModel->AddItem("plane");
	SXMaterialEditor::ComboBoxTestModel->SetSel(0);

	SXMaterialEditor::StaticTex = SXGUICrStatic("Tex:",5,400,30,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticTex->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticTex->SetColorText(0,0,0);
	SXMaterialEditor::StaticTex->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticTex->SetTransparentTextBk(true);
	SXMaterialEditor::StaticTex->SetColorBrush(220,220,220);
	SXMaterialEditor::EditTex = SXGUICrEdit("0",40,400,230,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditTex->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditTex->SetColorText(0,0,0);
	SXMaterialEditor::EditTex->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditTex->SetTransparentTextBk(true);
	SXMaterialEditor::EditTex->SetColorBrush(255,255,255);
	SXMaterialEditor::EditTex->AddHandler(SXMaterialEditor_EditTex_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::StaticVS = SXGUICrStatic("VS:",5,420,30,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticVS->SetColorText(0,0,0);
	SXMaterialEditor::StaticVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticVS->SetTransparentTextBk(true);
	SXMaterialEditor::StaticVS->SetColorBrush(220,220,220);

	SXMaterialEditor::EditVS = SXGUICrEdit("0",40,420,230,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditVS->SetColorText(0,0,0);
	SXMaterialEditor::EditVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditVS->SetTransparentTextBk(true);
	SXMaterialEditor::EditVS->SetColorBrush(255,255,255);
	SXMaterialEditor::EditVS->AddHandler(SXMaterialEditor_EditVS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::StaticPS = SXGUICrStatic("PS:",5,440,30,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticPS->SetColorText(0,0,0);
	SXMaterialEditor::StaticPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticPS->SetTransparentTextBk(true);
	SXMaterialEditor::StaticPS->SetColorBrush(220,220,220);

	SXMaterialEditor::EditPS = SXGUICrEdit("0",40,440,230,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditPS->SetColorText(0,0,0);
	SXMaterialEditor::EditPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditPS->SetTransparentTextBk(true);
	SXMaterialEditor::EditPS->SetColorBrush(255,255,255);
	SXMaterialEditor::EditPS->AddHandler(SXMaterialEditor_EditPS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::ButtonTex = SXGUICrButton("...", 275, 400, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonTex->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonTex->AddHandler(SXMaterialEditor_ButtonTex_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonVS = SXGUICrButton("...", 275, 420, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonVS->AddHandler(SXMaterialEditor_ButtonVS_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonPS = SXGUICrButton("...", 275, 440, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonPS->AddHandler(SXMaterialEditor_ButtonPS_Click, WM_LBUTTONUP);


	SXMaterialEditor::CheckBoxAlphaTest = SXGUICrCheckBox("alpha test", 430, 30, 70, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxAlphaTest->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxAlphaTest->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxAlphaTest->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxAlphaTest->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxAlphaTest->SetColorBrush(220, 220, 220);

	SXMaterialEditor::StaticMR = SXGUICrStatic("M R:",305,175,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticMR->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMR->SetColorText(0,0,0);
	SXMaterialEditor::StaticMR->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticMR->SetTransparentTextBk(true);
	SXMaterialEditor::StaticMR->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticRoughness = SXGUICrStatic("Roughness:",305,50,70,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticRoughness->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticRoughness->SetColorText(0,0,0);
	SXMaterialEditor::StaticRoughness->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticRoughness->SetTransparentTextBk(true);
	SXMaterialEditor::StaticRoughness->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticF0 = SXGUICrStatic("f0:",305,70,70,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticF0->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticF0->SetColorText(0,0,0);
	SXMaterialEditor::StaticF0->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticF0->SetTransparentTextBk(true);
	SXMaterialEditor::StaticF0->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticMG = SXGUICrStatic("M G:",305,195,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticMG->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMG->SetColorText(0,0,0);
	SXMaterialEditor::StaticMG->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticMG->SetTransparentTextBk(true);
	SXMaterialEditor::StaticMG->SetColorBrush(220,220,220);
	SXMaterialEditor::EditRoughness = SXGUICrEdit("0",375,50,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditRoughness->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditRoughness->SetColorText(0,0,0);
	SXMaterialEditor::EditRoughness->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditRoughness->SetTransparentTextBk(true);
	SXMaterialEditor::EditRoughness->SetColorBrush(255,255,255);
	SXMaterialEditor::EditRoughness->AddHandler(SXMaterialEditor_EditRoughness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditF0 = SXGUICrEdit("0",375,70,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditF0->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditF0->SetColorText(0,0,0);
	SXMaterialEditor::EditF0->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditF0->SetTransparentTextBk(true);
	SXMaterialEditor::EditF0->SetColorBrush(255,255,255);
	SXMaterialEditor::EditF0->AddHandler(SXMaterialEditor_EditF0_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditMR = SXGUICrEdit("0",360,175,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditMR->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMR->SetColorText(0,0,0);
	SXMaterialEditor::EditMR->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditMR->SetTransparentTextBk(true);
	SXMaterialEditor::EditMR->SetColorBrush(255,255,255);
	SXMaterialEditor::EditMR->AddHandler(SXMaterialEditor_EditMR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::CheckBoxLighting = SXGUICrCheckBox("Lighting",305,30,100,15,SXMaterialEditor::JobWindow->GetHWND(),0,0,false);
	SXMaterialEditor::CheckBoxLighting->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxLighting->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxLighting->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxLighting->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxLighting->SetColorBrush(220,220,220);
	
	SXMaterialEditor::TrackBarRoughness = SXGUICrTrackBar("",430,50,200,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::TrackBarRoughness->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarRoughness->AddHandler(SXMaterialEditor_TrackBarRoughness_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarF0 = SXGUICrTrackBar("",430,70,200,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::TrackBarF0->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarF0->AddHandler(SXMaterialEditor_TrackBarF0_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::StaticMaD = SXGUICrStatic("Microrelief and detail",305,135,100,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticMaD->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMaD->SetColorText(0,0,0);
	SXMaterialEditor::StaticMaD->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticMaD->SetTransparentTextBk(true);
	SXMaterialEditor::StaticMaD->SetColorBrush(220,220,220);

	SXMaterialEditor::EditTexLighting = SXGUICrEdit("0",370,90,230,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditTexLighting->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditTexLighting->SetColorText(0,0,0);
	SXMaterialEditor::EditTexLighting->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditTexLighting->SetTransparentTextBk(true);
	SXMaterialEditor::EditTexLighting->SetColorBrush(255,255,255);
	SXMaterialEditor::EditTexLighting->AddHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::ButtonTexLighting = SXGUICrButton("...", 605, 90, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonTexLighting->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonTexLighting->AddHandler(SXMaterialEditor_ButtonTexLighting_Click, WM_LBUTTONUP);

	SXMaterialEditor::CheckBoxTexLighting = SXGUICrCheckBox("texure:", 305, 90, 60, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxTexLighting->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxTexLighting->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxTexLighting->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxTexLighting->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxTexLighting->SetColorBrush(220,220,220);
	SXMaterialEditor::ComboBoxTypeRefract = SXGUICrComboBox("",305,110,155,70,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::ComboBoxTypeRefract->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxTypeRefract->SetColorText(0,0,0);
	SXMaterialEditor::ComboBoxTypeRefract->SetColorTextBk(255,255,255);
	SXMaterialEditor::ComboBoxTypeRefract->SetTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTypeRefract->SetColorBrush(255,255,255);
	SXMaterialEditor::ComboBoxTypeRefract->AddItem("none");
	SXMaterialEditor::ComboBoxTypeRefract->AddItem("transparency lighting");
	SXMaterialEditor::ComboBoxTypeRefract->SetSel(0);

	SXMaterialEditor::CheckBoxDoSVVS = SXGUICrCheckBox("vs", 340, 390, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSVVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSVVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSVVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSVVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSVVS->SetColorBrush(220,220,220);
	SXMaterialEditor::ComboBoxTypeReflect = SXGUICrComboBox("",475,110,155,120,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::ComboBoxTypeReflect->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxTypeReflect->SetColorText(0,0,0);
	SXMaterialEditor::ComboBoxTypeReflect->SetColorTextBk(255,255,255);
	SXMaterialEditor::ComboBoxTypeReflect->SetTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTypeReflect->SetColorBrush(255,255,255);
	SXMaterialEditor::ComboBoxTypeReflect->AddItem("none");
	SXMaterialEditor::ComboBoxTypeReflect->AddItem("reflection plane");
	SXMaterialEditor::ComboBoxTypeReflect->AddItem("reflection cube dynamic");
	SXMaterialEditor::ComboBoxTypeReflect->AddItem("reflection cube static");
	SXMaterialEditor::ComboBoxTypeReflect->SetSel(0);

	SXMaterialEditor::StaticPhysic = SXGUICrStatic("Physic material:",5,465,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticPhysic->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticPhysic->SetColorText(0,0,0);
	SXMaterialEditor::StaticPhysic->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticPhysic->SetTransparentTextBk(true);
	SXMaterialEditor::StaticPhysic->SetColorBrush(220,220,220);
	SXMaterialEditor::ComboBoxPhysic = SXGUICrComboBox("",100,462,200,120,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::ComboBoxPhysic->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxPhysic->SetColorText(0,0,0);
	SXMaterialEditor::ComboBoxPhysic->SetColorTextBk(255,255,255);
	SXMaterialEditor::ComboBoxPhysic->SetTransparentTextBk(true);
	SXMaterialEditor::ComboBoxPhysic->SetColorBrush(255,255,255);
	SXMaterialEditor::ComboBoxPhysic->AddItem("Бетон");
	SXMaterialEditor::ComboBoxPhysic->AddItem("Металл");
	SXMaterialEditor::ComboBoxPhysic->AddItem("Стекло");
	SXMaterialEditor::ComboBoxPhysic->AddItem("Пластик");
	SXMaterialEditor::ComboBoxPhysic->AddItem("Дерево");
	SXMaterialEditor::ComboBoxPhysic->AddItem("Плоть");
	SXMaterialEditor::ComboBoxPhysic->AddItem("Земля/песок");
	SXMaterialEditor::ComboBoxPhysic->SetSel(0);

	SXMaterialEditor::StaticMask = SXGUICrStatic("Mask:", 305, 152, 50, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::StaticMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticMask->SetColorText(0, 0, 0);
	SXMaterialEditor::StaticMask->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::StaticMask->SetTransparentTextBk(true);
	SXMaterialEditor::StaticMask->SetColorBrush(220, 220, 220);

	SXMaterialEditor::EditMask = SXGUICrEdit("0", 360, 152, 240, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::EditMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditMask->SetColorText(0, 0, 0);
	SXMaterialEditor::EditMask->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::EditMask->SetTransparentTextBk(true);
	SXMaterialEditor::EditMask->SetColorBrush(255, 255, 255);
	SXMaterialEditor::EditMask->AddHandler(SXMaterialEditor_EditMask_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::ButtonMask = SXGUICrButton("...", 605, 152, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonMask->AddHandler(SXMaterialEditor_ButtonMask_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonMR = SXGUICrButton("...", 605, 175, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonMR->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMR->AddHandler(SXMaterialEditor_ButtonMR_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticMB = SXGUICrStatic("M B:",305,215,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticMB->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMB->SetColorText(0,0,0);
	SXMaterialEditor::StaticMB->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticMB->SetTransparentTextBk(true);
	SXMaterialEditor::StaticMB->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticMA = SXGUICrStatic("M A:",305,235,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticMA->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMA->SetColorText(0,0,0);
	SXMaterialEditor::StaticMA->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticMA->SetTransparentTextBk(true);
	SXMaterialEditor::StaticMA->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDR = SXGUICrStatic("D R:",305,260,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDR->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDR->SetColorText(0,0,0);
	SXMaterialEditor::StaticDR->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDR->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDR->SetColorBrush(220,220,220);

	SXMaterialEditor::EditMG = SXGUICrEdit("0",360,195,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditMG->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMG->SetColorText(0,0,0);
	SXMaterialEditor::EditMG->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditMG->SetTransparentTextBk(true);
	SXMaterialEditor::EditMG->SetColorBrush(255,255,255);
	SXMaterialEditor::EditMG->AddHandler(SXMaterialEditor_EditMG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditMB = SXGUICrEdit("0",360,215,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditMB->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMB->SetColorText(0,0,0);
	SXMaterialEditor::EditMB->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditMB->SetTransparentTextBk(true);
	SXMaterialEditor::EditMB->SetColorBrush(255,255,255);
	SXMaterialEditor::EditMB->AddHandler(SXMaterialEditor_EditMB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditMA = SXGUICrEdit("0",360,235,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditMA->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMA->SetColorText(0,0,0);
	SXMaterialEditor::EditMA->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditMA->SetTransparentTextBk(true);
	SXMaterialEditor::EditMA->SetColorBrush(255,255,255);
	SXMaterialEditor::EditMA->AddHandler(SXMaterialEditor_EditMA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditDR = SXGUICrEdit("0",360,260,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditDR->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDR->SetColorText(0,0,0);
	SXMaterialEditor::EditDR->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditDR->SetTransparentTextBk(true);
	SXMaterialEditor::EditDR->SetColorBrush(255,255,255);
	SXMaterialEditor::EditDR->AddHandler(SXMaterialEditor_EditDR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::ButtonMG = SXGUICrButton("...", 605, 195, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonMG->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMG->AddHandler(SXMaterialEditor_ButtonMG_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonMB = SXGUICrButton("...", 605, 215, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonMB->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMB->AddHandler(SXMaterialEditor_ButtonMB_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonMA = SXGUICrButton("...", 605, 235, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonMA->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMA->AddHandler(SXMaterialEditor_ButtonMA_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonDR = SXGUICrButton("...", 605, 260, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonDR->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDR->AddHandler(SXMaterialEditor_ButtonDR_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticDG = SXGUICrStatic("D G:",305,280,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDG->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDG->SetColorText(0,0,0);
	SXMaterialEditor::StaticDG->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDG->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDG->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDB = SXGUICrStatic("D B:",305,300,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDB->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDB->SetColorText(0,0,0);
	SXMaterialEditor::StaticDB->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDB->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDB->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDA = SXGUICrStatic("D A:",305,320,50,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDA->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDA->SetColorText(0,0,0);
	SXMaterialEditor::StaticDA->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDA->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDA->SetColorBrush(220,220,220);

	SXMaterialEditor::EditDG = SXGUICrEdit("0",360,280,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditDG->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDG->SetColorText(0,0,0);
	SXMaterialEditor::EditDG->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditDG->SetTransparentTextBk(true);
	SXMaterialEditor::EditDG->SetColorBrush(255,255,255);
	SXMaterialEditor::EditDG->AddHandler(SXMaterialEditor_EditDG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditDB = SXGUICrEdit("0",360,300,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditDB->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDB->SetColorText(0,0,0);
	SXMaterialEditor::EditDB->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditDB->SetTransparentTextBk(true);
	SXMaterialEditor::EditDB->SetColorBrush(255,255,255);
	SXMaterialEditor::EditDB->AddHandler(SXMaterialEditor_EditDB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditDA = SXGUICrEdit("0",360,320,240,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditDA->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDA->SetColorText(0,0,0);
	SXMaterialEditor::EditDA->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditDA->SetTransparentTextBk(true);
	SXMaterialEditor::EditDA->SetColorBrush(255,255,255);
	SXMaterialEditor::EditDA->AddHandler(SXMaterialEditor_EditDA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::ButtonDG = SXGUICrButton("...", 605, 280, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonDG->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDG->AddHandler(SXMaterialEditor_ButtonDG_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonDB = SXGUICrButton("...", 605, 300, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonDB->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDB->AddHandler(SXMaterialEditor_ButtonDB_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonDA = SXGUICrButton("...", 605, 320, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::ButtonDA->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDA->AddHandler(SXMaterialEditor_ButtonDA_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticDoS = SXGUICrStatic("Data for shaders:",305,350,100,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDoS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoS->SetColorText(0,0,0);
	SXMaterialEditor::StaticDoS->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDoS->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoS->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDoSW = SXGUICrStatic("W mat:",305,370,35,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDoSW->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSW->SetColorText(0,0,0);
	SXMaterialEditor::StaticDoSW->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDoSW->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoSW->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSWVS = SXGUICrCheckBox("vs", 340, 370, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSWVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSWVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSWPS = SXGUICrCheckBox("ps", 373, 370, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSWPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSWPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWPS->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDoSV = SXGUICrStatic("V mat:",305,390,35,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDoSV->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSV->SetColorText(0,0,0);
	SXMaterialEditor::StaticDoSV->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDoSV->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoSV->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSVPS = SXGUICrCheckBox("ps", 373, 390, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSVPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSVPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSVPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSVPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSVPS->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDosP = SXGUICrStatic("P mat:",305,410,35,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDosP->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticDosP->SetColorText(0, 0, 0);
	SXMaterialEditor::StaticDosP->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::StaticDosP->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDosP->SetColorBrush(220, 220, 220);
	SXMaterialEditor::CheckBoxDoSPVS = SXGUICrCheckBox("vs", 340, 410, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSPVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSPVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSPVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSPVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSPVS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSPPS = SXGUICrCheckBox("ps", 373, 410, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSPPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSPPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSPPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSPPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSPPS->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticBoxDoSWV = SXGUICrStatic("WV mat:",410,370,45,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticBoxDoSWV->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticBoxDoSWV->SetColorText(0,0,0);
	SXMaterialEditor::StaticBoxDoSWV->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticBoxDoSWV->SetTransparentTextBk(true);
	SXMaterialEditor::StaticBoxDoSWV->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDoSWVP = SXGUICrStatic("WVP:",410,390,45,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDoSWVP->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSWVP->SetColorText(0,0,0);
	SXMaterialEditor::StaticDoSWVP->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDoSWVP->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoSWVP->SetColorBrush(220,220,220);
	SXMaterialEditor::StaticDoSCampos = SXGUICrStatic("Campos:",410,410,45,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticDoSCampos->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSCampos->SetColorText(0,0,0);
	SXMaterialEditor::StaticDoSCampos->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticDoSCampos->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoSCampos->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSWVVS = SXGUICrCheckBox("vs", 455, 370, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSWVVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSWVVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVVS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSWVPS = SXGUICrCheckBox("ps", 488, 370, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSWVPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVPS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSCamposVS = SXGUICrCheckBox("vs", 455, 410, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSCamposVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSCamposVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSCamposVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSCamposVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSCamposVS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSWVPVS = SXGUICrCheckBox("vs", 455, 390, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVPVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSWVPVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVPVS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSWVPPS = SXGUICrCheckBox("ps", 488, 390, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVPPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSWVPPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVPPS->SetColorBrush(220,220,220);
	SXMaterialEditor::CheckBoxDoSCamposPS = SXGUICrCheckBox("ps", 488, 411, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSCamposPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSCamposPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxDoSCamposPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxDoSCamposPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSCamposPS->SetColorBrush(220,220,220);

	SXMaterialEditor::StaticDoSTimeDelta = SXGUICrStatic("count_td:", 525, 370, 45, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::StaticDoSTimeDelta->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticDoSTimeDelta->SetColorText(0, 0, 0);
	SXMaterialEditor::StaticDoSTimeDelta->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::StaticDoSTimeDelta->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoSTimeDelta->SetColorBrush(220, 220, 220);

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS = SXGUICrCheckBox("vs", 570, 370, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->SetColorBrush(220, 220, 220);

	SXMaterialEditor::CheckBoxDoSTimeDeltaPS = SXGUICrCheckBox("ps", 603, 370, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->SetColorBrush(220, 220, 220);


	SXMaterialEditor::StaticDoSWinSize = SXGUICrStatic("win size:", 525, 390, 40, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::StaticDoSWinSize->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticDoSWinSize->SetColorText(0, 0, 0);
	SXMaterialEditor::StaticDoSWinSize->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::StaticDoSWinSize->SetTransparentTextBk(true);
	SXMaterialEditor::StaticDoSWinSize->SetColorBrush(220, 220, 220);

	SXMaterialEditor::CheckBoxDoSWinSizeVS = SXGUICrCheckBox("vs", 570, 390, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->SetColorBrush(220, 220, 220);

	SXMaterialEditor::CheckBoxDoSWinSizePS = SXGUICrCheckBox("ps", 603, 390, 30, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWinSizePS->SetColorBrush(220, 220, 220);


	SXMaterialEditor::CheckBoxUDVS = SXGUICrCheckBox("User data VS:", 305, 440, 100, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDVS->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxUDVS->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxUDVS->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxUDVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDVS->SetColorBrush(220, 220, 220);

	SXMaterialEditor::CheckBoxUDVSInPS = SXGUICrCheckBox("in PS", 445, 440, 100, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDVSInPS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxUDVSInPS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxUDVSInPS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxUDVSInPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDVSInPS->SetColorBrush(220,220,220);

	SXMaterialEditor::EditUDVSX = SXGUICrEdit("0",325,460,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDVSX->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSX->SetColorText(0,0,0);
	SXMaterialEditor::EditUDVSX->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDVSX->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDVSX->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDVSX->AddHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditUDVSY = SXGUICrEdit("0",325,480,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDVSY->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSY->SetColorText(0,0,0);
	SXMaterialEditor::EditUDVSY->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDVSY->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDVSY->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDVSY->AddHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	
	SXMaterialEditor::StaticUDVSX = SXGUICrStatic("x:",305,460,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDVSX->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSX->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDVSX->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDVSX->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSX->SetColorBrush(220,220,220);

	SXMaterialEditor::StaticUDVSY = SXGUICrStatic("y:",305,480,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDVSY->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSY->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDVSY->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDVSY->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSY->SetColorBrush(220,220,220);

	SXMaterialEditor::StaticUDVSZ = SXGUICrStatic("z:",305,500,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDVSZ->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSZ->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDVSZ->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDVSZ->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSZ->SetColorBrush(220,220,220);

	SXMaterialEditor::EditUDVSZ = SXGUICrEdit("0",325,500,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDVSZ->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSZ->SetColorText(0,0,0);
	SXMaterialEditor::EditUDVSZ->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDVSZ->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDVSZ->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDVSZ->AddHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);


	SXMaterialEditor::StaticUDVSW = SXGUICrStatic("w:",305,520,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDVSW->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSW->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDVSW->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDVSW->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSW->SetColorBrush(220,220,220);

	SXMaterialEditor::EditUDVSW = SXGUICrEdit("0",325,520,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDVSW->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSW->SetColorText(0,0,0);
	SXMaterialEditor::EditUDVSW->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDVSW->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDVSW->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDVSW->AddHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::TrackBarUDVSX = SXGUICrTrackBar("", 410, 460, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDVSX->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDVSX->AddHandler(SXMaterialEditor_TrackBarUDVSX_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDVSY = SXGUICrTrackBar("", 410, 480, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDVSY->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDVSY->AddHandler(SXMaterialEditor_TrackBarUDVSY_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDVSZ = SXGUICrTrackBar("", 410, 500, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDVSZ->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDVSZ->AddHandler(SXMaterialEditor_TrackBarUDVSZ_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDVSW = SXGUICrTrackBar("",410,520,220,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::TrackBarUDVSW->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDVSW->AddHandler(SXMaterialEditor_TrackBarUDVSW_MouseMove, WM_MOUSEMOVE);
	


	SXMaterialEditor::CheckBoxUDPS = SXGUICrCheckBox("User data PS:", 305, 550, 100, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDPS->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxUDPS->SetColorText(0, 0, 0);
	SXMaterialEditor::CheckBoxUDPS->SetColorTextBk(255, 255, 255);
	SXMaterialEditor::CheckBoxUDPS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDPS->SetColorBrush(220, 220, 220);


	SXMaterialEditor::CheckBoxUDPSInVS = SXGUICrCheckBox("in VS", 445, 550, 100, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDPSInVS->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxUDPSInVS->SetColorText(0,0,0);
	SXMaterialEditor::CheckBoxUDPSInVS->SetColorTextBk(255,255,255);
	SXMaterialEditor::CheckBoxUDPSInVS->SetTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDPSInVS->SetColorBrush(220,220,220);


	SXMaterialEditor::StaticUDPSX = SXGUICrStatic("x:",305,570,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDPSX->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSX->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDPSX->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDPSX->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSX->SetColorBrush(220,220,220);

	SXMaterialEditor::StaticUDPSY = SXGUICrStatic("y:",305,590,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDPSY->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSY->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDPSY->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDPSY->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSY->SetColorBrush(220,220,220);

	SXMaterialEditor::StaticUDPSZ = SXGUICrStatic("z:",305,610,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDPSZ->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSZ->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDPSZ->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDPSZ->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSZ->SetColorBrush(220,220,220);

	SXMaterialEditor::StaticUDPSW = SXGUICrStatic("w:",305,630,15,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::StaticUDPSW->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSW->SetColorText(0,0,0);
	SXMaterialEditor::StaticUDPSW->SetColorTextBk(255,255,255);
	SXMaterialEditor::StaticUDPSW->SetTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSW->SetColorBrush(220,220,220);


	SXMaterialEditor::EditUDPSX = SXGUICrEdit("0",325,570,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDPSX->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSX->SetColorText(0,0,0);
	SXMaterialEditor::EditUDPSX->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDPSX->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDPSX->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDPSX->AddHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditUDPSY = SXGUICrEdit("0",325,590,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDPSY->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSY->SetColorText(0,0,0);
	SXMaterialEditor::EditUDPSY->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDPSY->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDPSY->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDPSY->AddHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXMaterialEditor::EditUDPSZ = SXGUICrEdit("0",325,610,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDPSZ->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSZ->SetColorText(0,0,0);
	SXMaterialEditor::EditUDPSZ->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDPSZ->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDPSZ->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDPSZ->AddHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	
	SXMaterialEditor::EditUDPSW = SXGUICrEdit("0",325,630,80,15,SXMaterialEditor::JobWindow->GetHWND(),0,0);
	SXMaterialEditor::EditUDPSW->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSW->SetColorText(0,0,0);
	SXMaterialEditor::EditUDPSW->SetColorTextBk(255,255,255);
	SXMaterialEditor::EditUDPSW->SetTransparentTextBk(true);
	SXMaterialEditor::EditUDPSW->SetColorBrush(255,255,255);
	SXMaterialEditor::EditUDPSW->AddHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);


	SXMaterialEditor::TrackBarUDPSX = SXGUICrTrackBar("", 410, 570, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSX->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDPSX->AddHandler(SXMaterialEditor_TrackBarUDPSX_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDPSY = SXGUICrTrackBar("", 410, 590, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSY->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDPSY->AddHandler(SXMaterialEditor_TrackBarUDPSY_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDPSZ = SXGUICrTrackBar("", 410, 610, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSZ->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDPSZ->AddHandler(SXMaterialEditor_TrackBarUDPSZ_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDPSW = SXGUICrTrackBar("", 410, 630, 220, 15, SXMaterialEditor::JobWindow->GetHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSW->SetMinMax(0, 100);
	SXMaterialEditor::TrackBarUDPSW->AddHandler(SXMaterialEditor_TrackBarUDPSW_MouseMove, WM_MOUSEMOVE);
	
}
void SXMaterialEditor::DeleteAllElements()
{
	mem_delete(SXMaterialEditor::WindowRender);
	mem_delete(SXMaterialEditor::ToolBar);
	mem_delete(SXMaterialEditor::StaticTypeModel);
	mem_delete(SXMaterialEditor::ComboBoxTypeModel);
	mem_delete(SXMaterialEditor::StaticTestModel);
	mem_delete(SXMaterialEditor::ComboBoxTestModel);
	mem_delete(SXMaterialEditor::StaticTex);
	mem_delete(SXMaterialEditor::EditTex);
	mem_delete(SXMaterialEditor::StaticVS);
	mem_delete(SXMaterialEditor::EditVS);
	mem_delete(SXMaterialEditor::StaticPS);
	mem_delete(SXMaterialEditor::EditPS);
	mem_delete(SXMaterialEditor::ButtonTex);
	mem_delete(SXMaterialEditor::ButtonVS);
	mem_delete(SXMaterialEditor::ButtonPS);
	mem_delete(SXMaterialEditor::StaticMR);
	mem_delete(SXMaterialEditor::StaticRoughness);
	mem_delete(SXMaterialEditor::StaticF0);
	mem_delete(SXMaterialEditor::StaticMG);
	mem_delete(SXMaterialEditor::EditRoughness);
	mem_delete(SXMaterialEditor::EditF0);
	mem_delete(SXMaterialEditor::EditMR);
	mem_delete(SXMaterialEditor::CheckBoxLighting);
	mem_delete(SXMaterialEditor::TrackBarRoughness);
	mem_delete(SXMaterialEditor::TrackBarF0);
	mem_delete(SXMaterialEditor::TrackBarUDVSX);
	mem_delete(SXMaterialEditor::StaticMaD);
	mem_delete(SXMaterialEditor::EditTexLighting);
	mem_delete(SXMaterialEditor::ButtonTexLighting);
	mem_delete(SXMaterialEditor::CheckBoxTexLighting);
	mem_delete(SXMaterialEditor::ComboBoxTypeRefract);
	mem_delete(SXMaterialEditor::CheckBoxDoSVVS);
	mem_delete(SXMaterialEditor::ComboBoxTypeReflect);
	mem_delete(SXMaterialEditor::StaticPhysic);
	mem_delete(SXMaterialEditor::ComboBoxPhysic);
	mem_delete(SXMaterialEditor::ButtonMR);
	mem_delete(SXMaterialEditor::StaticMB);
	mem_delete(SXMaterialEditor::StaticMA);
	mem_delete(SXMaterialEditor::StaticDR);
	mem_delete(SXMaterialEditor::EditMG);
	mem_delete(SXMaterialEditor::EditMB);
	mem_delete(SXMaterialEditor::EditMA);
	mem_delete(SXMaterialEditor::EditDR);
	mem_delete(SXMaterialEditor::ButtonMG);
	mem_delete(SXMaterialEditor::ButtonMB);
	mem_delete(SXMaterialEditor::ButtonMA);
	mem_delete(SXMaterialEditor::ButtonDR);
	mem_delete(SXMaterialEditor::StaticDG);
	mem_delete(SXMaterialEditor::StaticDB);
	mem_delete(SXMaterialEditor::StaticDA);
	mem_delete(SXMaterialEditor::EditDG);
	mem_delete(SXMaterialEditor::EditDB);
	mem_delete(SXMaterialEditor::EditDA);
	mem_delete(SXMaterialEditor::ButtonDG);
	mem_delete(SXMaterialEditor::ButtonDB);
	mem_delete(SXMaterialEditor::ButtonDA);
	mem_delete(SXMaterialEditor::StaticDoS);
	mem_delete(SXMaterialEditor::StaticDoSW);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWPS);
	mem_delete(SXMaterialEditor::StaticDoSV);
	mem_delete(SXMaterialEditor::CheckBoxDoSVPS);
	mem_delete(SXMaterialEditor::StaticDosP);
	mem_delete(SXMaterialEditor::CheckBoxDoSPVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSPPS);
	mem_delete(SXMaterialEditor::StaticBoxDoSWV);
	mem_delete(SXMaterialEditor::StaticDoSWVP);
	mem_delete(SXMaterialEditor::StaticDoSCampos);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVPS);
	mem_delete(SXMaterialEditor::CheckBoxDoSCamposVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVPVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVPPS);
	mem_delete(SXMaterialEditor::CheckBoxDoSCamposPS);
	mem_delete(SXMaterialEditor::CheckBoxUDVS);
	mem_delete(SXMaterialEditor::CheckBoxUDVSInPS);
	mem_delete(SXMaterialEditor::EditUDVSX);
	mem_delete(SXMaterialEditor::EditUDVSY);
	mem_delete(SXMaterialEditor::TrackBarUDVSY);
	mem_delete(SXMaterialEditor::StaticUDVSX);
	mem_delete(SXMaterialEditor::StaticUDVSY);
	mem_delete(SXMaterialEditor::StaticUDVSZ);
	mem_delete(SXMaterialEditor::EditUDVSZ);
	mem_delete(SXMaterialEditor::TrackBarUDVSZ);
	mem_delete(SXMaterialEditor::StaticUDVSW);
	mem_delete(SXMaterialEditor::EditUDVSW);
	mem_delete(SXMaterialEditor::TrackBarUDVSW);
	mem_delete(SXMaterialEditor::StaticDoSTimeDelta);
	mem_delete(SXMaterialEditor::CheckBoxDoSTimeDeltaVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSTimeDeltaPS);
	mem_delete(SXMaterialEditor::CheckBoxUDPS);
	mem_delete(SXMaterialEditor::CheckBoxUDPSInVS);
	mem_delete(SXMaterialEditor::StaticUDPSX);
	mem_delete(SXMaterialEditor::StaticUDPSY);
	mem_delete(SXMaterialEditor::StaticUDPSZ);
	mem_delete(SXMaterialEditor::StaticUDPSW);
	mem_delete(SXMaterialEditor::EditUDPSX);
	mem_delete(SXMaterialEditor::EditUDPSY);
	mem_delete(SXMaterialEditor::TrackBarUDPSX);
	mem_delete(SXMaterialEditor::TrackBarUDPSY);
	mem_delete(SXMaterialEditor::EditUDPSZ);
	mem_delete(SXMaterialEditor::TrackBarUDPSZ);
	mem_delete(SXMaterialEditor::EditUDPSW);
	mem_delete(SXMaterialEditor::TrackBarUDPSW);
	mem_delete(SXMaterialEditor::StaticMask);
	mem_delete(SXMaterialEditor::EditMask);
	mem_delete(SXMaterialEditor::ButtonMask);
	mem_delete(SXMaterialEditor::StaticDoSWinSize);
	mem_delete(SXMaterialEditor::CheckBoxDoSWinSizeVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWinSizePS);
	mem_delete(SXMaterialEditor::CheckBoxAlphaTest);
	mem_delete(SXMaterialEditor::ButtonSkyBox);
	mem_delete(SXMaterialEditor::EditSkyBox);
	mem_delete(SXMaterialEditor::CheckBoxModelRot);
	mem_delete(SXMaterialEditor::JobWindow);
}
