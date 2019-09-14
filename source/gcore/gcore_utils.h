
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
#include <gcore/gcore_data.h>

//##########################################################################

extern IGXDevice *g_pDevice;
extern HMODULE m_hLibGXAPI;
extern Array<DEVMODE> g_aModes;
extern IGXRenderBuffer *g_pScreenTextureRB;
extern IGXBlendState *g_pToneMappingBS;
extern IGXSamplerState *g_pSamplerFilterPoint;
extern IGXSamplerState *g_pSamplerFilterLinear;

//##########################################################################

//! инициализация устройства
void InitDevice(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed);

//! инициализация отладочной инфы
void InitFPStext();

//! инициализация полноэкранного квадрата
void InitFullScreenQuad();

//! инициализация массива режимов монитора
void InitArrModes();

//! инициализация рендер таргетов для 
void InitRT4Gbuffer();

//! загрузка шейдеров
void LoadShaders();

void InitToneMappingStates();

//! заполнение массива для tone mapping
void ToneMappingGetArrDownScale4x4(int iWidth, int iHeight, float2 aDS[]);

#endif