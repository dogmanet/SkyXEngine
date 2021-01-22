#ifndef __FILE_RECURSIVE_EXT_ITERATOR_H
#define __FILE_RECURSIVE_EXT_ITERATOR_H

#include "FileSystem.h"

class FileRecursiveExtIterator final : public IXUnknownImplementation<IFileIterator>
{
private:
	String m_basePath;
    String m_sPath;
    String m_pathStr;
	Array<String> m_folderList;

    const char *m_szExt;

    HANDLE m_handle = nullptr;

public:
	FileRecursiveExtIterator(const char *szPath, const char *szExts);

    const char* XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

	~FileRecursiveExtIterator();
};

#endif