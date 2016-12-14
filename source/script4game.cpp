

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
	int countarg =  s4g_cfcount_arg(_s4gm);
	long a2 = s4g_cfget_int(s4gm, 2);
	float a3 = s4g_cfget_float(s4gm, 3);
	UINT a4 = s4g_cfget_uint(s4gm, 4);
	bool a5 = s4g_cfget_bool(s4gm, 5);
	const char* a6 = s4g_cfget_str(s4gm, 6);
	s4g_c_function a7 = s4g_cfget_cfunc(s4gm, 7);
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
	
	int status2 = luaL_dofile(LuaState, "D:\\project\\engine\\SkyXEngine\\SkyXEngine\\Debug\\test.script");//luaL_dofile(L, file);
	

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

	int counttt = 10000;
	
	DWORD time2 = timeGetTime();
	for (int i = 0; i < counttt; i++)
	{
		//lua_call(LuaState, 0, 0);
		lua_getfield(LuaState, LUA_GLOBALSINDEX, "testcall");
		lua_pushnumber(LuaState, 2);
		lua_pushnumber(LuaState, 7);
		if (lua_pcall(LuaState, 2, 1,0) != 0)	//!!!Run-time Lua error
		{
		char errorfmt[1024];
		const char* error = lua_tostring(LuaState, -1);
		sprintf(errorfmt, "File generator error: %s:%d\n%s", __FILE__, __LINE__, error);
		return -1;
		}
	}
	int lcs = lua_gettop(LuaState);
	time2 = timeGetTime() - time2;
	int qwert2 = 0;
	*/



	s4gm = s4g_init("");
	s4g_push_c_func(s4gm, testcf);
	s4g_store_g(s4gm, "testcf");
	//DWORD ttime = timeGetTime();
	
	int status = s4g_load_file(s4gm,"D:/project/engine/SkyXEngine/SkyXEngine/Debug/test.script");
	if (s4g_call(s4gm, 0) != 0)
	{

	}
	

	/*DWORD time = timeGetTime();
	for (int i = 0; i < counttt; i++)
	{
		s4g_get_g(s4gm, "testcall");
		s4g_push_int(s4gm, 2);
		s4g_push_int(s4gm, 7);
		s4g_call(s4gm, 2);
		

		if (status != 0)
		{
			sprintf(s4gm->strerror, "%s", s4gm->vmachine->strerror);
			return status;
		}
		s4g_int tnum = s4g_sget_int(s4gm, -1);
	}
	time = timeGetTime() - time;
	int qwert = 0;
	*/
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