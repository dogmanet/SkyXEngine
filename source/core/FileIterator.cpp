#include "FileIterator.h"

const char *CFileIterator::next()
{
    assert(!"This method has not been tested");

    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    while (m_current != m_end)
    {
        hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(m_current->c_str(), &FindFileData) : m_handle;

        if (hf != INVALID_HANDLE_VALUE)
        {
            if (FindNextFile(hf, &FindFileData) != 0)
            {
                m_handle = hf;

                return (*m_current + "\\" + FindFileData.cFileName).c_str();
            }
        }
        else
        {
            ++m_current;
        }
    }
    return nullptr;
}

void CFileIterator::reset()
{
    m_current = m_begin;
    CLOSE_HANDLE(m_handle);
}

CFileIterator::~CFileIterator()
{
    CLOSE_HANDLE(m_handle);
}