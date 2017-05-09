
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#include <input/sxinput.h>
#include <input/input.cpp>
#include <common/String.cpp>

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

void cmd_bind(int argc, const char ** argv)
{
	if(argc == 3)
	{
		ObjectInput->Bind(argv[1], argv[2]);
	}
	else
	{
		printf(COLOR_GREEN "Usage:\n    " COLOR_LGREEN "bind <key> <command>" COLOR_GREEN " - Bind <command> to <key>\n" COLOR_RESET);
	}
}

void InitIntup(const char* name, HWND hwnd)
{
	ObjectInput = new SXInput(name);

	Core_0RegisterConcmdArg("bind", cmd_bind, "Bind command to key");

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
							return;
						}
				}
			InitIntup(name, hwnd);
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


bool SSInput_GetExeEventsS(InMess *Event)
{
	SI_PRECOND(false);
	return ObjectInput->GetExeEvents(Event);
}

bool SSInput_GetExeEvents(InputCode sect, InputEvents code)
{
	SI_PRECOND(false);
	return ObjectInput->GetExeEvents(sect,code);
}

bool SSInput_GetActiveKeyOrButton()
{
	SI_PRECOND(false);
		
	for(int i=0;i<SXI_KEYMAP_SIZE;i++)
	{
		if(ObjectInput->GetKeyState(i))
			return true;
	}

	return false;
}

bool SSInput_GetActiveButton()
{
	SI_PRECOND(false);
	for(int i = SIM_START; i <= SIM_END; i++)
	{
		if(ObjectInput->GetKeyState(i))
			return true;
	}
	return false;
}

bool SSInput_GetActiveKey()
{
	SI_PRECOND(false);
	for(int i = 0; i<SXI_KEYMAP_SIZE; i++)
	{
		if((i < SIM_START || i > SIM_END) && ObjectInput->GetKeyState(i))
			return true;
	}

	return false;
}

void SSInput_AddMsg(const IMSG & msg)
{
	if(ObjectInput)
	{
		ObjectInput->QueueMsg(msg);
	}
}

void SSInput_GetMouseDelta(int * x, int * y)
{
	SI_PRECOND(_VOID);

	ObjectInput->GetMouseDelta(x, y);
}