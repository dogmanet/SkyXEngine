#ifndef _IDSGshaderOgl_H_
#define _IDSGshaderOgl_H_

#include "GXContext.h"

class CGXVertexShader: public IGXVertexShader
{
	friend class CGXContext;

	CGXVertexShader(CGXContext * pRender):m_pRender(pRender)
	{
	};
	CGXContext * m_pRender;
	GLuint m_pShader;
public:
	void Release();
};

class CGXPixelShader: public IGXPixelShader
{
	friend class CGXContext;

	CGXPixelShader(CGXContext * pRender):m_pRender(pRender)
	{
	};
	CGXContext * m_pRender;
	GLuint m_pShader;
public:
	void Release();
};

#endif
