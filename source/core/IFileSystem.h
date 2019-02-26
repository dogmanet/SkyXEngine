
#include <gdefines.h>
#include <common/array.h>
#include <common/string.h>

// Интерфейс для работы с файловой системой (в том числе с упакованными архивами в будущем)
// Может производить поиск по нескольким корневым путям

#define FILE_NOT_FOUND ((size_t)-1)

//! типы списков для #getFileList
enum FILE_LIST_TYPE
{
	//! файлы
	FILE_LIST_TYPE_FILES,

	//! директории
	FILE_LIST_TYPE_DIRS,

	//! все
	FILE_LIST_TYPE_ALL,
};

enum FILE_OPEN_MODE
{
	FILE_MODE_READ   = 0x00000001,
	FILE_MODE_WRITE  = 0x00000002,
	FILE_MODE_APPEND = 0x00000004
};
DEFINE_ENUM_FLAG_OPERATORS(FILE_OPEN_MODE);

class IFile;
class IFileSystem: public IBaseObject
{
public:
	/*! Добавить корневой путь (позволяется как абсолютнй путь, так и относительно каталога build)
	 Так же, принимает путь к упакованному архиву с префиксом '@', например "@c:/engine/build/textures.ext" или "@textures.ext" (относительно build)
	*/
	virtual UINT addRoot(const char *szPath, int iPriority = -1) = 0;
	virtual UINT getRootCount() = 0;
	virtual const char *getRoot(UINT id);
	/*! Помечает корневой путь путем для записи. По этому пути будут осуществляться все операции создания файлов
		Путь для записи может быть только один, если явно не задан - используется первый добавленный путь, не являющийся архивом
	*/
	virtual void setWritableRoot(UINT id) = 0;

	//! Проверяет наличие файла или каталога по указанному пути
	virtual bool fileExists(const char *szPath) = 0;
	//! Получает размер файла в байтах, либо FILE_NOT_FOUND в случае, если файл не существует, либо не является файлом
	virtual size_t fileGetSize(const char *szPath) = 0;
	//! Проверяет, что путь существует, и является файлом
	virtual bool isFile(const char *szPath) = 0;
	//! Проверяет, что путь существует, и является каталогом
	virtual bool isDirectory(const char *szPath) = 0;
	//! возвращает время последнего изменения файла
	virtual time_t fileGetModifyTime(const char *szPath) = 0;

	/*! возвращает массив со всеми данными находящимися по пути szPath,
	szPath может быть не канонизирован и необязательно должен заканчиваться слэшем,
	szPath уже может содержать фильтр (к примеру C:/*), но если его нет, тогда функция подставит сама
	*/
	virtual Array<String> getFileList(const char *szPath, FILE_LIST_TYPE type) = 0;

	/*! возвращает список всех файлов или папок (в зависимости от type),
	szPath не должен содержать фильтров, может быть не канонизирован и без последнего слэша,
	szExt - расширение файла без точки
	*/
	virtual Array<String> getFileListRecursive(const char *szPath, FILE_LIST_TYPE type, const char *szExt = 0) = 0;
	//! То же, что предыдущая, только позволяет использовать массив расширений для поиска. Последний элемент массива NULL
	virtual Array<String> getFileListRecursive(const char *szPath, FILE_LIST_TYPE type, const char **szExts) = 0;

	//! Создает директорию по указанному пути, рекурсивно
	virtual bool createDirectory(const char *szPath) = 0;

	//! Открыть файл. При открытии с возможностью записи файла, находящегося вне записываемого корня, файл копируется в записывающийся корень и открывается копия.
	virtual IFile *openFileText(const char *szPath, FILE_OPEN_MODE mode = FILE_MODE_READ) = 0;
	virtual IFile *openFileBin(const char *szPath, FILE_OPEN_MODE mode = FILE_MODE_READ) = 0;
};
