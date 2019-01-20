#include "GXBlendState.h"

void CGXBlendState::Release()
{
	m_pRender->destroyBlendState(this);
}

CGXBlendState::~CGXBlendState()
{
	mem_release(m_pStateBlock);
}
