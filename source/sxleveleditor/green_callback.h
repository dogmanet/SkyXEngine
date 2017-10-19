
#ifndef __GREEN_CALLBACK_H
#define __GREEN_CALLBACK_H

#include "level_editor.h"

LRESULT SXLevelEditor_EditGreenName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenLod2_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_TrackBarGreenDensity_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenMask_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenNav_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGreenGenerate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif