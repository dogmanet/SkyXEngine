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

	cleanData();
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
	SGCore_GetDXDevice()->getThreadContext()->setVSConstant(m_pObjectConstantBuffer, SCR_OBJECT);
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
		SGCore_GetDXDevice()->getThreadContext()->setPSTexture(pTex, slot);
		mem_release(pTex);
	}
	else
	{
		SGCore_GetDXDevice()->getThreadContext()->setPSTexture(NULL, slot);
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

XVertexFormatHandler* XMETHODCALLTYPE CMaterialSystem::registerVertexFormat(const char *szName, XVertexOutputElement *pDecl)
{
	if(m_mVertexFormats.KeyExists(szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "CMaterialSystem::registerVertexFormat(): format '%s' already been registered!\n", szName);
		return(NULL);
	}

	VertexFormatData &format = m_mVertexFormats[szName];

	XVertexOutputElement *pTmp = pDecl;
	int iCount = 0;
	while(pTmp->szName)
	{
		++iCount;
		++pTmp;
	}

	format.aDecl.reserve(iCount);
	pTmp = pDecl;
	while(pTmp->szName)
	{
		XVertexOutputElement tmp = *pTmp;
		tmp.szName = strdup(tmp.szName);
		format.aDecl.push_back(tmp);
		++pTmp;
	}

	updateReferences();

	return(&format);
}
void XMETHODCALLTYPE CMaterialSystem::unregisterVertexFormat(const char *szName)
{
	assert(!"Not implemented");
	//! @todo: implement me!

	updateReferences();
}
XVertexFormatHandler* XMETHODCALLTYPE CMaterialSystem::getVertexFormat(const char *szName)
{
	const AssotiativeArray<String, VertexFormatData>::Node *pNode;
	if(m_mVertexFormats.KeyExists(szName, &pNode))
	{
		return(pNode->Val);
	}
	return(NULL);
}

XVertexShaderHandler* XMETHODCALLTYPE CMaterialSystem::registerVertexShader(XVertexFormatHandler *pVertexFormat, const char *szShaderFile, GXMacro *pDefines)
{
	assert(pVertexFormat);
	assert(szShaderFile);

	VertexFormatData *pVertexFormatData = (VertexFormatData*)pVertexFormat;

	VertexShaderData *vsData = m_poolVSdata.Alloc();
	vsData->pVertexFormat = pVertexFormatData;
	vsData->idShader = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, szShaderFile, NULL, pDefines);
	vsData->szShaderFile = strdup(szShaderFile);

	while(pDefines && pDefines->szName)
	{
		GXMacro tmp;
		tmp.szName = strdup(pDefines->szName);
		tmp.szDefinition = strdup(pDefines->szDefinition);
		vsData->aDefines.push_back(tmp);
		++pDefines;
	}

	pVertexFormatData->aVS.push_back(vsData);

	updateReferences();

	return(vsData);
}

void XMETHODCALLTYPE CMaterialSystem::bindVS(XVertexShaderHandler *pVertexShader)
{
	m_pCurrentVS = (VertexShaderData*)pVertexShader;
}

XGeometryShaderHandler* XMETHODCALLTYPE CMaterialSystem::registerGeometryShader(const char *szShaderFile, const char **aszRequiredParameters, GXMacro *pDefines)
{
	assert(szShaderFile);

	GeometryShaderData *gsData = m_poolGSdata.Alloc();
	//vsData->idShader = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, szShaderFile, NULL, pDefines);
	gsData->szShaderFile = strdup(szShaderFile);

	while(aszRequiredParameters)
	{
		gsData->aszRequiredParameters.push_back(*aszRequiredParameters);
		++aszRequiredParameters;
	}

	while(pDefines && pDefines->szName)
	{
		GXMacro tmp;
		tmp.szName = strdup(pDefines->szName);
		tmp.szDefinition = strdup(pDefines->szDefinition);
		gsData->aDefines.push_back(tmp);
		++pDefines;
	}

	updateReferences();

	return(gsData);
}
void XMETHODCALLTYPE CMaterialSystem::bindGS(XGeometryShaderHandler *pGeometryShader)
{
	m_pCurrentGS = (GeometryShaderData*)pGeometryShader;
}


void CMaterialSystem::updateReferences()
{
	for(AssotiativeArray<String, VertexFormatData>::Iterator i = m_mVertexFormats.begin(); i; i++)
	{
		VertexFormatData *pFormat = i.second;
		for(UINT j = 0, jl = m_aGeometryShaders.size(); j < jl; ++j)
		{
			GeometryShaderData *pGS = m_aGeometryShaders[j];

			if(pFormat->aGS.size() <= j || !pFormat->aGS[j])
			{
				/*
				$iFound = 0;
				$aPassParameters = [];
				foreach($data['decl'] as $decl)
				{
					if(in_array($decl['key'], $requiredParameters))
					{
						// echo("using: {$decl['key']}\n");
						++$iFound;
					}
					else
					{
						// echo("passing: {$decl['key']}\n");
						$aPassParameters[] = $decl;
					}
				}
				if($iFound + count($aPassParameters) != count($data['decl']))
				{
					// var_dump($iFound, count($aPassParameters), count($data['decl']));
					echo("Cannot use shader {$sFile} with {$sFormat} format. Skipping.\n");
					$data['GS'][$key] = null;
				}
				else
				{
					// print_r($aPassParameters);
				}
				$defines = [];
				$aStruct = [];
				$aPass = [];
				foreach($aPassParameters as $decl)
				{
					$aStruct[] = $this->getHLSLType($decl['type'])." {$decl['key']}: ".$this->getHLSLSemantic($decl['usage']);
					$aPass[] = "(dst).{$decl['key']} = (src).{$decl['key']}";
				}
				$defines['XMAT_GS_STRUCT()'] = implode('; ', $aStruct);
				$defines['XMAT_GS_PASS(dst, src)'] = implode('; ', $aPass);
					
				$data['GS'][$key] = [
					'file' => $sFile,
					'defines' => array_merge($aDefines, $defines),
					'isCompiled' => false,
				];
				*/
			}
		}
	}
	/*
		foreach($this->m_aGeometryShaders as $key => &$aGS)
		{
			$sFile = $aGS['file'];
			$requiredParameters = $aGS['params'];
			$aDefines = $aGS['defines'];
			
			
			foreach($this->m_aVertexFormats as $sFormat => &$data)
			{
				if(!isset($data['GS'][$key]))
				{
					
				}
			}
		}
	*/
}

void CMaterialSystem::cleanData() 
{
	for(AssotiativeArray<String, VertexFormatData>::Iterator i = m_mVertexFormats.begin(); i; i++)
	{
		for(UINT j = 0, jl = i.second->aDecl.size(); j < jl; ++j)
		{
			free((void*)i.second->aDecl[j].szName);
		}

		for(UINT j = 0, jl = i.second->aVS.size(); j < jl; ++j)
		{
			for(UINT k = 0, kl = i.second->aVS[j]->aDefines.size(); k < kl; ++k)
			{
				free((void*)i.second->aVS[j]->aDefines[k].szDefinition);
				free((void*)i.second->aVS[j]->aDefines[k].szName);
			}

			m_poolVSdata.Delete(i.second->aVS[j]);
		}
	}

	for(UINT i = 0, l = m_aGeometryShaders.size(); i < l; ++i)
	{
		for(UINT k = 0, kl = m_aGeometryShaders[i]->aDefines.size(); k < kl; ++k)
		{
			free((void*)m_aGeometryShaders[i]->aDefines[k].szDefinition);
			free((void*)m_aGeometryShaders[i]->aDefines[k].szName);
		}

		for(UINT k = 0, kl = m_aGeometryShaders[i]->aszRequiredParameters.size(); k < kl; ++k)
		{
			free((void*)m_aGeometryShaders[i]->aszRequiredParameters[k]);
		}

		m_poolGSdata.Delete(m_aGeometryShaders[i]);
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

void XMETHODCALLTYPE CMaterial::getMainTexture(IXTexture **ppTexture)
{
	*ppTexture = NULL;
	/*ID id = SMtrl_MtlGetTextureID(m_id);
	*ppTexture = new CTexture(id);*/
}

ID CMaterial::getId()
{
	return(m_id);
}

bool XMETHODCALLTYPE CMaterial::isTransparent()
{
	return(SMtrl_MtlGetTransparency(m_id));
}

bool XMETHODCALLTYPE CMaterial::isRefractive()
{
	return(SMtrl_MtlGetRefractivity(m_id));
}

bool XMETHODCALLTYPE CMaterial::isBlurred()
{
	return(false);
}

void XMETHODCALLTYPE CMaterial::setFlag(const char *szFlag, bool isSet) {}
bool XMETHODCALLTYPE CMaterial::getFlag(const char *szFlag)
{
	return(false);
}

void XMETHODCALLTYPE CMaterial::setParam(const char *szFlag, float fValue) {}
float XMETHODCALLTYPE CMaterial::getParam(const char *szFlag)
{
	return(0.0f);
}
