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

void CBaseFileIterator::canonizePaths(Array<String> &sPaths)
{
	for (int i = 0, I = sPaths.size(); i < I; ++i)
	{
		canonizePath(sPaths[i]);
	}
}