#include "FileRecursiveExtPathsIterator.h"

FileRecursiveExtPathsIterator::FileRecursiveExtPathsIterator(Array<String> &paths, String &sBasePath, const char *szExt)
	: m_szExt(szExt)
{
	this->canonizePaths(paths);
	this->canonizePath(sBasePath);

	this->m_sPaths = paths;
	this->m_sBasePath = sBasePath;
}

const char *FileRecursiveExtPathsIterator::next()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	UINT maxPathIndex = m_sPaths.size();
	while (pathIndex < maxPathIndex)
	{
		m_currentFullPath = !m_currentFullPath.length() ? m_sPaths[pathIndex] : m_currentFullPath;
		do {
			String fileName = m_sPaths[pathIndex] + "*.*";

			//Проверяем указатель, если m_handle пустой, то ищем первый файл с расширением szExts
			hf = INVALID_OR_NULL(m_handle) ? FindFirstFile(fileName.c_str(), &FindFileData) : m_handle;

			if (hf != INVALID_HANDLE_VALUE)
			{
				
				do {
					//Сохраняем HANDLE файла, что бы можно было продожлить с того места
					m_handle = hf;

					String fullName = m_sPaths[pathIndex] + FindFileData.cFileName;

					DWORD flag = GetFileAttributes(fullName.c_str());

					if (!strcmp(FindFileData.cFileName, "..") || !strcmp(FindFileData.cFileName, "."))
					{
						continue;
					}

					if (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY))
					{
						m_folderList.push_back(fullName + '/');
						continue;
					}

					//Если указали расширение файла - то добавляем его к имени пути, иначе ищем все файлы
					if (m_szExt != NULL && !strstr(FindFileData.cFileName, m_szExt))
					{
						continue;
					}

					if (flag != INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
					{
						//Если это файл - получаем относительный путь и ищем его в списке
						m_pathStr = strstr(fullName.c_str(), m_sBasePath.c_str());
						//m_pathStr = m_sBasePath + FindFileData.cFileName;
						if (m_mapExistPath.KeyExists(m_pathStr))
						{
							continue;
						} 
						else
						{
							m_mapExistPath[m_pathStr] = pathIndex;
							return m_pathStr.c_str();
						}
					}
					//Если указатель на файл валидный, то проверяем все отфильтрованные файлы по порядку
				} while (FindNextFile(hf, &FindFileData) != 0);

				if (m_folderList.size() != 0)
				{
					UINT index = 0;
					m_sPaths[pathIndex] = m_folderList[index];
					m_folderList.erase(index);	
					m_handle = NULL;
				}
				else
				{
					m_sPaths[pathIndex] = m_currentFullPath;
				}
			}
		} while (m_sPaths[pathIndex] != m_currentFullPath);
		++pathIndex;
		m_currentFullPath.release();
		m_handle = NULL;
	}

	//Если вообще не нашли файлов возвращаем nullptr
	return nullptr;
}

void FileRecursiveExtPathsIterator::reset()
{
	if (m_sPaths.size() < pathIndex) 
		m_sPaths[pathIndex] = m_currentFullPath;

	m_currentFullPath.release();
	m_mapExistPath.clear();
	pathIndex = 0;
	CLOSE_HANDLE(m_handle);
}

FileRecursiveExtPathsIterator::~FileRecursiveExtPathsIterator()
{
	CLOSE_HANDLE(m_handle);
}
