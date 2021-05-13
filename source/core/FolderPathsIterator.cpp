#include "FolderPathsIterator.h"

CFolderPathsIterator::CFolderPathsIterator(Array<String> &paths, String &sBasePath)
{
	this->canonizePaths(paths);
	this->canonizePath(sBasePath);

	this->m_paths = paths;
	this->m_sBasePath = sBasePath;
}

const char *CFolderPathsIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

	memset(FindFileData.cFileName, '\0', MAX_PATH);

	UINT size = m_paths.size();

    while (index < size)
    {
		hf = INVALID_OR_NULL(m_handle) ? FindFirstFile((m_paths[index] + "*.*").c_str(), &FindFileData) : m_handle;

        if (hf != INVALID_HANDLE_VALUE)
        {
            do {
                m_handle = hf;

				m_pathStr = (m_paths)[index] + FindFileData.cFileName;

                DWORD flag = GetFileAttributes(m_pathStr.c_str());

				if (emptyOrRepeatPath(FindFileData.cFileName))
                {
                    continue;
                }

                //Берет только имена директорий
                if (flag != INVALID_FILE_ATTRIBUTES && flag & FILE_ATTRIBUTE_DIRECTORY)
                {
					m_pathStr = (m_sBasePath + FindFileData.cFileName);
					if (m_mapExistPath.KeyExists(m_pathStr))
					{
						continue;
					}
					else
					{
						//Возвращаем относительный путь к директории
						m_mapExistPath[m_pathStr] = index;
						return m_pathStr.c_str();
					}
                }
			} while (FindNextFile(hf, &FindFileData) != 0);
        }
		FIND_CLOSE(m_handle);
		++index;
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
}
