#ifndef __sxinput
#define __sxinput

#include <gdefines.h>

//коды клавиш
//клавиатура
#define SIK_ESCAPE          0x01

//F1-F15
//{
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
//}

//цифры основной клавиатуры
//{
#define SIK_0               0x0B
#define SIK_1               0x02
#define SIK_2               0x03
#define SIK_3               0x04
#define SIK_4               0x05
#define SIK_5               0x06
#define SIK_6               0x07
#define SIK_7               0x08
#define SIK_8               0x09
#define SIK_9               0x0A

#define SIK_TAB             0x0F
#define SIK_MINUS           0x0C
#define SIK_EQUALS          0x0D
#define SIK_BACKSPACE       0x0E
//}

//стрелки
//{
#define SIK_LEFT            0xCB
#define SIK_RIGHT           0xCD
#define SIK_DOWN            0xD0
#define SIK_UP              0xC8
//}

//цифровая клавиатура
//{
#define SIK_NUMPAD0         0x52
#define SIK_NUMPAD1         0x4F
#define SIK_NUMPAD2         0x50
#define SIK_NUMPAD3         0x51
#define SIK_NUMPAD4         0x4B
#define SIK_NUMPAD5         0x4C
#define SIK_NUMPAD6         0x4D
#define SIK_NUMPAD7         0x47
#define SIK_NUMPAD8         0x48
#define SIK_NUMPAD9         0x49

#define SIK_NUMLOCK         0x45
#define SIK_DIVIDE          0xB5    /* / */
#define SIK_MULTIPLY        0x37    /* * */
#define SIK_SUBTRACT        0x4A    /* - */
#define SIK_ADD             0x4E    /* + */
#define SIK_DECIMAL         0x53    /* . */
#define SIK_NUMPADENTER     0x9C    /* Enter */
//}

//A-Z
//{
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
//}

//CTRL
#define SIK_LCONTROL        0x1D
#define SIK_RCONTROL        0x9D

//SHIFT
#define SIK_LSHIFT          0x2A
#define SIK_RSHIFT          0x36

//ALT
#define SIK_LALT           0x38
#define SIK_RALT           0xB8

#define SIK_LBRACKET        0x1A	/* { */
#define SIK_RBRACKET        0x1B	/* } */

#define SIK_RETURN          0x1C    /* Enter на основной */
#define SIK_SPACE           0x39	/* пробел */
#define SIK_CAPSLOCK		0x3A

#define SIK_LWIN            0xDB    /* left Windows key */
#define SIK_RWIN            0xDC    /* right Windows key */

#define SIK_SEMICOLON       0x27	/* ; */
#define SIK_APOSTROPHE      0x28	/* ' */
#define SIK_GRAVE           0x29    /* ` */

#define SIK_BACKSLASH       0x2B	/* \ */
#define SIK_SLASH           0x35    /* / на основной */
#define SIK_PERIOD          0x34    /* . на основной */
#define SIK_SCROLLLOCK		0x46    /* scroll lock */

#define SIK_HOME            0xC7    /* Home */
#define SIK_END             0xCF    /* End */

#define SIK_PGUP			0xC9    /* Page Up */
#define SIK_PGDOWN			0xD1    /* Page Down */

#define SIK_INSERT          0xD2    /* Insert */
#define SIK_DELETE          0xD3    /* Delete */


//мышь
#define SIM_LBUTTON			0
#define SIM_RBUTTON			1
#define SIM_MBUTTON			2
////

#define SX_INPUT_COUNT_EVENTS_FOR_DBL_CLICK		3	/*сколько событий должно произойти для возникновения события двойного клика*/
#define SX_INPUT_PERIOD_RE_PRESSED				200	/*период циклического события, когда кнопку нажали, ставим таймер и периодично выдаем события DOWN*/
#define SX_INPUT_PERIOD_NON_DBL_CLICK			150	/*время в течении которого двойной клик не будет засчитан*/
#define SX_INPUT_PERIOD_NON_DOWN_KEY			50	/*время в течении которого зажатая клавиша будет считаться отпущенной*/
#define SX_INPUT_PERIOD_DBL_CLICK				500	/*максимальное время в течении которого (-PERIOD_NON_DBL_CLICK) может произойти двойной клик*/

typedef unsigned char InputCode;	//тип "код клавиши/кнопки"

//поддерживаемые устройства
enum InputDevice
{
	dev_keyboard,	//клавиатура
	dev_mouse		//мышь
};

//виды обытий (клавиатура, мышь)
enum InputEvents
{
	iv_dissable	= 0,	//неактивно

	//клавиши клавиатуры
	iv_k_first	= 1,	//клавиша нажата впервые
	iv_k_down	= 2,	//клавиша зажата
	iv_k_up		= 3,	//клавиша отжата

	//кнопки мыши
	iv_m_first	= 4,	//кнопка нажата впервые
	iv_m_down	= 5,	//кнопка зажата
	iv_m_up		= 6,	//кнопка отжата
	iv_m_dbl	= 7,	//двойной клик кнопкой

	iv_m_scroll	= 8 	//сроллинг
};

//стрктура для шаблона сообщений
struct InMess
{
	InMess::InMess(){}
	InMess::InMess(InputDevice type, InputCode sect, InputEvents code)
	{
		TypeDevice = type; Section = sect; Code = code;
	}

	InputDevice TypeDevice;	//тип устройства мышь/клавиатура
	InputCode Section;		//секция кнопка/клавиша
	InputEvents Code;		//код сообщения из InputEvents
};

//коды букв и цифр распределенные по массивам
namespace InputSymbol
{
	InputCode KeyLetters[27]		= { SIK_A, SIK_B, SIK_C, SIK_D, SIK_E, SIK_F, SIK_G, SIK_H, SIK_I, SIK_J, SIK_K, SIK_L, SIK_M, SIK_N, SIK_O, SIK_P, SIK_Q, SIK_R, SIK_S, SIK_T, SIK_U, SIK_V, SIK_W, SIK_X, SIK_Y, SIK_Z, SIK_SPACE };
	char BigLetters[27]				= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
	char SmallLetters[27]			= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};

	InputCode KeyNumbers[10]		= { SIK_1, SIK_2, SIK_3, SIK_4, SIK_5, SIK_6, SIK_7, SIK_8, SIK_9, SIK_0 };
	InputCode KeyNumbers2[10]		= { SIK_NUMPAD1, SIK_NUMPAD2, SIK_NUMPAD3, SIK_NUMPAD4, SIK_NUMPAD5, SIK_NUMPAD6, SIK_NUMPAD7, SIK_NUMPAD8, SIK_NUMPAD9, SIK_NUMPAD0 };
	char NumbersStr[10]				= {'1','2','3','4','5','6','7','8','9','0'};
};

SX_LIB_API long SSInput_0GetVersion(); //возвращает версию ядра
SX_LIB_API void SSInput_Dbg_Set(report_func rf); //возвращает версию ядра
SX_LIB_API void SSInput_0Create(const char* name,HWND hwnd,bool is_unic=true);

SX_LIB_API void SSInput_Update(); //обновление состояния устройств

SX_LIB_API bool SSInput_GetKeyState(InputCode Key);			//нажата ли клавиша под номером Key
SX_LIB_API bool SSInput_GetButtonState(InputCode Number);	//нажата ли кнопка мыши под номером Number

SX_LIB_API bool SSInput_IsOtherButtonOn(InputCode Button);	//нажаты ли другие кнопки мыши, Button - для которой отслеживаем единственное нажатие

SX_LIB_API InputEvents SSInput_GetKeyEvents(InputCode Key);			//получить событие посылаемое клавишей на клавиатуре
SX_LIB_API InputEvents SSInput_GetButtonEvent(InputCode Button);	//получить событие посылаемое кнопкой мыши

SX_LIB_API long SSInput_GetScroll();						//получить скролл, -120 - назад, 120 - вперед (120 - 1 проход)
SX_LIB_API bool SSInput_GetMouseDouble(InputCode Button);	//был ли сделан кнопкой мыши Button двойной клик
SX_LIB_API InputCode SSInput_IsMouseClick();				//возвращает код нажатой сейчас кнопки мыши

//было ли совершенно данное событие
SX_LIB_API bool SSInput_GetExeEventsS(InMess *Event); 
SX_LIB_API bool SSInput_GetExeEvents(InputDevice type, InputCode sect, InputEvents code);

SX_LIB_API bool SSInput_GetActiveKeyOrButton(); //нажата ли хоть какая-то кнопка клавиатуры либо клавиша мыши
SX_LIB_API bool SSInput_GetActiveButton();		//нажата ли хоть какая-то клавиша мыши
SX_LIB_API bool SSInput_GetActiveKey();			//нажата ли хоть какая-то кнопка клавиатуры

#endif