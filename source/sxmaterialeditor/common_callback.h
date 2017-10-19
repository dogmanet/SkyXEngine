
#ifndef __COMMON_CALLBACK_H
#define __COMMON_CALLBACK_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <mtllight/sxmtllight.h>
#include <render/sxrender.h>
#include <sxmaterialeditor/resource.h>

#include "material_editor.h"

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_ToolBar_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

///////

LRESULT SXMaterialEditor_JobWindow_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif