

#pragma once

#define SX_EXE

#include <windows.h>
#include <process.h>
#include <ctime>
#include <fstream>
#include <vector>
#pragma comment(lib, "winmm.lib")
#include <LuaJIT-2.0.3\src\lua.hpp>
#include <s4g\s4g.cpp>

s4g_main* s4gm = 0;
lua_State* LuaState = 0;

WORD countrecurs = 0;

int testcf(s4g_main* _s4gm)
{
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(0));

	/*int* qw1 = new int;
	int* qw2 = qw1;
	mem_delete(qw1);*/

	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	freopen("CONIN$", "rt", stdin);


	/*LuaState = luaL_newstate();

	luaL_openlibs(LuaState);
	DWORD time2 = timeGetTime();
	int status2 = luaL_loadfile(LuaState, "D:\\project\\engine\\SkyXEngine\\SkyXEngine\\Debug\\test.script");//luaL_dofile(L, file);
	time2 = timeGetTime() - time2;
	int qwert2 = 0;

	if (status2)
	{
		char errorfmt[1024];
		const char* error = lua_tostring(LuaState, -1);
		sprintf(errorfmt, "File generator error: %s:%d\n%s", __FILE__, __LINE__, error);
		//ObjectScriptCore->SetError(errorfmt);
		return -1;
		//MessageBox(0,errorfmt,0,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		//exit(0);
	}

	int counttt = 1;
	
	for (int i = 0; i < counttt; i++)
	{
		//lua_call(LuaState, 0, 0);
		if (lua_pcall(LuaState, 0, 0,0) != 0)	//!!!Run-time Lua error
		{
		char errorfmt[1024];
		const char* error = lua_tostring(LuaState, -1);
		sprintf(errorfmt, "File generator error: %s:%d\n%s", __FILE__, __LINE__, error);
		return -1;
		}
	}*/
	



	s4gm = s4g_init();
	s4g_store_g(s4gm, "testcf", testcf);
	//DWORD ttime = timeGetTime();
	DWORD time = timeGetTime();
	int status = s4g_load_file(s4gm,"D:\\project\\engine\\SkyXEngine\\SkyXEngine\\Debug\\test.script");
	time = timeGetTime() - time;
	int qwert = 0;
	//for (int i = 0; i < counttt; i++)
	//{
		//status = s4gm->vmachine->run(s4gm->commands, &(s4gm->vmachine->gvars));
		/*if (status != 0)
		{
			sprintf(s4gm->strerror, "%s", s4gm->vmachine->strerror);
			return status;
		}*/
	//}
	
	MessageBox(0,s4gm->strerror,0,0);
	

	

	
	/*if (lua_pcall(LuaState, 0, 0, 0) != 0)	//!!!Run-time Lua error
	{
		char errorfmt[1024];
		const char* error = lua_tostring(LuaState, -1);
		sprintf(errorfmt, "File generator error: %s:%d\n%s", __FILE__, __LINE__, error);
		//ObjectScriptCore->SetError(errorfmt);
		return -1;

		//AddFunc::ClipBoardCopy(errorfmt);
		//MessageBox(0,errorfmt,0,MB_OK|MB_ICONSTOP|MB_SYSTEMMODAL);
		//exit(0);
	}*/

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
	



	//static DWORD lastTime = timeGetTime();
	//static DWORD TimeCCadr = 0;

		while(msg.message != WM_QUIT /*&& IsWindow(SkyXEngine::Core::Data::HandleWinD3D)*/)
		{
				if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else
				{	
					/*static DWORD TimeStart = 0;
					DWORD TimeThis = timeGetTime();

					DWORD currTime  = timeGetTime();
					DWORD timeDelta = (currTime - lastTime);
					timeDelta = (timeDelta + TimeCCadr) / 2;
					//SkyXEngine::Core::Render::MainRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;*/
				}
		}

    return msg.wParam;
}