
#ifndef __EDITS_CALLBACK_H
#define __EDITS_CALLBACK_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <mtllight/sxmtllight.h>
#include <render/sxrender.h>
#include <sxmaterialeditor/resource.h>

#include "material_editor.h"

LRESULT SXMaterialEditor_EditSkyBox_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditTex_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditVS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditPS_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditPenetration_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT SXMaterialEditor_EditTexLighting_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT SXMaterialEditor_EditMask_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditMR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditMG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditMB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditMA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT SXMaterialEditor_EditDR_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditDG_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditDB_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditDA_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/////////////

LRESULT SXMaterialEditor_EditRoughness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditF0_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditThickness_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



LRESULT SXMaterialEditor_EditUDVSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditUDVSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditUDVSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditUDVSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT SXMaterialEditor_EditUDPSX_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditUDPSY_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditUDPSZ_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXMaterialEditor_EditUDPSW_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif