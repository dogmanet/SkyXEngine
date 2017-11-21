
#ifndef __PARTICLES_EDITOR_H
#define __PARTICLES_EDITOR_H

#include <sxguiwinapi/sxgui.h>
#include <sxparticleseditor/resource.h>

#include <common/string.h>

#include <particles/sxparticles.h>
#include <level/sxlevel.h>
#include <render/sxrender.h>

#include "callback_common.h"
#include "callback_tabs.h"

#define MAINWIN_SIZE_X	820
#define MAINWIN_SIZE_Y	675

namespace SXParticlesEditor
{
	extern ISXGUIBaseWnd* JobWindow;
	extern ISXGUIMenu* MainMenu;
	extern ISXGUIStatusBar* StatusBar1;
	extern ISXGUIGroupBox* GroupBoxList;
	extern ISXGUIGroupBox* GroupBoxTabs;
	extern ISXGUIGroupBox* GroupBoxData;
	extern ISXGUIGroupBox* GroupBoxData2;
	extern ISXGUIBaseWnd* WindowRender;

	extern ISXGUIToolBar* ToolBar1;

	extern ISXGUIButton* ButtonTBReOpen;
	extern ISXGUIButton* ButtonTBSave;
	extern ISXGUICheckBox* CheckBoxTBGrid;
	extern ISXGUICheckBox* CheckBoxTBAxes;
	extern ISXGUICheckBox* CheckBoxTBBound;
	extern ISXGUICheckBox* CheckBoxTBPlay;
	extern ISXGUICheckBox* CheckBoxTBPause;
	extern ISXGUICheckBox* CheckBoxTBStop;

	extern ISXGUIListBox* ListBoxEffects;
	extern ISXGUIListBox* ListBoxEmitters;
	extern ISXGUIStatic* StaticEffects;
	extern ISXGUIButton* ButtonEffectsCreate;
	extern ISXGUIStatic* StaticEmitters;
	//ISXGUIButton* ButtonEffectsEdit;
	extern ISXGUIButton* ButtonEffectsDelete;
	extern ISXGUIButton* ButtonEmittersCreate;
	extern ISXGUIButton* ButtonEmittersBasis;
	extern ISXGUIButton* ButtonEmittersDelete;

	extern ISXGUIRadioButton* ButtonBase;
	extern ISXGUIRadioButton* ButtonTextureAnimTex;
	extern ISXGUIRadioButton* ButtonSpawn;
	extern ISXGUIRadioButton* ButtonBound;
	extern ISXGUIRadioButton* ButtonCharacters;
	extern ISXGUIRadioButton* ButtonVelocityAcceleration;
	extern ISXGUIStatic* StaticEffectsCount;
	extern ISXGUIStatic* StaticEmittersCount;

	//effect
	//{
	extern ISXGUIStatic* StaticEffName;
	extern ISXGUIEdit* EditEffName;
	extern ISXGUIButton* ButtonEffCreate;
	//}

	//base
	//{
	extern ISXGUIStatic* StaticName;
	extern ISXGUIEdit* EditName;
	extern ISXGUIButton* ButtonEmitterCreate;
	extern ISXGUIStatic* StaticReCreateCount;
	extern ISXGUIEdit* EditReCreateCount;
	extern ISXGUIStatic* StaticCount;
	extern ISXGUIEdit* EditCount;
	extern ISXGUIStatic* StaticColorCoef;
	extern ISXGUIEdit* EditColorCoef;
	extern ISXGUIStatic* StaticSoftCoef;
	extern ISXGUIEdit* EditSoftCoef;
	extern ISXGUIStatic* StaticRefractionCoef;
	extern ISXGUIEdit* EditRefractionCoef;
	extern ISXGUIStatic* StaticTransparencyCoef;
	extern ISXGUIEdit* EditTransparencyCoef;
	extern ISXGUICheckBox* CheckBoxLighting;
	extern ISXGUIStatic* StaticFigureType;
	extern ISXGUIComboBox* ComboBoxFigureType;
	extern ISXGUIStatic* StaticFigureCountQuads;
	extern ISXGUIEdit* EditFigureCountQuads;
	extern ISXGUICheckBox* CheckBoxFigureRotRand;
	extern ISXGUICheckBox* CheckBoxFigureTapX;
	extern ISXGUICheckBox* CheckBoxFigureTapY;
	extern ISXGUICheckBox* CheckBoxFigureTapZ;
	extern ISXGUIStatic* StaticAlphaBlendType;
	extern ISXGUIComboBox* ComboBoxAlphaBlendType;

	extern ISXGUIStatic* StaticColor;
	extern ISXGUIStatic* StaticColorR;
	extern ISXGUIEdit* EditColorR;
	extern ISXGUIStatic* StaticColorG;
	extern ISXGUIEdit* EditColorG;
	extern ISXGUIStatic* StaticColorB;
	extern ISXGUIEdit* EditColorB;
	extern ISXGUIStatic* StaticColorA;
	extern ISXGUIEdit* EditColorA;

	extern ISXGUIStatic* StaticTimeLife;
	extern ISXGUIEdit* EditTimeLife;
	extern ISXGUIStatic* StaticTimeLifeDisp;
	extern ISXGUIEdit* EditTimeLifeDisp;
	extern ISXGUIStatic* StaticAlphaAgeDepend;
	extern ISXGUIComboBox* ComboBoxAlphaDependAge;
	extern ISXGUIStatic* StaticSize;
	extern ISXGUIStatic* StaticSizeX;
	extern ISXGUIEdit* EditSizeX;
	extern ISXGUIStatic* StaticSizeY;
	extern ISXGUIEdit* EditSizeY;
	extern ISXGUIStatic* StaticSizeDisp;
	extern ISXGUIEdit* EditSizeDisp;
	extern ISXGUIStatic* StaticSizeDependAge;
	extern ISXGUIComboBox* ComboBoxSizeDependAge;
	extern ISXGUICheckBox* CheckBoxCollisionDelete;

	extern ISXGUICheckBox* CheckBoxTrack;
	extern ISXGUIStatic* StaticTrackSize;
	extern ISXGUIEdit* EditTrackSize;
	extern ISXGUIStatic* StaticTrackTime;
	extern ISXGUIEdit* EditTrackTime;

	//}

	//bound
	//{
	extern ISXGUIStatic* StaticBoundType;
	extern ISXGUIComboBox* ComboBoxBoundType;
	extern ISXGUIStatic* StaticBoundVec1;
	extern ISXGUIStatic* StaticBoundVec1X;
	extern ISXGUIEdit* EditBoundVec1X;
	extern ISXGUIStatic* StaticBoundVec1Y;
	extern ISXGUIEdit* EditBoundVec1Y;
	extern ISXGUIStatic* StaticBoundVec1Z;
	extern ISXGUIEdit* EditBoundVec1Z;
	extern ISXGUIStatic* StaticBoundVec1W;
	extern ISXGUIEdit* EditBoundVec1W;
	extern ISXGUIStatic* StaticBoundVec2;
	extern ISXGUIStatic* StaticBoundVec2X;
	extern ISXGUIEdit* EditBoundVec2X;
	extern ISXGUIStatic* StaticBoundVec2Y;
	extern ISXGUIEdit* EditBoundVec2Y;
	extern ISXGUIStatic* StaticBoundVec2Z;
	extern ISXGUIEdit* EditBoundVec2Z;
	extern ISXGUIStatic* StaticBoundVec2W;
	extern ISXGUIEdit* EditBoundVec2W;
	//}

	//character
	//{
	extern ISXGUICheckBox* CheckBoxCircle;
	extern ISXGUIStatic* StaticCircleAxis;
	extern ISXGUIComboBox* ComboBoxCircleAxis;
	extern ISXGUIStatic* StaticCircleAngle;
	extern ISXGUIEdit* EditCircleAngle;
	extern ISXGUIStatic* StaticCircleAngleDisp;
	extern ISXGUIEdit* EditCircleAngleDisp;
	extern ISXGUICheckBox* CheckBoxCircleAngleDispNeg;
	extern ISXGUICheckBox* CheckBoxRotate;
	extern ISXGUIStatic* StaticRotateAngle;
	extern ISXGUIEdit* EditRotateAngle;
	extern ISXGUIStatic* StaticRotateAngleDisp;
	extern ISXGUIEdit* EditRotateAngleDisp;
	extern ISXGUICheckBox* CheckBoxRotateAngleDispNeg;
	extern ISXGUICheckBox* CheckBoxDeviation;
	extern ISXGUIStatic* StaticDeviationType;
	extern ISXGUIComboBox* ComboBoxDeviationType;
	extern ISXGUIStatic* StaticDeviationAmplitude;
	extern ISXGUIStatic* StaticDeviationCoefAngle;
	extern ISXGUIEdit* EditDeviationAmplitude;
	extern ISXGUIEdit* EditDeviationCoefAngle;
	extern ISXGUIStatic* StaticDeviationAxis;
	extern ISXGUIComboBox* ComboBoxDeviationAxis;
	extern ISXGUIStatic* StaticDeviationCountDelayMls;
	extern ISXGUIEdit* EditDeviationCountDelayMls;
	extern ISXGUIStatic* StaticDeviationCoefAngleDisp;
	extern ISXGUIEdit* EditDeviationCoefAngleDisp;
	extern ISXGUICheckBox* CheckBoxDeviationCoefAngleDispNeg;
	extern ISXGUICheckBox* CheckBoxDeviationTapX;
	extern ISXGUICheckBox* CheckBoxDeviationTapY;
	extern ISXGUICheckBox* CheckBoxDeviationTapZ;
	//}

	//spawn
	//{
	extern ISXGUIStatic* StaticSpawnPosType;
	extern ISXGUIComboBox* ComboBoxSpawnPosType;
	extern ISXGUIStatic* StaticSpawnOrigin;
	extern ISXGUIStatic* StaticSpawnOriginX;
	extern ISXGUIEdit* EditSpawnOriginX;
	extern ISXGUIStatic* StaticSpawnOriginY;
	extern ISXGUIEdit* EditSpawnOriginY;
	extern ISXGUIStatic* StaticSpawnOriginZ;
	extern ISXGUIEdit* EditSpawnOriginZ;
	extern ISXGUIStatic* StaticSpawnOriginDisp;
	extern ISXGUIEdit* EditSpawnOriginDisp;
	extern ISXGUICheckBox* CheckBoxSpawnOriginDispXPos;
	extern ISXGUICheckBox* CheckBoxSpawnOriginDispXNeg;
	extern ISXGUICheckBox* CheckBoxSpawnOriginDispYPos;
	extern ISXGUICheckBox* CheckBoxSpawnOriginDispYNeg;
	extern ISXGUICheckBox* CheckBoxSpawnOriginDispZPos;
	extern ISXGUICheckBox* CheckBoxSpawnOriginDispZNeg;
	extern ISXGUIStatic* StaticSpawnNextTime;
	extern ISXGUIEdit* EditSpawnNextTime;
	extern ISXGUIStatic* StaticSpawnNextTimeDisp;
	extern ISXGUIEdit* EditSpawnNextTimeDisp;
	//}

	//tex
	//{
	extern ISXGUIStatic* StaticTexture;
	extern ISXGUIEdit* EditTexture;
	extern ISXGUIButton* ButtonTextureSel;
	extern ISXGUIStatic* StaticAnimTexCountCadrsX;
	extern ISXGUIStatic* StaticAnimTexCountCadrsY;
	extern ISXGUIEdit* EditAnimTexCountCadrsX;
	extern ISXGUIEdit* EditAnimTexCountCadrsY;
	extern ISXGUIStatic* StaticAnimTexRate;
	extern ISXGUIEdit* EditAnimTexRate;
	extern ISXGUIStatic* StaticAnimTexRateDisp;
	extern ISXGUIEdit* EditAnimTexRateDisp;
	extern ISXGUIStatic* StaticAnimTexStartCadr;
	extern ISXGUIEdit* EditAnimTexStartCadr;
	extern ISXGUIStatic* StaticAnimTexStartCadrDisp;
	extern ISXGUIEdit* EditAnimTexStartCadrDisp;
	//}

	//velocity/acceleration
	//{
	extern ISXGUIStatic* StaticVelocity;
	extern ISXGUIStatic* StaticVelocityX;
	extern ISXGUIEdit* EditVelocityX;
	extern ISXGUIStatic* StaticVelocityY;
	extern ISXGUIEdit* EditVelocityY;
	extern ISXGUIStatic* StaticVelocityZ;
	extern ISXGUIEdit* EditVelocityZ;
	extern ISXGUIStatic* StaticVelocityDisp;
	extern ISXGUIEdit* EditVelocityDisp;
	extern ISXGUICheckBox* CheckBoxVelocityDispXNeg;
	extern ISXGUICheckBox* CheckBoxVelocityDispYNeg;
	extern ISXGUICheckBox* CheckBoxVelocityDispZNeg;
	extern ISXGUIStatic* StaticAcceleration;
	extern ISXGUIStatic* StaticAccelerationX;
	extern ISXGUIEdit* EditAccelerationX;
	extern ISXGUIStatic* StaticAccelerationY;
	extern ISXGUIEdit* EditAccelerationY;
	extern ISXGUIStatic* StaticAccelerationZ;
	extern ISXGUIEdit* EditAccelerationZ;
	extern ISXGUIStatic* StaticAccelerationDisp;
	extern ISXGUIEdit* EditAccelerationDisp;
	extern ISXGUICheckBox* CheckBoxAccelerationDispXNeg;
	extern ISXGUICheckBox* CheckBoxAccelerationDispYNeg;
	extern ISXGUICheckBox* CheckBoxAccelerationDispZNeg;
	//}

	void InitAllElements();

	void UpdateCountInfo();

	void EffReOpen();
	void EffSave();

	void EffInitList();

	void EffVisible(bool visible, bool iscreate);
	void EffNulling();
	void EffDataInit();

	void TabsVisible(bool visible);

	void AllInTabsVisible(bool visible);
	void AllInTabsNulling();

	void EmitterCreateVisible(bool visible);

	void BaseVisible(bool visible);
	void BaseDataInit();
	void BaseNulling();

	void BoundVisible(bool visible);
	void BoundDataInit();
	void BoundNulling();

	void CharacterVisible(bool visible);
	void CharacterDataInit();
	void CharacterNulling();

	void SpawnVisible(bool visible);
	void SpawnDataInit();
	void SpawnNulling();

	void TexVisible(bool visible);
	void TexDataInit();
	void TexNulling();

	void VelocityAccVisible(bool visible);
	void VelocityDataInit();
	void VelocityAccNulling();

	void DeleteAllElements();

	void ParticlesEditorUpdate(DWORD timeDelta);


	void PEcreateData();
	void PEdeleteData();

	extern bool canRenderBound;

	extern ID3DXMesh* FigureBox;		//!< бокс для определения ограничивающего объема для эммитера партиклов
	extern ID3DXMesh* FigureSphere;		//!< сфера для определения ограничивающего объема для эммитера партиклов
	extern ID3DXMesh* FigureCone;		//!< конус для определения ограничивающего объема для эммитера партиклов
	extern float3_t FigureConeParam;	//!< параметры для FigureCone (x - верхний радиус, y - нижний радиус, z - высота)
	extern float3 FigureConePos;		//!< позиция для FigureCone

	extern ID SelEffID;
	extern ID SelEmitterID;
};

#endif 