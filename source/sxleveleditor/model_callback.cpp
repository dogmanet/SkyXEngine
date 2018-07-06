
#include "model_callback.h"

void SXLevelEditor::GeomActivateAll(bool bf)
{
	SXLevelEditor::GeomActivateCreate(bf);
	SXLevelEditor::GeomActivateTrans(bf);
}

void SXLevelEditor::GeomActivateCreate(bool bf)
{
	SXLevelEditor::StaticGeomName->setVisible(bf);
	SXLevelEditor::StaticGeomModel->setVisible(bf);
	SXLevelEditor::StaticGeomLod1->setVisible(bf);
	SXLevelEditor::EditGeomName->setVisible(bf);
	SXLevelEditor::EditGeomName->setText("");
	SXLevelEditor::EditGeomModel->setVisible(bf);
	SXLevelEditor::EditGeomModel->setText("");
	SXLevelEditor::EditGeomLod1->setVisible(bf);
	SXLevelEditor::EditGeomLod1->setText("");
	SXLevelEditor::ButtonGeomLod1->setVisible(bf);
	SXLevelEditor::ButtonGeomModel->setVisible(bf);
	SXLevelEditor::ButtonGeomFinish->setVisible(bf);
}


void SXLevelEditor::GeomActivateTrans(bool bf)
{
	SXLevelEditor::StaticGeomPos->setVisible(bf);
	SXLevelEditor::EditGeomPosX->setVisible(bf);
	SXLevelEditor::EditGeomPosY->setVisible(bf);
	SXLevelEditor::EditGeomPosZ->setVisible(bf);
	SXLevelEditor::RadioButtonGeomPosX->setVisible(bf);
	SXLevelEditor::RadioButtonGeomPosY->setVisible(bf);
	SXLevelEditor::RadioButtonGeomPosZ->setVisible(bf);
	SXLevelEditor::StaticGeomRot->setVisible(bf);
	SXLevelEditor::EditGeomRotX->setVisible(bf);
	SXLevelEditor::EditGeomRotY->setVisible(bf);
	SXLevelEditor::EditGeomRotZ->setVisible(bf);
	SXLevelEditor::RadioButtonGeomRotX->setVisible(bf);
	SXLevelEditor::RadioButtonGeomRotY->setVisible(bf);
	SXLevelEditor::RadioButtonGeomRotZ->setVisible(bf);
	SXLevelEditor::StaticGeomScale->setVisible(bf);
	SXLevelEditor::EditGeomScaleX->setVisible(bf);
	SXLevelEditor::EditGeomScaleY->setVisible(bf);
	SXLevelEditor::EditGeomScaleZ->setVisible(bf);
	SXLevelEditor::RadioButtonGeomScaleX->setVisible(bf);
	SXLevelEditor::RadioButtonGeomScaleY->setVisible(bf);
	SXLevelEditor::RadioButtonGeomScaleZ->setVisible(bf);

	SXLevelEditor::EditGeomModel->setEnable(true);
	SXLevelEditor::ButtonGeomModel->setEnable(true);
}

void SXLevelEditor::GeomSel(int sel)
{
	if (sel >= 0 && sel < SGeom_ModelsGetCount())
	{
		SXLevelEditor::ActiveElement = sel;
		SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;

		SXLevelEditor::GeomActivateTrans(true);

		SXLevelEditor::EditGeomModel->setText(SGeom_ModelsMGetPathName(sel));
		SXLevelEditor::EditGeomLod1->setText(SGeom_ModelsMGetLodPath(sel));
		char* tmpname = SGeom_ModelsMGetName(sel);
		float3* pos = SGeom_ModelsMGetPosition(sel);
		float3* rot = SGeom_ModelsMGetRotation(sel);
		float3* scale = SGeom_ModelsMGetScale(sel);

		float3 min, max;
		SGeom_ModelsMGetMinMax(sel, &min, &max);

		SXLevelEditor::HelperPos = (max + min) * 0.5f;
		SXLevelEditor::HelperScale = *scale;

		SXLevelEditor::ObjAxesHelper->SetPosition(SXLevelEditor::HelperPos);
		SXLevelEditor::ObjAxesHelper->SetRotation(*rot);
		SXLevelEditor::ObjAxesHelper->SetScale(float3(1,1,1));

		SXLevelEditor::EditGeomName->setText(tmpname);

		char tmpval[64];

		sprintf(tmpval, "%f", pos->x);
		SXLevelEditor::EditGeomPosX->setText(tmpval);

		sprintf(tmpval, "%f", pos->y);
		SXLevelEditor::EditGeomPosY->setText(tmpval);

		sprintf(tmpval, "%f", pos->z);
		SXLevelEditor::EditGeomPosZ->setText(tmpval);


		sprintf(tmpval, "%f", rot->x);
		SXLevelEditor::EditGeomRotX->setText(tmpval);

		sprintf(tmpval, "%f", rot->y);
		SXLevelEditor::EditGeomRotY->setText(tmpval);

		sprintf(tmpval, "%f", rot->z);
		SXLevelEditor::EditGeomRotZ->setText(tmpval);


		sprintf(tmpval, "%f", scale->x);
		SXLevelEditor::EditGeomScaleX->setText(tmpval);

		sprintf(tmpval, "%f", scale->y);
		SXLevelEditor::EditGeomScaleY->setText(tmpval);

		sprintf(tmpval, "%f", scale->z);
		SXLevelEditor::EditGeomScaleZ->setText(tmpval);

		SXLevelEditor::EditGeomModel->setEnable(false);
		SXLevelEditor::ButtonGeomModel->setEnable(false);
		SXLevelEditor::ButtonGeomFinish->setVisible(false);
	}
}

LRESULT SXLevelEditor_EditGeomName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->getSel();
	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		SXLevelEditor::EditGeomName->getText(SGeom_ModelsMGetName(sel), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGeomModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), SXLevelEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		SXLevelEditor::EditGeomModel->setText(sRpath.c_str());
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGeomLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFileStd(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), "dse", "Select model", true, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), SXLevelEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetDSEinfo))
	{
		String sRpath = StrCutStrI(tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
		SXLevelEditor::EditGeomLod1->setText(sRpath.c_str());
		int sel = SXLevelEditor::ListBoxList->getSel();
		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (sel >= 0 && sel < SGeom_ModelsGetCount())
				SGeom_ModelsMSetLodPath(sel, sRpath.c_str());
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
	SXLevelEditor::EditGeomName->getText(model_name, 256);
	SXLevelEditor::EditGeomModel->getText(path_model, 1024);
	SXLevelEditor::EditGeomLod1->getText(path_model_lod, 1024);

	if (!def_str_validate(path_model))
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
	if (def_str_validate(path_model_lod) && !FileExistsFile(tmppath))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model/lod [", path_model_lod, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	if (!def_str_validate(model_name))
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


	SGeom_ModelsAddModel(path_model, (def_str_validate(path_model_lod) ? path_model_lod : 0), model_name);

	char tmpnamecountpoly[1024];
	sprintf(tmpnamecountpoly, "%s | %d", SGeom_ModelsMGetName(SGeom_ModelsGetCount() - 1), SGeom_ModelsMGetCountPoly(SGeom_ModelsGetCount() - 1));
	SXLevelEditor::ListBoxList->addItem(tmpnamecountpoly);

	SXLevelEditor::GeomActivateTrans(true);
	SXLevelEditor::ListBoxList->setSel(SXLevelEditor::ListBoxList->getItemCount() - 1);
	SXLevelEditor::GeomSel(SXLevelEditor::ListBoxList->getSel());

	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;
	SXLevelEditor::ActiveElement = SXLevelEditor::ListBoxList->getSel();

	return 0;
}

LRESULT SXLevelEditor_EditTransformPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = SXLevelEditor::ListBoxList->getSel();
	float3* pos = SGeom_ModelsMGetPosition(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditGeomPosX->getHWND())
	{
		SXLevelEditor::EditGeomPosX->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->x));
	}
	else if (hwnd == SXLevelEditor::EditGeomPosY->getHWND())
	{
		SXLevelEditor::EditGeomPosY->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->y));
	}
	else if (hwnd == SXLevelEditor::EditGeomPosZ->getHWND())
	{
		SXLevelEditor::EditGeomPosZ->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}

LRESULT SXLevelEditor_EditTransformRot_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = SXLevelEditor::ListBoxList->getSel();
	float3* rot = SGeom_ModelsMGetRotation(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditGeomRotX->getHWND())
	{
		SXLevelEditor::EditGeomRotX->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->x));
	}
	else if (hwnd == SXLevelEditor::EditGeomRotY->getHWND())
	{
		SXLevelEditor::EditGeomRotY->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->y));
	}
	else if (hwnd == SXLevelEditor::EditGeomRotZ->getHWND())
	{
		SXLevelEditor::EditGeomRotZ->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}

LRESULT SXLevelEditor_EditTransformScale_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = SXLevelEditor::ListBoxList->getSel();
	float3* scale = SGeom_ModelsMGetScale(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditGeomScaleX->getHWND())
	{
		SXLevelEditor::EditGeomScaleX->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->x));
	}
	else if (hwnd == SXLevelEditor::EditGeomScaleY->getHWND())
	{
		SXLevelEditor::EditGeomScaleY->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->y));
	}
	else if (hwnd == SXLevelEditor::EditGeomScaleZ->getHWND())
	{
		SXLevelEditor::EditGeomScaleZ->getText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}