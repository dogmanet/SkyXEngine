/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, Ivan Dokunov, 2019
See the license in LICENSE
***********************************************************/

#ifndef __FOLDER_PATHS_ITERATOR_H
#define __FOLDER_PATHS_ITERATOR_H

#include "FileSystem.h"

class CFolderPathsIterator final: public IXUnknownImplementation<IFileIterator>
{
private:

    Array<String>* m_paths;
	String m_sPath;
    String m_pathStr;
	AssotiativeArray<String, int> m_mapExistPath;

    int index = 0;

    HANDLE m_handle = nullptr;

public:
    CFolderPathsIterator(Array<String> *paths, const String &path);

    const char* XMETHODCALLTYPE next() override;

    void XMETHODCALLTYPE reset() override;

    ~CFolderPathsIterator();
};

#endif
