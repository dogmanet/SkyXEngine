
#ifndef __MATERIAL_EDITOR_H
#define __MATERIAL_EDITOR_H

#define MAINWIN_SIZE_X	645
#define MAINWIN_SIZE_Y	700

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <light/sxlight.h>
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
	extern ISXGUIBaseWnd *pJobWindow;
	extern ISXGUIMenuWindow *pMainMenu;
	extern ISXGUIBaseWnd *pWindowRender;

	extern ISXGUIToolBar *pToolBar;
	extern ISXGUIButton *pButtonTBOpen;
	extern ISXGUIButton *pButtonTBSave;

	extern ISXGUICheckBox *pCheckBoxTBCam;
	extern ISXGUIButton *pButtonTBView;

	extern ISXGUICheckBox *pCheckBoxTBRColor;
	extern ISXGUICheckBox *pCheckBoxTBRNormal;
	extern ISXGUICheckBox *pCheckBoxTBRParam;
	extern ISXGUICheckBox *pCheckBoxTBRAmDiff;
	extern ISXGUICheckBox *pCheckBoxTBRSpecular;
	extern ISXGUICheckBox *pCheckBoxTBRLighting;

	extern ISXGUIButton *pButtonSkyBox;
	extern ISXGUIEdit *pEditSkyBox;
	extern ISXGUICheckBox *pCheckBoxModelRot;
	extern ISXGUIButton *pButtonRotAngle0;
	extern ISXGUIStatic *pStaticTypeModel;
	extern ISXGUIComboBox *pComboBoxTypeModel;

	extern ISXGUIStatic *pStaticLigthColorText;
	extern ISXGUIStatic *pStaticLigthColorR;
	extern ISXGUIEdit *pEditLigthColorR;
	extern ISXGUIStatic *pStaticLigthColorG;
	extern ISXGUIEdit *pEditLigthColorG;
	extern ISXGUIStatic *pStaticLigthColorB;
	extern ISXGUIEdit *pEditLigthColorB;
	extern ISXGUIButton *pButtonLigthColor;
	extern ISXGUIStatic *pStaticLigthColor;

	extern ISXGUIStatic *pStaticPenetration;
	extern ISXGUIEdit *pEditHitChance;
	extern ISXGUITrackBar *pTrackBarHitChance;
	extern ISXGUIStatic *pStaticDurability;
	extern ISXGUIEdit *pEditDurability;
	extern ISXGUIStatic *pStaticDensity;
	extern ISXGUIEdit *pEditDensity;

	extern ISXGUIStatic *pStaticTestModel;
	extern ISXGUIComboBox *pComboBoxTestModel;
	extern ISXGUIStatic *pStaticTex;
	extern ISXGUIEdit *pEditTex;
	extern ISXGUIStatic *pStaticVS;
	extern ISXGUIEdit *pEditVS;
	extern ISXGUIStatic *pStaticPS;
	extern ISXGUIEdit *pEditPS;
	extern ISXGUIButton *pButtonTex;

	extern ISXGUIStatic *pStaticShaders;
	extern ISXGUIComboBox *pComboBoxShaders;
	
	extern ISXGUIButton *pButtonVS;
	extern ISXGUIButton *pButtonPS;
	extern ISXGUICheckBox *pCheckBoxAlphaTest;
	extern ISXGUICheckBox *pCheckBoxTransparent;
	extern ISXGUIStatic *pStaticMR;
	extern ISXGUIStatic *pStaticRoughness;
	extern ISXGUIStatic *pStaticF0;
	extern ISXGUIStatic *pStaticMG;
	extern ISXGUIEdit *pEditRoughness;
	extern ISXGUIEdit *pEditF0;
	extern ISXGUIEdit *pEditMR;
	extern ISXGUICheckBox *pCheckBoxLighting;
	extern ISXGUITrackBar *pTrackBarRoughness;
	extern ISXGUITrackBar *pTrackBarF0;

	extern ISXGUIStatic *pStaticThickness;
	extern ISXGUIEdit *pEditThickness;
	extern ISXGUITrackBar *pTrackBarThickness;

	extern ISXGUITrackBar *pTrackBarUDVSX;
	extern ISXGUIStatic *pStaticMaD;
	extern ISXGUIEdit *pEditTexLighting;
	extern ISXGUIButton *pButtonTexLighting;
	extern ISXGUICheckBox *pCheckBoxTexLighting;

	extern ISXGUIStatic *pStaticParamL;
	extern ISXGUIComboBox *pComboBoxParamL;

	extern ISXGUICheckBox *pCheckBoxDoSVVS;
	extern ISXGUIStatic *pStaticTypeReflect;
	extern ISXGUIComboBox *pComboBoxTypeReflect;
	extern ISXGUIStatic *pStaticPhysic;
	extern ISXGUIComboBox *pComboBoxPhysic;
	extern ISXGUIStatic *pStaticMask;
	extern ISXGUIEdit *pEditMask;
	extern ISXGUIButton *pButtonMask;
	extern ISXGUIButton *pButtonMR;
	extern ISXGUIStatic *pStaticMB;
	extern ISXGUIStatic *pStaticMA;
	extern ISXGUIStatic *pStaticDR;
	extern ISXGUIEdit *pEditMG;
	extern ISXGUIEdit *pEditMB;
	extern ISXGUIEdit *pEditMA;
	extern ISXGUIEdit *pEditDR;
	extern ISXGUIButton *pButtonMG;
	extern ISXGUIButton *pButtonMB;
	extern ISXGUIButton *pButtonMA;
	extern ISXGUIButton *pButtonDR;
	extern ISXGUIStatic *pStaticDG;
	extern ISXGUIStatic *pStaticDB;
	extern ISXGUIStatic *pStaticDA;
	extern ISXGUIEdit *pEditDG;
	extern ISXGUIEdit *pEditDB;
	extern ISXGUIEdit *pEditDA;
	extern ISXGUIButton *pButtonDG;
	extern ISXGUIButton *pButtonDB;
	extern ISXGUIButton *pButtonDA;
	extern ISXGUIStatic *pStaticDoS;
	extern ISXGUIStatic *pStaticDoSW;
	extern ISXGUICheckBox *pCheckBoxDoSWVS;
	extern ISXGUICheckBox *pCheckBoxDoSWPS;
	extern ISXGUIStatic *pStaticDoSV;
	extern ISXGUICheckBox *pCheckBoxDoSVPS;
	extern ISXGUIStatic *pStaticDosP;
	extern ISXGUICheckBox *pCheckBoxDoSPVS;
	extern ISXGUICheckBox *pCheckBoxDoSPPS;
	extern ISXGUIStatic *pStaticBoxDoSWV;
	extern ISXGUIStatic *pStaticDoSWVP;
	extern ISXGUIStatic *pStaticDoSCampos;
	extern ISXGUICheckBox *pCheckBoxDoSWVVS;
	extern ISXGUICheckBox *pCheckBoxDoSWVPS;
	extern ISXGUICheckBox *pCheckBoxDoSCamposVS;
	extern ISXGUICheckBox *pCheckBoxDoSWVPVS;
	extern ISXGUICheckBox *pCheckBoxDoSWVPPS;
	extern ISXGUICheckBox *pCheckBoxDoSCamposPS;
	extern ISXGUIStatic *pStaticDoSWinSize;
	extern ISXGUICheckBox *pCheckBoxDoSWinSizeVS;
	extern ISXGUICheckBox *pCheckBoxDoSWinSizePS;

	extern ISXGUICheckBox *pCheckBoxUDVS;

	extern ISXGUICheckBox *pCheckBoxUDVSInPS;
	extern ISXGUIEdit *pEditUDVSX;
	extern ISXGUIEdit *pEditUDVSY;
	extern ISXGUITrackBar *pTrackBarUDVSY;
	extern ISXGUIStatic *pStaticUDVSX;
	extern ISXGUIStatic *pStaticUDVSY;
	extern ISXGUIStatic *pStaticUDVSZ;
	extern ISXGUIEdit *pEditUDVSZ;
	extern ISXGUITrackBar *pTrackBarUDVSZ;
	extern ISXGUIStatic *pStaticUDVSW;
	extern ISXGUIEdit *pEditUDVSW;
	extern ISXGUITrackBar *pTrackBarUDVSW;
	extern ISXGUIStatic *pStaticDoSTimeDelta;
	extern ISXGUICheckBox *pCheckBoxDoSTimeDeltaVS;
	extern ISXGUICheckBox *pCheckBoxDoSTimeDeltaPS;

	extern ISXGUICheckBox *pCheckBoxUDPS;

	extern ISXGUICheckBox *pCheckBoxUDPSInVS;
	extern ISXGUIStatic *pStaticUDPSX;
	extern ISXGUIStatic *pStaticUDPSY;
	extern ISXGUIStatic *pStaticUDPSZ;
	extern ISXGUIStatic *pStaticUDPSW;
	extern ISXGUIEdit *pEditUDPSX;
	extern ISXGUIEdit *pEditUDPSY;
	extern ISXGUITrackBar *pTrackBarUDPSX;
	extern ISXGUITrackBar *pTrackBarUDPSY;
	extern ISXGUIEdit *pEditUDPSZ;
	extern ISXGUITrackBar *pTrackBarUDPSZ;
	extern ISXGUIEdit *pEditUDPSW;
	extern ISXGUITrackBar *pTrackBarUDPSW;

	void InitAllElements();

	void DeleteAllElements();

	void Nulling();
	void InitMtl(ID id);

	void MtlOpen();
	void MtlSave();

	void FinalImageUncheckedMenu();

	void MaterialEditorUpdate(DWORD timeDelta);

	extern ID idMat;

	extern CShadersKit *pShaders;
	extern CParamLigthKit *pParamL;
};



void SXMaterialEditor::InitAllElements();
void SXMaterialEditor::DeleteAllElements();

#endif