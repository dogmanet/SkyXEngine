#ifndef __FILEINMEMORY_H
#define __FILEINMEMORY_H

#include "sxcore.h"

class CFileInMemory: public IFile
{
public:
	CFileInMemory(byte *pData, size_t uDataSize, bool bTakeOwnMemory = false);
	~CFileInMemory();
	void Release();

	int open(const char *szPath, int iType = CORE_FILE_TEXT);	//!< открыть файл
	int create(const char *szPath, int iType = CORE_FILE_TEXT);	//!< создать файл
	int add(const char *szPath, int iType = CORE_FILE_TEXT);	//!< добавить в конец файла
	size_t readBin(void *pDest, size_t iSize);					//!< считать в dest количетсво байт size
	size_t writeBin(const void *pSrc, size_t iSize);			//!< записать src в количетве size байт
	size_t readText(const char *szFormat, ...);					//!< чтение из файла, в аргументы только указатели
	size_t writeText(const char *szFormat, ...);				//!< запись в файл
	size_t getSize() const;		//!< получить размер файла в байтах
	int readChar();				//!< считать символ
	size_t getPos() const;		//!< текущая позиция курсора в файле
	void setPos(size_t iPos);	//!< установить позицию
	void close();				//!< закрыть файл
	bool isEOF() const;			//!< текущая позиция является концом файла?

protected:
	byte *m_pData = NULL;
	size_t m_uSize = 0;
	size_t m_uPos = 0;
};

#endif
