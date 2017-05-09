
#ifndef SX_INPUT_CLASS_H
#define SX_INPUT_CLASS_H

#include <common/String.h>

#include <queue>

#pragma once

#define SXI_KEYMAP_SIZE 256

//коды ошибок
#define SX_INPUT_ERROR_NON_ACQUIRE 2
#define SX_INPUT_ERROR_UNKNOWN 1

#define SX_INPUT_ERR_CREATE_DI					-1	/*DirectInput не создано*/
#define SX_INPUT_ERR_REGISTER_RI				-15	/*Не удалось зарегистрировать RAW input device*/
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

//элемент карты ввода
struct IKM
{
	const char * bind;
	char * cmd;
	BOOL state;
	BOOL changed;
	BOOL is_dblclick;
};

struct SXIKD
{
	UINT repeatCount : 16;
	UINT scanCode : 8;
	UINT isExtended : 1;
	UINT reserved : 4;
	UINT contextCode : 1;
	UINT previousState : 1;
	UINT transitionState : 1;
};

class SXInput
{
public:
	SXInput(const char* name);
	~SXInput();

	long Init(HWND hwnd, HINSTANCE hinst);
	//необходимые пользовательские функции и переменные
	
	void	Update();	//обновление стэйтов, включает в себя ReadMouseData и ReadKeyBoardData

	
	bool	GetKeyState(InputCode Key);	//нажата ли клавиша под номером Key
	bool	IsOtherButtonOn(InputCode Button);	//нажаты ли другие кнопки мыши, Button - для которой отслеживаем единственное нажатие

	InputEvents	GetKeyEvents(InputCode Key);	//получить событие посылаемое клавишей на клавиатуре
	InputEvents	GetButtonEvent(InputCode Button);//получить событие посылаемое кнопкой мыши
	long	GetScroll();
	bool	GetMouseDouble(InputCode Button);	//был ли сделан кнопкой мыши Button двойной клик

	bool	GetExeEvents(InMess *Event);					//было ли совершенно данное событие
	bool	GetExeEvents(InputCode sect, InputEvents code);	//аналог

	void QueueMsg(const IMSG & msg);

	void InitKeymap(); // инициализация карты ввода

	// связывает кнопку с консольной командой
	void Bind(const char * key, const char * cmd);

	void GetMouseDelta(int * x, int * y);

private:
	char Name[CORE_NAME_MAX_LEN];

	IKM m_vKeyMap[SXI_KEYMAP_SIZE]; // карта состояния ввода

	struct mpoint
	{
		int x;
		int y;
	};

	mpoint mdelta;

	std::queue<IMSG> m_qMsgs; // буфер сообщений ввода. // Нам действительно надо буферизовать это?

	mpoint mdeltaOld;

	int wheelDelta = 0;
	int wheelCount = 0;
};


#endif