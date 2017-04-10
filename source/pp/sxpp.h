
#ifndef __sxpostprocess
#define __sxpostprocess

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

//значения цветов для создания эффекта sepia
#define PP_SEPIA_LIGHT_COLOR float3_t(1.0,0.9,0.5)
#define PP_SEPIA_DARK_COLOR float3_t(0.2,0.05,0.0)
#define PP_SEPIA_WEIGHTS_COLOR float3_t(0.27, 0.67, 0.06)

#define PP_SUN_RADIUS 30 //радиус солнца в пикселях для lens flare
#define PP_MAX_ANGLE_VISIBLE_SUN 1.57 //максимальный угол между взглядом наблюдателя и позицией источнкиа света при котором еще видно солнце, больше этого солнце уже не видно

SX_LIB_API long SPP_0GetVersion();				//возвращает версию подсистемы
SX_LIB_API void SPP_Dbg_Set(report_func rf);	//установка функции обработки сообщений

//инициализация подсистемы
SX_LIB_API void SPP_0Create(
	const char* name,			//имя подсистемы
	IDirect3DDevice9* device,	
	float2_t* winsize,			//размеры backbuffer (окна рендера)
	bool is_unic				//должно ли быть имя подсистемы уникальным
	);

SX_LIB_API void SPP_0Kill();	//уничтожение подсистемы

//установка основных render targets
SX_LIB_API void SPP_RTSetInput(ID rt);	//входное изображение которое будет модифицироваться
SX_LIB_API void SPP_RTSetOutput(ID rt);	//дополнительное изображение для внутренних нужд
//!!! вся глубина должна быть линейная, от 0 до 1, где 0 это возле камеры, а 1 это максимальная удаленность от камеры
//!!! причем линейность в пространстве камеры
//!!! однако допускается и такая глубина: depth = (z + near)/far
//где near и far ближняя и дальняя плоскости отсечения соответственно (к примеру 0,25 и 400)
//z - z компонента трехмерного вектора отрансформированного wordview projection матрицей и интерпалированного в пиксельный шейдер
SX_LIB_API void SPP_RTSetDepth0(ID rt);	//глубина (линейная 0-1), абсолютно непрозрачные пиксели
//и непрозрачные и прозрачные пиксели, однако прозрачные 9если есть) будут перекрывать непрозрачные
//и в этом случае их глубина будет 1 (для идентификации)
SX_LIB_API void SPP_RTSetDepth1(ID rt);	//глубина (линейная 0-1)
SX_LIB_API void SPP_RTSetNormal(ID rt);	//нормали (в том числе и микрорейльеф)

//работа с переключением (очереди) и получением текущих rt
SX_LIB_API ID SPP_RTGetCurrRender();	//поулчить текущий rt для рендера в него
SX_LIB_API ID SPP_RTGetCurrSend();		//получить текущий rt для отправки в шейдер
SX_LIB_API void SPP_RTIncr();			//переключить очередь между ке в который рисуем и который отправляем
SX_LIB_API void SPP_RTNull();			//обнулить очередь rt, то есть после вызова rt рендера станет input, а дополнительной output

//обновление данных постпроцессорной подсистемы
//необходимо вызывать перед началом рендера постпроцесса
SX_LIB_API void SPP_Update(
	float3_t* viewpos,	//позиция глаз наблюдателя
	float3_t* viewdir,	//направление взгляда
	float4x4* view,		//матрица вида
	float4x4* proj,		//матрица проекции
	float2_t* winsize,	//текущие размеры окна рендера
	float2_t* nearfar,	//ближняя и дальняя плоскости отсечения
	float projfov		//угол обзора в радианах
	);
SX_LIB_API void SPP_UpdateSun(float3* sunpos);		//обвновление позиции солнца
SX_LIB_API void SPP_ChangeTexSun(const char* str);	//сменить текстуру солнца на str


//ЭФФЕКТЫ

//рендер солнца, использует depth0
//используется текстура загруженная функцией SPP_ChangeTexSun
//sun_color - rgb цвет (0-1), w - на сколько цвет применится к текстуре (0-1)
SX_LIB_API void SPP_RenderSun(float4_t* sun_color);

//bloom
//param.x - во сколько раз увеличиваем цвет пикселя при анализе
//param.y - минимальное значение цвета для прохождение теста bright pass
//param.z - во сколько раз увеличиваем цвет пикселя если он прошел тест
//примерные значения: 1, 0.8, 1
SX_LIB_API void SPP_RenderBloom(float3_t* param);

//lens flare, эффект восприятия яркого света линзами, использует depth0
//param.x - дисперсия (разброс)
//param.y - ширина свечения
//param.z - множитель интенсивности
//use_bloom - использовать ли в дополнение данные от bloom прохода, если true то этот эффект должен идти поле bloom
//примерные данные для 8 (NumSamples) выборок: 0.25f, 0.3f, 0.9f, use_bloom - на любителя
SX_LIB_API void SPP_RenderLensFlare(float3_t* param, bool use_bloom);

//depth of field, глубина резкости, использует depth0
//все компоненты param указываются в метрах
//param.x - откуда начинается нормальное видиние
//param.y - где оно заканчивается
//param.z - постепенный переход на ближней плоскости
//param.w - постепенный переход на дальней плоскости
//sky_blur - на сколько % размывать небо (0-1)
//для примеры: param:0, 200, 0, 50, sky_blur: 0
SX_LIB_API void SPP_RenderDOF(float4_t* param, float sky_blur);

//linear fog, линейный туман, использует depth0
//color - цвет тумана (0-1)
//param.x - интенсивность тумана
//param.y - на сколько небо в тумане
//param.z - минимальное значение тумана
//param.w - максимальное значение тумана
//примерные значения: 0.8, 1, 0.1, 0.9
SX_LIB_API void SPP_RenderFog(float3_t* color, float4_t* param);

//space screen ambient occulusion, глобальное освещение (точнее затенение) в пространстве экрана, использует depth1
//param.x - радиус
//param.y - минимально допустимая разница в глубине между пикселя для просчета затенения
//param.z - освещенность пикселя по дефолту (если разница меньше минимальной)
//param.w - допустимая разница для сравнения
//примерные значения: 0.3f, 0.1f, 0.8f, 0.3f / GData::NearFar.y
SX_LIB_API void SPP_RenderSSAO(float4_t* param);

//motion blur, размытие в движении, использует depth0
//coef - коэфициент размытия, 0 - 1
//timeDelta - время рендера текущего кадра в млсек
SX_LIB_API void SPP_RenderMotionBlur(float coef, DWORD timeDelta);

//стандартные эффекты
SX_LIB_API void SPP_RenderWhiteBlack(float coef);	//четное белое изображение, coef - коэфициент перехода от цветного к черное-белому (0-1)
SX_LIB_API void SPP_RenderSepia(float coef);		//эффект сепия, , coef - коэфициент перехода от цветного к сепии (0-1)
//коррекция изображения
//param.x - contrast/контраст
//param.y - brightness/яркость
//param.z - gamma/гамма-коррекция
//нейтральные параметры: 1,1,0
SX_LIB_API void SPP_RenderCBG(float3_t* param);


//ANTI ALIASING

//nfaa
//param.x - strength, множитель нормали, усиляет влияние нормали
//param.y - scale, охватываемый масштаб
//param.z - notmal map, вывести сформированные нормали
//рекомендуемые параметры: 2, 1, 0
SX_LIB_API void SPP_RenderNFAA(float3_t* param);
SX_LIB_API void SPP_RenderDLAA();	//dlaa

#endif