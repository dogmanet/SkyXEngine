
#include <input\sxinput.h>
#include <input\sxinput_class.cpp>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

#define SXINPUT_VERSION 1

SXInput* ObjectInput = 0;
report_func reportf = def_report;

long SSInput_0GetVersion()
{
	return SXINPUT_VERSION;
}

void SSInput_Dbg_Set(report_func rf)
{
	reportf = rf;
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
							ObjectInput = new SXInput(name);
							int cerr = ObjectInput->Init(hwnd,GetModuleHandle(0));
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
				}
				else
				{
					ObjectInput = new SXInput(name);
					int cerr = ObjectInput->Init(hwnd,GetModuleHandle(0));
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
		}
		else
			reportf(-1, "%s - not init argument [name] for system sound", gen_msg_location);
}

void SSInput_Update()
{
		if(ObjectInput)
			ObjectInput->Update();
		else
			reportf(-1, "%s - input sub system is not init",gen_msg_location);
}

bool SSInput_GetKeyState(unsigned char Key)
{
		if(ObjectInput)
			return ObjectInput->GetKeyState(Key);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetButtonState(unsigned char Number)
{
		if(ObjectInput)
			return ObjectInput->GetButtonState(Number);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_IsOtherButtonOn(unsigned char Button)
{
		if(ObjectInput)
			return ObjectInput->IsOtherButtonOn(Button);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

DWORD SSInput_GetKeyEvents(unsigned char Key)
{
		if(ObjectInput)
			return ObjectInput->GetKeyEvents(Key);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

WORD SSInput_GetButtonEvent(unsigned char Button)
{
		if(ObjectInput)
			return ObjectInput->GetButtonEvent(Button);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

long SSInput_GetScroll()
{
		if(ObjectInput)
			return ObjectInput->GetScroll();
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetMouseDouble(unsigned char Button)
{
		if(ObjectInput)
			return ObjectInput->GetMouseDouble(Button);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

int SSInput_IsMouseClick()
{
		if(ObjectInput)
			return ObjectInput->IsMouseClick(&(ObjectInput->MouseState));
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetExeEventsS(SXInMess *Event)
{
		if(ObjectInput)
			return ObjectInput->GetExeEvents(Event);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetExeEvents(WORD type,WORD sect,WORD code)
{
		if(ObjectInput)
			return ObjectInput->GetExeEvents(type,sect,code);
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetActiveKeyOrButton()
{
		if(ObjectInput)
		{
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
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetActiveButton()
{
		if(ObjectInput)
		{
				for(int i=0;i<3;i++)
				{
						if(ObjectInput->GetButtonState(i))
							return true;
				}
			return 0;
		}
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}

bool SSInput_GetActiveKey()
{
		if(ObjectInput)
		{
				for(int i=0;i<256;i++)
				{
						if(ObjectInput->GetKeyState(i))
							return true;
				}

			return 0;
		}
		else
			reportf(-1, "%s - input sub system is not init", gen_msg_location);
}
