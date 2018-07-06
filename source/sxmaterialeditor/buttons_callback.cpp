
#include "buttons_callback.h"

LRESULT SXMaterialEditor_ButtonSkyBox_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char szName[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	
	if (gui_func::dialogs::SelectFileOwn(szName, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		SXMaterialEditor::EditSkyBox->setText(szName);

		SGCore_SkyBoxLoadTex(szName);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonRotAngle0_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SRender_SimModelSetRotationY(0);
	return 0;
}

LRESULT SXMaterialEditor_ButtonTex_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditTex->setText(tmpname);
		
		SML_MtlSetTexture(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonVS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_FILTER_VS);

	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), "vs", "Select vertex shader", true, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SXMaterialEditor::JobWindow->getHWND()))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditVS->setText(tmpname);

		SML_MtlSetVS(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonPS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), FILE_FILTER_PS);
	
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), "ps", "Select pixel shader", true, Core_RStringGet(G_RI_STRING_PATH_GS_SHADERS), SXMaterialEditor::JobWindow->getHWND()))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditPS->setText(tmpname);

		SML_MtlSetPS(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonTexLighting_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditTexLighting->setText(tmpname);

		SML_MtlSetTextureLighting(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMask->setText(tmpname);

		SML_MtlSetMaskTex(SXMaterialEditor::IDMat, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMR->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMG->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMB->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonMA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditMA->setText(tmpname);

		SML_MtlSetMRTex(SXMaterialEditor::IDMat, 3, tmpname);
	}
	return 0;
}


LRESULT SXMaterialEditor_ButtonDR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDR->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 0, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDG->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 1, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDB->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 2, tmpname);
	}
	return 0;
}

LRESULT SXMaterialEditor_ButtonDA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char tmppath[1024];
	tmppath[0] = 0;
	char tmpname[1024];
	//gui_func::dialogs::SelectFile(SXGUI_DIALOG_FILE_OPEN, tmppath, 0, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), FILE_FILTER_TEXTURE);
	if (gui_func::dialogs::SelectFileOwn(tmpname, tmppath, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), "dds", "Select texture", true, Core_RStringGet(G_RI_STRING_PATH_GS_TEXTURES), SXMaterialEditor::JobWindow->getHWND(), SkyXEngine_EditorHandlerGetPreviewData, SkyXEngine_EditorHandlerGetTextureInfo))
	{
		//StrCutName(tmppath, tmpname);
		SXMaterialEditor::EditDA->setText(tmpname);

		SML_MtlSetDTex(SXMaterialEditor::IDMat, 3, tmpname);
	}
	return 0;
}