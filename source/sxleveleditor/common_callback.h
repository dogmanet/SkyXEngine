
#ifndef __COMMON_CALLBACK_H
#define __COMMON_CALLBACK_H

#include <common/file_utils.h>
#include "level_editor.h"

LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT MsgEditSize(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



LRESULT SXLevelEditor_RenderWindow_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_RenderWindow_LDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_RenderWindow_LClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_RenderWindow_RClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_RenderWindow_MBUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_RenderWindow_Delete(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGeometryOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGameObjectOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIGridOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ListBoxList_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



LRESULT SXLevelEditor_GroupBox_Notify(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ToolBar1_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_GroupBox_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_GroupBoxList_CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SXLevelEditor_Transform(DWORD timeDelta);

#endif