#include "FileSystem.h"
#include <shellapi.h>
#include <ShlObj.h>

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
    m_filePaths.push_back(String(szPath));
    m_priority.push_back(iPriority);
    return m_filePaths.size() - 1;
}

UINT inline CFileSystem::getRootCount()
{
    return m_filePaths.size();
}

const char *CFileSystem::getRoot(UINT id)
{
    FILEID_CHECKED(m_filePaths.size());

    return m_filePaths[id].c_str();
}

void CFileSystem::setWritableRoot(UINT id)
{
    FILEID_CHECKED(m_filePaths.size());

    m_writableRoot = id;
}

bool CFileSystem::resolvePath(const char *szPath, char *szOut, int iOutMax)
{
    assert(!"No Implementation");
    return false;
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

IFileSystem::IFileIterator *CFileSystem::getFileList(const char *szPath, FILE_TYPE type)
{
    assert(!"No Implementation");
    return nullptr;
}

IFileSystem::IFileIterator *CFileSystem::getFileListRecursive(const char *szPath, FILE_TYPE type, const char *szExt = 0)
{
    assert(!"No Implementation");
    return nullptr;
}

IFileSystem::IFileIterator *CFileSystem::getFileListRecursive(const char *szPath, FILE_TYPE type, const char **szExts)
{
    assert(!"No Implementation");
    return nullptr;
}

bool CFileSystem::createDirectory(const char *szPath)
{
    // Если вернуло не 0, то все плохо
    return SHCreateDirectoryEx(nullptr, szPath, nullptr) == ERROR_SUCCESS;
}

bool CFileSystem::deleteDirectory(const char *szPath)
{
    SHFILEOPSTRUCT file_op = {
        NULL,
        FO_DELETE,
        szPath,
        "",
        FOF_NOCONFIRMATION |
        FOF_NOERRORUI |
        FOF_SILENT,
        false,
        0,
        "" };

    // Если вернуло не 0, то все плохо
    return SHFileOperation(&file_op) == 0;
}

IFile *CFileSystem::openFileText(const char *szPath, FILE_OPEN_MODE mode = FILE_MODE_READ)
{
    assert(!"No Implementation");
    return nullptr;
}

IFile *CFileSystem::openFileBin(const char *szPath, FILE_OPEN_MODE mode = FILE_MODE_READ)
{
    assert(!"No Implementation");
    return nullptr;
}
