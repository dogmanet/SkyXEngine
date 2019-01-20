#ifndef _CGXBlendState_H_
#define _CGXBlendState_H_

#include "GXContext.h"

class CGXBlendState: public IGXBlendState
{
	friend class CGXContext;

	CGXBlendState(CGXContext * pRender):m_pRender(pRender)
	{
	};
	~CGXBlendState();

	CGXContext * m_pRender;
	IDirect3DStateBlock9 *m_pStateBlock = NULL;

public:
	void Release();
};

#endif
