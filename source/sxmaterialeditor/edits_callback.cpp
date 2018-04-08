
#include "edits_callback.h"

LRESULT SXMaterialEditor_EditSkyBox_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditSkyBox->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SGCore_SkyBoxLoadTex(tmptextval);
	else
	{
		SGCore_SkyBoxGetActiveTex(tmptextval);
		SXMaterialEditor::EditSkyBox->setText(tmptextval);

		MessageBox(0,"texture not found",0,0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditTex->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);
	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetTexture(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetTexture(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditTex->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_SHADER_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditVS->getText(tmptextval, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(tmptextval))
		SML_MtlSetVS(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetVS(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditVS->setText(tmptextval);

		MessageBox(0, "shader not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_SHADER_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditPS->getText(tmptextval, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(tmptextval))
		SML_MtlSetPS(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetPS(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditPS->setText(tmptextval);

		MessageBox(0, "shader not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPenetration_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditPenetration->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarPenetration->setPos(tmpval * 100);
	SML_MtlSetPenetration(SXMaterialEditor::IDMat, tmpval*0.01f);

	return 0;
}


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;
	
	SXMaterialEditor::EditTexLighting->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetTextureLighting(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetTextureLighting(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditTexLighting->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;
	
	SXMaterialEditor::EditMask->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMaskTex(SXMaterialEditor::IDMat, tmptextval);
	else
	{
		SML_MtlGetMaskTex(SXMaterialEditor::IDMat, tmptextval);
		SXMaterialEditor::EditMask->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;
	
	SXMaterialEditor::EditMR->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 0, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 0, tmptextval);
		SXMaterialEditor::EditMR->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditMG->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 1, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 1, tmptextval);
		SXMaterialEditor::EditMG->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditMB->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 2, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 2, tmptextval);
		SXMaterialEditor::EditMB->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditMA->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 3, tmptextval);
	else
	{
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 3, tmptextval);
		SXMaterialEditor::EditMA->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDR->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 0, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 0, tmptextval);
		SXMaterialEditor::EditDR->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDG->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 1, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 1, tmptextval);
		SXMaterialEditor::EditDR->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDB->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 2, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 2, tmptextval);
		SXMaterialEditor::EditDR->setText(tmptextval);

		MessageBox(0, "texture not found", 0, 0);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	tmptextval[0] = 0;

	SXMaterialEditor::EditDA->getText(tmptextval, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(tmptextval))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 3, tmptextval);
	else
	{
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 3, tmptextval);
		SXMaterialEditor::EditDR->setText(tmptextval);

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

	SXMaterialEditor::EditRoughness->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarRoughness->setPos(tmpval*100);
	SML_MtlSetRoughness(SXMaterialEditor::IDMat, tmpval);

	return 0;
}

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditF0->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarF0->setPos(tmpval * 100);
	SML_MtlSetF0(SXMaterialEditor::IDMat, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditThickness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditThickness->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarThickness->setPos(tmpval * 100);
	SML_MtlSetThickness(SXMaterialEditor::IDMat, tmpval);
	return 0;
}



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSX->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSX->setPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSY->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSY->setPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSZ->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSZ->setPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSW->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSW->setPos(tmpval * 100);
	SML_MtlSetUDVS(SXMaterialEditor::IDMat, 3, tmpval);
	return 0;
}


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSX->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSX->setPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSY->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSY->setPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSZ->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSZ->setPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmptextval[256];
	tmptextval[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSW->getText(tmptextval, 256);

	sscanf(tmptextval, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSW->setPos(tmpval * 100);
	SML_MtlSetUDPS(SXMaterialEditor::IDMat, 3, tmpval);
	return 0;
}