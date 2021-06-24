
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __INPUT_H
#define __INPUT_H

#include <queue>

#define SXI_KEYMAP_SIZE 256

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

	long Init(HWND hwnd);
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

	void setEnable(bool bEnable);

	void getBindEntry(int n, const char **pszKey, const char **pszCmd);

	void onNextKeyPress(void(*pfnCallback)(const char *szKey));

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

	bool m_bEnabled = false;

	bool m_isFirstRIM = true;

	void(*m_pfnCallback)(const char *szKey);
	bool m_bDisableOnNextKeyPress;

	void execOnNextKeyPress(const char *szKey);
};


#endif
