
#include "callback_common.h"

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Exit?", "Exit?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

void SXParticlesEditor::EffReOpen()
{
	SPE_EffectsClear();
	SLevel_LoadParticles();

	SXParticlesEditor::EffInitList();
}

void SXParticlesEditor::EffSave()
{
	SLevel_SaveParticles();
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
		SXParticlesEditor::MainMenu->setCheckItem(id, !SXParticlesEditor::MainMenu->getCheckedItem(id));
		SRender_EditorSetRenderGrid(SXParticlesEditor::MainMenu->getCheckedItem(id));
	}
	else if (id == ID_VIEW_AXES)
	{
		SXParticlesEditor::MainMenu->setCheckItem(id, !SXParticlesEditor::MainMenu->getCheckedItem(id));
		SRender_EditorSetRenderAxesStatic(SXParticlesEditor::MainMenu->getCheckedItem(id));
	}
	else if (id == ID_VIEW_BOUND)
	{
		SXParticlesEditor::MainMenu->setCheckItem(id, !SXParticlesEditor::MainMenu->getCheckedItem(id));
		SXParticlesEditor::canRenderBound = SXParticlesEditor::MainMenu->getCheckedItem(id);
	}

	return 0;
}

LRESULT SXParticlesEditor_ToolBar1_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int Notification = HIWORD(wParam);
	HWND handle_elem = (HWND)(lParam);
	if (Notification == BN_CLICKED)
	{
		if (SXParticlesEditor::ButtonTBReOpen->getHWND() == handle_elem)
		{
			SXParticlesEditor::EffReOpen();
		}
		else if (SXParticlesEditor::ButtonTBSave->getHWND() == handle_elem)
		{
			SXParticlesEditor::EffSave();
		}
		

		else if (SXParticlesEditor::CheckBoxTBGrid->getHWND() == handle_elem)
		{
			SXParticlesEditor::MainMenu->setCheckItem(ID_VIEW_GRID, SXParticlesEditor::CheckBoxTBGrid->getCheck());
			SRender_EditorSetRenderGrid(SXParticlesEditor::CheckBoxTBGrid->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxTBAxes->getHWND() == handle_elem)
		{
			SXParticlesEditor::MainMenu->setCheckItem(ID_VIEW_AXES, SXParticlesEditor::CheckBoxTBAxes->getCheck());
			SRender_EditorSetRenderAxesStatic(SXParticlesEditor::CheckBoxTBAxes->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxTBBound->getHWND() == handle_elem)
		{
			SXParticlesEditor::MainMenu->setCheckItem(ID_VIEW_BOUND, SXParticlesEditor::CheckBoxTBBound->getCheck());
			SXParticlesEditor::canRenderBound = SXParticlesEditor::CheckBoxTBBound->getCheck();
		}

		else if (SXParticlesEditor::CheckBoxTBPlay->getHWND() == handle_elem)
		{
			Core_TimeWorkingSet(Core_RIntGet(G_RI_INT_TIMER_RENDER), SXParticlesEditor::CheckBoxTBPlay->getCheck());
			if (SXParticlesEditor::SelEffID >= 0)
			{
				if (SXParticlesEditor::SelEmitterID >= 0 && !SPE_EmitterEnableGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID))
					SXParticlesEditor_ListBoxEmitters_Click(hwnd, msg, wParam, lParam);
				else if (!SPE_EffectEnableGet(SXParticlesEditor::SelEffID))
					SXParticlesEditor_ListBoxEffects_Click(hwnd, msg, wParam, lParam);
			}
			else
				SXParticlesEditor::CheckBoxTBPlay->setCheck(false);

			SXParticlesEditor::CheckBoxTBPause->setCheck(false);
			SXParticlesEditor::CheckBoxTBStop->setCheck(false);
		}
		else if (SXParticlesEditor::CheckBoxTBPause->getHWND() == handle_elem)
		{
			if (SXParticlesEditor::CheckBoxTBPlay->getCheck())
				Core_TimeWorkingSet(Core_RIntGet(G_RI_INT_TIMER_RENDER), !SXParticlesEditor::CheckBoxTBPause->getCheck());
			else
				SXParticlesEditor::CheckBoxTBPause->setCheck(false);

			SXParticlesEditor::CheckBoxTBPlay->setCheck(false);
			SXParticlesEditor::CheckBoxTBStop->setCheck(false);
		}
		else if (SXParticlesEditor::CheckBoxTBStop->getHWND() == handle_elem)
		{
			Core_TimeWorkingSet(Core_RIntGet(G_RI_INT_TIMER_RENDER), true);
			if (SXParticlesEditor::SelEffID >= 0)
				SPE_EffectEnableSet(SXParticlesEditor::SelEffID, false);

			SXParticlesEditor::CheckBoxTBPlay->setCheck(false);
			SXParticlesEditor::CheckBoxTBPause->setCheck(false);
		}
	}

	return 0;
}

LRESULT SXParticlesEditor_ButtonTBReOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SPE_EffectsClear();
	SLevel_LoadParticles();

	SXParticlesEditor::EffInitList();

	return 0;
}

LRESULT SXParticlesEditor_ButtonTBSave_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SLevel_SaveParticles();

	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
		r_resize = (int*)GET_PCVAR_INT("r_resize");

	if (!r_resize)
		return 0;

	*r_resize = RENDER_RESIZE_RESIZE;
	return 0;
}

///////

LRESULT SXParticlesEditor_ButtonTextureSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXParticlesEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXParticlesEditor::EditTexture->setText(tmpname);

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

		if (SXParticlesEditor::CheckBoxTrack->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Track, SXParticlesEditor::CheckBoxTrack->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxAccelerationDispXNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispXNeg, SXParticlesEditor::CheckBoxAccelerationDispXNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxAccelerationDispYNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispYNeg, SXParticlesEditor::CheckBoxAccelerationDispYNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxAccelerationDispZNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDispZNeg, SXParticlesEditor::CheckBoxAccelerationDispZNeg->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxVelocityDispXNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispXNeg, SXParticlesEditor::CheckBoxVelocityDispXNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxVelocityDispYNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispYNeg, SXParticlesEditor::CheckBoxVelocityDispYNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxVelocityDispZNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDispZNeg, SXParticlesEditor::CheckBoxVelocityDispZNeg->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXNeg, SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispXPos->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateXPos, SXParticlesEditor::CheckBoxSpawnOriginDispXPos->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYNeg, SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispYPos->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateYPos, SXParticlesEditor::CheckBoxSpawnOriginDispYPos->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZNeg, SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxSpawnOriginDispZPos->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnBoundBindCreateZPos, SXParticlesEditor::CheckBoxSpawnOriginDispZPos->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxLighting->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Lighting, SXParticlesEditor::CheckBoxLighting->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxFigureRotRand->getHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureRotRand, SXParticlesEditor::CheckBoxFigureRotRand->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxFigureTapX->getHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapX, SXParticlesEditor::CheckBoxFigureTapX->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxFigureTapY->getHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapY, SXParticlesEditor::CheckBoxFigureTapY->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxFigureTapZ->getHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureTapZ, SXParticlesEditor::CheckBoxFigureTapZ->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxCircle->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircle, SXParticlesEditor::CheckBoxCircle->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxCircleAngleDispNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDispNeg, SXParticlesEditor::CheckBoxCircleAngleDispNeg->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxRotate->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotate, SXParticlesEditor::CheckBoxRotate->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxRotateAngleDispNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDispNeg, SXParticlesEditor::CheckBoxRotateAngleDispNeg->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxDeviation->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviation, SXParticlesEditor::CheckBoxDeviation->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDispNeg, SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationTapX->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapX, SXParticlesEditor::CheckBoxDeviationTapX->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationTapY->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapY, SXParticlesEditor::CheckBoxDeviationTapY->getCheck());
		}
		else if (SXParticlesEditor::CheckBoxDeviationTapZ->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationTapZ, SXParticlesEditor::CheckBoxDeviationTapZ->getCheck());
		}

		else if (SXParticlesEditor::CheckBoxCollisionDelete->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CollisionDelete, SXParticlesEditor::CheckBoxCollisionDelete->getCheck());
		}
	}
	else if (Notification == CBN_SELCHANGE)
	{
		HWND handle_elem = (HWND)(lParam);
		
		if (SXParticlesEditor::ComboBoxBoundType->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundType, (PARTICLESTYPE_BOUND)SXParticlesEditor::ComboBoxBoundType->getSel());
		}
		else if (SXParticlesEditor::ComboBoxSpawnPosType->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnPosType, (PARTICLESTYPE_SPAWNPOS)SXParticlesEditor::ComboBoxSpawnPosType->getSel());
		}
		else if (SXParticlesEditor::ComboBoxFigureType->getHWND() == handle_elem)
		{
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureType, (PARTICLESTYPE_FIGURE)SXParticlesEditor::ComboBoxFigureType->getSel());
		}
		else if (SXParticlesEditor::ComboBoxAlphaBlendType->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaBlendType, (PARTICLESTYPE_ALPHABLEND)SXParticlesEditor::ComboBoxAlphaBlendType->getSel());
		}

		else if (SXParticlesEditor::ComboBoxAlphaDependAge->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AlphaDependAge, (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxAlphaDependAge->getSel());
		}
		else if (SXParticlesEditor::ComboBoxSizeDependAge->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDependAge, (PARTICLESTYPE_DEPEND)SXParticlesEditor::ComboBoxSizeDependAge->getSel());
		}

		else if (SXParticlesEditor::ComboBoxCircleAxis->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAxis, (PARTICLES_AXIS)SXParticlesEditor::ComboBoxCircleAxis->getSel());
		}

		else if (SXParticlesEditor::ComboBoxDeviationType->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationType, (PARTICLESTYPE_DEVIATION)SXParticlesEditor::ComboBoxDeviationType->getSel());
		}
		else if (SXParticlesEditor::ComboBoxDeviationAxis->getHWND() == handle_elem)
		{
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAxis, (PARTICLES_AXIS)SXParticlesEditor::ComboBoxDeviationAxis->getSel());
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
		SXParticlesEditor::ListBoxEffects->setItemText(SXParticlesEditor::ListBoxEffects->getSel(), ttext);
	}
	else
	{
		SPE_EffectNameGet(SXParticlesEditor::SelEffID, ttext);
		SXParticlesEditor::EditEffName->setText(ttext);
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

	if (hwnd == SXParticlesEditor::EditName->getHWND())
	{
		if (def_str_validate(ttext))
		{
			SPE_EmitterNameSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ttext);
			SXParticlesEditor::ListBoxEmitters->setItemText(SXParticlesEditor::ListBoxEmitters->getSel(), ttext);
		}
		else
		{
			SPE_EmitterNameGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ttext);
			SXParticlesEditor::EditName->setText(ttext);
			MessageBox(0,"unresolved name",0,0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditCount->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint > 0)
			SPE_EmitterCountSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, tint);
		else
		{
			MessageBox(0, "unresolved count particles", 0, 0);
			SXParticlesEditor::EditCount->setText(String(SPE_EmitterCountGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID)).c_str());
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditColorCoef->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ColorCoef, String(ttext).toDouble());
	}

	else if (hwnd == SXParticlesEditor::EditColorR->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.x, String(ttext).toDouble());
	}
	else if (hwnd == SXParticlesEditor::EditColorG->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.y, String(ttext).toDouble());
	}
	else if (hwnd == SXParticlesEditor::EditColorB->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.z, String(ttext).toDouble());
	}
	else if (hwnd == SXParticlesEditor::EditColorA->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Color.w, String(ttext).toDouble());
	}

	else if (hwnd == SXParticlesEditor::EditReCreateCount->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, ReCreateCount, String(ttext).toInt());
	}

	else if (hwnd == SXParticlesEditor::EditSoftCoef->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SoftCoef, String(ttext).toDouble());
	}
	else if (hwnd == SXParticlesEditor::EditRefractionCoef->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, RefractionCoef, String(ttext).toDouble());
	}
	else if (hwnd == SXParticlesEditor::EditTransparencyCoef->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TransparencyCoef, String(ttext).toDouble());
	}

	else if (hwnd == SXParticlesEditor::EditFigureCountQuads->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint > 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureCountQuads, tint)
		else
		{
			SXParticlesEditor::EditFigureCountQuads->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, FigureCountQuads)).c_str());
			MessageBox(0, "unresolved count quads", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditTimeLife->getHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLife, tlong)
		else
		{
			SXParticlesEditor::EditTimeLife->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLife)).c_str());
			MessageBox(0, "unresolved time", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditTimeLifeDisp->getHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLifeDisp, tlong)
		else
		{
			SXParticlesEditor::EditTimeLifeDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TimeLifeDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditSizeX->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat > 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.x, tfloat)
		else
		{
			SXParticlesEditor::EditSizeX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.x)).c_str());
			MessageBox(0, "unresolved size", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditSizeY->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat > 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.y, tfloat)
		else
		{
			SXParticlesEditor::EditSizeY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Size.y)).c_str());
			MessageBox(0, "unresolved size", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditSizeDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDisp, tfloat)
		else
		{
			SXParticlesEditor::EditSizeDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SizeDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditTrackSize->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TrackSize, String(ttext).toDouble());
	}
	else if (hwnd == SXParticlesEditor::EditTrackTime->getHWND())
	{
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, TrackTime, String(ttext).toDouble());
	}

	else if (hwnd == SXParticlesEditor::EditAnimTexCountCadrsX->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsX, tint)
		else
		{
			SXParticlesEditor::EditAnimTexCountCadrsX->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsX)).c_str());
			MessageBox(0, "unresolved count cadrs", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexCountCadrsY->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsY, tint)
		else
		{
			SXParticlesEditor::EditAnimTexCountCadrsY->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexCountCadrsY)).c_str());
			MessageBox(0, "unresolved count cadrs", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexRate->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRate, tint)
		else
		{
			SXParticlesEditor::EditAnimTexRate->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRate)).c_str());
			MessageBox(0, "unresolved rate", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexRateDisp->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRateDisp, tint)
		else
		{
			SXParticlesEditor::EditAnimTexRateDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexRateDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexStartCadr->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadr, tint)
		else
		{
			SXParticlesEditor::EditAnimTexStartCadr->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadr)).c_str());
			MessageBox(0, "unresolved start cadr", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditAnimTexStartCadrDisp->getHWND())
	{
		sscanf(ttext, "%d", &tint);
		if (tint >= 0)
			SPE_EmitterSetR(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadrDisp, tint)
		else
		{
			SXParticlesEditor::EditAnimTexStartCadrDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AnimTexStartCadrDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditSpawnOriginX->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditSpawnOriginY->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditSpawnOriginZ->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOrigin.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditSpawnOriginDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		if (tfloat >= 0.f)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp, tfloat)
		else
		{
			SXParticlesEditor::EditSpawnOriginDisp->setText(String(SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}

		tfloat = SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnOriginDisp);
		int qwert = 0;
	}
	else if (hwnd == SXParticlesEditor::EditSpawnNextTime->getHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTime, tlong)
		else
		{
			SXParticlesEditor::EditSpawnNextTime->setText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTime)).c_str());
			MessageBox(0, "unresolved time", 0, 0);
			return 0;
		}
	}
	else if (hwnd == SXParticlesEditor::EditSpawnNextTimeDisp->getHWND())
	{
		sscanf(ttext, "%d", &tlong);
		if (tlong >= 0)
			SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTimeDisp, tlong)
		else
		{
			SXParticlesEditor::EditSpawnNextTimeDisp->setText(String((DWORD)SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, SpawnNextTimeDisp)).c_str());
			MessageBox(0, "unresolved dispersion", 0, 0);
			return 0;
		}
	}

	else if (hwnd == SXParticlesEditor::EditBoundVec1X->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec1Y->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec1Z->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec1W->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec1.w, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2X->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2Y->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2Z->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditBoundVec2W->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundVec2.w, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditVelocityX->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditVelocityY->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditVelocityZ->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Velocity.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditVelocityDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, VelocityDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditAccelerationX->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.x, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditAccelerationY->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.y, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditAccelerationZ->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, Acceleration.z, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditAccelerationDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, AccelerationDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditCircleAngle->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngle, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditCircleAngleDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterCircleAngleDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditRotateAngle->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngle, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditRotateAngleDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterRotateAngleDisp, tfloat);
	}

	else if (hwnd == SXParticlesEditor::EditDeviationAmplitude->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationAmplitude, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditDeviationCoefAngle->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngle, tfloat);
	}
	else if (hwnd == SXParticlesEditor::EditDeviationCoefAngleDisp->getHWND())
	{
		sscanf(ttext, "%f", &tfloat);
		SPE_EmitterSet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, CharacterDeviationCoefAngleDisp, tfloat);
	}
	return 0;
}