
#include "edits_callback.h"

void TexProcInputString(const char *szStr)
{
	if (StrTrim(szStr).length() > 0)
	{
		char szMessage[1024];
		szMessage[0] = 0;
		sprintf(szMessage, "Texture '%s' not found", szStr);
		MessageBox(0, szMessage, 0, 0);
	}
}

LRESULT SXMaterialEditor_EditSkyBox_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditSkyBox->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	//MessageBox(0, "it is not done yet :)", 0, 0);
	if (SGCore_LoadTexFileExists(szStr))
		SGCore_SkyBoxLoadTex(szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SGCore_SkyBoxGetActiveTex(szStr2);
		SXMaterialEditor::EditSkyBox->setText(szStr2);
		TexProcInputString(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditTex->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);
	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetTexture(SXMaterialEditor::IDMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetTexture(SXMaterialEditor::IDMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditTex->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_SHADER_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditVS->getText(szStr, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(szStr))
		SML_MtlSetVS(SXMaterialEditor::IDMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetVS(SXMaterialEditor::IDMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditVS->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_SHADER_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditPS->getText(szStr, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(szStr))
		SML_MtlSetPS(SXMaterialEditor::IDMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetPS(SXMaterialEditor::IDMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditPS->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditHitChance_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	SXMaterialEditor::EditHitChance->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SXMaterialEditor::TrackBarHitChance->setPos(fValue * 100);
	SML_MtlSetHitChance(SXMaterialEditor::IDMat, fValue*0.01f);
	SXMaterialEditor::EditHitChance->setText(String(fValue).c_str());

	return 0;
}

LRESULT SXMaterialEditor_EditDurability_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	SXMaterialEditor::EditDurability->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SML_MtlSetDurability(SXMaterialEditor::IDMat, fValue);
	SXMaterialEditor::EditDurability->setText(String(fValue).c_str());

	return 0;
}

LRESULT SXMaterialEditor_EditDensity_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	SXMaterialEditor::EditDensity->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SML_MtlSetDensity(SXMaterialEditor::IDMat, fValue);
	SXMaterialEditor::EditDensity->setText(String(fValue).c_str());

	return 0;
}


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	SXMaterialEditor::EditTexLighting->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetTextureLighting(SXMaterialEditor::IDMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetTextureLighting(SXMaterialEditor::IDMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditTexLighting->setText(szStr);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	SXMaterialEditor::EditMask->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMaskTex(SXMaterialEditor::IDMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMaskTex(SXMaterialEditor::IDMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditMask->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	SXMaterialEditor::EditMR->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 0, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 0, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditMR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditMG->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 1, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 1, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditMG->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditMB->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 2, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 2, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditMB->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditMA->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 3, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::IDMat, 3, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditMA->setText(szStr);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditDR->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 0, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 0, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditDG->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 1, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 1, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditDB->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 2, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 2, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::EditDA->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::IDMat, 3, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::IDMat, 3, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::EditDR->setText(szStr);
	}

	return 0;
}

//##########################################################################

LRESULT SXMaterialEditor_EditRoughness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditRoughness->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarRoughness->setPos(tmpval*100);
	SML_MtlSetRoughness(SXMaterialEditor::IDMat, tmpval);

	return 0;
}

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditF0->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarF0->setPos(tmpval * 100);
	SML_MtlSetF0(SXMaterialEditor::IDMat, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditThickness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditThickness->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarThickness->setPos(tmpval * 100);
	SML_MtlSetThickness(SXMaterialEditor::IDMat, tmpval);
	return 0;
}



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSX->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSX->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::IDMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSY->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSY->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::IDMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSZ->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSZ->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::IDMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDVSW->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDVSW->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::IDMat, 3, tmpval);
	return 0;
}


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSX->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSX->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::IDMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSY->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSY->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::IDMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSZ->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSZ->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::IDMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::EditUDPSW->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::TrackBarUDPSW->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::IDMat, 3, tmpval);
	return 0;
}