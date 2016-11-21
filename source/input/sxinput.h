#ifndef __sxinput
#define __sxinput

#include <Mmsystem.h>
#include <DInput.h>
#include <gdefines.h>

//DirectInput keyboard scan codes
#define DIK_ESCAPE          0x01
#define DIK_1               0x02
#define DIK_2               0x03
#define DIK_3               0x04
#define DIK_4               0x05
#define DIK_5               0x06
#define DIK_6               0x07
#define DIK_7               0x08
#define DIK_8               0x09
#define DIK_9               0x0A
#define DIK_0               0x0B
#define DIK_MINUS           0x0C    /* - on main keyboard */
#define DIK_EQUALS          0x0D
#define DIK_BACK            0x0E    /* backspace */
#define DIK_TAB             0x0F
#define DIK_Q               0x10
#define DIK_W               0x11
#define DIK_E               0x12
#define DIK_R               0x13
#define DIK_T               0x14
#define DIK_Y               0x15
#define DIK_U               0x16
#define DIK_I               0x17
#define DIK_O               0x18
#define DIK_P               0x19
#define DIK_LBRACKET        0x1A
#define DIK_RBRACKET        0x1B
#define DIK_RETURN          0x1C    /* Enter on main keyboard */
#define DIK_LCONTROL        0x1D
#define DIK_A               0x1E
#define DIK_S               0x1F
#define DIK_D               0x20
#define DIK_F               0x21
#define DIK_G               0x22
#define DIK_H               0x23
#define DIK_J               0x24
#define DIK_K               0x25
#define DIK_L               0x26
#define DIK_SEMICOLON       0x27
#define DIK_APOSTROPHE      0x28
#define DIK_GRAVE           0x29    /* accent grave */
#define DIK_LSHIFT          0x2A
#define DIK_BACKSLASH       0x2B
#define DIK_Z               0x2C
#define DIK_X               0x2D
#define DIK_C               0x2E
#define DIK_V               0x2F
#define DIK_B               0x30
#define DIK_N               0x31
#define DIK_M               0x32
#define DIK_COMMA           0x33
#define DIK_PERIOD          0x34    /* . on main keyboard */
#define DIK_SLASH           0x35    /* / on main keyboard */
#define DIK_RSHIFT          0x36
#define DIK_MULTIPLY        0x37    /* * on numeric keypad */
#define DIK_LMENU           0x38    /* left Alt */
#define DIK_SPACE           0x39
#define DIK_CAPITAL         0x3A
#define DIK_F1              0x3B
#define DIK_F2              0x3C
#define DIK_F3              0x3D
#define DIK_F4              0x3E
#define DIK_F5              0x3F
#define DIK_F6              0x40
#define DIK_F7              0x41
#define DIK_F8              0x42
#define DIK_F9              0x43
#define DIK_F10             0x44
#define DIK_NUMLOCK         0x45
#define DIK_SCROLL          0x46    /* Scroll Lock */
#define DIK_NUMPAD7         0x47
#define DIK_NUMPAD8         0x48
#define DIK_NUMPAD9         0x49
#define DIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define DIK_NUMPAD4         0x4B
#define DIK_NUMPAD5         0x4C
#define DIK_NUMPAD6         0x4D
#define DIK_ADD             0x4E    /* + on numeric keypad */
#define DIK_NUMPAD1         0x4F
#define DIK_NUMPAD2         0x50
#define DIK_NUMPAD3         0x51
#define DIK_NUMPAD0         0x52
#define DIK_DECIMAL         0x53    /* . on numeric keypad */
#define DIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define DIK_F11             0x57
#define DIK_F12             0x58
#define DIK_F13             0x64    /*                     (NEC PC98) */
#define DIK_F14             0x65    /*                     (NEC PC98) */
#define DIK_F15             0x66    /*                     (NEC PC98) */
#define DIK_KANA            0x70    /* (Japanese keyboard)            */
#define DIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define DIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define DIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define DIK_YEN             0x7D    /* (Japanese keyboard)            */
#define DIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define DIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define DIK_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define DIK_AT              0x91    /*                     (NEC PC98) */
#define DIK_COLON           0x92    /*                     (NEC PC98) */
#define DIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define DIK_KANJI           0x94    /* (Japanese keyboard)            */
#define DIK_STOP            0x95    /*                     (NEC PC98) */
#define DIK_AX              0x96    /*                     (Japan AX) */
#define DIK_UNLABELED       0x97    /*                        (J3100) */
#define DIK_NEXTTRACK       0x99    /* Next Track */
#define DIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define DIK_RCONTROL        0x9D
#define DIK_MUTE            0xA0    /* Mute */
#define DIK_CALCULATOR      0xA1    /* Calculator */
#define DIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define DIK_MEDIASTOP       0xA4    /* Media Stop */
#define DIK_VOLUMEDOWN      0xAE    /* Volume - */
#define DIK_VOLUMEUP        0xB0    /* Volume + */
#define DIK_WEBHOME         0xB2    /* Web home */
#define DIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define DIK_DIVIDE          0xB5    /* / on numeric keypad */
#define DIK_SYSRQ           0xB7
#define DIK_RMENU           0xB8    /* right Alt */
#define DIK_PAUSE           0xC5    /* Pause */
#define DIK_HOME            0xC7    /* Home on arrow keypad */
#define DIK_UP              0xC8    /* UpArrow on arrow keypad */
#define DIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define DIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define DIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define DIK_END             0xCF    /* End on arrow keypad */
#define DIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define DIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define DIK_INSERT          0xD2    /* Insert on arrow keypad */
#define DIK_DELETE          0xD3    /* Delete on arrow keypad */
#define DIK_LWIN            0xDB    /* Left Windows key */
#define DIK_RWIN            0xDC    /* Right Windows key */
#define DIK_APPS            0xDD    /* AppMenu key */
#define DIK_POWER           0xDE    /* System Power */
#define DIK_SLEEP           0xDF    /* System Sleep */
#define DIK_WAKE            0xE3    /* System Wake */
#define DIK_WEBSEARCH       0xE5    /* Web Search */
#define DIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define DIK_WEBREFRESH      0xE7    /* Web Refresh */
#define DIK_WEBSTOP         0xE8    /* Web Stop */
#define DIK_WEBFORWARD      0xE9    /* Web Forward */
#define DIK_WEBBACK         0xEA    /* Web Back */
#define DIK_MYCOMPUTER      0xEB    /* My Computer */
#define DIK_MAIL            0xEC    /* Mail */
#define DIK_MEDIASELECT     0xED    /* Media Select */

/*
 *  Alternate names for keys, to facilitate transition from DOS.
 */
#define DIK_BACKSPACE       DIK_BACK            /* backspace */
#define DIK_NUMPADSTAR      DIK_MULTIPLY        /* * on numeric keypad */
#define DIK_LALT            DIK_LMENU           /* left Alt */
#define DIK_CAPSLOCK        DIK_CAPITAL         /* CapsLock */
#define DIK_NUMPADMINUS     DIK_SUBTRACT        /* - on numeric keypad */
#define DIK_NUMPADPLUS      DIK_ADD             /* + on numeric keypad */
#define DIK_NUMPADPERIOD    DIK_DECIMAL         /* . on numeric keypad */
#define DIK_NUMPADSLASH     DIK_DIVIDE          /* / on numeric keypad */
#define DIK_RALT            DIK_RMENU           /* right Alt */
#define DIK_UPARROW         DIK_UP              /* UpArrow on arrow keypad */
#define DIK_PGUP            DIK_PRIOR           /* PgUp on arrow keypad */
#define DIK_LEFTARROW       DIK_LEFT            /* LeftArrow on arrow keypad */
#define DIK_RIGHTARROW      DIK_RIGHT           /* RightArrow on arrow keypad */
#define DIK_DOWNARROW       DIK_DOWN            /* DownArrow on arrow keypad */
#define DIK_PGDN            DIK_NEXT            /* PgDn on arrow keypad */

/*
 *  Alternate names for keys originally not used on US keyboards.
 */
#define DIK_CIRCUMFLEX      DIK_PREVTRACK       /* Japanese keyboard */
////

#define SX_INPUT_OK 0

#define SX_INPUT_ATTACH_THREAD_IN	1	//удачно подцепился к пользовательскому вводу
#define SX_INPUT_ATTACH_THREAD_OUT	2	//удачно отцепился от пользовательского ввода

#define SX_INPUT_ERR_CREATE_DI					-1	//DirectInput не создано
#define SX_INPUT_ERR_CDI_INVALID_ARG			-12	//неверные параметры
#define SX_INPUT_ERR_CDI_OUT_OF_MEM				-13	//нехватка памяти
#define SX_INPUT_ERR_CDI_NONE_ERR				-14	//неопознаная ошибка

#define SX_INPUT_ERR_ATTACH_THREAD_IN			-2	//не удалось подцепиться к пользовательскому вводу
#define SX_INPUT_ERR_ATTACH_THREAD_OUT			-3	//не удалось отцепиться от пользовательского ввода

#define SX_INPUT_ERR_CREATE_DEVICE_KEYBOARD		-4	//не удалось создать устройство "клавиатура"
#define SX_INPUT_ERR_SET_DATA_FORMAT_KEYBOARD	-5	//не удалось установить формат ввода для устройства "клавиатура"
#define SX_INPUT_ERR_COOPERATIVE_KEYBOARD		-6	//не удалось установить уровень кооперации для устройства "клавиатура"
#define SX_INPUT_ERR_ACQUIRE_KEYBOARD			-7	//не удалось захватить устройство "клавиатура"

#define SX_INPUT_ERR_CREATE_DEVICE_MOUSE		-8	//не удалось создать устройство "мышь"
#define SX_INPUT_ERR_SET_DATA_FORMAT_MOUSE		-9	//не удалось установить формат ввода для устройства "мышь"
#define SX_INPUT_ERR_COOPERATIVE_MOUSE			-10	//не удалось установить уровень кооперации для устройства "мышь"
#define SX_INPUT_ERR_ACQUIRE_MOUSE				-11	//не удалось захватить устройство "мышь"


//девайсы для сообщений
#define SX_INPUT_DEVICE_MOUSE					0
#define SX_INPUT_DEVICE_KEYBOARD				1

#define SX_INPUT_COUNT_EVENTS_FOR_DBL_CLICK		3	//сколько событий должно произойти для возникновения события двойного клика
#define SX_INPUT_PERIOD_RE_PRESSED				200	//период циклического события, когда кнопку нажали, ставим таймер и периодично выдаем события DOWN
#define SX_INPUT_PERIOD_NON_DBL_CLICK			150	//время в течении которого двойной клик не будет засчитан
#define SX_INPUT_PERIOD_NON_DOWN_KEY			50	//время в течении которого зажатая клавиша будет считаться отпущенной
#define SX_INPUT_PERIOD_DBL_CLICK				500	//максимальное время в течении которого (-PERIOD_NON_DBL_CLICK) может произойти двойной клик

//коды ошибок
#define SX_INPUT_ERROR_NON_ACQUIRE 2
#define SX_INPUT_ERROR_UNKNOWN 1

struct SXStateMouse
{
	int Buttons[3];
	DWORD Timer[3];
};

//стрктура для шаблона сообщений
struct SXInMess
{
	SXInMess::SXInMess(){}
	SXInMess::SXInMess(WORD type,WORD sect,WORD code)
	{
		TypeDevice = type; Section = sect; Code = code;
	}

	WORD TypeDevice;	//тип устройства мышь/клавиатура
	WORD Section;		//секция кнопка/клавиша
	WORD Code;			//код сообщения из InputEvents
};

enum SXInputEvents
{
	DISSABLE		= 0,	//неактивно, для удобства, калбэк на это врядли повешать

	KEY_FIRST		= 1,	//клавиша нажата впервые
	KEY_DOWN		= 2,	//клавиша зажата
	KEY_UP			= 3,	//клавиша отжата

	BUTTON_FIRST	= 4,	//кнопка нажата впервые
	BUTTON_DOWN		= 5,	//кнопка зажата
	BUTTON_UP		= 6,	//кнопка отжата
	BUTTON_DBL		= 7,	//двойной клик кнопкой

	MOUSE_SCROLL	= 8 	//сроллинг
};

enum SXInputDev
{
	LBUTTON	= 0,
	RBUTTON	= 1,
	MBUTTON	= 2,

	KEY		= 3 
};

namespace SXInputSymbol
{
	WORD KeyLetters[27]				= {DIK_A,DIK_B,DIK_C,DIK_D,DIK_E,DIK_F,DIK_G,DIK_H,DIK_I,DIK_J,DIK_K,DIK_L,DIK_M,DIK_N,DIK_O,DIK_P,DIK_Q,DIK_R,DIK_S,DIK_T,DIK_U,DIK_V,DIK_W,DIK_X,DIK_Y,DIK_Z,DIK_SPACE};
	char BigLetters[27]				= {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
	char SmallLetters[27]			= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',' '};

	WORD KeyNumbers[10]				= {DIK_1,DIK_2,DIK_3,DIK_4,DIK_5,DIK_6,DIK_7,DIK_8,DIK_9,DIK_0};
	WORD KeyNumbers2[10]			= {DIK_NUMPAD1,DIK_NUMPAD2,DIK_NUMPAD3,DIK_NUMPAD4,DIK_NUMPAD5,DIK_NUMPAD6,DIK_NUMPAD7,DIK_NUMPAD8,DIK_NUMPAD9,DIK_NUMPAD0};
	char NumbersStr[10]				= {'1','2','3','4','5','6','7','8','9','0'};
};

SX_LIB_API long SSInput_0GetVersion(); //возвращает версию ядра
SX_LIB_API void SSInput_Dbg_Set(report_func rf); //возвращает версию ядра
SX_LIB_API void SSInput_0Create(const char* name,HWND hwnd,bool is_unic=true);
SX_LIB_API void SSInput_Update(); //обновление состояния устройств
SX_LIB_API bool SSInput_GetKeyState(unsigned char Key);	//нажата ли клавиша под номером Key
SX_LIB_API bool SSInput_GetButtonState(unsigned char Number); //нажата ли кнопка мыши под номером Number
SX_LIB_API bool SSInput_IsOtherButtonOn(unsigned char Button); //нажаты ли другие кнопки мыши, Button - для которой отслеживаем единственное нажатие
SX_LIB_API DWORD SSInput_GetKeyEvents(unsigned char Key); //получить событие посылаемое клавишей на клавиатуре
SX_LIB_API WORD SSInput_GetButtonEvent(unsigned char Button); //получить событие посылаемое кнопкой мыши
SX_LIB_API long SSInput_GetScroll(); //получить скролл, -120 - назад, 120 - вперед (120 - 1 проход)
SX_LIB_API bool SSInput_GetMouseDouble(unsigned char Button); //был ли сделан кнопкой мыши Button двойной клик
SX_LIB_API int SSInput_IsMouseClick(); //возвращает код нажатой сейчас кнопки мыши
SX_LIB_API bool SSInput_GetExeEventsS(SXInMess *Event); //было ли совершенно данное событие
SX_LIB_API bool SSInput_GetExeEvents(WORD type,WORD sect,WORD code); //аналог
SX_LIB_API bool SSInput_GetActiveKeyOrButton(); //нажата ли хоть какая-то кнопка клавиатуры либо клавиша мыши
SX_LIB_API bool SSInput_GetActiveButton(); //нажата ли хоть какая-то клавиша мыши
SX_LIB_API bool SSInput_GetActiveKey(); //нажата ли хоть какая-то кнопка клавиатуры

#endif