
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __UTILS_H
#define __UTILS_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <common/array.h>
#include <common/Math.h>

#include <gcore/sxgcore.h>

//##########################################################################

extern IGXDevice *g_pDevice;
extern HMODULE m_hLibGXAPI;
extern Array<DEVMODE> g_aModes;
extern IGXRenderBuffer *g_pScreenTextureRB;

//##########################################################################

//! инициализация устройства
void InitDevice(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed);

//! инициализация полноэкранного квадрата
void InitFullScreenQuad();

//! инициализация массива режимов монитора
void InitArrModes();

#endif
