
#ifndef __FILE_H
#define __FILE_H

#include "sxcore.h"
#include <fstream>

class CFile : public IFile
{
public:

	CFile();
	~CFile();
	void Release();
	int open(const char *szPath, int iType = CORE_FILE_TEXT);
	int create(const char *szPath, int iType = CORE_FILE_TEXT);
	int add(const char *szPath, int iType = CORE_FILE_TEXT);

	size_t readBin(void *pDest, size_t iSize);
	size_t writeBin(const void *pSrc, size_t iSize);

	size_t readText(const char *szFormat, ...);
	size_t writeText(const char *szFormat, ...);

	size_t getSize() const;
	int readChar();

	size_t getPos() const;
	void setPos(size_t iPos);

	void close();
	bool isEOF() const;

protected:
	FILE * m_pFile;
};

#endif
