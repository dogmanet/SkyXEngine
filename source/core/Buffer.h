/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __BUFFER_H
#define __BUFFER_H

#include <xcommon/IXBuffer.h>

//##########################################################################

//! реализация интерфейса IXBuffer
class CBuffer : public IXBuffer
{
public:
	~CBuffer(){ free(); }
	virtual void XMETHODCALLTYPE alloc(size_t size) override
	{
		if (m_size > 0 && m_pBuffer)
			free();

		m_size = size;
		m_pBuffer = new BYTE[m_size];
	}

	virtual void XMETHODCALLTYPE free() override
	{
		mem_delete_a(m_pBuffer);
		m_size = 0;
	}

	virtual size_t XMETHODCALLTYPE size() override
	{
		return m_size;
	}

	virtual BYTE* XMETHODCALLTYPE get() override
	{
		return m_pBuffer;
	}

protected:
	BYTE *m_pBuffer = NULL;
	size_t m_size = 0;
};

#endif
