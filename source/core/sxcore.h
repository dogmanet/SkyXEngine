
/*! 
\mainpage Документация SkyXEngine
Это документация API (интерфейса программирования приложения) 3D движка real-time рендера SkyXEngine
*/

/*!
\file 
Заголовочный файл sxcore - основного ядра движка SkyXEngine
*/

/*! \defgroup sxcore sxcore - основное ядро движка SkyXEngine
@{
*/

#ifndef __sxcore
#define __sxcore

#include <fstream>
#include <gdefines.h>

#define SM_D3D_CONVERSIONS
#include <common\sxmath.h>

//! тип функции для обработки в менеджере задач
typedef void(*THREAD_UPDATE_FUNCTION)();

//! значения определющие поведение задачи
enum CoreTaskFlag
{
	CoreTF_NONE = 0x0,

	CoreTF_REPEATING = 0x1 << 0,	//!< Задача будет повторяться, если не указано - выполнится только один раз
	CoreTF_THREADSAFE = 0x1 << 1,	//!< Задача может быть выполнена в любом потоке
	CoreTF_FRAME_SYNC = 0x1 << 2,	//!< Задаче необходима синхронизация по границе кадра
	CoreTF_ON_SYNC = 0x1 << 3,		//!< Это выполняется в момент синхронизации(выполняет необходимые действия для обмена данными во время синхронизации)

	CoreTF_SINGLETHREADED = CoreTF_NONE,				//!< Задача выполняется в главном потоке, один раз
	CoreTF_SINGLETHREADED_REPEATING = CoreTF_REPEATING, //!< Задача выполняется в главном потоке, повторяется
	CoreTF_BACKGROUND = CoreTF_THREADSAFE,				//!< Задача выполняется в фоне(не ожидает синхронизации) один раз
	CoreTF_BACKGROUND_REPEATING = CoreTF_THREADSAFE | CoreTF_REPEATING,		//!< Задача выполняется в фоне(не ожидает синхронизации), по завершении повторяется
	CoreTF_BACKGROUND_SYNC = CoreTF_THREADSAFE | CoreTF_FRAME_SYNC,			//!< Задача может выполняться в любом потоке, ожидает синхронизации, выполняется один раз
	CoreTF_BACKGROUND_SYNC_REPEATING = CoreTF_THREADSAFE | CoreTF_REPEATING | CoreTF_FRAME_SYNC,	//!< Задача может выполняться в любом потоке, ожидает синхронизации, выполняется многократно

	CoreTF_ALL = ~0x0
};

/*!@name Базовые функции ядра */
//!@{
SX_LIB_API long Core_0GetVersion();	//!< возвращает версию ядра
SX_LIB_API void Core_0Create(const char* name, bool is_unic = true); //!< создание нового ядра, name - имя, is_unic - должно ли имя ядра быть уникальным
SX_LIB_API void Core_Dbg_Set(report_func rf); //!< установка своего обработчика вывода отладочной информации
SX_LIB_API bool Core_0FileExists(const char* path); //!< существует ли файл
SX_LIB_API bool Core_0ClipBoardCopy(const char *str); //!< копирует строку в буфер обмена

SX_LIB_API void Core_AKill(); //!< уничтожить ядро
SX_LIB_API void Core_AGetName(char* name); ///< получить имя ядра
//!@}

/*! @name Менеджер задач 
 создает потоки по количеству ядер
*/
//! @{

//! добавить задачу
SX_LIB_API void Core_MTaskAdd(	
								THREAD_UPDATE_FUNCTION func, //!< функция обработки
								DWORD flag = CoreTF_SINGLETHREADED_REPEATING //!< флаг из #CoreTaskFlag 
								); 
SX_LIB_API void Core_MTaskStart();	//!< стартовать обрабатывать все задачи
SX_LIB_API void Core_MTaskStop();	//!< остановить все задачи
//! @}

/*! @name Регистры
создает потоки по количеству ядер
*/
//! @{

#define CORE_REGISTRY_SIZE 64	/*!< размер массива регистров */

SX_LIB_API void Core_RBoolSet(int id, bool val);	//!< установка значения в регистр bool типа
SX_LIB_API bool Core_RBoolGet(int id);				//!< получение значения из регистра bool типа

SX_LIB_API void Core_RIntSet(int id, int32_t val);	//!< установка значения в регистр int32_t типа
SX_LIB_API int32_t Core_RIntGet(int id);			//!< получение значения из регистра int32_t типа

SX_LIB_API void Core_RFloatSet(int id, float32_t val);	//!< установка значения в регистр float32_t типа
SX_LIB_API float32_t Core_RFloatGet(int id);			//!< получение значения из регистра float32_t типа

SX_LIB_API void Core_RMatrixSet(int id, float4x4* val);	//!< установка значения в регистр float4x4 типа
SX_LIB_API void Core_RMatrixGet(int id, float4x4* val);	//!< получение значения из регистра float4x4 типа

SX_LIB_API void Core_RFloat3Set(int id, float3* val);	//!< установка значения в регистр float3 типа
SX_LIB_API void Core_RFloat3Get(int id, float3* val);	//!< получение значения из регистра float3 типа

//! @}

/*! @name Режимы открытия файлов */
//! @{
#define CORE_FILE_BIN	0	/*!< двоичный */
#define CORE_FILE_TEXT	1	/*!< текстовый */
//! @}

#define CORE_FILE_EOF	EOF	/*!< конец файла */

/*! Интерфейс для записи/чтения файлов
 \note аргумент type - режим отрытия файла
*/
struct ISXFile : public IBaseObject
{
	virtual ~ISXFile(){};
	
	virtual int Open(const char* path, int type = CORE_FILE_TEXT) = 0;	//!<  открыть файл
	virtual int Create(const char* path, int type = CORE_FILE_TEXT) = 0;//!< создать файл
	virtual int Add(const char* path, int type = CORE_FILE_TEXT) = 0;	//!< добавить в конец файла
	virtual size_t ReadB(void* dest, size_t size)=0;	//!< считать в dest количетсво байт size
	virtual size_t WriteB(void* src, size_t size)=0;	//!< записать src в количетве size байт
	virtual size_t ReadT(const char* format, ...) = 0;	//!< чтение из файла, в аргументы только указатели
	virtual size_t WriteT(const char* format, ...) = 0;	//!< запись в файл
	virtual size_t GetSize()=0;	//!< получить размер файла в байтах
	virtual int ReadChar()=0;	//!< считать символ
	virtual size_t GetPos() = 0;//!< текущая позиция курсора в файле
	virtual void SetPos(size_t pos) = 0; //!< установить позицию
	virtual void Close()=0;		//!< закрыть файл
	virtual bool IsEOF()=0;		//!< текущая позиция является концом файла?
};

/*! @name Создание экземпляров файлов */
//!@{
SX_LIB_API ISXFile* Core_CrFile(); //!< создать экземпляр класса ISXFile
SX_LIB_API ISXFile* Core_OpFile(const char* path, int type); //!< открыть файл
//!@}
////////

/*! интерфейс для работы с файлами конфигурации (ini) 
 \warning секции и ключи хранятся в виде дерева, и нет гарантии что может быть доступ по порядковому номеру, 
можно получить общее количество секций/ключей, дальше плясать */
struct ISXLConfig : public IBaseObject
{
	virtual ~ISXLConfig(){};
	virtual int Open(const char* path)=0;	//!< открыть файл
	virtual const char* GetKey(const char* section, const char* key) = 0;	//!< получить значения ключа key который в секции section
	virtual const char* GetKeyName(const char* section, int key) = 0;		//!< получить имя ключа по номеру
	virtual const char* GetSectionName(int num) = 0;						//!< получить имя секции по номеру
	virtual void Set(const char* section, const char* key, const char* val)=0;	//!< установить значение val ключа key котор в секции section
	virtual int Save()=0;				//!< сохранить файл
	virtual int GetSectionCount()=0;	//!< количество секций в файле
	virtual int GetKeyCount()=0;		//!< общее количество ключей
	virtual int GetKeyCount(const char* section) = 0;				//!< общее количество ключей в секции
	virtual bool SectionExists(const char* section) = 0;			//!< существует ли секция section
	virtual bool KeyExists(const char* section, const char* key)=0;	//!< существует ли ключ key в секции section
};

/*!@name Создание экземпляров конфигурацииных файлов */
//!@{
SX_LIB_API ISXLConfig* Core_CrLConfig(); //!< создать файл экземпляр класса ISXLConfig
SX_LIB_API ISXLConfig* Core_OpLConfig(const char* path); //!< открыть файл конфигов
//!@}

#endif

/*! @} */
