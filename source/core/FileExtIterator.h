/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, Ivan Dokunov, 2019
See the license in LICENSE
***********************************************************/

#ifndef __FILE_EXT_ITERATOR_H
#define __FILE_EXT_ITERATOR_H

#include "FileSystem.h"

class CFileExtIterator final : public IFileIterator
{
private:
    String m_sPath;
    String m_pathStr;

    const char *m_szExt;

    HANDLE m_handle = nullptr;

public:
    CFileExtIterator(const char *szPath, const char *szExt = nullptr);

    const char* XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFileExtIterator();
};

#endif
