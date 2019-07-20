#include "FileExtPathsIterator.h"

CFileExtrPathsIterator::CFileExtrPathsIterator(Array<String> *paths, const char *szExt)
: m_paths(paths), m_szExt(szExt)
{}

const char *CFileExtrPathsIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    int size = m_paths->size();

    while (index < size)
    {
        //Если указали расширение файла - то добавляем его к имени пути, иначе ищем все файлы
        String fileName = m_szExt == nullptr ? ((*m_paths)[index] + "*.*") : ((*m_paths)[index] + "*." + *m_szExt);

        //Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
        hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(fileName.c_str(), &FindFileData) : m_handle;

        if (hf != INVALID_HANDLE_VALUE)
        {
            //Если указатель на файл валидный, то проверяем все отфильтрованные файлы по порядку
            while (FindNextFile(hf, &FindFileData) != 0)
            {
                //Сохраняем HANDLE файла, что бы можно было продожлить с того места
                m_handle = hf;

                //По стандарту FindNextFile находит .. (корневую директорию)
                if (!strcmp(FindFileData.cFileName, ".."))
                {
                    continue;
                }

                m_pathStr = (*m_paths)[index] + "/" + FindFileData.cFileName;

                //Возвращаем полный путь, вместе с именем файла и расширением
                return m_pathStr.c_str();
            }
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
