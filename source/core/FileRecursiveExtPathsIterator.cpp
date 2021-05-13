#include "FileRecursiveExtPathsIterator.h"

CFileRecursiveExtPathsIterator::CFileRecursiveExtPathsIterator(Array<String> &paths, String &sBasePath, const char **szExts, int iExtSize)
{
	this->canonizePaths(paths);
	this->canonizePath(sBasePath);
	this->fillExtensionsArray(m_exts, szExts, iExtSize);

	this->m_sPaths = paths;
	this->m_sBasePath = sBasePath;
}

const char *CFileRecursiveExtPathsIterator::next()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	memset(FindFileData.cFileName, '\0', MAX_PATH);

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

					if (emptyOrRepeatPath(FindFileData.cFileName))
					{
						continue;
					}

					if (flag != INVALID_FILE_ATTRIBUTES && (flag & FILE_ATTRIBUTE_DIRECTORY))
					{
						m_folderList.push_back(fullName + '/');
						continue;
					}

					if (flag != INVALID_FILE_ATTRIBUTES && !(flag & FILE_ATTRIBUTE_DIRECTORY))
					{
						if (!findExtensionsInPath(FindFileData.cFileName, m_exts))
						{
							continue;
						}
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

void CFileRecursiveExtPathsIterator::reset()
{
	if (m_sPaths.size() < pathIndex) 
		m_sPaths[pathIndex] = m_currentFullPath;

	m_currentFullPath.release();
	m_mapExistPath.clear();
	pathIndex = 0;
	CLOSE_HANDLE(m_handle);
}

CFileRecursiveExtPathsIterator::~CFileRecursiveExtPathsIterator()
{
	CLOSE_HANDLE(m_handle);
}
