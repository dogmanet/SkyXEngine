#ifndef _CGXDepthStencilState_H_
#define _CGXDepthStencilState_H_

#include "GXContext.h"

class CGXDepthStencilState: public IGXDepthStencilState
{
	friend class CGXContext;

	CGXDepthStencilState(CGXContext * pRender):m_pRender(pRender)
	{
	};
	~CGXDepthStencilState();

	CGXContext * m_pRender;
	IDirect3DStateBlock9 *m_pStateBlock = NULL;

public:
	void Release();
};

#endif
