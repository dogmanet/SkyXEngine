


void MCActivateAllElems(bool bf)
{
	SXLevelEditor::StaticName->Visible(bf);
	SXLevelEditor::StaticModel->Visible(bf);
	SXLevelEditor::StaticLod1->Visible(bf);
	SXLevelEditor::EditName->Visible(bf);
	SXLevelEditor::EditName->SetText("");
	SXLevelEditor::EditModel->Visible(bf);
	SXLevelEditor::EditModel->SetText("");
	SXLevelEditor::EditLod1->Visible(bf);
	SXLevelEditor::EditLod1->SetText("");
	SXLevelEditor::ButtonLod1->Visible(bf);
	SXLevelEditor::ButtonModel->Visible(bf);
	//SXLevelEditor::CheckBoxCG->Visible(bf);
	//SXLevelEditor::CheckBoxCG->SetCheck(false);
	SXLevelEditor::ButtonFinish->Visible(bf);
}

void MCActivateTrans(bool bf)
{
	SXLevelEditor::StaticPos->Visible(bf);
	SXLevelEditor::EditPosX->Visible(bf);
	SXLevelEditor::EditPosY->Visible(bf);
	SXLevelEditor::EditPosZ->Visible(bf);
	SXLevelEditor::RadioButtonPosX->Visible(bf);
	SXLevelEditor::RadioButtonPosY->Visible(bf);
	SXLevelEditor::RadioButtonPosZ->Visible(bf);
	SXLevelEditor::StaticRot->Visible(bf);
	SXLevelEditor::EditRotX->Visible(bf);
	SXLevelEditor::EditRotY->Visible(bf);
	SXLevelEditor::EditRotZ->Visible(bf);
	SXLevelEditor::RadioButtonRotX->Visible(bf);
	SXLevelEditor::RadioButtonRotY->Visible(bf);
	SXLevelEditor::RadioButtonRotZ->Visible(bf);
	SXLevelEditor::StaticScale->Visible(bf);
	SXLevelEditor::EditScaleX->Visible(bf);
	SXLevelEditor::EditScaleY->Visible(bf);
	SXLevelEditor::EditScaleZ->Visible(bf);
	SXLevelEditor::RadioButtonScaleX->Visible(bf);
	SXLevelEditor::RadioButtonScaleY->Visible(bf);
	SXLevelEditor::RadioButtonScaleZ->Visible(bf);

	SXLevelEditor::EditModel->Enable(true);
	SXLevelEditor::ButtonModel->Enable(true);
}

void MCInitElemsSelModel(int sel)
{
	if (sel >= 0 && sel < SGeom_ModelsGetCount())
	{
		MCActivateTrans(true);

		SXLevelEditor::EditModel->SetText(SGeom_ModelsMGetPathName(sel));
		SXLevelEditor::EditLod1->SetText(SGeom_ModelsMGetLodPath(sel));
		char* tmpname = SGeom_ModelsMGetName(sel);
		float3* pos = SGeom_ModelsMGetPosition(sel);
		float3* rot = SGeom_ModelsMGetRotation(sel);
		float3* scale = SGeom_ModelsMGetScale(sel);

		SXLevelEditor::EditName->SetText(tmpname);

		char tmpval[64];

		sprintf(tmpval, "%f", pos->x);
		SXLevelEditor::EditPosX->SetText(tmpval);

		sprintf(tmpval, "%f", pos->y);
		SXLevelEditor::EditPosY->SetText(tmpval);

		sprintf(tmpval, "%f", pos->z);
		SXLevelEditor::EditPosZ->SetText(tmpval);


		sprintf(tmpval, "%f", rot->x);
		SXLevelEditor::EditRotX->SetText(tmpval);

		sprintf(tmpval, "%f", rot->y);
		SXLevelEditor::EditRotY->SetText(tmpval);

		sprintf(tmpval, "%f", rot->z);
		SXLevelEditor::EditRotZ->SetText(tmpval);


		sprintf(tmpval, "%f", scale->x);
		SXLevelEditor::EditScaleX->SetText(tmpval);

		sprintf(tmpval, "%f", scale->y);
		SXLevelEditor::EditScaleY->SetText(tmpval);

		sprintf(tmpval, "%f", scale->z);
		SXLevelEditor::EditScaleZ->SetText(tmpval);

		SXLevelEditor::EditModel->Enable(false);
		SXLevelEditor::ButtonModel->Enable(false);
		SXLevelEditor::ButtonFinish->Visible(false);
	}
}



LRESULT SXLevelEditor_ButtonFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char path_model[1024];
	char path_model_lod[1024];

	//sprintf(path_model, "%s", GData::Pathes::Meshes);
	//sprintf(path_model_lod, "%s", GData::Pathes::Meshes);

	char model_name[256];
	model_name[0] = 0;
	SXLevelEditor::EditName->GetText(model_name, 256);
	SXLevelEditor::EditModel->GetText(path_model, 1024);
	SXLevelEditor::EditLod1->GetText(path_model_lod, 1024);

	if (!def_str_validate(path_model))
	{
		return 0;
	}

	char tmppath[1024];
	sprintf(tmppath, "%s%s", GData::Pathes::Meshes, path_model);
	if (!Core_0FileExists(tmppath))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Модель [", tmppath, "] не существует");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	sprintf(tmppath, "%s%s", GData::Pathes::Meshes, path_model_lod);
	if (def_str_validate(path_model_lod) && !Core_0FileExists(tmppath))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Модель/лод [", path_model_lod, "] не существует");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (!def_str_validate(model_name))
	{
		if (MessageBox(0, "Нет имени у модели, ввести имя файла автоматически?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		{
			for (int i = 0; i<strlen(path_model); i++)
			{
				if (path_model[strlen(path_model) - i] == '\\')
				{
					sprintf(model_name, "%s", path_model + strlen(path_model) - (i - 1));
					break;
				}
			}
		}
		else
			return 0;
	}


	SGeom_ModelsAddModel(path_model, (def_str_validate(path_model_lod) ? path_model_lod : 0), model_name);

	char tmpnamecountpoly[1024];
	sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelsMGetName(SGeom_ModelsGetCount() - 1), SGeom_ModelsMGetCountPoly(SGeom_ModelsGetCount() - 1));
	SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);

	MCActivateTrans(true);
	SXLevelEditor::ListBoxList->SetSel(SXLevelEditor::ListBoxList->GetCountItem() - 1);
	MCInitElemsSelModel(SXLevelEditor::ListBoxList->GetSel());

	SXLevelEditor::HowActivateType = 1;

	return 0;
}

LRESULT SXLevelEditor_EditTransformPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::HowActivateType != 1)
		return 0;
	int sel = SXLevelEditor::ListBoxList->GetSel();
	float3* pos = SGeom_ModelsMGetPosition(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditPosX->GetHWND())
	{
		SXLevelEditor::EditPosX->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->x));
	}
	else if (hwnd == SXLevelEditor::EditPosY->GetHWND())
	{
		SXLevelEditor::EditPosY->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->y));
	}
	else if (hwnd == SXLevelEditor::EditPosZ->GetHWND())
	{
		SXLevelEditor::EditPosZ->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}

LRESULT SXLevelEditor_EditTransformRot_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::HowActivateType != 1)
		return 0;
	int sel = SXLevelEditor::ListBoxList->GetSel();
	float3* rot = SGeom_ModelsMGetRotation(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditRotX->GetHWND())
	{
		SXLevelEditor::EditRotX->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->x));
	}
	else if (hwnd == SXLevelEditor::EditRotY->GetHWND())
	{
		SXLevelEditor::EditRotY->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->y));
	}
	else if (hwnd == SXLevelEditor::EditRotZ->GetHWND())
	{
		SXLevelEditor::EditRotZ->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}

LRESULT SXLevelEditor_EditTransformScale_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::HowActivateType != 1)
		return 0;
	int sel = SXLevelEditor::ListBoxList->GetSel();
	float3* scale = SGeom_ModelsMGetScale(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditScaleX->GetHWND())
	{
		SXLevelEditor::EditScaleX->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->x));
	}
	else if (hwnd == SXLevelEditor::EditScaleY->GetHWND())
	{
		SXLevelEditor::EditScaleY->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->y));
	}
	else if (hwnd == SXLevelEditor::EditScaleZ->GetHWND())
	{
		SXLevelEditor::EditScaleZ->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}