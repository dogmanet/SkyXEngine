#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <xcommon/IFileSystem.h>
#include <cassert>

#define CLOSE_HANDLE(handle) if (handle != INVALID_HANDLE_VALUE) \
{\
    CloseHandle(handle); \
    handle = nullptr; \
}

#define FIND_CLOSE(handle) if (handle != INVALID_HANDLE_VALUE) \
{\
    FindClose(handle); \
    handle = nullptr; \
}

//! Сравнение длинны массива и буфера записи
#define CHECK_SIZE(len, buffSize) \
{ \
    if (len > buffSize) \
    { \
        return false; \
    } \
}

//! Проверка существования пути с таким ID 
#define FILEID_CHECKED(size) if (size - 1 < id) \
{\
    assert(size - 1 < id && "The path ID you entered does not exist"); \
}

class CFileSystem final : public IFileSystem
{
public:
    CFileSystem();

	UINT addRoot(const char *szPath, int iPriority = -1) override;

    UINT getRootCount() override;

    const char *getRoot(UINT id) override;

    void setWritableRoot(UINT id) override;

    bool resolvePath(const char *szPath, char *szOut, int iOutMax) override;

    bool fileExists(const char *szPath) override;

    size_t fileGetSize(const char *szPath) override;

    bool isFile(const char *szPath) override;

    bool isDirectory(const char *szPath) override;

    //! Данная функция возвращает время в UTC (Coordinated Universal Time)
    time_t getFileModifyTime(const char *szPath) override;

    IFileIterator *getFolderList(const char *szPath) override;

    IFileIterator *getFileList(const char *szPath, const char *szExt = 0) override;

    IFileIterator *getFileList(const char *szPath, const char **szExts, int extsCount) override;

    //! No implementation
     IFileIterator *getFileListRecursive(const char *szPath,  const char *szExt)  override;
    
    //! No implementation
     IFileIterator *getFileListRecursive(const char *szPath, const char **szExts, int extsCount) override;

     bool createDirectory(const char *szPath) override;

     bool deleteDirectory(const char *szPath) override;

     IFile *openFile(const char *szPath, FILE_OPEN_MODE) override;

private:
    //! Возвращает абсолютный канонизированный путь
    char *getAbsoliteCanonizePath(const char *szPath);

    char *getFullPathToBuild();

    String *getFileName(const char *name);

    //! Вспомогательная функция для конвертирования FILETIME в time_t
    time_t convertFiletimeToTime_t(const FILETIME& ft);

    HANDLE getFileHandle(const char *szPath);

    bool isAbsolutePath(const char* szPath);

    String *copyFile(const char* szPath);

    //корневые пути и приоритет
    Array<String> m_filePaths;
    Array<int> m_priority;

    //Полный путь к build
    String m_pathToBuild;

    //!Наш текущий ID корневого пути для записи
    //! -1 - значит не установлен
    int m_writableRoot = -1;
};

#endif
