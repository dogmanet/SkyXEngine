#include "GXShader.h"

void CGXVertexShader::Release()
{
	m_pRender->destroyVertexShader(this);
}

void CGXPixelShader::Release()
{
	m_pRender->destroyPixelShader(this);
}
