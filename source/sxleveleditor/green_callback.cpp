
#include "green_callback.h"

void SXLevelEditor::GreenActivateAll(bool bf)
{
	SXLevelEditor::GreenActivateMain(bf);
	SXLevelEditor::GreenActivateCreate(bf);
	SXLevelEditor::GreenActivateEdit(bf);
}

void SXLevelEditor::GreenActivateMain(bool bf)
{
	SXLevelEditor::StaticGreenName->setVisible(bf);
	SXLevelEditor::EditGreenName->setVisible(bf);
	SXLevelEditor::EditGreenName->setText("");

	SXLevelEditor::StaticGreenModel->setVisible(bf);
	SXLevelEditor::EditGreenModel->setVisible(bf);
	SXLevelEditor::EditGreenModel->setText("");
	SXLevelEditor::ButtonGreenModel->setVisible(bf);

	SXLevelEditor::StaticGreenLod1->setVisible(bf);
	SXLevelEditor::EditGreenLod1->setVisible(bf);
	SXLevelEditor::EditGreenLod1->setText("");
	SXLevelEditor::ButtonGreenLod1->setVisible(bf);
	
	SXLevelEditor::StaticGreenLod2->setVisible(bf);
	SXLevelEditor::EditGreenLod2->setVisible(bf);
	SXLevelEditor::EditGreenLod2->setText("");
	SXLevelEditor::ButtonGreenLod2->setVisible(bf);

	SXLevelEditor::StaticGreenMask->setVisible(bf);
	SXLevelEditor::EditGreenMask->setVisible(bf);
	SXLevelEditor::EditGreenMask->setText("");
	SXLevelEditor::ButtonGreenMask->setVisible(bf);

	SXLevelEditor::StaticGreenNav->setVisible(bf);
	SXLevelEditor::EditGreenNav->setVisible(bf);
	SXLevelEditor::EditGreenNav->setText("");
	SXLevelEditor::ButtonGreenNav->setVisible(bf);
}

void SXLevelEditor::GreenActivateCreate(bool bf)
{
	SXLevelEditor::StaticGreenMask->setEnable(bf);
	SXLevelEditor::EditGreenMask->setEnable(bf);
	SXLevelEditor::ButtonGreenMask->setEnable(bf);

	SXLevelEditor::ButtonGreenGenerate->setVisible(bf);

	SXLevelEditor::TrackBarGreenDensity->setVisible(bf);
	SXLevelEditor::TrackBarGreenDensity->setPos(0);

	SXLevelEditor::StaticGreenDensityVal->setVisible(bf);
	SXLevelEditor::StaticGreenDensityText->setVisible(bf);
}

void SXLevelEditor::GreenActivateEdit(bool bf)
{
	SXLevelEditor::ButtonGreenGenerate->setVisible(bf);

	SXLevelEditor::TrackBarGreenDensity->setVisible(bf);
	SXLevelEditor::TrackBarGreenDensity->setPos(0);

	SXLevelEditor::StaticGreenDensityVal->setVisible(bf);
	SXLevelEditor::StaticGreenDensityText->setVisible(bf);

	//SXLevelEditor::CheckBoxSingleSel->setVisible(bf);
	//SXLevelEditor::StaticSingleSelPos->setVisible(bf);
	SXLevelEditor::EditGreenSelX->setVisible(bf);
	SXLevelEditor::EditGreenSelX->setText("");
	SXLevelEditor::EditGreenSelY->setVisible(bf);
	SXLevelEditor::EditGreenSelY->setText("");
	SXLevelEditor::EditGreenSelZ->setVisible(bf);
	SXLevelEditor::EditGreenSelZ->setText("");
	SXLevelEditor::RadioButtonGreenSelX->setVisible(bf);
	SXLevelEditor::RadioButtonGreenSelY->setVisible(bf);
	SXLevelEditor::RadioButtonGreenSelZ->setVisible(bf);
	SXLevelEditor::ComboBoxGreenSel->setVisible(bf);
	SXLevelEditor::StaticGreenSelX->setVisible(bf);
	SXLevelEditor::StaticGreenSelY->setVisible(bf);
	SXLevelEditor::StaticGreenSelZ->setVisible(bf);
}

void SXLevelEditor::GreenSel(int sel)
{
	if (sel >= 0 && sel < SGeom_GreenGetCount())
	{
		SXLevelEditor::ActiveElement = sel;
		SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;

		SXLevelEditor::GreenActivateCreate(false);
		SXLevelEditor::GreenActivateEdit(true);
		SXLevelEditor::ButtonGreenGenerate->setVisible(false);

		SXLevelEditor::EditGreenModel->setText(SGeom_GreenMGetModel(sel));
		SXLevelEditor::EditGreenLod1->setText((SGeom_GreenMGetLod1(sel) ? SGeom_GreenMGetLod1(sel) : ""));
		SXLevelEditor::EditGreenLod2->setText((SGeom_GreenMGetLod2(sel) ? SGeom_GreenMGetLod2(sel) : ""));
		SXLevelEditor::EditGreenMask->setText(SGeom_GreenMGetMask(sel));
		SXLevelEditor::EditGreenName->setText(SGeom_GreenMGetName(sel));
		SXLevelEditor::EditGreenNav->setText(SGeom_GreenMGetNav(sel));
	}
}

LRESULT SXLevelEditor_EditGreenName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->getSel();
	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		SXLevelEditor::EditGreenName->getText(SGeom_GreenMGetName(sel), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGreenModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenModel->setText(tmpname);
		int sel = SXLevelEditor::ListBoxList->getSel();
		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
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
	gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenLod1->setText(tmpname);
		int sel = SXLevelEditor::ListBoxList->getSel();
		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
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
	gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenLod2->setText(tmpname);
		int sel = SXLevelEditor::ListBoxList->getSel();
		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
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
	int pos = SXLevelEditor::TrackBarGreenDensity->getPos();
	sprintf(freqstr, "%d%", pos);
	SXLevelEditor::StaticGreenDensityVal->setText(freqstr);
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXLevelEditor::EditGreenMask->setText(tmpname);
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenNav_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGreenNav->setText(tmpname);
		int sel = SXLevelEditor::ListBoxList->getSel();
		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
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
		MessageBox(0, "Not found geomety!", "Not found geomety", 0);
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

	int greentype = GREEN_TYPE_TREE;

	SXLevelEditor::EditGreenName->getText(tmp_name, 1024);
	SXLevelEditor::EditGreenMask->getText(tmp_tex, 1024);
	SXLevelEditor::EditGreenModel->getText(tmp_model, 1024);
	SXLevelEditor::EditGreenLod1->getText(tmp_lod1, 1024);
	SXLevelEditor::EditGreenLod2->getText(tmp_lod2, 1024);
	SXLevelEditor::EditGreenNav->getText(tmp_navmesh, 1024);

	if (tmp_lod1[0] == 0 && tmp_lod2[0] == 0)
		greentype = GREEN_TYPE_GRASS;

	if (tmp_tex[0] != 0)
		sprintf(path_tex, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), tmp_tex);

	if (tmp_model[0] != 0)
		sprintf(path_model, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmp_model);

	if (tmp_lod1[0] != 0)
		sprintf(path_lod1, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmp_lod1);

	if (tmp_lod2[0] != 0)
		sprintf(path_lod2, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmp_lod2);

	if (tmp_navmesh[0] != 0)
		sprintf(path_navmesh, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmp_navmesh);
	
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
		sprintf(tmpstr, "%s%s%s", "Model [", path_model, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (def_str_validate(tmp_navmesh) && !Core_0FileExists(path_navmesh))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model [", path_navmesh, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (greentype == GREEN_TYPE_TREE)
	{
		if (!Core_0FileExists(path_lod1))
		{
			char tmpstr[2048];
			sprintf(tmpstr, "%s%s%s", "Model [", path_lod1, "] not found");
			MessageBox(0, tmpstr, 0, 0);
			return 0;
		}

		if (!Core_0FileExists(path_lod2))
		{
			char tmpstr[2048];
			sprintf(tmpstr, "%s%s%s", "Model [", path_lod2, "] not found");
			MessageBox(0, tmpstr, 0, 0);
			return 0;
		}
	}

	if (!def_str_validate(tmp_name))
	{
		if (MessageBox(0, "No name for the model, enter the file name automatically?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
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

	float pos = SXLevelEditor::TrackBarGreenDensity->getPos();

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
			(SGeom_GreenMGetTypeCountGen(SGeom_GreenGetCount() - 1) == GREEN_TYPE_GRASS ? "grass" : "tree/shrub"),
			SGeom_GreenMGetCountGen(SGeom_GreenGetCount() - 1));
		SXLevelEditor::ListBoxList->addItem(tmpnamecountpoly);

	SXLevelEditor::GreenActivateCreate(false);
	SXLevelEditor::ListBoxList->setSel(SXLevelEditor::ListBoxList->getItemCount() - 1);

	SXLevelEditor::GreenSel(SXLevelEditor::ListBoxList->getItemCount() - 1);

	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
	SXLevelEditor::ActiveElement = SXLevelEditor::ListBoxList->getSel();

	return 0;
}