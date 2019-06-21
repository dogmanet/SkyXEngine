#include "MaterialSystem.h"
#include "sxmtrl.h"

CMaterialSystem::CMaterialSystem()
{
	if(SGCore_GetDXDevice())
	{
		m_pObjectConstantBuffer = SGCore_GetDXDevice()->createConstantBuffer(sizeof(CObjectData));
	}
}
CMaterialSystem::~CMaterialSystem()
{
	mem_release(m_pObjectConstantBuffer);
}

void CMaterialSystem::loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount, XSHADER_VARIANT_DESC *pVariantsDesc)
{
	assert(!uVariantCount && "Variants is not implemented!");

	ID id = SMtrl_MtlLoad2(szName, pDefaultShaders, uVariantCount, pVariantsDesc);
	*ppMaterial = new CMaterial(id);
}
bool CMaterialSystem::getMaterial(const char *szName, IXMaterial **ppMaterial)
{
	ID id = SMtrl_MtlLoad(szName);
	if(!ID_VALID(id))
	{
		return(false);
	}
	*ppMaterial = new CMaterial(id);
	return(true);
}

bool CMaterialSystem::getTexture(const char *szName, IXTexture **ppTexture)
{
	ID id = SGCore_LoadTexGetID(szName);
	if(!ID_VALID(id))
	{
		return(false);
	}
	*ppTexture = new CTexture(id);
	return(true);
}
void CMaterialSystem::addTexture(const char *szName, IGXTexture2D *pTexture)
{
	pTexture->AddRef();
	SGCore_LoadTexCreate(szName, pTexture);
}

void CMaterialSystem::setWorld(const SMMATRIX &mWorld)
{
	//SMMATRIX mV, mP;
	//Core_RMatrixGet(G_RI_MATRIX_VIEW, &mV);
	//Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mP);
	m_objectData.m_mW = SMMatrixTranspose(mWorld);
	// m_objectData.m_mWV = SMMatrixTranspose(mV) * m_objectData.m_mW;
	// m_objectData.m_mWVP = SMMatrixTranspose(mP) * m_objectData.m_mWV;
	

	m_pObjectConstantBuffer->update(&m_objectData);
	SGCore_GetDXDevice()->setVertexShaderConstant(m_pObjectConstantBuffer, SCR_OBJECT);
	//SGCore_GetDXDevice()->setPixelShaderConstant(m_pObjectConstantBuffer, SCR_OBJECT);
}
void CMaterialSystem::bindMaterial(IXMaterial *pMaterial, IXShaderVariant *pShaderVariant)
{
	CMaterial *pMat = (CMaterial*)pMaterial;
	SMtrl_MtlRender(pMaterial ? pMat->getId() : 0, NULL);
	// SGCore_MtlSet(pMat->getId(), NULL);
}
void CMaterialSystem::bindTexture(IXTexture *pTexture, UINT slot)
{
	if(pTexture)
	{
		IGXBaseTexture *pTex;
		pTexture->getAPITexture(&pTex);
		SGCore_GetDXDevice()->setTexture(pTex, slot);
		mem_release(pTex);
	}
	else
	{
		SGCore_GetDXDevice()->setTexture(NULL, slot);
	}
}

void CMaterialSystem::overridePixelShader(ID id)
{
	SMtrl_MtlPixelShaderOverride(id);
}

void CMaterialSystem::overrideGeometryShader(ID id)
{
	SMtrl_MtlGeometryShaderOverride(id);
}

//#############################################################################

CTexture::CTexture(ID id):
	m_id(id)
{
}

void CTexture::getAPITexture(IGXBaseTexture **ppTexture)
{
	IGXBaseTexture *pTex = SGCore_LoadTexGetTex(m_id);
	if(pTex)
	{
		pTex->AddRef();
	}
	*ppTexture = pTex;
}

ID CTexture::getId()
{
	return(m_id);
}

//#############################################################################

CMaterial::CMaterial(ID id):
	m_id(id)
{
}

void CMaterial::getMainTexture(IXTexture **ppTexture)
{
	ID id = SMtrl_MtlGetTextureID(m_id);
	*ppTexture = new CTexture(id);
}

ID CMaterial::getId()
{
	return(m_id);
}

bool CMaterial::isTransparent()
{
	return(SMtrl_MtlGetTransparency(m_id));
}
