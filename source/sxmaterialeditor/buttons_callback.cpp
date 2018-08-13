
#include "buttons_callback.h"

LRESULT SXMaterialEditor_ButtonSkyBox_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szName[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	
	if (gui_func::dialogs::SelectFileOwn(szName, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		SXMaterialEditor::pEditSkyBox->setText(szName);

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

	ID idGlobalLight = SML_LigthsGetGlobal();
	float3 vColor;
	SML_LigthsGetColor(idGlobalLight, &vColor);

	oChooseColor.rgbResult = RGB(DWORD(vColor.x * 255.0), DWORD(vColor.y * 255.0), DWORD(vColor.z * 255.0));

	if (ChooseColor(&oChooseColor))
	{
		vColor.x = float(GetRValue(oChooseColor.rgbResult)) / 255.0;
		vColor.y = float(GetGValue(oChooseColor.rgbResult)) / 255.0;
		vColor.z = float(GetBValue(oChooseColor.rgbResult)) / 255.0;
		SML_LigthsSetColor(idGlobalLight, &vColor);
		SXMaterialEditor::pStaticLigthColor->setColorBrush(oChooseColor.rgbResult);

		SXMaterialEditor::pEditLigthColorR->setText(String(vColor.x).c_str());
		SXMaterialEditor::pEditLigthColorG->setText(String(vColor.y).c_str());
		SXMaterialEditor::pEditLigthColorB->setText(String(vColor.z).c_str());
	}

	return 0;
}

LRESULT SXMaterialEditor_ButtonTex_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditTex->setText(tmpname);
		
		SML_MtlSetTexture(SXMaterialEditor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonVS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_FILTER_VS);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), "vs", "Select vertex shader", true, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SXMaterialEditor::pJobWindow->getHWND()))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditVS->setText(tmpname);

		SML_MtlSetVS(SXMaterialEditor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonPS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_FILTER_PS);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), "ps", "Select pixel shader", true, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SXMaterialEditor::pJobWindow->getHWND()))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditPS->setText(tmpname);

		SML_MtlSetPS(SXMaterialEditor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonTexLighting_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditTexLighting->setText(tmpname);

		SML_MtlSetTextureLighting(SXMaterialEditor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditMask->setText(tmpname);

		SML_MtlSetMaskTex(SXMaterialEditor::idMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditMR->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::idMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditMG->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::idMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditMB->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::idMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditMA->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::idMat, 3, tmpname);
	}
	return 0;
}


LRESULT SXMaterialEditor_ButtonDR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditDR->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::idMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditDG->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::idMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditDB->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::idMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::pJobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::pEditDA->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::idMat, 3, tmpname);
	}
	return 0;
}