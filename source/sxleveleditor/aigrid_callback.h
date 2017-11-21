
#ifndef __AIGRID_CALLBACK_H
#define __AIGRID_CALLBACK_H

#include "level_editor.h"

LRESULT SXLevelEditor_EditAIBBDimensions_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_EditAIBBPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIBBFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIQuadsDelSel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIGridGen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIGridClear_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIGPGen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIGPClear_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIGridValidation_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonAIClearAll_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif