#include "FileExtsIterator.h"

CFileExtsIterator::CFileExtsIterator(const char *szPath, const char **szExts, int extArraySize)
: m_sPath(szPath), m_szExts(szExts), m_extCount(extArraySize)
{}

const char *CFileExtsIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    while (m_currentExt < m_extCount)
    {
        String fileName = m_sPath + "*." + *m_szExts[m_currentExt];

        //Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
        hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(fileName.c_str(), &FindFileData) : m_handle;

        if (hf != INVALID_HANDLE_VALUE)
        {
            while (FindNextFile(hf, &FindFileData) != 0)
            {
                m_handle = hf;

                m_pathStr = m_sPath + "/" + FindFileData.cFileName;

                return m_pathStr.c_str();
            }
        }
		++m_currentExt;
    }

    return nullptr;
}

void CFileExtsIterator::reset()
{
    m_currentExt = 0;
    CLOSE_HANDLE(m_handle);
}

CFileExtsIterator::~CFileExtsIterator()
{
    CLOSE_HANDLE(m_handle);
}
