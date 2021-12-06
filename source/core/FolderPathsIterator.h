/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, Ivan Dokunov, 2019
See the license in LICENSE
***********************************************************/

#ifndef __FOLDER_PATHS_ITERATOR_H
#define __FOLDER_PATHS_ITERATOR_H

#include "BaseFileIterator.h"

class CFolderPathsIterator final : public CBaseFileIterator
{
private:

    Array<String> m_paths;
	String m_sBasePath;
    String m_pathStr;
	AssotiativeArray<String, int> m_mapExistPath;

    int index = 0;

    HANDLE m_handle = nullptr;

public:
	CFolderPathsIterator(Array<String> &paths, String &sBasePath);

    const char* XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFolderPathsIterator();
};

#endif
