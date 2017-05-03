
LRESULT SXMaterialEditor_ButtonSkyBox_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditSkyBox->SetText(tmpname);

		SGCore_SkyBoxLoadTex(tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonTex_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditTex->SetText(tmpname);
		
		SML_MtlSetTexture(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonVS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Shaders, FILE_FILTER_VS);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditVS->SetText(tmpname);

		SML_MtlSetVS(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonPS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Shaders, FILE_FILTER_PS);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditPS->SetText(tmpname);

		SML_MtlSetPS(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonTexLighting_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditTexLighting->SetText(tmpname);

		SML_MtlSetTextureLighting(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMask->SetText(tmpname);

		SML_MtlSetMaskTex(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMR->SetText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMG->SetText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMB->SetText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMA->SetText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 3, tmpname);
	}
	return 0;
}


LRESULT SXMaterialEditor_ButtonDR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDR->SetText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDG->SetText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDB->SetText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	SXGUIDialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, GData::Pathes::Textures, FILE_FILTER_TEXTURE);
	if (def_str_validate(tmppath))
	{
		StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDA->SetText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 3, tmpname);
	}
	return 0;
}