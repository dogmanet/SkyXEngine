
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __IFILE_H
#define __IFILE_H

#include <gdefines.h>

/*! \name Режимы открытия файлов
@{*/

/*! двоичный */
#define CORE_FILE_BIN	0	

/*! текстовый */
#define CORE_FILE_TEXT	1	

//!@}

/*! конец файла */
#define CORE_FILE_EOF	EOF	

/*! Интерфейс для записи/чтения файлов
\note аргумент iType - режим отрытия файла
*/
class IFile: public IBaseObject
{
public:
	virtual ~IFile()
	{
	};

	virtual int open(const char *szPath, int iType = CORE_FILE_TEXT) = 0;	//!< открыть файл
	virtual int create(const char *szPath, int iType = CORE_FILE_TEXT) = 0;	//!< создать файл
	virtual int add(const char *szPath, int iType = CORE_FILE_TEXT) = 0;	//!< добавить в конец файла
	virtual size_t readBin(void *pDest, size_t iSize) = 0;					//!< считать в dest количетсво байт size
	virtual size_t writeBin(const void *pSrc, size_t iSize) = 0;			//!< записать src в количетве size байт
	virtual size_t readText(const char *szFormat, ...) = 0;					//!< чтение из файла, в аргументы только указатели
	virtual size_t writeText(const char *szFormat, ...) = 0;				//!< запись в файл
	virtual size_t getSize() const = 0;		//!< получить размер файла в байтах
	virtual int readChar() = 0;				//!< считать символ
	virtual size_t getPos() const = 0;		//!< текущая позиция курсора в файле
	virtual void setPos(size_t iPos) = 0;	//!< установить позицию
	virtual void close() = 0;				//!< закрыть файл
	virtual bool isEOF() const = 0;			//!< текущая позиция является концом файла?
};

#endif
