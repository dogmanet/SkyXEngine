#include "FolderPathsIterator.h"

CFolderPathsIterator::CFolderPathsIterator(Array<String> *paths)
: m_paths(paths)
{

}

const char *CFolderPathsIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    int size = m_paths->size();

    while (index < size)
    {
        hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(((*m_paths)[index] + "*.*").c_str(), &FindFileData) : m_handle;

        if (hf != INVALID_HANDLE_VALUE)
        {
            while (FindNextFile(hf, &FindFileData) != 0)
            {
                m_handle = hf;

                m_pathStr = (*m_paths)[index] + FindFileData.cFileName;

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
            FIND_CLOSE(m_handle);
            ++index;
        }
    }

    //Если вообще не нашли файлов возвращаем nullptr
    return nullptr;
}

void CFolderPathsIterator::reset()
{
    index = 0;
    FIND_CLOSE(m_handle);
}

CFolderPathsIterator::~CFolderPathsIterator()
{
    FIND_CLOSE(m_handle);
    mem_delete(m_paths);
}
