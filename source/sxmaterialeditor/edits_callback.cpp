
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

	material_editor::pEditSkyBox->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	//MessageBox(0, "it is not done yet :)", 0, 0);
	if (SGCore_LoadTexFileExists(szStr))
		SGCore_SkyBoxLoadTex(szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SGCore_SkyBoxGetActiveTex(szStr2);
		material_editor::pEditSkyBox->setText(szStr2);
		TexProcInputString(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditLigthColorR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[32];
	szStr[0] = 0;

	material_editor::pEditLigthColorR->getText(szStr, 32);

	ID idGlobalLight = SLight_GetGlobal();

	if (idGlobalLight > -1)
	{
		float3 vColor;
		SLight_GetColor(idGlobalLight, &vColor);
		sscanf(szStr, "%f", &vColor.x);
		SLight_SetColor(idGlobalLight, &vColor);
		material_editor::pStaticLigthColor->setColorBrush(RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0)));
	}

	return 0;
}

LRESULT SXMaterialEditor_EditLigthColorG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[32];
	szStr[0] = 0;

	material_editor::pEditLigthColorG->getText(szStr, 32);

	ID idGlobalLight = SLight_GetGlobal();

	if (idGlobalLight > -1)
	{
		float3 vColor;
		SLight_GetColor(idGlobalLight, &vColor);
		sscanf(szStr, "%f", &vColor.y);
		SLight_SetColor(idGlobalLight, &vColor);
		material_editor::pStaticLigthColor->setColorBrush(RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0)));
	}

	return 0;
}

LRESULT SXMaterialEditor_EditLigthColorB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[32];
	szStr[0] = 0;

	material_editor::pEditLigthColorB->getText(szStr, 32);

	ID idGlobalLight = SLight_GetGlobal();

	if (idGlobalLight > -1)
	{
		float3 vColor;
		SLight_GetColor(idGlobalLight, &vColor);
		sscanf(szStr, "%f", &vColor.z);
		SLight_SetColor(idGlobalLight, &vColor);
		material_editor::pStaticLigthColor->setColorBrush(RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0)));
	}

	return 0;
}


LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditTex->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);
	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetTexture(material_editor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetTexture(material_editor::idMat, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditTex->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_SHADER_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditVS->getText(szStr, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(szStr))
		SMtrl_MtlSetVS(material_editor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetVS(material_editor::idMat, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditVS->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_SHADER_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditPS->getText(szStr, SXGC_SHADER_MAX_SIZE_DIRNAME);

	if (SGCore_ShaderFileExists(szStr))
		SMtrl_MtlSetPS(material_editor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetPS(material_editor::idMat, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditPS->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditHitChance_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	material_editor::pEditHitChance->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	material_editor::pTrackBarHitChance->setPos(fValue * 100);
	SMtrl_MtlSetHitChance(material_editor::idMat, fValue*0.01f);
	material_editor::pEditHitChance->setText(String(fValue).c_str());

	return 0;
}

LRESULT SXMaterialEditor_EditDurability_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	material_editor::pEditDurability->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SMtrl_MtlSetDurability(material_editor::idMat, fValue);
	material_editor::pEditDurability->setText(String(fValue).c_str());

	return 0;
}

LRESULT SXMaterialEditor_EditDensity_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float fValue = 0;

	material_editor::pEditDensity->getText(szStr, 256);

	sscanf(szStr, "%f", &fValue);

	SMtrl_MtlSetDensity(material_editor::idMat, fValue);
	material_editor::pEditDensity->setText(String(fValue).c_str());

	return 0;
}


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	material_editor::pEditTexLighting->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetTextureLighting(material_editor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetTextureLighting(material_editor::idMat, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditTexLighting->setText(szStr);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	material_editor::pEditMask->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetMaskTex(material_editor::idMat, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetMaskTex(material_editor::idMat, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditMask->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;
	
	material_editor::pEditMR->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetMRTex(material_editor::idMat, 0, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetMRTex(material_editor::idMat, 0, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditMR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditMG->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetMRTex(material_editor::idMat, 1, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetMRTex(material_editor::idMat, 1, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditMG->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditMB->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetMRTex(material_editor::idMat, 2, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetMRTex(material_editor::idMat, 2, szStr2);
		TexProcInputString(szStr);
		
		material_editor::pEditMB->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditMA->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetMRTex(material_editor::idMat, 3, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetMRTex(material_editor::idMat, 3, szStr);
		TexProcInputString(szStr);
		
		material_editor::pEditMA->setText(szStr);
	}

	return 0;
}


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditDR->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetDTex(material_editor::idMat, 0, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetDTex(material_editor::idMat, 0, szStr);
		TexProcInputString(szStr);
		
		material_editor::pEditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditDG->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetDTex(material_editor::idMat, 1, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetDTex(material_editor::idMat, 1, szStr);
		TexProcInputString(szStr);
		
		material_editor::pEditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditDB->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetDTex(material_editor::idMat, 2, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetDTex(material_editor::idMat, 2, szStr);
		TexProcInputString(szStr);
		
		material_editor::pEditDR->setText(szStr);
	}

	return 0;
}

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	szStr[0] = 0;

	material_editor::pEditDA->getText(szStr, SXGC_LOADTEX_MAX_SIZE_DIRNAME);

	if (SGCore_LoadTexFileExists(szStr))
		SMtrl_MtlSetDTex(material_editor::idMat, 3, szStr);
	else
	{
		char szStr2[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
		szStr2[0] = 0;
		SMtrl_MtlGetDTex(material_editor::idMat, 3, szStr);
		TexProcInputString(szStr);
		
		material_editor::pEditDR->setText(szStr);
	}

	return 0;
}

//##########################################################################

LRESULT SXMaterialEditor_EditRoughness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditRoughness->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarRoughness->setPos(tmpval*100);
	SMtrl_MtlSetRoughness(material_editor::idMat, tmpval);

	return 0;
}

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditF0->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarF0->setPos(tmpval * 100);
	SMtrl_MtlSetF0(material_editor::idMat, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditThickness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditThickness->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarThickness->setPos(tmpval * 100);
	SMtrl_MtlSetThickness(material_editor::idMat, tmpval);
	return 0;
}



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDVSX->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDVSX->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataVS(material_editor::idMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDVSY->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDVSY->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataVS(material_editor::idMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDVSZ->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDVSZ->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataVS(material_editor::idMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDVSW->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDVSW->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataVS(material_editor::idMat, 3, tmpval);
	return 0;
}


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDPSX->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDPSX->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataPS(material_editor::idMat, 0, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDPSY->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDPSY->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataPS(material_editor::idMat, 1, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDPSZ->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDPSZ->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataPS(material_editor::idMat, 2, tmpval);
	return 0;
}

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szStr[256];
	szStr[0] = 0;
	float tmpval = 0;

	material_editor::pEditUDPSW->getText(szStr, 256);

	sscanf(szStr, "%f", &tmpval);

	material_editor::pTrackBarUDPSW->setPos(tmpval * 100);
	SMtrl_MtlSetUserDataPS(material_editor::idMat, 3, tmpval);
	return 0;
}