#include "GXShader.h"

void CGXVertexShader::Release()
{
	m_pRender->destroyVertexShader(this);
}

CGXVertexShader::~CGXVertexShader()
{
	mem_release(m_pShader);
	mem_delete_a(m_pConstBufferF);
	mem_delete_a(m_pConstBufferI);
}

void CGXVertexShader::parseConstantTable(ID3DXConstantTable *pConstTable)
{
	D3DXCONSTANTTABLE_DESC desc;
	pConstTable->GetDesc(&desc);

	D3DXCONSTANT_DESC constantDesc;
	UINT count = 1;
	UINT uBuffSizeI = 0;
	UINT uBuffSizeF = 0;
	for(UINT i = 0; i < desc.Constants; ++i)
	{
		D3DXHANDLE hConstant = pConstTable->GetConstant(0, i);
		count = 1;
		pConstTable->GetConstantDesc(hConstant, &constantDesc, &count);

		UINT uSize = constantDesc.RegisterIndex + constantDesc.RegisterCount;

		if(constantDesc.RegisterSet == D3DXRS_FLOAT4)
		{
			uBuffSizeF = max(uBuffSizeF, uSize);
		}
		else if(constantDesc.RegisterSet == D3DXRS_INT4)
		{
			uBuffSizeI = max(uBuffSizeI, uSize);
		}
		else
		{
			continue;
		}

		AAString sName;
		sName.setName(constantDesc.Name);
		m_mConstLocations[sName] = constantDesc;
	}

	m_uConstBuffRegCountI = uBuffSizeI;
	m_uConstBuffRegCountF = uBuffSizeF;

	if(m_uConstBuffRegCountF)
	{
		m_pConstBufferF = new float[m_uConstBuffRegCountF * 4];
	}
	if(m_uConstBuffRegCountI)
	{
		m_pConstBufferI = new int[m_uConstBuffRegCountI * 4];
	}
}

void CGXVertexShader::setConstantF(UINT uStartRegister, const float *pConstantData, UINT uVector4fCount)
{
	assert(uStartRegister + uVector4fCount <= m_uConstBuffRegCountF);

	memcpy(m_pConstBufferF, pConstantData, sizeof(float) * 4 * uVector4fCount);
}

void CGXVertexShader::setConstantI(UINT uStartRegister, const int *pConstantData, UINT uVector4iCount)
{
	assert(uStartRegister + uVector4iCount <= m_uConstBuffRegCountI);

	memcpy(m_pConstBufferI, pConstantData, sizeof(int) * 4 * uVector4iCount);
}

UINT CGXVertexShader::getConstantLocation(const char *szConstName)
{
	const AssotiativeArray<AAString, D3DXCONSTANT_DESC>::Node *pNode;
	if(m_mConstLocations.KeyExists(AAString(szConstName), &pNode))
	{
		return(pNode->Val->RegisterIndex);
	}
	return(~0);
}

//##########################################################################

void CGXPixelShader::Release()
{
	m_pRender->destroyPixelShader(this);
}

CGXPixelShader::~CGXPixelShader()
{
	mem_release(m_pShader);
	mem_delete_a(m_pConstBufferF);
	mem_delete_a(m_pConstBufferI);
}

void CGXPixelShader::parseConstantTable(ID3DXConstantTable *pConstTable)
{
	D3DXCONSTANTTABLE_DESC desc;
	pConstTable->GetDesc(&desc);

	D3DXCONSTANT_DESC constantDesc;
	UINT count = 1;
	UINT uBuffSizeI = 0;
	UINT uBuffSizeF = 0;
	for(UINT i = 0; i < desc.Constants; ++i)
	{
		D3DXHANDLE hConstant = pConstTable->GetConstant(0, i);
		count = 1;
		pConstTable->GetConstantDesc(hConstant, &constantDesc, &count);

		UINT uSize = constantDesc.RegisterIndex + constantDesc.RegisterCount;

		if(constantDesc.RegisterSet == D3DXRS_FLOAT4)
		{
			uBuffSizeF = max(uBuffSizeF, uSize);
		}
		else if(constantDesc.RegisterSet == D3DXRS_INT4)
		{
			uBuffSizeI = max(uBuffSizeI, uSize);
		}
		else
		{
			continue;
		}

		AAString sName;
		sName.setName(constantDesc.Name);
		m_mConstLocations[sName] = constantDesc;
	}

	m_uConstBuffRegCountI = uBuffSizeI;
	m_uConstBuffRegCountF = uBuffSizeF;

	if(m_uConstBuffRegCountF)
	{
		m_pConstBufferF = new float[m_uConstBuffRegCountF * 4];
	}
	if(m_uConstBuffRegCountI)
	{
		m_pConstBufferI = new int[m_uConstBuffRegCountI * 4];
	}
}

void CGXPixelShader::setConstantF(UINT uStartRegister, const float *pConstantData, UINT uVector4fCount)
{
	assert(uStartRegister + uVector4fCount <= m_uConstBuffRegCountF);

	memcpy(m_pConstBufferF, pConstantData, sizeof(float) * 4 * uVector4fCount);
}

void CGXPixelShader::setConstantI(UINT uStartRegister, const int *pConstantData, UINT uVector4iCount)
{
	assert(uStartRegister + uVector4iCount <= m_uConstBuffRegCountI);

	memcpy(m_pConstBufferI, pConstantData, sizeof(int) * 4 * uVector4iCount);
}

UINT CGXPixelShader::getConstantLocation(const char *szConstName)
{
	const AssotiativeArray<AAString, D3DXCONSTANT_DESC>::Node *pNode;
	if(m_mConstLocations.KeyExists(AAString(szConstName), &pNode))
	{
		return(pNode->Val->RegisterIndex);
	}
	return(~0);
}
