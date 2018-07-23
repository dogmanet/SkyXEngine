
#ifndef __TRACKBARS_CALLBACK_H
#define __TRACKBARS_CALLBACK_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <mtllight/sxmtllight.h>
#include <render/sxrender.h>
#include <sxmaterialeditor/resource.h>

#include "material_editor.h"

LRESULT SXMaterialEditor_TrackBarHitChance_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarRoughness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarThickness_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarF0_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//////////

LRESULT SXMaterialEditor_TrackBarUDVSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarUDVSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarUDVSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarUDVSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

///////

LRESULT SXMaterialEditor_TrackBarUDPSX_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarUDPSY_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarUDPSZ_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_TrackBarUDPSW_MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif