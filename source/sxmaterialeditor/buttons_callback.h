
#ifndef __BUTTONS_CALLBACK_H
#define __BUTTONS_CALLBACK_H

#include <common/string.h>
#include <windows.h>
#include <sxguiwinapi/sxgui.h>
#include <mtllight/sxmtllight.h>
#include <render/sxrender.h>
#include <sxmaterialeditor/resource.h>

#include "material_editor.h"

LRESULT SXMaterialEditor_ButtonSkyBox_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonRotAngle0_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonLigthColor_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonTex_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonVS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonPS_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonTexLighting_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonMR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonMG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonMB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonMA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT SXMaterialEditor_ButtonDR_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonDG_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonDB_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ButtonDA_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif