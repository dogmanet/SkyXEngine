
#include "particles_editor.h"

void SXParticlesEditor::EffInitList()
{
	SXParticlesEditor::ListBoxEffects->clear();
	SXParticlesEditor::ListBoxEmitters->clear();
	int effcount = SPE_EffectCountGet();
	char effname[OBJECT_NAME_MAX_LEN];
	for (int i = 0; i < effcount; ++i)
	{
		effname[0] = '!';
		effname[1] = 0;
		SPE_EffectNameGet(SPE_EffectIdOfKey(i), effname);
		SXParticlesEditor::ListBoxEffects->addItem(effname);
		if (effname[0] == 0)
			effname[0] = '!';
	}

	SXParticlesEditor::StaticEffectsCount->setText(String(effcount).c_str());
	SXParticlesEditor::StaticEmittersCount->setText("0");
	SXParticlesEditor::TabsVisible(false);

	SXParticlesEditor::SelEffID = SXParticlesEditor::SelEmitterID = -1;
}

void SXParticlesEditor::EffVisible(bool visible, bool iscreate)
{
	StaticEffName->setVisible(visible);
	EditEffName->setVisible(visible);
	ButtonEffCreate->setVisible(iscreate);
}

void SXParticlesEditor::EffNulling()
{
	EditEffName->setText("0");
}

void SXParticlesEditor::EffDataInit()
{
	char effname[OBJECT_NAME_MAX_LEN];
	SPE_EffectNameGet(SXParticlesEditor::SelEffID, effname);
	SXParticlesEditor::EditEffName->setText(effname);
}

void SXParticlesEditor::TabsVisible(bool visible)
{
	SXParticlesEditor::GroupBoxTabs->setVisible(visible);

	if (visible == false)
	{
		AllInTabsVisible(false);
	}
	else if (!(SXParticlesEditor::GroupBoxTabs->getVisible()) && visible)
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
		SXParticlesEditor::ButtonBase->setCheck(false);
		SXParticlesEditor::ButtonBound->setCheck(false);
		SXParticlesEditor::ButtonCharacters->setCheck(false);
		SXParticlesEditor::ButtonSpawn->setCheck(false);
		SXParticlesEditor::ButtonTextureAnimTex->setCheck(false);
		SXParticlesEditor::ButtonVelocityAcceleration->setCheck(false);
		SXParticlesEditor::ButtonEmitterCreate->setVisible(false);
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
	SXParticlesEditor::ButtonEmitterCreate->setVisible(visible);
}

void SXParticlesEditor::BaseVisible(bool visible)
{
	StaticName->setVisible(visible);
	EditName->setVisible(visible);
	
	StaticReCreateCount->setVisible(visible);
	EditReCreateCount->setVisible(visible);
	StaticCount->setVisible(visible);
	EditCount->setVisible(visible);
	StaticColorCoef->setVisible(visible);
	EditColorCoef->setVisible(visible);
	StaticSoftCoef->setVisible(visible);
	EditSoftCoef->setVisible(visible);
	StaticRefractionCoef->setVisible(visible);
	EditRefractionCoef->setVisible(visible);
	StaticTransparencyCoef->setVisible(visible);
	EditTransparencyCoef->setVisible(visible);
	CheckBoxLighting->setVisible(visible);
	StaticFigureType->setVisible(visible);
	ComboBoxFigureType->setVisible(visible);
	StaticFigureCountQuads->setVisible(visible);
	EditFigureCountQuads->setVisible(visible);
	CheckBoxFigureRotRand->setVisible(visible);
	CheckBoxFigureTapX->setVisible(visible);
	CheckBoxFigureTapY->setVisible(visible);
	CheckBoxFigureTapZ->setVisible(visible);
	StaticAlphaBlendType->setVisible(visible);
	ComboBoxAlphaBlendType->setVisible(visible);
	StaticTimeLife->setVisible(visible);
	EditTimeLife->setVisible(visible);
	StaticTimeLifeDisp->setVisible(visible);
	EditTimeLifeDisp->setVisible(visible);
	StaticAlphaAgeDepend->setVisible(visible);
	ComboBoxAlphaDependAge->setVisible(visible);
	StaticSize->setVisible(visible);
	StaticSizeX->setVisible(visible);
	EditSizeX->setVisible(visible);
	StaticSizeY->setVisible(visible);
	EditSizeY->setVisible(visible);
	StaticSizeDisp->setVisible(visible);
	EditSizeDisp->setVisible(visible);
	StaticSizeDependAge->setVisible(visible);
	ComboBoxSizeDependAge->setVisible(visible);
	CheckBoxCollisionDelete->setVisible(visible);

	SXParticlesEditor::StaticColor->setVisible(visible);
	SXParticlesEditor::StaticColorR->setVisible(visible);
	SXParticlesEditor::EditColorR->setVisible(visible);
	SXParticlesEditor::StaticColorG->setVisible(visible);
	SXParticlesEditor::EditColorG->setVisible(visible);
	SXParticlesEditor::StaticColorB->setVisible(visible);
	SXParticlesEditor::EditColorB->setVisible(visible);
	SXParticlesEditor::StaticColorA->setVisible(visible);
	SXParticlesEditor::EditColorA->setVisible(visible);

	SXParticlesEditor::CheckBoxTrack->setVisible(visible);
	SXParticlesEditor::StaticTrackSize->setVisible(visible);
	SXParticlesEditor::EditTrackSize->setVisible(visible);
	SXParticlesEditor::StaticTrackTime->setVisible(visible);
	SXParticlesEditor::EditTrackTime->setVisible(visible);
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
	SXParticlesEditor::EditName->setText(tmpname);
	SXParticlesEditor::EditCount->setText(String(SPE_EmitterCountGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID)).c_str());
	SXParticlesEditor::EditReCreateCount->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ReCreateCount)).c_str());
	SXParticlesEditor::EditColorCoef->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ColorCoef)).c_str());
	SXParticlesEditor::EditSoftCoef->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SoftCoef)).c_str());
	SXParticlesEditor::EditRefractionCoef->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, RefractionCoef)).c_str());
	SXParticlesEditor::EditTransparencyCoef->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TransparencyCoef)).c_str());
	SXParticlesEditor::CheckBoxLighting->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Lighting));
	SXParticlesEditor::ComboBoxFigureType->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureType));
	SXParticlesEditor::ComboBoxAlphaBlendType->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaBlendType));
	SXParticlesEditor::EditFigureCountQuads->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureCountQuads)).c_str());
	SXParticlesEditor::CheckBoxFigureRotRand->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureRotRand));
	SXParticlesEditor::CheckBoxFigureTapX->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapX));
	SXParticlesEditor::CheckBoxFigureTapY->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapY));
	SXParticlesEditor::CheckBoxFigureTapZ->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapZ));
	SXParticlesEditor::ComboBoxAlphaBlendType->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaBlendType));
	SXParticlesEditor::EditTimeLife->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLife)).c_str());
	SXParticlesEditor::EditTimeLifeDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLifeDisp)).c_str());
	SXParticlesEditor::ComboBoxAlphaDependAge->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaDependAge));
	SXParticlesEditor::EditSizeX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.x)).c_str());
	SXParticlesEditor::EditSizeY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.y)).c_str());
	SXParticlesEditor::EditSizeDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDisp)).c_str());
	SXParticlesEditor::ComboBoxSizeDependAge->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDependAge));
	SXParticlesEditor::CheckBoxCollisionDelete->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CollisionDelete));

	SXParticlesEditor::EditColorR->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.x)).c_str());
	SXParticlesEditor::EditColorG->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.y)).c_str());
	SXParticlesEditor::EditColorB->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.z)).c_str());
	SXParticlesEditor::EditColorA->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.w)).c_str());

	SXParticlesEditor::CheckBoxTrack->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Track));
	SXParticlesEditor::EditTrackSize->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TrackSize)).c_str());
	SXParticlesEditor::EditTrackTime->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TrackTime)).c_str());
}

void SXParticlesEditor::BaseNulling()
{
	SXParticlesEditor::EditName->setText("");
	SXParticlesEditor::EditCount->setText("0");
	SXParticlesEditor::EditReCreateCount->setText("0");
	SXParticlesEditor::EditColorCoef->setText("1");
	SXParticlesEditor::EditSoftCoef->setText("0");
	SXParticlesEditor::EditRefractionCoef->setText("0");
	SXParticlesEditor::EditTransparencyCoef->setText("1");
	SXParticlesEditor::CheckBoxLighting->setCheck(false);
	SXParticlesEditor::ComboBoxFigureType->setSel(0);
	SXParticlesEditor::EditFigureCountQuads->setText("1");
	SXParticlesEditor::CheckBoxFigureRotRand->setCheck(false);
	SXParticlesEditor::CheckBoxFigureTapX->setCheck(false);
	SXParticlesEditor::CheckBoxFigureTapY->setCheck(false);
	SXParticlesEditor::CheckBoxFigureTapZ->setCheck(false);
	SXParticlesEditor::ComboBoxAlphaBlendType->setSel(0);
	SXParticlesEditor::EditTimeLife->setText("0");
	SXParticlesEditor::EditTimeLifeDisp->setText("0");
	SXParticlesEditor::ComboBoxAlphaDependAge->setSel(0);
	SXParticlesEditor::EditSizeX->setText("0.5");
	SXParticlesEditor::EditSizeY->setText("0.5");
	SXParticlesEditor::EditSizeDisp->setText("0");
	SXParticlesEditor::ComboBoxSizeDependAge->setSel(0);
	SXParticlesEditor::CheckBoxCollisionDelete->setCheck(false);

	SXParticlesEditor::EditColorR->setText("1");
	SXParticlesEditor::EditColorG->setText("1");
	SXParticlesEditor::EditColorB->setText("1");
	SXParticlesEditor::EditColorA->setText("0");

	SXParticlesEditor::CheckBoxTrack->setCheck(false);
	SXParticlesEditor::EditTrackSize->setText("0");
	SXParticlesEditor::EditTrackTime->setText("0");
}

void SXParticlesEditor::BoundVisible(bool visible)
{
	SXParticlesEditor::StaticBoundType->setVisible(visible);
	SXParticlesEditor::ComboBoxBoundType->setVisible(visible);
	SXParticlesEditor::StaticBoundVec1->setVisible(visible);
	SXParticlesEditor::StaticBoundVec1X->setVisible(visible);
	SXParticlesEditor::EditBoundVec1X->setVisible(visible);
	SXParticlesEditor::StaticBoundVec1Y->setVisible(visible);
	SXParticlesEditor::EditBoundVec1Y->setVisible(visible);
	SXParticlesEditor::StaticBoundVec1Z->setVisible(visible);
	SXParticlesEditor::EditBoundVec1Z->setVisible(visible);
	SXParticlesEditor::StaticBoundVec1W->setVisible(visible);
	SXParticlesEditor::EditBoundVec1W->setVisible(visible);
	SXParticlesEditor::StaticBoundVec2->setVisible(visible);
	SXParticlesEditor::StaticBoundVec2X->setVisible(visible);
	SXParticlesEditor::EditBoundVec2X->setVisible(visible);
	SXParticlesEditor::StaticBoundVec2Y->setVisible(visible);
	SXParticlesEditor::EditBoundVec2Y->setVisible(visible);
	SXParticlesEditor::StaticBoundVec2Z->setVisible(visible);
	SXParticlesEditor::EditBoundVec2Z->setVisible(visible);
	SXParticlesEditor::StaticBoundVec2W->setVisible(visible);
	SXParticlesEditor::EditBoundVec2W->setVisible(visible);
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

	SXParticlesEditor::ComboBoxBoundType->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundType));
	SXParticlesEditor::EditBoundVec1X->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.x)).c_str());
	SXParticlesEditor::EditBoundVec1Y->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.y)).c_str());
	SXParticlesEditor::EditBoundVec1Z->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.z)).c_str());
	SXParticlesEditor::EditBoundVec1W->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.w)).c_str());
	SXParticlesEditor::EditBoundVec2X->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.x)).c_str());
	SXParticlesEditor::EditBoundVec2Y->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.y)).c_str());
	SXParticlesEditor::EditBoundVec2Z->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.z)).c_str());
	SXParticlesEditor::EditBoundVec2W->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.w)).c_str());
}

void SXParticlesEditor::BoundNulling()
{
	SXParticlesEditor::ComboBoxBoundType->setSel(0);
	SXParticlesEditor::EditBoundVec1X->setText("0");
	SXParticlesEditor::EditBoundVec1Y->setText("0");
	SXParticlesEditor::EditBoundVec1Z->setText("0");
	SXParticlesEditor::EditBoundVec1W->setText("0");
	SXParticlesEditor::EditBoundVec2X->setText("0");
	SXParticlesEditor::EditBoundVec2Y->setText("0");
	SXParticlesEditor::EditBoundVec2Z->setText("0");
	SXParticlesEditor::EditBoundVec2W->setText("0");
}

void SXParticlesEditor::CharacterVisible(bool visible)
{
	SXParticlesEditor::CheckBoxCircle->setVisible(visible);
	SXParticlesEditor::StaticCircleAxis->setVisible(visible);
	SXParticlesEditor::ComboBoxCircleAxis->setVisible(visible);
	SXParticlesEditor::StaticCircleAngle->setVisible(visible);
	SXParticlesEditor::EditCircleAngle->setVisible(visible);
	SXParticlesEditor::StaticCircleAngleDisp->setVisible(visible);
	SXParticlesEditor::EditCircleAngleDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxRotate->setVisible(visible);
	SXParticlesEditor::StaticRotateAngle->setVisible(visible);
	SXParticlesEditor::EditRotateAngle->setVisible(visible);
	SXParticlesEditor::StaticRotateAngleDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setVisible(visible);
	SXParticlesEditor::EditRotateAngleDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxDeviation->setVisible(visible);
	SXParticlesEditor::StaticDeviationType->setVisible(visible);
	SXParticlesEditor::ComboBoxDeviationType->setVisible(visible);
	SXParticlesEditor::StaticDeviationAmplitude->setVisible(visible);
	SXParticlesEditor::StaticDeviationCoefAngle->setVisible(visible);
	SXParticlesEditor::EditDeviationAmplitude->setVisible(visible);
	SXParticlesEditor::EditDeviationCoefAngle->setVisible(visible);
	SXParticlesEditor::StaticDeviationAxis->setVisible(visible);
	SXParticlesEditor::ComboBoxDeviationAxis->setVisible(visible);
	SXParticlesEditor::StaticDeviationCountDelayMls->setVisible(visible);
	SXParticlesEditor::EditDeviationCountDelayMls->setVisible(visible);
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setVisible(visible);
	SXParticlesEditor::EditDeviationCoefAngleDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxDeviationTapX->setVisible(visible);
	SXParticlesEditor::CheckBoxDeviationTapY->setVisible(visible);
	SXParticlesEditor::CheckBoxDeviationTapZ->setVisible(visible);
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

	SXParticlesEditor::CheckBoxCircle->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircle));
	SXParticlesEditor::ComboBoxCircleAxis->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAxis));
	SXParticlesEditor::EditCircleAngle->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngle)).c_str());
	SXParticlesEditor::EditCircleAngleDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDisp)).c_str());
	SXParticlesEditor::CheckBoxRotate->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotate));
	SXParticlesEditor::EditRotateAngle->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngle)).c_str());
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDispNeg));
	SXParticlesEditor::EditRotateAngleDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDisp)).c_str());
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDispNeg));
	SXParticlesEditor::CheckBoxDeviation->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviation));
	SXParticlesEditor::ComboBoxDeviationType->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationType));
	SXParticlesEditor::EditDeviationAmplitude->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAmplitude)).c_str());
	SXParticlesEditor::EditDeviationCoefAngle->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngle)).c_str());
	SXParticlesEditor::ComboBoxDeviationAxis->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAxis));
	SXParticlesEditor::EditDeviationCountDelayMls->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCountDelayMls)).c_str());
	SXParticlesEditor::EditDeviationCoefAngleDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDisp)).c_str());
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDispNeg));
	SXParticlesEditor::CheckBoxDeviationTapX->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapX));
	SXParticlesEditor::CheckBoxDeviationTapY->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapY));
	SXParticlesEditor::CheckBoxDeviationTapZ->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapZ));
}

void SXParticlesEditor::CharacterNulling()
{
	SXParticlesEditor::CheckBoxCircle->setCheck(false);
	SXParticlesEditor::ComboBoxCircleAxis->setSel(0);
	SXParticlesEditor::EditCircleAngle->setText("0");
	SXParticlesEditor::EditCircleAngleDisp->setText("0");
	SXParticlesEditor::CheckBoxRotate->setCheck(false);
	SXParticlesEditor::EditRotateAngle->setText("0");
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setCheck(false);
	SXParticlesEditor::EditRotateAngleDisp->setText("0");
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setCheck(false);
	SXParticlesEditor::CheckBoxDeviation->setCheck(false);
	SXParticlesEditor::ComboBoxDeviationType->setSel(0);
	SXParticlesEditor::EditDeviationAmplitude->setText("0");
	SXParticlesEditor::EditDeviationCoefAngle->setText("0");
	SXParticlesEditor::ComboBoxDeviationAxis->setSel(0);
	SXParticlesEditor::EditDeviationCountDelayMls->setText("0");
	SXParticlesEditor::EditDeviationCoefAngleDisp->setText("0");
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setCheck(false);
	SXParticlesEditor::CheckBoxDeviationTapX->setCheck(false);
	SXParticlesEditor::CheckBoxDeviationTapY->setCheck(false);
	SXParticlesEditor::CheckBoxDeviationTapZ->setCheck(false);
}

void SXParticlesEditor::SpawnVisible(bool visible)
{
	SXParticlesEditor::StaticSpawnPosType->setVisible(visible);
	SXParticlesEditor::ComboBoxSpawnPosType->setVisible(visible);
	SXParticlesEditor::StaticSpawnOrigin->setVisible(visible);
	SXParticlesEditor::StaticSpawnOriginX->setVisible(visible);
	SXParticlesEditor::EditSpawnOriginX->setVisible(visible);
	SXParticlesEditor::StaticSpawnOriginY->setVisible(visible);
	SXParticlesEditor::EditSpawnOriginY->setVisible(visible);
	SXParticlesEditor::StaticSpawnOriginZ->setVisible(visible);
	SXParticlesEditor::EditSpawnOriginZ->setVisible(visible);
	SXParticlesEditor::StaticSpawnOriginDisp->setVisible(visible);
	SXParticlesEditor::EditSpawnOriginDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setVisible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setVisible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setVisible(visible);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setVisible(visible);
	SXParticlesEditor::StaticSpawnNextTime->setVisible(visible);
	SXParticlesEditor::EditSpawnNextTime->setVisible(visible);
	SXParticlesEditor::StaticSpawnNextTimeDisp->setVisible(visible);
	SXParticlesEditor::EditSpawnNextTimeDisp->setVisible(visible);
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

	SXParticlesEditor::ComboBoxSpawnPosType->setSel(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnPosType));
	SXParticlesEditor::EditSpawnOriginX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.x)).c_str());
	SXParticlesEditor::EditSpawnOriginY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.y)).c_str());
	SXParticlesEditor::EditSpawnOriginZ->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.z)).c_str());
	SXParticlesEditor::EditSpawnOriginDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp)).c_str());
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXPos));
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXNeg));
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYPos));
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYNeg));
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZPos));
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZNeg));
	SXParticlesEditor::EditSpawnNextTime->setText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTime)).c_str());
	SXParticlesEditor::EditSpawnNextTimeDisp->setText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTimeDisp)).c_str());
}

void SXParticlesEditor::SpawnNulling()
{
	SXParticlesEditor::ComboBoxSpawnPosType->setSel(0);
	SXParticlesEditor::EditSpawnOriginX->setText("0");
	SXParticlesEditor::EditSpawnOriginY->setText("0");
	SXParticlesEditor::EditSpawnOriginZ->setText("0");
	SXParticlesEditor::EditSpawnOriginDisp->setText("0");
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setCheck(false);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setCheck(false);
	SXParticlesEditor::EditSpawnNextTime->setText("0");
	SXParticlesEditor::EditSpawnNextTimeDisp->setText("0");
}

void SXParticlesEditor::TexVisible(bool visible)
{
	SXParticlesEditor::StaticTexture->setVisible(visible);
	SXParticlesEditor::EditTexture->setVisible(visible);
	SXParticlesEditor::ButtonTextureSel->setVisible(visible);
	SXParticlesEditor::StaticAnimTexCountCadrsX->setVisible(visible);
	SXParticlesEditor::StaticAnimTexCountCadrsY->setVisible(visible);
	SXParticlesEditor::EditAnimTexCountCadrsX->setVisible(visible);
	SXParticlesEditor::EditAnimTexCountCadrsY->setVisible(visible);
	SXParticlesEditor::StaticAnimTexRate->setVisible(visible);
	SXParticlesEditor::EditAnimTexRate->setVisible(visible);
	SXParticlesEditor::StaticAnimTexRateDisp->setVisible(visible);
	SXParticlesEditor::EditAnimTexRateDisp->setVisible(visible);
	SXParticlesEditor::StaticAnimTexStartCadr->setVisible(visible);
	SXParticlesEditor::EditAnimTexStartCadr->setVisible(visible);
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setVisible(visible);
	SXParticlesEditor::EditAnimTexStartCadrDisp->setVisible(visible);
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

	SXParticlesEditor::EditTexture->setText(tmptex);
	SXParticlesEditor::EditAnimTexCountCadrsX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsX)).c_str());
	SXParticlesEditor::EditAnimTexCountCadrsY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsY)).c_str());
	SXParticlesEditor::EditAnimTexRate->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRate)).c_str());
	SXParticlesEditor::EditAnimTexRateDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRateDisp)).c_str());
	SXParticlesEditor::EditAnimTexStartCadr->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadr)).c_str());
	SXParticlesEditor::EditAnimTexStartCadrDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadrDisp)).c_str());
}

void SXParticlesEditor::TexNulling()
{
	SXParticlesEditor::EditTexture->setText("");
	SXParticlesEditor::EditAnimTexCountCadrsX->setText("0");
	SXParticlesEditor::EditAnimTexCountCadrsY->setText("0");
	SXParticlesEditor::EditAnimTexRate->setText("0");
	SXParticlesEditor::EditAnimTexRateDisp->setText("0");
	SXParticlesEditor::EditAnimTexStartCadr->setText("0");
	SXParticlesEditor::EditAnimTexStartCadrDisp->setText("0");
}

void SXParticlesEditor::VelocityAccVisible(bool visible)
{
	SXParticlesEditor::StaticVelocity->setVisible(visible);
	SXParticlesEditor::StaticVelocityX->setVisible(visible);
	SXParticlesEditor::EditVelocityX->setVisible(visible);
	SXParticlesEditor::StaticVelocityY->setVisible(visible);
	SXParticlesEditor::EditVelocityY->setVisible(visible);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setVisible(visible);
	SXParticlesEditor::StaticVelocityZ->setVisible(visible);
	SXParticlesEditor::EditVelocityZ->setVisible(visible);
	SXParticlesEditor::StaticVelocityDisp->setVisible(visible);
	SXParticlesEditor::EditVelocityDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setVisible(visible);
	SXParticlesEditor::StaticAcceleration->setVisible(visible);
	SXParticlesEditor::StaticAccelerationX->setVisible(visible);
	SXParticlesEditor::EditAccelerationX->setVisible(visible);
	SXParticlesEditor::StaticAccelerationY->setVisible(visible);
	SXParticlesEditor::EditAccelerationY->setVisible(visible);
	SXParticlesEditor::StaticAccelerationZ->setVisible(visible);
	SXParticlesEditor::EditAccelerationZ->setVisible(visible);
	SXParticlesEditor::StaticAccelerationDisp->setVisible(visible);
	SXParticlesEditor::EditAccelerationDisp->setVisible(visible);
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setVisible(visible);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setVisible(visible);
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

	SXParticlesEditor::EditVelocityX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.x)).c_str());
	SXParticlesEditor::EditVelocityY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.y)).c_str());
	SXParticlesEditor::EditVelocityZ->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.z)).c_str());
	SXParticlesEditor::EditVelocityDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDisp)).c_str());
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispYNeg));
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispZNeg));
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispXNeg));
	SXParticlesEditor::EditAccelerationX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.x)).c_str());
	SXParticlesEditor::EditAccelerationY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.y)).c_str());
	SXParticlesEditor::EditAccelerationZ->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.z)).c_str());
	SXParticlesEditor::EditAccelerationDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDisp)).c_str());
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispXNeg));
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispYNeg));
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setCheck(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispZNeg));
}

void SXParticlesEditor::VelocityAccNulling()
{
	SXParticlesEditor::EditVelocityX->setText("0");
	SXParticlesEditor::EditVelocityY->setText("0");
	SXParticlesEditor::EditVelocityZ->setText("0");
	SXParticlesEditor::EditVelocityDisp->setText("0");
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setCheck(false);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setCheck(false);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setCheck(false);
	SXParticlesEditor::EditAccelerationX->setText("0");
	SXParticlesEditor::EditAccelerationY->setText("0");
	SXParticlesEditor::EditAccelerationZ->setText("0");
	SXParticlesEditor::EditAccelerationDisp->setText("0");
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setCheck(false);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setCheck(false);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setCheck(false);
}