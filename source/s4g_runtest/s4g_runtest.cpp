#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <memory>
#include <direct.h>

#include <string/string.h>
#include "ColorPrint.h"

#if defined(_WINDOWS)
#	define popen _popen
#	define pclose _pclose
#endif

ColorPrint g_color;
String g_sEXE;
char g_sTEXE[MAX_PATH];
String g_sOutDir;
String g_sTestDir;

const char * g_szSections[] = {
	"--TEST--",
	"--FILE--",
	"--EXPECT--"
};

enum TEST_SECTION
{
	TS_TEST = 0,
	TS_FILE,
	TS_EXPECT,

	TS_LAST
};

void RunTest(const char * file);
String exec(const char * cmd);
bool IsDir(const char * path);
bool WriteFile(const char * file, const char * data, size_t len);
void RecursiveTest(const char * szPath, const char * lpSearch);

int main(int argc, char ** argv)
{
	g_color.setBG(ANSI_BLACK);
	g_color.setColor(ANSI_WHITE);

	GetModuleFileNameA(NULL, g_sTEXE, MAX_PATH);
	//g_sEXE = "C:\\DSe\\revo\\SkyXEngine\\SkyXEngine\\Debug\\s4g_launcher.exe";
	//g_sOutDir = "C:/DSe/revo/source/s4g/tests/failed/";

	char * test = NULL;

	for(int i = 1; i < argc - 1; ++i)
	{
		if(argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case 't': // test file to run
				test = argv[i + 1];
			case 'o': // out dir
				g_sOutDir = argv[i + 1];
				break;
			case 'e': // executable file
				g_sEXE = argv[i + 1];
				break;
			case 'd': // test dir
				g_sTestDir = argv[i + 1];
				break;
			}
		}
	}


	if(!IsDir(g_sOutDir.c_str()))
	{
		if(_mkdir(g_sOutDir.c_str()))
		{
			g_color.setColor(ANSI_LRED);
			printf("Unable to create out dir '%s'\n", g_sOutDir.c_str());
			return(1);
		}
	}
	if(test)
	{
		RunTest(test);
	}
	else
	{
		if(!g_sTestDir.length())
		{
			g_color.setColor(ANSI_LRED);
			printf("Nether -t nor -d options specified\n");
			return(1);
		}

		RecursiveTest(g_sTestDir.c_str(), "*.s4gt");
	}

	g_color.setDefault();
	system("pause");
	return(0);
}

void RecursiveTest(const char * szPath, const char * lpSearch)
{
	WIN32_FIND_DATA fdFindData;
	HANDLE hFind;
	//printf("%s\n", szPath);
	String Path = String(szPath);
	if(Path[Path.length() - 1] != '/' && Path[Path.length() - 1] != '\\')
	{
		Path += '/';
	}
	String search = Path + "*";

	hFind = FindFirstFile(search.c_str(), &fdFindData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	String cur;
	do
	{
		if((0 == strcmp(fdFindData.cFileName, ".")) ||
			(0 == strcmp(fdFindData.cFileName, "..")))
		{
			continue;
		}
		cur = Path + fdFindData.cFileName;
		if(fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			cur += '/';
			RecursiveTest(cur.c_str(), lpSearch);
		}
	}
	while(FindNextFile(hFind, &fdFindData));
	FindClose(hFind);


	search = Path + lpSearch;
	hFind = FindFirstFile(search.c_str(), &fdFindData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	do
	{
		if((0 == strcmp(fdFindData.cFileName, ".")) ||
			(0 == strcmp(fdFindData.cFileName, "..")))
		{
			continue;
		}
		cur = Path + fdFindData.cFileName;
		if(!(fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			RunTest(cur.c_str());
		}
	}
	while(FindNextFile(hFind, &fdFindData));
	FindClose(hFind);
}

void RunTest(const char * file)
{
	size_t fTestSize;
	FILE * fTest;
	char * tmps;
	String line;

	String sName = file;
	sName[sName.length() - 1] = 0;

	g_color.setColor(ANSI_YELLOW);
	printf("### %s...", file);
	fTest = fopen(file, "rb");
	if(!fTest)
	{
		g_color.setColor(ANSI_RED);
		printf("\r### %s - Unable to open file!\n", file);
		return;
	}
	fseek(fTest, 0, SEEK_END);
	fTestSize = ftell(fTest);
	fseek(fTest, 0, SEEK_SET);
	tmps = (char*)alloca(fTestSize + 1);
	fread(tmps, 1, fTestSize, fTest);
	tmps[fTestSize] = '\n';
	fclose(fTest);


	String sTEST, sFILE, sEXPECT;

	TEST_SECTION curSect;
	size_t sPos = 0;
	bool f;
	for(int i = 0; i < fTestSize; ++i)
	{
		if(tmps[i] == '\n')
		{
			line.Reserve(i - sPos + 1);
			memcpy(&line[0], tmps + sPos, i - sPos);
			*(&line[0] + i - sPos) = 0;
			sPos = i + 1;
			f = false;
			for(int j = 0; j < TS_LAST; ++j)
			{
				if(!strcmp(g_szSections[j], line.trim().c_str()))
				{
					curSect = (TEST_SECTION)j;
					f = true;
					break;
				}
			}
			if(f)
			{
				continue;
			}
			line += '\n';
			switch(curSect)
			{
			case TS_TEST:
				sTEST += line;
				break;
			case TS_FILE:
				sFILE += line;
				break;
			case TS_EXPECT:
				sEXPECT += line;
				break;
			}
		}
	}

	fTest = fopen(sName.c_str(), "rb");
	if(fTest)
	{
		g_color.setColor(ANSI_RED);
		printf("\r### %s - Temp s4g file already exists!\n", file);
		fclose(fTest);
		return;
	}

	fTest = fopen(sName.c_str(), "wb");
	fwrite(sFILE.c_str(), 1, sFILE.length(), fTest);
	fclose(fTest);

	String cmd = g_sEXE + ' ' + sName;
	String out = exec(cmd.c_str()) + '\n';
	remove(sName.c_str());

	out.ReplaceAll("\r\n", "\n");
	sEXPECT.ReplaceAll("\r\n", "\n");

	if(out == sEXPECT)
	{
		g_color.setColor(ANSI_LGREEN);
		printf("\r### %s - Done!\n", file);
	}
	else
	{
		g_color.setColor(ANSI_LRED);
		printf("\r### %s - Fail!\n", file);

		char _fname[_MAX_FNAME];
		char _ext[_MAX_EXT];

		_splitpath(sName.c_str(), NULL, NULL, _fname, _ext);
		String outDir = g_sOutDir + '/' + _fname;
		if(!IsDir(outDir.c_str()))
		{
			_mkdir(outDir.c_str());
		}
		outDir += String('/') + _fname;
		//WriteFile((outDir + ".s4gt").c_str(), tmps, fTestSize);
		WriteFile((outDir + ".s4g").c_str(), sFILE.c_str(), sFILE.length());
		WriteFile((outDir + ".expect").c_str(), sEXPECT.c_str(), sEXPECT.length());
		WriteFile((outDir + ".out").c_str(), out.c_str(), out.length());

		String bat = String("@echo off\n") +
			"\"" + g_sTEXE + "\" -t \"" + file + "\" -e \"" + g_sEXE + "\" -o \"" + g_sOutDir + "\"\n";
		WriteFile((outDir + ".bat").c_str(), bat.c_str(), bat.length());
	}
}

bool WriteFile(const char * file, const char * data, size_t len)
{
	FILE * fTest = fopen(file, "wb");
	if(!fTest)
	{
		return(false);
	}
	fwrite(data, 1, len, fTest);
	fclose(fTest);
	return(true);
}

bool IsDir(const char * path)
{
	DWORD attr = GetFileAttributesA(path);
	if(attr == INVALID_FILE_ATTRIBUTES)
	{
		return(false);
	}

	return(attr & FILE_ATTRIBUTE_DIRECTORY);
}

String exec(const char* cmd)
{
	char buffer[128];
	String result = "";
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	if(pipe)
	{
		while(!feof(pipe.get()))
		{
			if(fgets(buffer, 128, pipe.get()))
			{
				result += buffer;
			}
		}
	}
	return result;
}