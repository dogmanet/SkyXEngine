#include "FileSystem.h"

time_t FileSystem::convertFiletimeToTime_t(const FILETIME& ft)
{
	ULARGE_INTEGER ull;
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

HANDLE FileSystem::GetFileHandle(const char *szPath)
{
	return CreateFile(szPath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
}

bool FileSystem::fileExists(const char *szPath)
{
	HANDLE hFile = GetFileHandle(szPath);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}

	//Если файл существует то hFile != INVALID_HANDLE_VALUE
	return hFile != INVALID_HANDLE_VALUE;
}

size_t FileSystem::fileGetSize(const char *szPath)
{
	HANDLE hFile = GetFileHandle(szPath);

	//Возможно, необходимо будет расширение для больших файлов
	size_t dwSize = GetFileSize(hFile, nullptr);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}

	return dwSize;
}

bool FileSystem::isFile(const char *szPath)
{
	DWORD flag = GetFileAttributes(szPath);

	//Если не существует или указанный путь ведет к каталогу
	if (flag == INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
	{
			return false;
	}

	return true;
}

bool FileSystem::isDirectory(const char *szPath)
{
	DWORD flag = GetFileAttributes(szPath);

	//Если не существует или указанный путь ведет не к каталогу
	return flag & FILE_ATTRIBUTE_DIRECTORY;
}

time_t FileSystem::fileGetModifyTime(const char *szPath)
{
	FILETIME mTime;

	HANDLE hFile = GetFileHandle(szPath);

	GetFileTime(hFile, nullptr, &mTime, nullptr);

	return convertFiletimeToTime_t(mTime);
}
