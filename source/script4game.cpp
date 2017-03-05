

#pragma once

#define SX_EXE
#include <vld.h> 
#include <windows.h>
#include <process.h>
#include <ctime>
#include <fstream>
#include <vector>
#pragma comment(lib, "winmm.lib")
#include <LuaJIT-2.0.3\src\lua.hpp>
#include <s4g\s4g.h>
#include <s4g\s4g_main.cpp>
#include <s4g\s4g_api.cpp>
#include <s4g\s4g_stdlib.h>

//#pragma comment(lib, "s4g.lib")

char* exestr = "testcall = function (a1,a2)\
			   asdf2 = 10;\
			   //asdf2 + _g.asdf;\n\
			   end;\
			   a = 10;\
			   function asdf(a1)\
			   a1 = 15;\
			   end;\
			   testcall2(a); ";

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
	
	int status2 = luaL_loadfile(LuaState, "D:\\project\\engine\\SkyXEngine\\SkyXEngine\\Debug\\lua.script");//luaL_dofile(L, file);
	

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

	int counttt = 100;
	
	DWORD time2 = timeGetTime();
	for (int i = 0; i < 1; i++)
	{
		//lua_call(LuaState, 0, 0);
		//lua_getfield(LuaState, LUA_GLOBALSINDEX, "testcall");
		//lua_pushnumber(LuaState, 2);
		//lua_pushnumber(LuaState, 7);
		//lua_call(LuaState, 2, 1);
		if (lua_pcall(LuaState, 0, 0,0) != 0)	//!!!Run-time Lua error
		{
		char errorfmt[1024];
		const char* error = lua_tostring(LuaState, -1);
		sprintf(errorfmt, "File generator error: %s:%d\n%s", __FILE__, __LINE__, error);
		MessageBox(0, errorfmt, 0, 0);
		return -1;
		}
		//lua_gc(LuaState, LUA_GCCOLLECT, 0);
	}
	//time2 = timeGetTime();
	//lua_gc(LuaState, LUA_GCCOLLECT, 0);
	time2 = timeGetTime() - time2;
	int lcs = lua_gettop(LuaState);
	int qwert2 = 0;*/


	//for (int i = 0; i < S4G_MAX_CALL; ++i)
	//{
		//s4g_table* tt = new s4g_table();
	//}
	
	s4gm = s4g_init("");
	s4g_export_stdlib(s4gm);
	//s4g_spush_c_func(s4gm, testcf);
	//s4g_sstore_g(s4gm, "testcf");
	
	int status = s4g_load_file(s4gm, "D:/project/engine/SkyXEngine/SkyXEngine/Debug/sort.script");
	//status = s4g_load_str(s4gm, exestr);
	
	
	
	DWORD tmpcount = 0;
	DWORD time = timeGetTime();
	s4g_call(s4gm);
	//s4g_clear(s4gm);
	//clrscr();
	//system("cls");
	printf("\n\n\n\n\n");
	//int* arrint = new int[100000];
	//s4g_load_file(s4gm, "D:/project/engine/SkyXEngine/SkyXEngine/Debug/sort.script");
	int qwerty = 0;
	for (int i = 0; i < 100000000; i++)
	{
		//s4gm->arr_lex->clear();
		//s4gm->arr_lex->read("D:/project/engine/SkyXEngine/SkyXEngine/Debug/sort.script");
		/*s4g_clear(s4gm);
		s4g_load_file(s4gm, "D:/project/engine/SkyXEngine/SkyXEngine/Debug/sort.script");*/
		//s4g_spush_precall(s4gm,)
		//s4g_spush_precall(s4gm);
		//s4g_sget(s4gm, S4G_NM_GLOBAL, "testcall");
		//s4g_spush_int(s4gm, 2);
		//s4g_spush_int(s4gm, 7);
		//s4g_call(s4gm,true);

		s4g_call(s4gm);
		
		
		//s4g_spop(s4gm, 1);
		/*++qwerty;
		if (qwerty >= 1)
		{
			long tmpcount = s4g_gc_mem_busy(s4gm);*/
			s4g_call_gc(s4gm);
			/*long tmpcount2 = s4g_gc_mem_busy(s4gm);
			long tmpcount3 = tmpcount - tmpcount2;
			long tmpcount4 = s4g_gc_mem_allocated(s4gm);
			qwerty = 0;
		}*/
		//s4gm->gc->resort();
		//tmpcount++;
		//s4g_int tnum = s4g_sget_int(s4gm, -1);
	}
	//s4g_int tnum = s4g_sget_int(s4gm, -1);
	//time = timeGetTime();
	//s4g_call_gc(s4gm);
	//s4gm->gc->resort();
	//s4g_clear(s4gm);
	//s4gm->arr_lex->clear();
	time = timeGetTime() - time;
	int qwert = 0;

	//time = timeGetTime();
	//s4g_call_gc(s4gm, 0);
	//s4gm->gc->resort();
	//time = timeGetTime() - time;
	/*char texttime[64];
	sprintf(texttime, "lua = %d | s4g = %d", time2, time);
	MessageBox(0, texttime, 0, 0);*/
	
	s4g_kill(s4gm);
	

	
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

		/*while(msg.message != WM_QUIT )
		{
				if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else
				{	
					
				}
		}

    return msg.wParam;*/

	return 0;
}