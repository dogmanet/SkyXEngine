
#ifndef __CALLBACK_TABS_H
#define __CALLBACK_TABS_H

#include <common/string.h>

#include <sxguiwinapi/sxgui.h>
#include <particles/sxparticles.h>
#include <level/sxlevel.h>
#include <render/sxrender.h>

#include "particles_editor.h"

LRESULT SXParticlesEditor_ButtonBase_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonTextureAnimTex_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonSpawn_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonBound_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonCharacters_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SXParticlesEditor_ButtonVelocityAcceleration_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif