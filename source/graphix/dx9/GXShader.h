#ifndef _IDSGshaderOgl_H_
#define _IDSGshaderOgl_H_

#include "GXContext.h"
#include <common/assotiativearray.h>
#include <common/AAString.h>

class CGXVertexShader: public IGXVertexShader
{
	friend class CGXContext;

	CGXVertexShader(CGXContext * pRender):m_pRender(pRender)
	{
	}
	~CGXVertexShader();

	CGXContext * m_pRender;
	IDirect3DVertexShader9 *m_pShader;
	AssotiativeArray<AAString, D3DXCONSTANT_DESC> m_mConstLocations;

	void parseConstantTable(ID3DXConstantTable *pConstTable);

	float *m_pConstBufferF = NULL;
	int *m_pConstBufferI = NULL;
	UINT m_uConstBuffRegCountI = 0;
	UINT m_uConstBuffRegCountF = 0;
public:
	void Release();

	void setConstantF(UINT uStartRegister, const float *pConstantData, UINT uVector4fCount);
	void setConstantI(UINT uStartRegister, const int *pConstantData, UINT uVector4iCount);
	UINT getConstantLocation(const char *szConstName);
};

class CGXPixelShader: public IGXPixelShader
{
	friend class CGXContext;

	CGXPixelShader(CGXContext * pRender):m_pRender(pRender)
	{
	}
	~CGXPixelShader();

	CGXContext * m_pRender;
	IDirect3DPixelShader9 *m_pShader;
	AssotiativeArray<AAString, D3DXCONSTANT_DESC> m_mConstLocations;

	void parseConstantTable(ID3DXConstantTable *pConstTable);

	float *m_pConstBufferF = NULL;
	int *m_pConstBufferI = NULL;
	UINT m_uConstBuffRegCountI = 0;
	UINT m_uConstBuffRegCountF = 0;
public:
	void Release();

	void setConstantF(UINT uStartRegister, const float *pConstantData, UINT uVector4fCount);
	void setConstantI(UINT uStartRegister, const int *pConstantData, UINT uVector4iCount);
	UINT getConstantLocation(const char *szConstName);
};


class CGXShader: public IGXShader
{
	friend class CGXContext;

	CGXShader(CGXContext * pRender, IGXVertexShader *pVS = NULL, IGXPixelShader *pPS = NULL):
		m_pRender(pRender),
		m_pVShader(pVS),
		m_pPShader(pPS)
	{
	}

	CGXContext * m_pRender;
	IGXVertexShader *m_pVShader;
	IGXPixelShader *m_pPShader;
public:
	void Release()
	{
		m_pRender->destroyShader(this);
	}

	IGXPixelShader *getPixelShader()
	{
		return(m_pPShader);
	}
	IGXVertexShader *getVertexShader()
	{
		return(m_pVShader);
	}

	void setPixelShader(IGXPixelShader *pShader)
	{
		m_pPShader = pShader;
	}
	void setVertexShader(IGXVertexShader *pShader)
	{
		m_pVShader = pShader;
	}
};

#endif
