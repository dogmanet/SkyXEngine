

#pragma once

#define SX_EXE

#define S4G_NPRINT_OPS

#include <windows.h>
#include <process.h>
#include <ctime>
#include <fstream>
#include <vector>
#pragma comment(lib, "winmm.lib")
#include <s4g\s4g.h>
#include <s4g\s4g_main.cpp>
#include <s4g\s4g_api.cpp>
#include <s4g\s4g_stdlib.h>

//#pragma comment(lib, "s4g.lib")

s4g_main* s4gm = 0;

WORD countrecurs = 0;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(0));

	/*int* qw1 = new int;
	int* qw2 = qw1;
	mem_delete(qw1);*/

	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	freopen("CONIN$", "rt", stdin);



	s4gm = s4g_init("");
	s4g_export_stdlib(s4gm);
	
	int status = s4g_load_file(s4gm, lpCmdLine);
	
	s4g_call(s4gm);
	
    return(0);
}