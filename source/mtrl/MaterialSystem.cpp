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

	auto pPluginManager = Core_GetIXCore()->getPluginManager();

	{
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

	{
		IXMaterialProxy *pProxy;
		UINT ic = 0;
		while((pProxy = (IXMaterialProxy*)pPluginManager->getInterface(IXMATERIALPROXY_GUID, ic++)))
		{
			LibReport(REPORT_MSG_LEVEL_NOTICE, "Registered material proxy:\n %s\n\n", pProxy->getDescription());

			m_aMaterialProxies.push_back(pProxy);
		}
	}

	{
		IXMaterialLoader *pLoader;
		UINT ic = 0;
		while((pLoader = (IXMaterialLoader*)pPluginManager->getInterface(IXMATERIALLOADER_GUID, ic++)))
		{
			LibReport(REPORT_MSG_LEVEL_NOTICE, "Registered material loader:\n %s\n By %s\n %s\n", pLoader->getDescription(), pLoader->getAuthor(), pLoader->getCopyright());

			for(UINT i = 0, l = pLoader->getExtCount(); i < l; ++i)
			{
				AAString sExt;
				sExt.setName(pLoader->getExt(i));
				strlwr(const_cast<char*>(sExt.getName()));
				m_mapMaterialLoaders[sExt].push_back({pLoader, pLoader->canSave(i)});
				m_aMaterialExts.push_back({pLoader->getExtText(i), pLoader->getExt(i)});
				LibReport(REPORT_MSG_LEVEL_NOTICE, " Ext: " COLOR_LCYAN "%s" COLOR_RESET ": " COLOR_WHITE "%s" COLOR_RESET "\n", pLoader->getExt(i), pLoader->getExtText(i));
			}
			LibReport(REPORT_MSG_LEVEL_NOTICE, " \n");
		}
	}

	m_pNotifyChannel = Core_GetIXCore()->getEventChannel<XEventMaterialChanged>(EVENT_MATERIAL_CHANGED_GUID);

	loadTexture("textures/dev/dev_null.dds", &m_pDefaultTexture);
}
CMaterialSystem::~CMaterialSystem()
{
	mem_release(m_pObjectConstantBuffer);
	mem_release(m_pDefaultTexture);

	cleanData();
}

void XMETHODCALLTYPE CMaterialSystem::loadMaterial(const char *szName, IXMaterial **ppMaterial, const char *szDefaultShader)
{
	String sName(szName);

	const AssotiativeArray<String, CMaterial*>::Node *pNode;
	if(m_mapMaterials.KeyExists(sName, &pNode) && *(pNode->Val))
	{
		*ppMaterial = *(pNode->Val);
		(*ppMaterial)->AddRef();
		return/*(true)*/;
	}


	UINT uFileNameLength = 0;
	bool isFound = false;
	UINT i, l;
	for(i = 0, l = m_aMaterialProxies.size(); i < l; ++i)
	{
		IXMaterialProxy *pProxy = m_aMaterialProxies[i];

		if(pProxy->resolveName(szName, NULL, &uFileNameLength))
		{
			isFound = true;
			break;
		}
	}

	char *szFileName = isFound ? ((char*)alloca(sizeof(char) * uFileNameLength)) : strdupa(szName);
	if(isFound)
	{
		m_aMaterialProxies[i]->resolveName(szName, szFileName, &uFileNameLength);
	}

	m_mapMaterials[sName] = NULL;
	m_mapMaterials.KeyExists(sName, &pNode);

	CMaterial *pNewMaterial = NULL;

	// ID id = SMtrl_MtlLoad2(szName, pDefaultShaders, uVariantCount, pVariantsDesc);
	pNewMaterial = new CMaterial(this, pNode->Key.c_str());
	*ppMaterial = pNewMaterial;
	m_mapMaterials[sName] = pNewMaterial;

	if(!loadMaterialFromFile(szFileName, pNewMaterial))
	{
		pNewMaterial->setShader(szDefaultShader ? szDefaultShader : "Default");
		pNewMaterial->setTexture("txBase", szName);
	}
	else
	{
		pNewMaterial->setProxy(m_aMaterialProxies[i]);
	}
}
bool XMETHODCALLTYPE CMaterialSystem::getMaterial(const char *szName, IXMaterial **ppMaterial)
{
	const AssotiativeArray<String, CMaterial*>::Node *pNode;
	if(m_mapMaterials.KeyExists(szName, &pNode) && *(pNode->Val))
	{
		*ppMaterial = *(pNode->Val);
		(*ppMaterial)->AddRef();
		return(true);
	}

	return(false);
}

const char* GetFileExtension(const char *szName)
{
	size_t iLen = strlen(szName);
	const char *szExt = NULL;
	for(size_t i = iLen - 1; i > 0; --i)
	{
		if(szName[i] == '.')
		{
			szExt = &szName[i + 1];
			break;
		}
	}

	return(szExt);
}

bool CMaterialSystem::loadMaterialFromFile(const char *szName, CMaterial *pMaterial)
{
	char *szFileName = strdupa(szName);
	char *szArg = NULL;
	char *szPound = strstr(szFileName, "#");
	if(szPound)
	{
		szArg = szPound + 1;
		*szPound = 0;
	}

	const char *szExt = GetFileExtension(szFileName);

	if(!szExt || !*szExt)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Invalid material name '%s'. Missing extension\n", szName);
		return(false);
	}

	char *szLowcaseExt = strdupa(szExt);
	strlwr(szLowcaseExt);

	const AssotiativeArray<AAString, Array<MaterialLoader>>::Node *pNode;
	if(m_mapMaterialLoaders.KeyExists(AAString(szLowcaseExt), &pNode))
	{
		auto &aLoaders = *pNode->Val;
		bool isSuccess = false;
		for(UINT i = 0, l = aLoaders.size(); i < l; ++i)
		{
			IXMaterialLoader *pLoader = aLoaders[i].pLoader;
			if(pLoader->open(szFileName, szArg))
			{
				if(pLoader->load(pMaterial))
				{
					isSuccess = true;
				}
				
				pLoader->close();

				if(isSuccess)
				{
					break;
				}
			}
		}

		if(isSuccess)
		{
			return(true);
		}

		LibReport(REPORT_MSG_LEVEL_ERROR, "No one plugin could load material '%s'\n", szName);
		return(false);
	}

	LibReport(REPORT_MSG_LEVEL_ERROR, "No plugins for material extension found '%s'\n", szName);
	return(false);
}

bool XMETHODCALLTYPE CMaterialSystem::loadTexture(const char *szName, IXTexture **ppTexture)
{
	String sName(szName);

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
void XMETHODCALLTYPE CMaterialSystem::bindMaterial(IXMaterial *pMaterial)
{
	if(m_pCurrentRP && m_pCurrentRP->bSkipMaterialShader)
	{
		if(!m_pCurrentVS)
		{
			return;
		}

		MaterialVariantVS *pVS = &m_pCurrentRP->aPassFormats[m_pCurrentVS->pVertexFormat->uID].aPassVariants[m_uCurrentRPvariant].aVertexShaders[m_pCurrentVS->uID];
		ID idShaderSet = -1;
		if(m_pCurrentGS)
		{
			idShaderSet = pVS->aGeometryShaders[m_pCurrentGS->uID];
		}
		else
		{
			idShaderSet = pVS->idSet;
		}

		SGCore_ShaderBind(idShaderSet);

		return;
	}

	CMaterial *pMat = (CMaterial*)pMaterial;
	if(pMaterial)
	{
		MaterialShader *pShader = (MaterialShader*)pMat->getShaderHandler();
		if(pShader && m_pCurrentVS)
		{
			for(UINT i = 0, l = pShader->aPasses.size(); i < l; ++i)
			{
				if(pShader->aPasses[i].pRenderPass == m_pCurrentRP)
				{
					MaterialShaderPassData *pPass = &pShader->aPasses[i];

					pMat->preparePass(i);

					UINT uVariant = 0;
					if(pMat->isDirty())
					{
						Array<MaterialProperty*> aStaticList;
						Array<MaterialProperty*> aVariableList;
						for(UINT j = 0, jl = pPass->aAllProperties.size(); j < jl; ++j)
						{
							MaterialProperty *pProp = pPass->aAllProperties[j];
							if(pProp->pCondition)
							{
								aVariableList.push_back(pProp);
							}
							else
							{
								aStaticList.push_back(pProp);
							}
						}

						bool bDoAgain = true;
						while(bDoAgain)
						{
							bDoAgain = false;
							for(UINT key = 0, keyl = aVariableList.size(); key < keyl; ++key)
							{
								MaterialProperty *pProp = aVariableList[key];

								bool isFound = false;

								if(!pProp->pCondition)
								{
									isFound = true;
								}
								else
								{
									pProp->pCondition->resetParams();
									for(UINT j = 0, jl = aStaticList.size(); j < jl; ++j)
									{
										pProp->pCondition->setParam(aStaticList[j]->prop.szDefine, true);
									}
									isFound = pProp->pCondition->evaluate();
								}

								if(isFound)
								{
									aStaticList.push_back(pProp);
									aVariableList.erase(key);
									bDoAgain = true;
									break;
								}
							}
						}

						
						for(UINT key = 0, keyl = aStaticList.size(); key < keyl; ++key)
						{
							XMaterialProperty *pProp = &aStaticList[key]->prop;
							bool isFound = false;
							switch(pProp->type)
							{
							case XMPT_PARAM_TEXTURE_OPT:
							case XMPT_PARAM_TEXTURE_CUBE_OPT:
								isFound = pMat->getTextureName(pProp->szKey)[0];
								break;
							case XMPT_PARAM_FLAG:
								isFound = pMat->getFlag(pProp->szKey);
								break;
							case XMPT_GLOBAL_FLAG:
								isFound = getFlag(pProp->szKey);
								break;
							}

							if(isFound)
							{
								int iIndex = pPass->aAllDefines.indexOf(pProp->szDefine, [](const MaterialDefine &a, const char *b){
									return(!fstrcmp(a.szName, b));
								});
								if(iIndex >= 0)
								{
									uVariant += 1 << iIndex;
								}
							}
						}
					
						pMat->setCachedVariant(uVariant);

						MaterialVariant *pVariant = &pPass->aVariants[uVariant];

						pMat->clearTextureBindings();
						for(UINT j = 0, jl = pVariant->aTextureMap.size(); j < jl; ++j)
						{
							if(pVariant->aTextureMap[j].szName)
							{
								pMat->bindTextureToSlot(pVariant->aTextureMap[j].szName, j);
							}
						}

						pMat->initConstantsBindings(pVariant->uConstantsSize);
						for(UINT j = 0, jl = pVariant->aConstants.size(); j < jl; ++j)
						{
							MaterialShaderConstants *pConst = &pVariant->aConstants[j];
							pMat->bindConstantToOffset(pConst->szKey, pConst->uOffset, getTypeSize(pConst->type) * 4, pConst->vDefault);
						}

						// XMPT_STATIC_PARAM
					}
					else
					{
						uVariant = pMat->getCachedVariant();
					}
					MaterialVariant *pVariant = &pPass->aVariants[uVariant];

					// pVariant->

					auto *pCtx = SGCore_GetDXDevice()->getThreadContext();

					for(UINT j = 0, jl = pPass->aTotalSamplers.size(); j < jl; ++j)
					{
						if(pPass->aTotalSamplers[j].pSampler)
						{
							pCtx->setSamplerState(pPass->aTotalSamplers[j].pSampler, j);
						}
					}

					MaterialVariantVS *pVS = &pVariant->aPassVariants[m_uCurrentRPvariant].aVertexShaders[m_pCurrentVS->uID];
					ID idShaderSet = -1;
					if(m_pCurrentGS)
					{
						idShaderSet = pVS->aGeometryShaders[m_pCurrentGS->uID];
					}
					else
					{
						idShaderSet = pVS->idSet;
					}

					SGCore_ShaderBind(idShaderSet);

					IXTexture *pTex = NULL;
					for(UINT j = 0, jl = pVariant->aTextureMap.size(); j < jl; ++j)
					{
						if(pVariant->aTextureMap[j].szName)
						{
							pTex = pMat->getTextureForSlot(j);
							bindTexture(pTex ? pTex : m_pDefaultTexture, j);
						}
					}
					
					pCtx->setPSConstant(pMat->getConstants(), SCR_XMATERIAL);
					
					break;
				}
			}

			return;
		}
	}

	// SMtrl_MtlRender(pMaterial ? pMat->getId() : 0, NULL);
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

void CMaterialSystem::onTextureRelease(CTexture *pTexture)
{
	assert(pTexture);

	m_mpTextures[pTexture->getName()] = NULL;

	m_poolTextures.Delete(pTexture);
}

void CMaterialSystem::onMaterialRelease(CMaterial *pMaterial)
{
	assert(pMaterial);

	m_mapMaterials[pMaterial->getName()] = NULL;
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
	format.uID = m_mVertexFormats.Size() - 1;

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

	m_isMateriallesRenderPassDirty = true;

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
	vsData->uID = pVertexFormatData->aVS.size();
	pVertexFormatData->aVS.push_back(vsData);

	m_isMateriallesRenderPassDirty = true;

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

	GeometryShader *gsData = m_poolGSdata.Alloc();
	//vsData->idShader = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, szShaderFile, NULL, pDefines);
	gsData->szShaderFile = strdup(szShaderFile);

	while(aszRequiredParameters && *aszRequiredParameters)
	{
		gsData->aszRequiredParameters.push_back(strdup(*aszRequiredParameters));
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

	gsData->uID = m_aGeometryShaders.size();
	m_aGeometryShaders.push_back(gsData);

	m_isMateriallesRenderPassDirty = true;

	updateReferences();

	return(gsData);
}
void XMETHODCALLTYPE CMaterialSystem::bindGS(XGeometryShaderHandler *pGeometryShader)
{
	m_pCurrentGS = (GeometryShader*)pGeometryShader;
}

XRenderPassHandler* XMETHODCALLTYPE CMaterialSystem::registerRenderPass(const char *szName, const char *szShaderFile, XRenderPassTexturesElement *pTextures, XRenderPassSamplersElement *pSamplers, XRenderPassOutputElement *pOutput, XRenderPassVariantElement *pVariants, bool bSkipMaterialShader)
{
	if(m_mRenderPasses.KeyExists(szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "CMaterialSystem::registerRenderPass(): pass '%s' already been registered!\n", szName);
		return(NULL);
	}

	RenderPass &pass = m_mRenderPasses[szName];

	pass.szName = strdup(szName);
	pass.szShaderFile = strdup(szShaderFile);
	pass.bSkipMaterialShader = bSkipMaterialShader;
	//assert(!bSkipMaterialShader && "bSkipMaterialShader is not currently supported!");

	if(bSkipMaterialShader)
	{
		m_isMateriallesRenderPassDirty = true;
	}

	while(pTextures && pTextures->szName)
	{
		XRenderPassTexturesElement tmp = *pTextures;
		tmp.szKey = strdup(tmp.szKey);
		tmp.szName = strdup(tmp.szName);

		pass.aTextures.push_back(tmp);
		++pTextures;
	}

	while(pSamplers && pSamplers->szName)
	{
		XRenderPassSamplersElement tmp = *pSamplers;
		tmp.szKey = strdup(tmp.szKey);
		tmp.szName = strdup(tmp.szName);

		pass.aSamplers.push_back(tmp);
		++pSamplers;
	}

	String sOutStruct;
	String sDefaultInitializer;
	bool isFirst = true;
	while(pOutput && pOutput->szName)
	{
		XRenderPassOutputElement tmp = *pOutput;
		tmp.szKey = strdup(tmp.szKey);
		tmp.szName = strdup(tmp.szName);
		tmp.szDefault = strdup(tmp.szDefault);
		
		if(!isFirst)
		{
			sOutStruct += "; ";
			sDefaultInitializer += "; ";
		}
		sOutStruct += String(getHLSLType(tmp.type)) + " " + tmp.szKey;
		sDefaultInitializer += String("OUT.") + tmp.szKey + " = " + String(tmp.szDefault);

		pass.aOutput.push_back(tmp);
		++pOutput;
		isFirst = false;
	}
	pass.aDefines.push_back({"XMATERIAL_OUTPUT_STRUCT()", strdup(sOutStruct.c_str())});
	pass.aDefines.push_back({"XMATERIAL_DEFAULT_LOADER()", strdup((String("XMaterial XMATERIAL_LOAD_DEFAULTS(){XMaterial OUT = (XMaterial)0; ") + sDefaultInitializer + "; return(OUT);}").c_str())});
	// pass.aDefines.push_back({"XMATERIAL_LOAD_DEFAULTS()", strdup("")});
	// aDefines

	Array<GXMacro> aMacro;
	pass.aVariants.push_back(aMacro);

	while(pVariants && pVariants->pMacro)
	{
		GXMacro *pMacro = pVariants->pMacro;
		while(pMacro && pMacro->szName)
		{
			aMacro.push_back({strdup(pMacro->szName), strdup(pMacro->szDefinition)});
			++pMacro;
		}
		pass.aVariants.push_back(aMacro);
		aMacro.clearFast();

		++pVariants;
	}

	updateReferences();

	return(&pass);
}
XRenderPassHandler* XMETHODCALLTYPE CMaterialSystem::getRenderPass(const char *szName)
{
	const AssotiativeArray<String, RenderPass>::Node *pNode;
	if(m_mRenderPasses.KeyExists(szName, &pNode))
	{
		return(pNode->Val);
	}
	return(NULL);
}
void XMETHODCALLTYPE CMaterialSystem::bindRenderPass(XRenderPassHandler *pRenderPass, UINT uVariant)
{
	m_pCurrentRP = (RenderPass*)pRenderPass;

	assert(uVariant < m_pCurrentRP->aVariants.size());

	if(uVariant >= m_pCurrentRP->aVariants.size())
	{
		uVariant = 0;
	}

	m_uCurrentRPvariant = uVariant;
}

static void CopyProps(XMaterialProperty *pProperties, Array<CMaterialSystem::MaterialProperty> &aTarget, const char *szShaderName)
{
	Stack<const char*> stack;
	while(pProperties && pProperties->type != XMPT_UNKNOWN)
	{
		CLogicExpression *pCondition = NULL;
		XMaterialProperty tmp = *pProperties;
		tmp.szTitle = strdups(tmp.szTitle);
		tmp.szKey = strdups(tmp.szKey);
		tmp.szDefine = strdups(tmp.szDefine);

		if(tmp.type == XMPT_PARAM_GROUP)
		{
			assert(tmp.szCondition);

			tmp.szCondition = strdup(tmp.szCondition);
			stack.push(tmp.szCondition);
		}
		else if(tmp.type == XMPT_PARAM_GROUP_END)
		{
			assert(stack.count() > 0);
			stack.popN(1);
		}
		else
		{
			size_t sizeTotal = 0;
			for(int i = 0, l = stack.count(); i < l; ++i)
			{
				sizeTotal += strlen(stack.get(i)) + 5;
			}
			/*
			CLogicExpression le;
			le.setExpression("A && !(B && C || D) || E");
			le.setParam("A", true);
			bool res = le.evaluate();
			*/
			if(sizeTotal)
			{
				char *szTemp = (char*)alloca(sizeTotal);
				//tmp.szCondition = szTemp;
				const char *szCondition = szTemp;
				for(int i = 0, l = stack.count(); i < l; ++i)
				{
					const char *tmp = stack.get(i);
					szTemp += sprintf(szTemp, "(%s)%s", tmp && tmp[0] ? tmp : "1", (i != l - 1) ? "&&" : "");
				}

				pCondition = new CLogicExpression();
				if(!pCondition->setExpression(szCondition))
				{
					LibReport(REPORT_MSG_LEVEL_ERROR, "CMaterialSystem::registerMaterialShader(): Invalid property conditions; shader '%s'\n  %s\n", szShaderName, szCondition);
				}
			}
		}


		aTarget.push_back({tmp, pCondition});
		++pProperties;
	}
	assert(stack.count() == 0);
}

XMaterialShaderHandler* XMETHODCALLTYPE CMaterialSystem::registerMaterialShader(const char *szName, XVertexFormatHandler *pVertexFormat, XMaterialShaderPass *pPasses, XMaterialProperty *pGenericProperties)
{
	assert(pVertexFormat);
	assert(pPasses);

	if(m_mMaterialShaders.KeyExists(szName))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "CMaterialSystem::registerMaterialShader(): shader '%s' has already been registered!\n", szName);
		return(NULL);
	}

	MaterialShader &shader = m_mMaterialShaders[szName];

	shader.szName = strdup(szName);
	shader.pVertexFormat = (VertexFormatData*)pVertexFormat;

	CopyProps(pGenericProperties, shader.aProperties, szName);

	for(UINT i = 0, l = shader.aProperties.size(); i < l; ++i)
	{
		auto *pProp = &shader.aProperties[i].prop;
		if(pProp->type == XMPT_STATIC_FLAG)
		{
			shader.aStaticFlags.push_back({pProp->szKey, pProp->fValue.x > 0.0f});
		}
	}

	while(pPasses && pPasses->pRenderPass)
	{
		MaterialShaderPassData *pPass = &shader.aPasses[shader.aPasses.size()];
		pPass->pRenderPass = (RenderPass*)pPasses->pRenderPass;
		pPass->isDirty = true;

		assert(!pPass->pRenderPass->bSkipMaterialShader && "Cannot use renderpass which is set to ignore material shaders!");

		pPass->szShaderFile = strdups(pPasses->szShaderFile);
		pPass->szEntryPoint = strdups(pPasses->szEntryPoint);

		{
			GXMacro *pTmp = pPasses->pDefines;
			while(pTmp && pTmp->szName)
			{
				GXMacro tmp;
				tmp.szName = strdup(pTmp->szName);
				tmp.szDefinition = strdup(pTmp->szDefinition);
				pPass->aDefines.push_back(tmp);
				++pTmp;
			}
		}

		for(UINT i = 0, l = pPass->pRenderPass->aDefines.size(); i < l; ++i)
		{
			GXMacro *pTmp = &pPass->pRenderPass->aDefines[i];
			GXMacro tmp;
			tmp.szName = strdup(pTmp->szName);
			tmp.szDefinition = strdup(pTmp->szDefinition);
			pPass->aDefines.push_back(tmp);
		}

		{
			IGXDevice *pDevice = SGCore_GetDXDevice();

			XMaterialShaderSampler *pTmp = pPasses->pSamplers;
			while(pTmp && pTmp->szKey)
			{
				MaterialShaderSamplerData tmp;
				tmp.szKey = strdup(pTmp->szKey);
				tmp.pSampler = pDevice ? pDevice->createSamplerState(&pTmp->samplerDesc) : NULL;
				assert(tmp.pSampler || !pDevice);

				pPass->aSamplers.push_back(tmp);
				++pTmp;
			}
		}

		CopyProps(pPasses->pProperties, pPass->aProperties, szName);
		
		++pPasses;
	}

	return(&shader);
}
XMaterialShaderHandler* XMETHODCALLTYPE CMaterialSystem::getMaterialShader(const char *szName)
{
	const AssotiativeArray<String, MaterialShader>::Node *pNode;
	if(m_mMaterialShaders.KeyExists(szName, &pNode))
	{
		return(pNode->Val);
	}
	return(NULL);
}

static void GetAllDefines(Array<CMaterialSystem::MaterialDefine> &aAllDefines, Array<CMaterialSystem::MaterialProperty> &aProperties, Array<CMaterialSystem::MaterialProperty*> &aAllProps)
{
	for(UINT i = 0, l = aProperties.size(); i < l; ++i)
	{
		if(aProperties[i].prop.szDefine)
		{
			if(aAllDefines.indexOf(aProperties[i].prop.szDefine, [](const CMaterialSystem::MaterialDefine &a, const char *b){
				return(!fstrcmp(a.szName, b));
			}) < 0)
			{
				aAllDefines.push_back({aProperties[i].prop.szDefine, aProperties[i].pCondition});
			}

			aAllProps.push_back(&aProperties[i]);
		}
	}
}

static bool EvalCondition(CLogicExpression *pExpr, Array<CMaterialSystem::MaterialDefine*> &aStaticList)
{
	if(!pExpr)
	{
		return(true);
	}
	pExpr->resetParams();
	for(UINT j = 0, jl = aStaticList.size(); j < jl; ++j)
	{
		pExpr->setParam(aStaticList[j]->szName, true);
	}
	return(pExpr->evaluate());
}

static void ParseTexturesConstants(Array<CMaterialSystem::MaterialProperty> &aProperties, Array<CMaterialSystem::MaterialDefine*> &aStaticList,
	Array<CMaterialSystem::MaterialTexture> &aTextures, Array<CMaterialSystem::MaterialShaderConstants> &aConstants)
{
	for(UINT i = 0, l = aProperties.size(); i < l; ++i)
	{
		CMaterialSystem::MaterialProperty *pProp = &aProperties[i];
		if(pProp->prop.type == XMPT_PARAM_TEXTURE || pProp->prop.type == XMPT_PARAM_TEXTURE_CUBE || (
				(pProp->prop.type == XMPT_PARAM_TEXTURE_OPT || 
				pProp->prop.type == XMPT_PARAM_TEXTURE_CUBE_OPT)
				&& (!pProp->prop.szDefine || aStaticList.indexOf(pProp->prop.szDefine, [](const CMaterialSystem::MaterialDefine *a, const char *b){
					return(!fstrcmp(a->szName, b));
				}) >= 0)
		))
		{
			if(EvalCondition(pProp->pCondition, aStaticList))
			{
				if(aTextures.indexOf(pProp->prop.szKey, [](const CMaterialSystem::MaterialTexture &a, const char *b){
					return(!fstrcmp(a.szName, b)); 
				}) < 0)
				{
					GXTEXTURE_TYPE type;
					switch(pProp->prop.type)
					{
					case XMPT_PARAM_TEXTURE:
					case XMPT_PARAM_TEXTURE_OPT:
						type = GXTEXTURE_TYPE_2D;
						break;

					case XMPT_PARAM_TEXTURE_CUBE:
					case XMPT_PARAM_TEXTURE_CUBE_OPT:
						type = GXTEXTURE_TYPE_CUBE;
						break;

					default:
						assert(!"Unknown type");
					}
					aTextures.push_back({pProp->prop.szKey, type});
				}
			}
		}
		else if(pProp->prop.varType != GXDECLTYPE_UNUSED)
		{
			if(EvalCondition(pProp->pCondition, aStaticList))
			{
				aConstants.push_back({pProp->prop.szKey, pProp->prop.varType, pProp->prop.fValue});
			}
		}
	}
}

void CMaterialSystem::updateReferences()
{
	for(AssotiativeArray<String, VertexFormatData>::Iterator i = m_mVertexFormats.begin(); i; i++)
	{
		VertexFormatData *pFormat = i.second;
		for(UINT j = 0, jl = m_aGeometryShaders.size(); j < jl; ++j)
		{
			GeometryShader *pGS = m_aGeometryShaders[j];

			if(pFormat->aGS.size() <= j || !pFormat->aGS[j])
			{
				int iFound = 0;
				Array<XVertexOutputElement*> aPassParameters;

				String sStruct;
				for(UINT k = 0, kl = pFormat->aDecl.size(); k < kl; ++k)
				{
					XVertexOutputElement *pEl = &pFormat->aDecl[k];

					if(k != 0)
					{
						sStruct += "; ";
					}
					sStruct += String(getHLSLType(pEl->type)) + " " + pEl->szName + ": " + getHLSLSemantic(pEl->usage);

					if(pGS->aszRequiredParameters.indexOf(pEl->szName, [](const char *a, const char *b){
						return(!fstrcmp(a, b));
					}) >= 0)
					{
						++iFound;
					}
					else 
					{
						aPassParameters.push_back(pEl);
					}
				}

				GeometryShaderData *gsData = m_aGeometryShadersData.Alloc();
				pFormat->aGS[j] = gsData;

				if(iFound != (int)pGS->aszRequiredParameters.size())
				{
					gsData->isSkipped = true;
					LibReport(REPORT_MSG_LEVEL_WARNING, "Cannot use shader '%s' with '%s' format. Skipping.\n", pGS->szShaderFile, i.first->c_str());
					continue;
				}

				gsData->isSkipped = false;
				gsData->aDefines = pGS->aDefines;
				gsData->iCommonDefines = gsData->aDefines.size();

				String sPass;
				for(UINT k = 0, kl = aPassParameters.size(); k < kl; ++k)
				{
					XVertexOutputElement *el = aPassParameters[k];
					if(k != 0)
					{
						sPass += "; ";
					}
					sPass += String("(dst).") + el->szName + " = (src)." + el->szName;
				}
				gsData->aDefines.push_back({"XMAT_GS_STRUCT()", strdup(sStruct.c_str())});
				gsData->aDefines.push_back({"XMAT_GS_PASS(dst, src)", strdup(sPass.c_str())});
				gsData->aDefines.push_back({NULL, NULL});

				gsData->idShader = SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, pGS->szShaderFile, NULL, &gsData->aDefines[0]);
			}
		}
	}

	for(AssotiativeArray<String, MaterialShader>::Iterator i = m_mMaterialShaders.begin(); i; i++)
	{
		MaterialShader *pShader = i.second;
		
		String sVSOstruct;
		for(UINT k = 0, kl = pShader->pVertexFormat->aDecl.size(); k < kl; ++k)
		{
			XVertexOutputElement *el = &pShader->pVertexFormat->aDecl[k];
			if(k != 0)
			{
				sVSOstruct += "; ";
			}
			sVSOstruct += String(getHLSLType(el->type)) + " " + el->szName + ": " + getHLSLSemantic(el->usage);
		}

		for(UINT uPass = 0, uPassl = pShader->aPasses.size(); uPass < uPassl; ++uPass)
		{
			MaterialShaderPassData *pPass = &pShader->aPasses[uPass];

			if(pPass->isDirty)
			{
			//	pPass->isDirty = false;

				RenderPass *pMetaPass = pPass->pRenderPass;
				Array<GXMacro> aGenericDefines = pPass->aDefines;
				aGenericDefines.push_back({"XMAT_PS_STRUCT()", sVSOstruct.c_str()});

				Array<MaterialShaderSamplerData> aSamplers({NULL, NULL}, GX_MAX_SAMPLERS);
				for(UINT i = 0, l = pMetaPass->aSamplers.size(); i < l; ++i)
				{
					aSamplers[pMetaPass->aSamplers[i].uSlot].szKey = pMetaPass->aSamplers[i].szKey;
				}

				UINT j = 0;
				for(UINT i = 0, l = pPass->aSamplers.size(); i < l; ++i)
				{
					for(; j < GX_MAX_SAMPLERS; ++j)
					{
						if(!aSamplers[j].szKey)
						{
							aSamplers[j] = pPass->aSamplers[i];
							break;
						}
					}
				}
				String sSamplers;
				bool isFirst = true;
				for(UINT i = 0, l = aSamplers.size(); i < l; ++i)
				{
					if(aSamplers[i].szKey)
					{
						if(!isFirst)
						{
							sSamplers += "; ";
						}
						isFirst = false;
						sSamplers += String("SamplerState g_") + aSamplers[i].szKey + ": register(s" + (int)i + ")";
					}
				}
				aGenericDefines.push_back({"XMAT_MS_SAMPLERS()", sSamplers.c_str()});
				pPass->aTotalSamplers.swap(aSamplers);

				Array<MaterialDefine> &aAllDefines = pPass->aAllDefines;
				aAllDefines.clearFast();
				pPass->aAllProperties.clearFast();
				GetAllDefines(aAllDefines, pShader->aProperties, pPass->aAllProperties);
				GetAllDefines(aAllDefines, pPass->aProperties, pPass->aAllProperties);


				UINT uVariants = 1 << aAllDefines.size();

				Array<MaterialDefine*> aStaticList;
				Array<MaterialDefine*> aVariableList;
				for(UINT s = 0; s < uVariants; ++s)
				{
					aStaticList.clear();
					aVariableList.clear();

					for(UINT j = 0, jl = aAllDefines.size(); j < jl; ++j)
					{
						if(s & (1 << j))
						{
							if(aAllDefines[j].pCondition)
							{
								aVariableList.push_back(&aAllDefines[j]);
							}
							else
							{
								aStaticList.push_back(&aAllDefines[j]);
							}
						}
					}

					bool bDoAgain = true;
					while(bDoAgain)
					{
						bDoAgain = false;
						for(UINT key = 0, keyl = aVariableList.size(); key < keyl; ++key)
						{
							MaterialDefine *pDefine = aVariableList[key];

							if(EvalCondition(pDefine->pCondition, aStaticList))
							{
								aStaticList.push_back(pDefine);
								aVariableList.erase(key);
								bDoAgain = true;
								break;
							}
						}
					}

					if(aVariableList.size())
					{
						// bad variant, skipping
						pPass->aVariants[s].isReady = false;
					}
					else
					{
						Array<GXMacro> aVariantDefines = aGenericDefines;
						for(UINT j = 0, jl = aStaticList.size(); j < jl; ++j)
						{
							aVariantDefines.push_back({aStaticList[j]->szName, "1"});
						}

						Array<MaterialTexture> aTextures;
						
						Array<MaterialShaderConstants> &aConstants = pPass->aVariants[s].aConstants;
						aConstants.clearFast();

						ParseTexturesConstants(pShader->aProperties, aStaticList, aTextures, aConstants);
						ParseTexturesConstants(pPass->aProperties, aStaticList, aTextures, aConstants);

						aConstants.quickSort([this](const MaterialShaderConstants &a, const MaterialShaderConstants &b){
							return(this->getTypeSize(a.type) > this->getTypeSize(b.type));
						});


						// Constants ---
						for(UINT j = 0, jl = aConstants.size(); j < jl; ++j)
						{
							aConstants[j].uGroup = UINT_MAX;
						}
						UINT uGroup = 0;
						UINT uGroupSize = 0;
						UINT uConstants = aConstants.size();
						while(uConstants != 0)
						{
							for(UINT j = 0, jl = aConstants.size(); j < jl; ++j)
							{
								auto *pConst = &aConstants[j];
								if(pConst->uGroup != UINT_MAX)
								{
									continue;
								}


								UINT uSize = getTypeSize(pConst->type);
								if(uGroupSize + uSize > 4)
								{
									++uGroup;
									uGroupSize = 0;
								}

								pConst->uGroup = uGroup;
								uGroupSize += uSize;
								--uConstants;
								break;
							}
						}
						
						for(UINT j = 1, jl = aConstants.size(); j < jl; ++j)
						{
							if(aConstants[j - 1].uGroup > aConstants[j].uGroup)
							{
								CMaterialSystem::MaterialShaderConstants tmp = aConstants[j - 1];
								aConstants[j] = aConstants[j - 1];
								aConstants[j - 1] = tmp;
							}
						}
						
						String sConstants;
						UINT uOffset = 0;
						for(UINT j = 0, jl = aConstants.size(); j < jl; ++j)
						{
							if(j != 0)
							{
								sConstants += "; ";
							}
							isFirst = false;
							sConstants += String(getHLSLType(aConstants[j].type)) + " " + aConstants[j].szKey;
							aConstants[j].uOffset = uOffset;
							uOffset += getTypeSize(aConstants[j].type) * 4;
						}
						pPass->aVariants[s].uConstantsSize = uOffset;
						aVariantDefines.push_back({"XMAT_MS_CONST_STRUCT()", strdupa(sConstants.c_str())});
						if(aConstants.size())
						{
							aVariantDefines.push_back({"XMATERIAL_HAS_CONSTANTS", "1"});
						}
						// ---

						// Textures ---
						Array<MaterialTexture> aTextureMap({0}, GX_MAX_TEXTURES);
						for(UINT j = 0, jl = pMetaPass->aTextures.size(); j < jl; ++j)
						{
							aTextureMap[pMetaPass->aTextures[j].uSlot] = {pMetaPass->aTextures[j].szKey, GXTEXTURE_TYPE_2D};
						}

						pPass->aVariants[s].aTextureMap = Array<MaterialTexture>({0}, GX_MAX_TEXTURES);
						UINT j = 0;
						for(UINT k = 0, kl = aTextures.size(); k < kl; ++k)
						{
							for(; j < GX_MAX_TEXTURES; ++j)
							{
								if(!aTextureMap[j].szName)
								{
									pPass->aVariants[s].aTextureMap[j] = aTextures[k];
									aTextureMap[j] = aTextures[k];
									break;
								}
							}
						}
						String sTextures;
						for(UINT j = 0; j < aTextureMap.size(); ++j)
						{
							if(aTextureMap[j].szName)
							{
								sTextures += (aTextureMap[j].type == GXTEXTURE_TYPE_2D ? String("Texture2D") : String("TextureCube")) + " g_" + aTextureMap[j].szName + ":register(t" + (int)j + ");";
							}
						}
						aVariantDefines.push_back({"XMAT_MS_TEXTURES()", strdupa(sTextures.c_str())});
						// ---
												
						char *szTemp = (char*)alloca(strlen(pPass->szShaderFile) + 3);
						sprintf(szTemp, "<%s>", pPass->szShaderFile);
						aVariantDefines.push_back({"XMATERIAL_SHADER", szTemp});
						aVariantDefines.push_back({"XMATERIAL_MAIN", pPass->szEntryPoint});

						char *szTempName = (char*)alloca(sizeof(char) * (strlen(pPass->szEntryPoint) + 9));
						sprintf(szTempName, "COMPILE_%s", pPass->szEntryPoint);
						aVariantDefines.push_back({szTempName, "1"});

						MaterialVariant *pVariant = &pPass->aVariants[s];
						pVariant->isReady = true;

						UINT uOldSize = aVariantDefines.size();

						pVariant->aPassVariants.clearFast();
						for(UINT uPassVariant = 0, uPassVariantl = pPass->pRenderPass->aVariants.size(); uPassVariant < uPassVariantl; ++uPassVariant)
						{
							aVariantDefines.resizeFast(uOldSize);

							auto &aPassVariants = pPass->pRenderPass->aVariants[uPassVariant];
							for(UINT m = 0, ml = aPassVariants.size(); m < ml; ++m)
							{
								aVariantDefines.push_back(aPassVariants[m]);
							}

							aVariantDefines.push_back({NULL, NULL});

							ID idShader = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, pMetaPass->szShaderFile, NULL, &aVariantDefines[0]);
							pVariant->aPassVariants[uPassVariant].idShader = idShader;


							pVariant->aPassVariants[uPassVariant].aVertexShaders.clearFast();
							for(UINT j = 0, jl = pShader->pVertexFormat->aVS.size(); j < jl; ++j)
							{
								VertexShaderData *pVS = pShader->pVertexFormat->aVS[j];
								MaterialVariantVS &vs = pVariant->aPassVariants[uPassVariant].aVertexShaders[j];
								vs.idSet = SGCore_ShaderCreateKit(pVS->idShader, idShader);

								for(UINT k = 0, kl = pShader->pVertexFormat->aGS.size(); k < kl; ++k)
								{
									GeometryShaderData *pGS = pShader->pVertexFormat->aGS[k];
									vs.aGeometryShaders[k] = SGCore_ShaderCreateKit(pVS->idShader, idShader, pGS->idShader);
								}
							}
						}
					}
				}
			}
		}
	}

#if 1
	if(m_isMateriallesRenderPassDirty)
	{
		for(AssotiativeArray<String, RenderPass>::Iterator i = m_mRenderPasses.begin(); i; i++)
		{
			RenderPass *pRP = i.second;
			if(!pRP->bSkipMaterialShader)
			{
				continue;
			}


			Array<GXMacro> aVariantDefines = pRP->aDefines;

			UINT uOldSize = aVariantDefines.size();

			for(AssotiativeArray<String, VertexFormatData>::Iterator i = m_mVertexFormats.begin(); i; i++)
			{
				VertexFormatData *pFormat = i.second;
				pRP->aPassFormats[pFormat->uID].aPassVariants.clearFast();
				for(UINT uPassVariant = 0, uPassVariantl = pRP->aVariants.size(); uPassVariant < uPassVariantl; ++uPassVariant)
				{
					aVariantDefines.resizeFast(uOldSize);

					auto &aPassVariants = pRP->aVariants[uPassVariant];
					for(UINT m = 0, ml = aPassVariants.size(); m < ml; ++m)
					{
						aVariantDefines.push_back(aPassVariants[m]);
					}

					aVariantDefines.push_back({NULL, NULL});

					ID idShader = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, pRP->szShaderFile, NULL, &aVariantDefines[0]);
					pRP->aPassFormats[pFormat->uID].aPassVariants[uPassVariant].idShader = idShader;


					pRP->aPassFormats[pFormat->uID].aPassVariants[uPassVariant].aVertexShaders.clearFast();


					for(UINT j = 0, jl = pFormat->aVS.size(); j < jl; ++j)
					{
						VertexShaderData *pVS = pFormat->aVS[j];
						MaterialVariantVS &vs = pRP->aPassFormats[pFormat->uID].aPassVariants[uPassVariant].aVertexShaders[j];
						vs.idSet = SGCore_ShaderCreateKit(pVS->idShader, idShader);

						for(UINT k = 0, kl = pFormat->aGS.size(); k < kl; ++k)
						{
							GeometryShaderData *pGS = pFormat->aGS[k];
							vs.aGeometryShaders[k] = SGCore_ShaderCreateKit(pVS->idShader, idShader, pGS->idShader);
						}
					}
				}

			}
		}
	}
#endif
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

		for(UINT j = 0, jl = i.second->aGS.size(); j < jl; ++j)
		{
			for(UINT k = i.second->aGS[j]->iCommonDefines, kl = i.second->aGS[j]->aDefines.size(); k < kl; ++k)
			{
				free((void*)i.second->aGS[j]->aDefines[k].szDefinition);
				//free((void*)i.second->aGS[j]->aDefines[k].szName);
			}

			m_aGeometryShadersData.Delete(i.second->aGS[j]);
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

	for(AssotiativeArray<String, RenderPass>::Iterator i = m_mRenderPasses.begin(); i; i++)
	{
		RenderPass *pPass = i.second;
		free((void*)pPass->szName);
		free((void*)pPass->szShaderFile);

		for(UINT j = 0, jl = pPass->aTextures.size(); j < jl; ++j)
		{
			free((void*)pPass->aTextures[j].szName);
			free((void*)pPass->aTextures[j].szKey);
		}

		for(UINT j = 0, jl = pPass->aDefines.size(); j < jl; ++j)
		{
			// free((void*)pPass->aDefines[j].szName);
			free((void*)pPass->aDefines[j].szDefinition);
		}

		for(UINT j = 0, jl = pPass->aSamplers.size(); j < jl; ++j)
		{
			free((void*)pPass->aSamplers[j].szName);
			free((void*)pPass->aSamplers[j].szKey);
		}

		for(UINT j = 0, jl = pPass->aOutput.size(); j < jl; ++j)
		{
			free((void*)pPass->aOutput[j].szName);
			free((void*)pPass->aOutput[j].szKey);
			free((void*)pPass->aOutput[j].szDefault);
		}

		for(UINT j = 0, jl = pPass->aVariants.size(); j < jl; ++j)
		{
			for(UINT k = 0, kl = pPass->aVariants[j].size(); k < kl; ++k)
			{
				free((void*)pPass->aVariants[j][k].szName);
				free((void*)pPass->aVariants[j][k].szDefinition);
			}
		}
	}

	for(AssotiativeArray<String, MaterialShader>::Iterator i = m_mMaterialShaders.begin(); i; i++)
	{
		MaterialShader *pShader = i.second;
		free((void*)pShader->szName);

		for(UINT j = 0, jl = pShader->aProperties.size(); j < jl; ++j)
		{
			free((void*)pShader->aProperties[j].prop.szCondition);
			free((void*)pShader->aProperties[j].prop.szDefine);
			free((void*)pShader->aProperties[j].prop.szKey);
			free((void*)pShader->aProperties[j].prop.szTitle);
			mem_delete(pShader->aProperties[j].pCondition);
		}

		for(UINT j = 0, jl = pShader->aPasses.size(); j < jl; ++j)
		{
			MaterialShaderPassData *pPass = &pShader->aPasses[j];

			free((void*)pPass->szEntryPoint);
			free((void*)pPass->szShaderFile);

			for(UINT k = 0, kl = pPass->aDefines.size(); k < kl; ++k)
			{
				free((void*)pPass->aDefines[k].szName);
				free((void*)pPass->aDefines[k].szDefinition);
			}

			for(UINT k = 0, kl = pPass->aProperties.size(); k < kl; ++k)
			{
				free((void*)pPass->aProperties[k].prop.szCondition);
				free((void*)pPass->aProperties[k].prop.szDefine);
				free((void*)pPass->aProperties[k].prop.szKey);
				free((void*)pPass->aProperties[k].prop.szTitle);
				mem_delete(pPass->aProperties[k].pCondition);
			}

			for(UINT k = 0, kl = pPass->aSamplers.size(); k < kl; ++k)
			{
				free((void*)pPass->aSamplers[k].szKey);
				mem_delete(pPass->aSamplers[k].pSampler);
			}
		}
	}
}

void CMaterialSystem::notifyChanged(XEventMaterialChanged *pEvent)
{
	assert(pEvent);

	m_pNotifyChannel->broadcastEvent(pEvent);
}

void CMaterialSystem::onMaterialShaderChange(CMaterial *pMaterial)
{
	assert(pMaterial);

	MaterialShader *pShader = (MaterialShader*)pMaterial->getShaderHandler();
	if(pShader)
	{
		pMaterial->clearStaticFlags();
		for(UINT i = 0, l = pShader->aStaticFlags.size(); i < l; ++i)
		{
			pMaterial->setStaticFlag(pShader->aStaticFlags[i].szName, pShader->aStaticFlags[i].bValue);
		}
	}
}

void XMETHODCALLTYPE CMaterialSystem::setFlag(const char *szFlag, bool isSet)
{
	IMaterialFlag *pFlag = getFlagHandler(szFlag);
	if(!pFlag)
	{
		pFlag = createFlag(szFlag);
	}

	pFlag->set(isSet);
}
bool XMETHODCALLTYPE CMaterialSystem::getFlag(const char *szFlag)
{
	IMaterialFlag *pFlag = getFlagHandler(szFlag);
	if(pFlag)
	{
		return(pFlag->get());
	}
	return(false);
}
IMaterialFlag* XMETHODCALLTYPE CMaterialSystem::getFlagHandler(const char *szFlag)
{
	const AssotiativeArray<AAString, CGlobalFlag>::Node *pNode;
	if(m_mapFlags.KeyExists(szFlag, &pNode))
	{
		return(pNode->Val);
	}
	return(NULL);
}

CGlobalFlag* CMaterialSystem::createFlag(const char *szName)
{
	AAString sKey;
	sKey.setName(szName);
	CGlobalFlag flag(NULL, sKey.getName());
	m_mapFlags[sKey] = flag;
	return(&m_mapFlags[sKey]);
}

void CMaterialSystem::onGlobalFlagChanged(CGlobalFlag *pFlag)
{
	Array<XMaterialShaderHandler*> aShaders;
	const char *szFlag = pFlag->getName();

	for(AssotiativeArray<String, MaterialShader>::Iterator i = m_mMaterialShaders.begin(); i; i++)
	{
		bool isFound = false;
		for(UINT j = 0, jl = i.second->aProperties.size(); j < jl; ++j)
		{
			XMaterialProperty *pProp = &i.second->aProperties[j].prop;

			if(pProp->type == XMPT_GLOBAL_FLAG && !fstrcmp(szFlag, pProp->szKey))
			{
				isFound = true;
				break;
			}
		}

		for(UINT k = 0, kl = i.second->aPasses.size(); k < kl && !isFound; ++k)
		{
			for(UINT j = 0, jl = i.second->aPasses[k].aProperties.size(); j < jl; ++j)
			{
				XMaterialProperty *pProp = &i.second->aPasses[k].aProperties[j].prop;

				if(pProp->type == XMPT_GLOBAL_FLAG && !fstrcmp(szFlag, pProp->szKey))
				{
					isFound = true;
					break;
				}
			}
		}

		if(isFound)
		{
			aShaders.push_back(i.second);
		}
	}

	if(aShaders.size())
	{
		for(AssotiativeArray<String, CMaterial*>::Iterator i = m_mapMaterials.begin(); i; i)
		{
			if(aShaders.indexOf((*i.second)->getShaderHandler()) >= 0)
			{
				(*i.second)->onGlobalFlagChanged(pFlag);
			}
		}
	}
}

bool CMaterialSystem::saveMaterial(CMaterial *pMaterial)
{
	assert(pMaterial);

	IXMaterialProxy *pProxy = pMaterial->getProxy();
	const char *szName = pMaterial->getName();

	if(pProxy)
	{
		UINT uFileNameLength = 0;
		if(pProxy->resolveName(szName, NULL, &uFileNameLength, true))
		{
			char *szFileName = (char*)alloca(sizeof(char) * uFileNameLength);
		
			pProxy->resolveName(szName, szFileName, &uFileNameLength, true);

			const char *szExt = GetFileExtension(szFileName);

			if(szExt && *szExt)
			{
				char *szLowcaseExt = strdupa(szExt);
				strlwr(szLowcaseExt);

				const AssotiativeArray<AAString, Array<MaterialLoader>>::Node *pNode;
				if(m_mapMaterialLoaders.KeyExists(AAString(szLowcaseExt), &pNode))
				{
					auto &aLoaders = *pNode->Val;
					bool isSuccess = false;
					for(UINT i = 0, l = aLoaders.size(); i < l; ++i)
					{
						if(aLoaders[i].canSave)
						{
							IXMaterialLoader *pLoader = aLoaders[i].pLoader;
							//! @fixme open::arg is not persists!
							if(pLoader->open(szFileName, ""))
							{
								isSuccess = pLoader->save(pMaterial);

								pLoader->close();

								if(isSuccess)
								{
									break;
								}
							}
						}
					}

					if(isSuccess)
					{
						return(true);
					}
				}
			}
		}
	}

	// save in other way
	UINT uFileNameLength = 0;
	for(UINT i = 0, l = m_aMaterialProxies.size(); i < l; ++i)
	{
		IXMaterialProxy *pProxy = m_aMaterialProxies[i];

		if(pProxy->resolveName(szName, NULL, &uFileNameLength))
		{
			char *szFileName = (char*)alloca(sizeof(char) * uFileNameLength);

			pProxy->resolveName(szName, szFileName, &uFileNameLength, true);

			const char *szExt = GetFileExtension(szFileName);

			if(szExt && *szExt)
			{
				char *szLowcaseExt = strdupa(szExt);
				strlwr(szLowcaseExt);

				const AssotiativeArray<AAString, Array<MaterialLoader>>::Node *pNode;
				if(m_mapMaterialLoaders.KeyExists(AAString(szLowcaseExt), &pNode))
				{
					auto &aLoaders = *pNode->Val;
					bool isSuccess = false;
					for(UINT i = 0, l = aLoaders.size(); i < l; ++i)
					{
						if(aLoaders[i].canSave)
						{
							IXMaterialLoader *pLoader = aLoaders[i].pLoader;
							//! @fixme open::arg is not persists!
							if(pLoader->open(szFileName, ""))
							{
								isSuccess = pLoader->save(pMaterial);

								pLoader->close();

								if(isSuccess)
								{
									break;
								}
							}
						}
					}

					if(isSuccess)
					{
						return(true);
					}
				}
			}
		}
	}

	return(false);
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

CMaterialProperty::CMaterialProperty(CMaterial *pMaterial, const char *szName, XEventMaterialChanged::TYPE typeNotifier):
	m_pMaterial(pMaterial),
	m_szName(szName),
	m_typeNotifier(typeNotifier)
{}

const char* XMETHODCALLTYPE CMaterialProperty::getName() const
{
	return(m_szName);
}

void XMETHODCALLTYPE CMaterialProperty::set(const float4_t &vValue)
{
	m_isSet = true;

	if(!memcmp(&m_vValue, &vValue, sizeof(m_vValue)))
	{
		return;
	}
	m_vValue = vValue;

	m_pMaterial->notifyChanged(m_typeNotifier, getName());

	setValue();
}
float4_t XMETHODCALLTYPE CMaterialProperty::get() const
{
	return(m_vValue);
}
void CMaterialProperty::setOffset(UINT uOffset, UINT uSize)
{
	m_pCurrentPass->uOffset = uOffset;
	m_pCurrentPass->uSize = uSize;
	
	setValue();
}

void CMaterialProperty::setDefault(const float4_t &vValue)
{
	if(!m_isSet)
	{
		m_vValue = vValue;

		setValue();
	}
}

void CMaterialProperty::preparePass(UINT uPass)
{
	m_pCurrentPass = &m_aPassCache[uPass];
}

void CMaterialProperty::setValue()
{
	for(UINT i = 0, l = m_aPassCache.size(); i < l; ++i)
	{
		if(m_aPassCache[i].uOffset != UINT_MAX)
		{
			m_pMaterial->setConstant(i, m_aPassCache[i].uOffset, &m_vValue, m_aPassCache[i].uSize);
		}
	}
}

//#############################################################################

CMaterialFlag::CMaterialFlag(CMaterial *pMaterial, const char *szName, XEventMaterialChanged::TYPE typeNotifier):
	m_pMaterial(pMaterial),
	m_szName(szName),
	m_typeNotifier(typeNotifier)
{}

const char* XMETHODCALLTYPE CMaterialFlag::getName() const
{
	return(m_szName);
}

void XMETHODCALLTYPE CMaterialFlag::set(bool bValue)
{
	if(m_bValue == bValue || m_isStatic)
	{
		return;
	}
	m_bValue = bValue;

	if(m_pMaterial)
	{
		m_pMaterial->notifyChanged(m_typeNotifier, getName());
	}
}
bool XMETHODCALLTYPE CMaterialFlag::get() const
{
	return(m_bValue);
}

void CMaterialFlag::clearStatic()
{
	m_isStatic = false;
}
void CMaterialFlag::setStatic(bool bValue)
{
	set(bValue);
	m_isStatic = true;
}

//#############################################################################

CGlobalFlag::CGlobalFlag(CMaterialSystem *pMaterialSystem, const char *szName):
m_pMaterialSystem(pMaterialSystem),
CMaterialFlag(NULL, szName)
{}

void XMETHODCALLTYPE CGlobalFlag::set(bool bValue)
{
	if(get() != bValue)
	{
		BaseClass::set(bValue);

		m_pMaterialSystem->onGlobalFlagChanged(this);
	}
}

//#############################################################################

template<typename T>
class CKeyIterator: public IKeyIterator
{
public:
	//template<typename T>
	CKeyIterator(AssotiativeArray<AAString, T> &map):
		m_iter(map.begin())
	{
	}

	const char* XMETHODCALLTYPE getCurrent() override
	{
		if(m_iter)
		{
			return(m_iter.first->getName());
		}
		return(NULL);
	}
	const char* XMETHODCALLTYPE getNext() override
	{
		m_iter++;
		if(m_iter)
		{
			return(m_iter.first->getName());
		}
		return(NULL);
	}
	bool XMETHODCALLTYPE isEnd() override
	{
		return(m_iter);
	}

private:
	typename AssotiativeArray<AAString, T>::Iterator m_iter;
};

//#############################################################################

CMaterial::CMaterial(CMaterialSystem *pMaterialSystem, const char *szName):
	m_pMaterialSystem(pMaterialSystem),
	m_szName(szName)
{
	m_pDurability = createProperty("durability", XEventMaterialChanged::TYPE_DURABILITY);
	m_pDurability->set(float4(1000.0f));

	m_pHitChance = createProperty("hit_chance", XEventMaterialChanged::TYPE_HIT_CHANCE);
	m_pHitChance->set(float4(1.0f));

	m_pDensity = createProperty("density", XEventMaterialChanged::TYPE_DENSITY);
	m_pDensity->set(float4(1000.0f));

	m_pTransparent = createFlag("transparent", XEventMaterialChanged::TYPE_TRANSPARENCY);
	m_pRefractive = createFlag("refractive", XEventMaterialChanged::TYPE_REFRACTIVITY);
	m_pBlurred = createFlag("blurred", XEventMaterialChanged::TYPE_BLURRED);

	setShader("Default");
}
CMaterial::~CMaterial()
{
	m_pMaterialSystem->onMaterialRelease(this);

	for(UINT i = 0, l = m_aPassCache.size(); i < l; ++i)
	{
		mem_release(m_aPassCache[i].pConstants);
		mem_delete_a(m_aPassCache[i].pConstantsBlob);
	}

	for(AssotiativeArray<AAString, MaterialTexture>::Iterator i = m_mapTextures.begin(); i; i++)
	{
		mem_release(i.second->pTexture);
	}
}

const char* XMETHODCALLTYPE CMaterial::getName() const
{
	return(m_szName);
}

//void XMETHODCALLTYPE CMaterial::getMainTexture(IXTexture **ppTexture)
//{
//	*ppTexture = NULL;
	/*ID id = SMtrl_MtlGetTextureID(m_id);
	*ppTexture = new CTexture(id);*/
//}


void XMETHODCALLTYPE CMaterial::setTransparent(bool bValue)
{
	m_pTransparent->set(bValue);
}
bool XMETHODCALLTYPE CMaterial::isTransparent() const
{
	return(m_pTransparent->get());
}

void XMETHODCALLTYPE CMaterial::setRefractive(bool bValue)
{
	m_pRefractive->set(bValue);

	notifyChanged(XEventMaterialChanged::TYPE_REFRACTIVITY);
}
bool XMETHODCALLTYPE CMaterial::isRefractive() const
{
	return(m_pRefractive->get());
}

void XMETHODCALLTYPE CMaterial::setBlurred(bool bValue)
{
	m_pBlurred->set(bValue);

	notifyChanged(XEventMaterialChanged::TYPE_BLURRED);
}
bool XMETHODCALLTYPE CMaterial::isBlurred() const
{
	return(m_pBlurred->get());
}

void XMETHODCALLTYPE CMaterial::setShader(const char *szShader)
{
	m_sShader = szShader;
	m_pShader = m_pMaterialSystem->getMaterialShader(szShader);
	if(!m_pShader)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to find shader '%s' for material '%s'\n", szShader, getName());
		setShader("Default");
	}
	else
	{
		notifyChanged(XEventMaterialChanged::TYPE_SHADER);

		m_pMaterialSystem->onMaterialShaderChange(this);
	}
}
const char* XMETHODCALLTYPE CMaterial::getShader() const
{
	return(m_sShader.c_str());
}
XMaterialShaderHandler* XMETHODCALLTYPE CMaterial::getShaderHandler() const
{
	return(m_pShader);
}

void XMETHODCALLTYPE CMaterial::setFlag(const char *szFlag, bool isSet)
{
	IMaterialFlag *pFlag = getFlagHandler(szFlag);
	if(!pFlag)
	{
		pFlag = createFlag(szFlag);
	}

	pFlag->set(isSet);
}
bool XMETHODCALLTYPE CMaterial::getFlag(const char *szFlag)
{
	IMaterialFlag *pFlag = getFlagHandler(szFlag);
	if(pFlag)
	{
		return(pFlag->get());
	}
	return(false);
}
IMaterialFlag* XMETHODCALLTYPE CMaterial::getFlagHandler(const char *szFlag)
{
	const AssotiativeArray<AAString, CMaterialFlag>::Node *pNode;
	if(m_mapFlags.KeyExists(szFlag, &pNode))
	{
		return(pNode->Val);
	}
	return(NULL);
}
IKeyIterator* XMETHODCALLTYPE CMaterial::getFlagsIterator()
{
	return(new CKeyIterator<CMaterialFlag>(m_mapFlags));
}

void XMETHODCALLTYPE CMaterial::setParam(const char *szFlag, const float4_t &vValue)
{
	IMaterialProperty *pProp = getParamHandler(szFlag);
	if(!pProp)
	{
		pProp = createProperty(szFlag);
	}

	pProp->set(vValue);
}
float4_t XMETHODCALLTYPE CMaterial::getParam(const char *szFlag)
{
	IMaterialProperty *pProp = getParamHandler(szFlag);
	if(pProp)
	{
		return(pProp->get());
	}
	return(float4_t());
}
IMaterialProperty* XMETHODCALLTYPE CMaterial::getParamHandler(const char *szParam)
{
	const AssotiativeArray<AAString, CMaterialProperty>::Node *pNode;
	if(m_mapProperties.KeyExists(szParam, &pNode))
	{
		return(pNode->Val);
	}
	return(NULL);
}
IKeyIterator* XMETHODCALLTYPE CMaterial::getParamsIterator()
{
	return(new CKeyIterator<CMaterialProperty>(m_mapProperties));
}

void XMETHODCALLTYPE CMaterial::setTexture(const char *szKey, const char *szTexture)
{
	assert(strlen(szKey) < AAS_MAXLEN);

	const AssotiativeArray<AAString, MaterialTexture>::Node *pNode;
	if(m_mapTextures.KeyExists(szKey, &pNode))
	{
		if(!fstrcmp(pNode->Val->sName.c_str(), szTexture))
		{
			return;
		}
		pNode->Val->sName = szTexture;
		mem_release(pNode->Val->pTexture);
		m_pMaterialSystem->loadTexture(szTexture, &pNode->Val->pTexture);
	}
	else
	{
		AAString sKey;
		sKey.setName(szKey);
		m_mapTextures[sKey].sName = szTexture;
		m_pMaterialSystem->loadTexture(szTexture, &m_mapTextures[sKey].pTexture);
	}
}
const char* XMETHODCALLTYPE CMaterial::getTextureName(const char *szKey) const
{
	const AssotiativeArray<AAString, MaterialTexture>::Node *pNode;
	if(m_mapTextures.KeyExists(szKey, &pNode))
	{
		return(pNode->Val->sName.c_str());
	}
	return("");
}
IXTexture* XMETHODCALLTYPE CMaterial::getTexture(const char *szKey) const
{
	const AssotiativeArray<AAString, MaterialTexture>::Node *pNode;
	if(m_mapTextures.KeyExists(szKey, &pNode))
	{
		return(pNode->Val->pTexture);
	}
	return(NULL);
}
IKeyIterator* XMETHODCALLTYPE CMaterial::getTexturesIterator()
{
	return(new CKeyIterator<MaterialTexture>(m_mapTextures));
}


void XMETHODCALLTYPE CMaterial::setPhysicsType(MTLTYPE_PHYSIC type)
{
	m_typePhysics = type;

	notifyChanged(XEventMaterialChanged::TYPE_PHYSICS_TYPE);
}
MTLTYPE_PHYSIC XMETHODCALLTYPE CMaterial::getPhysicsType() const
{
	return(m_typePhysics);
}

void XMETHODCALLTYPE CMaterial::setDurablility(float fValue)
{
	assert(fValue >= 0);

	m_pDurability->set(float4_t(fValue, 0.0f, 0.0f, 0.0f));
}
float XMETHODCALLTYPE CMaterial::getDurablility() const
{
	return(m_pDurability->get().x);
}

void XMETHODCALLTYPE CMaterial::setHitChance(float fValue)
{
	assert(fValue >= 0 && fValue <= 1.0f);

	m_pHitChance->set(float4_t(fValue, 0.0f, 0.0f, 0.0f));
}
float XMETHODCALLTYPE CMaterial::getHitChance() const
{
	return(m_pHitChance->get().x);
}

void XMETHODCALLTYPE CMaterial::setDensity(float fValue)
{
	assert(fValue >= 0);

	m_pDensity->set(float4_t(fValue, 0.0f, 0.0f, 0.0f));
}
float XMETHODCALLTYPE CMaterial::getDensity() const
{
	return(m_pDensity->get().x);
}

bool XMETHODCALLTYPE CMaterial::save()
{
	return(m_pMaterialSystem->saveMaterial(this));
}

void CMaterial::notifyChanged(XEventMaterialChanged::TYPE type, const char *szReference)
{
	XEventMaterialChanged ev;
	ev.type = type;
	ev.pMaterial = this;
	ev.szReference = szReference;

	m_pMaterialSystem->notifyChanged(&ev);

	switch(ev.type)
	{
	case XEventMaterialChanged::TYPE_TRANSPARENCY:
	case XEventMaterialChanged::TYPE_REFRACTIVITY:
	case XEventMaterialChanged::TYPE_BLURRED:
	case XEventMaterialChanged::TYPE_FLAG:
	case XEventMaterialChanged::TYPE_SHADER:
		updateShader();
		break;

	case XEventMaterialChanged::TYPE_PHYSICS_TYPE:
	case XEventMaterialChanged::TYPE_DURABILITY:
	case XEventMaterialChanged::TYPE_HIT_CHANCE:
	case XEventMaterialChanged::TYPE_DENSITY:
	case XEventMaterialChanged::TYPE_PROPERTY:
		// do nothing;
		break;
	}
}

CMaterialFlag* CMaterial::createFlag(const char *szName, XEventMaterialChanged::TYPE type)
{
	AAString sKey;
	sKey.setName(szName);
	CMaterialFlag flag(this, sKey.getName(), type);
	m_mapFlags[sKey] = flag;
	return(&m_mapFlags[sKey]);
}
CMaterialProperty* CMaterial::createProperty(const char *szName, XEventMaterialChanged::TYPE type)
{
	AAString sKey;
	sKey.setName(szName);
	CMaterialProperty prop(this, sKey.getName(), type);
	m_mapProperties[sKey] = prop;

	CMaterialProperty *pProp = &m_mapProperties[sKey];
	m_aProperties.push_back(pProp);

	if(m_uCurrentPass != UINT_MAX)
	{
		pProp->preparePass(m_uCurrentPass);
	}

	return(pProp);
}

void CMaterial::updateShader()
{
	if(m_pCurrentPass)
	{
		m_pCurrentPass->isDirty = true;
	}
	/*if(m_pShader)
	{

	}*/
}

IXTexture* CMaterial::getTextureForSlot(UINT uSlot)
{
	if(uSlot < m_pCurrentPass->aTextureMap.size())
	{
		return(m_pCurrentPass->aTextureMap[uSlot] ? m_pCurrentPass->aTextureMap[uSlot]->pTexture : NULL);
	}
	return(NULL);
}
void CMaterial::clearTextureBindings()
{
	for(UINT i = 0, l = m_pCurrentPass->aTextureMap.size(); i < l; ++i)
	{
		m_pCurrentPass->aTextureMap[i] = NULL;
	}
}
void CMaterial::bindTextureToSlot(const char *szKey, UINT uSlot)
{
	assert(strlen(szKey) < AAS_MAXLEN);

	const AssotiativeArray<AAString, MaterialTexture>::Node *pNode;
	if(m_mapTextures.KeyExists(szKey, &pNode))
	{
		m_pCurrentPass->aTextureMap[uSlot] = pNode->Val;
	}
	else
	{
		AAString sKey;
		sKey.setName(szKey);
		m_pCurrentPass->aTextureMap[uSlot] = &m_mapTextures[sKey];
		m_pCurrentPass->aTextureMap[uSlot]->pTexture = NULL;
	}
}

IGXConstantBuffer* CMaterial::getConstants()
{
	if(!m_pCurrentPass->pConstants)
	{
		return(NULL);
	}

	if(m_pCurrentPass->isConstantsDirty)
	{
		m_pCurrentPass->isConstantsDirty = false;
		m_pCurrentPass->pConstants->update(m_pCurrentPass->pConstantsBlob);
	}

	return(m_pCurrentPass->pConstants);
}

void CMaterial::initConstantsBindings(UINT uSize)
{
	if(uSize > m_pCurrentPass->uConstantSize)
	{
		m_pCurrentPass->uConstantSize = ((uSize + 15) / 16) * 16;
		mem_release(m_pCurrentPass->pConstants);
		mem_delete_a(m_pCurrentPass->pConstantsBlob);

		m_pCurrentPass->pConstantsBlob = new byte[m_pCurrentPass->uConstantSize];
		memset(m_pCurrentPass->pConstantsBlob, 0, m_pCurrentPass->uConstantSize);
		m_pCurrentPass->pConstants = SGCore_GetDXDevice()->createConstantBuffer(m_pCurrentPass->uConstantSize);
	}
	m_pCurrentPass->isConstantsDirty = true;

	for(UINT i = 0, l = m_aProperties.size(); i < l; ++i)
	{
		m_aProperties[i]->setOffset(UINT_MAX, 4);
	}
}

void CMaterial::bindConstantToOffset(const char *szConstant, UINT uOffset, UINT uSize, const float4_t &vDefault)
{
	CMaterialProperty *pProp = (CMaterialProperty*)getParamHandler(szConstant);
	if(!pProp)
	{
		pProp = createProperty(szConstant);
	}
	pProp->setDefault(vDefault);

	pProp->setOffset(uOffset, uSize);
}

void CMaterial::setConstant(UINT uPass, UINT uOffset, const void *pValue, UINT uSize)
{
	assert(uOffset + uSize <= m_aPassCache[uPass].uConstantSize);
	
	if(m_aPassCache[uPass].pConstantsBlob)
	{
		memcpy(m_aPassCache[uPass].pConstantsBlob + uOffset, pValue, uSize);

		m_aPassCache[uPass].isConstantsDirty = true;
	}
}

void CMaterial::preparePass(UINT uPass)
{
	m_uCurrentPass = uPass;
	m_pCurrentPass = &m_aPassCache[uPass];

	for(UINT i = 0, l = m_aProperties.size(); i < l; ++i)
	{
		m_aProperties[i]->preparePass(uPass);
	}
}

void CMaterial::clearStaticFlags()
{
	for(AssotiativeArray<AAString, CMaterialFlag>::Iterator i = m_mapFlags.begin(); i; i++)
	{
		i.second->clearStatic();
	}
}
void CMaterial::setStaticFlag(const char *szName, bool bValue)
{
	CMaterialFlag *pFlag = (CMaterialFlag*)getFlagHandler(szName);
	if(!pFlag)
	{
		pFlag = createFlag(szName);
	}
	pFlag->setStatic(bValue);
}

void CMaterial::onGlobalFlagChanged(CGlobalFlag *pFlag)
{
	for(UINT i = 0, l = m_aPassCache.size(); i < l; ++i)
	{
		m_aPassCache[i].isDirty = true;
	}
}

void CMaterial::setProxy(IXMaterialProxy *pProxy)
{
	m_pProxy = pProxy;
}
IXMaterialProxy* CMaterial::getProxy()
{
	return(m_pProxy);
}
