#ifndef SX_INPUT_CLASS_H
#define SX_INPUT_CLASS_H

#include <Mmsystem.h>
#include <DInput.h>

#pragma once

class SXInput
{
public:
	SXInput(const char* name);
	~SXInput();

	long Init(HWND hwnd, HINSTANCE hinst);
	//необходимые пользовательские функции и переменные
	int	ReadMouseData		(IDirectInputDevice8 *DIDevice);	//обновление состо€ни€€ мыши
	int	ReadKeyBoardData	(IDirectInputDevice8 *DIDevice);	//обновление состо€ни€ клавиатуры

	void	Update();	//обновление стэйтов, включает в себ€ ReadMouseData и ReadKeyBoardData

	//пользовательские функции и переменные

	bool	GetKeyState		(unsigned char Key);	//нажата ли клавиша под номером Key
	bool	GetButtonState	(unsigned char Number);	//нажата ли кнопка мыши под номером Number
	bool	IsOtherButtonOn	(unsigned char Button);	//нажаты ли другие кнопки мыши, Button - дл€ которой отслеживаем единственное нажатие

	DWORD	GetKeyEvents	(unsigned char Key);	//получить событие посылаемое клавишей на клавиатуре
	WORD	GetButtonEvent	(unsigned char Button);	//получить событие посылаемое кнопкой мыши
	long	GetScroll		();						//получить скролл, -120 - назад, 120 - вперед (120 - 1 проход)
	bool	GetMouseDouble	(unsigned char Button);	//был ли сделан кнопкой мыши Button двойной клик

	int		IsMouseClick	(DIMOUSESTATE *DIMouseState);	//возвращает код нажатой сейчас кнопки мыши

	bool	GetExeEvents(SXInMess *Event);					//было ли совершенно данное событие
	bool	GetExeEvents(WORD type,WORD sect,WORD code);	//аналог

	//продвинутые пользовательские функции и переменные

	char	KeyStateBuffer		[256];	//буффер состо€ний клавиатуры
	char	OldKeyStateBuffer	[256];	//буффер состо€ний клавиатуры при прошлом обновлении
	DWORD	KeyStateEvents		[256];	//буффер сообщений посылаемых от клавиатуры
	DWORD	TimerKeyStateBuffer	[256];	//буффер таймеров дл€ каждой клавиши клавиатуры

	DIMOUSESTATE	MouseState;				//стэйты мыши
	DIMOUSESTATE	OldMouseState;			//стэйты мыши во врем€ совершени€ прошлого нажати€ кнопки, необходимо дл€ двойного клика
	DIMOUSESTATE	SomeTimesMouseState;	//стэйты мыши при прошлом обновлении

	SXStateMouse	EventsMouse;		//событи€ посылаемые мышью
	SXStateMouse	OldEventsMouse;		//событи€ при прошлом апдэйте

private:
	char Name[CORE_NAME_MAX_LEN];
	IDirectInput8		*DirectInput;
	IDirectInputDevice8	*DeviceKeyBoard;
	IDirectInputDevice8	*DeviceMouse;

	DWORD	InsTime;		//таймер дл€ отслеживани€ двйного клика
	DWORD	DoubleClick;	//на какую кнопку поставлен двойной клик, если -1 значит ни на какую, дл€ удобства отдельно

	int	ControlMouseButton;	//аналог, дл€ удобства манипул€ций
	int	ShiftEvent;			//количество событий которые произошли при совершении двойного клика

	long InitKeyboard (HWND hwnd, IDirectInput8 *DirectInput,IDirectInputDevice8** dev);	//устанавливает устройство клавиатуры и возвращает его
	long InitMouse (HWND hwnd, IDirectInput8 *DirectInput,IDirectInputDevice8** dev);	//устанавливает устройство мыши и возвращает его
};


#endif