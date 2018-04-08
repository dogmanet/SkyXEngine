
#include "callback_list.h"

LRESULT SXParticlesEditor_ListBoxEffects_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXParticlesEditor::ListBoxEffects->getSel();
	if (sel < 0)
		return 0;

	SXParticlesEditor::ListBoxEmitters->clear();
	if (SPE_EffectCountGet() == 0)
	{
		SXParticlesEditor::ListBoxEffects->clear();
		SXParticlesEditor::EffVisible(true, true);
		SXParticlesEditor::TabsVisible(false);
		return 0;
	}
	
	SXParticlesEditor::SelEffID = SPE_EffectIdOfKey(sel);
	int partcount = SPE_EmitterSCountGet(SXParticlesEditor::SelEffID);
	char partname[OBJECT_NAME_MAX_LEN];
	for (int i = 0; i < partcount; ++i)
	{
		partname[0] = '!';
		partname[1] = 0;
		SPE_EmitterNameGet(SXParticlesEditor::SelEffID, i, partname);
		if (partname[0] == 0)
			partname[0] = '!';
		SXParticlesEditor::ListBoxEmitters->addItem(partname);
	}

	SXParticlesEditor::StaticEmittersCount->setText(String(partcount).c_str());

	if (hwnd != 0)
		SXParticlesEditor::TabsVisible(false);

	for (int i = 0; i < SPE_EffectCountGet(); ++i)
	{
		SPE_EffectEnableSet(SPE_EffectIdOfKey(i), false);
	}

	SPE_EffectEnableSet(SXParticlesEditor::SelEffID, true);

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
	int countemitters = SPE_EmitterSCountGet(SXParticlesEditor::SelEffID);
	if (SXParticlesEditor::SelEmitterID < 0 || countemitters <= 0 || SPE_EmitterSCountGet(SXParticlesEditor::SelEffID) <= SXParticlesEditor::SelEmitterID)
		return 0;

	SPE_EffectEnableSet(SXParticlesEditor::SelEffID, true);

	for (int i = 0; i < SPE_EmitterSCountGet(SXParticlesEditor::SelEffID); ++i)
	{
		SPE_EmitterEnableSet(SXParticlesEditor::SelEffID, i, false);
	}
	
	SPE_EmitterEnableSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, true);
	
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

	for (int i = 0; i < SPE_EffectCountGet(); ++i)
	{
		SPE_EffectEnableSet(SPE_EffectIdOfKey(i), false);
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
	SXParticlesEditor::StaticEffectsCount->setText(String(SPE_EffectCountGet()).c_str());

	if (SPE_EffectCountGet() > 0)
	{
		SXParticlesEditor::ListBoxEffects->setSel((sel < SXParticlesEditor::ListBoxEffects->getItemCount() ? sel : sel - 1));
		SXParticlesEditor_ListBoxEffects_Click(hwnd, msg, wParam, lParam);
	}
	else
		SXParticlesEditor_ButtonEffectsCreate_Click(hwnd, msg, wParam, lParam);

	if (SXParticlesEditor::SelEffID >= 0)
		SPE_EffectEnableSet(SXParticlesEditor::SelEffID, false);

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

	for (int i = 0; i < SPE_EmitterSCountGet(SXParticlesEditor::SelEffID); ++i)
	{
		SPE_EmitterEnableSet(SXParticlesEditor::SelEffID, i, false);
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

	for (int i = 0; i < SPE_EmitterSCountGet(SXParticlesEditor::SelEffID); ++i)
	{
		SPE_EmitterEnableSet(SXParticlesEditor::SelEffID, i, false);
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
	SXParticlesEditor::StaticEmittersCount->setText(String(SPE_EmitterSCountGet(SXParticlesEditor::SelEffID)).c_str());

	if (SPE_EmitterSCountGet(SXParticlesEditor::SelEffID))
	{
		SXParticlesEditor::ListBoxEmitters->setSel((sel < SXParticlesEditor::ListBoxEmitters->getItemCount() ? sel : sel - 1));
		SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);
	}
	else
		SXParticlesEditor_ButtonEmittersCreate_Click(hwnd, msg, wParam, lParam);

	if (SXParticlesEditor::SelEffID >= 0)
		SPE_EffectEnableSet(SXParticlesEditor::SelEffID, false);

	return 0;
}

///

LRESULT SXParticlesEditor_ButtonEffCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (int i = 0; i < SPE_EffectCountGet(); ++i)
	{
		SPE_EffectEnableSet(SPE_EffectIdOfKey(i), false);
	}

	char effname[OBJECT_NAME_MAX_LEN];
	SXParticlesEditor::EditEffName->getText(effname, OBJECT_NAME_MAX_LEN);

	SXParticlesEditor::SelEffID = SPE_EffectAdd(effname);

	SXParticlesEditor::ListBoxEffects->addItem(effname);
	SXParticlesEditor::ListBoxEffects->setSel(SXParticlesEditor::ListBoxEffects->getItemCount() - 1);
	SXParticlesEditor::StaticEffectsCount->setText(String(SPE_EffectCountGet()).c_str());
	SXParticlesEditor::EffVisible(true, false);

	return 0;
}

///////////

LRESULT SXParticlesEditor_ButtonEmitterCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char partname[OBJECT_NAME_MAX_LEN];
	SXParticlesEditor::EditName->getText(partname, OBJECT_NAME_MAX_LEN);

	if (!def_str_validate(partname))
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

	ParticlesData pdata;

	SXParticlesEditor::EditAccelerationX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Acceleration.x));
	SXParticlesEditor::EditAccelerationY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Acceleration.y));
	SXParticlesEditor::EditAccelerationZ->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Acceleration.z));

	SXParticlesEditor::EditAccelerationDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.AccelerationDisp));

	pdata.AccelerationDispXNeg = SXParticlesEditor::CheckBoxAccelerationDispXNeg->getCheck();
	pdata.AccelerationDispYNeg = SXParticlesEditor::CheckBoxAccelerationDispYNeg->getCheck();
	pdata.AccelerationDispZNeg = SXParticlesEditor::CheckBoxAccelerationDispZNeg->getCheck();


	SXParticlesEditor::EditVelocityX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Velocity.x));
	SXParticlesEditor::EditVelocityY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Velocity.y));
	SXParticlesEditor::EditVelocityZ->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Velocity.z));

	SXParticlesEditor::EditVelocityDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.VelocityDisp));

	pdata.VelocityDispXNeg = SXParticlesEditor::CheckBoxVelocityDispXNeg->getCheck();
	pdata.VelocityDispYNeg = SXParticlesEditor::CheckBoxVelocityDispYNeg->getCheck();
	pdata.VelocityDispZNeg = SXParticlesEditor::CheckBoxVelocityDispZNeg->getCheck();


	pdata.BoundType = (PARTICLESTYPE_BOUND)SXParticlesEditor::ComboBoxBoundType->getSel();

	SXParticlesEditor::EditBoundVec1X->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec1.x));
	SXParticlesEditor::EditBoundVec1Y->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec1.y));
	SXParticlesEditor::EditBoundVec1Z->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec1.z));

	SXParticlesEditor::EditBoundVec2X->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec2.x));
	SXParticlesEditor::EditBoundVec2Y->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec2.y));
	SXParticlesEditor::EditBoundVec2Z->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec2.z));


	pdata.SpawnPosType = (PARTICLESTYPE_SPAWNPOS)SXParticlesEditor::ComboBoxSpawnPosType->getSel();
	SXParticlesEditor::EditSpawnOriginX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOrigin.x));
	SXParticlesEditor::EditSpawnOriginY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOrigin.y));
	SXParticlesEditor::EditSpawnOriginZ->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOrigin.z));

	SXParticlesEditor::EditSpawnOriginDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOriginDisp));

	pdata.SpawnBoundBindCreateXNeg = SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->getCheck();
	pdata.SpawnBoundBindCreateXPos = SXParticlesEditor::CheckBoxSpawnOriginDispXPos->getCheck();
	pdata.SpawnBoundBindCreateYNeg = SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->getCheck();
	pdata.SpawnBoundBindCreateYPos = SXParticlesEditor::CheckBoxSpawnOriginDispYPos->getCheck();
	pdata.SpawnBoundBindCreateZNeg = SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->getCheck();
	pdata.SpawnBoundBindCreateZPos = SXParticlesEditor::CheckBoxSpawnOriginDispZPos->getCheck();

	SXParticlesEditor::EditSpawnNextTime->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.SpawnNextTime));

	SXParticlesEditor::EditSpawnNextTimeDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.SpawnNextTimeDisp));


	SXParticlesEditor::EditAnimTexCountCadrsX->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexCountCadrsX));
	SXParticlesEditor::EditAnimTexCountCadrsY->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexCountCadrsY));

	SXParticlesEditor::EditAnimTexRate->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexRate));
	SXParticlesEditor::EditAnimTexRateDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexRateDisp));

	SXParticlesEditor::EditAnimTexStartCadr->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexStartCadr));
	SXParticlesEditor::EditAnimTexStartCadrDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexStartCadrDisp));


	SXParticlesEditor::EditReCreateCount->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.ReCreateCount));
	SXParticlesEditor::EditSoftCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SoftCoef));
	SXParticlesEditor::EditRefractionCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.RefractionCoef));
	SXParticlesEditor::EditTransparencyCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.TransparencyCoef));
	SXParticlesEditor::EditColorCoef->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.ColorCoef));

	pdata.Lighting = SXParticlesEditor::CheckBoxLighting->getCheck();

	pdata.FigureType = (PARTICLESTYPE_FIGURE)SXParticlesEditor::ComboBoxFigureType->getSel();
	SXParticlesEditor::EditFigureCountQuads->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.FigureCountQuads));
	pdata.FigureRotRand = SXParticlesEditor::CheckBoxFigureRotRand->getCheck();
	pdata.FigureTapX = SXParticlesEditor::CheckBoxFigureTapX->getCheck();
	pdata.FigureTapY = SXParticlesEditor::CheckBoxFigureTapY->getCheck();
	pdata.FigureTapZ = SXParticlesEditor::CheckBoxFigureTapZ->getCheck();

	pdata.AlphaBlendType = (PARTICLESTYPE_ALPHABLEND)SXParticlesEditor::ComboBoxAlphaBlendType->getSel();

	SXParticlesEditor::EditTimeLife->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.TimeLife));
	SXParticlesEditor::EditTimeLifeDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.TimeLifeDisp));

	pdata.AlphaDependAge = (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxAlphaDependAge->getSel();
	pdata.SizeDependAge = (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxSizeDependAge->getSel();
	pdata.CollisionDelete = (PARTICLESTYPE_DEPEND)SXParticlesEditor::CheckBoxCollisionDelete->getCheck();

	SXParticlesEditor::EditSizeX->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Size.x));
	SXParticlesEditor::EditSizeY->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Size.y));
	SXParticlesEditor::EditSizeDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SizeDisp));


	pdata.CharacterCircle = SXParticlesEditor::CheckBoxCircle->getCheck();
	pdata.CharacterCircleAxis = (PARTICLES_AXIS)SXParticlesEditor::ComboBoxCircleAxis->getSel();
	SXParticlesEditor::EditCircleAngle->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterCircleAngle));
	SXParticlesEditor::EditCircleAngleDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterCircleAngleDisp));
	pdata.CharacterCircleAngleDispNeg = SXParticlesEditor::CheckBoxCircleAngleDispNeg->getCheck();


	pdata.CharacterRotate = SXParticlesEditor::CheckBoxRotate->getCheck();
	SXParticlesEditor::EditRotateAngle->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterRotateAngle));
	SXParticlesEditor::EditRotateAngleDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterRotateAngleDisp));
	pdata.CharacterRotateAngleDispNeg = SXParticlesEditor::CheckBoxRotateAngleDispNeg->getCheck();


	pdata.CharacterDeviation = SXParticlesEditor::CheckBoxDeviation->getCheck();
	pdata.CharacterDeviationType = (PARTICLESTYPE_DEVIATION)SXParticlesEditor::ComboBoxDeviationType->getSel();
	SXParticlesEditor::EditDeviationAmplitude->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterDeviationAmplitude));
	SXParticlesEditor::EditDeviationCoefAngle->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterDeviationCoefAngle));
	SXParticlesEditor::EditDeviationCoefAngleDisp->getText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterDeviationCoefAngleDisp));
	pdata.CharacterDeviationCoefAngleDispNeg = SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->getCheck();
	pdata.CharacterDeviationAxis = (PARTICLES_AXIS)SXParticlesEditor::ComboBoxDeviationAxis->getSel();
	pdata.CharacterDeviationTapX = SXParticlesEditor::CheckBoxDeviationTapX->getCheck();
	pdata.CharacterDeviationTapY = SXParticlesEditor::CheckBoxDeviationTapY->getCheck();
	pdata.CharacterDeviationTapZ = SXParticlesEditor::CheckBoxDeviationTapZ->getCheck();
	

	ID partid = SPE_EmitterAdd(SXParticlesEditor::SelEffID, &pdata);
	SPE_EmitterNameSet(SXParticlesEditor::SelEffID, partid, partname);

	SPE_EmitterTextureSet(SXParticlesEditor::SelEffID, partid, tmptex);

	SXParticlesEditor::EditCount->getText(tmptxt, 64);
	int partcount;
	sscanf(tmptxt, "%d", &partcount);
	SPE_EmitterCountSet(SXParticlesEditor::SelEffID, partid, partcount);

	SXParticlesEditor::ListBoxEmitters->addItem(partname);
	SXParticlesEditor::ListBoxEmitters->setSel(SXParticlesEditor::ListBoxEmitters->getItemCount() - 1);
	SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);

	return 0;
}