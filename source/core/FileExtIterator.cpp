#include "FileExtIterator.h"

CFileExtIterator::CFileExtIterator(const char *szPath, const char *szExt)
: m_sPath(szPath), m_szExt(szExt)
{}

const char *CFileExtIterator::next()
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;

    //Если указали расширение файла - то добавляем его к имени пути, иначе ищем все файлы
    String fileName = m_szExt == nullptr ? (m_sPath + "*.*") : (m_sPath + "*." + *m_szExt);

    //Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
    hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(fileName.c_str(), &FindFileData) : m_handle;

    if (hf != INVALID_HANDLE_VALUE)
    {
        //Если указатель на файл валидный, то проверяем все отфильтрованные файлы по порядку
        while (FindNextFile(hf, &FindFileData) != 0)
        {
            //Сохраняем HANDLE файла, что бы можно было продожлить с того места
            m_handle = hf;

            /*В папке может быть максимум один "..". Следовательно можно добавить оптимизацию
            на то что если в папке уже нашли один ".." то делать проверку на его наличие бессмысленно
            TODO: сделать оптимизацию позже */
            if (!strcmp(FindFileData.cFileName, "..") || !strcmp(FindFileData.cFileName, "."))
            {
                continue;
            }

            m_pathStr = m_sPath + "/" + FindFileData.cFileName;

            //Возвращаем полный путь, вместе с именем файла и расширением
            return m_pathStr.c_str();
        }
    }

    //Если вообще не нашли файлов возвращаем nullptr
    return nullptr;
}

void CFileExtIterator::reset()
{
    FIND_CLOSE(m_handle);
}

CFileExtIterator::~CFileExtIterator()
{
    FIND_CLOSE(m_handle);
}
