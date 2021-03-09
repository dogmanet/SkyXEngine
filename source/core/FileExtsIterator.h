/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, Ivan Dokunov, 2021
See the license in LICENSE
***********************************************************/

#ifndef __FILE_EXST_ITERATOR_H
#define __FILE_EXST_ITERATOR_H

#include "CBaseFileIterator.h"

class CFileExtsIterator final : public CBaseFileIterator
{
private:
	Array<String> m_paths;
	String m_pathStr;
	String m_sBasePath;
	Array<String> m_exts;
	AssotiativeArray<String, int> m_mapExistPath;

	int index = 0;
	HANDLE m_handle = nullptr;

	//Текущее расширение
	int m_currentExt = 0;

public:
	CFileExtsIterator::CFileExtsIterator(Array<String> &paths, String &sBasePath, const char **szExt, int iExtSize);

	const char* XMETHODCALLTYPE next() override;

	void XMETHODCALLTYPE reset() override;

	~CFileExtsIterator();
};

#endif