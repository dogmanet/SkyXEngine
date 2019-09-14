/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, Ivan Dokunov, 2019
See the license in LICENSE
***********************************************************/

#ifndef __FILE_EXT_PATHS_ITERATOR_H
#define __FILE_EXT_PATHS_ITERATOR_H

#include "FileSystem.h"

class CFileExtrPathsIterator final : public IFileIterator
{
private:
    Array<String>* m_paths;
    String m_pathStr;

    int index = 0;
    const char *m_szExt;

    HANDLE m_handle = nullptr;

public:
    CFileExtrPathsIterator(Array<String> *paths, const char *szExt = nullptr);

    const char* XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFileExtrPathsIterator();
};

#endif
