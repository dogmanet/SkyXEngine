
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

	SXMaterialEditor::pEditSkyBox->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	//MessageBox(0, "it is not done yet :)", 0, 0);
	if (SGCore_LoadTexFileExists(szStr))
		SGCore_SkyBoxLoadTex(szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SGCore_SkyBoxGetActiveTex(szStr2);
		SXMaterialEditor::pEditSkyBox->setText(szStr2);
		TexProcInputString(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditLigthColorR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[32];
	szStr[0] = 0;

	SXMaterialEditor::pEditLigthColorR->getText(szStr, 32);

	ID idGlobalLight = SML_LigthsGetGlobal();

	if (idGlobalLight > -1)
	{
		float3 vColor;
		SML_LigthsGetColor(idGlobalLight, &vColor);
		sscanf(szStr, "%f", &vColor.x);
		SML_LigthsSetColor(idGlobalLight, &vColor);
		SXMaterialEditor::pStaticLigthColor->setColorBrush(RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0)));
	}

	return 0;
}

LRESULT SXMaterialEditor_EditLigthColorG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[32];
	szStr[0] = 0;

	SXMaterialEditor::pEditLigthColorG->getText(szStr, 32);

	ID idGlobalLight = SML_LigthsGetGlobal();

	if (idGlobalLight > -1)
	{
		float3 vColor;
		SML_LigthsGetColor(idGlobalLight, &vColor);
		sscanf(szStr, "%f", &vColor.y);
		SML_LigthsSetColor(idGlobalLight, &vColor);
		SXMaterialEditor::pStaticLigthColor->setColorBrush(RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0)));
	}

	return 0;
}

LRESULT SXMaterialEditor_EditLigthColorB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[32];
	szStr[0] = 0;

	SXMaterialEditor::pEditLigthColorB->getText(szStr, 32);

	ID idGlobalLight = SML_LigthsGetGlobal();

	if (idGlobalLight > -1)
	{
		float3 vColor;
		SML_LigthsGetColor(idGlobalLight, &vColor);
		sscanf(szStr, "%f", &vColor.z);
		SML_LigthsSetColor(idGlobalLight, &vColor);
		SXMaterialEditor::pStaticLigthColor->setColorBrush(RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0)));
	}

	return 0;
}


LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditTex->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);
	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetTexture(SXMaterialEditor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetTexture(SXMaterialEditor::idMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditTex->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_SHADER_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditVS->getText(szStr, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(szStr))
		SML_MtlSetVS(SXMaterialEditor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetVS(SXMaterialEditor::idMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditVS->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_SHADER_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditPS->getText(szStr, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(szStr))
		SML_MtlSetPS(SXMaterialEditor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetPS(SXMaterialEditor::idMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditPS->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditHitChance_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	SXMaterialEditor::pEditHitChance->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SXMaterialEditor::pTrackBarHitChance->setPos(fValue * 100);
	SML_MtlSetHitChance(SXMaterialEditor::idMat, fValue*0.01f);
	SXMaterialEditor::pEditHitChance->setText(String(fValue).c_str());

	return 0;
}

LRESULT SXMaterialEditor_EditDurability_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	SXMaterialEditor::pEditDurability->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SML_MtlSetDurability(SXMaterialEditor::idMat, fValue);
	SXMaterialEditor::pEditDurability->setText(String(fValue).c_str());

	return 0;
}

LRESULT SXMaterialEditor_EditDensity_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	SXMaterialEditor::pEditDensity->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SML_MtlSetDensity(SXMaterialEditor::idMat, fValue);
	SXMaterialEditor::pEditDensity->setText(String(fValue).c_str());

	return 0;
}


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	SXMaterialEditor::pEditTexLighting->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetTextureLighting(SXMaterialEditor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetTextureLighting(SXMaterialEditor::idMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditTexLighting->setText(szStr);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	SXMaterialEditor::pEditMask->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMaskTex(SXMaterialEditor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMaskTex(SXMaterialEditor::idMat, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditMask->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	SXMaterialEditor::pEditMR->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::idMat, 0, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::idMat, 0, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditMR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditMG->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::idMat, 1, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::idMat, 1, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditMG->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditMB->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::idMat, 2, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::idMat, 2, szStr2);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditMB->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditMA->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetMRTex(SXMaterialEditor::idMat, 3, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetMRTex(SXMaterialEditor::idMat, 3, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditMA->setText(szStr);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditDR->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::idMat, 0, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::idMat, 0, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditDG->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::idMat, 1, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::idMat, 1, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditDB->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::idMat, 2, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::idMat, 2, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	SXMaterialEditor::pEditDA->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SML_MtlSetDTex(SXMaterialEditor::idMat, 3, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SML_MtlGetDTex(SXMaterialEditor::idMat, 3, szStr);
		TexProcInputString(szStr);
		
		SXMaterialEditor::pEditDR->setText(szStr);
	}

	return 0;
}

//##########################################################################

LRESULT SXMaterialEditor_EditRoughness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditRoughness->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarRoughness->setPos(tmpval*100);
	SML_MtlSetRoughness(SXMaterialEditor::idMat, tmpval);

	return 0;
}

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditF0->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarF0->setPos(tmpval * 100);
	SML_MtlSetF0(SXMaterialEditor::idMat, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditThickness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditThickness->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarThickness->setPos(tmpval * 100);
	SML_MtlSetThickness(SXMaterialEditor::idMat, tmpval);
	return 0;
}



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDVSX->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDVSX->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDVSY->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDVSY->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDVSZ->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDVSZ->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDVSW->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDVSW->setPos(tmpval * 100);
	SML_MtlSetUserDataVS(SXMaterialEditor::idMat, 3, tmpval);
	return 0;
}


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDPSX->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDPSX->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDPSY->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDPSY->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDPSZ->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDPSZ->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	SXMaterialEditor::pEditUDPSW->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	SXMaterialEditor::pTrackBarUDPSW->setPos(tmpval * 100);
	SML_MtlSetUserDataPS(SXMaterialEditor::idMat, 3, tmpval);
	return 0;
}