#ifndef __FILE_RECURSIVE_EXT_PATHS_ITERATOR_H
#define __FILE_RECURSIVE_EXT_PATHS_ITERATOR_H

#include "CBaseFileIterator.h"

class FileRecursiveExtPathsIterator final : public CBaseFileIterator
{
private:
	Array<String> m_folderList;
	Array<String> m_sPaths;
	AssotiativeArray<String, int> m_mapExistPath;

	String m_sBasePath;
	String m_currentFullPath;
    String m_pathStr;
	
    const char *m_szExt;
	UINT pathIndex = 0;

    HANDLE m_handle = nullptr;

public:
	FileRecursiveExtPathsIterator(Array<String> &paths, String &sBasePath, const char *szExt);

    const char* XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

	~FileRecursiveExtPathsIterator();
};

#endif