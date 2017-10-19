
#ifndef __CALLBACK_LIST_H
#define __CALLBACK_LIST_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <particles/sxparticles.h>
#include <level/sxlevel.h>
#include <render/sxrender.h>

#include "particles_editor.h"


LRESULT SXParticlesEditor_ListBoxEffects_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ListBoxEmitters_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT SXParticlesEditor_ButtonEffectsCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonEffectsDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonEmittersCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonEmittersBasis_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonEmittersDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

///

LRESULT SXParticlesEditor_ButtonEffCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

///////////

LRESULT SXParticlesEditor_ButtonEmitterCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif