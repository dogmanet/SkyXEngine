#include "FileSystem.h"

#define INVALID_OR_NULL(handle) handle == nullptr || handle == INVALID_HANDLE_VALUE

class CFileIterator : IFileSystem::IFileIterator
{
private:
    String *m_begin;
    String *m_end;
    String *m_current;

    HANDLE m_handle = nullptr;

public:
    const char *XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFileIterator();
};