#include "FileSystem.h"

time_t CFileSystem::convertFiletimeToTime_t(const FILETIME& ft)
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

HANDLE CFileSystem::getFileHandle(const char *szPath)
{
	return CreateFile(szPath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
}

UINT CFileSystem::addRoot(const char *szPath, int iPriority = -1)
{
    filePaths.push_back(String(szPath));
    priority.push_back(iPriority);
    return filePaths.size() - 1;
}

UINT CFileSystem::getRootCount()
{
    return filePaths.size();
}

bool CFileSystem::fileExists(const char *szPath)
{
	HANDLE hFile = getFileHandle(szPath);

	CLOSE_HANDLE(hFile);

	//Если файл существует то hFile != INVALID_HANDLE_VALUE
	return hFile != INVALID_HANDLE_VALUE;
}

size_t CFileSystem::fileGetSize(const char *szPath)
{
	WIN32_FILE_ATTRIBUTE_DATA lpFileInformation;

	int result = GetFileAttributesEx(szPath, GetFileExInfoStandard, &lpFileInformation);

	//Преобразование размера из старших и младших бит
	ULONGLONG FileSize = (static_cast<ULONGLONG>(lpFileInformation.nFileSizeHigh) <<
		sizeof(lpFileInformation.nFileSizeLow) * sizeof(ULONGLONG)) |
		lpFileInformation.nFileSizeLow;

	//Если result != 0 то все хорошо, если 0 то файл не найден
	return result != 0 ? FileSize : FILE_NOT_FOUND;
}

bool CFileSystem::isFile(const char *szPath)
{
	DWORD flag = GetFileAttributes(szPath);

	//Если не существует или указанный путь ведет к каталогу
	if (flag == INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
	{
		return false;
	}

	return true;
}

bool CFileSystem::isDirectory(const char *szPath)
{
	DWORD flag = GetFileAttributes(szPath);

	//Если не существует или указанный путь ведет не к каталогу
	return flag & FILE_ATTRIBUTE_DIRECTORY;
}

time_t CFileSystem::getFileModifyTime(const char *szPath)
{
	FILETIME mTime;

	HANDLE hFile = getFileHandle(szPath);

	GetFileTime(hFile, nullptr, &mTime, nullptr);

	CLOSE_HANDLE(hFile);

	return convertFiletimeToTime_t(mTime);
}
