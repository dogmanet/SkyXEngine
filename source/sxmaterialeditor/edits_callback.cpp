


//////////

LRESULT SXMaterialEditor_EditSkyBox_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditSkyBox->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SGCore_SkyBoxLoadTex(tmptextval);
	else
	{
		SGCore_SkyBoxGetActiveTex(tmptextval);
		SXMaterialEditor::EditSkyBox->SetText(tmptextval);

		MessageBox(0,"texture not found",0,0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditTex->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);
	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetTexture(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetTexture(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditTex->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_SHADER_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditVS->GetText(tmptextval, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(tmptextval))
		SML_MtlSetVS(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetVS(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditVS->SetText(tmptextval);

		MessageBox(0, "shader not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_SHADER_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditPS->GetText(tmptextval, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(tmptextval))
		SML_MtlSetPS(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetPS(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditPS->SetText(tmptextval);

		MessageBox(0, "shader not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPenetration_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditPenetration->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarPenetration->SetPos(tmpval * 100);
	SML_MtlSetPenetration(SXMaterialEditor::IDMat, tmpval*0.01f);

	return 0;
}


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;
	
	SXMaterialEditor::EditTexLighting->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetTextureLighting(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetTextureLighting(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditTexLighting->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;
	
	SXMaterialEditor::EditMask->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMaskTex(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetMaskTex(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditMask->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;
	
	SXMaterialEditor::EditMR->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 0, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 0, tmptextval);
		SXMaterialEditor::EditMR->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditMG->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 1, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 1, tmptextval);
		SXMaterialEditor::EditMG->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditMB->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 2, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 2, tmptextval);
		SXMaterialEditor::EditMB->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditMA->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 3, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 3, tmptextval);
		SXMaterialEditor::EditMA->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDR->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 0, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 0, tmptextval);
		SXMaterialEditor::EditDR->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDG->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 1, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 1, tmptextval);
		SXMaterialEditor::EditDR->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDB->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 2, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 2, tmptextval);
		SXMaterialEditor::EditDR->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDA->GetText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 3, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 3, tmptextval);
		SXMaterialEditor::EditDR->SetText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

/////////////

LRESULT SXMaterialEditor_EditRoughness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditRoughness->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarRoughness->SetPos(tmpval*100);
	SML_MtlSetRoughness(SXMaterialEditor::IDMat, tmpval);

	return 0;
}

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditF0->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarF0->SetPos(tmpval * 100);
	SML_MtlSetF0(SXMaterialEditor::IDMat, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditThickness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditThickness->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarThickness->SetPos(tmpval * 100);
	SML_MtlSetThickness(SXMaterialEditor::IDMat, tmpval);
	return 0;
}



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSX->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSX->SetPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSY->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSY->SetPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSZ->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSZ->SetPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSW->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSW->SetPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 3, tmpval);
	return 0;
}


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSX->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSX->SetPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSY->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSY->SetPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSZ->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSZ->SetPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSW->GetText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSW->SetPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 3, tmpval);
	return 0;
}