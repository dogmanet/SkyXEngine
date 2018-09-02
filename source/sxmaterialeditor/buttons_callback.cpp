
#include "buttons_callback.h"

LRESULT SXMaterialEditor_ButtonSkyBox_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szName[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	
	if (gui_func::dialogs::SelectFileOwn(szName, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		material_editor::pEditSkyBox->setText(szName);

		SGCore_SkyBoxLoadTex(szName);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonRotAngle0_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SRender_SimModelSetRotationY(0);
	return 0;
}

LRESULT SXMaterialEditor_ButtonLigthColor_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static COLORREF aCustClr[16];
	CHOOSECOLOR oChooseColor;
	ZeroMemory(&oChooseColor, sizeof(CHOOSECOLOR));
	oChooseColor.hwndOwner = hwnd;
	oChooseColor.lStructSize = sizeof(CHOOSECOLOR);
	oChooseColor.lpCustColors = aCustClr;
	oChooseColor.Flags = CC_FULLOPEN | CC_RGBINIT;

	ID idGlobalLight = SLight_GetGlobal();
	float3 vColor;
	SLight_GetColor(idGlobalLight, &vColor);

	oChooseColor.rgbResult = RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0));

	if (ChooseColor(&oChooseColor))
	{
		vColor.x = float(GetRValue(oChooseColor.rgbResult)) / 255.0;
		vColor.y = float(GetGValue(oChooseColor.rgbResult)) / 255.0;
		vColor.z = float(GetBValue(oChooseColor.rgbResult)) / 255.0;
		SLight_SetColor(idGlobalLight, &vColor);
		material_editor::pStaticLigthColor->setColorBrush(oChooseColor.rgbResult);

		material_editor::pEditLigthColorR->setText(String(vColor.x).c_str());
		material_editor::pEditLigthColorG->setText(String(vColor.y).c_str());
		material_editor::pEditLigthColorB->setText(String(vColor.z).c_str());
	}

	return 0;
}

LRESULT SXMaterialEditor_ButtonTex_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditTex->setText(tmpname);
		
		SMtrl_MtlSetTexture(material_editor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonVS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_FILTER_VS);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), "vs", "Select vertex shader", true, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), material_editor::pJobWindow->getHWND()))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditVS->setText(tmpname);

		SMtrl_MtlSetVS(material_editor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonPS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_FILTER_PS);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), "ps", "Select pixel shader", true, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), material_editor::pJobWindow->getHWND()))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditPS->setText(tmpname);

		SMtrl_MtlSetPS(material_editor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonTexLighting_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditTexLighting->setText(tmpname);

		SMtrl_MtlSetTextureLighting(material_editor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditMask->setText(tmpname);

		SMtrl_MtlSetMaskTex(material_editor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditMR->setText(tmpname);

		SMtrl_MtlSetMRTex(material_editor::idMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditMG->setText(tmpname);

		SMtrl_MtlSetMRTex(material_editor::idMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditMB->setText(tmpname);

		SMtrl_MtlSetMRTex(material_editor::idMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditMA->setText(tmpname);

		SMtrl_MtlSetMRTex(material_editor::idMat, 3, tmpname);
	}
	return 0;
}


LRESULT SXMaterialEditor_ButtonDR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditDR->setText(tmpname);

		SMtrl_MtlSetDTex(material_editor::idMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditDG->setText(tmpname);

		SMtrl_MtlSetDTex(material_editor::idMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditDB->setText(tmpname);

		SMtrl_MtlSetDTex(material_editor::idMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), material_editor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		material_editor::pEditDA->setText(tmpname);

		SMtrl_MtlSetDTex(material_editor::idMat, 3, tmpname);
	}
	return 0;
}