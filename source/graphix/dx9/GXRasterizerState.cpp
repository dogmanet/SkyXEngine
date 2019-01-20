#include "GXRasterizerState.h"

void CGXRasterizerState::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyRasterizerState(this);
	}
}

CGXRasterizerState::~CGXRasterizerState()
{
	mem_release(m_pStateBlock);
}
