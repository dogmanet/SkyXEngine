
void GCActivateAllElems(bool bf)
{
	SXLevelEditor::StaticName->Visible(bf);
	SXLevelEditor::EditName->Visible(bf);
	SXLevelEditor::EditName->SetText("");

	SXLevelEditor::StaticModel->Visible(bf);
	SXLevelEditor::EditModel->Visible(bf);
	SXLevelEditor::EditModel->SetText("");
	SXLevelEditor::ButtonModel->Visible(bf);

	SXLevelEditor::StaticLod1->Visible(bf);
	SXLevelEditor::EditLod1->Visible(bf);
	SXLevelEditor::EditLod1->SetText("");
	SXLevelEditor::ButtonLod1->Visible(bf);
	
	SXLevelEditor::StaticLod2->Visible(bf);
	SXLevelEditor::EditLod2->Visible(bf);
	SXLevelEditor::EditLod2->SetText("");
	SXLevelEditor::ButtonLod2->Visible(bf);

	SXLevelEditor::StaticMask->Visible(bf);
	SXLevelEditor::EditMask->Visible(bf);
	SXLevelEditor::EditMask->SetText("");
	SXLevelEditor::ButtonMask->Visible(bf);

	SXLevelEditor::ButtonGenerate->Visible(bf);

	SXLevelEditor::TrackBarDensity->Visible(bf);
	SXLevelEditor::TrackBarDensity->SetPos(0);

	SXLevelEditor::StaticDensityVal->Visible(bf);
	SXLevelEditor::StaticDensityText->Visible(bf);

	SXLevelEditor::StaticGreenNav->Visible(bf);
	SXLevelEditor::EditGreenNav->Visible(bf);
	SXLevelEditor::ButtonGreenNav->Visible(bf);
}

void GCActivateCreatingElems(bool bf)
{
	SXLevelEditor::StaticMask->Enable(bf);
	SXLevelEditor::EditMask->Enable(bf);
	SXLevelEditor::ButtonMask->Enable(bf);

	SXLevelEditor::ButtonGenerate->Visible(bf);

	SXLevelEditor::TrackBarDensity->Visible(bf);
	SXLevelEditor::TrackBarDensity->SetPos(0);

	SXLevelEditor::StaticDensityVal->Visible(bf);
	SXLevelEditor::StaticDensityText->Visible(bf);
}

void GCInitElemsSelModel(int sel)
{
	if (sel >= 0 && sel < SGeom_GreenGetCount())
	{
		GCActivateCreatingElems(false);

		SXLevelEditor::EditModel->SetText(SGeom_GreenMGetModel(sel));
		SXLevelEditor::EditLod1->SetText((SGeom_GreenMGetLod1(sel) ? SGeom_GreenMGetLod1(sel) : ""));
		SXLevelEditor::EditLod2->SetText((SGeom_GreenMGetLod2(sel) ? SGeom_GreenMGetLod2(sel) : ""));
		SXLevelEditor::EditMask->SetText(SGeom_GreenMGetMask(sel));
		SXLevelEditor::EditName->SetText(SGeom_GreenMGetName(sel));
		SXLevelEditor::EditGreenNav->SetText(SGeom_GreenMGetNav(sel));
	}
}



LRESULT SXLevelEditor_TrackBarDensity_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char freqstr[64];
	int pos = SXLevelEditor::TrackBarDensity->GetPos();
	sprintf(freqstr, "%d%", pos);
	SXLevelEditor::StaticDensityVal->SetText(freqstr);
	return 0;
}

LRESULT SXLevelEditor_ButtonMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXLevelEditor::EditMask->SetText(tmpname);
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

LRESULT SXLevelEditor_ButtonGenerate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

	SXLevelEditor::EditName->GetText(tmp_name, 1024);
	SXLevelEditor::EditMask->GetText(tmp_tex, 1024);
	SXLevelEditor::EditModel->GetText(tmp_model, 1024);
	SXLevelEditor::EditLod1->GetText(tmp_lod1, 1024);
	SXLevelEditor::EditLod2->GetText(tmp_lod2, 1024);
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

	float pos = SXLevelEditor::TrackBarDensity->GetPos();

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

	GCActivateCreatingElems(false);
	SXLevelEditor::ListBoxList->SetSel(SXLevelEditor::ListBoxList->GetCountItem() - 1);

	GCInitElemsSelModel(SXLevelEditor::ListBoxList->GetCountItem() - 1);

	GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
	GData::Editors::ActiveElement = SXLevelEditor::ListBoxList->GetSel();

	return 0;
}