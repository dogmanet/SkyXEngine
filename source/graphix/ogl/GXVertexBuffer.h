#ifndef _IDSRvertexBufferOgl_H_
#define _IDSRvertexBufferOgl_H_

#include "GXContext.h"

class CGXVertexBuffer: public IGXVertexBuffer
{
	friend class CGXContext;

	CGXVertexBuffer(CGXContext * pRender):m_pRender(pRender)
	{
	};
	CGXContext * m_pRender;
	GLuint m_pBuffer;
public:
	void Release();
};

#endif
