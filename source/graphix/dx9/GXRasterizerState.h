#ifndef _CGXRasterizerState_H_
#define _CGXRasterizerState_H_

#include "GXContext.h"

class CGXRasterizerState: public IGXRasterizerState
{
	friend class CGXContext;

	CGXRasterizerState(CGXContext * pRender):m_pRender(pRender)
	{
	};
	~CGXRasterizerState();

	CGXContext * m_pRender;
	IDirect3DStateBlock9 *m_pStateBlock = NULL;

public:
	void Release();
};

#endif
