#ifndef __FILE_EXT_ITERATOR_H
#define __FILE_EXT_ITERATOR_H

#include "FileSystem.h"

class CFileExtIterator final : public IFileSystem::IFileIterator
{
private:
    String m_szPath;
    String m_pathStr;

    const char *m_szExt;

    HANDLE m_handle = nullptr;

public:
    CFileExtIterator(const char *szPath, const char *szExt = nullptr);

    const char *XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFileExtIterator();
};

#endif
