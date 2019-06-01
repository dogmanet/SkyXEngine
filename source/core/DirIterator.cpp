#include "DirIterator.h"

CDirIterator::CDirIterator(const char *szPath)
: m_sPath(szPath)
{}

const char *CDirIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    //Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
    hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(m_sPath.c_str(), &FindFileData) : m_handle;

    if (hf != INVALID_HANDLE_VALUE)
    {
        while (FindNextFile(hf, &FindFileData) != 0)
        {
            //Если это папка - то записывает handle и возвращаем путь к папке
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                m_handle = hf;

                m_pathStr = m_sPath + "/" + FindFileData.cFileName;

                return m_pathStr.c_str();
            }
        }
    }

    return nullptr;
}

void CDirIterator::reset()
{
    CLOSE_HANDLE(m_handle);
}

CDirIterator ::~CDirIterator()
{
    CLOSE_HANDLE(m_handle);
}
