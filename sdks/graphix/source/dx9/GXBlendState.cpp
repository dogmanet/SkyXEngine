#include "GXBlendState.h"

void CGXBlendState::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyBlendState(this);
	}
}

CGXBlendState::~CGXBlendState()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		mem_release(m_pStateBlock);
	}
}
