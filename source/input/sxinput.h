
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxinput - системы ввода информации с устройств
*/

/*! \defgroup sxinput sxinput - система ввода информации с устройств
 \warning Замечена ошибочная идентификация нажатия клавиш клавиатуры и кнопок мыши, когда неактивно окно, для которого установлен sxinput
@{
*/

#ifndef __SXINPUT_H
#define __SXINPUT_H

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <core/sxcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#include <xcommon/input/XKeyCodes.h>

//#############################################################################

typedef unsigned char InputCode;	//!< тип "код клавиши/кнопки"

//! Структура входящего сообщения для системы ввода
typedef struct IMSG_s
{
	UINT        message;
	WPARAM      wParam;
	LPARAM      lParam;
} IMSG;

//! виды обытий (клавиатура, мышь)
enum InputEvents
{
	iv_dissable	= 0,	//!< неактивно

	//клавиши клавиатуры
	iv_k_first	= 1,	//!< клавиша нажата впервые
	iv_k_down	= 2,	//!< клавиша зажата
	iv_k_up		= 3,	//!< клавиша отжата
	iv_k_dbl    = 4 	//!< двойной клик кнопкой
};

//! стрктура для шаблона сообщений
struct InMess
{
	InMess::InMess() = default;
	InMess::InMess(InputCode sect, InputEvents code)
	{
		Section = sect; Code = code;
	}

	InputCode Section = 0;		//!< секция кнопка/клавиша
	InputEvents Code = iv_dissable;		//!< код сообщения из #InputEvents
};

//#############################################################################

#ifdef SX_EXE
//! коды букв и цифр распределенные по массивам
namespace InputSymbol
{
	/*! \name Массивы букв и кодов для них
	@{*/
	//! массив с кодами букв
	//InputCode KeyLetters[27]		= { SIK_A, SIK_B, SIK_C, SIK_D, SIK_E, SIK_F, SIK_G, SIK_H, SIK_I, SIK_J, SIK_K, SIK_L, SIK_M, SIK_N, SIK_O, SIK_P, SIK_Q, SIK_R, SIK_S, SIK_T, SIK_U, SIK_V, SIK_W, SIK_X, SIK_Y, SIK_Z, SIK_SPACE };
	//! массив с заглавными буквами
	//char BigLetters[27]				= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
	//!массив с прописными буквами
	//char SmallLetters[27]			= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};
	//!@}

	/*! \name Массивы цифр и коды для них, основнйо и цифровой клавиатуры
	@{*/
	//InputCode KeyNumbers[10]		= { SIK_1, SIK_2, SIK_3, SIK_4, SIK_5, SIK_6, SIK_7, SIK_8, SIK_9, SIK_0 };
	//InputCode KeyNumbers2[10]		= { SIK_NUMPAD1, SIK_NUMPAD2, SIK_NUMPAD3, SIK_NUMPAD4, SIK_NUMPAD5, SIK_NUMPAD6, SIK_NUMPAD7, SIK_NUMPAD8, SIK_NUMPAD9, SIK_NUMPAD0 };
	//char NumbersStr[10]				= {'1','2','3','4','5','6','7','8','9','0'};
	//!@}
};
#endif

//#############################################################################

SX_LIB_API long SSInput_0GetVersion(); //!< возвращает версию ядра
SX_LIB_API void SSInput_Dbg_Set(report_func rf); //!< возвращает версию ядра
SX_LIB_API void SSInput_0Create(const char* name,HWND hwnd,bool is_unic=true);

SX_LIB_API void SSInput_AddMsg(const IMSG & msg); //!< добавление события ввода
SX_LIB_API void SSInput_Update(); //!< обновление состояния устройств

SX_LIB_API bool SSInput_GetKeyState(InputCode Key);			//!< нажата ли клавиша под номером Key

SX_LIB_API bool SSInput_IsOtherButtonOn(InputCode Button);	//!< нажаты ли другие кнопки мыши, Button - для которой отслеживаем единственное нажатие

SX_LIB_API InputEvents SSInput_GetKeyEvents(InputCode Key);			//!< получить событие посылаемое клавишей на клавиатуре

SX_LIB_API long SSInput_GetScroll();						//!<получить скролл, -120 - назад, 120 - вперед (120 - 1 проход)
SX_LIB_API bool SSInput_GetMouseDouble(InputCode Button);	//!< был ли сделан кнопкой мыши Button двойной клик


SX_LIB_API bool SSInput_GetExeEventsS(InMess *Event); //!< было ли совершенно данное событие Event
SX_LIB_API bool SSInput_GetExeEvents(InputCode sect, InputEvents code); //!< было ли совершенно данное событие

SX_LIB_API bool SSInput_GetActiveKeyOrButton(); //!< нажата ли хоть какая-то кнопка клавиатуры либо клавиша мыши
SX_LIB_API bool SSInput_GetActiveButton();		//!< нажата ли хоть какая-то клавиша мыши
SX_LIB_API bool SSInput_GetActiveKey();			//!< нажата ли хоть какая-то кнопка клавиатуры

SX_LIB_API void SSInput_GetMouseDelta(int * x, int * y);//!< получает изменение координат мыши

SX_LIB_API void SSInput_SetEnable(bool bEnable);//!< Включает/отключает обработку событий

SX_LIB_API int SSInput_GetKeymapSize();//!< Количество клавиш
SX_LIB_API void SSInput_GetBindEntry(int n, const char **pszKey, const char **pszCmd);//!< строка таблицы биндов
SX_LIB_API void SSInput_OnNextKeyPress(void(*pfnCallback)(const char *szKey));//!< Вызвать функцию при следующем нажатии клавиши

#endif

//!@} sxinput
