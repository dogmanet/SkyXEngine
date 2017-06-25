
LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Вы действительно хотите выйти?", "Выход?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void SXParticlesEditor::EffReOpen()
{
	SPE_EffectsClear();
	Level::LoadParticles();

	SXParticlesEditor::EffInitList();
}

void SXParticlesEditor::EffSave()
{
	Level::SaveParticles();;
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//файл
	//открыть
	if (id == ID_FILE_REOPEN)
	{
		SXParticlesEditor::EffReOpen();
	}
	//сохранить
	else if (id == ID_FILE_SAVE)
	{
		SXParticlesEditor::EffSave();
	}
	//выход
	else if (id == ID_FILE_EXIT)
	{
		return TrueExit(hwnd, msg, wParam, lParam);
	}

	else if (id == ID_VIEW_GRID)
	{
		SXParticlesEditor::MainMenu->CheckItem(id, !SXParticlesEditor::MainMenu->GetCheckedItem(id));
		GData::Editors::RenderGrid = SXParticlesEditor::MainMenu->GetCheckedItem(id);
	}
	else if (id == ID_VIEW_AXES)
	{
		SXParticlesEditor::MainMenu->CheckItem(id, !SXParticlesEditor::MainMenu->GetCheckedItem(id));
		GData::Editors::RenderAxesStatic = SXParticlesEditor::MainMenu->GetCheckedItem(id);
	}
	else if (id == ID_VIEW_BOUND)
	{
		SXParticlesEditor::MainMenu->CheckItem(id, !SXParticlesEditor::MainMenu->GetCheckedItem(id));
		GData::Editors::RenderBound = SXParticlesEditor::MainMenu->GetCheckedItem(id);
	}

	return 0;
}

LRESULT SXParticlesEditor_ToolBar1_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (SXParticlesEditor::ButtonTBReOpen->GetHWND() == handle_elem)
		{
			SXParticlesEditor::EffReOpen();
		}
		else if (SXParticlesEditor::ButtonTBSave->GetHWND() == handle_elem)
		{
			SXParticlesEditor::EffSave();
		}
		

		else if (SXParticlesEditor::CheckBoxTBGrid->GetHWND() == handle_elem)
		{
			SXParticlesEditor::MainMenu->CheckItem(ID_VIEW_GRID, SXParticlesEditor::CheckBoxTBGrid->GetCheck());
			GData::Editors::RenderGrid = SXParticlesEditor::CheckBoxTBGrid->GetCheck();
		}
		else if (SXParticlesEditor::CheckBoxTBAxes->GetHWND() == handle_elem)
		{
			SXParticlesEditor::MainMenu->CheckItem(ID_VIEW_AXES, SXParticlesEditor::CheckBoxTBAxes->GetCheck());
			GData::Editors::RenderAxesStatic = SXParticlesEditor::CheckBoxTBAxes->GetCheck();
		}
		else if (SXParticlesEditor::CheckBoxTBBound->GetHWND() == handle_elem)
		{
			SXParticlesEditor::MainMenu->CheckItem(ID_VIEW_BOUND, SXParticlesEditor::CheckBoxTBBound->GetCheck());
			GData::Editors::RenderBound = SXParticlesEditor::CheckBoxTBBound->GetCheck();
		}

		else if (SXParticlesEditor::CheckBoxTBPlay->GetHWND() == handle_elem)
		{
			if (SXParticlesEditor::SelEffID >= 0)
			{
				if (SXParticlesEditor::SelEmitterID >= 0)
					SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);
				else
					SXParticlesEditor_ListBoxEffects_Click(hwnd, msg, wParam, lParam);
			}
			else
				SXParticlesEditor::CheckBoxTBPlay->SetCheck(false);

			SXParticlesEditor::CheckBoxTBPause->SetCheck(false);
			SXParticlesEditor::CheckBoxTBStop->SetCheck(false);
		}
		else if (SXParticlesEditor::CheckBoxTBPause->GetHWND() == handle_elem)
		{
			Core_TimeWorkingSet(G_Timer_Render_Scene, !SXParticlesEditor::CheckBoxTBPause->GetCheck());

			SXParticlesEditor::CheckBoxTBPlay->SetCheck(false);
			SXParticlesEditor::CheckBoxTBStop->SetCheck(false);
		}
		else if (SXParticlesEditor::CheckBoxTBStop->GetHWND() == handle_elem)
		{
			if (SXParticlesEditor::SelEffID >= 0)
				SPE_EffectEnableSet(SXParticlesEditor::SelEffID, false);

			SXParticlesEditor::CheckBoxTBPlay->SetCheck(false);
			SXParticlesEditor::CheckBoxTBPause->SetCheck(false);
		}
	}

	return 0;
}

LRESULT SXParticlesEditor_ButtonTBReOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SPE_EffectsClear();
	Level::LoadParticles();

	SXParticlesEditor::EffInitList();

	return 0;
}

LRESULT SXParticlesEditor_ButtonTBSave_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Level::SaveParticles();

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GData::ReSize = true;
	return 0;
}

///////

LRESULT SXParticlesEditor_ButtonTextureSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXParticlesEditor::EditTexture->SetText(tmpname);

		if (SXParticlesEditor::SelEffID >= 0 && SXParticlesEditor::SelEmitterID >= 0)
			SPE_EmitterTextureSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, tmpname);
	}
	return 0;
}

LRESULT SXParticlesEditor_GroupBoxData2_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);

	if (!(SXParticlesEditor::SelEffID >= 0 && SXParticlesEditor::SelEmitterID >= 0))
	{
		return 0;
	}

	if (Notification == BN_CLICKED)
	{
		HWND handle_elem = (HWND)(lParam);

		if (SXParticlesEditor::CheckBoxAccelerationDispXNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispXNeg, SXParticlesEditor::CheckBoxAccelerationDispXNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxAccelerationDispYNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispYNeg, SXParticlesEditor::CheckBoxAccelerationDispYNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxAccelerationDispZNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispZNeg, SXParticlesEditor::CheckBoxAccelerationDispZNeg->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxVelocityDispXNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispXNeg, SXParticlesEditor::CheckBoxVelocityDispXNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxVelocityDispYNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispYNeg, SXParticlesEditor::CheckBoxVelocityDispYNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxVelocityDispZNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispZNeg, SXParticlesEditor::CheckBoxVelocityDispZNeg->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXNeg, SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispXPos->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXPos, SXParticlesEditor::CheckBoxSpawnOriginDispXPos->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYNeg, SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispYPos->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYPos, SXParticlesEditor::CheckBoxSpawnOriginDispYPos->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZNeg, SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispZPos->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZPos, SXParticlesEditor::CheckBoxSpawnOriginDispZPos->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxLighting->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Lighting, SXParticlesEditor::CheckBoxLighting->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxFigureRotRand->GetHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureRotRand, SXParticlesEditor::CheckBoxFigureRotRand->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxFigureTapX->GetHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapX, SXParticlesEditor::CheckBoxFigureTapX->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxFigureTapY->GetHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapY, SXParticlesEditor::CheckBoxFigureTapY->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxFigureTapZ->GetHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapZ, SXParticlesEditor::CheckBoxFigureTapZ->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxCircle->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircle, SXParticlesEditor::CheckBoxCircle->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxCircleAngleDispNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDispNeg, SXParticlesEditor::CheckBoxCircleAngleDispNeg->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxRotate->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotate, SXParticlesEditor::CheckBoxRotate->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxRotateAngleDispNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDispNeg, SXParticlesEditor::CheckBoxRotateAngleDispNeg->GetCheck());
		}

		else if (SXParticlesEditor::CheckBoxDeviation->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviation, SXParticlesEditor::CheckBoxDeviation->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDispNeg, SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationTapX->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapX, SXParticlesEditor::CheckBoxDeviationTapX->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationTapY->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapY, SXParticlesEditor::CheckBoxDeviationTapY->GetCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationTapZ->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapZ, SXParticlesEditor::CheckBoxDeviationTapZ->GetCheck());
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		
		if (SXParticlesEditor::ComboBoxBoundType->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundType, (ParticlesBoundType)SXParticlesEditor::ComboBoxBoundType->GetSel());
		}
		else if (SXParticlesEditor::ComboBoxSpawnPosType->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnPosType, (ParticlesSpawnPosType)SXParticlesEditor::ComboBoxSpawnPosType->GetSel());
		}
		else if (SXParticlesEditor::ComboBoxFigureType->GetHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureType, (ParticlesFigureType)SXParticlesEditor::ComboBoxFigureType->GetSel());
		}
		else if (SXParticlesEditor::ComboBoxAlphaBlendType->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaBlendType, (ParticlesAlphaBlendType)SXParticlesEditor::ComboBoxAlphaBlendType->GetSel());
		}

		else if (SXParticlesEditor::ComboBoxAlphaDependAge->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaDependAge, (ParticlesDependType)SXParticlesEditor::ComboBoxAlphaDependAge->GetSel());
		}
		else if (SXParticlesEditor::ComboBoxSizeDependAge->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDependAge, (ParticlesDependType)SXParticlesEditor::ComboBoxSizeDependAge->GetSel());
		}

		else if (SXParticlesEditor::ComboBoxCircleAxis->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAxis, (ParticlesAxis)SXParticlesEditor::ComboBoxCircleAxis->GetSel());
		}

		else if (SXParticlesEditor::ComboBoxDeviationType->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationType, (ParticlesDeviationType)SXParticlesEditor::ComboBoxDeviationType->GetSel());
		}
		else if (SXParticlesEditor::ComboBoxDeviationAxis->GetHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAxis, (ParticlesAxis)SXParticlesEditor::ComboBoxDeviationAxis->GetSel());
		}
	}
	return 0;
}

LRESULT SXParticlesEditor_EditEffName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXParticlesEditor::SelEffID < 0)
	{
		return 0;
	}

	char ttext[1024];
	GetWindowText(hwnd, ttext, 1024);
	if (def_str_validate(ttext))
	{
		SPE_EffectNameSet(SXParticlesEditor::SelEffID, ttext);
		SXParticlesEditor::ListBoxEffects->SetTextItem(SXParticlesEditor::ListBoxEffects->GetSel(), ttext);
	}
	else
	{
		SPE_EffectNameGet(SXParticlesEditor::SelEffID, ttext);
		SXParticlesEditor::EditEffName->SetText(ttext);
		MessageBox(0, "unresolved name", 0, 0);
	}
	return 0;
}

LRESULT SXParticlesEditor_Edits_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char ttext[1024];
	GetWindowText(hwnd, ttext, 1024);
	int tint = 0;
	long tlong = 0;
	float tfloat = 0.f;

	if (SXParticlesEditor::SelEffID < 0 || SXParticlesEditor::SelEmitterID < 0)
	{
		return 0;
	}

	if (hwnd == SXParticlesEditor::EditName->GetHWND())
	{
		if (def_str_validate(ttext))
		{
			SPE_EmitterNameSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ttext);
			SXParticlesEditor::ListBoxEmitters->SetTextItem(SXParticlesEditor::ListBoxEmitters->GetSel(), ttext);
		}
		else
		{
			SPE_EmitterNameGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ttext);
			SXParticlesEditor::EditName->SetText(ttext);
			MessageBox(0,"unresolved name",0,0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditCount->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint > 0)
			SPE_EmitterCountSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, tint);
		else
		{
			MessageBox(0, "unresolved count particles", 0, 0);
			SXParticlesEditor::EditCount->SetText(String(SPE_EmitterCountGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID)).c_str());
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditColorCoef->GetHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ColorCoef, String(ttext).ToDouble());
	}
	else if (hwnd == SXParticlesEditor::EditReCreateCount->GetHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ReCreateCount, String(ttext).ToInt());
	}

	else if (hwnd == SXParticlesEditor::EditSoftCoef->GetHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SoftCoef, String(ttext).ToDouble());
	}
	else if (hwnd == SXParticlesEditor::EditRefractionCoef->GetHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, RefractionCoef, String(ttext).ToDouble());
	}
	else if (hwnd == SXParticlesEditor::EditTransparencyCoef->GetHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TransparencyCoef, String(ttext).ToDouble());
	}

	else if (hwnd == SXParticlesEditor::EditFigureCountQuads->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint > 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureCountQuads, tint)
		else
		{
			SXParticlesEditor::EditFigureCountQuads->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureCountQuads)).c_str());
			MessageBox(0, "unresolved count quads", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditTimeLife->GetHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLife, tlong)
		else
		{
			SXParticlesEditor::EditTimeLife->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLife)).c_str());
			MessageBox(0, "unresolved time", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditTimeLifeDisp->GetHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLifeDisp, tlong)
		else
		{
			SXParticlesEditor::EditTimeLifeDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLifeDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditSizeX->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat > 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.x, tfloat)
		else
		{
			SXParticlesEditor::EditSizeX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.x)).c_str());
			MessageBox(0, "unresolved size", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditSizeY->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat > 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.y, tfloat)
		else
		{
			SXParticlesEditor::EditSizeY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.y)).c_str());
			MessageBox(0, "unresolved size", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditSizeDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDisp, tfloat)
		else
		{
			SXParticlesEditor::EditSizeDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditAnimTexCountCadrsX->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsX, tint)
		else
		{
			SXParticlesEditor::EditAnimTexCountCadrsX->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsX)).c_str());
			MessageBox(0, "unresolved count cadrs", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexCountCadrsY->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsY, tint)
		else
		{
			SXParticlesEditor::EditAnimTexCountCadrsY->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsY)).c_str());
			MessageBox(0, "unresolved count cadrs", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexRate->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRate, tint)
		else
		{
			SXParticlesEditor::EditAnimTexRate->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRate)).c_str());
			MessageBox(0, "unresolved rate", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexRateDisp->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRateDisp, tint)
		else
		{
			SXParticlesEditor::EditAnimTexRateDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRateDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexStartCadr->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadr, tint)
		else
		{
			SXParticlesEditor::EditAnimTexStartCadr->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadr)).c_str());
			MessageBox(0, "unresolved start cadr", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexStartCadrDisp->GetHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadrDisp, tint)
		else
		{
			SXParticlesEditor::EditAnimTexStartCadrDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadrDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditSpawnOriginX->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditSpawnOriginY->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditSpawnOriginZ->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditSpawnOriginDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat >= 0.f)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp, tfloat)
		else
		{
			SXParticlesEditor::EditSpawnOriginDisp->SetText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}

		tfloat = SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp);
		int qwert = 0;
	}
	else if (hwnd == SXParticlesEditor::EditSpawnNextTime->GetHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTime, tlong)
		else
		{
			SXParticlesEditor::EditSpawnNextTime->SetText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTime)).c_str());
			MessageBox(0, "unresolved time", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditSpawnNextTimeDisp->GetHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTimeDisp, tlong)
		else
		{
			SXParticlesEditor::EditSpawnNextTimeDisp->SetText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTimeDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditBoundVec1X->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec1Y->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec1Z->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec1W->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.w, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2X->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2Y->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2Z->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2W->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.w, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditVelocityX->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditVelocityY->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditVelocityZ->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditVelocityDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditAccelerationX->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditAccelerationY->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditAccelerationZ->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditAccelerationDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditCircleAngle->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngle, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditCircleAngleDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditRotateAngle->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngle, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditRotateAngleDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditDeviationAmplitude->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAmplitude, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditDeviationCoefAngle->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngle, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditDeviationCoefAngleDisp->GetHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDisp, tfloat);
	}
	return 0;
}