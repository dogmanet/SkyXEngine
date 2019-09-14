#include "FileInMemory.h"

CFileInMemory::CFileInMemory(byte *pData, size_t uDataSize, bool bTakeOwnMemory)
{
	assert(pData);

	if(bTakeOwnMemory)
	{
		m_pData = pData;
		m_uSize = uDataSize;
	}
}
CFileInMemory::~CFileInMemory()
{
	mem_delete_a(m_pData);
}
void CFileInMemory::Release()
{
	delete this;
}

int CFileInMemory::open(const char *szPath, int iType)
{
	return(-1);
}
int CFileInMemory::create(const char *szPath, int iType)
{
	return(-1);
}
int CFileInMemory::add(const char *szPath, int iType)
{
	return(-1);
}
size_t CFileInMemory::readBin(void *pDest, size_t iSize)
{
	size_t uWillRead = min(iSize, m_uSize - m_uPos);
	
	memcpy(pDest, m_pData + m_uPos, uWillRead);
	m_uPos += uWillRead;

	return(uWillRead);
}
size_t CFileInMemory::writeBin(const void *pSrc, size_t iSize)
{
	return(0);
}
size_t CFileInMemory::readText(const char *szFormat, ...)
{
	assert(!"Not implemented!");
	return(0);
}
size_t CFileInMemory::writeText(const char *szFormat, ...)
{
	return(0);
}
size_t CFileInMemory::getSize() const
{
	return(m_uSize);
}
int CFileInMemory::readChar()
{
	assert(!"Not implemented!");
	return(0);
}
size_t CFileInMemory::getPos() const
{
	return(m_uPos);
}
void CFileInMemory::setPos(size_t iPos)
{
	m_uPos = iPos;
}
void CFileInMemory::close()
{
}
bool CFileInMemory::isEOF() const
{
	return(m_uPos >= m_uSize);
}
