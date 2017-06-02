
void SXParticlesEditor::EffInitList()
{
	SXParticlesEditor::ListBoxEffects->Clear();
	SXParticlesEditor::ListBoxEmitters->Clear();
	int effcount = SPE_EffectCountGet();
	char effname[OBJECT_NAME_MAX_LEN];
	for (int i = 0; i < effcount; ++i)
	{
		effname[0] = '!';
		effname[1] = 0;
		SPE_EffectNameGet(SPE_EffectIdOfKey(i), effname);
		SXParticlesEditor::ListBoxEffects->AddItem(effname);
		if (effname[0] == 0)
			effname[0] = '!';
	}

	SXParticlesEditor::StaticEffectsCount->SetText(String(effcount).c_str());
	SXParticlesEditor::StaticEmittersCount->SetText("0");
	SXParticlesEditor::TabsVisible(false);

	SXParticlesEditor::SelEffID = SXParticlesEditor::SelEmitterID = -1;
}

void SXParticlesEditor::EffVisible(bool visible, bool iscreate)
{
	StaticEffName->Visible(visible);
	EditEffName->Visible(visible);
	ButtonEffCreate->Visible(iscreate);
}

void SXParticlesEditor::EffNulling()
{
	EditEffName->SetText("0");
}

void SXParticlesEditor::EffDataInit()
{
	char effname[OBJECT_NAME_MAX_LEN];
	SPE_EffectNameGet(SXParticlesEditor::SelEffID, effname);
	SXParticlesEditor::EditEffName->SetText(effname);
}

void SXParticlesEditor::TabsVisible(bool visible)
{
	SXParticlesEditor::GroupBoxTabs->Visible(visible);

	if (visible == false)
	{
		AllInTabsVisible(false);
	}
	else if (!(SXParticlesEditor::GroupBoxTabs->Visible()) && visible)
	{
		SXParticlesEditor::BaseVisible(true);
	}
}

void SXParticlesEditor::AllInTabsVisible(bool visible)
{
	SXParticlesEditor::BaseVisible(visible);
	SXParticlesEditor::BoundVisible(visible);
	SXParticlesEditor::CharacterVisible(visible);
	SXParticlesEditor::SpawnVisible(visible);
	SXParticlesEditor::TexVisible(visible);
	SXParticlesEditor::VelocityAccVisible(visible);

	if (!visible)
	{
		SXParticlesEditor::ButtonBase->SetCheck(false);
		SXParticlesEditor::ButtonBound->SetCheck(false);
		SXParticlesEditor::ButtonCharacters->SetCheck(false);
		SXParticlesEditor::ButtonSpawn->SetCheck(false);
		SXParticlesEditor::ButtonTextureAnimTex->SetCheck(false);
		SXParticlesEditor::ButtonVelocityAcceleration->SetCheck(false);
		SXParticlesEditor::ButtonEmitterCreate->Visible(false);
	}
}

void SXParticlesEditor::AllInTabsNulling()
{
	SXParticlesEditor::BaseNulling();
	SXParticlesEditor::BoundNulling();
	SXParticlesEditor::CharacterNulling();
	SXParticlesEditor::SpawnNulling();
	SXParticlesEditor::TexNulling();
	SXParticlesEditor::VelocityAccNulling();
}

void SXParticlesEditor::EmitterCreateVisible(bool visible)
{
	SXParticlesEditor::ButtonEmitterCreate->Visible(visible);
}

void SXParticlesEditor::BaseVisible(bool visible)
{
	StaticName->Visible(visible);
	EditName->Visible(visible);
	
	StaticReCreateCount->Visible(visible);
	EditReCreateCount->Visible(visible);
	StaticCount->Visible(visible);
	EditCount->Visible(visible);
	StaticColorCoef->Visible(visible);
	EditColorCoef->Visible(visible);
	StaticSoftCoef->Visible(visible);
	EditSoftCoef->Visible(visible);
	StaticRefractionCoef->Visible(visible);
	EditRefractionCoef->Visible(visible);
	StaticTransparencyCoef->Visible(visible);
	EditTransparencyCoef->Visible(visible);
	CheckBoxLighting->Visible(visible);
	StaticFigureType->Visible(visible);
	ComboBoxFigureType->Visible(visible);
	StaticFigureCountQuads->Visible(visible);
	EditFigureCountQuads->Visible(visible);
	CheckBoxFigureRotRand->Visible(visible);
	CheckBoxFigureTapX->Visible(visible);
	CheckBoxFigureTapY->Visible(visible);
	CheckBoxFigureTapZ->Visible(visible);
	StaticAlphaBlendType->Visible(visible);
	ComboBoxAlphaBlendType->Visible(visible);
	StaticTimeLife->Visible(visible);
	EditTimeLife->Visible(visible);
	StaticTimeLifeDisp->Visible(visible);
	EditTimeLifeDisp->Visible(visible);
	StaticAlphaAgeDepend->Visible(visible);
	ComboBoxAlphaDependAge->Visible(visible);
	StaticSize->Visible(visible);
	StaticSizeX->Visible(visible);
	EditSizeX->Visible(visible);
	StaticSizeY->Visible(visible);
	EditSizeY->Visible(visible);
	StaticSizeDisp->Visible(visible);
	EditSizeDisp->Visible(visible);
	StaticSizeDependAge->Visible(visible);
	ComboBoxSizeDependAge->Visible(visible);
}

void SXParticlesEditor::BaseDataInit()
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return;
	}

	if (SXParticlesEditor::SelEmitterID == -1)
	{
		return;
	}

	char tmpname[OBJECT_NAME_MAX_LEN];
	SPE_EmitterNameGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, tmpname);
	SXParticlesEditor::EditName->SetText(tmpname);
	SXParticlesEditor::EditCount->SetText(String(SPE_EmitterCountGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID)).c_str());
	SXParticlesEditor::EditReCreateCount->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ReCreateCount)).c_str());
	SXParticlesEditor::EditColorCoef->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ColorCoef)).c_str());
	SXParticlesEditor::EditSoftCoef->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SoftCoef)).c_str());
	SXParticlesEditor::EditRefractionCoef->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, RefractionCoef)).c_str());
	SXParticlesEditor::EditTransparencyCoef->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TransparencyCoef)).c_str());
	SXParticlesEditor::CheckBoxLighting->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Lighting));
	SXParticlesEditor::ComboBoxFigureType->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureType));
	SXParticlesEditor::EditFigureCountQuads->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureCountQuads)).c_str());
	SXParticlesEditor::CheckBoxFigureRotRand->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureRotRand));
	SXParticlesEditor::CheckBoxFigureTapX->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapX));
	SXParticlesEditor::CheckBoxFigureTapY->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapY));
	SXParticlesEditor::CheckBoxFigureTapZ->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapZ));
	SXParticlesEditor::ComboBoxAlphaBlendType->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaBlendType));
	SXParticlesEditor::EditTimeLife->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLife)).c_str());
	SXParticlesEditor::EditTimeLifeDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLifeDisp)).c_str());
	SXParticlesEditor::ComboBoxAlphaDependAge->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaDependAge));
	SXParticlesEditor::EditSizeX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.x)).c_str());
	SXParticlesEditor::EditSizeY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.y)).c_str());
	SXParticlesEditor::EditSizeDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDisp)).c_str());
	SXParticlesEditor::ComboBoxSizeDependAge->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDependAge));
}

void SXParticlesEditor::BaseNulling()
{
	SXParticlesEditor::EditName->SetText("");
	SXParticlesEditor::EditCount->SetText("0");
	SXParticlesEditor::EditReCreateCount->SetText("0");
	SXParticlesEditor::EditColorCoef->SetText("1");
	SXParticlesEditor::EditSoftCoef->SetText("0");
	SXParticlesEditor::EditRefractionCoef->SetText("0");
	SXParticlesEditor::EditTransparencyCoef->SetText("1");
	SXParticlesEditor::CheckBoxLighting->SetCheck(false);
	SXParticlesEditor::ComboBoxFigureType->SetSel(0);
	SXParticlesEditor::EditFigureCountQuads->SetText("1");
	SXParticlesEditor::CheckBoxFigureRotRand->SetCheck(false);
	SXParticlesEditor::CheckBoxFigureTapX->SetCheck(false);
	SXParticlesEditor::CheckBoxFigureTapY->SetCheck(false);
	SXParticlesEditor::CheckBoxFigureTapZ->SetCheck(false);
	SXParticlesEditor::ComboBoxAlphaBlendType->SetSel(0);
	SXParticlesEditor::EditTimeLife->SetText("0");
	SXParticlesEditor::EditTimeLifeDisp->SetText("0");
	SXParticlesEditor::ComboBoxAlphaDependAge->SetSel(0);
	SXParticlesEditor::EditSizeX->SetText("0.5");
	SXParticlesEditor::EditSizeY->SetText("0.5");
	SXParticlesEditor::EditSizeDisp->SetText("0");
	SXParticlesEditor::ComboBoxSizeDependAge->SetSel(0);
}

void SXParticlesEditor::BoundVisible(bool visible)
{
	SXParticlesEditor::StaticBoundType->Visible(visible);
	SXParticlesEditor::ComboBoxBoundType->Visible(visible);
	SXParticlesEditor::StaticBoundVec1->Visible(visible);
	SXParticlesEditor::StaticBoundVec1X->Visible(visible);
	SXParticlesEditor::EditBoundVec1X->Visible(visible);
	SXParticlesEditor::StaticBoundVec1Y->Visible(visible);
	SXParticlesEditor::EditBoundVec1Y->Visible(visible);
	SXParticlesEditor::StaticBoundVec1Z->Visible(visible);
	SXParticlesEditor::EditBoundVec1Z->Visible(visible);
	SXParticlesEditor::StaticBoundVec1W->Visible(visible);
	SXParticlesEditor::EditBoundVec1W->Visible(visible);
	SXParticlesEditor::StaticBoundVec2->Visible(visible);
	SXParticlesEditor::StaticBoundVec2X->Visible(visible);
	SXParticlesEditor::EditBoundVec2X->Visible(visible);
	SXParticlesEditor::StaticBoundVec2Y->Visible(visible);
	SXParticlesEditor::EditBoundVec2Y->Visible(visible);
	SXParticlesEditor::StaticBoundVec2Z->Visible(visible);
	SXParticlesEditor::EditBoundVec2Z->Visible(visible);
	SXParticlesEditor::StaticBoundVec2W->Visible(visible);
	SXParticlesEditor::EditBoundVec2W->Visible(visible);
}

void SXParticlesEditor::BoundDataInit()
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return;
	}

	if (SXParticlesEditor::SelEmitterID == -1)
	{
		return;
	}

	SXParticlesEditor::ComboBoxBoundType->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundType));
	SXParticlesEditor::EditBoundVec1X->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.x)).c_str());
	SXParticlesEditor::EditBoundVec1Y->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.y)).c_str());
	SXParticlesEditor::EditBoundVec1Z->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.z)).c_str());
	SXParticlesEditor::EditBoundVec1W->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.w)).c_str());
	SXParticlesEditor::EditBoundVec2X->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.x)).c_str());
	SXParticlesEditor::EditBoundVec2Y->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.y)).c_str());
	SXParticlesEditor::EditBoundVec2Z->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.z)).c_str());
	SXParticlesEditor::EditBoundVec2W->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.w)).c_str());
}

void SXParticlesEditor::BoundNulling()
{
	SXParticlesEditor::ComboBoxBoundType->SetSel(0);
	SXParticlesEditor::EditBoundVec1X->SetText("0");
	SXParticlesEditor::EditBoundVec1Y->SetText("0");
	SXParticlesEditor::EditBoundVec1Z->SetText("0");
	SXParticlesEditor::EditBoundVec1W->SetText("0");
	SXParticlesEditor::EditBoundVec2X->SetText("0");
	SXParticlesEditor::EditBoundVec2Y->SetText("0");
	SXParticlesEditor::EditBoundVec2Z->SetText("0");
	SXParticlesEditor::EditBoundVec2W->SetText("0");
}

void SXParticlesEditor::CharacterVisible(bool visible)
{
	SXParticlesEditor::CheckBoxCircle->Visible(visible);
	SXParticlesEditor::StaticCircleAxis->Visible(visible);
	SXParticlesEditor::ComboBoxCircleAxis->Visible(visible);
	SXParticlesEditor::StaticCircleAngle->Visible(visible);
	SXParticlesEditor::EditCircleAngle->Visible(visible);
	SXParticlesEditor::StaticCircleAngleDisp->Visible(visible);
	SXParticlesEditor::EditCircleAngleDisp->Visible(visible);
	SXParticlesEditor::CheckBoxRotate->Visible(visible);
	SXParticlesEditor::StaticRotateAngle->Visible(visible);
	SXParticlesEditor::EditRotateAngle->Visible(visible);
	SXParticlesEditor::StaticRotateAngleDisp->Visible(visible);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->Visible(visible);
	SXParticlesEditor::EditRotateAngleDisp->Visible(visible);
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->Visible(visible);
	SXParticlesEditor::CheckBoxDeviation->Visible(visible);
	SXParticlesEditor::StaticDeviationType->Visible(visible);
	SXParticlesEditor::ComboBoxDeviationType->Visible(visible);
	SXParticlesEditor::StaticDeviationAmplitude->Visible(visible);
	SXParticlesEditor::StaticDeviationCoefAngle->Visible(visible);
	SXParticlesEditor::EditDeviationAmplitude->Visible(visible);
	SXParticlesEditor::EditDeviationCoefAngle->Visible(visible);
	SXParticlesEditor::StaticDeviationAxis->Visible(visible);
	SXParticlesEditor::ComboBoxDeviationAxis->Visible(visible);
	SXParticlesEditor::StaticDeviationCountDelayMls->Visible(visible);
	SXParticlesEditor::EditDeviationCountDelayMls->Visible(visible);
	SXParticlesEditor::StaticDeviationCoefAngleDisp->Visible(visible);
	SXParticlesEditor::EditDeviationCoefAngleDisp->Visible(visible);
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->Visible(visible);
	SXParticlesEditor::CheckBoxDeviationTapX->Visible(visible);
	SXParticlesEditor::CheckBoxDeviationTapY->Visible(visible);
	SXParticlesEditor::CheckBoxDeviationTapZ->Visible(visible);
}

void SXParticlesEditor::CharacterDataInit()
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return;
	}

	if (SXParticlesEditor::SelEmitterID == -1)
	{
		return;
	}

	SXParticlesEditor::CheckBoxCircle->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircle));
	SXParticlesEditor::ComboBoxCircleAxis->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAxis));
	SXParticlesEditor::EditCircleAngle->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngle)).c_str());
	SXParticlesEditor::EditCircleAngleDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDisp)).c_str());
	SXParticlesEditor::CheckBoxRotate->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotate));
	SXParticlesEditor::EditRotateAngle->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngle)).c_str());
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDispNeg));
	SXParticlesEditor::EditRotateAngleDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDisp)).c_str());
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDispNeg));
	SXParticlesEditor::CheckBoxDeviation->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviation));
	SXParticlesEditor::ComboBoxDeviationType->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationType));
	SXParticlesEditor::EditDeviationAmplitude->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAmplitude)).c_str());
	SXParticlesEditor::EditDeviationCoefAngle->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngle)).c_str());
	SXParticlesEditor::ComboBoxDeviationAxis->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAxis));
	SXParticlesEditor::EditDeviationCountDelayMls->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCountDelayMls)).c_str());
	SXParticlesEditor::EditDeviationCoefAngleDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDisp)).c_str());
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDispNeg));
	SXParticlesEditor::CheckBoxDeviationTapX->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapX));
	SXParticlesEditor::CheckBoxDeviationTapY->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapY));
	SXParticlesEditor::CheckBoxDeviationTapZ->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapZ));
}

void SXParticlesEditor::CharacterNulling()
{
	SXParticlesEditor::CheckBoxCircle->SetCheck(false);
	SXParticlesEditor::ComboBoxCircleAxis->SetSel(0);
	SXParticlesEditor::EditCircleAngle->SetText("0");
	SXParticlesEditor::EditCircleAngleDisp->SetText("0");
	SXParticlesEditor::CheckBoxRotate->SetCheck(false);
	SXParticlesEditor::EditRotateAngle->SetText("0");
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->SetCheck(false);
	SXParticlesEditor::EditRotateAngleDisp->SetText("0");
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxDeviation->SetCheck(false);
	SXParticlesEditor::ComboBoxDeviationType->SetSel(0);
	SXParticlesEditor::EditDeviationAmplitude->SetText("0");
	SXParticlesEditor::EditDeviationCoefAngle->SetText("0");
	SXParticlesEditor::ComboBoxDeviationAxis->SetSel(0);
	SXParticlesEditor::EditDeviationCountDelayMls->SetText("0");
	SXParticlesEditor::EditDeviationCoefAngleDisp->SetText("0");
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxDeviationTapX->SetCheck(false);
	SXParticlesEditor::CheckBoxDeviationTapY->SetCheck(false);
	SXParticlesEditor::CheckBoxDeviationTapZ->SetCheck(false);
}

void SXParticlesEditor::SpawnVisible(bool visible)
{
	SXParticlesEditor::StaticSpawnPosType->Visible(visible);
	SXParticlesEditor::ComboBoxSpawnPosType->Visible(visible);
	SXParticlesEditor::StaticSpawnOrigin->Visible(visible);
	SXParticlesEditor::StaticSpawnOriginX->Visible(visible);
	SXParticlesEditor::EditSpawnOriginX->Visible(visible);
	SXParticlesEditor::StaticSpawnOriginY->Visible(visible);
	SXParticlesEditor::EditSpawnOriginY->Visible(visible);
	SXParticlesEditor::StaticSpawnOriginZ->Visible(visible);
	SXParticlesEditor::EditSpawnOriginZ->Visible(visible);
	SXParticlesEditor::StaticSpawnOriginDisp->Visible(visible);
	SXParticlesEditor::EditSpawnOriginDisp->Visible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->Visible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->Visible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->Visible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->Visible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->Visible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->Visible(visible);
	SXParticlesEditor::StaticSpawnNextTime->Visible(visible);
	SXParticlesEditor::EditSpawnNextTime->Visible(visible);
	SXParticlesEditor::StaticSpawnNextTimeDisp->Visible(visible);
	SXParticlesEditor::EditSpawnNextTimeDisp->Visible(visible);
}

void SXParticlesEditor::SpawnDataInit()
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return;
	}

	if (SXParticlesEditor::SelEmitterID == -1)
	{
		return;
	}

	SXParticlesEditor::ComboBoxSpawnPosType->SetSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnPosType));
	SXParticlesEditor::EditSpawnOriginX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.x)).c_str());
	SXParticlesEditor::EditSpawnOriginY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.y)).c_str());
	SXParticlesEditor::EditSpawnOriginZ->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.z)).c_str());
	SXParticlesEditor::EditSpawnOriginDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp)).c_str());
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXPos));
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXNeg));
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYPos));
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYNeg));
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZPos));
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZNeg));
	SXParticlesEditor::EditSpawnNextTime->SetText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTime)).c_str());
	SXParticlesEditor::EditSpawnNextTimeDisp->SetText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTimeDisp)).c_str());
}

void SXParticlesEditor::SpawnNulling()
{
	SXParticlesEditor::ComboBoxSpawnPosType->SetSel(0);
	SXParticlesEditor::EditSpawnOriginX->SetText("0");
	SXParticlesEditor::EditSpawnOriginY->SetText("0");
	SXParticlesEditor::EditSpawnOriginZ->SetText("0");
	SXParticlesEditor::EditSpawnOriginDisp->SetText("0");
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->SetCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->SetCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->SetCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->SetCheck(false);
	SXParticlesEditor::EditSpawnNextTime->SetText("0");
	SXParticlesEditor::EditSpawnNextTimeDisp->SetText("0");
}

void SXParticlesEditor::TexVisible(bool visible)
{
	SXParticlesEditor::StaticTexture->Visible(visible);
	SXParticlesEditor::EditTexture->Visible(visible);
	SXParticlesEditor::ButtonTextureSel->Visible(visible);
	SXParticlesEditor::StaticAnimTexCountCadrsX->Visible(visible);
	SXParticlesEditor::StaticAnimTexCountCadrsY->Visible(visible);
	SXParticlesEditor::EditAnimTexCountCadrsX->Visible(visible);
	SXParticlesEditor::EditAnimTexCountCadrsY->Visible(visible);
	SXParticlesEditor::StaticAnimTexRate->Visible(visible);
	SXParticlesEditor::EditAnimTexRate->Visible(visible);
	SXParticlesEditor::StaticAnimTexRateDisp->Visible(visible);
	SXParticlesEditor::EditAnimTexRateDisp->Visible(visible);
	SXParticlesEditor::StaticAnimTexStartCadr->Visible(visible);
	SXParticlesEditor::EditAnimTexStartCadr->Visible(visible);
	SXParticlesEditor::StaticAnimTexStartCadrDisp->Visible(visible);
	SXParticlesEditor::EditAnimTexStartCadrDisp->Visible(visible);
}

void SXParticlesEditor::TexDataInit()
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return;
	}

	if (SXParticlesEditor::SelEmitterID == -1)
	{
		return;
	}

	char tmptex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	SPE_EmitterTextureGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, tmptex);

	SXParticlesEditor::EditTexture->SetText(tmptex);
	SXParticlesEditor::EditAnimTexCountCadrsX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsX)).c_str());
	SXParticlesEditor::EditAnimTexCountCadrsY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsY)).c_str());
	SXParticlesEditor::EditAnimTexRate->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRate)).c_str());
	SXParticlesEditor::EditAnimTexRateDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRateDisp)).c_str());
	SXParticlesEditor::EditAnimTexStartCadr->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadr)).c_str());
	SXParticlesEditor::EditAnimTexStartCadrDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadrDisp)).c_str());
}

void SXParticlesEditor::TexNulling()
{
	SXParticlesEditor::EditTexture->SetText("");
	SXParticlesEditor::EditAnimTexCountCadrsX->SetText("0");
	SXParticlesEditor::EditAnimTexCountCadrsY->SetText("0");
	SXParticlesEditor::EditAnimTexRate->SetText("0");
	SXParticlesEditor::EditAnimTexRateDisp->SetText("0");
	SXParticlesEditor::EditAnimTexStartCadr->SetText("0");
	SXParticlesEditor::EditAnimTexStartCadrDisp->SetText("0");
}

void SXParticlesEditor::VelocityAccVisible(bool visible)
{
	SXParticlesEditor::StaticVelocity->Visible(visible);
	SXParticlesEditor::StaticVelocityX->Visible(visible);
	SXParticlesEditor::EditVelocityX->Visible(visible);
	SXParticlesEditor::StaticVelocityY->Visible(visible);
	SXParticlesEditor::EditVelocityY->Visible(visible);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->Visible(visible);
	SXParticlesEditor::StaticVelocityZ->Visible(visible);
	SXParticlesEditor::EditVelocityZ->Visible(visible);
	SXParticlesEditor::StaticVelocityDisp->Visible(visible);
	SXParticlesEditor::EditVelocityDisp->Visible(visible);
	SXParticlesEditor::CheckBoxVelocityDispYNeg->Visible(visible);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->Visible(visible);
	SXParticlesEditor::StaticAcceleration->Visible(visible);
	SXParticlesEditor::StaticAccelerationX->Visible(visible);
	SXParticlesEditor::EditAccelerationX->Visible(visible);
	SXParticlesEditor::StaticAccelerationY->Visible(visible);
	SXParticlesEditor::EditAccelerationY->Visible(visible);
	SXParticlesEditor::StaticAccelerationZ->Visible(visible);
	SXParticlesEditor::EditAccelerationZ->Visible(visible);
	SXParticlesEditor::StaticAccelerationDisp->Visible(visible);
	SXParticlesEditor::EditAccelerationDisp->Visible(visible);
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->Visible(visible);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->Visible(visible);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->Visible(visible);
}

void SXParticlesEditor::VelocityDataInit()
{
	if (SXParticlesEditor::SelEffID == -1)
	{
		return;
	}

	if (SXParticlesEditor::SelEmitterID == -1)
	{
		return;
	}

	SXParticlesEditor::EditVelocityX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.x)).c_str());
	SXParticlesEditor::EditVelocityY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.y)).c_str());
	SXParticlesEditor::EditVelocityZ->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.z)).c_str());
	SXParticlesEditor::EditVelocityDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDisp)).c_str());
	SXParticlesEditor::CheckBoxVelocityDispYNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispYNeg));
	SXParticlesEditor::CheckBoxVelocityDispZNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispZNeg));
	SXParticlesEditor::CheckBoxVelocityDispXNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispXNeg));
	SXParticlesEditor::EditAccelerationX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.x)).c_str());
	SXParticlesEditor::EditAccelerationY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.y)).c_str());
	SXParticlesEditor::EditAccelerationZ->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.z)).c_str());
	SXParticlesEditor::EditAccelerationDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDisp)).c_str());
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispXNeg));
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispYNeg));
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->SetCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispZNeg));
}

void SXParticlesEditor::VelocityAccNulling()
{
	SXParticlesEditor::EditVelocityX->SetText("0");
	SXParticlesEditor::EditVelocityY->SetText("0");
	SXParticlesEditor::EditVelocityZ->SetText("0");
	SXParticlesEditor::EditVelocityDisp->SetText("0");
	SXParticlesEditor::CheckBoxVelocityDispYNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->SetCheck(false);
	SXParticlesEditor::EditAccelerationX->SetText("0");
	SXParticlesEditor::EditAccelerationY->SetText("0");
	SXParticlesEditor::EditAccelerationZ->SetText("0");
	SXParticlesEditor::EditAccelerationDisp->SetText("0");
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->SetCheck(false);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->SetCheck(false);
}