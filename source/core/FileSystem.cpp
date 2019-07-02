#include "FileSystem.h"
#include "FileExtIterator.h"
#include "FileExtsIterator.h"
#include "DirIterator.h"
#include "File.h"
#include <shellapi.h>
#include <ShlObj.h>

char *CFileSystem::getFullPathToBuild()
{
    char *path = new char[MAX_PATH];

    GetModuleFileName(nullptr, path, MAX_PATH);

    char *pos = strstr(path, "build\\");

    pos[6] = '\0';

    return path;
}

String *CFileSystem::getFileName(const char *name)
{
    LPWIN32_FIND_DATAA wfd;

    HANDLE const hFind = FindFirstFile(name, wfd);

    FindClose(hFind);

    return new String(wfd->cFileName[0]);
}

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

bool CFileSystem::isAbsolutePath(const char *szPath)
{
    while (*szPath != '\0')
    {
        //Для корректности нужна проверка на разные слеши, ведь на вход может прийти путь не с /
        if (*szPath == ':' && (*(szPath + 1) == '/' || *(szPath + 1) == '\\'))
        {
            return true;
        }
        ++szPath;
    }
    return false;
}

String *CFileSystem::copyFile(const char* szPath)
{
    String *newFilePath = new String(m_filePaths[m_writableRoot] + '/' + getFileName(szPath));
    CopyFile(szPath, newFilePath->c_str(), false);

    return newFilePath;
}

CFileSystem::CFileSystem()
{
    char *path = getFullPathToBuild();
    m_pathToBuild = path;

    mem_delete_a(path);
}

//! Возвращает абсолютный канонизированный путь
char *CFileSystem::getAbsoliteCanonizePath(const char *szPath)
{
    bool absolute = isAbsolutePath(szPath);
    bool correctPath = true;

    int len = absolute ? strlen(szPath) + 1 : MAX_PATH;
    char *fullPath = new char[len];

    absolute ? memcpy(fullPath, szPath, len) : correctPath = resolvePath(szPath, fullPath, len);

    //Во время поиска пути могут произойти ошибки - путь может быть не найден, или слишком маленький буфер для записи
    if (correctPath)
    {
        //Если все корректно прошло, то путь можно канонизировать
        canonize_path(fullPath);

        return fullPath;
    }

    mem_delete_a(fullPath);

    return nullptr;
}

UINT CFileSystem::addRoot(const char *szPath, int iPriority)
{
    String str;

    //Если путь не абсолютный - то прибавляем к нему часть пути к папке build
    if (!isAbsolutePath(szPath))
    {
        str += m_pathToBuild;
    }

    str += szPath; // <--- Оптимизация для того что бы не создавать временных объектов

    m_filePaths.push_back(str);
    m_priority.push_back(iPriority);

    //Если у нас некорректный путь для записи и путь не является архивным
    if (m_writableRoot == -1 && *szPath != '@')
    {
        m_writableRoot = m_filePaths.size() - 1;
    }

    return m_filePaths.size() - 1;
}

UINT CFileSystem::getRootCount()
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
    int len = 0;

    if (isAbsolutePath(szPath))
    {
        len = strlen(szPath) + 1;

        CHECK_SIZE(len, iOutMax);

        memcpy(szOut, szPath, len);
        return true;
    }
    
    String buff;

    for (UINT i = 0, l = m_filePaths.size(); i < l; ++i)
    {
        buff = (m_filePaths[i] + '/' + szPath);

        if (fileExists(buff.c_str()) && isFile(buff.c_str()))
        {
            CHECK_SIZE(len, iOutMax);

            len = buff.length() + 1;
            memcpy(szOut, buff.c_str(), len);
            return true;
        }
    }

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

	//Если не существует или указанный путь ведет не к файлу
    return !(flag & FILE_ATTRIBUTE_DIRECTORY);
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

IFileSystem::IFileIterator *CFileSystem::getFolderList(const char *szPath)
{
    return new CDirIterator(szPath);
}

IFileSystem::IFileIterator *CFileSystem::getFileList(const char *szPath, const char *szExt)
{
    return new CFileExtIterator(szPath, szExt);
}

IFileSystem::IFileIterator *CFileSystem::getFileList(const char *szPath, const char **szExts, int extsCount)
{
    return new CFileExtsIterator(szPath, szExts, extsCount);
}

IFileSystem::IFileIterator *CFileSystem::getFileListRecursive(const char *szPath, const char *szExt = 0)
{
    assert(!"No Implementation");
    return nullptr;
}

IFileSystem::IFileIterator *CFileSystem::getFileListRecursive(const char *szPath, const char **szExts, int extsCount)
{
    assert(!"No Implementation");
    return nullptr;
}

bool CFileSystem::createDirectory(const char *szPath)
{
    // Если вернуло не 0, то все плохо
    return SHCreateDirectoryEx(nullptr, szPath, nullptr) == NO_ERROR;
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

IFile *CFileSystem::openFile(const char *szPath, FILE_OPEN_MODE mode = FILE_MODE_READ)
{
    //Если путей на запись нет, и количество корневых путей нулевое
    if (m_filePaths.size() == 0)
    {
        return nullptr;
    }

    char *fullPath = getAbsoliteCanonizePath(szPath);

    CFile *file = new CFile;

    String *newFileName;

    switch (mode)
    {
    case FILE_MODE_READ:
        file->open(fullPath, CORE_FILE_BIN);

        break;
    case FILE_MODE_WRITE:
        newFileName = copyFile(fullPath);
        file->open(newFileName->c_str(), CORE_FILE_BIN);
        mem_delete(newFileName);

        break;
    case FILE_MODE_APPEND:
        newFileName = copyFile(fullPath);
        file->add(newFileName->c_str(), CORE_FILE_BIN);
        mem_delete(newFileName);

        break;
    default:
        assert(false && "You cannot use multiple opening types at the same time");
        break;
    }

    mem_delete_a(fullPath);

    return file;
}