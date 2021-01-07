#include "FileExtPathsIterator.h"

CFileExtrPathsIterator::CFileExtrPathsIterator(Array<String> *paths, const String &sPath, const char *szExt)
: m_paths(paths), m_szExt(szExt), m_sPath(sPath)
{}

const char *CFileExtrPathsIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    int size = m_paths->size();

    while (index < size)
    {
        //Если указали расширение файла - то добавляем его к имени пути, иначе ищем все файлы
        String fileName = m_szExt == nullptr ? ((*m_paths)[index] + "*.*") : ((*m_paths)[index] + "*." + m_szExt);

        //Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
        hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(fileName.c_str(), &FindFileData) : m_handle;

        if (hf != INVALID_HANDLE_VALUE)
        {
            do {
                //Сохраняем HANDLE файла, что бы можно было продожлить с того места
                m_handle = hf;

                m_pathStr = (*m_paths)[index] + FindFileData.cFileName;

                DWORD flag = GetFileAttributes(m_pathStr.c_str());

                if (flag != INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
                {
					m_pathStr = (m_sPath + FindFileData.cFileName);
					if (m_mapExistPath.KeyExists(m_pathStr))
					{
						continue;
					} 
					else
					{
						//Возвращаем относительный путь, вместе с именем файла и расширением
						m_mapExistPath[m_pathStr] = index;
						return m_pathStr.c_str();
					}
                }
				//Если указатель на файл валидный, то проверяем все отфильтрованные файлы по порядку
			} while (FindNextFile(hf, &FindFileData) != 0);
            ++index;
            FIND_CLOSE(m_handle);
        }
    }

    //Если вообще не нашли файлов возвращаем nullptr
    return nullptr;
}

void CFileExtrPathsIterator::reset()
{
    index = 0;
    FIND_CLOSE(m_handle);
}

CFileExtrPathsIterator::~CFileExtrPathsIterator()
{
    FIND_CLOSE(m_handle);
    mem_delete(m_paths);
}
