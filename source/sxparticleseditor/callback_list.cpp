
#include "callback_list.h"

LRESULT SXParticlesEditor_ListBoxEffects_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXParticlesEditor::ListBoxEffects->GetSel();
	if (sel < 0)
		return 0;

	SXParticlesEditor::ListBoxEmitters->Clear();
	if (SPE_EffectCountGet() == 0)
	{
		SXParticlesEditor::ListBoxEffects->Clear();
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
		SXParticlesEditor::ListBoxEmitters->AddItem(partname);
	}

	SXParticlesEditor::StaticEmittersCount->SetText(String(partcount).c_str());

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

	SXParticlesEditor::SelEmitterID = SXParticlesEditor::ListBoxEmitters->GetSel();
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
	SXParticlesEditor::ListBoxEmitters->Clear();
	SXParticlesEditor::StaticEmittersCount->SetText("0");

	int sel = SXParticlesEditor::ListBoxEffects->GetSel();
	SXParticlesEditor::ListBoxEffects->DeleteItem(SXParticlesEditor::ListBoxEffects->GetSel());
	SXParticlesEditor::StaticEffectsCount->SetText(String(SPE_EffectCountGet()).c_str());

	if (SPE_EffectCountGet() > 0)
	{
		SXParticlesEditor::ListBoxEffects->SetSel((sel < SXParticlesEditor::ListBoxEffects->GetCountItem() ? sel : sel - 1));
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
	int sel = SXParticlesEditor::ListBoxEmitters->GetSel();
	SXParticlesEditor::ListBoxEmitters->DeleteItem(sel);
	SXParticlesEditor::StaticEmittersCount->SetText(String(SPE_EmitterSCountGet(SXParticlesEditor::SelEffID)).c_str());

	if (SPE_EmitterSCountGet(SXParticlesEditor::SelEffID))
	{
		SXParticlesEditor::ListBoxEmitters->SetSel((sel < SXParticlesEditor::ListBoxEmitters->GetCountItem() ? sel : sel - 1));
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
	SXParticlesEditor::EditEffName->GetText(effname, OBJECT_NAME_MAX_LEN);

	SXParticlesEditor::SelEffID = SPE_EffectAdd(effname);

	SXParticlesEditor::ListBoxEffects->AddItem(effname);
	SXParticlesEditor::ListBoxEffects->SetSel(SXParticlesEditor::ListBoxEffects->GetCountItem() - 1);
	SXParticlesEditor::StaticEffectsCount->SetText(String(SPE_EffectCountGet()).c_str());
	SXParticlesEditor::EffVisible(true, false);

	return 0;
}

///////////

LRESULT SXParticlesEditor_ButtonEmitterCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char partname[OBJECT_NAME_MAX_LEN];
	SXParticlesEditor::EditName->GetText(partname, OBJECT_NAME_MAX_LEN);

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
	SXParticlesEditor::EditTexture->GetText(tmptex, SXGC_LOADTEX_MAX_SIZE_DIRNAME);


	char tmptxt[64];

	ParticlesData pdata;

	SXParticlesEditor::EditAccelerationX->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Acceleration.x));
	SXParticlesEditor::EditAccelerationY->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Acceleration.y));
	SXParticlesEditor::EditAccelerationZ->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Acceleration.z));

	SXParticlesEditor::EditAccelerationDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.AccelerationDisp));

	pdata.AccelerationDispXNeg = SXParticlesEditor::CheckBoxAccelerationDispXNeg->GetCheck();
	pdata.AccelerationDispYNeg = SXParticlesEditor::CheckBoxAccelerationDispYNeg->GetCheck();
	pdata.AccelerationDispZNeg = SXParticlesEditor::CheckBoxAccelerationDispZNeg->GetCheck();


	SXParticlesEditor::EditVelocityX->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Velocity.x));
	SXParticlesEditor::EditVelocityY->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Velocity.y));
	SXParticlesEditor::EditVelocityZ->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Velocity.z));

	SXParticlesEditor::EditVelocityDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.VelocityDisp));

	pdata.VelocityDispXNeg = SXParticlesEditor::CheckBoxVelocityDispXNeg->GetCheck();
	pdata.VelocityDispYNeg = SXParticlesEditor::CheckBoxVelocityDispYNeg->GetCheck();
	pdata.VelocityDispZNeg = SXParticlesEditor::CheckBoxVelocityDispZNeg->GetCheck();


	pdata.BoundType = (PARTICLESTYPE_BOUND)SXParticlesEditor::ComboBoxBoundType->GetSel();

	SXParticlesEditor::EditBoundVec1X->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec1.x));
	SXParticlesEditor::EditBoundVec1Y->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec1.y));
	SXParticlesEditor::EditBoundVec1Z->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec1.z));

	SXParticlesEditor::EditBoundVec2X->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec2.x));
	SXParticlesEditor::EditBoundVec2Y->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec2.y));
	SXParticlesEditor::EditBoundVec2Z->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.BoundVec2.z));


	pdata.SpawnPosType = (PARTICLESTYPE_SPAWNPOS)SXParticlesEditor::ComboBoxSpawnPosType->GetSel();
	SXParticlesEditor::EditSpawnOriginX->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOrigin.x));
	SXParticlesEditor::EditSpawnOriginY->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOrigin.y));
	SXParticlesEditor::EditSpawnOriginZ->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOrigin.z));

	SXParticlesEditor::EditSpawnOriginDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SpawnOriginDisp));

	pdata.SpawnBoundBindCreateXNeg = SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->GetCheck();
	pdata.SpawnBoundBindCreateXPos = SXParticlesEditor::CheckBoxSpawnOriginDispXPos->GetCheck();
	pdata.SpawnBoundBindCreateYNeg = SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->GetCheck();
	pdata.SpawnBoundBindCreateYPos = SXParticlesEditor::CheckBoxSpawnOriginDispYPos->GetCheck();
	pdata.SpawnBoundBindCreateZNeg = SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->GetCheck();
	pdata.SpawnBoundBindCreateZPos = SXParticlesEditor::CheckBoxSpawnOriginDispZPos->GetCheck();

	SXParticlesEditor::EditSpawnNextTime->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.SpawnNextTime));

	SXParticlesEditor::EditSpawnNextTimeDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.SpawnNextTimeDisp));


	SXParticlesEditor::EditAnimTexCountCadrsX->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexCountCadrsX));
	SXParticlesEditor::EditAnimTexCountCadrsY->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexCountCadrsY));

	SXParticlesEditor::EditAnimTexRate->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexRate));
	SXParticlesEditor::EditAnimTexRateDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexRateDisp));

	SXParticlesEditor::EditAnimTexStartCadr->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexStartCadr));
	SXParticlesEditor::EditAnimTexStartCadrDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.AnimTexStartCadrDisp));


	SXParticlesEditor::EditReCreateCount->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.ReCreateCount));
	SXParticlesEditor::EditSoftCoef->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SoftCoef));
	SXParticlesEditor::EditRefractionCoef->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.RefractionCoef));
	SXParticlesEditor::EditTransparencyCoef->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.TransparencyCoef));
	SXParticlesEditor::EditColorCoef->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.ColorCoef));

	pdata.Lighting = SXParticlesEditor::CheckBoxLighting->GetCheck();

	pdata.FigureType = (PARTICLESTYPE_FIGURE)SXParticlesEditor::ComboBoxFigureType->GetSel();
	SXParticlesEditor::EditFigureCountQuads->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.FigureCountQuads));
	pdata.FigureRotRand = SXParticlesEditor::CheckBoxFigureRotRand->GetCheck();
	pdata.FigureTapX = SXParticlesEditor::CheckBoxFigureTapX->GetCheck();
	pdata.FigureTapY = SXParticlesEditor::CheckBoxFigureTapY->GetCheck();
	pdata.FigureTapZ = SXParticlesEditor::CheckBoxFigureTapZ->GetCheck();

	pdata.AlphaBlendType = (PARTICLESTYPE_ALPHABLEND)SXParticlesEditor::ComboBoxAlphaBlendType->GetSel();

	SXParticlesEditor::EditTimeLife->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.TimeLife));
	SXParticlesEditor::EditTimeLifeDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%d", &(pdata.TimeLifeDisp));

	pdata.AlphaDependAge = (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxAlphaDependAge->GetSel();
	pdata.SizeDependAge = (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxSizeDependAge->GetSel();
	pdata.CollisionDelete = (PARTICLESTYPE_DEPEND)SXParticlesEditor::CheckBoxCollisionDelete->GetCheck();

	SXParticlesEditor::EditSizeX->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Size.x));
	SXParticlesEditor::EditSizeY->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.Size.y));
	SXParticlesEditor::EditSizeDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.SizeDisp));


	pdata.CharacterCircle = SXParticlesEditor::CheckBoxCircle->GetCheck();
	pdata.CharacterCircleAxis = (PARTICLES_AXIS)SXParticlesEditor::ComboBoxCircleAxis->GetSel();
	SXParticlesEditor::EditCircleAngle->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterCircleAngle));
	SXParticlesEditor::EditCircleAngleDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterCircleAngleDisp));
	pdata.CharacterCircleAngleDispNeg = SXParticlesEditor::CheckBoxCircleAngleDispNeg->GetCheck();


	pdata.CharacterRotate = SXParticlesEditor::CheckBoxRotate->GetCheck();
	SXParticlesEditor::EditRotateAngle->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterRotateAngle));
	SXParticlesEditor::EditRotateAngleDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterRotateAngleDisp));
	pdata.CharacterRotateAngleDispNeg = SXParticlesEditor::CheckBoxRotateAngleDispNeg->GetCheck();


	pdata.CharacterDeviation = SXParticlesEditor::CheckBoxDeviation->GetCheck();
	pdata.CharacterDeviationType = (PARTICLESTYPE_DEVIATION)SXParticlesEditor::ComboBoxDeviationType->GetSel();
	SXParticlesEditor::EditDeviationAmplitude->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterDeviationAmplitude));
	SXParticlesEditor::EditDeviationCoefAngle->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterDeviationCoefAngle));
	SXParticlesEditor::EditDeviationCoefAngleDisp->GetText(tmptxt, 64);
	sscanf(tmptxt, "%f", &(pdata.CharacterDeviationCoefAngleDisp));
	pdata.CharacterDeviationCoefAngleDispNeg = SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->GetCheck();
	pdata.CharacterDeviationAxis = (PARTICLES_AXIS)SXParticlesEditor::ComboBoxDeviationAxis->GetSel();
	pdata.CharacterDeviationTapX = SXParticlesEditor::CheckBoxDeviationTapX->GetCheck();
	pdata.CharacterDeviationTapY = SXParticlesEditor::CheckBoxDeviationTapY->GetCheck();
	pdata.CharacterDeviationTapZ = SXParticlesEditor::CheckBoxDeviationTapZ->GetCheck();
	

	ID partid = SPE_EmitterAdd(SXParticlesEditor::SelEffID, &pdata);
	SPE_EmitterNameSet(SXParticlesEditor::SelEffID, partid, partname);

	SPE_EmitterTextureSet(SXParticlesEditor::SelEffID, partid, tmptex);

	SXParticlesEditor::EditCount->GetText(tmptxt, 64);
	int partcount;
	sscanf(tmptxt, "%d", &partcount);
	SPE_EmitterCountSet(SXParticlesEditor::SelEffID, partid, partcount);

	SXParticlesEditor::ListBoxEmitters->AddItem(partname);
	SXParticlesEditor::ListBoxEmitters->SetSel(SXParticlesEditor::ListBoxEmitters->GetCountItem() - 1);
	SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);

	return 0;
}