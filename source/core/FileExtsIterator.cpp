#include "FileExtsIterator.h"

CFileExtsIterator::CFileExtsIterator(Array<String> &paths, String &sBasePath, const char **szExt, int iExtSize)
{
	this->canonizePaths(paths);
	this->canonizePath(sBasePath);
	this->fillExtensionsArray(m_exts, szExt, iExtSize);

	this->m_paths = paths;
	this->m_sBasePath = sBasePath;
}

const char *CFileExtsIterator::next()
{
	WIN32_FIND_DATAW FindFileData;
	HANDLE hf;

	FindFileData.cFileName[0] = '\0';

	int size = m_paths.size();
	int sizeExt = m_exts.size();

	while (index < size)
	{
		const String &fileName = (m_paths[index] + "*.*");

		//Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
		hf = INVALID_OR_NULL(m_handle) ? FindFirstFileW(CMB2WC(fileName.c_str()), &FindFileData) : m_handle;

		if (hf != INVALID_HANDLE_VALUE)
		{
			do {
				//Сохраняем HANDLE файла, что бы можно было продожлить с того места
				m_handle = hf;

				m_pathStr = m_paths[index] + CWC2MB(FindFileData.cFileName);

				DWORD flag = GetFileAttributesW(CMB2WC(m_pathStr.c_str()));

				if (flag != INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!findExtensionsInPath(CWC2MB(FindFileData.cFileName), m_exts))
					{
						continue;
					}

					m_pathStr = (m_sBasePath + CWC2MB(FindFileData.cFileName));
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
			} while (FindNextFileW(hf, &FindFileData) != 0);
		}
		++index;
		FIND_CLOSE(m_handle);
	}

	//Если вообще не нашли файлов возвращаем nullptr
	return nullptr;
}

void CFileExtsIterator::reset()
{
	index = 0;
	FIND_CLOSE(m_handle);
}

CFileExtsIterator::~CFileExtsIterator()
{
	FIND_CLOSE(m_handle);
}
