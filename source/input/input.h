
#ifndef SX_INPUT_CLASS_H
#define SX_INPUT_CLASS_H

#include <Mmsystem.h>
#include <DInput.h>

#pragma once

//коды ошибок
#define SX_INPUT_ERROR_NON_ACQUIRE 2
#define SX_INPUT_ERROR_UNKNOWN 1

#define SX_INPUT_ERR_CREATE_DI					-1	/*DirectInput не создано*/
#define SX_INPUT_ERR_CDI_INVALID_ARG			-12	/*неверные параметры*/
#define SX_INPUT_ERR_CDI_OUT_OF_MEM				-13	/*нехватка памяти*/
#define SX_INPUT_ERR_CDI_NONE_ERR				-14	/*неопознаная ошибка*/

#define SX_INPUT_ERR_ATTACH_THREAD_IN			-2	/*не удалось подцепиться к пользовательскому вводу*/
#define SX_INPUT_ERR_ATTACH_THREAD_OUT			-3	/*не удалось отцепиться от пользовательского ввода*/

#define SX_INPUT_ERR_CREATE_DEVICE_KEYBOARD		-4	/*не удалось создать устройство "клавиатура"*/
#define SX_INPUT_ERR_SET_DATA_FORMAT_KEYBOARD	-5	/*не удалось установить формат ввода для устройства "клавиатура"*/
#define SX_INPUT_ERR_COOPERATIVE_KEYBOARD		-6	/*не удалось установить уровень кооперации для устройства "клавиатура"*/
#define SX_INPUT_ERR_ACQUIRE_KEYBOARD			-7	/*не удалось захватить устройство "клавиатура"*/

#define SX_INPUT_ERR_CREATE_DEVICE_MOUSE		-8	/*не удалось создать устройство "мышь"*/
#define SX_INPUT_ERR_SET_DATA_FORMAT_MOUSE		-9	/*не удалось установить формат ввода для устройства "мышь"*/
#define SX_INPUT_ERR_COOPERATIVE_MOUSE			-10	/*не удалось установить уровень кооперации для устройства "мышь"*/
#define SX_INPUT_ERR_ACQUIRE_MOUSE				-11	/*не удалось захватить устройство "мышь"*/

//состояния клавиш мыши
struct StateMouse
{
	InputEvents Buttons[3];	//текущие события для каждой кнопки мыши
	DWORD Timer[3];
};

class SXInput
{
public:
	SXInput(const char* name);
	~SXInput();

	long Init(HWND hwnd, HINSTANCE hinst);
	//необходимые пользовательские функции и переменные
	int	ReadMouseData		(IDirectInputDevice8 *DIDevice);	//обновление состоянияя мыши
	int	ReadKeyBoardData	(IDirectInputDevice8 *DIDevice);	//обновление состояния клавиатуры

	void	Update();	//обновление стэйтов, включает в себя ReadMouseData и ReadKeyBoardData

	
	bool	GetKeyState(InputCode Key);	//нажата ли клавиша под номером Key
	bool	GetButtonState(InputCode Number);	//нажата ли кнопка мыши под номером Number
	bool	IsOtherButtonOn(InputCode Button);	//нажаты ли другие кнопки мыши, Button - для которой отслеживаем единственное нажатие

	InputEvents	GetKeyEvents(InputCode Key);	//получить событие посылаемое клавишей на клавиатуре
	InputEvents	GetButtonEvent(InputCode Button);//получить событие посылаемое кнопкой мыши
	long	GetScroll		();						//получить скролл, -120 - назад, 120 - вперед (120 - 1 проход)
	bool	GetMouseDouble(InputCode Button);	//был ли сделан кнопкой мыши Button двойной клик

	InputCode IsMouseClick();	//возвращает код нажатой сейчас кнопки мыши

	bool	GetExeEvents(InMess *Event);					//было ли совершенно данное событие
	bool	GetExeEvents(InputDevice type, InputCode sect, InputEvents code);	//аналог

private:
	char Name[CORE_NAME_MAX_LEN];
	IDirectInput8		*DirectInput;
	IDirectInputDevice8	*DeviceKeyBoard;
	IDirectInputDevice8	*DeviceMouse;

	char	KeyStateBuffer[256];	//буффер состояний клавиатуры
	char	OldKeyStateBuffer[256];	//буффер состояний клавиатуры при прошлом обновлении
	InputEvents	KeyStateEvents[256];	//буффер сообщений посылаемых от клавиатуры
	DWORD		TimerKeyStateBuffer[256];	//буффер таймеров для каждой клавиши клавиатуры

	DIMOUSESTATE	MouseState;				//стэйты мыши
	DIMOUSESTATE	OldMouseState;			//стэйты мыши во время совершения прошлого нажатия кнопки, необходимо для двойного клика
	DIMOUSESTATE	SomeTimesMouseState;	//стэйты мыши при прошлом обновлении

	StateMouse	EventsMouse;		//события посылаемые мышью
	StateMouse	OldEventsMouse;		//события при прошлом апдэйте

	DWORD	InsTime;		//таймер для отслеживания двойного клика
	DWORD	DoubleClick;	//на какую кнопку поставлен двойной клик, если -1 значит ни на какую, для удобства отдельно

	int	ControlMouseButton;	//аналог, для удобства манипуляций
	int	ShiftEvent;			//количество событий которые произошли при совершении двойного клика

	long InitKeyboard (HWND hwnd, IDirectInput8 *DirectInput,IDirectInputDevice8** dev);	//устанавливает устройство клавиатуры и возвращает его
	long InitMouse (HWND hwnd, IDirectInput8 *DirectInput,IDirectInputDevice8** dev);	//устанавливает устройство мыши и возвращает его
};


#endif