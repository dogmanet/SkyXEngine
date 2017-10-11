
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxinput - системы ввода информации с устройств
*/

/*! \defgroup sxinput sxinput - система ввода информации с устройств
 \warning Замечена ошибочная идентификация нажатия клавиш клавиатуры и кнопок мыши, когда неактивно окно, для которого установлен sxinput
@{
*/

#ifndef __sxinput
#define __sxinput

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.h>

#define SX_LIB_API extern "C" __declspec (dllimport)
#include <core\\sxcore.h>

#ifdef SX_DLL
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

/*! \defgroup sxinput_code_keyboard Коды клавиш клавиатуры
 \ingroup sxinput
@{
*/

#define SIK_OFFS 128

/*! \name F1-F15
@{
*/

#define SIK_F1              0x3B
#define SIK_F2              0x3C
#define SIK_F3              0x3D
#define SIK_F4              0x3E
#define SIK_F5              0x3F
#define SIK_F6              0x40
#define SIK_F7              0x41
#define SIK_F8              0x42
#define SIK_F9              0x43
#define SIK_F10             0x44
#define SIK_F11             0x57
#define SIK_F12             0x58

//!@}

/*! \name Цифры основной клавиатуры
@{
*/

#define SIK_1               0x02
#define SIK_2               0x03
#define SIK_3               0x04
#define SIK_4               0x05
#define SIK_5               0x06
#define SIK_6               0x07
#define SIK_7               0x08
#define SIK_8               0x09
#define SIK_9               0x0A
#define SIK_0               0x0B
//!@}

/*! \name Стрелки
@{
*/

#define SIK_LEFT            (SIK_NUMPAD4 + SIK_OFFS)
#define SIK_RIGHT           (SIK_NUMPAD6 + SIK_OFFS)
#define SIK_DOWN            (SIK_NUMPAD2 + SIK_OFFS)
#define SIK_UP              (SIK_NUMPAD8 + SIK_OFFS)

//!@}

/*! \name Цифровая клавиатура
@{
*/

#define SIK_NUMPAD0         0x52	/*!< 0*/
#define SIK_NUMPAD1         0x4F	/*!< 1*/
#define SIK_NUMPAD2         0x50	/*!< 2*/
#define SIK_NUMPAD3         0x51	/*!< 3*/
#define SIK_NUMPAD4         0x4B	/*!< 4*/
#define SIK_NUMPAD5         0x4C	/*!< 5*/
#define SIK_NUMPAD6         0x4D	/*!< 6*/
#define SIK_NUMPAD7         0x47	/*!< 7*/
#define SIK_NUMPAD8         0x48	/*!< 8*/
#define SIK_NUMPAD9         0x49	/*!< 9*/

#define SIK_NUMLOCK         (0x45 + SIK_OFFS)
#define SIK_DIVIDE          (SIK_SLASH + SIK_OFFS)    /*!< / */
#define SIK_MULTIPLY        0x37    /*!< \* */
#define SIK_SUBTRACT        0x4A    /*!< \- */
#define SIK_ADD             0x4E    /*!< \+ */
#define SIK_DECIMAL         0x53    /*!< \. */
#define SIK_NUMPADENTER     (SIK_ENTER + SIK_OFFS)    /*!< Enter */

//!@}

/*! \name A-Z
@{
*/

#define SIK_A               0x1E
#define SIK_B               0x30
#define SIK_C               0x2E
#define SIK_D               0x20
#define SIK_E               0x12
#define SIK_F               0x21
#define SIK_G               0x22
#define SIK_J               0x24
#define SIK_H               0x23
#define SIK_I               0x17
#define SIK_K               0x25
#define SIK_L               0x26
#define SIK_M               0x32
#define SIK_N               0x31
#define SIK_O               0x18
#define SIK_P               0x19
#define SIK_Q               0x10
#define SIK_R               0x13
#define SIK_S               0x1F
#define SIK_T               0x14
#define SIK_U               0x16
#define SIK_V               0x2F
#define SIK_W               0x11
#define SIK_X               0x2D
#define SIK_Y               0x15
#define SIK_Z               0x2C

//!@}

/*! \name Ctrl
@{
*/
#define SIK_CONTROL         0x88
#define SIK_LCONTROL        0x1D
#define SIK_RCONTROL        (SIK_LCONTROL + SIK_OFFS)
//!@}

/*! \name Shift
@{
*/
#define SIK_SHIFT           0x87
#define SIK_LSHIFT          0x2A
#define SIK_RSHIFT          0x36
//!@}

/*! \name Alt
@{
*/
#define SIK_ALT             0x89
#define SIK_LALT            0x38
#define SIK_RALT            (SIK_LALT + SIK_OFFS)
//!@}

/*! \name Windows key
@{
*/
#define SIK_LWIN            (91 + SIK_OFFS)    /*!< left Windows key */
#define SIK_RWIN            (92 + SIK_OFFS)    /*!< right Windows key */
#define SIK_APPS            (93 + SIK_OFFS)
//!@}

#define SIK_ESCAPE          0x01

#define SIK_LBRACKET        0x1A	/*!< { */
#define SIK_RBRACKET        0x1B	/*!< } */

#define SIK_ENTER           0x1C    /*!< Enter на основной */
#define SIK_SPACE           0x39	/*!< пробел */
#define SIK_CAPSLOCK		0x3A	
#define SIK_TAB             0x0F	

#define SIK_GRAVE           0x29    /*!< \` */
#define SIK_MINUS           0x0C	/*!< \- */
#define SIK_EQUALS          0x0D	/*!< = */
#define SIK_BACKSPACE       0x0E

#define SIK_SEMICOLON       0x27	/*!< ; */
#define SIK_APOSTROPHE      0x28	/*!< ' */

#define SIK_BACKSLASH       0x2B	/*!< \\ */
#define SIK_SLASH           0x35    /*!< / на основной */
#define SIK_COMMA           0x33    /*!< \, на основной */
#define SIK_PERIOD          0x34    /*!< \. на основной */
#define SIK_SCROLLLOCK		0x46    /*!< scroll lock */

#define SIK_HOME            (SIK_NUMPAD7 + SIK_OFFS)    /*!< Home */
#define SIK_END             (SIK_NUMPAD1 + SIK_OFFS)    /*!< End */

#define SIK_PGUP			(SIK_NUMPAD9 + SIK_OFFS)    /*!< Page Up */
#define SIK_PGDOWN			(SIK_NUMPAD3 + SIK_OFFS)    /*!< Page Down */

#define SIK_INSERT          (SIK_NUMPAD0 + SIK_OFFS)    /*!< Insert */
#define SIK_DELETE          (SIK_DECIMAL + SIK_OFFS)    /*!< Delete */

#define SIK_PAUSE           0x45

//!@} sxinput_code_keyboard

//#############################################################################

/*! \defgroup sxinput_code_mouse Коды кнопок мыши
 \ingroup sxinput
@{
*/
#define SIM_START           SIM_LBUTTON
#define SIM_LBUTTON			(SIK_OFFS + 0)	/*!< левая кнопка */
#define SIM_RBUTTON			(SIK_OFFS + 1)	/*!< правая кнопка */
#define SIM_MBUTTON			(SIK_OFFS + 2)	/*!< средняя кнопка (она же скролл) */
#define SIM_XBUTTON1		(SIK_OFFS + 3)	/*!< дополнительная кнопка 1 */
#define SIM_XBUTTON2		(SIK_OFFS + 4)	/*!< дополнительная кнопка 2 */
#define SIM_END             SIM_XBUTTON2
#define SIM_MWHEELUP		(SIK_OFFS + 5)	/*!< колесо вверх */
#define SIM_MWHEELDOWN		(SIK_OFFS + 6)	/*!< колесо вниз */

//!@} sxinput_code_mouse

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
	InMess::InMess(){}
	InMess::InMess(InputCode sect, InputEvents code)
	{
		Section = sect; Code = code;
	}

	InputCode Section;		//!< секция кнопка/клавиша
	InputEvents Code;		//!< код сообщения из #InputEvents
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

#endif

//!@} sxinput
