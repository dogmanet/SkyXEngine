#include "GXDepthStencilState.h"

void CGXDepthStencilState::Release()
{
	m_pRender->destroyDepthStencilState(this);
}

CGXDepthStencilState::~CGXDepthStencilState()
{
	mem_release(m_pStateBlock);
}
