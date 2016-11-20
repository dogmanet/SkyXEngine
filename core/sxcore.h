
#ifndef __sxcore
#define __sxcore

#include <fstream>
#include <gdefines.h>

//тип функции для обработки в менеджере задач
typedef void(*THREAD_UPDATE_FUNCTION)();

enum CoreTaskFlag
{
	CoreTF_NONE = 0x0,

	CoreTF_REPEATING = 0x1 << 0, //< Задача будет повторяться, если не указано - выполнится только один раз
	CoreTF_THREADSAFE = 0x1 << 1, //< Задача может быть выполнена в любом потоке
	CoreTF_FRAME_SYNC = 0x1 << 2, //< Задаче необходима синхронизация по границе кадра
	CoreTF_ON_SYNC = 0x1 << 3, //< Это выполняется в момент синхронизации(выполняет необходимые действия для обмена данными во время синхронизации)

	CoreTF_SINGLETHREADED = CoreTF_NONE, //< Задача выполняется в главном потоке, один раз
	CoreTF_SINGLETHREADED_REPEATING = CoreTF_REPEATING, //< Задача выполняется в главном потоке, повторяется
	CoreTF_BACKGROUND = CoreTF_THREADSAFE,  //< Задача выполняется в фоне(не ожидает синхронизации) один раз
	CoreTF_BACKGROUND_REPEATING = CoreTF_THREADSAFE | CoreTF_REPEATING, //< Задача выполняется в фоне(не ожидает синхронизации), по завершении повторяется
	CoreTF_BACKGROUND_SYNC = CoreTF_THREADSAFE | CoreTF_FRAME_SYNC, //< Задача может выполняться в любом потоке, ожидает синхронизации, выполняется один раз
	CoreTF_BACKGROUND_SYNC_REPEATING = CoreTF_THREADSAFE | CoreTF_REPEATING | CoreTF_FRAME_SYNC, //< Задача может выполняться в любом потоке, ожидает синхронизации, выполняется многократно

	CoreTF_ALL = ~0x0
};

//базовые функции ядра
//только функции Core_0 могу вызываться без инициализированного ядра

SX_LIB_API long Core_0GetVersion();	//возвращает версию ядра
SX_LIB_API void Core_0Create(const char* name, bool is_unic = true); //создание нового ядра, name - имя, is_unic - должно ли имя ядра быть уникальным
SX_LIB_API void Core_Dbg_Set(report_func rf); //установка своего обработчика вывода отладочной информации
SX_LIB_API int Core_0FileExists(const char* path); //существует ли файл
SX_LIB_API char** Core_0CommandLineToArgvA(char* CmdLine,int* _argc); //возвращает массив строк с аргументами в строке CmdLine, в _argc записывает количество считанных элементов, то есть количество ключей созданного массива
SX_LIB_API int Core_0ClipBoardCopy(const char *str); //копирует память в буфер обмена

//менеджер задач
//создает потоки по количеству ядер
SX_LIB_API void Core_MTaskAdd(	//добавить задачу
								THREAD_UPDATE_FUNCTION func, //функция обработки
								DWORD flag = CoreTF_SINGLETHREADED_REPEATING); //флаг из CoreTaskFlag
SX_LIB_API void Core_MTaskStart();	//стартовать обрабатывать все задачи
SX_LIB_API void Core_MTaskStop();	//остановить все задачи

SX_LIB_API void Core_AKill(); //уничтожить ядро
SX_LIB_API void Core_AGetName(char* name); //получить имя ядра

///////
//интерфейс для записи/чтения файлов

//режим открытия
#define SXFILE_BIN 0	//двоичный
#define SXFILE_TEXT 1	//текстовый
//аргумент type - режим отрытия файла
struct ISXFile : public IBaseObject
{
	virtual ~ISXFile(){};
	virtual int Open(const char* path, int type = SXFILE_TEXT) = 0;	//открыть файл
	virtual int Create(const char* path, int type = SXFILE_TEXT) = 0;//создать файл
	virtual int Add(const char* path, int type = SXFILE_TEXT) = 0;	//добавить в конец файла
	virtual size_t ReadB(void* dest, size_t size)=0;	//считать в dest количетсво байт size
	virtual size_t WriteB(void* src, size_t size)=0;	//записать src в количетве size байт
	virtual size_t ReadT(const char* format, ...) = 0;	//чтение из файла, ... - только указатели
	virtual size_t WriteT(const char* format, ...) = 0;	//запись в файл
	virtual size_t GetSize()=0;	//получить размер файла в байтах
	virtual char ReadChar()=0;	//считать символ
	virtual size_t GetPos() = 0;//размер файла
	virtual void SetPos(size_t pos) = 0; //установить позицию
	virtual void Close()=0;		//закрыть файл
	virtual BOOL IsEOF()=0;		//текущая позиция является концом файла?
};

SX_LIB_API ISXFile* Core_CrFile(); //создать экземпляр класса ISXFile
SX_LIB_API ISXFile* Core_OpFile(const char* path, int type); //открыть файл

////////
//интерфейс для работы с файлами конфигурации (ini)
//аргумент acceptIncludes - использовать ли включения других файлов конфигов
//!!!секции и ключи хранятся в виде дерева, и нет гарантии что может быть доступ по порядковому номеру
//!!! можно получить общее количество секций/ключей, дальше плясать
struct ISXLConfig : public IBaseObject
{
	virtual ~ISXLConfig(){};
	virtual int Open(const char* path)=0;	//открыть файл
	virtual const char* GetKey(const char* section, const char* key) = 0; //получить значения ключа key который в секции section
	virtual const char* GetKeyName(const char* section, int key) = 0; //получить имя ключа по номеру
	virtual const char* GetSectionName(int num) = 0; //получить имя секци по номеру
	virtual void Set(const char* section, const char* key, const char* val)=0; //установить значение val ключа key котор в секции section
	virtual int Save()=0;	//сохранить файл
	virtual int GetSectionCount(bool acceptIncludes = false)=0;	//количество секций в файле
	virtual int GetKeyCount(bool acceptIncludes = false)=0; //общее количество ключей
	virtual int GetKeyCount(const char* section, bool acceptIncludes = false) = 0; //общее количество ключей в секции
	virtual bool SectionExists(const char* section, bool acceptIncludes = false)=0; //существует ли секция section
	virtual bool KeyExists(const char* section, const char* key, bool acceptIncludes = false)=0; //существует ли ключ key в секции section
};

SX_LIB_API ISXLConfig* Core_CrLConfig(); //создать файл экземпляр класса ISXLConfig
SX_LIB_API ISXLConfig* Core_OpLConfig(const char* path); //открыть файл конфигов

#endif