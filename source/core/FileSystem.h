#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <xcommon/IFileSystem.h>

#define CLOSE_HANDLE(handle) if (handle != INVALID_HANDLE_VALUE) \
{\
	CloseHandle(handle);\
}

class СFileSystem final : public IFileSystem
{
private:
	//! Вспомогательная функция для конвертирования FILETIME в time_t
	time_t convertFiletimeToTime_t(const FILETIME& ft);

	HANDLE getFileHandle(const char *szPath);

    //корневые пути и приоритет
    Array<String> filePaths;
    Array<int> priority;

public:
	UINT addRoot(const char *szPath, int iPriority) override;

    UINT getRootCount() override;

	bool fileExists(const char *szPath) override;

	size_t fileGetSize(const char *szPath) override;

    bool isFile(const char *szPath) override;

	bool isDirectory(const char *szPath) override;

	//! Данная функция возвращает время в UTC (Coordinated Universal Time)
	time_t getFileModifyTime(const char *szPath) override;
};

#endif
