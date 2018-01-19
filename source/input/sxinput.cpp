
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxinput.h"
#include "input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#define SXINPUT_VERSION 2

SXInput* ObjectInput = 0;
#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

#define SI_PRECOND(retval) if(!ObjectInput){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxinput is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SSInput_0GetVersion()
{
	return SXINPUT_VERSION;
}

SX_LIB_API void SSInput_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
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

	ObjectInput->Init(hwnd);
}

SX_LIB_API void SSInput_0Create(const char* name, HWND hwnd, bool is_unic)
{
		if(name && strlen(name) > 1)
		{
				if(is_unic)
				{
					HANDLE hMutex = CreateMutex(NULL, FALSE, name);
						if(GetLastError() == ERROR_ALREADY_EXISTS)
						{
							CloseHandle(hMutex);
							LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name for system input", GEN_MSG_LOCATION);
							return;
						}
				}
			InitIntup(name, hwnd);
		}
		else
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name] for system input", GEN_MSG_LOCATION);
}

SX_LIB_API void SSInput_Update()
{
	SI_PRECOND(_VOID);
	ObjectInput->Update();
}

SX_LIB_API bool SSInput_GetKeyState(InputCode Key)
{
	SI_PRECOND(false);
	return ObjectInput->GetKeyState(Key);
}

SX_LIB_API bool SSInput_IsOtherButtonOn(InputCode Button)
{
	SI_PRECOND(false);
	return ObjectInput->IsOtherButtonOn(Button);
}

SX_LIB_API InputEvents SSInput_GetKeyEvents(InputCode Key)
{
	SI_PRECOND(InputEvents::iv_dissable);
	return ObjectInput->GetKeyEvents(Key);
}

SX_LIB_API long SSInput_GetScroll()
{
	SI_PRECOND(0);
	return ObjectInput->GetScroll();
}

SX_LIB_API bool SSInput_GetMouseDouble(InputCode Button)
{
	SI_PRECOND(false);
	return ObjectInput->GetMouseDouble(Button);
}


SX_LIB_API bool SSInput_GetExeEventsS(InMess *Event)
{
	SI_PRECOND(false);
	return ObjectInput->GetExeEvents(Event);
}

SX_LIB_API bool SSInput_GetExeEvents(InputCode sect, InputEvents code)
{
	SI_PRECOND(false);
	return ObjectInput->GetExeEvents(sect,code);
}

SX_LIB_API bool SSInput_GetActiveKeyOrButton()
{
	SI_PRECOND(false);
		
	for(int i=0;i<SXI_KEYMAP_SIZE;i++)
	{
		if(ObjectInput->GetKeyState(i))
			return true;
	}

	return false;
}

SX_LIB_API bool SSInput_GetActiveButton()
{
	SI_PRECOND(false);
	for(int i = SIM_START; i <= SIM_END; i++)
	{
		if(ObjectInput->GetKeyState(i))
			return true;
	}
	return false;
}

SX_LIB_API bool SSInput_GetActiveKey()
{
	SI_PRECOND(false);
	for(int i = 0; i<SXI_KEYMAP_SIZE; i++)
	{
		if((i < SIM_START || i > SIM_END) && ObjectInput->GetKeyState(i))
			return true;
	}

	return false;
}

SX_LIB_API void SSInput_AddMsg(const IMSG & msg)
{
	if(ObjectInput)
	{
		ObjectInput->QueueMsg(msg);
	}
}

SX_LIB_API void SSInput_GetMouseDelta(int * x, int * y)
{
	SI_PRECOND(_VOID);

	ObjectInput->GetMouseDelta(x, y);
}