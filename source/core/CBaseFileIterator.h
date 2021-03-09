#ifndef __CBASE_FILE_ITERATOR_
#define __CBASE_FILE_ITERATOR_

#include "FileSystem.h"

class CBaseFileIterator : public IXUnknownImplementation<IFileIterator>
{
public:
	void canonizePath(String &sPath);

	void canonizePaths(Array<String> &paths);

	void fillExtensionsArray(Array<String> &extsArray, const char **exts, int iExtsSize);

	bool findExtensionsInPath(const char *szPath, const Array<String> &exts);
};

#endif 
