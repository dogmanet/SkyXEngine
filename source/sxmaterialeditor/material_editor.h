
#ifndef __MATERIAL_EDITOR_H
#define __MATERIAL_EDITOR_H

#define MAINWIN_SIZE_X	645
#define MAINWIN_SIZE_Y	700

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <mtllight/sxmtllight.h>
#include <render/sxrender.h>
#include <sxmaterialeditor/resource.h>

#include "common_callback.h"
#include "buttons_callback.h"
#include "trackbars_callback.h"
#include "edits_callback.h"

#include "shaders_kit.h"
#include "paraml_kit.h"

namespace SXMaterialEditor
{
	extern ISXGUIBaseWnd* JobWindow;
	extern ISXGUIMenuWindow* MainMenu;
	extern ISXGUIBaseWnd* WindowRender;

	extern ISXGUIToolBar* ToolBar;
	extern ISXGUIButton* ButtonTBOpen;
	extern ISXGUIButton* ButtonTBSave;

	extern ISXGUICheckBox* CheckBoxTBCam;
	extern ISXGUIButton* ButtonTBView;

	extern ISXGUICheckBox* CheckBoxTBRColor;
	extern ISXGUICheckBox* CheckBoxTBRNormal;
	extern ISXGUICheckBox* CheckBoxTBRParam;
	extern ISXGUICheckBox* CheckBoxTBRAmDiff;
	extern ISXGUICheckBox* CheckBoxTBRSpecular;
	extern ISXGUICheckBox* CheckBoxTBRLighting;

	extern ISXGUIButton* ButtonSkyBox;
	extern ISXGUIEdit* EditSkyBox;
	extern ISXGUICheckBox* CheckBoxModelRot;
	extern ISXGUIButton* ButtonRotAngle0;
	extern ISXGUIStatic* StaticTypeModel;
	extern ISXGUIComboBox* ComboBoxTypeModel;

	extern ISXGUIStatic* StaticPenetration;
	extern ISXGUIEdit* EditHitChance;
	extern ISXGUITrackBar* TrackBarHitChance;
	extern ISXGUIStatic* StaticDurability;
	extern ISXGUIEdit* EditDurability;
	extern ISXGUIStatic* StaticDensity;
	extern ISXGUIEdit* EditDensity;

	extern ISXGUIStatic* StaticTestModel;
	extern ISXGUIComboBox* ComboBoxTestModel;
	extern ISXGUIStatic* StaticTex;
	extern ISXGUIEdit* EditTex;
	extern ISXGUIStatic* StaticVS;
	extern ISXGUIEdit* EditVS;
	extern ISXGUIStatic* StaticPS;
	extern ISXGUIEdit* EditPS;
	extern ISXGUIButton* ButtonTex;

	extern ISXGUIStatic* StaticShaders;
	extern ISXGUIComboBox* ComboBoxShaders;
	
	extern ISXGUIButton* ButtonVS;
	extern ISXGUIButton* ButtonPS;
	extern ISXGUICheckBox* CheckBoxAlphaTest;
	extern ISXGUICheckBox* CheckBoxTransparent;
	extern ISXGUIStatic* StaticMR;
	extern ISXGUIStatic* StaticRoughness;
	extern ISXGUIStatic* StaticF0;
	extern ISXGUIStatic* StaticMG;
	extern ISXGUIEdit* EditRoughness;
	extern ISXGUIEdit* EditF0;
	extern ISXGUIEdit* EditMR;
	extern ISXGUICheckBox* CheckBoxLighting;
	extern ISXGUITrackBar* TrackBarRoughness;
	extern ISXGUITrackBar* TrackBarF0;

	extern ISXGUIStatic* StaticThickness;
	extern ISXGUIEdit* EditThickness;
	extern ISXGUITrackBar* TrackBarThickness;

	extern ISXGUITrackBar* TrackBarUDVSX;
	extern ISXGUIStatic* StaticMaD;
	extern ISXGUIEdit* EditTexLighting;
	extern ISXGUIButton* ButtonTexLighting;
	extern ISXGUICheckBox* CheckBoxTexLighting;

	extern ISXGUIStatic* StaticParamL;
	extern ISXGUIComboBox* ComboBoxParamL;

	extern ISXGUICheckBox* CheckBoxDoSVVS;
	extern ISXGUIStatic* StaticTypeReflect;
	extern ISXGUIComboBox* ComboBoxTypeReflect;
	extern ISXGUIStatic* StaticPhysic;
	extern ISXGUIComboBox* ComboBoxPhysic;
	extern ISXGUIStatic* StaticMask;
	extern ISXGUIEdit* EditMask;
	extern ISXGUIButton* ButtonMask;
	extern ISXGUIButton* ButtonMR;
	extern ISXGUIStatic* StaticMB;
	extern ISXGUIStatic* StaticMA;
	extern ISXGUIStatic* StaticDR;
	extern ISXGUIEdit* EditMG;
	extern ISXGUIEdit* EditMB;
	extern ISXGUIEdit* EditMA;
	extern ISXGUIEdit* EditDR;
	extern ISXGUIButton* ButtonMG;
	extern ISXGUIButton* ButtonMB;
	extern ISXGUIButton* ButtonMA;
	extern ISXGUIButton* ButtonDR;
	extern ISXGUIStatic* StaticDG;
	extern ISXGUIStatic* StaticDB;
	extern ISXGUIStatic* StaticDA;
	extern ISXGUIEdit* EditDG;
	extern ISXGUIEdit* EditDB;
	extern ISXGUIEdit* EditDA;
	extern ISXGUIButton* ButtonDG;
	extern ISXGUIButton* ButtonDB;
	extern ISXGUIButton* ButtonDA;
	extern ISXGUIStatic* StaticDoS;
	extern ISXGUIStatic* StaticDoSW;
	extern ISXGUICheckBox* CheckBoxDoSWVS;
	extern ISXGUICheckBox* CheckBoxDoSWPS;
	extern ISXGUIStatic* StaticDoSV;
	extern ISXGUICheckBox* CheckBoxDoSVPS;
	extern ISXGUIStatic* StaticDosP;
	extern ISXGUICheckBox* CheckBoxDoSPVS;
	extern ISXGUICheckBox* CheckBoxDoSPPS;
	extern ISXGUIStatic* StaticBoxDoSWV;
	extern ISXGUIStatic* StaticDoSWVP;
	extern ISXGUIStatic* StaticDoSCampos;
	extern ISXGUICheckBox* CheckBoxDoSWVVS;
	extern ISXGUICheckBox* CheckBoxDoSWVPS;
	extern ISXGUICheckBox* CheckBoxDoSCamposVS;
	extern ISXGUICheckBox* CheckBoxDoSWVPVS;
	extern ISXGUICheckBox* CheckBoxDoSWVPPS;
	extern ISXGUICheckBox* CheckBoxDoSCamposPS;
	extern ISXGUIStatic* StaticDoSWinSize;
	extern ISXGUICheckBox* CheckBoxDoSWinSizeVS;
	extern ISXGUICheckBox* CheckBoxDoSWinSizePS;

	extern ISXGUICheckBox* CheckBoxUDVS;

	extern ISXGUICheckBox* CheckBoxUDVSInPS;
	extern ISXGUIEdit* EditUDVSX;
	extern ISXGUIEdit* EditUDVSY;
	extern ISXGUITrackBar* TrackBarUDVSY;
	extern ISXGUIStatic* StaticUDVSX;
	extern ISXGUIStatic* StaticUDVSY;
	extern ISXGUIStatic* StaticUDVSZ;
	extern ISXGUIEdit* EditUDVSZ;
	extern ISXGUITrackBar* TrackBarUDVSZ;
	extern ISXGUIStatic* StaticUDVSW;
	extern ISXGUIEdit* EditUDVSW;
	extern ISXGUITrackBar* TrackBarUDVSW;
	extern ISXGUIStatic* StaticDoSTimeDelta;
	extern ISXGUICheckBox* CheckBoxDoSTimeDeltaVS;
	extern ISXGUICheckBox* CheckBoxDoSTimeDeltaPS;

	extern ISXGUICheckBox* CheckBoxUDPS;

	extern ISXGUICheckBox* CheckBoxUDPSInVS;
	extern ISXGUIStatic* StaticUDPSX;
	extern ISXGUIStatic* StaticUDPSY;
	extern ISXGUIStatic* StaticUDPSZ;
	extern ISXGUIStatic* StaticUDPSW;
	extern ISXGUIEdit* EditUDPSX;
	extern ISXGUIEdit* EditUDPSY;
	extern ISXGUITrackBar* TrackBarUDPSX;
	extern ISXGUITrackBar* TrackBarUDPSY;
	extern ISXGUIEdit* EditUDPSZ;
	extern ISXGUITrackBar* TrackBarUDPSZ;
	extern ISXGUIEdit* EditUDPSW;
	extern ISXGUITrackBar* TrackBarUDPSW;

	void InitAllElements();

	void DeleteAllElements();

	void Nulling();
	void InitMtl(ID id);

	void MtlOpen();
	void MtlSave();

	void FinalImageUncheckedMenu();

	void MaterialEditorUpdate(DWORD timeDelta);

	extern ID IDMat;

	extern ShadersKit* Shaders;
	extern ParamLKit* ParamL;
};



void SXMaterialEditor::InitAllElements();
void SXMaterialEditor::DeleteAllElements();

#endif