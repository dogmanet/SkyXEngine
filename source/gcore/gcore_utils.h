
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __UTILS_H
#define __UTILS_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <common/array.h>
#include <common/SXMath.h>

#include <gcore/sxgcore.h>
#include <gcore/gcore_data.h>

//##########################################################################

extern IGXContext *g_pDXDevice;
extern HMODULE m_hLibGXAPI;
extern Array<DEVMODE> g_aModes;
extern IMesh *g_pScreenTexture;

//##########################################################################

//! инициализаци€ устройства
void InitDevice(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed, DWORD dwFlags);

//! инициализаци€ отладочной инфы
void InitFPStext();

//! инициализаци€ полноэкранного квадрата
void InitFullScreenQuad();

//! инициализаци€ массива режимов монитора
void InitArrModes();

//! инициализаци€ рендер таргетов дл€ 
void InitRT4Gbuffer();

//! загрузка шейдеров
void LoadShaders();

//! заполнение массива дл€ tone mapping
void ToneMappingGetArrDownScale4x4(int iWidth, int iHeight, float2 aDS[]);

//! расчет tone mapping
void ToneMappingCom(DWORD timeDelta, float fFactorAdapted);

#endif