#ifndef _IDSRindexBufferOgl_H_
#define _IDSRindexBufferOgl_H_

#include "GXContext.h"


class CGXIndexBuffer: public IGXIndexBuffer
{
	friend class CGXContext;

	CGXIndexBuffer(CGXContext * pRender):m_pRender(pRender)
	{
	};
	CGXContext * m_pRender;
	GLuint m_pBuffer;
	UINT m_uIndexSize;
public:
	void Release();
};

#endif
