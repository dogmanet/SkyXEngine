
/*
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
license MIT see in LICENSE or
https://s4g.su/
*/

#pragma once

#define SX_EXE

#define S4G_NPRINT_OPS
#define S4G_FASTFAIL


#include <process.h>
#include <ctime>
#include <fstream>
#include <vector>

#include "s4g\s4g.h"
#include "s4g\s4g_main.cpp"
#include "s4g\s4g_api.cpp"
#include "s4g\s4g_stdlib.h"

s4g_main* s4gm = 0;
bool IsPause = true;

void report(s4g_main* s4gm, int level, const char* name_ss, const char* format, ...)
{
#if defined(_WINDOWS)
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
#endif
	char buf[4096];
	va_list va;
	va_start(va, format);
	vsprintf_s(buf, sizeof(buf), format, va);
	va_end(va);

	if (level == 0)
		fprintf(stdout, "s4g %s notice: %s\n", name_ss, buf);
	else if (level == 1)
		fprintf(stdout, "s4g %s warning: %s\n", name_ss, buf);
	else
	{
		fprintf(stdout, "s4g %s error: %s\n", name_ss, buf);

	if(IsPause)
		system("pause");

		exit(1);
	}
}


int main(int argc, char ** argv)
{
	srand((unsigned int)time(0));

	if(argc < 2)
	{
		fprintf(stderr, "No input file");
		return(1);
	}

	s4g_set_rf(report);

	for (int i = 0; i < argc; ++i){
		if (stricmp(argv[i], "nonpause") == 0){
			IsPause = false;
			break;
		}
	}
	
	s4gm = s4g_init("");
	s4g_export_stdlib(s4gm);
	
	int status = s4g_load_file(s4gm, argv[1]);
	
	s4g_call(s4gm);

	s4g_kill(s4gm);

	if (IsPause)
		system("pause");

    return(0);
}