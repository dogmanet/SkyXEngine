
#include "green_callback.h"

void level_editor::GreenActivateAll(bool bf)
{
	level_editor::GreenActivateMain(bf);
	level_editor::GreenActivateCreate(bf);
	level_editor::GreenActivateEdit(bf);
}

void level_editor::GreenActivateMain(bool bf)
{
	level_editor::pStaticGreenName->setVisible(bf);
	level_editor::pEditGreenName->setVisible(bf);
	level_editor::pEditGreenName->setText("");

	level_editor::pStaticGreenModel->setVisible(bf);
	level_editor::pEditGreenModel->setVisible(bf);
	level_editor::pEditGreenModel->setText("");
	level_editor::pButtonGreenModel->setVisible(bf);

	level_editor::pStaticGreenLod1->setVisible(bf);
	level_editor::pEditGreenLod1->setVisible(bf);
	level_editor::pEditGreenLod1->setText("");
	level_editor::pButtonGreenLod1->setVisible(bf);
	
	level_editor::pStaticGreenLod2->setVisible(bf);
	level_editor::pEditGreenLod2->setVisible(bf);
	level_editor::pEditGreenLod2->setText("");
	level_editor::pButtonGreenLod2->setVisible(bf);

	level_editor::pStaticGreenMask->setVisible(bf);
	level_editor::pEditGreenMask->setVisible(bf);
	level_editor::pEditGreenMask->setText("");
	level_editor::pButtonGreenMask->setVisible(bf);

	level_editor::pStaticGreenNav->setVisible(bf);
	level_editor::pEditGreenNav->setVisible(bf);
	level_editor::pEditGreenNav->setText("");
	level_editor::pButtonGreenNav->setVisible(bf);
}

void level_editor::GreenActivateCreate(bool bf)
{
	level_editor::pStaticGreenMask->setEnable(bf);
	level_editor::pEditGreenMask->setEnable(bf);
	level_editor::pButtonGreenMask->setEnable(bf);

	level_editor::pButtonGreenGenerate->setVisible(bf);

	level_editor::pTrackBarGreenDensity->setVisible(bf);
	level_editor::pTrackBarGreenDensity->setPos(0);

	level_editor::pStaticGreenDensityVal->setVisible(bf);
	level_editor::pStaticGreenDensityText->setVisible(bf);
}

void level_editor::GreenActivateEdit(bool bf)
{
	level_editor::pButtonGreenGenerate->setVisible(bf);

	level_editor::pTrackBarGreenDensity->setVisible(bf);
	level_editor::pTrackBarGreenDensity->setPos(0);

	level_editor::pStaticGreenDensityVal->setVisible(bf);
	level_editor::pStaticGreenDensityText->setVisible(bf);

	//level_editor::CheckBoxSingleSel->setVisible(bf);
	//level_editor::pStaticSingleSelPos->setVisible(bf);
	level_editor::pEditGreenSelX->setVisible(bf);
	level_editor::pEditGreenSelX->setText("");
	level_editor::pEditGreenSelY->setVisible(bf);
	level_editor::pEditGreenSelY->setText("");
	level_editor::pEditGreenSelZ->setVisible(bf);
	level_editor::pEditGreenSelZ->setText("");
	level_editor::pRadioButtonGreenSelX->setVisible(bf);
	level_editor::pRadioButtonGreenSelY->setVisible(bf);
	level_editor::pRadioButtonGreenSelZ->setVisible(bf);
	level_editor::pComboBoxGreenSel->setVisible(bf);
	level_editor::pStaticGreenSelX->setVisible(bf);
	level_editor::pStaticGreenSelY->setVisible(bf);
	level_editor::pStaticGreenSelZ->setVisible(bf);
}

void level_editor::GreenSel(int sel)
{
	if (sel >= 0 && sel < SGreen_GetCount())
	{
		level_editor::idActiveElement = sel;
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;

		level_editor::GreenActivateCreate(false);
		level_editor::GreenActivateEdit(true);
		level_editor::pButtonGreenGenerate->setVisible(false);

		level_editor::pEditGreenModel->setText(SGreen_MGetModel(sel));
		level_editor::pEditGreenLod1->setText((SGreen_MGetLod1(sel) ? SGreen_MGetLod1(sel) : ""));
		level_editor::pEditGreenLod2->setText((SGreen_MGetLod2(sel) ? SGreen_MGetLod2(sel) : ""));
		level_editor::pEditGreenMask->setText(SGreen_MGetMask(sel));
		level_editor::pEditGreenName->setText(SGreen_MGetName(sel));
		level_editor::pEditGreenNav->setText(SGreen_MGetNav(sel));
	}
}

LRESULT SXLevelEditor_EditGreenName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = level_editor::pListBoxList->getSel();
	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
	{
		level_editor::pEditGreenName->getText(SGreen_MGetName(sel), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGreenModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenModel->setText(sRpath.c_str());
		int sel = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGreen_GetCount())
				SGreen_MSetLod(sel, 0, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenLod1->setText(sRpath.c_str());
		int sel = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGreen_GetCount())
				SGreen_MSetLod(sel, 1, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenLod2_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenLod2->setText(sRpath.c_str());
		int sel = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGreen_GetCount())
				SGreen_MSetLod(sel, 2, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_TrackBarGreenDensity_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char freqstr[64];
	int pos = level_editor::pTrackBarGreenDensity->getPos();
	sprintf(freqstr, "%d%", pos);
	level_editor::pStaticGreenDensityVal->setText(freqstr);
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_TEXTURE);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture mask", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		level_editor::pEditGreenMask->setText(tmpname);
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenNav_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGreenNav->setText(sRpath.c_str());
		int sel = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (sel >= 0 && sel < SGreen_GetCount())
				SGreen_MSetNav(sel, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGreenGenerate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SGeom_GetCountModels() == 0)
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

	level_editor::pEditGreenName->getText(tmp_name, 1024);
	level_editor::pEditGreenMask->getText(tmp_tex, 1024);
	level_editor::pEditGreenModel->getText(tmp_model, 1024);
	level_editor::pEditGreenLod1->getText(tmp_lod1, 1024);
	level_editor::pEditGreenLod2->getText(tmp_lod2, 1024);
	level_editor::pEditGreenNav->getText(tmp_navmesh, 1024);

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

	if (!FileExistsFile(path_model))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model [", path_model, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (STR_VALIDATE(tmp_navmesh) && !FileExistsFile(path_navmesh))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model [", path_navmesh, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (greentype == GREEN_TYPE_TREE)
	{
		if (!FileExistsFile(path_lod1))
		{
			char tmpstr[2048];
			sprintf(tmpstr, "%s%s%s", "Model [", path_lod1, "] not found");
			MessageBox(0, tmpstr, 0, 0);
			return 0;
		}

		if (!FileExistsFile(path_lod2))
		{
			char tmpstr[2048];
			sprintf(tmpstr, "%s%s%s", "Model [", path_lod2, "] not found");
			MessageBox(0, tmpstr, 0, 0);
			return 0;
		}
	}

	if (!STR_VALIDATE(tmp_name))
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

	float pos = level_editor::pTrackBarGreenDensity->getPos();

	SXPhysics_LoadGeom(NULL);

	SGreen_AddGreen(tmp_name,
		tmp_tex,
		false,
		pos,
		tmp_model,
		(tmp_lod1[0] ? tmp_lod1 : 0),
		(tmp_lod2[0] ? tmp_lod2 : 0),
		(tmp_navmesh[0] ? tmp_navmesh : 0));

	char tmpnamecountpoly[1024];
		sprintf(tmpnamecountpoly, "%s | %s | %d",
			SGreen_MGetName(SGreen_GetCount() - 1),
			(SGreen_MGetTypeCountGen(SGreen_GetCount() - 1) == GREEN_TYPE_GRASS ? "grass" : "tree/shrub"),
			SGreen_MGetCountGen(SGreen_GetCount() - 1));
		level_editor::pListBoxList->addItem(tmpnamecountpoly);

	level_editor::GreenActivateCreate(false);
	level_editor::pListBoxList->setSel(level_editor::pListBoxList->getItemCount() - 1);

	level_editor::GreenSel(level_editor::pListBoxList->getItemCount() - 1);

	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
	level_editor::idActiveElement = level_editor::pListBoxList->getSel();

	return 0;
}