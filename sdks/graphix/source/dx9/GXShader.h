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
	UINT getConstantCount();
	UINT getConstantLocation(const char *szConstName);
	UINT getConstantSizeV4(const char *szConstName);
	void getData(void *pData, UINT *pSize);
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
	UINT getConstantCount();
	UINT getConstantLocation(const char *szConstName);
	UINT getConstantSizeV4(const char *szConstName);
	void getData(void *pData, UINT *pSize);
};


class CGXShader: public IGXShader
{
	friend class CGXContext;

	CGXShader(CGXContext * pRender, IGXVertexShader *pVS = NULL, IGXPixelShader *pPS = NULL):
		m_pRender(pRender),
		m_pVShader(pVS),
		m_pPShader(pPS)
	{
		if(pVS)
		{
			pVS->AddRef();
		}
		if(pPS)
		{
			pPS->AddRef();
		}
	}

	CGXContext * m_pRender;
	IGXVertexShader *m_pVShader;
	IGXPixelShader *m_pPShader;
public:
	void Release()
	{
		--m_uRefCount;
		if(!m_uRefCount)
		{
			mem_release(m_pVShader);
			mem_release(m_pPShader);
			m_pRender->destroyShader(this);
		}
	}

	IGXPixelShader *getPixelShader()
	{
		if(m_pPShader)
		{
			m_pPShader->AddRef();
		}
		return(m_pPShader);
	}
	IGXVertexShader *getVertexShader()
	{
		if(m_pVShader)
		{
			m_pVShader->AddRef();
		}
		return(m_pVShader);
	}

	void setPixelShader(IGXPixelShader *pShader)
	{
		mem_release(m_pPShader);
		m_pPShader = pShader;
		if(pShader)
		{
			pShader->AddRef();
		}
	}
	void setVertexShader(IGXVertexShader *pShader)
	{
		mem_release(m_pVShader);
		m_pVShader = pShader;
		if(pShader)
		{
			pShader->AddRef();
		}
	}
};

#endif
