
#pragma once

#define SX_EXE

#include <windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <gdefines.h>
/*#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif*/

#include <handler_out_log.cpp>
#include <core\\sxcore.cpp>
#include <sxmath.h>
#include <input\\sxinput.cpp>



#pragma pack(push, 1)
struct jmp_far
{
 BYTE instr_push;
 DWORD arg;
 BYTE  instr_ret;
};
#pragma pack(pop)

int my_printf(const char * _Format, ...)
{
	Core_Report(0,"1111111111111111111");
	int qwert = 0;
	return 0;
}


#include <TlHelp32.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(0));

	/*jmp_far jump_printf;

	jump_printf.instr_push = 0x68;
	jump_printf.arg = (DWORD)&my_printf;
	jump_printf.instr_ret = 0xC3;
	DWORD written;
	//GetCurrentProcess();
	//HMODULE sxcore = GetModuleHandle("skyxengine_d.exe");


	PROCESSENTRY32 pe;
	DWORD pid = 0;
	char* _p_name = "skyxengine_d.exe";
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(snapshot==INVALID_HANDLE_VALUE) 
	{
		printf("SnapShot Failed.");
		return 0;
	}
	pe.dwSize = sizeof(PROCESSENTRY32);
	int curr = Process32First(snapshot,&pe);
	while(curr)
	{		
		CharLowerBuff(pe.szExeFile,lstrlen(pe.szExeFile));
		if(strstr(pe.szExeFile,_p_name)) 
		{pid = pe.th32ProcessID; break;}
	    curr = Process32Next(snapshot,&pe);
	}
	if(pid==NULL) 
	{
		printf("Searching of process failed.");
		return 0;
	}
	HANDLE pracess2 = GetCurrentProcess();
	HANDLE process = OpenProcess(PROCESS_ALL_ACCESS,false,pid);
	






	HMODULE msvcrt = GetModuleHandle("msvcrt.dll");
	void* lpFunc = GetProcAddress(msvcrt, "printf");
	BOOL ret = WriteProcessMemory(process, (void*)&printf, (void*)&jump_printf, sizeof(jmp_far), &written);

//ERROR_ACCESS_DENIED
	Core_0Create("test");
	//BOOL qw = AttachConsole(GetCurrentProcessId());
	//DWORD err = GetLastError();
	/*AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	freopen("CONOUT$", "wt", stderr);*/
	printf("wwwwwwwwwwwwwwww");
	Core_Report(0,"eeeeeeee");
	SSInput_Update();
	ISXFile* tmpnf = GetNewSXFile();
	tmpnf->Create("D:\\project\\engine\\build\\log2",1);
	tmpnf->Write("qwert",5);
	tmpnf->Close();
	tmpnf->Release();
	ISXLConfig* config = GetOpenSXLConfig("D:\\project\\builds\\build_22_04_2016\\gamesource\\configs\\camera_fly\\main.cf");
	const char* str = config->Get("test_start2","second_section");
	/*WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile("C:\\SkyXEngine2\\project\\*",&FindFileData);
	
		if(hf != INVALID_HANDLE_VALUE)
		{
				do
				{
					FindFileData.cFileName;
					int qwer = 0;
				}
				while(FindNextFile(hf,&FindFileData) != 0);
			FindClose(hf);
		}*/

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static DWORD lastTime = timeGetTime();
	static DWORD TimeCCadr = 0;

		while(msg.message != WM_QUIT /*&& IsWindow(SkyXEngine::Core::Data::HandleWinD3D)*/)
		{
				if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else
				{	
					static DWORD TimeStart = 0;
					DWORD TimeThis = timeGetTime();

					DWORD currTime  = timeGetTime();
					DWORD timeDelta = (currTime - lastTime);
					timeDelta = (timeDelta + TimeCCadr) / 2;
					//SkyXEngine::Core::Render::MainRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

    return msg.wParam;
}