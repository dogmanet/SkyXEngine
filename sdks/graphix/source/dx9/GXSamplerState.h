#ifndef _CGXSamplerState_H_
#define _CGXSamplerState_H_

#include "GXContext.h"

class CGXSamplerState: public IGXSamplerState
{
	friend class CGXContext;

	CGXSamplerState(CGXContext * pRender):m_pRender(pRender)
	{
	};

	CGXContext * m_pRender;
	//IDirect3DStateBlock9 *m_pStateBlock;
	DWORD m_dwCurrentStates[D3DSAMP_SRGBTEXTURE];
	
	DWORD getD3DTADDRESS(GXTEXTURE_ADDRESS_MODE mode);
	void getMagMinMipFilter(GXFILTER filter, DWORD *pdwMag, DWORD *pdwMin, DWORD *pdwMip);

public:
	void Release();
};

#endif
