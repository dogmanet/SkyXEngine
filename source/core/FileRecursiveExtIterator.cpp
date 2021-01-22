#include "FileRecursiveExtIterator.h"

FileRecursiveExtIterator::FileRecursiveExtIterator(const char *szPath, const char *szExts)
: m_sPath(szPath), m_szExt(szExts)
{
	char symbol = szPath[strlen(szPath) - 1];

	/*Дело в том что абсолютный путь к файлу может не иметь символ "/"
	или "\\" на конце строки, и, если его не будет путь будет некорректен*/
	if (symbol != '\\' && symbol != '/')
	{
		m_sPath += '/';
	}
	m_basePath = m_sPath;
}

const char *FileRecursiveExtIterator::next()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	do{
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

				m_pathStr = m_sPath + FindFileData.cFileName;

				DWORD flag = GetFileAttributes(m_pathStr.c_str());

				if (!strcmp(FindFileData.cFileName, "..") || !strcmp(FindFileData.cFileName, "."))
				{
					continue;
				}

				if (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY))
				{
					m_folderList.push_back(m_pathStr + '/');
				}

				if (flag != INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
				{
					//Возвращаем полный путь, вместе с именем файла и расширением
					return m_pathStr.c_str();
				}
			}

			if (m_folderList.size() != 0)
			{
				UINT index = 0;
				m_sPath = m_folderList[index];
				m_folderList.erase(index);
				m_handle = NULL;
			} 
			else
			{
				m_sPath = m_basePath;
			}
		}
	} while (m_sPath != m_basePath);

	//Если вообще не нашли файлов возвращаем nullptr
	return nullptr;
}

void FileRecursiveExtIterator::reset()
{
	CLOSE_HANDLE(m_handle);
}

FileRecursiveExtIterator::~FileRecursiveExtIterator()
{
	CLOSE_HANDLE(m_handle);
}
