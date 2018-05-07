
#ifndef __CALBACK_COMMON_H
#define __CALBACK_COMMON_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <particles/sxparticles.h>
#include <level/sxlevel.h>
#include <render/sxrender.h>
#include <sxparticleseditor/resource.h>
#include "particles_editor.h"
#include "callback_list.h"

#include <skyxengine.h>

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ToolBar1_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonTBReOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonTBSave_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

///////

LRESULT SXParticlesEditor_ButtonTextureSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_GroupBoxData2_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_EditEffName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_Edits_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif