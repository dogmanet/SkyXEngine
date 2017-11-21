
#include "model_callback.h"

void SXLevelEditor::GeomActivateAll(bool bf)
{
	SXLevelEditor::GeomActivateCreate(bf);
	SXLevelEditor::GeomActivateTrans(bf);
}

void SXLevelEditor::GeomActivateCreate(bool bf)
{
	SXLevelEditor::StaticGeomName->Visible(bf);
	SXLevelEditor::StaticGeomModel->Visible(bf);
	SXLevelEditor::StaticGeomLod1->Visible(bf);
	SXLevelEditor::EditGeomName->Visible(bf);
	SXLevelEditor::EditGeomName->SetText("");
	SXLevelEditor::EditGeomModel->Visible(bf);
	SXLevelEditor::EditGeomModel->SetText("");
	SXLevelEditor::EditGeomLod1->Visible(bf);
	SXLevelEditor::EditGeomLod1->SetText("");
	SXLevelEditor::ButtonGeomLod1->Visible(bf);
	SXLevelEditor::ButtonGeomModel->Visible(bf);
	SXLevelEditor::ButtonGeomFinish->Visible(bf);
}


void SXLevelEditor::GeomActivateTrans(bool bf)
{
	SXLevelEditor::StaticGeomPos->Visible(bf);
	SXLevelEditor::EditGeomPosX->Visible(bf);
	SXLevelEditor::EditGeomPosY->Visible(bf);
	SXLevelEditor::EditGeomPosZ->Visible(bf);
	SXLevelEditor::RadioButtonGeomPosX->Visible(bf);
	SXLevelEditor::RadioButtonGeomPosY->Visible(bf);
	SXLevelEditor::RadioButtonGeomPosZ->Visible(bf);
	SXLevelEditor::StaticGeomRot->Visible(bf);
	SXLevelEditor::EditGeomRotX->Visible(bf);
	SXLevelEditor::EditGeomRotY->Visible(bf);
	SXLevelEditor::EditGeomRotZ->Visible(bf);
	SXLevelEditor::RadioButtonGeomRotX->Visible(bf);
	SXLevelEditor::RadioButtonGeomRotY->Visible(bf);
	SXLevelEditor::RadioButtonGeomRotZ->Visible(bf);
	SXLevelEditor::StaticGeomScale->Visible(bf);
	SXLevelEditor::EditGeomScaleX->Visible(bf);
	SXLevelEditor::EditGeomScaleY->Visible(bf);
	SXLevelEditor::EditGeomScaleZ->Visible(bf);
	SXLevelEditor::RadioButtonGeomScaleX->Visible(bf);
	SXLevelEditor::RadioButtonGeomScaleY->Visible(bf);
	SXLevelEditor::RadioButtonGeomScaleZ->Visible(bf);

	SXLevelEditor::EditGeomModel->Enable(true);
	SXLevelEditor::ButtonGeomModel->Enable(true);
}

void SXLevelEditor::GeomSel(int sel)
{
	if (sel >= 0 && sel < SGeom_ModelsGetCount())
	{
		SXLevelEditor::ActiveElement = sel;
		SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;

		SXLevelEditor::GeomActivateTrans(true);

		SXLevelEditor::EditGeomModel->SetText(SGeom_ModelsMGetPathName(sel));
		SXLevelEditor::EditGeomLod1->SetText(SGeom_ModelsMGetLodPath(sel));
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

		SXLevelEditor::EditGeomName->SetText(tmpname);

		char tmpval[64];

		sprintf(tmpval, "%f", pos->x);
		SXLevelEditor::EditGeomPosX->SetText(tmpval);

		sprintf(tmpval, "%f", pos->y);
		SXLevelEditor::EditGeomPosY->SetText(tmpval);

		sprintf(tmpval, "%f", pos->z);
		SXLevelEditor::EditGeomPosZ->SetText(tmpval);


		sprintf(tmpval, "%f", rot->x);
		SXLevelEditor::EditGeomRotX->SetText(tmpval);

		sprintf(tmpval, "%f", rot->y);
		SXLevelEditor::EditGeomRotY->SetText(tmpval);

		sprintf(tmpval, "%f", rot->z);
		SXLevelEditor::EditGeomRotZ->SetText(tmpval);


		sprintf(tmpval, "%f", scale->x);
		SXLevelEditor::EditGeomScaleX->SetText(tmpval);

		sprintf(tmpval, "%f", scale->y);
		SXLevelEditor::EditGeomScaleY->SetText(tmpval);

		sprintf(tmpval, "%f", scale->z);
		SXLevelEditor::EditGeomScaleZ->SetText(tmpval);

		SXLevelEditor::EditGeomModel->Enable(false);
		SXLevelEditor::ButtonGeomModel->Enable(false);
		SXLevelEditor::ButtonGeomFinish->Visible(false);
	}
}

LRESULT SXLevelEditor_EditGeomName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int sel = SXLevelEditor::ListBoxList->GetSel();
	if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
	{
		SXLevelEditor::EditGeomName->GetText(SGeom_ModelsMGetName(sel), 64);
	}

	return 0;
}

LRESULT SXLevelEditor_ButtonGeomModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGeomModel->SetText(tmpname);
	}
	return 0;
}

LRESULT SXLevelEditor_ButtonGeomLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), FILE_FILTER_MODEL);
	if (def_str_validate(tmppath))
	{
		StrCutMesh(tmppath, tmpname);
		SXLevelEditor::EditGeomLod1->SetText(tmpname);
		int sel = SXLevelEditor::ListBoxList->GetSel();
		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (sel >= 0 && sel < SGeom_ModelsGetCount())
				SGeom_ModelsMSetLodPath(sel, tmpname);
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
	SXLevelEditor::EditGeomName->GetText(model_name, 256);
	SXLevelEditor::EditGeomModel->GetText(path_model, 1024);
	SXLevelEditor::EditGeomLod1->GetText(path_model_lod, 1024);

	if (!def_str_validate(path_model))
	{
		return 0;
	}

	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path_model);
	if (!Core_0FileExists(tmppath))
	{
		char tmpstr[2048];
		sprintf(tmpstr, "%s%s%s", "Model [", tmppath, "] not found");
		MessageBox(0, tmpstr, 0, 0);
		return 0;
	}

	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path_model_lod);
	if (def_str_validate(path_model_lod) && !Core_0FileExists(tmppath))
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
	SXLevelEditor::ListBoxList->AddItem(tmpnamecountpoly);

	SXLevelEditor::GeomActivateTrans(true);
	SXLevelEditor::ListBoxList->SetSel(SXLevelEditor::ListBoxList->GetCountItem() - 1);
	SXLevelEditor::GeomSel(SXLevelEditor::ListBoxList->GetSel());

	SXLevelEditor::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;
	SXLevelEditor::ActiveElement = SXLevelEditor::ListBoxList->GetSel();

	return 0;
}

LRESULT SXLevelEditor_EditTransformPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = SXLevelEditor::ListBoxList->GetSel();
	float3* pos = SGeom_ModelsMGetPosition(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditGeomPosX->GetHWND())
	{
		SXLevelEditor::EditGeomPosX->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->x));
	}
	else if (hwnd == SXLevelEditor::EditGeomPosY->GetHWND())
	{
		SXLevelEditor::EditGeomPosY->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->y));
	}
	else if (hwnd == SXLevelEditor::EditGeomPosZ->GetHWND())
	{
		SXLevelEditor::EditGeomPosZ->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(pos->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}

LRESULT SXLevelEditor_EditTransformRot_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = SXLevelEditor::ListBoxList->GetSel();
	float3* rot = SGeom_ModelsMGetRotation(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditGeomRotX->GetHWND())
	{
		SXLevelEditor::EditGeomRotX->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->x));
	}
	else if (hwnd == SXLevelEditor::EditGeomRotY->GetHWND())
	{
		SXLevelEditor::EditGeomRotY->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->y));
	}
	else if (hwnd == SXLevelEditor::EditGeomRotZ->GetHWND())
	{
		SXLevelEditor::EditGeomRotZ->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(rot->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}

LRESULT SXLevelEditor_EditTransformScale_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (SXLevelEditor::ActiveGroupType != EDITORS_LEVEL_GROUPTYPE_GEOM)
		return 0;
	int sel = SXLevelEditor::ListBoxList->GetSel();
	float3* scale = SGeom_ModelsMGetScale(sel);
	char tmpstr[64];
	if (hwnd == SXLevelEditor::EditGeomScaleX->GetHWND())
	{
		SXLevelEditor::EditGeomScaleX->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->x));
	}
	else if (hwnd == SXLevelEditor::EditGeomScaleY->GetHWND())
	{
		SXLevelEditor::EditGeomScaleY->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->y));
	}
	else if (hwnd == SXLevelEditor::EditGeomScaleZ->GetHWND())
	{
		SXLevelEditor::EditGeomScaleZ->GetText(tmpstr, 64);
		sscanf(tmpstr, "%f", &(scale->z));
	}

	SGeom_ModelsMApplyTransform(sel);

	return 0;
}