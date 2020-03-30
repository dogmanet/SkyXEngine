
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "File.h"

CFile::CFile()
{
	m_pFile = 0;
}

CFile::~CFile()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
}

void CFile::Release()
{
	delete this;
}

int CFile::open(const char *szPath, int iType)
{
    const char *szMode = (iType == CORE_FILE_BIN) ? "rb" : "r+";

    m_pFile = fopen(szPath, szMode);
		if(!m_pFile)
			return -1;
	return 0;
}

int CFile::create(const char *szPath, int iType)
{
    const char *szMode = (iType == CORE_FILE_BIN) ? "wb" : "w";

    m_pFile = fopen(szPath, szMode);
	if (!m_pFile)
		return -1;
	return 0;
}

int CFile::add(const char *szPath, int iType)
{
	m_pFile = fopen(szPath, (iType == CORE_FILE_BIN) ? "rb+" : "r+");
	if(m_pFile)
	{
		_fseeki64(m_pFile, 0, SEEK_END);
		return(0);
	}
	else
	{
		m_pFile = fopen(szPath, (iType == CORE_FILE_BIN) ? "wb+" : "w+");
		if(m_pFile)
		{
			return(0);
		}
	}
	return(-1);
}

size_t CFile::readBin(void *pDest, size_t iSize)
{
	return(fread(pDest, 1, iSize, m_pFile));
}

size_t CFile::writeBin(const void *pSrc, size_t iSize)
{
	return(fwrite(pSrc, 1, iSize, m_pFile));
}

size_t CFile::readText(const char *szFormat, ...)
{
	va_list va;
	va_start(va, szFormat);
	int* argn = va_arg(va, int*);
	int count = 0;
	int countwrite = 0;
	int cwr = 0;
	char tmpbuf[3];
	while (1)
	{
		if ((szFormat + count * 2)[0] == '\0')
			break;

		memcpy(tmpbuf, szFormat + count * 2, 2);
		tmpbuf[2] = 0;
		int cwr = fscanf_s(m_pFile, tmpbuf, argn);
		if (cwr == EOF)
			break;
		countwrite += cwr;
		//int tll = ftell(m_pFile);
		argn = va_arg(va, int*);
		count++;
	}

	return countwrite;
}

size_t CFile::writeText(const char *szFormat, ...)
{
	va_list ptr;
	va_start(ptr, szFormat);
	int countout = vfprintf(m_pFile, szFormat, ptr);
	va_end(ptr);
	return countout;
}

size_t CFile::getPos() const
{
	if (isEOF())
		return CORE_FILE_EOF;
	return ftell(m_pFile);
}

void CFile::setPos(size_t iPos)
{
	_fseeki64(m_pFile, iPos, SEEK_SET);
}

size_t CFile::getSize() const
{
	size_t size;
	size_t curr_pos = _ftelli64(m_pFile);
	_fseeki64(m_pFile, 0, SEEK_END);
	size = ftell(m_pFile);
	_fseeki64(m_pFile, curr_pos, SEEK_SET);
	return size;
}

int CFile::readChar()
{
	return fgetc(m_pFile);
}

void CFile::close()
{
	fclose(m_pFile);
}

bool CFile::isEOF() const
{
	return(feof(m_pFile) != 0);
}
