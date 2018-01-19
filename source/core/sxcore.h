
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file 
Заголовочный файл sxcore - основного ядра
*/

/*! \defgroup sxcore sxcore - основное ядро
@{
*/

#ifndef __SXCORE_H
#define __SXCORE_H

#include <fstream>
#include <gdefines.h>

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)

#include <io.h>
#include <fcntl.h>
#include <tlhelp32.h>

#define SM_D3D_CONVERSIONS
#include <common/sxmath.h>

//! тип функции для обработки в менеджере задач
typedef void(*THREAD_UPDATE_FUNCTION)();

//! значения определющие поведение задачи
enum CORE_TASK_FLAG
{
	CORE_TASK_FLAG_NONE			= 0x0,

	CORE_TASK_FLAG_REPEATING	= 0x1 << 0,	//!< Задача будет повторяться, если не указано - выполнится только один раз
	CORE_TASK_FLAG_THREADSAFE	= 0x1 << 1,	//!< Задача может быть выполнена в любом потоке
	CORE_TASK_FLAG_FRAME_SYNC	= 0x1 << 2,	//!< Задаче необходима синхронизация по границе кадра
	CORE_TASK_FLAG_ON_SYNC		= 0x1 << 3,	//!< Это выполняется в момент синхронизации(выполняет необходимые действия для обмена данными во время синхронизации)

	CORE_TASK_FLAG_SINGLETHREADED = CORE_TASK_FLAG_NONE,				//!< Задача выполняется в главном потоке, один раз
	CORE_TASK_FLAG_SINGLETHREADED_REPEATING = CORE_TASK_FLAG_REPEATING, //!< Задача выполняется в главном потоке, повторяется
	CORE_TASK_FLAG_BACKGROUND = CORE_TASK_FLAG_THREADSAFE,				//!< Задача выполняется в фоне(не ожидает синхронизации) один раз
	CORE_TASK_FLAG_BACKGROUND_REPEATING = CORE_TASK_FLAG_THREADSAFE | CORE_TASK_FLAG_REPEATING,		//!< Задача выполняется в фоне(не ожидает синхронизации), по завершении повторяется
	CORE_TASK_FLAG_BACKGROUND_SYNC = CORE_TASK_FLAG_THREADSAFE | CORE_TASK_FLAG_FRAME_SYNC,			//!< Задача может выполняться в любом потоке, ожидает синхронизации, выполняется один раз
	CORE_TASK_FLAG_BACKGROUND_SYNC_REPEATING = CORE_TASK_FLAG_THREADSAFE | CORE_TASK_FLAG_REPEATING | CORE_TASK_FLAG_FRAME_SYNC,	//!< Задача может выполняться в любом потоке, ожидает синхронизации, выполняется многократно

	CORE_TASK_FLAG_ALL = ~0x0
};

/*! \name Базовые функции ядра 
!@{*/

//! возвращает версию ядра
SX_LIB_API long Core_0GetVersion();	

//! создание нового ядра, name - имя, is_unic - должно ли имя ядра быть уникальным
SX_LIB_API void Core_0Create(const char *szName, bool isUnic = true); 

//! установка своего обработчика вывода отладочной информации
SX_LIB_API void Core_Dbg_Set(report_func rf); 

//! существует ли файл
SX_LIB_API bool Core_0FileExists(const char *szPath);

//! возвращает время последнего изменения файла
SX_LIB_API UINT Core_0GetTimeLastModify(const char *szPath);

//! копирует строку в буфер обмена
SX_LIB_API bool Core_0ClipBoardCopy(const char *szStr);		

//! запущен ли процесс
SX_LIB_API bool Core_0IsProcessRun(const char *szProcess);	

//! уничтожить ядро
SX_LIB_API void Core_AKill(); 

//! получить имя ядра
SX_LIB_API void Core_AGetName(char *szName); 

//!@}

//##########################################################################

/*! @name Менеджер задач 
 создает потоки по количеству ядер
@{*/

//! добавить задачу
SX_LIB_API void Core_MTaskAdd(	
								THREAD_UPDATE_FUNCTION func, //!< функция обработки
								DWORD flag = CORE_TASK_FLAG_SINGLETHREADED_REPEATING //!< флаг из #CoreTaskFlag 
								); 

//! стартовать обрабатывать все задачи
SX_LIB_API void Core_MTaskStart();	

//! остановить все задачи
SX_LIB_API void Core_MTaskStop();	

//! @}

//##########################################################################

/*! @name Регистры
Массивы данных, определенных типов, доступные в любой библиотеке включающей в себя ядро
@{*/

/*! размер массива регистров */
#define CORE_REGISTRY_SIZE 64	

//! установка значения в регистр bool типа
SX_LIB_API void Core_RBoolSet(int id, bool val);	

//! получение значения из регистра bool типа
SX_LIB_API bool Core_RBoolGet(int id);		


//! установка значения в регистр int32_t типа
SX_LIB_API void Core_RIntSet(int id, int32_t val);	

//! получение значения из регистра int32_t типа
SX_LIB_API int32_t Core_RIntGet(int id);			


//! установка значения в регистр float32_t типа
SX_LIB_API void Core_RFloatSet(int id, float32_t val);	

//! получение значения из регистра float32_t типа
SX_LIB_API float32_t Core_RFloatGet(int id);			


//! установка значения в регистр float4x4 типа
SX_LIB_API void Core_RMatrixSet(int id, float4x4* val);	

//! получение значения из регистра float4x4 типа
SX_LIB_API void Core_RMatrixGet(int id, float4x4* val);	


//! установка значения в регистр float3 типа
SX_LIB_API void Core_RFloat3Set(int id, float3* val);	

//! получение значения из регистра float3 типа
SX_LIB_API void Core_RFloat3Get(int id, float3* val);	


//! установка значения в регистр string типа
SX_LIB_API void Core_RStringSet(int id, const char *val);	

//! получение значения из регистра string типа
SX_LIB_API const char* Core_RStringGet(int id);				

//! @}

//##########################################################################

/*! \name Таймеры
 \note Для Unix даты рекомендуется использовать функции:
  - localtime или localtime_s (из структуры tm в общее количество секунд), 
  - mktime (из общего количества секунд в структуру tm).
 \note По умолчанию Unix дата используется time(NULL) в момент создания таймера
 \note Все возвращаемое время вычисляется в соответсвии с коэфициентом скорости
 \note Функции возвращающие время без постфикса U (update) требуют обновления, которое происходит в функции #Core_TimesUpdate
@{
*/

//! добавление таймера
SX_LIB_API ID Core_TimeAdd();		

//! обновление всех таймеров
SX_LIB_API void Core_TimesUpdate();	

//! установка скорости течения времени
SX_LIB_API void Core_TimeSpeedSet(ID id, float fSpeed);	

//!< возвращает скоротечность времени для таймера
SX_LIB_API float Core_TimeSpeedGet(ID id);				


//! установка состояния запуска таймера
SX_LIB_API void Core_TimeWorkingSet(ID id, bool isWorking);	

//! запущен ли таймер
SX_LIB_API bool Core_TimeWorkingGet(ID id);					


//! установить стартовую дату в Unix в секундах
SX_LIB_API void Core_TimeUnixStartSet(ID id, int64_t iStartTime);	

//! возвращает стартовую дату в Unix в секундах
SX_LIB_API int64_t Core_TimeUnixStartGet(ID id);					

//! возвращает текущюю дату в Unix в секундах
SX_LIB_API int64_t Core_TimeUnixCurrGet(ID id);						


//! возвращает общее время работы таймера в микросекундах (требует обновления)
SX_LIB_API int64_t Core_TimeTotalMcsGet(ID id);						


//! возвращает общее время работы таймера в миллисекундах (требует обновления)
#define Core_TimeTotalMlsGet(id) Core_TimeTotalMcsGet(id)/1000		


//!< возвращает общее время работы таймера в миллисекундах (требует обновления)
#define TimeGetMls(id) Core_TimeTotalMlsGet(id)						

//! возвращает общее время работы таймера в микросекундах (требует обновления)
#define TimeGetMcs Core_TimeTotalMcsGet								


//! возвращает общее время работы таймера в микросекундах (независимо от обновления)
SX_LIB_API int64_t Core_TimeTotalMcsGetU(ID id);					


//! возвращает общее время работы таймера в микросекундах (независимо от обновления)
#define Core_TimeTotalMlsGetU(id) Core_TimeTotalMcsGetU(id)/1000	

//! возвращает общее время работы таймера в миллисекундах (независимо от обновления)
#define TimeGetMlsU(id) Core_TimeTotalMlsGetU(id)					

//! возвращает общее время работы таймера в микросекундах (независимо от обновления)
#define TimeGetMcsU Core_TimeTotalMcsGetU							

//!@}

//##########################################################################

/*! \name Режимы открытия файлов 
@{*/

/*! двоичный */
#define CORE_FILE_BIN	0	

/*! текстовый */
#define CORE_FILE_TEXT	1	

//!@}

/*! конец файла */
#define CORE_FILE_EOF	EOF	

/*! Интерфейс для записи/чтения файлов
 \note аргумент iType - режим отрытия файла
*/
struct IFile : public IBaseObject
{
	virtual ~IFile(){};
	
	virtual int open(const char *szPath, int iType = CORE_FILE_TEXT) = 0;	//!< открыть файл
	virtual int create(const char *szPath, int iType = CORE_FILE_TEXT) = 0;	//!< создать файл
	virtual int add(const char *szPath, int iType = CORE_FILE_TEXT) = 0;	//!< добавить в конец файла
	virtual size_t readBin(void *pDest, size_t iSize) = 0;					//!< считать в dest количетсво байт size
	virtual size_t writeBin(const void *pSrc, size_t iSize) = 0;			//!< записать src в количетве size байт
	virtual size_t readText(const char *szFormat, ...) = 0;					//!< чтение из файла, в аргументы только указатели
	virtual size_t writeText(const char *szFormat, ...) = 0;				//!< запись в файл
	virtual size_t getSize() const = 0;		//!< получить размер файла в байтах
	virtual int readChar() = 0;				//!< считать символ
	virtual size_t getPos() const = 0;		//!< текущая позиция курсора в файле
	virtual void setPos(size_t iPos) = 0;	//!< установить позицию
	virtual void close() = 0;				//!< закрыть файл
	virtual bool isEOF() const = 0;			//!< текущая позиция является концом файла?
};

/*! \name Создание экземпляров файлов 
!@{*/

//! создать экземпляр класса IFile
SX_LIB_API IFile* Core_CrFile(); 

//! открыть файл
SX_LIB_API IFile* Core_OpFile(const char* szPath, int iType); 

//!@}

//##########################################################################

/*! интерфейс для работы с файлами конфигурации (ini) 
 \warning секции и ключи хранятся в виде дерева, и нет гарантии что может быть доступ по порядковому номеру, 
можно получить общее количество секций/ключей, дальше плясать */
struct ISXConfig : public IBaseObject
{
	virtual ~ISXConfig(){};
	virtual void New(const char *path) = 0;	//!< новый файл
	virtual int open(const char *path) = 0;	//!< открыть файл
	virtual const char* getKey(const char *section, const char *key) = 0;	//!< получить значения ключа key который в секции section
	virtual const char* getKeyName(const char *section, int key) = 0;		//!< получить имя ключа по номеру
	virtual const char* getSectionName(int num) = 0;						//!< получить имя секции по номеру
	virtual void set(const char* section, const char *key, const char * val)=0;	//!< установить значение val ключа key котор в секции section
	virtual int save()=0;				//!< сохранить файл
	virtual int getSectionCount()=0;	//!< количество секций в файле
	virtual int getKeyCount()=0;		//!< общее количество ключей
	virtual int getKeyCount(const char *section) = 0;				//!< общее количество ключей в секции
	virtual bool sectionExists(const char *section) = 0;			//!< существует ли секция section
	virtual bool keyExists(const char *section, const char *key) = 0;	//!< существует ли ключ key в секции section
};

/*! \name Создание экземпляров конфигурацииных файлов 
@{*/

//! создать файл экземпляр класса ISXLConfig
SX_LIB_API ISXConfig* Core_CrConfig(); 

//! открыть файл конфигов
SX_LIB_API ISXConfig* Core_OpConfig(const char* path); 

//!@}

/*! \name Работа с консолью 
!@{*/
typedef void(*SXCONCMD)(); /*!< Тип функции для регистрации команды без аргументов */
typedef void(*SXCONCMDARG)(int argc, const char ** argv); /*!< Тип функции для регистрации команды с аргументами */

class ConCmdStub{}; /*!< Класс-заглушка, для определения следующих типов */
typedef void(ConCmdStub::* SXCONCMDCLS)(); /*!< Тип метода для регистрации команды-члена класса без аргументов */
typedef void(ConCmdStub::* SXCONCMDCLSARG)(int argc, const char ** argv); /*!< Тип метода для регистрации команды-члена класса с аргументами */

SX_LIB_API void Core_0RegisterConcmd(char * name, SXCONCMD cmd, const char * desc = NULL); //!< Регистрация консольной функции без аргументов
SX_LIB_API void Core_0RegisterConcmdArg(char * name, SXCONCMDARG cmd, const char * desc = NULL); //!< Регистрация консольной функции с аргументами
SX_LIB_API void Core_0RegisterConcmdCls(char * name, void * pObject, SXCONCMDCLS cmd, const char * desc = NULL); //!< Регистрация консольной функции-члена класса без аргументов
SX_LIB_API void Core_0RegisterConcmdClsArg(char * name, void * pObject, SXCONCMDCLSARG cmd, const char * desc = NULL); //!< Регистрация консольной функции-члена класса с аргументами

SX_LIB_API void Core_0ConsoleUpdate(); //!< Обновление консоли, выполнение буфера команд
SX_LIB_API void Core_0ConsoleExecCmd(const char * format, ...); //!< Добавление команды на исполнение в буфер команд

SX_LIB_API UINT_PTR Core_ConsoleGetOutHandler();

/*! Устанавливает поток вывода. Для работы консоли
	\warning Должна быть инлайнова, чтобы выполняться в контексте вызывающей библиотеки
*/
__inline void Core_SetOutPtr()
{
	UINT_PTR sock = Core_ConsoleGetOutHandler();
	if(sock == ~0)
	{
		return;
	}
	int hOut = _open_osfhandle(sock, O_RDONLY | O_RDWR | O_WRONLY | _O_APPEND);
	FILE * fOut = ::_fdopen(hOut, "a+");
	::setvbuf(fOut, NULL, _IONBF, 0);

	*stdout = *fOut;
	*stderr = *fOut;

	fOut->_file = 1;
}

//!@}

/*! \name Работа с реестром конфигурации (cvars) 
!@{*/

//! Флаги кваров
enum CVAR_FLAG
{
	FCVAR_NONE     = 0x00, //!< нет
	FCVAR_CHEAT    = 0x01, //!< Изменение этой переменной с дефолтного значения разрешено только в режиме разработки
	FCVAR_READONLY = 0x02  //!< Только для чтения
};

//! Регистрирует строковую переменную
SX_LIB_API void Core_0RegisterCVarString(
	const char * name, //!< Имя квара
	const char * value,  //!< значение по умолчанию
	const char * desc=NULL, //!< краткое описание
	int flags = 0 //!< флаги из CVAR_FLAG
	);

//! Регистрирует целую переменную
SX_LIB_API void Core_0RegisterCVarInt(
	const char * name, //!< Имя квара
	int value,  //!< значение по умолчанию
	const char * desc = NULL, //!< краткое описание
	int flags = 0 //!< флаги из CVAR_FLAG
	);

//! Регистрирует дробную переменную
SX_LIB_API void Core_0RegisterCVarFloat(
	const char * name, //!< Имя квара
	float value,  //!< значение по умолчанию
	const char * desc = NULL, //!< краткое описание
	int flags = 0 //!< флаги из CVAR_FLAG
	);

//! Регистрирует логическую переменную
SX_LIB_API void Core_0RegisterCVarBool(
	const char * name, //!< Имя квара
	bool value,  //!< значение по умолчанию
	const char * desc = NULL, //!< краткое описание
	int flags = 0 //!< флаги из CVAR_FLAG
	);

//! Регистрирует указатель
SX_LIB_API void Core_0RegisterCVarPointer(
	const char * name, //!< Имя квара
	UINT_PTR value  //!< значение по умолчанию
	);

//! Получает указатель на значение строкового квара. При отсутствии квара запрошенного типа возвращает NULL
SX_LIB_API const char ** Core_0GetPCVarString(const char * name);
#define GET_PCVAR_STRING(k) Core_0GetPCVarString(k);

//! Получает указатель на значение целочисленного квара. При отсутствии квара запрошенного типа возвращает NULL
SX_LIB_API const int * Core_0GetPCVarInt(const char * name);
#define GET_PCVAR_INT(k) Core_0GetPCVarInt(k);

//! Получает указатель на значение дробного квара. При отсутствии квара запрошенного типа возвращает NULL
SX_LIB_API const float * Core_0GetPCVarFloat(const char * name);
#define GET_PCVAR_FLOAT(k) Core_0GetPCVarFloat(k);

//! Получает указатель на значение логического квара. При отсутствии квара запрошенного типа возвращает NULL
SX_LIB_API const bool * Core_0GetPCVarBool(const char * name);
#define GET_PCVAR_BOOL(k) Core_0GetPCVarBool(k);

//! Получает указатель по имени. При отсутствии квара запрошенного типа возвращает NULL
SX_LIB_API UINT_PTR * Core_0GetPCVarPointer(const char * name);
#define GET_PCVAR_POINTER(k) Core_0GetPCVarPointer(k);

//! Устанавливает новое значение квара. Должен существовать
SX_LIB_API void Core_0SetCVarString(const char * name, const char * value);

//! Устанавливает новое значение квара. Должен существовать
SX_LIB_API void Core_0SetCVarInt(const char * name, int value);

//! Устанавливает новое значение квара. Должен существовать
SX_LIB_API void Core_0SetCVarFloat(const char * name, float value);

//! Устанавливает новое значение квара. Должен существовать
SX_LIB_API void Core_0SetCVarBool(const char * name, bool value);


//!@}

#endif

/*! @} */
