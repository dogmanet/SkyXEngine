
LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (MessageBox(0, "Вы действительно хотите выйти?", "Выход?!", MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		DefWindowProc(hwnd, msg, wParam, lParam);

	return 0;
}

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);

	//файл
	//новый
	if (id == ID_FILE_NEW)
	{
		SGeom_ModelsClear();
		SGeom_GreenClear();
		SML_LigthsClear();
	}
	//открыть
	else if (id == ID_FILE_OPEN)
	{
		char tmppath[1024];
		tmppath[0] = 0;
		char tmpname[1024];
		SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Levels, FILE_FILTER_LEVEL);
		if (def_str_validate(tmppath))
		{
			StrCutNameNEx(tmppath, tmpname);
			Level::Load(tmpname);
		}

	}
	//сохранить
	else if (id == ID_FILE_SAVE)
	{
		char tmppath[1024];
		tmppath[0] = 0;
		char tmpname[1024];
		SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_SAVE, tmppath, 0, GData::Pathes::Levels, FILE_FILTER_LEVEL);
		if (StrValidate(tmppath))
		{
			StrCutNameNEx(tmppath, tmpname);
			Level::Save(tmpname);
		}
	}
	//выход
	else if (id == ID_FILE_EXIT)
	{
		return TrueExit(hwnd, msg, wParam, lParam);
	}


	return 0;
}

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GData::ReSize = true;
	return 0;
}

///////

LRESULT SXLevelEditor_ButtonGeometryOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();
	long tmpcountmodel = SGeom_ModelsGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->SetText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel;++i)
	{
		sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelsMGetName(i), SGeom_ModelsMGetCountPoly(i));
		SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);
	}

	GCActivateAllElems(false);

	LCActivateAllElems(false);
	LCActivateCreatingElems(false);

	MCActivateAllElems(true);
	MCActivateTrans(false);

	SXLevelEditor::HowActivateType = -1;
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();
	long tmpcountmodel = SGeom_GreenGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->SetText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	for (int i = 0; i < tmpcountmodel; ++i)
	{
		sprintf(tmpnamecountpoly, "%s | %s | %d", 
			SGeom_GreenMGetName(i),
			(SGeom_GreenMGetTypeCountGen(i) == GeomGreenType::ggt_grass ? "grass" : "tree/shrub"),
			SGeom_GreenMGetCountGen(i));
		SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);
	}

	MCActivateAllElems(false);
	MCActivateTrans(false);

	LCActivateAllElems(false);
	LCActivateCreatingElems(false);

	GCActivateCreatingElems(true);
	GCActivateAllElems(true);
	
	//MCActivateTrans(false);

	SXLevelEditor::HowActivateType = -2;
	return 0;
}

LRESULT SXLevelEditor_ButtonLightOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXLevelEditor::ListBoxList->Clear();
	long tmpcountmodel = SML_LigthsGetCount();
	char tmptextvalcountmodel[64];
	sprintf(tmptextvalcountmodel, "%d", tmpcountmodel);
	SXLevelEditor::StaticListValCount->SetText(tmptextvalcountmodel);

	char tmpnamecountpoly[1024];
	char texttypel[64];
	for (int i = 0; i < tmpcountmodel; ++i)
	{
		if (SML_LigthsGetType(i) == LightsTypeLight::ltl_global)
			sprintf(texttypel, "%s", "global");
		else if (SML_LigthsGetType(i) == LightsTypeLight::ltl_point)
			sprintf(texttypel, "%s", "point");
		else if (SML_LigthsGetType(i) == LightsTypeLight::ltl_direction)
			sprintf(texttypel, "%s", "direction");

		sprintf(tmpnamecountpoly, "%s | %s",
			SML_LigthsGetName(i),
			texttypel);
		SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);
	}

	MCActivateAllElems(false);
	MCActivateTrans(false);
	GCActivateCreatingElems(false);
	GCActivateAllElems(false);

	LCActivateAllElems(true);
	LCActivateCreatingElems(true);

	/*LCActivateGlobal(true);
	LCActivateDirect(true);
	LCActivatePoint(true);*/
	LCActivateType(LightsTypeLight::ltl_point);

	SXLevelEditor::HowActivateType = -3;
	return 0;
}

LRESULT SXLevelEditor_ListBoxList_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();

	if (sel < 0)
		return 0;

	if (SXLevelEditor::HowActivateType == -1 || SXLevelEditor::HowActivateType == 1)
	{
		if (SGeom_ModelsGetCount() > 0 && sel < SGeom_ModelsGetCount())
		{
			MCInitElemsSelModel(sel);
			SXLevelEditor::HowActivateType = 1;
		}
	}
	else if (SXLevelEditor::HowActivateType == -2 || SXLevelEditor::HowActivateType == 2)
	{
		if (SGeom_GreenGetCount() > 0 && sel < SGeom_GreenGetCount())
		{
			GCInitElemsSelModel(sel);
			SXLevelEditor::HowActivateType = 2;
		}
	}
	else if (SXLevelEditor::HowActivateType == -3 || SXLevelEditor::HowActivateType == 3)
	{
		if (SML_LigthsGetCount() > 0 && sel < SML_LigthsGetCount())
		{
			LCInitElemsSelModel(sel);
			SXLevelEditor::HowActivateType = 3;
		}
	}

	return 0;
}

LRESULT SXLevelEditor_EditName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();
	if (SXLevelEditor::HowActivateType == 1)
	{
		SXLevelEditor::EditName->GetText(SGeom_ModelsMGetName(sel), 64);
	}
	else if (SXLevelEditor::HowActivateType == 2)
	{
		SXLevelEditor::EditName->GetText(SGeom_GreenMGetName(sel), 64);
	}
	else if (SXLevelEditor::HowActivateType == 3)
	{
		SXLevelEditor::EditName->GetText(SML_LigthsGetName(sel), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();

	if (sel < 0)
		return 0;

	if (SXLevelEditor::HowActivateType == 1)
	{
		if (SGeom_ModelsGetCount() > 0 && sel < SGeom_ModelsGetCount())
		{
			SGeom_ModelsDelModel(sel);
			SXLevelEditor::ListBoxList->DeleteItem(sel);
			if (SGeom_ModelsGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_ModelsGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->SetSel(sel);
				MCInitElemsSelModel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGeometryOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (SXLevelEditor::HowActivateType == 2)
	{
		if (SGeom_GreenGetCount() > 0 && sel < SGeom_GreenGetCount())
		{
			SGeom_GreenDelGreen(sel);
			SXLevelEditor::ListBoxList->DeleteItem(sel);
			if (SGeom_GreenGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SGeom_GreenGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->SetSel(sel);
				GCInitElemsSelModel(sel);
			}
			else
			{
				SXLevelEditor_ButtonGreenOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}
	else if (SXLevelEditor::HowActivateType == 3)
	{
		if (SML_LigthsGetCount() > 0 && sel >= 0 && sel < SML_LigthsGetCount())
		{
			SML_LigthsDeleteLight(SML_LigthsGetIDOfKey(sel));
			SXLevelEditor::ListBoxList->DeleteItem(sel);
			if (SML_LigthsGetCount() > 0)
			{
				if (sel > 0)
				{
					if (SML_LigthsGetCount() <= sel)
						sel -= 1;
				}
				SXLevelEditor::ListBoxList->SetSel(sel);
				LCInitElemsSelModel(sel);
			}
			else
			{
				SXLevelEditor_ButtonLightOpen_Click(hwnd, msg, wParam, lParam);
			}
		}
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditModel->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (SXLevelEditor::HowActivateType == 2)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetLod(sel, 0, tmpname);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditLod1->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (SXLevelEditor::HowActivateType == 1)
		{
			if (sel >= 0 && sel < SGeom_ModelsGetCount())
				SGeom_ModelsMSetLodPath(sel, tmpname);
		}
		else if (SXLevelEditor::HowActivateType == 2)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetLod(sel, 1, tmpname);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonLod2_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditLod2->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (SXLevelEditor::HowActivateType == 2)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetLod(sel, 2, tmpname);
		}
	}
	return 0;
}

void SXLevelEditor_Transform(DWORD timeDelta)
{
	static bool IsSFirstLBMTransform = false;
	static bool IsSFirstRBMTransform = false;
	static int CoordinateTransformation = -1;

	if (SXLevelEditor::RadioButtonPosX->GetCheck() || SXLevelEditor::RadioButtonRotX->GetCheck() || SXLevelEditor::RadioButtonScaleX->GetCheck() || SXLevelEditor::LightRadioButtonPosX->GetCheck() || SXLevelEditor::LightRadioButtonRotX->GetCheck())
		CoordinateTransformation = 0;
	else if (SXLevelEditor::RadioButtonPosY->GetCheck() || SXLevelEditor::RadioButtonRotY->GetCheck() || SXLevelEditor::RadioButtonScaleY->GetCheck() || SXLevelEditor::LightRadioButtonPosY->GetCheck() || SXLevelEditor::LightRadioButtonRotY->GetCheck())
		CoordinateTransformation = 1;
	else if (SXLevelEditor::RadioButtonPosZ->GetCheck() || SXLevelEditor::RadioButtonRotZ->GetCheck() || SXLevelEditor::RadioButtonScaleZ->GetCheck() || SXLevelEditor::LightRadioButtonPosZ->GetCheck() || SXLevelEditor::LightRadioButtonRotZ->GetCheck())
		CoordinateTransformation = 2;

	if (SXLevelEditor::HowActivateType == 1)
	{
		if (::GetAsyncKeyState(VK_LSHIFT) && SGeom_ModelsGetCount() > 0)
		{
			DWORD selmodel = SXLevelEditor::ListBoxList->GetSel();
			if (SXLevelEditor::RadioButtonPosX->GetCheck() || SXLevelEditor::RadioButtonPosY->GetCheck() || SXLevelEditor::RadioButtonPosZ->GetCheck())
			{
				float3& pos = *(SGeom_ModelsMGetPosition(selmodel));

				if (::GetAsyncKeyState(VK_UP) & 0x8000f)
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
					pos[CoordinateTransformation] -= timeDelta * 0.001f;
				if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
				{
					if (IsSFirstRBMTransform)
					{
						UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
						UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditPosX->SetText(tmpPosX);
				SXLevelEditor::EditPosY->SetText(tmpPosY);
				SXLevelEditor::EditPosZ->SetText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}

			else if (SXLevelEditor::RadioButtonRotX->GetCheck() || SXLevelEditor::RadioButtonRotY->GetCheck() || SXLevelEditor::RadioButtonRotZ->GetCheck())
			{
				float3& pos = *(SGeom_ModelsMGetRotation(selmodel));

				if (::GetAsyncKeyState(VK_UP) & 0x8000f)
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
					pos[CoordinateTransformation] -= timeDelta * 0.001f;
				if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
				{
					if (IsSFirstRBMTransform)
					{
						UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
						UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditRotX->SetText(tmpPosX);
				SXLevelEditor::EditRotY->SetText(tmpPosY);
				SXLevelEditor::EditRotZ->SetText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}

			else if (SXLevelEditor::RadioButtonScaleX->GetCheck() || SXLevelEditor::RadioButtonScaleY->GetCheck() || SXLevelEditor::RadioButtonScaleZ->GetCheck())
			{
				float3& pos = *(SGeom_ModelsMGetScale(selmodel));

				if (::GetAsyncKeyState(VK_UP) & 0x8000f)
					pos[CoordinateTransformation] += timeDelta * 0.001f;
				if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
					pos[CoordinateTransformation] -= timeDelta * 0.001f;
				if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
				{
					if (IsSFirstRBMTransform)
					{
						UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
						UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
						POINT p;
						GetCursorPos(&p);
						POINT centr;
						centr.x = cx; centr.y = cy;

						if (cy != UINT(p.y))
							pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
					}
					else
						IsSFirstRBMTransform = true;
					CameraUpdate::CentererCursor();
				}
				else
				{
					IsSFirstLBMTransform = false;
					IsSFirstRBMTransform = false;
				}

				char tmpPosX[32];
				char tmpPosY[32];
				char tmpPosZ[32];

				sprintf(tmpPosX, "%f", pos.x);
				sprintf(tmpPosY, "%f", pos.y);
				sprintf(tmpPosZ, "%f", pos.z);

				SXLevelEditor::EditScaleX->SetText(tmpPosX);
				SXLevelEditor::EditScaleY->SetText(tmpPosY);
				SXLevelEditor::EditScaleZ->SetText(tmpPosZ);

				SGeom_ModelsMApplyTransform(selmodel);
			}
		}
	}

	///////////////////


	else if (SXLevelEditor::HowActivateType == 3)
	{
		if (!(::GetAsyncKeyState(VK_LSHIFT) && SML_LigthsGetCount() > 0))
			return;

		DWORD selmodel = SXLevelEditor::ListBoxList->GetSel();
		if (SXLevelEditor::LightRadioButtonPosX->GetCheck() || SXLevelEditor::LightRadioButtonPosY->GetCheck() || SXLevelEditor::LightRadioButtonPosZ->GetCheck())
		{
			float3 pos;
			(SML_LigthsGetPos(SML_LigthsGetIDOfKey(selmodel), &pos, false/*SXLevelEditor::LightCheckBoxPosLightOrMesh->GetCheck()*/));

			if (::GetAsyncKeyState(VK_UP) & 0x8000f)
				pos[CoordinateTransformation] += timeDelta * 0.001f;
			if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
				pos[CoordinateTransformation] -= timeDelta * 0.001f;
			if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
			{
				if (IsSFirstRBMTransform)
				{
					UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
					UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
					POINT p;
					GetCursorPos(&p);
					POINT centr;
					centr.x = cx; centr.y = cy;

					if (cy != UINT(p.y))
						pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
				}
				else
					IsSFirstRBMTransform = true;
				CameraUpdate::CentererCursor();
			}
			else
			{
				IsSFirstLBMTransform = false;
				IsSFirstRBMTransform = false;
			}

			char tmpPosX[32];
			char tmpPosY[32];
			char tmpPosZ[32];

			sprintf(tmpPosX, "%f", pos.x);
			sprintf(tmpPosY, "%f", pos.y);
			sprintf(tmpPosZ, "%f", pos.z);

			SXLevelEditor::LightEditPosX->SetText(tmpPosX);
			SXLevelEditor::LightEditPosY->SetText(tmpPosY);
			SXLevelEditor::LightEditPosZ->SetText(tmpPosZ);

			SML_LigthsSetPos(SML_LigthsGetIDOfKey(selmodel), &pos/*, SXLevelEditor::LightCheckBoxPosLightOrMesh->GetCheck()*/);
		}

		else if (SXLevelEditor::LightRadioButtonRotX->GetCheck() || SXLevelEditor::LightRadioButtonRotY->GetCheck() || SXLevelEditor::LightRadioButtonRotZ->GetCheck())
		{
			float3 pos;
			(SML_LigthsGetRot(SML_LigthsGetIDOfKey(selmodel), &pos, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck()));

			if (::GetAsyncKeyState(VK_UP) & 0x8000f)
				pos[CoordinateTransformation] += timeDelta * 0.001f;
			if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
				pos[CoordinateTransformation] -= timeDelta * 0.001f;
			if (::GetAsyncKeyState(VK_LBUTTON) & 0x8000f)
			{
				if (IsSFirstRBMTransform)
				{
					UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
					UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
					POINT p;
					GetCursorPos(&p);
					POINT centr;
					centr.x = cx; centr.y = cy;

					if (cy != UINT(p.y))
						pos[CoordinateTransformation] += timeDelta * 0.001f * float(-int(p.y - cy));
				}
				else
					IsSFirstRBMTransform = true;
				CameraUpdate::CentererCursor();
			}
			else
			{
				IsSFirstLBMTransform = false;
				IsSFirstRBMTransform = false;
			}

			char tmpPosX[32];
			char tmpPosY[32];
			char tmpPosZ[32];

			sprintf(tmpPosX, "%f", pos.x);
			sprintf(tmpPosY, "%f", pos.y);
			sprintf(tmpPosZ, "%f", pos.z);

			SXLevelEditor::LightEditRotX->SetText(tmpPosX);
			SXLevelEditor::LightEditRotY->SetText(tmpPosY);
			SXLevelEditor::LightEditRotZ->SetText(tmpPosZ);

			SML_LigthsSetRot(SML_LigthsGetIDOfKey(selmodel), &pos, SXLevelEditor::LightCheckBoxRotLightOrMesh->GetCheck());
		}
	}
}