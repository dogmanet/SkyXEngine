
#include "model_callback.h"

void level_editor::GeomActivateAll(bool bf)
{
	level_editor::GeomActivateCreate(bf);
	level_editor::GeomActivateTrans(bf);
}

void level_editor::GeomActivateCreate(bool bf)
{
	level_editor::pStaticGeomName->setVisible(bf);
	level_editor::pStaticGeomModel->setVisible(bf);
	level_editor::pStaticGeomLod1->setVisible(bf);
	level_editor::pEditGeomName->setVisible(bf);
	level_editor::pEditGeomName->setText("");
	level_editor::pEditGeomModel->setVisible(bf);
	level_editor::pEditGeomModel->setText("");
	level_editor::pEditGeomLod1->setVisible(bf);
	level_editor::pEditGeomLod1->setText("");
	level_editor::pButtonGeomLod1->setVisible(bf);
	level_editor::pButtonGeomModel->setVisible(bf);
	level_editor::pButtonGeomFinish->setVisible(bf);
}


void level_editor::GeomActivateTrans(bool bf)
{
	level_editor::pStaticGeomPos->setVisible(bf);
	level_editor::pEditGeomPosX->setVisible(bf);
	level_editor::pEditGeomPosY->setVisible(bf);
	level_editor::pEditGeomPosZ->setVisible(bf);
	level_editor::pRadioButtonGeomPosX->setVisible(bf);
	level_editor::pRadioButtonGeomPosY->setVisible(bf);
	level_editor::pRadioButtonGeomPosZ->setVisible(bf);
	level_editor::pStaticGeomRot->setVisible(bf);
	level_editor::pEditGeomRotX->setVisible(bf);
	level_editor::pEditGeomRotY->setVisible(bf);
	level_editor::pEditGeomRotZ->setVisible(bf);
	level_editor::pRadioButtonGeomRotX->setVisible(bf);
	level_editor::pRadioButtonGeomRotY->setVisible(bf);
	level_editor::pRadioButtonGeomRotZ->setVisible(bf);
	level_editor::pStaticGeomScale->setVisible(bf);
	level_editor::pEditGeomScaleX->setVisible(bf);
	level_editor::pEditGeomScaleY->setVisible(bf);
	level_editor::pEditGeomScaleZ->setVisible(bf);
	level_editor::pRadioButtonGeomScaleX->setVisible(bf);
	level_editor::pRadioButtonGeomScaleY->setVisible(bf);
	level_editor::pRadioButtonGeomScaleZ->setVisible(bf);

	level_editor::pEditGeomModel->setEnable(true);
	level_editor::pButtonGeomModel->setEnable(true);
}

void level_editor::GeomSel(int sel)
{
	if (sel >= 0 && sel < SGeom_GetCountModels())
	{
		level_editor::idActiveElement = sel;
		level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;

		level_editor::GeomActivateTrans(true);

		level_editor::pEditGeomModel->setText(SGeom_ModelGetPath4Model(sel));
		level_editor::pEditGeomLod1->setText(SGeom_ModelGetPath4Lod(sel));
		const char* tmpname = SGeom_ModelGetName(sel);
		const float3* pos = SGeom_ModelGetPosition(sel);
		const float3* rot = SGeom_ModelGetRotation(sel);
		const float3* scale = SGeom_ModelGetScale(sel);

		float3 min, max;
		SGeom_ModelGetMinMax(sel, &min, &max);

		level_editor::vHelperPos = (max + min) * 0.5f;
		level_editor::vHelperScale = *scale;

		level_editor::pAxesHelper->setPosition(level_editor::vHelperPos);
		level_editor::pAxesHelper->setRotation(*rot);
		level_editor::pAxesHelper->setScale(float3(1,1,1));

		level_editor::pEditGeomName->setText(tmpname);

		char tmpval[64];

		sprintf(tmpval, "%f", pos->x);
		level_editor::pEditGeomPosX->setText(tmpval);

		sprintf(tmpval, "%f", pos->y);
		level_editor::pEditGeomPosY->setText(tmpval);

		sprintf(tmpval, "%f", pos->z);
		level_editor::pEditGeomPosZ->setText(tmpval);


		sprintf(tmpval, "%f", rot->x);
		level_editor::pEditGeomRotX->setText(tmpval);

		sprintf(tmpval, "%f", rot->y);
		level_editor::pEditGeomRotY->setText(tmpval);

		sprintf(tmpval, "%f", rot->z);
		level_editor::pEditGeomRotZ->setText(tmpval);


		sprintf(tmpval, "%f", scale->x);
		level_editor::pEditGeomScaleX->setText(tmpval);

		sprintf(tmpval, "%f", scale->y);
		level_editor::pEditGeomScaleY->setText(tmpval);

		sprintf(tmpval, "%f", scale->z);
		level_editor::pEditGeomScaleZ->setText(tmpval);

		level_editor::pEditGeomModel->setEnable(false);
		level_editor::pButtonGeomModel->setEnable(false);
		level_editor::pButtonGeomFinish->setVisible(false);
	}
}

LRESULT SXLevelEditor_EditGeomName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = level_editor::pListBoxList->getSel();
	if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		char szStr[64];
		level_editor::pEditGeomName->getText(szStr, 64);
		SGeom_ModelSetName(sel, szStr);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGeomModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGeomModel->setText(sRpath.c_str());
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGeomLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), level_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		level_editor::pEditGeomLod1->setText(sRpath.c_str());
		int sel = level_editor::pListBoxList->getSel();
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (sel >= 0 && sel < SGeom_GetCountModels())
				SGeom_ModelSetPath4Lod(sel, sRpath.c_str());
		}
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGeomFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char path_model[1024];
	char path_model_lod[1024];

	//sprintf(path_model, "%s", GData::Pathes::Meshes);
	//sprintf(path_model_lod, "%s", GData::Pathes::Meshes);

	char model_name[256];
	model_name[0] = 0;
	level_editor::pEditGeomName->getText(model_name, 256);
	level_editor::pEditGeomModel->getText(path_model, 1024);
	level_editor::pEditGeomLod1->getText(path_model_lod, 1024);

	if (!STR_VALIDATE(path_model))
	{
		return 0;
	}

	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path_model);
	if (!FileExistsFile(tmppath))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model [", tmppath, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path_model_lod);
	if (STR_VALIDATE(path_model_lod) && !FileExistsFile(tmppath))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model/lod [", path_model_lod, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (!STR_VALIDATE(model_name))
	{
		if (MessageBox(0, "No name for the model, enter the file name automatically?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDYES)
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


	SGeom_ModelAdd(path_model, model_name, (STR_VALIDATE(path_model_lod) ? path_model_lod : 0), 0, false);

	char tmpnamecountpoly[1024];
	sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelGetName(SGeom_GetCountModels() - 1), SGeom_ModelGetCountPoly(SGeom_GetCountModels() - 1));
	level_editor::pListBoxList->addItem(tmpnamecountpoly);

	level_editor::GeomActivateTrans(true);
	level_editor::pListBoxList->setSel(level_editor::pListBoxList->getItemCount() - 1);
	level_editor::GeomSel(level_editor::pListBoxList->getSel());

	level_editor::iActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;
	level_editor::idActiveElement = level_editor::pListBoxList->getSel();

	return 0;
}

LRESULT SXLevelEditor_EditTransformPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = level_editor::pListBoxList->getSel();
	float3 pos = *(SGeom_ModelGetPosition(sel));
	char tmpstr[64];
	if (hwnd == level_editor::pEditGeomPosX->getHWND())
	{
		level_editor::pEditGeomPosX->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos.x));
	}
	else if (hwnd == level_editor::pEditGeomPosY->getHWND())
	{
		level_editor::pEditGeomPosY->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos.y));
	}
	else if (hwnd == level_editor::pEditGeomPosZ->getHWND())
	{
		level_editor::pEditGeomPosZ->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos.z));
	}

	SGeom_ModelSetPosition(sel, &pos);

	return 0;
}

LRESULT SXLevelEditor_EditTransformRot_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = level_editor::pListBoxList->getSel();
	float3 rot = *(SGeom_ModelGetRotation(sel));
	char tmpstr[64];
	if (hwnd == level_editor::pEditGeomRotX->getHWND())
	{
		level_editor::pEditGeomRotX->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot.x));
	}
	else if (hwnd == level_editor::pEditGeomRotY->getHWND())
	{
		level_editor::pEditGeomRotY->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot.y));
	}
	else if (hwnd == level_editor::pEditGeomRotZ->getHWND())
	{
		level_editor::pEditGeomRotZ->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot.z));
	}

	SGeom_ModelSetRotation(sel, &rot);

	return 0;
}

LRESULT SXLevelEditor_EditTransformScale_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (level_editor::iActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = level_editor::pListBoxList->getSel();
	float3 scale = *(SGeom_ModelGetScale(sel));
	char tmpstr[64];
	if (hwnd == level_editor::pEditGeomScaleX->getHWND())
	{
		level_editor::pEditGeomScaleX->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale.x));
	}
	else if (hwnd == level_editor::pEditGeomScaleY->getHWND())
	{
		level_editor::pEditGeomScaleY->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale.y));
	}
	else if (hwnd == level_editor::pEditGeomScaleZ->getHWND())
	{
		level_editor::pEditGeomScaleZ->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale.z));
	}

	SGeom_ModelSetScale(sel, &scale);

	return 0;
}