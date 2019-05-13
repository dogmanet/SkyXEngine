#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <xcommon/IFileSystem.h>
#include <cassert>

#define CLOSE_HANDLE(handle) if (handle != INVALID_HANDLE_VALUE) \
{\
    CloseHandle(handle); \
}

//! Проверка существования пути с таким ID 
#define FILEID_CHECKED(size) if (size - 1 < id) \
{\
    assert(size - 1 < id && "The path ID you entered does not exist"); \
}

class CFileSystem final : public IFileSystem
{
private:
    //! Вспомогательная функция для конвертирования FILETIME в time_t
    time_t convertFiletimeToTime_t(const FILETIME& ft);

    HANDLE getFileHandle(const char *szPath);

    //корневые пути и приоритет
    Array<String> m_filePaths;
    Array<int> m_priority;

    //!Наш текущий ID корневого пути
    UINT m_writableRoot = 0;

public:
    UINT addRoot(const char *szPath, int iPriority) override;

    UINT inline getRootCount() override;

    const char *getRoot(UINT id) override;

    void setWritableRoot(UINT id) override;

    //! No implementation
    bool resolvePath(const char *szPath, char *szOut, int iOutMax) override;

    bool fileExists(const char *szPath) override;

    size_t fileGetSize(const char *szPath) override;

    bool isFile(const char *szPath) override;

    bool isDirectory(const char *szPath) override;

    //! Данная функция возвращает время в UTC (Coordinated Universal Time)
    time_t getFileModifyTime(const char *szPath) override;

    //! No implementation
    IFileIterator *getFileList(const char *szPath, FILE_TYPE type) override;

    //! No implementation
     IFileIterator *getFileListRecursive(const char *szPath, FILE_TYPE type, const char *szExt)  override;
    
    //! No implementation
     IFileIterator *getFileListRecursive(const char *szPath, FILE_TYPE type, const char **szExts) override;

     //! No implementation
     bool createDirectory(const char *szPath) override;

     //! No implementation
     bool deleteDirectory(const char *szPath) override;

     //! No implementation
     IFile *openFileText(const char *szPath, FILE_OPEN_MODE) override;

     //! No implementation
     IFile *openFileBin(const char *szPath, FILE_OPEN_MODE) override;
};

#endif
