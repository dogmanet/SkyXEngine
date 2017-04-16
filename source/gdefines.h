#ifndef gdefines_H
#define gdefines_H
//макрос экспорта/ипорта из библиотек
#if !defined(SX_LIB_API)
	#if defined(SX_EXE)
		#define SX_LIB_API extern "C" __declspec (dllimport)
	#elif defined(SX_DLL)
		#define SX_LIB_API extern "C" __declspec (dllexport)
	#else
		#define SX_LIB_API 
	#endif
#endif

#define CORE_NAME_MAX_LEN 32	//максимальная длина имени объекта ядра/подсистемы
#define OBJECT_NAME_MAX_LEN 64	//максимальная длина имени объекта
#define CONFIG_SECTION_MAX_LEN 64	//максимальная длина секции конфигурационного файла

//for warning C4003: not enough actual parameters for macro
#define _VOID

#ifndef IFACEBASEOBJECT
#define IFACEBASEOBJECT

//базовый интерфейс, от него происходит наследование во всех интерфейсах
//дял удаления объекта вызвать Release, после сего объект будет уничтожен, но адрес не обнулится
//для удаления и обнуления использовать mem_release_delete
struct IBaseObject 
{
	virtual ~IBaseObject(){};
	virtual void Release()=0;
};

#endif

#include <common/sxtypes.h>

#define format_str(buf,format) va_list va; va_start(va, format); vsprintf_s(buf, sizeof(buf), format, va); va_end(va);

//формирвоание строки вида файл:номер строки, для вставки использовать gen_msg_location
#define macro_text_(x) #x
#define macro_text(x) macro_text_(x)
#define gen_msg_location __FILE__ ## ": " ## macro_text(__LINE__)


#define _assert_s(str,...) AllocConsole();freopen("CONOUT$", "wt", stdout); fprintf(stdout, str, ...); exit(1);
#define _assert(expr) if(!expr) _assert_s(#expr)

//тип функции вывода отладочной информации
typedef void (*report_func) (int level,const char* format,...);

#include <cstdio> 
#if defined(_WINDOWS)
#	include <Windows.h>
#endif

//уровни критичности сообщений для функции репортов
#define REPORT_MSG_LEVEL_NOTICE 0
#define REPORT_MSG_LEVEL_WARRNING 1
#define REPORT_MSG_LEVEL_ERROR -1

#define REPORT_MSG_MAX_LEN 4096

#ifndef DEFAULT_FUNCTION_REPORT 
#define DEFAULT_FUNCTION_REPORT

//дефолтовая функция вывода отладочной информации
//!!!ВМЕСТО НЕЕ В ЯДРО/ПОДСИСТЕМУ НУЖНО ОТПРАВЛЯТЬ СВОЮ
inline void def_report(int level, const char* format, ...)
{
#if defined(_WINDOWS)
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
#endif
	char buf[4096];
	int strl = sizeof(buf);
	format_str(buf, format);
	fprintf(stdout, "!!! report function is not init !!! %s\n", buf);
	fprintf(stdout, "work program will be stopped within 5 seconds ...");
	Sleep(5000);
	exit(1);
}

//парсинг строки (имени файла) str на имя папки path и имя файла name
//str = test_file_1.ex - path = test, name = file_1.ex
inline bool ParseStrPathName(const char* str, char* path, char* name)
{
	bool IsTruePath = false;
	//обрезаем имя текстуры и папку
	for (DWORD i = 0; i<strlen(str); i++)
	{
		if (str[i] == '_')
		{
			memcpy(path, str, sizeof(char)*i);
			path[i] = 0;
			sprintf(name, "%s", str + i + 1);
			IsTruePath = true;
			break;
		}
	}

	return IsTruePath;
}

#endif
#endif
