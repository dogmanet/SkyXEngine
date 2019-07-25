/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, Ivan Dokunov, 2019
See the license in LICENSE
***********************************************************/

#ifndef __FOLDER_ITERATOR_H
#define __FOLDER_ITERATOR_H

#include "FileSystem.h"

class CFolderIterator final : public IFileSystem::IFileIterator
{
private:
    String m_sPath;
    String m_pathStr;

    HANDLE m_handle = nullptr;

public:
    CFolderIterator(const char *szPath);

    const char *XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFolderIterator();
};

#endif
