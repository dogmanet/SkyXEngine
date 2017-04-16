
#include <input\sxinput.h>
#include <input\input.cpp>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#define SXINPUT_VERSION 1

SXInput* ObjectInput = 0;
#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

#define SI_PRECOND(retval) if(!ObjectInput){reportf(-1, "%s - sxinput is not init", gen_msg_location); return retval;}

long SSInput_0GetVersion()
{
	return SXINPUT_VERSION;
}

void SSInput_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void InitIntup(const char* name, HWND hwnd)
{
	ObjectInput = new SXInput(name);
	int cerr = ObjectInput->Init(hwnd, GetModuleHandle(0));
	if (cerr == SX_INPUT_ERR_CDI_INVALID_ARG)
		reportf(-1, "%s - invalid args, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_CDI_OUT_OF_MEM)
		reportf(-1, "%s - out of memory, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_CDI_NONE_ERR)
		reportf(-1, "%s - other error, system input", gen_msg_location);

	else if (cerr == SX_INPUT_ERR_CREATE_DEVICE_KEYBOARD)
		reportf(-1, "%s - none init device keyboard, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_SET_DATA_FORMAT_KEYBOARD)
		reportf(-1, "%s - none init format for device keyboard, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_COOPERATIVE_KEYBOARD)
		reportf(-1, "%s - none init level cooperation for device keyboard, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_ACQUIRE_KEYBOARD)
		reportf(-1, "%s - none acquire device keyboard, system input", gen_msg_location);

	else if (cerr == SX_INPUT_ERR_CREATE_DEVICE_MOUSE)
		reportf(-1, "%s - none init device mouse, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_SET_DATA_FORMAT_MOUSE)
		reportf(-1, "%s - none init format for device mouse, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_COOPERATIVE_MOUSE)
		reportf(-1, "%s - none init level cooperation for device mouse, system input", gen_msg_location);
	else if (cerr == SX_INPUT_ERR_ACQUIRE_MOUSE)
		reportf(-1, "%s - none acquire device mouse, system input", gen_msg_location);
}

void SSInput_0Create(const char* name,HWND hwnd,bool is_unic)
{
		if(name && strlen(name) > 1)
		{
				if(is_unic)
				{
					HANDLE hMutex = CreateMutex(NULL, FALSE, name);
						if(GetLastError() == ERROR_ALREADY_EXISTS)
						{
							CloseHandle(hMutex);
							reportf(-1, "%s - none unic name for system input", gen_msg_location);
						}
						else
						{
							InitIntup(name, hwnd);
						}
				}
				else
				{
					InitIntup(name, hwnd);
				}
		}
		else
			reportf(-1, "%s - not init argument [name] for system sound", gen_msg_location);
}

void SSInput_Update()
{
	SI_PRECOND(_VOID);
	ObjectInput->Update();
}

bool SSInput_GetKeyState(InputCode Key)
{
	SI_PRECOND(false);
	return ObjectInput->GetKeyState(Key);
}

bool SSInput_GetButtonState(InputCode Number)
{
	SI_PRECOND(false);
	return ObjectInput->GetButtonState(Number);
}

bool SSInput_IsOtherButtonOn(InputCode Button)
{
	SI_PRECOND(false);
	return ObjectInput->IsOtherButtonOn(Button);
}

InputEvents SSInput_GetKeyEvents(InputCode Key)
{
	SI_PRECOND(InputEvents::iv_dissable);
	return ObjectInput->GetKeyEvents(Key);
}

InputEvents SSInput_GetButtonEvent(InputCode Button)
{
	SI_PRECOND(InputEvents::iv_dissable);
	return ObjectInput->GetButtonEvent(Button);
}

long SSInput_GetScroll()
{
	SI_PRECOND(0);
	return ObjectInput->GetScroll();
}

bool SSInput_GetMouseDouble(InputCode Button)
{
	SI_PRECOND(false);
	return ObjectInput->GetMouseDouble(Button);
}

InputCode SSInput_IsMouseClick()
{
	SI_PRECOND(-1);
	return ObjectInput->IsMouseClick();
}

bool SSInput_GetExeEventsS(InMess *Event)
{
	SI_PRECOND(false);
	return ObjectInput->GetExeEvents(Event);
}

bool SSInput_GetExeEvents(InputDevice type, InputCode sect, InputEvents code)
{
	SI_PRECOND(false);
	return ObjectInput->GetExeEvents(type,sect,code);
}

bool SSInput_GetActiveKeyOrButton()
{
	SI_PRECOND(false);
		
	for(int i=0;i<256;i++)
	{
		if(ObjectInput->GetKeyState(i))
			return true;
	}

	for(int i=0;i<3;i++)
	{
		if(ObjectInput->GetButtonState(i))
			return true;
	}
	return 0;
}

bool SSInput_GetActiveButton()
{
	SI_PRECOND(false);
	for(int i=0;i<3;i++)
	{
		if(ObjectInput->GetButtonState(i))
			return true;
	}
	return 0;
}

bool SSInput_GetActiveKey()
{
	SI_PRECOND(false);
	for(int i=0;i<256;i++)
	{
		if(ObjectInput->GetKeyState(i))
			return true;
	}

	return 0;
}
