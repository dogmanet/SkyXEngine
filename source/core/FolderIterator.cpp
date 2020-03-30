#include "FolderIterator.h"

CFolderIterator::CFolderIterator(const char *szPath)
: m_sPath(szPath)
{
    char symbol = szPath[strlen(szPath) - 1];

    if (symbol != '\\' && symbol != '/')
    {
        m_sPath += '/';
    }
}

const char *CFolderIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    /*В отличии от fileIterator тут мы должны искать все файлы
    для корректной проверки, является ли файл - директорией*/
    hf = INVALID_OR_NULL(m_handle) ? FindFirstFile((m_sPath + "*.*").c_str(), &FindFileData) : m_handle;

    if (hf != INVALID_HANDLE_VALUE)
    {
        while (FindNextFile(hf, &FindFileData) != 0)
        {
            m_handle = hf;

            m_pathStr = m_sPath + FindFileData.cFileName;

            DWORD flag = GetFileAttributes(m_pathStr.c_str());

            if (!strcmp(FindFileData.cFileName, "..") || !strcmp(FindFileData.cFileName, "."))
            {
                continue;
            }

            //Берет только имена директорий
            if (flag != INVALID_FILE_ATTRIBUTES && flag & FILE_ATTRIBUTE_DIRECTORY)
            {
                //Возвращаем полный путь к директории
                return m_pathStr.c_str();
            }
        }
    }

    //Если вообще не нашли файлов возвращаем nullptr
    return nullptr;
}

void CFolderIterator::reset()
{
    FIND_CLOSE(m_handle);
}

CFolderIterator::~CFolderIterator()
{
    FIND_CLOSE(m_handle);
}
