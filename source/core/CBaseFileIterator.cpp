#include "CBaseFileIterator.h"

void CBaseFileIterator::canonizePath(String &sPath)
{
	char symbol = sPath[sPath.length() - 1];

	/*Дело в том что абсолютный путь к файлу может не иметь символ "/"
	или "\\" на конце строки, и, если его не будет путь будет некорректен*/
	if (symbol != '\\' && symbol != '/')
	{
		sPath += '/';
	}
}

void CBaseFileIterator::canonizePaths(Array<String> &paths)
{
	for (int i = 0, I = paths.size(); i < I; ++i)
	{
		canonizePath(paths[i]);
	}
}

void CBaseFileIterator::fillExtensionsArray(Array<String> &extsArray, const char **exts, int iExtsSize)
{
	for (int i = 0; i < iExtsSize; ++i)
	{
		extsArray.push_back(exts[i]);
	}
}

bool CBaseFileIterator::findExtensionsInPath(const char *szPath, const Array<String> &exts)
{
	for (int i = 0, I = exts.size(); i < I; ++i)
	{
		if (strstr(szPath, exts[i].c_str()) != NULL)
		{
			return true;
		}
	}
	return !exts.size();
}
