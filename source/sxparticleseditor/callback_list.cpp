
#include "callback_list.h"

LRESULT SXParticlesEditor_ListBoxEffects_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXParticlesEditor::ListBoxEffects->getSel();
	if (sel < 0)
		return 0;

	SXParticlesEditor::ListBoxEmitters->clear();
	if (SPE_EffectGetCount() == 0)
	{
		SXParticlesEditor::ListBoxEffects->clear();
		SXParticlesEditor::EffVisible(true, true);
		SXParticlesEditor::TabsVisible(false);
		return 0;
	}
	
	SXParticlesEditor::SelEffID = SPE_EffectGetIdOfKey(sel);
	int partcount = SPE_EmitterGetSCount(SXParticlesEditor::SelEffID);
	char partname[OBJECT_NAME_MAX_LEN];
	for (int i = 0; i < partcount; ++i)
	{
		partname[0] = '!';
		partname[1] = 0;
		SPE_EmitterGetName(SXParticlesEditor::SelEffID, i, partname);
		if (partname[0] == 0)
			partname[0] = '!';
		SXParticlesEditor::ListBoxEmitters->addItem(partname);
	}

	SXParticlesEditor::StaticEmittersCount->setText(String(partcount).c_str());

	if (hwnd != 0)
		SXParticlesEditor::TabsVisible(false);

	for (int i = 0; i < SPE_EffectGetCount(); ++i)
	{
		SPE_EffectSetEnable(SPE_EffectGetIdOfKey(i), false);
	}

	SPE_EffectSetEnable(SXParticlesEditor::SelEffID, true);

	if (hwnd != 0)
	{
		SXParticlesEditor::EffDataInit();
		SXParticlesEditor::EffVisible(true, false);
		SXParticlesEditor::EmitterCreateVisible(false);
	}

	SXParticlesEditor::SelEmitterID = -1;

	return 0;
}

LRESULT SXParticlesEditor_ListBoxEmitters_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXParticlesEditor::SelEffID < 0)
		return 0;

	SXParticlesEditor::SelEmitterID = SXParticlesEditor::ListBoxEmitters->getSel();
	int countemitters = SPE_EmitterGetSCount(SXParticlesEditor::SelEffID);
	if (SXParticlesEditor::SelEmitterID < 0 || countemitters <= 0 || SPE_EmitterGetSCount(SXParticlesEditor::SelEffID) <= SXParticlesEditor::SelEmitterID)
		return 0;

	SPE_EffectSetEnable(SXParticlesEditor::SelEffID, true);

	for (int i = 0; i < SPE_EmitterGetSCount(SXParticlesEditor::SelEffID); ++i)
	{
		SPE_EmitterSetEnable(SXParticlesEditor::SelEffID, i, false);
	}
	
	SPE_EmitterSetEnable(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, true);
	
	SXParticlesEditor::EffVisible(false, false);
	SXParticlesEditor::AllInTabsVisible(false);
	SXParticlesEditor::AllInTabsNulling();
	SXParticlesEditor::TabsVisible(true);
	SXParticlesEditor::BaseVisible(true);
	SXParticlesEditor::BaseDataInit();
	SXParticlesEditor::BoundDataInit();
	SXParticlesEditor::CharacterDataInit();
	SXParticlesEditor::SpawnDataInit();
	SXParticlesEditor::TexDataInit();
	SXParticlesEditor::VelocityDataInit();
	SXParticlesEditor::EmitterCreateVisible(false);
	return 0;
}


LRESULT SXParticlesEditor_ButtonEffectsCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXParticlesEditor::EffNulling();
	SXParticlesEditor::EffVisible(true, true);

	SXParticlesEditor::TabsVisible(false);

	for (int i = 0; i < SPE_EffectGetCount(); ++i)
	{
		SPE_EffectSetEnable(SPE_EffectGetIdOfKey(i), false);
	}

	return 0;
}

LRESULT SXParticlesEditor_ButtonEffectsDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return 0;
	}

	SPE_EffectDelete(SXParticlesEditor::SelEffID);

	SXParticlesEditor::SelEffID = -1;
	SXParticlesEditor::ListBoxEmitters->clear();
	SXParticlesEditor::StaticEmittersCount->setText("0");

	int sel = SXParticlesEditor::ListBoxEffects->getSel();
	SXParticlesEditor::ListBoxEffects->deleteItem(SXParticlesEditor::ListBoxEffects->getSel());
	SXParticlesEditor::StaticEffectsCount->setText(String(SPE_EffectGetCount()).c_str());

	if (SPE_EffectGetCount() > 0)
	{
		SXParticlesEditor::ListBoxEffects->setSel((sel < SXParticlesEditor::ListBoxEffects->getItemCount() ? sel : sel - 1));
		SXParticlesEditor_ListBoxEffects_Click(hwnd, msg, wParam, lParam);
	}
	else
		SXParticlesEditor_ButtonEffectsCreate_Click(hwnd, msg, wParam, lParam);

	if (SXParticlesEditor::SelEffID >= 0)
		SPE_EffectSetEnable(SXParticlesEditor::SelEffID, false);

	return 0;
}

LRESULT SXParticlesEditor_ButtonEmittersCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXParticlesEditor::SelEffID < 0)
		return 0;

	SXParticlesEditor::EffVisible(false, false);

	SXParticlesEditor::AllInTabsVisible(false);
	SXParticlesEditor::AllInTabsNulling();
	SXParticlesEditor::TabsVisible(true);
	SXParticlesEditor::EmitterCreateVisible(true);
	SXParticlesEditor::BaseVisible(true);

	for (int i = 0; i < SPE_EmitterGetSCount(SXParticlesEditor::SelEffID); ++i)
	{
		SPE_EmitterSetEnable(SXParticlesEditor::SelEffID, i, false);
	}

	SXParticlesEditor::SelEmitterID = -1;

	return 0;
}

LRESULT SXParticlesEditor_ButtonEmittersBasis_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXParticlesEditor::EffVisible(false, false);

	SXParticlesEditor::AllInTabsVisible(false);
	//SXParticlesEditor::AllInTabsNulling();
	SXParticlesEditor::TabsVisible(true);
	SXParticlesEditor::EmitterCreateVisible(true);
	SXParticlesEditor::BaseVisible(true);

	for (int i = 0; i < SPE_EmitterGetSCount(SXParticlesEditor::SelEffID); ++i)
	{
		SPE_EmitterSetEnable(SXParticlesEditor::SelEffID, i, false);
	}

	SXParticlesEditor::SelEmitterID = -1;

	return 0;
}

LRESULT SXParticlesEditor_ButtonEmittersDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXParticlesEditor::SelEmitterID < 0 || SXParticlesEditor::SelEmitterID < 0)
	{
		return 0;
	}

	SPE_EmitterDelete(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
	SXParticlesEditor::SelEmitterID = -1;
	int sel = SXParticlesEditor::ListBoxEmitters->getSel();
	SXParticlesEditor::ListBoxEmitters->deleteItem(sel);
	SXParticlesEditor::StaticEmittersCount->setText(String(SPE_EmitterGetSCount(SXParticlesEditor::SelEffID)).c_str());

	if (SPE_EmitterGetSCount(SXParticlesEditor::SelEffID))
	{
		SXParticlesEditor::ListBoxEmitters->setSel((sel < SXParticlesEditor::ListBoxEmitters->getItemCount() ? sel : sel - 1));
		SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);
	}
	else
		SXParticlesEditor_ButtonEmittersCreate_Click(hwnd, msg, wParam, lParam);

	if (SXParticlesEditor::SelEffID >= 0)
		SPE_EffectSetEnable(SXParticlesEditor::SelEffID, false);

	return 0;
}

///

LRESULT SXParticlesEditor_ButtonEffCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < SPE_EffectGetCount(); ++i)
	{
		SPE_EffectSetEnable(SPE_EffectGetIdOfKey(i), false);
	}

	char effname[OBJECT_NAME_MAX_LEN];
	SXParticlesEditor::EditEffName->getText(effname, OBJECT_NAME_MAX_LEN);

	SXParticlesEditor::SelEffID = SPE_EffectAdd(effname);

	SXParticlesEditor::ListBoxEffects->addItem(effname);
	SXParticlesEditor::ListBoxEffects->setSel(SXParticlesEditor::ListBoxEffects->getItemCount() - 1);
	SXParticlesEditor::StaticEffectsCount->setText(String(SPE_EffectGetCount()).c_str());
	SXParticlesEditor::EffVisible(true, false);

	return 0;
}

///////////

LRESULT SXParticlesEditor_ButtonEmitterCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char partname[OBJECT_NAME_MAX_LEN];
	SXParticlesEditor::EditName->getText(partname, OBJECT_NAME_MAX_LEN);

	if (!STR_VALIDATE(partname))
	{
		MessageBox(0,"unresolved name for particles",0,0);
		return 0;
	}

	if (SXParticlesEditor::SelEffID == -1)
	{
		MessageBox(0, "it is necessary to select effect", 0, 0);
		return 0;
	}

	char tmptex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	SXParticlesEditor::EditTexture->getText(tmptex, SXGC_LOADTEX_MAX_SIZE_DIRNAME);


	char tmptxt[64];

	CParticlesData pdata;

	SXParticlesEditor::EditAccelerationX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vAcceleration.x));
	SXParticlesEditor::EditAccelerationY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vAcceleration.y));
	SXParticlesEditor::EditAccelerationZ->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vAcceleration.z));

	SXParticlesEditor::EditAccelerationDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fAccelerationDisp));

	pdata.m_shouldAccelerationDispXNeg = SXParticlesEditor::CheckBoxAccelerationDispXNeg->getCheck();
	pdata.m_shouldAccelerationDispYNeg = SXParticlesEditor::CheckBoxAccelerationDispYNeg->getCheck();
	pdata.m_shouldAccelerationDispZNeg = SXParticlesEditor::CheckBoxAccelerationDispZNeg->getCheck();


	SXParticlesEditor::EditVelocityX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vVelocity.x));
	SXParticlesEditor::EditVelocityY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vVelocity.y));
	SXParticlesEditor::EditVelocityZ->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vVelocity.z));

	SXParticlesEditor::EditVelocityDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fVelocityDisp));

	pdata.m_shouldVelocityDispXNeg = SXParticlesEditor::CheckBoxVelocityDispXNeg->getCheck();
	pdata.m_shouldVelocityDispYNeg = SXParticlesEditor::CheckBoxVelocityDispYNeg->getCheck();
	pdata.m_shouldVelocityDispZNeg = SXParticlesEditor::CheckBoxVelocityDispZNeg->getCheck();


	pdata.m_typeBound = (PARTICLESTYPE_BOUND)SXParticlesEditor::ComboBoxBoundType->getSel();

	SXParticlesEditor::EditBoundVec1X->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vBoundVec1.x));
	SXParticlesEditor::EditBoundVec1Y->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vBoundVec1.y));
	SXParticlesEditor::EditBoundVec1Z->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vBoundVec1.z));

	SXParticlesEditor::EditBoundVec2X->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vBoundVec2.x));
	SXParticlesEditor::EditBoundVec2Y->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vBoundVec2.y));
	SXParticlesEditor::EditBoundVec2Z->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vBoundVec2.z));


	pdata.m_typeSpawnPos = (PARTICLESTYPE_SPAWNPOS)SXParticlesEditor::ComboBoxSpawnPosType->getSel();
	SXParticlesEditor::EditSpawnOriginX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vSpawnOrigin.x));
	SXParticlesEditor::EditSpawnOriginY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vSpawnOrigin.y));
	SXParticlesEditor::EditSpawnOriginZ->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vSpawnOrigin.z));

	SXParticlesEditor::EditSpawnOriginDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fSpawnOriginDisp));

	pdata.m_shouldSpawnBoundBindCreateXNeg = SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->getCheck();
	pdata.m_shouldSpawnBoundBindCreateXPos = SXParticlesEditor::CheckBoxSpawnOriginDispXPos->getCheck();
	pdata.m_shouldSpawnBoundBindCreateYNeg = SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->getCheck();
	pdata.m_shouldSpawnBoundBindCreateYPos = SXParticlesEditor::CheckBoxSpawnOriginDispYPos->getCheck();
	pdata.m_shouldSpawnBoundBindCreateZNeg = SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->getCheck();
	pdata.m_shouldSpawnBoundBindCreateZPos = SXParticlesEditor::CheckBoxSpawnOriginDispZPos->getCheck();

	SXParticlesEditor::EditSpawnNextTime->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_uiSpawnNextTime));

	SXParticlesEditor::EditSpawnNextTimeDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_uiSpawnNextTimeDisp));


	SXParticlesEditor::EditAnimTexCountCadrsX->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iAnimTexCountCadrsX));
	SXParticlesEditor::EditAnimTexCountCadrsY->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iAnimTexCountCadrsY));

	SXParticlesEditor::EditAnimTexRate->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iAnimTexRate));
	SXParticlesEditor::EditAnimTexRateDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iAnimTexRateDisp));

	SXParticlesEditor::EditAnimTexStartCadr->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iAnimTexStartCadr));
	SXParticlesEditor::EditAnimTexStartCadrDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iAnimTexStartCadrDisp));


	SXParticlesEditor::EditReCreateCount->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iReCreateCount));
	SXParticlesEditor::EditSoftCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fSoftCoef));
	SXParticlesEditor::EditRefractionCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fRefractionCoef));
	SXParticlesEditor::EditTransparencyCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fTransparencyCoef));
	SXParticlesEditor::EditColorCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fColorCoef));

	pdata.m_isLighting = SXParticlesEditor::CheckBoxLighting->getCheck();

	pdata.m_typeFigure = (PARTICLESTYPE_FIGURE)SXParticlesEditor::ComboBoxFigureType->getSel();
	SXParticlesEditor::EditFigureCountQuads->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_iFigureCountQuads));
	pdata.m_useFigureRotRand = SXParticlesEditor::CheckBoxFigureRotRand->getCheck();
	pdata.m_useFigureTapX = SXParticlesEditor::CheckBoxFigureTapX->getCheck();
	pdata.m_useFigureTapY = SXParticlesEditor::CheckBoxFigureTapY->getCheck();
	pdata.m_useFigureTapZ = SXParticlesEditor::CheckBoxFigureTapZ->getCheck();

	pdata.m_typeAlphaBlend = (PARTICLESTYPE_ALPHABLEND)SXParticlesEditor::ComboBoxAlphaBlendType->getSel();

	SXParticlesEditor::EditTimeLife->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_uiTimeLife));
	SXParticlesEditor::EditTimeLifeDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.m_uiTimeLifeDisp));

	pdata.m_typeAlphaDependAge = (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxAlphaDependAge->getSel();
	pdata.m_typeSizeDependAge = (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxSizeDependAge->getSel();
	pdata.m_useCollisionDelete = (PARTICLESTYPE_DEPEND)SXParticlesEditor::CheckBoxCollisionDelete->getCheck();

	SXParticlesEditor::EditSizeX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vSize.x));
	SXParticlesEditor::EditSizeY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_vSize.y));
	SXParticlesEditor::EditSizeDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fSizeDisp));


	pdata.m_useCharacterCircle = SXParticlesEditor::CheckBoxCircle->getCheck();
	pdata.m_typeCharacterCircleAxis = (PARTICLES_AXIS)SXParticlesEditor::ComboBoxCircleAxis->getSel();
	SXParticlesEditor::EditCircleAngle->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterCircleAngle));
	SXParticlesEditor::EditCircleAngleDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterCircleAngleDisp));
	pdata.m_useCharacterCircleAngleDispNeg = SXParticlesEditor::CheckBoxCircleAngleDispNeg->getCheck();


	pdata.m_useCharacterRotate = SXParticlesEditor::CheckBoxRotate->getCheck();
	SXParticlesEditor::EditRotateAngle->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterRotateAngle));
	SXParticlesEditor::EditRotateAngleDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterRotateAngleDisp));
	pdata.m_useCharacterRotateAngleDispNeg = SXParticlesEditor::CheckBoxRotateAngleDispNeg->getCheck();


	pdata.m_useCharacterDeviation = SXParticlesEditor::CheckBoxDeviation->getCheck();
	pdata.m_typeCharacterDeviation = (PARTICLESTYPE_DEVIATION)SXParticlesEditor::ComboBoxDeviationType->getSel();
	SXParticlesEditor::EditDeviationAmplitude->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterDeviationAmplitude));
	SXParticlesEditor::EditDeviationCoefAngle->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterDeviationCoefAngle));
	SXParticlesEditor::EditDeviationCoefAngleDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.m_fCharacterDeviationCoefAngleDisp));
	pdata.m_useCharacterDeviationCoefAngleDispNeg = SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->getCheck();
	pdata.m_typeCharacterDeviationAxis = (PARTICLES_AXIS)SXParticlesEditor::ComboBoxDeviationAxis->getSel();
	pdata.m_useCharacterDeviationTapX = SXParticlesEditor::CheckBoxDeviationTapX->getCheck();
	pdata.m_useCharacterDeviationTapY = SXParticlesEditor::CheckBoxDeviationTapY->getCheck();
	pdata.m_useCharacterDeviationTapZ = SXParticlesEditor::CheckBoxDeviationTapZ->getCheck();
	

	ID partid = SPE_EmitterAdd(SXParticlesEditor::SelEffID, &pdata);
	SPE_EmitterSetName(SXParticlesEditor::SelEffID, partid, partname);

	SPE_EmitterSetTexture(SXParticlesEditor::SelEffID, partid, tmptex);

	SXParticlesEditor::EditCount->getText(tmptxt, 64);
	int partcount;
	sscanf(tmptxt, "%d", &partcount);
	SPE_EmitterSetCount(SXParticlesEditor::SelEffID, partid, partcount);

	SXParticlesEditor::ListBoxEmitters->addItem(partname);
	SXParticlesEditor::ListBoxEmitters->setSel(SXParticlesEditor::ListBoxEmitters->getItemCount() - 1);
	SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);

	return 0;
}