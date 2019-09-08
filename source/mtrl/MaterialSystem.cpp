#include "MaterialSystem.h"
#include "sxmtrl.h"
#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/resource/IXResourceTexture.h>

CMaterialSystem::CMaterialSystem()
{
	if(SGCore_GetDXDevice())
	{
		m_pObjectConstantBuffer = SGCore_GetDXDevice()->createConstantBuffer(sizeof(CObjectData));
	}

	{
		auto pPluginManager = Core_GetIXCore()->getPluginManager();

		IXTextureProxy *pProxy;
		UINT ic = 0;
		while((pProxy = (IXTextureProxy*)pPluginManager->getInterface(IXTEXTUREPROXY_GUID, ic++)))
		{
			if(pProxy->getVersion() == IXTEXTUREPROXY_VERSION)
			{
				LibReport(REPORT_MSG_LEVEL_NOTICE, "Registered texture proxy:\n %s\n\n", pProxy->getDescription());

				m_aTextureProxies.push_back(pProxy);
			}
		}
	}
}
CMaterialSystem::~CMaterialSystem()
{
	mem_release(m_pObjectConstantBuffer);
}

void XMETHODCALLTYPE CMaterialSystem::loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount, XSHADER_VARIANT_DESC *pVariantsDesc)
{
	ID id = SMtrl_MtlLoad2(szName, pDefaultShaders, uVariantCount, pVariantsDesc);
	*ppMaterial = new CMaterial(id);
}
bool XMETHODCALLTYPE CMaterialSystem::getMaterial(const char *szName, IXMaterial **ppMaterial)
{
	ID id = SMtrl_MtlGetId(szName);
	if(!ID_VALID(id))
	{
		return(false);
	}
	*ppMaterial = new CMaterial(id);
	return(true);
}

bool XMETHODCALLTYPE CMaterialSystem::loadTexture(const char *szName, IXTexture **ppTexture)
{
	String sName = szName;

	const AssotiativeArray<String, CTexture*>::Node *pNode;
	if(m_mpTextures.KeyExists(sName, &pNode) && *(pNode->Val))
	{
		*ppTexture = *(pNode->Val);
		(*ppTexture)->AddRef();
		return(true);
	}

	UINT uFileNameLength = 0;
	bool isFound = false;
	UINT i, l;
	for(i = 0, l = m_aTextureProxies.size(); i < l; ++i)
	{
		IXTextureProxy *pProxy = m_aTextureProxies[i];

		if(pProxy->resolveName(szName, NULL, &uFileNameLength))
		{
			isFound = true;
			break;
		}
	}

	char *szFileName = isFound ? ((char*)alloca(sizeof(char) * uFileNameLength)) : strdupa(szName);
	if(isFound)
	{
		m_aTextureProxies[i]->resolveName(szName, szFileName, &uFileNameLength);
	}

	IXResourceTexture *pRes = NULL;
	bool useStub = false;
	while(true)
	{
		if(Core_GetIXCore()->getResourceManager()->getTexture(useStub ? "textures/dev/dev_null.dds" : szFileName, &pRes))
		{
			UINT uWidth = 0;
			UINT uHeight = 0;
			switch(pRes->getType())
			{
			case GXTEXTURE_TYPE_2D:
			{
				IXResourceTexture2D *pRes2D = pRes->as2D();
				uWidth = pRes2D->getWidth();
				uHeight = pRes2D->getHeight();
			}
			break;
			case GXTEXTURE_TYPE_CUBE:
			{
				IXResourceTextureCube *pResCube = pRes->asCube();
				uWidth = uHeight = pResCube->getSize();
			}
			break;
			default:
				assert(!"Unknown format!");
			}

			if((uWidth & (uWidth - 1)) != 0 || (uHeight & (uHeight - 1)) != 0)
			{
				LibReport(REPORT_MSG_LEVEL_WARNING, "Texture '%s' is non power of two (%ux%u)!\n", szFileName, uWidth, uHeight);
			}
			if(pRes->getMipmapCount() == 1)
			{
				LibReport(REPORT_MSG_LEVEL_WARNING, "Texture '%s' has no mipmaps!\n", szFileName);
			}

			//CTexture *pTex = new CTexture(this, pRes);
			CTexture *pTex = m_poolTextures.Alloc(this, pRes);
			
			m_mpTextures[sName] = pTex;
			pTex->setName(m_mpTextures.TmpNode->Key.c_str());

			*ppTexture = pTex;
			return(true);
		}

		if(useStub)
		{
			LibReport(REPORT_MSG_LEVEL_FATAL, "Unable to load stub texture (textures/dev/dev_null.dds)! Exiting now!\n");
		}

		useStub = true;
	}
	
	return(false);
}

bool XMETHODCALLTYPE CMaterialSystem::getTexture(const char *szName, IXTexture **ppTexture)
{
	const AssotiativeArray<String, CTexture*>::Node *pNode;
	if(m_mpTextures.KeyExists(szName, &pNode) && *(pNode->Val))
	{
		*ppTexture = *(pNode->Val);
		(*ppTexture)->AddRef();
		return(true);
	}

	return(false);
}
//void XMETHODCALLTYPE CMaterialSystem::addTexture(const char *szName, IGXTexture2D *pTexture)
//{
//	pTexture->AddRef();
//	SGCore_LoadTexCreate(szName, pTexture);
//}

void XMETHODCALLTYPE CMaterialSystem::setWorld(const SMMATRIX &mWorld)
{
	//SMMATRIX mV, mP;
	//Core_RMatrixGet(G_RI_MATRIX_VIEW, &mV);
	//Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mP);
	m_objectData.m_mW = SMMatrixTranspose(mWorld);
	// m_objectData.m_mWV = SMMatrixTranspose(mV) * m_objectData.m_mW;
	// m_objectData.m_mWVP = SMMatrixTranspose(mP) * m_objectData.m_mWV;
	

	m_pObjectConstantBuffer->update(&m_objectData);
	SGCore_GetDXDevice()->getDirectContext()->setVSConstant(m_pObjectConstantBuffer, SCR_OBJECT);
	//SGCore_GetDXDevice()->setPixelShaderConstant(m_pObjectConstantBuffer, SCR_OBJECT);
}
void XMETHODCALLTYPE CMaterialSystem::bindMaterial(IXMaterial *pMaterial, IXShaderVariant *pShaderVariant)
{
	CMaterial *pMat = (CMaterial*)pMaterial;
	SMtrl_MtlRender(pMaterial ? pMat->getId() : 0, NULL);
	// SGCore_MtlSet(pMat->getId(), NULL);
}
void XMETHODCALLTYPE CMaterialSystem::bindTexture(IXTexture *pTexture, UINT slot)
{
	if(pTexture)
	{
		IGXBaseTexture *pTex;
		pTexture->getAPITexture(&pTex);
		SGCore_GetDXDevice()->getDirectContext()->setPSTexture(pTex, slot);
		mem_release(pTex);
	}
	else
	{
		SGCore_GetDXDevice()->getDirectContext()->setPSTexture(NULL, slot);
	}
}

void XMETHODCALLTYPE CMaterialSystem::overridePixelShader(ID id)
{
	SMtrl_MtlPixelShaderOverride(id);
}

void XMETHODCALLTYPE CMaterialSystem::overrideGeometryShader(ID id)
{
	SMtrl_MtlGeometryShaderOverride(id);
}

void CMaterialSystem::onTextureRelease(CTexture *pTexture)
{
	assert(pTexture);

	m_mpTextures[pTexture->getName()] = NULL;

	m_poolTextures.Delete(pTexture);
}

void CMaterialSystem::queueTextureUpload(CTexture *pTexture)
{
	m_queueTextureToLoad.push(pTexture);
}

void CMaterialSystem::update(float fDT)
{
	for(UINT i = 0, l = m_queueTextureToLoad.size(); i < l; ++i)
	{
		CTexture *pTexture = m_queueTextureToLoad.pop();
		pTexture->initGPUresources();
		mem_release(pTexture);
	}
}

//#############################################################################

CTexture::CTexture(CMaterialSystem *pMaterialSystem, IXResourceTexture *m_pResource):
	m_pMaterialSystem(pMaterialSystem),
	m_pResource(m_pResource)
{
	m_uFrameCount = m_pResource->getFrameCount();
	m_fFrameTime = m_pResource->getFrameTime();

	m_type = m_pResource->getType();

	switch(m_type)
	{
	case GXTEXTURE_TYPE_2D:
		{
			IXResourceTexture2D *pRes2D = m_pResource->as2D();
			m_uWidth = pRes2D->getWidth();
			m_uHeight = pRes2D->getHeight();
		}
		break;
	case GXTEXTURE_TYPE_CUBE:
		{
			IXResourceTextureCube *pResCube = m_pResource->asCube();
			m_uWidth = m_uHeight = pResCube->getSize();
		}
		break;
	default:
		assert(!"Unknown texture type!");
	}

	if(Core_GetIXCore()->isOnMainThread())
	{
		initGPUresources();
	}
	else
	{
		AddRef();
		m_pMaterialSystem->queueTextureUpload(this);
	}
}

CTexture::~CTexture()
{
	mem_release(m_pResource);

	for(UINT i = 0; i < m_uFrameCount; ++i)
	{
		mem_release(m_ppGXTexture[i]);
	}
	mem_delete_a(m_ppGXTexture);
}

void XMETHODCALLTYPE CTexture::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pMaterialSystem->onTextureRelease(this);
	}
}

void XMETHODCALLTYPE CTexture::getAPITexture(IGXBaseTexture **ppTexture, UINT uFrame)
{
	assert(uFrame < m_uFrameCount);

	if(m_ppGXTexture && m_ppGXTexture[uFrame])
	{
		m_ppGXTexture[uFrame]->AddRef();
		*ppTexture = m_ppGXTexture[uFrame];
	}
	else
	{
		*ppTexture = NULL;
	}
}

bool XMETHODCALLTYPE CTexture::isReady() const
{
	return(!!m_ppGXTexture);
}

void CTexture::initGPUresources()
{
	if(m_ppGXTexture)
	{
		return;
	}

	IGXDevice *pDevice = SGCore_GetDXDevice();
	if(!pDevice)
	{
		return;
	}

	switch(m_pResource->getType())
	{
	case GXTEXTURE_TYPE_2D:
		{
			UINT uMipCount = m_pResource->getMipmapCount();
			GXImageMip *pMips = (GXImageMip*)alloca(sizeof(GXImageMip) * uMipCount);
			IXResourceTexture2D *pRes2D = m_pResource->as2D();
			XImageMip *pMip;

			IGXBaseTexture **ppGXTexture = new IGXBaseTexture*[m_uFrameCount];

			for(UINT f = 0; f < m_uFrameCount; ++f)
			{
				for(UINT i = 0; i < uMipCount; ++i)
				{
					pMip = pRes2D->getMip(i, f);
					pMips[i].pData = pMip->pData;
					pMips[i].sizeData = pMip->sizeData;
				}

				ppGXTexture[f] = pDevice->createTexture2D(pRes2D->getWidth(), pRes2D->getHeight(), uMipCount, GX_TEXFLAG_INIT_ALL_MIPS, pRes2D->getFormat(), pMips);
				assert(ppGXTexture[f]);
			}
			m_ppGXTexture = ppGXTexture;
		}
		break;
	case GXTEXTURE_TYPE_CUBE:
		{
			UINT uMipCount = m_pResource->getMipmapCount();
			GXImageMip *pMips = (GXImageMip*)alloca(sizeof(GXImageMip) * uMipCount * 6);
			IXResourceTextureCube *pResCube = m_pResource->asCube();
			XImageMip *pMip;

			IGXBaseTexture **ppGXTexture = new IGXBaseTexture*[m_uFrameCount];

			for(UINT f = 0; f < m_uFrameCount; ++f)
			{
				for(UINT s = 0; s < 6; ++s)
				{
					for(UINT i = 0; i < uMipCount; ++i)
					{
						pMip = pResCube->getMip((GXCUBEMAP_FACES)s, i, f);
						pMips[i + s * uMipCount].pData = pMip->pData;
						pMips[i + s * uMipCount].sizeData = pMip->sizeData;
					}
				}
				ppGXTexture[f] = pDevice->createTextureCube(pResCube->getSize(), uMipCount, GX_TEXFLAG_INIT_ALL_MIPS, pResCube->getFormat(), pMips);
				assert(ppGXTexture[f]);
			}
			m_ppGXTexture = ppGXTexture;
		}
		break;

	default:
		assert(!"Unsupported texture type!");
	}
}

void CTexture::setName(const char *szName)
{
	m_szName = szName;
}
const char* CTexture::getName() const
{
	return(m_szName);
}

UINT XMETHODCALLTYPE CTexture::getNumFrames() const
{
	return(m_uFrameCount);
}
float XMETHODCALLTYPE CTexture::getFrameTime() const
{
	return(m_fFrameTime);
}

GXTEXTURE_TYPE XMETHODCALLTYPE CTexture::getType() const
{
	return(m_type);
}

UINT XMETHODCALLTYPE CTexture::getWidth() const
{
	return(m_uWidth);
}
UINT XMETHODCALLTYPE CTexture::getHeight() const
{
	return(m_uHeight);
}
UINT XMETHODCALLTYPE CTexture::getDepth() const
{
	return(m_uDepth);
}

//#############################################################################

CMaterial::CMaterial(ID id):
	m_id(id)
{
}

void CMaterial::getMainTexture(IXTexture **ppTexture)
{
	*ppTexture = NULL;
	/*ID id = SMtrl_MtlGetTextureID(m_id);
	*ppTexture = new CTexture(id);*/
}

ID CMaterial::getId()
{
	return(m_id);
}

bool CMaterial::isTransparent()
{
	return(SMtrl_MtlGetTransparency(m_id));
}

bool CMaterial::isRefractive()
{
	return(SMtrl_MtlGetRefractivity(m_id));
}

bool CMaterial::isBlurred()
{
	return(false);
}
