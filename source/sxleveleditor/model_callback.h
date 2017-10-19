
#ifndef __MODEL_CALLBACK_H
#define __MODEL_CALLBACK_H

#include "level_editor.h"

LRESULT SXLevelEditor_EditGeomName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGeomModel_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGeomLod1_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_ButtonGeomFinish_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_EditTransformPos_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_EditTransformRot_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXLevelEditor_EditTransformScale_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif