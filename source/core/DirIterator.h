#ifndef __DIRECTORY_ITERATOR_H
#define __DIRECTORY_ITERATOR_H

#include "FileSystem.h"

#define INVALID_OR_NULL(handle) handle == nullptr || handle == INVALID_HANDLE_VALUE

class CDirIterator final : public IFileSystem::IFileIterator
{
private:
    String m_sPath;

    HANDLE m_handle = nullptr;

public:
    CDirIterator(const char *szPath);

    const char *XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CDirIterator();
};

#endif