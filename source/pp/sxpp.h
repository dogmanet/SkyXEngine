
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxpp - библиотеки постпроцесса
*/

/*! \defgroup sxpp sxpp - библиотека постпроцесса
@{
*/
#ifndef __SXPP_H
#define __SXPP_H

#if 1

#else
#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif
#endif
//##########################################################################

/*! \name Значения цветов для создания эффекта sepia
@{*/

#define PP_SEPIA_LIGHT_COLOR float3_t(1.0,0.9,0.5)
#define PP_SEPIA_DARK_COLOR float3_t(0.2,0.05,0.0)
#define PP_SEPIA_WEIGHTS_COLOR float3_t(0.27, 0.67, 0.06)

//!@}

//**************************************************************************

/*! радиус солнца в пикселях для lens flare */
#define PP_SUN_RADIUS 30 

/*! максимальный угол между взглядом наблюдателя и позицией источнкиа света при котором еще видно солнце, больше этого солнце уже не видно */
#define PP_MAX_ANGLE_VISIBLE_SUN 1.57 

//##########################################################################

/*! \name Базовые функции библиотеки
@{*/
#if 0
//! возвращает версию подсистемы
SX_LIB_API long SPP_0GetVersion();				

//! установка функции обработки сообщений
SX_LIB_API void SPP_Dbg_Set(report_func fnFunc);	

//! инициализация подсистемы
SX_LIB_API void SPP_0Create(
	const char *szName,		//!< имя подсистемы
	bool isUnic				//!< должно ли быть имя подсистемы уникальным
	);

SX_LIB_API void SPP_OnLostDevice();

//! вызывать при попытке сброса устройства
SX_LIB_API void SPP_OnDeviceReset();

//! уничтожение подсистемы
SX_LIB_API void SPP_0Kill();	

//!@}

//**************************************************************************

/*! \name Установка основных render targets
 \warning Глубина должна быть линейная, от 0 до 1, где 0 это возле камеры, а 1 это максимальная удаленность от камеры. 
Причем линейность в пространстве камеры. 
Однако допускается и такая глубина: \n 
depth = (z + near)/far \n
где near и far ближняя и дальняя плоскости отсечения соответственно (к примеру 0.25 и 400), 
z - z компонента трехмерного вектора отрансформированного wordview projection матрицей и интерпалированного в пиксельный шейдер
@{*/

//! входное изображение которое будет модифицироваться
SX_LIB_API void SPP_RTSetInput(ID idRT);	

//! дополнительное изображение для внутренних нужд
SX_LIB_API void SPP_RTSetOutput(ID idRT);


//! глубина (линейная 0-1), абсолютно непрозрачные пиксели
SX_LIB_API void SPP_RTSetDepth0(ID idRT);

/*! глубина (линейная 0-1), непрозрачные и прозрачные пиксели, однако прозрачные (если есть) будут перекрывать непрозрачные,
и в этом случае их глубина будет 1 то есть максимальной (для идентификации)*/
SX_LIB_API void SPP_RTSetDepth1(ID idRT);

//! нормали (в том числе и микрорейльеф)
SX_LIB_API void SPP_RTSetNormal(ID idRT);

//!@}

//**************************************************************************

/*! \name Переключение очереди и получением текущих rt
@{*/

//! поулчить текущий rt для рендера в него
SX_LIB_API ID SPP_RTGetCurrRender();	

//! получить текущий rt для отправки в шейдер
SX_LIB_API ID SPP_RTGetCurrSend();		

//! переключить очередь между ке в который рисуем и который отправляем
SX_LIB_API void SPP_RTIncr();			

//! обнулить очередь rt, то есть после вызова rt рендера станет input, а дополнительной output
SX_LIB_API void SPP_RTNull();			

//!@}

//**************************************************************************

/*! обновление данных постпроцессорной подсистемы, необходимо вызывать перед началом рендера постпроцесса*/
SX_LIB_API void SPP_Update();

//! обвновление позиции солнца, если pSunPos == 0 тогда солнца в сцене нет
SX_LIB_API void SPP_UpdateSun(const float3 *pSunPos);		

//! сменить текстуру солнца на str
SX_LIB_API void SPP_ChangeTexSun(const char *szTexture);	

//##########################################################################

/*! \defgroup sxpp_eff Эффекты
 \ingroup sxpp
@{*/

/*! рендер солнца, использует depth0, 
используется текстура загруженная функцией #SPP_ChangeTexSun \n 
pSunColor - rgb цвет (0-1), w - на сколько цвет применится к текстуре (0-1)
*/
SX_LIB_API void SPP_RenderSun(const float4_t *pSunColor);

/*! bloom \n 
pParam.x - во сколько раз увеличиваем цвет пикселя при анализе \n
pParam.y - минимальное значение цвета для прохождение теста bright pass \n
pParam.z - во сколько раз увеличиваем цвет пикселя если он прошел тест \n
примерные значения: 1, 0.8, 1
*/
SX_LIB_API void SPP_RenderBloom(const float3_t *pParam);

/*! lens flare, эффект восприятия яркого света линзами, использует depth0 \n 
pParam.x - дисперсия (разброс) \n
pParam.y - ширина свечения \n
pParam.z - множитель интенсивности \n
pSunColor - цвет света солнца (сила) в xyz, w - сила света
useBloom - использовать ли в дополнение данные от bloom прохода, если true то этот эффект должен идти поле bloom \n
примерные данные для 8 (NumSamples) выборок: 0.25f, 0.3f, 0.9f, use_bloom - на любителя*/
SX_LIB_API void SPP_RenderLensFlare(const float3_t *pParam, const float4_t *pSunColor, bool useBloom);

/*! depth of field, глубина резкости, использует depth0 \n 
все компоненты param указываются в метрах \n 
pParam.x - откуда начинается нормальное видиние \n
pParam.y - где оно заканчивается \n
pParam.z - постепенный переход на ближней плоскости \n
pParam.w - постепенный переход на дальней плоскости \n
fSkyBlur - на сколько % размывать небо (0-1) \n
для примеры: param:0, 200, 0, 50, sky_blur: 0*/
SX_LIB_API void SPP_RenderDOF(const float4_t *pParam, float fSkyBlur);

/*! linear fog, линейный туман, использует depth0 \n 
pColor - цвет тумана (0-1) \n
fDensity - интенсивность тумана \n
примерные значения: 0.8*/
SX_LIB_API void SPP_RenderFogLinear(const float3_t *pColor, float fDensity);

/*! space screen ambient occulusion, глобальное освещение (точнее затенение) в пространстве экрана, использует depth1 \n 
pParam.x - радиус \n
pParam.y - минимально допустимая разница в глубине между пикселя для просчета затенения \n
pParam.z - освещенность пикселя по дефолту (если разница меньше минимальной) \n
pParam.w - допустимая разница для сравнения \n
примерные значения: 0.3f, 0.1f, 0.8f, 0.3f / GData::NearFar.y
iQuality - качество (количество выборок)
iQuality == 2 - 16
iQuality == 3 - 24
иначе 8
*/
SX_LIB_API void SPP_RenderSSAO(const float4_t *pParam, int iQuality);

/*! motion blur, размытие в движении, использует depth0 \n 
fCoef - коэфициент размытия, 0 - 1 \n
timeDelta - время рендера текущего кадра в млсек*/
SX_LIB_API void SPP_RenderMotionBlur(float fCoef, DWORD timeDelta);

//##########################################################################

/*! \name Стандартные эффекты
@{*/

//! черно-белое изображение, coef - коэфициент перехода от цветного к черное-белому (0-1)
SX_LIB_API void SPP_RenderWhiteBlack(float fCoef);	

//! эффект сепия, coef - коэфициент перехода от цветного к сепии (0-1)
SX_LIB_API void SPP_RenderSepia(float fCoef);

/*! коррекция изображения \n 
pParam.x - contrast/контраст \n
pParam.y - brightness/яркость \n
pParam.z - gamma/гамма-коррекция \n
нейтральные параметры: 1,1,0*/
SX_LIB_API void SPP_RenderCBG(const float3_t *pParam);

//!@}

//##########################################################################

/*! \name Anti Aliasing
@{*/

/*! nfaa (Normal Filter Anti Aliasing) \n 
pParam.x - strength, множитель нормали, усиляет влияние нормали \n
pParam.y - scale, охватываемый масштаб \n
pParam.z - notmal map, вывести сформированные нормали \n
рекомендуемые параметры: 1, 2, 0*/
SX_LIB_API void SPP_RenderNFAA(const float3_t *pParam);

//! dlaa (Directionally Localized Anti Aliasing)
SX_LIB_API void SPP_RenderDLAA();	
#endif
//!@}

//!@} sxpp_eff

#endif

//!@} sxpp