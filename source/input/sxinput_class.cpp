
#include <input\sxinput.h>
#include <input\sxinput_class.h>

#pragma once

SXInput::SXInput(const char* name)
{
	sprintf(Name,"%s",name);
}	

long SXInput::Init(HWND hwnd, HINSTANCE hinst)
{
	long tmperror = 0;
	HRESULT hr = DirectInput8Create(hinst, DIRECTINPUT_HEADER_VERSION,IID_IDirectInput8, (void**)&DirectInput, NULL);
		if(hr == DI_OK)
		{
			 tmperror = InitKeyboard(hwnd,DirectInput,&DeviceKeyBoard);
				if(tmperror != 0)
					return tmperror;

			 tmperror = InitMouse(hwnd,DirectInput,&DeviceMouse);
				if(tmperror != 0)
					return tmperror;

			InsTime = 0;
			DoubleClick = -1;

				for(int i=0;i<256;i++)
				{
					TimerKeyStateBuffer[i] = 0;
					KeyStateEvents[i] = 0;
				}

			ControlMouseButton = -1;
			ShiftEvent = false;
		}
		else
		{
			if (hr == DIERR_INVALIDPARAM)
				return SX_INPUT_ERR_CDI_INVALID_ARG;
			else if (hr == DIERR_OUTOFMEMORY)
				return SX_INPUT_ERR_CDI_OUT_OF_MEM;
			else
				return SX_INPUT_ERR_CDI_NONE_ERR;
		}
	return 0;
}

SXInput::~SXInput()
{
	DirectInput->Release();
	DeviceKeyBoard->Release();
	DeviceMouse->Release();
}

long SXInput::InitKeyboard(HWND hwnd, IDirectInput8 *DirectInput,IDirectInputDevice8** dev)
{
	HWND CurrWnd = GetForegroundWindow();
	int MyTID = GetCurrentThreadId();
	int CurrTID = GetWindowThreadProcessId(CurrWnd,0);

	AttachThreadInput(MyTID, CurrTID, TRUE);
	SetForegroundWindow(hwnd);
	AttachThreadInput(MyTID, CurrTID, FALSE);

	/////////////////////
	HRESULT hr = 0;
		if (FAILED(hr = DirectInput->CreateDevice(GUID_SysKeyboard, dev, NULL)))
		{
			return SX_INPUT_ERR_CREATE_DEVICE_KEYBOARD;
		}

		if (FAILED(hr = (*dev)->SetDataFormat(&c_dfDIKeyboard)))
		{
			(*dev)->Release();
			return SX_INPUT_ERR_SET_DATA_FORMAT_KEYBOARD;
		}

		if (FAILED(hr = (*dev)->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY)))
		{
			(*dev)->Release();
			return SX_INPUT_ERR_COOPERATIVE_KEYBOARD;
		}

		if (FAILED(hr = (*dev)->Acquire()))
		{
			(*dev)->Release();
			return SX_INPUT_ERR_ACQUIRE_KEYBOARD;
		}

    return 0;
}

long SXInput::InitMouse(HWND hwnd, IDirectInput8 *DirectInput,IDirectInputDevice8** dev)
{
	HWND CurrWnd = GetForegroundWindow();
	int MyTID = GetCurrentThreadId();
	int CurrTID = GetWindowThreadProcessId(CurrWnd,0);

	AttachThreadInput(MyTID, CurrTID, TRUE);
	SetForegroundWindow(hwnd);
	AttachThreadInput(MyTID, CurrTID, FALSE);

		if (FAILED(DirectInput->CreateDevice(GUID_SysMouse, dev, NULL)))
		{
			return SX_INPUT_ERR_CREATE_DEVICE_MOUSE;
		}

		if (FAILED((*dev)->SetDataFormat(&c_dfDIMouse)))
		{
			(*dev)->Release();
			return SX_INPUT_ERR_SET_DATA_FORMAT_MOUSE;
		}

		if (FAILED((*dev)->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY)))
		{
			(*dev)->Release();
			return SX_INPUT_ERR_COOPERATIVE_MOUSE;
		}

		if (FAILED((*dev)->Acquire()))
		{
			(*dev)->Release();
			return SX_INPUT_ERR_ACQUIRE_MOUSE;
		}

    return 0;
}

int SXInput::ReadKeyBoardData(IDirectInputDevice8 *DIDevice)
{
	HRESULT hr;

		while(1) 
		{
			DIDevice->Poll();

				if(SUCCEEDED(hr = DIDevice->GetDeviceState(256,(void*)KeyStateBuffer)))
					break;

				if(hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED)
				{
					return SX_INPUT_ERROR_NON_ACQUIRE;
				}

				if(FAILED(DIDevice->Acquire()))
				{
					return SX_INPUT_ERROR_UNKNOWN;
				}
		}


		for(int i=0;i<256;i++)
		{
				if(!(KeyStateBuffer[i]) && (OldKeyStateBuffer[i]))
				{
					//key up
					KeyStateEvents[i] = SXInputEvents::KEY_UP;
					TimerKeyStateBuffer[i] = 0;
				}
				else if((KeyStateBuffer[i]) && !(OldKeyStateBuffer[i]))
				{
					//key first down
					KeyStateEvents[i] = SXInputEvents::KEY_FIRST;
					TimerKeyStateBuffer[i] = timeGetTime();
				}
				
				else if((KeyStateBuffer[i]) && (OldKeyStateBuffer[i]))
				{
						if(TimerKeyStateBuffer[i] > 0)
						{
								if(TimerKeyStateBuffer[i] <= timeGetTime())
								{
									KeyStateEvents[i] = SXInputEvents::KEY_DOWN;
									TimerKeyStateBuffer[i] = 0;
								}
								else
								{
									KeyStateEvents[i] = 0;	
								}
						}
						else
						{
							TimerKeyStateBuffer[i] = timeGetTime() + SX_INPUT_PERIOD_NON_DOWN_KEY;
							KeyStateEvents[i] = 0;
						}
				}
				else
				{
					KeyStateEvents[i] = 0;
					TimerKeyStateBuffer[i] = 0;
				}

			OldKeyStateBuffer[i] = KeyStateBuffer[i];
		}

	return 0;
}

int SXInput::ReadMouseData(IDirectInputDevice8 *DIDevice)
{
	HRESULT hr;
	DoubleClick = -1;
		while(1) 
		{
			DIDevice->Poll();

				if(SUCCEEDED(hr = DIDevice->GetDeviceState(sizeof(DIMOUSESTATE),(void*)&MouseState)))
					break;

				if(hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED)
				{
					return SX_INPUT_ERROR_NON_ACQUIRE;
				}

				if(FAILED(DIDevice->Acquire()))
				{
					return SX_INPUT_ERROR_UNKNOWN;
				}
		}


		for(int i=0;i<3;i++)
		{
				if((MouseState.rgbButtons[i] & 0x80) && !(SomeTimesMouseState.rgbButtons[i] & 0x80))
				{
					EventsMouse.Buttons[i] = SXInputEvents::BUTTON_FIRST;
				}
				else if(!(MouseState.rgbButtons[i] & 0x80) && (SomeTimesMouseState.rgbButtons[i] & 0x80))
				{
					EventsMouse.Buttons[i] = SXInputEvents::BUTTON_UP;
				}
				else if((MouseState.rgbButtons[i] & 0x80) && (SomeTimesMouseState.rgbButtons[i] & 0x80))
				{
						//if(EventsMouse.Buttons[i] != SXInputEvents::BUTTON_DOWN)
						//{
								if(EventsMouse.Timer[i] > 0)
								{
									//LOG(ToPointChar(EventsMouse.Timer[i]));
										if(EventsMouse.Timer[i] <= timeGetTime())
										{
											EventsMouse.Buttons[i] = SXInputEvents::BUTTON_DOWN;
											EventsMouse.Timer[i] = 0;
												//заглушка, на случай если событие двойного клика стартовало
												//а мы меняем стэйты кнопки
												if(InsTime)
													ShiftEvent--;
										}
										else
										{
											EventsMouse.Buttons[i] = 0;	
										}
								}
								else
								{
									//LOG(ToPointChar("PERIOD_RE_PRESSED |" + ToString(EventsMouse.Timer[i])));
									EventsMouse.Timer[i] = timeGetTime() + SX_INPUT_PERIOD_RE_PRESSED;
									EventsMouse.Buttons[i] = 0;
								}
						//}
				}
				else
				{
					EventsMouse.Timer[i] = 0;
					EventsMouse.Buttons[i] = 0;
				}
		}

		//если таймер еще не ставили и нажата какая-то кнопка мыши значит событие двоного клика не стартовало и можно его стартонуть
		//ControlMouseButton - номер кнопки для которой стартует событие двойного клика
		if(!InsTime && (ControlMouseButton = IsMouseClick(&MouseState)) > -1)
		{
			OldMouseState = MouseState;
			InsTime = timeGetTime();
		}
		//если таймер уже стоит и нажата другая кнопка тогда убиваем событие
		else if(InsTime && !(ControlMouseButton == IsMouseClick(&MouseState) || IsMouseClick(&MouseState) == -1))
		{
			OldMouseState = MouseState;
			InsTime = 0;
			DoubleClick = -1;
			ControlMouseButton = -1;
			ShiftEvent = 0;
		}
		else if(!InsTime)
			ControlMouseButton = -1;


		if(InsTime != 0)
		{
			//если время двойного клика не истекло
			if(timeGetTime() - InsTime <= SX_INPUT_PERIOD_DBL_CLICK && timeGetTime() - InsTime >= SX_INPUT_PERIOD_NON_DBL_CLICK)
			{
					if(ShiftEvent >= SX_INPUT_COUNT_EVENTS_FOR_DBL_CLICK && (MouseState.rgbButtons[ControlMouseButton] & 0x80) && (OldMouseState.rgbButtons[ControlMouseButton] & 0x80))
					{
						DoubleClick = ControlMouseButton;
						EventsMouse.Buttons[ControlMouseButton] = SXInputEvents::BUTTON_DBL;
						//EventsMouse.Timer[ControlMouseButton] = 0;
						InsTime = 0;
						ControlMouseButton = -1;
						ShiftEvent = 0;
						OldMouseState = MouseState;
					}

			}
			//если время двойного клика истекло
			else if(timeGetTime() - InsTime > SX_INPUT_PERIOD_DBL_CLICK)
			{
				InsTime = 0;
				DoubleClick = -1;
				ControlMouseButton = -1;
				ShiftEvent = 0;
				OldMouseState = MouseState;
			}
			else if(ShiftEvent < SX_INPUT_COUNT_EVENTS_FOR_DBL_CLICK)
			{
					if(EventsMouse.Buttons[ControlMouseButton] != OldEventsMouse.Buttons[ControlMouseButton])
						ShiftEvent++;
			}
		}

	SomeTimesMouseState = MouseState;
	OldEventsMouse = EventsMouse;

	return 0;
}

void SXInput::Update()
{
	ReadKeyBoardData(DeviceKeyBoard);
	ReadMouseData(DeviceMouse);
}

bool SXInput::GetKeyState(unsigned char Key)
{
	return KeyStateBuffer[Key] ? true : false;
}

DWORD SXInput::GetKeyEvents(unsigned char Key)
{
	return KeyStateEvents[Key];
}

bool SXInput::GetButtonState(unsigned char Number)
{
	return (MouseState.rgbButtons[Number] & 0x80) ? true : false;
}

long SXInput::GetScroll()
{
	return MouseState.lZ;
}

bool SXInput::GetMouseDouble(unsigned char Button)
{
 return (Button == DoubleClick);
}

WORD SXInput::GetButtonEvent(unsigned char Button)
{
	return EventsMouse.Buttons[Button];
}

int SXInput::IsMouseClick(DIMOUSESTATE *DIMouseState)
{
		for(int i=0;i<3;i++)
		{
				if(DIMouseState->rgbButtons[i] & 0x80)
					return i;
		}
	return -1;
}

bool SXInput::IsOtherButtonOn(unsigned char Button)
{
		for(int i=0;i<3;i++)
		{
				if(i != Button)
				{
						if(GetButtonEvent(i) > 0)
							return false;
				}
		}
	return true;
}

bool SXInput::GetExeEvents(SXInMess *Event)
{
		if(Event->TypeDevice == SX_INPUT_DEVICE_MOUSE)
		{
			return GetButtonEvent(Event->Section) == Event->Code?true:false;
		}
		else if(Event->TypeDevice == SX_INPUT_DEVICE_KEYBOARD)
		{
			return GetKeyEvents(Event->Section) == Event->Code?true:false;
		}
	return false;
}

bool SXInput::GetExeEvents(WORD type,WORD sect,WORD code)
{
		if(type == SX_INPUT_DEVICE_MOUSE)
		{
			return GetButtonEvent(sect) == code?true:false;
		}
		else if(type == SX_INPUT_DEVICE_KEYBOARD)
		{
			return GetKeyEvents(sect) == code?true:false;
		}
	return false;
}


bool GetActiveKeyOrButton()
{

		for(int i=0;i<256;i++)
		{
				//if(SXInputCom::DevInput->GetKeyState(i))
					return true;
		}

		for(int i=0;i<3;i++)
		{
				//if(SXInputCom::DevInput->GetButtonState(i))
					return true;
		}
	return false;
}

bool GetActiveButton()
{
		for(int i=0;i<3;i++)
		{
			//if(SXInputCom::DevInput->GetButtonState(i))
					return true;
		}
	return false;
}

bool GetActiveKey()
{
		for(int i=0;i<256;i++)
		{
				//if(SXInputCom::DevInput->GetKeyState(i))
					return true;
		}
	return false;
}
