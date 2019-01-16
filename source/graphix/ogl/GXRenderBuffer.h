#ifndef _IDSGrenderBufferOgl_H_
#define _IDSGrenderBufferOgl_H_

#include "GXContext.h"

class CGXRenderBuffer: public IGXRenderBuffer
{
	friend class CGXContext;

	CGXRenderBuffer(CGXContext * pRender);

	CGXContext * m_pRender;
	GLuint m_uVAO;

public:
	void Release();
};

#endif
