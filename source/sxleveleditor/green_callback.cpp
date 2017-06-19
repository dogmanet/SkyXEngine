
void SXLevelEditor::GreenActivateAll(bool bf)
{
	SXLevelEditor::GreenActivateMain(bf);
	SXLevelEditor::GreenActivateCreate(bf);
	SXLevelEditor::GreenActivateEdit(bf);
}

void SXLevelEditor::GreenActivateMain(bool bf)
{
	SXLevelEditor::StaticGreenName->Visible(bf);
	SXLevelEditor::EditGreenName->Visible(bf);
	SXLevelEditor::EditGreenName->SetText("");

	SXLevelEditor::StaticGreenModel->Visible(bf);
	SXLevelEditor::EditGreenModel->Visible(bf);
	SXLevelEditor::EditGreenModel->SetText("");
	SXLevelEditor::ButtonGreenModel->Visible(bf);

	SXLevelEditor::StaticGreenLod1->Visible(bf);
	SXLevelEditor::EditGreenLod1->Visible(bf);
	SXLevelEditor::EditGreenLod1->SetText("");
	SXLevelEditor::ButtonGreenLod1->Visible(bf);
	
	SXLevelEditor::StaticGreenLod2->Visible(bf);
	SXLevelEditor::EditGreenLod2->Visible(bf);
	SXLevelEditor::EditGreenLod2->SetText("");
	SXLevelEditor::ButtonGreenLod2->Visible(bf);

	SXLevelEditor::StaticGreenMask->Visible(bf);
	SXLevelEditor::EditGreenMask->Visible(bf);
	SXLevelEditor::EditGreenMask->SetText("");
	SXLevelEditor::ButtonGreenMask->Visible(bf);

	SXLevelEditor::StaticGreenNav->Visible(bf);
	SXLevelEditor::EditGreenNav->Visible(bf);
	SXLevelEditor::EditGreenNav->SetText("");
	SXLevelEditor::ButtonGreenNav->Visible(bf);
}

void SXLevelEditor::GreenActivateCreate(bool bf)
{
	SXLevelEditor::StaticGreenMask->Enable(bf);
	SXLevelEditor::EditGreenMask->Enable(bf);
	SXLevelEditor::ButtonGreenMask->Enable(bf);

	SXLevelEditor::ButtonGreenGenerate->Visible(bf);

	SXLevelEditor::TrackBarGreenDensity->Visible(bf);
	SXLevelEditor::TrackBarGreenDensity->SetPos(0);

	SXLevelEditor::StaticGreenDensityVal->Visible(bf);
	SXLevelEditor::StaticGreenDensityText->Visible(bf);
}

void SXLevelEditor::GreenActivateEdit(bool bf)
{
	SXLevelEditor::ButtonGreenGenerate->Visible(bf);

	SXLevelEditor::TrackBarGreenDensity->Visible(bf);
	SXLevelEditor::TrackBarGreenDensity->SetPos(0);

	SXLevelEditor::StaticGreenDensityVal->Visible(bf);
	SXLevelEditor::StaticGreenDensityText->Visible(bf);

	//SXLevelEditor::CheckBoxSingleSel->Visible(bf);
	//SXLevelEditor::StaticSingleSelPos->Visible(bf);
	SXLevelEditor::EditGreenSelX->Visible(bf);
	SXLevelEditor::EditGreenSelX->SetText("");
	SXLevelEditor::EditGreenSelY->Visible(bf);
	SXLevelEditor::EditGreenSelY->SetText("");
	SXLevelEditor::EditGreenSelZ->Visible(bf);
	SXLevelEditor::EditGreenSelZ->SetText("");
	SXLevelEditor::RadioButtonGreenSelX->Visible(bf);
	SXLevelEditor::RadioButtonGreenSelY->Visible(bf);
	SXLevelEditor::RadioButtonGreenSelZ->Visible(bf);
	SXLevelEditor::ComboBoxGreenSel->Visible(bf);
	SXLevelEditor::StaticGreenSelX->Visible(bf);
	SXLevelEditor::StaticGreenSelY->Visible(bf);
	SXLevelEditor::StaticGreenSelZ->Visible(bf);
}

void SXLevelEditor::GreenSel(int sel)
{
	if (sel >= 0 && sel < SGeom_GreenGetCount())
	{
		GData::Editors::ActiveElement = sel;
		GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;

		SXLevelEditor::GreenActivateCreate(false);
		SXLevelEditor::GreenActivateEdit(true);
		SXLevelEditor::ButtonGreenGenerate->Visible(false);

		SXLevelEditor::EditGreenModel->SetText(SGeom_GreenMGetModel(sel));
		SXLevelEditor::EditGreenLod1->SetText((SGeom_GreenMGetLod1(sel) ? SGeom_GreenMGetLod1(sel) : ""));
		SXLevelEditor::EditGreenLod2->SetText((SGeom_GreenMGetLod2(sel) ? SGeom_GreenMGetLod2(sel) : ""));
		SXLevelEditor::EditGreenMask->SetText(SGeom_GreenMGetMask(sel));
		SXLevelEditor::EditGreenName->SetText(SGeom_GreenMGetName(sel));
		SXLevelEditor::EditGreenNav->SetText(SGeom_GreenMGetNav(sel));
	}
}

LRESULT SXLevelEditor_EditGreenName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();
	if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		SXLevelEditor::EditGreenName->GetText(SGeom_GreenMGetName(sel), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGreenModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenModel->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetLod(sel, 0, tmpname);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenLod1->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetLod(sel, 1, tmpname);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenLod2_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenLod2->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetLod(sel, 2, tmpname);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_TrackBarGreenDensity_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char freqstr[64];
	int pos = SXLevelEditor::TrackBarGreenDensity->GetPos();
	sprintf(freqstr, "%d%", pos);
	SXLevelEditor::StaticGreenDensityVal->SetText(freqstr);
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXLevelEditor::EditGreenMask->SetText(tmpname);
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenNav_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Meshes, FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenNav->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGeom_GreenGetCount())
				SGeom_GreenMSetNav(sel, tmpname);
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenGenerate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SGeom_ModelsGetCount() == 0)
	{
		MessageBox(0, "Необходимо сначала загрузить модели, и только после сего добавлять растительность!", "Нет моделей", 0);
		return 0;
	}

	char tmp_name[1024];
	tmp_name[0] = 0;
	char path_tex[1024];
	path_tex[0] = 0;
	char path_model[1024];
	path_model[0] = 0;
	char path_lod1[1024];
	path_lod1[0] = 0;
	char path_lod2[1024];
	path_lod2[0] = 0;
	char path_navmesh[1024];
	path_navmesh[0] = 0;

	char tmp_tex[1024];
	tmp_tex[0] = 0;
	char tmp_model[1024];
	tmp_model[0] = 0;
	char tmp_lod1[1024];
	tmp_lod1[0] = 0;
	char tmp_lod2[1024];
	tmp_lod2[0] = 0;

	char tmp_navmesh[1024];
	tmp_navmesh[0] = 0;

	int greentype = GeomGreenType::ggt_tree;

	SXLevelEditor::EditGreenName->GetText(tmp_name, 1024);
	SXLevelEditor::EditGreenMask->GetText(tmp_tex, 1024);
	SXLevelEditor::EditGreenModel->GetText(tmp_model, 1024);
	SXLevelEditor::EditGreenLod1->GetText(tmp_lod1, 1024);
	SXLevelEditor::EditGreenLod2->GetText(tmp_lod2, 1024);
	SXLevelEditor::EditGreenNav->GetText(tmp_navmesh, 1024);

	if (tmp_lod1[0] == 0 && tmp_lod2[0] == 0)
		greentype = GeomGreenType::ggt_grass;

	if (tmp_tex[0] != 0)
		sprintf(path_tex,"%s%s",GData::Pathes::Textures,tmp_tex);

	if (tmp_model[0] != 0)
		sprintf(path_model, "%s%s", GData::Pathes::Meshes, tmp_model);

	if (tmp_lod1[0] != 0)
		sprintf(path_lod1, "%s%s", GData::Pathes::Meshes, tmp_lod1);

	if (tmp_lod2[0] != 0)
		sprintf(path_lod2, "%s%s", GData::Pathes::Meshes, tmp_lod2);

	if (tmp_navmesh[0] != 0)
		sprintf(path_navmesh, "%s%s", GData::Pathes::Meshes, tmp_navmesh);
	
	/*if (!Core_0FileExists(path_tex))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Текстура [", path_tex, "] не существует");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}*/

	if (!Core_0FileExists(path_model))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Модель [", path_model, "] не существует");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (def_str_validate(tmp_navmesh) && !Core_0FileExists(path_navmesh))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Модель [", path_navmesh, "] не существует");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (greentype == GeomGreenType::ggt_tree)
	{
		if (!Core_0FileExists(path_lod1))
		{
			char tmpstr[2048];
			sprintf(tmpstr, "%s%s%s", "Модель [", path_lod1, "] не существует");
			MessageBox(0, tmpstr, 0, 0);
			return 0;
		}

		if (!Core_0FileExists(path_lod2))
		{
			char tmpstr[2048];
			sprintf(tmpstr, "%s%s%s", "Модель [", path_lod2, "] не существует");
			MessageBox(0, tmpstr, 0, 0);
			return 0;
		}
	}

	if (!def_str_validate(tmp_name))
	{
		if (MessageBox(0, "Нет имени у модели, ввести имя файла автоматически?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
		{
			for (int i = 0; i<strlen(path_model); i++)
			{
				if (path_model[strlen(path_model) - i] == '\\')
				{
					sprintf(tmp_name, "%s", path_model + strlen(path_model) - (i - 1));
					break;
				}
			}
		}
		else
			return 0;
	}

	float pos = SXLevelEditor::TrackBarGreenDensity->GetPos();

	SGeom_GreenAddGreen(tmp_name,
		tmp_tex,
		pos,
		tmp_model,
		(tmp_lod1[0] ? tmp_lod1 : 0),
		(tmp_lod2[0] ? tmp_lod2 : 0),
		(tmp_navmesh[0] ? tmp_navmesh : 0));

	char tmpnamecountpoly[1024];
		sprintf(tmpnamecountpoly, "%s | %s | %d",
			SGeom_GreenMGetName(SGeom_GreenGetCount() - 1),
			(SGeom_GreenMGetTypeCountGen(SGeom_GreenGetCount() - 1) == GeomGreenType::ggt_grass ? "grass" : "tree/shrub"),
			SGeom_GreenMGetCountGen(SGeom_GreenGetCount() - 1));
		SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);

	SXLevelEditor::GreenActivateCreate(false);
	SXLevelEditor::ListBoxList->SetSel(SXLevelEditor::ListBoxList->GetCountItem() - 1);

	SXLevelEditor::GreenSel(SXLevelEditor::ListBoxList->GetCountItem() - 1);

	GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
	GData::Editors::ActiveElement = SXLevelEditor::ListBoxList->GetSel();

	return 0;
}