
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "light.h"

CLights::CLights()
{
	m_iHowShadow = 0;

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	m_idShadowMap = SGCore_RTcreateTexture(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap", 1);
	m_idShadowMap2 = SGCore_RTcreateTexture(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap2", 1);

	m_idGlobalLight = -1;
	m_isCastGlobalShadow = false;
}

CLights::~CLights()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		mem_delete(m_aLights[i]);
	}

	m_aLights.clear();

	SGCore_RTDelete(m_idShadowMap);
	SGCore_RTDelete(m_idShadowMap2);
}

ID CLights::createCopy(ID id)
{
	LIGHTS_PRE_COND_ID(id,-1);
	
	CLight *pLight = m_aLights[id];
	CLight *pLight2 = new CLight();
	pLight2->m_fAngle = pLight->m_fAngle;
	pLight2->m_pBoundVolume = SGCore_CrBound();

	IDirect3DVertexBuffer9 *pVertex;
	pLight->m_pMesh->GetVertexBuffer(&pVertex);
	pLight->m_pBoundVolume->calcBound(pVertex, pLight->m_pMesh->GetNumVertices(), pLight->m_pMesh->GetNumBytesPerVertex());
	mem_release(pVertex);

	pLight2->m_fPower = pLight->m_fPower;
	pLight2->m_fDist = pLight->m_fDist;
	pLight2->m_vColor = pLight->m_vColor;
	pLight2->m_fDistFor = -1;
	pLight2->m_fGAngleX = pLight->m_fGAngleX;
	pLight2->m_fGAngleY = pLight->m_fGAngleY;
	pLight2->m_isEnable = pLight->m_isEnable;
	pLight2->m_isGlobal = pLight->m_isGlobal;
	pLight2->m_isVisible = pLight->m_isVisible;
	pLight2->m_isVisibleFor = false;
	sprintf(pLight2->m_szName, "%s", pLight->m_szName);
	pLight2->m_vPosition = pLight->m_vPosition;
	pLight2->m_qQuaternion = pLight->m_qQuaternion;
	pLight2->m_vTopBottomRadius = pLight->m_vTopBottomRadius;
	pLight2->m_typeLight = pLight->m_typeLight;
	pLight2->m_mWorld = pLight->m_mWorld;

	pLight2->m_typeShadowed = pLight->m_typeShadowed;
	pLight2->m_iCountUpdate = pLight->m_iCountUpdate;

	if (pLight->m_pShadowPSSM)
	{
		pLight2->m_pShadowPSSM = new CShadowGlobal();
		pLight2->m_pShadowPSSM->init();
		pLight2->m_fGAngleX = pLight->m_fGAngleX;
		pLight2->m_fGAngleY = pLight->m_fGAngleY;
		pLight2->m_pShadowPSSM->setPosition(&float3(pLight->m_vPosition.x, pLight->m_vPosition.y, pLight->m_vPosition.z));
		memcpy((pLight2->m_pShadowPSSM->m_aNearFar), (pLight->m_pShadowPSSM->m_aNearFar), sizeof(float2)* 5);
		memcpy((pLight2->m_pShadowPSSM->m_aIsUpdate), (pLight->m_pShadowPSSM->m_aIsUpdate), sizeof(int)* 5);
	}
	else
		pLight2->m_pShadowPSSM = 0;

	if (pLight->m_pShadowSM)
	{
		pLight2->m_pShadowSM = new CShadowMap();
		pLight2->m_pShadowSM->init();
		pLight2->m_pShadowSM->setPosition(&float3(pLight->m_vPosition.x, pLight->m_vPosition.y, pLight->m_vPosition.z));
		pLight2->m_pShadowSM->setDirection(&(pLight->m_qQuaternion * LIGHTS_DIR_BASE));

		pLight2->m_pShadowSM->setBias(pLight->m_pShadowSM->getBias());
		pLight2->m_pShadowSM->setBlurPixel(pLight->m_pShadowSM->getBlurPixel());
		float3 anf;
		pLight2->m_pShadowSM->getAngleNearFar(&anf);
		pLight2->m_pShadowSM->setAngleNearFar(&anf);
	}
	else
		pLight2->m_pShadowSM = 0;

	if (pLight->m_pShadowCube)
	{
		pLight2->m_pShadowCube = new CShadowMapCube();
		pLight2->m_pShadowCube->init();
		pLight2->m_pShadowCube->setPosition(&float3(pLight->m_vPosition.x, pLight->m_vPosition.y, pLight->m_vPosition.z));
		pLight2->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, pLight->m_fDist));
		pLight2->m_pShadowCube->setNear(pLight2->m_pShadowCube->getNear());
		pLight2->m_pShadowCube->setFar(pLight2->m_pShadowCube->getFar());
		pLight2->m_pShadowCube->setBias(pLight->m_pShadowCube->getBias());
		pLight2->m_pShadowCube->setBlurPixel(pLight->m_pShadowCube->getBlurPixel());

		for (int i = 0; i < 6; ++i)
		{
			pLight2->m_pShadowCube->setEnableCubeEdge(i, pLight2->m_pShadowCube->getEnableCubeEdge(i));
		}
	}
	else
		pLight2->m_pShadowCube = 0;

	pLight->m_pMesh->CloneMeshFVF(pLight->m_pMesh->GetOptions(), pLight->m_pMesh->GetFVF(), light_data::pDXDevice, &(pLight2->m_pMesh));

	return addLight(pLight);
}

CLights::CLight::CLight()
{
	m_typeLight = LTYPE_LIGHT_NONE;
	m_szName[0] = 0;

	m_fShadowIntensity = 1.0;

	m_id = -1;
	
	m_typeShadowed = LTYPE_SHADOW_STATIC;
	m_iCountUpdate = 0;

	m_isVisible = m_isEnable = m_isGlobal = false;

	m_pMesh = 0;
	m_fPower = m_fDist = 0;
	m_fGAngleX = m_fGAngleY = 0;

	m_fAngle = 0;

	m_pBoundVolume = 0;

	m_isVisibleFor = false;
	m_fDistFor = -1;
	m_qQuaternion = SMQuaternion(-SM_PI, 'z');

	m_pShadowPSSM = 0;
	m_pShadowSM = 0;
	m_pShadowCube = 0;
}

CLights::CLight::~CLight()
{
	mem_release_del(m_pMesh);
	mem_release_del(m_pBoundVolume);
	
	mem_delete(m_pShadowPSSM);
	mem_delete(m_pShadowSM);
	mem_delete(m_pShadowCube);
}

void CLights::onLostDevice()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		CLight *pLight = m_aLights[i];

		if (!pLight)
			continue;

		if (pLight->m_pShadowSM)
			pLight->m_pShadowSM->onLostDevice();
		else if (pLight->m_pShadowCube)
			pLight->m_pShadowCube->onLostDevice();
		else if (pLight->m_pShadowPSSM)
			pLight->m_pShadowPSSM->onLostDevice();
	}
}

void CLights::onResetDevice()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		CLight *pLight = m_aLights[i];
		if (!pLight)
			continue;

		if (pLight->m_pShadowSM)
			pLight->m_pShadowSM->onResetDevice();
		else if (pLight->m_pShadowCube)
			pLight->m_pShadowCube->onResetDevice();
		else if (pLight->m_pShadowPSSM)
			pLight->m_pShadowPSSM->onResetDevice();

		lightCountUpdateNull(i);
	}
}

ID CLights::addLight(CLight *pObj)
{
	ID idAdd = -1;

	if (m_aFreeIDs.size() > 0)
	{
		m_aLights[m_aFreeIDs[0]] = pObj;
		pObj->m_id = m_aFreeIDs[0];
		m_aFreeIDs.erase(0);
	}
	else
	{
		m_aLights.push_back(pObj);
		pObj->m_id = m_aLights.size() - 1;
	}

	return pObj->m_id;
}

int CLights::getCountLights() const 
{
	return m_aLights.size();
}

void CLights::clear()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		mem_delete(m_aLights[i]);
	}

	m_aLights.clear();
	m_idGlobalLight = -1;
}

void CLights::resetUpdate4Local()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		if (m_aLights[i])
		{
			lightCountUpdateNull(i);
		}
	}
}

void CLights::clearIDArr()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		CLight *pLight = m_aLights[i];

		if (!pLight)
			continue;

		if (pLight->m_pShadowSM)
		{
			for (int k = 0; k < pLight->m_pShadowSM->getCountVisCalcObj(); ++k)
			{
				pLight->m_pShadowSM->setIdVisCalcObj(k, -1);
			}
		}
		else if (pLight->m_pShadowCube)
		{
			for (int k = 0; k < pLight->m_pShadowCube->getCountVisCalcObj(); ++k)
			{
				pLight->m_pShadowCube->setIdVisCalcObj(k, 0, -1);
				pLight->m_pShadowCube->setIdVisCalcObj(k, 1, -1);
				pLight->m_pShadowCube->setIdVisCalcObj(k, 2, -1);
				pLight->m_pShadowCube->setIdVisCalcObj(k, 3, -1);
				pLight->m_pShadowCube->setIdVisCalcObj(k, 4, -1);
				pLight->m_pShadowCube->setIdVisCalcObj(k, 5, -1);
			}
		}
		else if (pLight->m_pShadowPSSM)
		{
			for (int k = 0; k < pLight->m_pShadowPSSM->getCountVisCalcObj(); ++k)
			{
				pLight->m_pShadowPSSM->setIdVisCalcObj(k, 0, -1);
				pLight->m_pShadowPSSM->setIdVisCalcObj(k, 1, -1);
				pLight->m_pShadowPSSM->setIdVisCalcObj(k, 2, -1);
				pLight->m_pShadowPSSM->setIdVisCalcObj(k, 3, -1);
				pLight->m_pShadowPSSM->setIdVisCalcObj(k, 4, -1);
			}
		}
	}
}

bool CLights::getExists(ID id) const
{
	return (m_aLights.size() > id && m_aLights[id]);
}

void CLights::deleteLight(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	CLight *pLight = m_aLights[id];

	if (pLight->m_isGlobal)
		m_idGlobalLight = -1;

	pLight->m_isEnable = false;
	m_aDelLights.push_back(pLight);
	pLight = 0;
	m_aFreeIDs.push_back(id);
}

char* CLights::getLightName(ID id)
{
	LIGHTS_PRE_COND_ID(id, 0);
	return m_aLights[id]->m_szName;
}

void CLights::setLightName(ID id, const char *szName)
{
	LIGHTS_PRE_COND_ID(id, _VOID);
	sprintf(m_aLights[id]->m_szName, "%s", szName);
}

ID CLights::createPoint(ID id, const float3 *pCenter, float fDist, const float3 *pColor, bool isGlobal, bool isShadow/*, const char* bound_volume*/)
{
	if (m_idGlobalLight != -1 && isGlobal)
	{
		setLightPos(m_idGlobalLight, pCenter);
		setLightColor(m_idGlobalLight, pColor);
		setLightTypeShadowed(m_idGlobalLight, (isShadow ? LTYPE_SHADOW_DYNAMIC : LTYPE_SHADOW_NONE));
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - light: global light exists, you can not create 2 global light sources\n", GEN_MSG_LOCATION);
		return m_idGlobalLight;
	}

	CLight *pLight = 0;

	if (id < 0)
		pLight = new CLight();
	else
	{
		pLight = new CLight();
		pLight->m_id = m_aLights[id]->m_id;
		mem_delete(m_aLights[id]);
		m_aLights[id] = pLight;
	}

	pLight->m_isGlobal = isGlobal;

	if (isGlobal)
		pLight->m_fDist = LIGHTS_GLOBAL_STD_RADIUS;
	else
		pLight->m_fDist = fDist;

	pLight->m_typeLight = (isGlobal ? LTYPE_LIGHT_GLOBAL : LTYPE_LIGHT_POINT);

	/*if(StrValidate(bound_volume))
	{
			
	}
	else
	{*/
	D3DXCreateSphere(light_data::pDXDevice, pLight->m_fDist, 20, 20, &pLight->m_pMesh, 0);
	//}

	if (isGlobal && isShadow)
	{
		pLight->m_pShadowPSSM = new CShadowGlobal();
		pLight->m_pShadowPSSM->init();
		pLight->m_pShadowPSSM->setPosition(pCenter);
	}
	else if (isShadow)
	{
		pLight->m_pShadowCube = new CShadowMapCube();
		pLight->m_pShadowCube->init();
		pLight->m_pShadowCube->setPosition(pCenter);
		pLight->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, pLight->m_fDist));
	}
	else
	{
		int qwerty = 0;
	}

	if (isShadow)
		pLight->m_typeShadowed = LTYPE_SHADOW_DYNAMIC;
	else
		pLight->m_typeShadowed = LTYPE_SHADOW_NONE;
		
	ID idLight = id;

	if (id == -1)
		idLight = addLight(pLight);

	setLightPos(idLight, pCenter);

	pLight->m_vColor = *pColor;
	pLight->m_isEnable = true;
	pLight->m_pBoundVolume = SGCore_CrBound();

	IDirect3DVertexBuffer9 *pVertex;
	pLight->m_pMesh->GetVertexBuffer(&pVertex);
	pLight->m_pBoundVolume->calcBound(pVertex, pLight->m_pMesh->GetNumVertices(), pLight->m_pMesh->GetNumBytesPerVertex());
	mem_release(pVertex);

	if (pLight->m_isGlobal)
		m_idGlobalLight = idLight;

	return idLight;
}

ID CLights::createDirection(ID id, const float3 *pPos, float fDist, const float3 *pColor, const SMQuaternion *pQ, float fTopRadius, float fAngle, bool isShadow/*, const char* bound_volume*/)
{
	CLight *pLight = 0;

	if (id < 0)
		pLight = new CLight();
	else
	{
		pLight = new CLight();
		pLight->m_id = m_aLights[id]->m_id;
		mem_delete(m_aLights[id]);
		m_aLights[id] = pLight;
	}

	pLight->m_isGlobal = false;
	pLight->m_typeLight = LTYPE_LIGHT_DIR;
	pLight->m_fAngle = fAngle;
	float bottom_radius = fTopRadius + fDist * tanf(fAngle / 2.f);
	pLight->m_vTopBottomRadius.x = fTopRadius;
	pLight->m_vTopBottomRadius.y = bottom_radius;

	/*if (StrValidate(bound_volume))
	{
		
	}
	else*/
	SGCore_FCreateCone(fTopRadius, bottom_radius, fDist, &pLight->m_pMesh, 32);

	pLight->m_vTopBottomRadius = float2_t(fTopRadius, bottom_radius);
	pLight->m_vColor = *pColor;
	
	if (pQ)
		pLight->m_qQuaternion = *pQ;

	pLight->m_fDist = fDist;

	pLight->m_isEnable = true;
	float4x4 mpos = SMMatrixTranslation(*pPos);
	pLight->m_mWorld = pLight->m_qQuaternion.GetMatrix() * mpos;

	pLight->m_vPosition = *pPos;
	
	if (isShadow)
	{
		pLight->m_pShadowSM = new CShadowMap();
		pLight->m_pShadowSM->init();
		pLight->m_pShadowSM->setPosition(pPos);
		pLight->m_pShadowSM->setDirection(&(pLight->m_qQuaternion * LIGHTS_DIR_BASE));
		pLight->m_pShadowSM->setAngleNearFar(&float3(fAngle, LIGHTS_LOCAL_STD_NEAR, fDist));
	}
	
	if (isShadow)
		pLight->m_typeShadowed = LTYPE_SHADOW_DYNAMIC;
	else
		pLight->m_typeShadowed = LTYPE_SHADOW_NONE;

	pLight->m_pBoundVolume = SGCore_CrBound();
	
	IDirect3DVertexBuffer9 *pVertex;
	pLight->m_pMesh->GetVertexBuffer(&pVertex);
	pLight->m_pBoundVolume->calcBound(pVertex, pLight->m_pMesh->GetNumVertices(), pLight->m_pMesh->GetNumBytesPerVertex());
	mem_release(pVertex);

	ID idLight = id;

	if (id == -1)
		idLight = addLight(pLight);
	
	return idLight;
}

void CLights::render(ID id, DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	CLight *pLight = m_aLights[id];

	light_data::pDXDevice->SetTransform(D3DTS_WORLD, &(pLight->m_mWorld.operator D3DXMATRIX()));
	pLight->m_pMesh->DrawSubset(0);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + (pLight->m_pMesh->GetNumFaces() / 3));
}

ID CLights::getLightGlobal() const
{
	return m_idGlobalLight;
}

bool CLights::getCastGlobalShadow() const
{
	return m_isCastGlobalShadow;
}

void CLights::setCastGlobalShadow(bool isShadowed)
{
	m_isCastGlobalShadow = isShadowed;
}

void CLights::getLightColor(ID id, float3 *pPos) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	(*pPos) = m_aLights[id]->m_vColor;
}

void CLights::setLightColor(ID id, const float3 *pPos)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_vColor = *pPos;
}

void CLights::getLightPos(ID id, float3 *pPos, bool isReal) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_isGlobal && !isReal)
	{
		pPos->x = m_aLights[id]->m_fGAngleX;
		pPos->y = m_aLights[id]->m_fGAngleY;
		pPos->z = 0;
	}
	else
	{
		pPos->x = m_aLights[id]->m_vPosition.x;
		pPos->y = m_aLights[id]->m_vPosition.y;
		pPos->z = m_aLights[id]->m_vPosition.z;
	}
}

float CLights::getLightPower(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	float power = m_aLights[id]->m_vColor.x;

	if (power < m_aLights[id]->m_vColor.y)
		power = m_aLights[id]->m_vColor.y;

	if (power < m_aLights[id]->m_vColor.z)
		power = m_aLights[id]->m_vColor.z;

	if (m_aLights[id]->m_isGlobal)
		return power;
	else
	{
		return power * (m_aLights[id]->m_fDist / LIGHTS_LOCAL_MAX_DIST);
	}
}

float CLights::getLightDist(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	return m_aLights[id]->m_fDist;
}

void CLights::setLightDist(ID id, float fRadiusHeight, bool isCreate)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_fDist = fRadiusHeight;
	if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_DIR && isCreate)
	{
		m_aLights[id]->m_vTopBottomRadius.y = m_aLights[id]->m_vTopBottomRadius.x + fRadiusHeight * tanf(m_aLights[id]->m_fAngle / 2.f);
		mem_release_del(m_aLights[id]->m_pMesh);
		SGCore_FCreateCone(m_aLights[id]->m_vTopBottomRadius.x, m_aLights[id]->m_vTopBottomRadius.y, fRadiusHeight, &m_aLights[id]->m_pMesh, 32);
	}

	if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->setAngleNearFar(&float3(m_aLights[id]->m_fAngle, 0.1, m_aLights[id]->m_fDist));

	if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_POINT && isCreate)
	{
		mem_release_del(m_aLights[id]->m_pMesh);
		D3DXCreateSphere(light_data::pDXDevice, fRadiusHeight, 20, 20, &m_aLights[id]->m_pMesh, 0);
	}

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, m_aLights[id]->m_fDist));

	if (m_aLights[id]->m_pMesh)
	{
		IDirect3DVertexBuffer9* vertexbuf;
		m_aLights[id]->m_pMesh->GetVertexBuffer(&vertexbuf);
		m_aLights[id]->m_pBoundVolume->calcBound(vertexbuf, m_aLights[id]->m_pMesh->GetNumVertices(), m_aLights[id]->m_pMesh->GetNumBytesPerVertex());
		mem_release_del(vertexbuf);
	}

	lightCountUpdateNull(id);
}

void CLights::setLightShadowIntensity(ID id, float fShadowIntensity)
{
	LIGHTS_PRE_COND_ID(id, _VOID);
	m_aLights[id]->m_fShadowIntensity = fShadowIntensity;
}

float CLights::getLightShadowIntensity(ID id)
{
	return m_aLights[id]->m_fShadowIntensity;
}

void CLights::setLightPos(ID id, const float3 *pPos, bool isReal)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_isGlobal)
	{
		CLight *pLight = m_aLights[id];
		pLight->m_fGAngleX = pPos->x;
		pLight->m_fGAngleY = pPos->y;

		if (pLight->m_fGAngleX > 360 || pLight->m_fGAngleX < 0)
			pLight->m_fGAngleX = 0;

		if (pLight->m_fGAngleY > 360 || pLight->m_fGAngleY < 0)
			pLight->m_fGAngleY = 0;

		float4x4 mat = SMMatrixRotationZ(-D3DXToRadian(pLight->m_fGAngleX)) * SMMatrixRotationY(D3DXToRadian(pLight->m_fGAngleY));
		pLight->m_vPosition = SMVector3Transform(float3(-1, 0, 0), mat);

		pLight->m_vPosition.x *= LIGHTS_POS_G_MAX;
		pLight->m_vPosition.y *= LIGHTS_POS_G_MAX;
		pLight->m_vPosition.z *= LIGHTS_POS_G_MAX;

		pLight->m_mWorld = SMMatrixTranslation(pLight->m_vPosition.x, pLight->m_vPosition.y, pLight->m_vPosition.z);
		if (pLight->m_pShadowPSSM)
			pLight->m_pShadowPSSM->setPosition(&float3(pLight->m_vPosition.x, pLight->m_vPosition.y, pLight->m_vPosition.z));
	}
	else
	{
		m_aLights[id]->m_vPosition.x = (*pPos).x;
		m_aLights[id]->m_vPosition.y = (*pPos).y;
		m_aLights[id]->m_vPosition.z = (*pPos).z;

		float4x4 mTranslation = SMMatrixTranslation(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z);
		m_aLights[id]->m_mWorld = m_aLights[id]->m_qQuaternion.GetMatrix() * mTranslation;

		if (m_aLights[id]->m_pShadowSM)
		{
			m_aLights[id]->m_pShadowSM->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
		}

		if (m_aLights[id]->m_pShadowCube)
		{
			m_aLights[id]->m_mWorld = SMMatrixTranslation(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z);
			m_aLights[id]->m_pShadowCube->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
		}
	}

	lightCountUpdateNull(id);
}

void CLights::getLightOrient(ID id, SMQuaternion *pQ) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	*pQ = m_aLights[id]->m_qQuaternion;
}

void CLights::setLightOrient(ID id, const SMQuaternion *pQ)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_qQuaternion = *pQ;

	if (m_aLights[id]->m_pShadowSM)
	{
		float4x4 mTranslate = SMMatrixTranslation(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z);
		m_aLights[id]->m_mWorld = m_aLights[id]->m_qQuaternion.GetMatrix() * mTranslate;
		m_aLights[id]->m_pShadowSM->setDirection(&(m_aLights[id]->m_qQuaternion * LIGHTS_DIR_BASE));
	}

	lightCountUpdateNull(id);
}

void CLights::setShadowBias(ID id, float fBias)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (m_aLights[id]->m_pShadowSM)
			m_aLights[id]->m_pShadowSM->setBias(fBias);
		else if (m_aLights[id]->m_pShadowCube)
			m_aLights[id]->m_pShadowCube->setBias(fBias);

		lightCountUpdateNull(id);
}

float CLights::getShadowBias(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (m_aLights[id]->m_pShadowSM)
		return m_aLights[id]->m_pShadowSM->getBias();
	else if (m_aLights[id]->m_pShadowCube)
		return m_aLights[id]->m_pShadowCube->getBias();
	return(0);
}

float CLights::getLightTopRadius(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (m_aLights[id]->m_pShadowSM)
		return m_aLights[id]->m_vTopBottomRadius.x;
	return(0);
}

float CLights::getLightAngle(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (m_aLights[id]->m_pShadowSM)
		return m_aLights[id]->m_fAngle;
	return(0);
}


bool CLights::comVisibleForFrustum(ID id, const IFrustum *pFrustum)
{
	LIGHTS_PRE_COND_ID(id, false);

	float3 vCenter;
	float fRadius;
	m_aLights[id]->m_pBoundVolume->getSphere(&vCenter, &fRadius);
	vCenter = SMVector3Transform(vCenter, m_aLights[id]->m_mWorld);

	return pFrustum->sphereInFrustum(&vCenter, fRadius);
}

bool CLights::getVisibleForFrustum(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return m_aLights[id]->m_isVisibleFor;
}

float CLights::comDistFor(ID id, const float3 *pPos)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return SMVector3Distance((float3)m_aLights[id]->m_vPosition, *pPos);
}

void CLights::comVisibleFrustumDistFor(const IFrustum *pFrustum, const float3 *pPos)
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		if (m_aLights[i])
		{
			m_aLights[i]->m_fDistFor = SMVector3Distance((float3)m_aLights[i]->m_vPosition, *pPos);
			m_aLights[i]->m_isVisibleFor = comVisibleForFrustum(m_aLights[i]->m_id, pFrustum);
		}
	}
}

float CLights::getDistFor(ID id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return m_aLights[id]->m_fDistFor;
}

IDirect3DTexture9* CLights::getShadow2()
{
	return SGCore_RTGetTexture((m_iHowShadow == 1 ? m_idShadowMap2 : m_idShadowMap));
}

bool CLights::getLightEnable(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return m_aLights[id]->m_isEnable;
}

void CLights::setLightEnable(ID id, bool val)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_isEnable = val;
	lightCountUpdateNull(id);
}

bool CLights::getLightShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return (m_aLights[id]->m_typeShadowed != LTYPE_SHADOW_NONE);
}

LTYPE_LIGHT CLights::getLightType(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LTYPE_LIGHT_NONE);
	
	CLight *pLight = m_aLights[id];
	return pLight->m_typeLight;
}

void CLights::shadowRenderBegin(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->begin();
	else if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->begin();
	else if (m_aLights[id]->m_pShadowPSSM)
		m_aLights[id]->m_pShadowPSSM->begin();
}

void CLights::shadowRenderEnd(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->end();
	else if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->end();
	else if (m_aLights[id]->m_pShadowPSSM)
		m_aLights[id]->m_pShadowPSSM->end();
}

void CLights::shadowRenderPre(ID id, int iCube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->pre(iCube);
	else if (m_aLights[id]->m_pShadowPSSM && iCube >= 0 && iCube < 5)
		m_aLights[id]->m_pShadowPSSM->preRender(iCube);
}

void CLights::shadowRenderPost(ID id, int iCube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->post(iCube);
}

void CLights::initShaderOfTypeMaterial(ID id, int iTypeMaterial, const float4x4 *pWorld)
{
	float4x4 mWorld = (pWorld ? (*pWorld) : SMMatrixIdentity());
	
	float4x4 mWVP;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mWVP);
	mWVP = SMMatrixTranspose(mWorld * mWVP);
	float4x4 mW = SMMatrixTranspose(mWorld);

	float4_t vPos = m_aLights[id]->m_vPosition;
	vPos.w = m_aLights[id]->m_fDist;

	if (m_aLights[id]->m_pShadowSM)
	{
		if (iTypeMaterial == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomDirect, "g_mWVP", &mWVP);
			//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomDirect, "g_vLightPos", &vPos);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomDirect);
		}
		else if (iTypeMaterial == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassDirect, "g_mWVP", &mWVP);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenDirect);
		}
		else if (iTypeMaterial == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeDirect, "g_mWVP", &mWVP);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenDirect);
		}
		else if (iTypeMaterial == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinDirect, "g_mWVP", &mWVP);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthSkinDirect);
		}
	}
	else if (m_aLights[id]->m_pShadowPSSM)
	{
		if (iTypeMaterial == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSM, "g_mWVP", &mWVP);
			//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomPSSM, "g_vLightPos", &vPos);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSM);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomPSSM);
		}
		else if (iTypeMaterial == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassPSSM, "g_mWVP", &mWVP);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassPSSM);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenPSSM);
		}
		else if (iTypeMaterial == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreePSSM, "g_mWVP", &mWVP);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreePSSM);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenPSSM);
		}
		else if (iTypeMaterial == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinPSSM, "g_mWVP", &mWVP);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinPSSM);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthSkinPSSM);
		}
	}
	else if (m_aLights[id]->m_pShadowCube)
	{
		if (iTypeMaterial == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_mWVP", &mWVP);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_mW", &mW);
			
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomCube, "g_vLightPos", &vPos);
			
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomCube);
		}
		else if (iTypeMaterial == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube, "g_mWVP", &mWVP);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube, "g_vLightPos", &vPos);
			
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenCube);
		}
		else if (iTypeMaterial == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube, "g_mWVP", &mWVP);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube, "g_vLightPos", &vPos);
			
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenCube);
		}
		else if (iTypeMaterial == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube, "g_mWVP", &mWVP);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube, "g_mW", &mW);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube, "g_vLightPos", &vPos);
			
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthSkinCube);
		}
	}
}

IFrustum* CLights::getLightFrustum(ID id, int how) const
{
	LIGHTS_PRE_COND_ID(id, 0);

		if (m_aLights[id]->m_pShadowSM)
		{
			if (how == 0)
				return m_aLights[id]->m_pShadowSM->m_pFrustum;
		}
		else if (m_aLights[id]->m_pShadowCube)
		{
			if (how >= 0 && how < 6)
				return m_aLights[id]->m_pShadowCube->m_aFrustums[how];
		}
		else if (m_aLights[id]->m_pShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return m_aLights[id]->m_pShadowPSSM->m_aFrustums[how];
		}
	return(NULL);
}

IFrustum* CLights::getLightFrustumG(ID id, int split) const
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (m_aLights[id]->m_pShadowPSSM && split >= 0 && split < 4 && m_aLights[id]->m_pShadowPSSM->m_aFrustums[split])
		return m_aLights[id]->m_pShadowPSSM->m_aFrustums[split];
	return(NULL);
}

void CLights::updateLightGFrustums(ID id, int split, const float3* pos, const float3* dir)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_pShadowPSSM->updateFrustums(split, pos, dir);
}

void CLights::shadowGen2(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->genShadow(SGCore_RTGetTexture(m_idShadowMap));
	else if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->genShadow(SGCore_RTGetTexture(m_idShadowMap));
	else if (m_aLights[id]->m_pShadowPSSM)
	{
		if (!m_isCastGlobalShadow)
			m_aLights[id]->m_pShadowPSSM->genShadow(SGCore_RTGetTexture(m_idShadowMap));
		else
			m_aLights[id]->m_pShadowPSSM->genShadowAll(SGCore_RTGetTexture(m_idShadowMap));
	}
}

void CLights::shadowNull()
{
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	SGCore_RTGetTexture(m_idShadowMap)->GetSurfaceLevel(0, &RenderSurf);
	light_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	light_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	light_data::pDXDevice->SetVertexShader(0);
	light_data::pDXDevice->SetPixelShader(0);

	light_data::pDXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

	m_iHowShadow = 0;
}

void CLights::setLightAngle(ID id, float angle, bool is_create)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		m_aLights[id]->m_fAngle = angle;
		if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_DIR && is_create)
		{
			mem_release_del(m_aLights[id]->m_pMesh);
			m_aLights[id]->m_vTopBottomRadius.y = m_aLights[id]->m_vTopBottomRadius.x + m_aLights[id]->m_fDist * tanf(angle / 2.f);
			m_aLights[id]->m_fAngle = angle;
			SGCore_FCreateCone(m_aLights[id]->m_vTopBottomRadius.x, m_aLights[id]->m_vTopBottomRadius.y, m_aLights[id]->m_fDist, &m_aLights[id]->m_pMesh, 32);
		}

		if (m_aLights[id]->m_pMesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			m_aLights[id]->m_pMesh->GetVertexBuffer(&vertexbuf);
			m_aLights[id]->m_pBoundVolume->calcBound(vertexbuf, m_aLights[id]->m_pMesh->GetNumVertices(), m_aLights[id]->m_pMesh->GetNumBytesPerVertex());
			mem_release(vertexbuf);
		}

		if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_DIR && m_aLights[id]->m_pShadowSM)
			m_aLights[id]->m_pShadowSM->setAngleNearFar(&float3(angle, 0.1, m_aLights[id]->m_fDist));

		lightCountUpdateNull(id);
}

void CLights::setLightTopRadius(ID id, float top_radius)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (m_aLights[id]->m_typeLight == 1)
		{
			mem_release_del(m_aLights[id]->m_pMesh);
			m_aLights[id]->m_vTopBottomRadius.x = top_radius;
			SGCore_FCreateCone(top_radius, m_aLights[id]->m_vTopBottomRadius.y, m_aLights[id]->m_fDist, &m_aLights[id]->m_pMesh, 32);
		}

		if (m_aLights[id]->m_pMesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			m_aLights[id]->m_pMesh->GetVertexBuffer(&vertexbuf);
			m_aLights[id]->m_pBoundVolume->calcBound(vertexbuf, m_aLights[id]->m_pMesh->GetNumVertices(), m_aLights[id]->m_pMesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		lightCountUpdateNull(id);
}

void CLights::setShadowBlurPixel(ID id, float blur_pixel)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (m_aLights[id]->m_pShadowCube)
			m_aLights[id]->m_pShadowCube->setBlurPixel(blur_pixel);
		else if (m_aLights[id]->m_pShadowSM)
			m_aLights[id]->m_pShadowSM->setBlurPixel(blur_pixel);
		else if (m_aLights[id]->m_pShadowPSSM)
			m_aLights[id]->m_pShadowPSSM->setBlurPixel(blur_pixel);

		lightCountUpdateNull(id);
}

float CLights::getShadowBlurPixel(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (m_aLights[id]->m_pShadowCube)
			return m_aLights[id]->m_pShadowCube->getBlurPixel();
		else if (m_aLights[id]->m_pShadowSM)
			return m_aLights[id]->m_pShadowSM->getBlurPixel();
		else if (m_aLights[id]->m_pShadowPSSM)
			return m_aLights[id]->m_pShadowPSSM->getBlurPixel();
	return(0);
}

void CLights::setShadowLocalNear(ID id, float slnear)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->setNear(slnear);
	else if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->setNear(slnear);

	lightCountUpdateNull(id);
}

float CLights::getShadowLocalNear(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (m_aLights[id]->m_pShadowCube)
		return m_aLights[id]->m_pShadowCube->getNear();
	else if (m_aLights[id]->m_pShadowSM)
		return m_aLights[id]->m_pShadowSM->getNear();
	return(0);
}

void CLights::setShadowLocalFar(ID id, float slfar)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
	{
		float2 tmpnf;
		m_aLights[id]->m_pShadowCube->getNearFar(&tmpnf);
		tmpnf.y = slfar;
		m_aLights[id]->m_pShadowCube->setNearFar(&tmpnf);
	}
	else if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->setFar(slfar);

	lightCountUpdateNull(id);
}

float CLights::getShadowLocalFar(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (m_aLights[id]->m_pShadowCube)
	{
		float2 tmpnf;
		m_aLights[id]->m_pShadowCube->getNearFar(&tmpnf);
		return tmpnf.y;
	}
	else if (m_aLights[id]->m_pShadowSM)
		return m_aLights[id]->m_pShadowSM->getFar();
	else
		return m_aLights[id]->m_fDist;
}

void CLights::setLightCubeEdgeEnable(ID id, int edge, bool enable)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->setEnableCubeEdge(edge, enable);

	lightCountUpdateNull(id);
}

bool CLights::getLightCubeEdgeEnable(ID id, int edge) const
{
	LIGHTS_PRE_COND_ID(id, false);

	if (m_aLights[id]->m_pShadowCube)
		return m_aLights[id]->m_pShadowCube->getEnableCubeEdge(edge);
	return(false);
}

ID CLights::getLightIDArr(ID id, ID inid, int how)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (m_aLights[id]->m_pShadowSM)
		{
			if (how == 0)
				return m_aLights[id]->m_pShadowSM->getIdVisCalcObj(inid);
		}
		else if (m_aLights[id]->m_pShadowCube)
		{
			if (how >= 0 && how < 6)
				return m_aLights[id]->m_pShadowCube->getIdVisCalcObj(inid, how);
		}
		else if (m_aLights[id]->m_pShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return m_aLights[id]->m_pShadowPSSM->getIdVisCalcObj(inid, how);
		}
	return(-1);
}

void CLights::setLightIDArr(ID id, ID inid, int how, ID id_arr)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (m_aLights[id]->m_pShadowSM)
		{
			if (how == 0)
				m_aLights[id]->m_pShadowSM->setIdVisCalcObj(inid, id_arr);
		}
		else if (m_aLights[id]->m_pShadowCube)
		{
			if (how >= 0 && how < 6)
				m_aLights[id]->m_pShadowCube->setIdVisCalcObj(inid, how, id_arr);
		}
		else if (m_aLights[id]->m_pShadowPSSM)
		{
			if (how >= 0 && how < 5)
				m_aLights[id]->m_pShadowPSSM->setIdVisCalcObj(inid, how, id_arr);
		}
}

void CLights::setLightTypeShadowed(ID id, LTYPE_SHADOW type)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_typeShadowed == type)
		return;

	m_aLights[id]->m_typeShadowed = type;

	if (type != LTYPE_SHADOW::LTYPE_SHADOW_NONE)
	{
		if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_DIR)
		{
			if (!m_aLights[id]->m_pShadowSM)
			{
				m_aLights[id]->m_pShadowSM = new CShadowMap();
				m_aLights[id]->m_pShadowSM->init();
				m_aLights[id]->m_pShadowSM->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
				m_aLights[id]->m_pShadowSM->setDirection(&(m_aLights[id]->m_qQuaternion * LIGHTS_DIR_BASE));
				m_aLights[id]->m_pShadowSM->setAngleNearFar(&float3(m_aLights[id]->m_fAngle, 0.1, m_aLights[id]->m_fDist));
			}
		}
		else if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_GLOBAL)
		{
			if (m_aLights[id]->m_isGlobal)
			{
				if (!m_aLights[id]->m_pShadowPSSM)
				{
					m_aLights[id]->m_pShadowPSSM = new CShadowGlobal();
					m_aLights[id]->m_pShadowPSSM->init();
					m_aLights[id]->m_pShadowPSSM->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
				}
			}
		}
		else if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_POINT)
		{
			if (!m_aLights[id]->m_pShadowCube)
			{
				m_aLights[id]->m_pShadowCube = new CShadowMapCube();
				m_aLights[id]->m_pShadowCube->init();
				m_aLights[id]->m_pShadowCube->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
				m_aLights[id]->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, m_aLights[id]->m_fDist));
			}
		}
	}
	else
	{
		int qwerty = 0;

		if (m_aLights[id]->m_pShadowSM)
		{
			mem_delete(m_aLights[id]->m_pShadowSM);
		}
		else if (m_aLights[id]->m_pShadowCube)
		{
			mem_delete(m_aLights[id]->m_pShadowCube);
		}
	}

	lightCountUpdateNull(id);
}

LTYPE_SHADOW CLights::getLightTypeShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LTYPE_SHADOW_NONE);

	return m_aLights[id]->m_typeShadowed;
}


bool CLights::lightCountUpdateUpdate(ID id, const float3* viewpos, int ghow)
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			CLight* tmpl = m_aLights[id];
			if (tmpl->m_pShadowPSSM)
			{
				if (ghow >= 0 && ghow < 4)
				{
					if (tmpl->m_pShadowPSSM->m_aIsUpdate[ghow] >= ghow*LIGHTS_UPDATE_PSSM_SPLIT)
						tmpl->m_pShadowPSSM->m_aIsUpdate[ghow] = -1;
				}
				else
				{
					if (tmpl->m_pShadowPSSM->m_aIsUpdate[ghow] >= LIGHTS_UPDATE_PSSM_SPLIT_5)
						tmpl->m_pShadowPSSM->m_aIsUpdate[ghow] = -1;
					
				}

				++(tmpl->m_pShadowPSSM->m_aIsUpdate[ghow]);
				
				return (tmpl->m_pShadowPSSM->m_aIsUpdate[ghow] == 0);
			}
		}

		else if (m_aLights[id]->m_typeShadowed == LTYPE_SHADOW_STATIC)
		{
			if (m_aLights[id]->m_iCountUpdate < LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
			{
				++(m_aLights[id]->m_iCountUpdate);
				return true;
			}
			else
				return false;
		}
		else
		{
			CLight* tmpl = m_aLights[id];

			float3 tmpcenter;
			float tmpradius;

			tmpl->m_pBoundVolume->getSphere(&tmpcenter, &tmpradius);
			float dist = SMVector3Distance(tmpl->m_vPosition, (*viewpos)) - tmpradius;
			if (dist < LIGHTS_UPDATE_L0_DIST)
				tmpl->m_iCountUpdate = -1;
			else if (dist < LIGHTS_UPDATE_L1_DIST && dist > LIGHTS_UPDATE_L0_DIST)
			{
				if (tmpl->m_iCountUpdate > 1)
					tmpl->m_iCountUpdate = -1;
			}
			else if (dist < LIGHTS_UPDATE_L2_DIST && dist > LIGHTS_UPDATE_L1_DIST)
			{
				if (tmpl->m_iCountUpdate > 2)
					tmpl->m_iCountUpdate = -1;
			}
			else if (dist > LIGHTS_UPDATE_L2_DIST)
			{
				tmpl->m_iCountUpdate = 4;
			}

			++(tmpl->m_iCountUpdate);

			return (tmpl->m_iCountUpdate == 0);
		}
	return(false);
}

bool CLights::lightCountUpdateAllowed(ID id, int ghow) const
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			if (m_aLights[id]->m_pShadowPSSM)
			{
				return (m_aLights[id]->m_pShadowPSSM->m_aIsUpdate[ghow] == 0);
			}
		}
		else if (m_aLights[id]->m_typeShadowed == LTYPE_SHADOW_STATIC)
		{
			if (m_aLights[id]->m_iCountUpdate >= LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
				return false;
		}
	return(m_aLights[id]->m_iCountUpdate == 0);
}

void CLights::lightCountUpdateNull(ID id)
{
	if (id >= 0 && id < m_aLights.size())
	{
		LIGHTS_PRE_COND_ID(id, _VOID);
		m_aLights[id]->m_iCountUpdate = 0;
	}
	else if (id < 0)
	{
		for (int i = 0; i < m_aLights.size(); ++i)
		{
			if (m_aLights[i])
				m_aLights[i]->m_iCountUpdate = 0;
		}
	}
}

void CLights::shadowSoft(bool randomsam, float size, bool isfirst)
{
	if (isfirst)
		m_iHowShadow = 0;

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(m_iHowShadow == 0)
			SGCore_RTGetTexture(m_idShadowMap2)->GetSurfaceLevel(0, &RenderSurf);
		else
			SGCore_RTGetTexture(m_idShadowMap)->GetSurfaceLevel(0, &RenderSurf);

	light_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	light_data::pDXDevice->SetRenderTarget(0,RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);

	light_data::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_DEPTH));
	
		if(m_iHowShadow == 0)
			light_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(m_idShadowMap));
		else
			light_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(m_idShadowMap2));
	
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idScreenOut);

		if(randomsam)
		{
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
			light_data::pDXDevice->SetTexture(2, SGCore_LoadTexGetTex(light_data::texture_id::idNoiseTex));
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPPBlurDepthBasedNoise);
		}
		else
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPPBlurDepthBasedNoise, "g_vPixelSize", &float2_t(size / float(*r_win_width), size / float(*r_win_height)));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPPBlurDepthBasedNoise, "g_vNearFar", &float2_t(*r_near, *r_far));
		}
		else
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPPBlurDepthBased, "g_vPixelSize", &float2_t(size / float(*r_win_width), size / float(*r_win_height)));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPPBlurDepthBased, "g_vNearFar", &float2_t(*r_near, *r_far));
		}

	
	SGCore_ScreenQuadDraw();

	light_data::pDXDevice->SetVertexShader(0);
	light_data::pDXDevice->SetPixelShader(0);

	light_data::pDXDevice->SetRenderTarget(0,BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

		if(m_iHowShadow == 1)
			m_iHowShadow = 0;
		else
			m_iHowShadow = 1;
}

void CLights::set4Or3Splits(ID id, bool is4)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowPSSM)
		m_aLights[id]->m_pShadowPSSM->set4Or3Splits(is4);
}

bool CLights::get4Or3Splits(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	if (m_aLights[id]->m_pShadowPSSM)
		return m_aLights[id]->m_pShadowPSSM->get4Or3Splits();

	return false;
}

//##########################################################################

int CLights::delGetCount()
{
	return m_aDelLights.size();
}

LTYPE_LIGHT CLights::delGetType(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, LTYPE_LIGHT_NONE);
	return m_aDelLights[key]->m_typeLight;
}

void CLights::delDel(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, _VOID);
	mem_delete(m_aDelLights[key]);
	m_aDelLights.erase(key);
}

ID CLights::delGetIDArr(ID key, ID inid, int how)
{
	LIGHTS_PRE_COND_KEY_DEL(key, -1);

	if (m_aDelLights[key]->m_pShadowSM)
	{
		if (how == 0)
			return m_aDelLights[key]->m_pShadowSM->getIdVisCalcObj(inid);
	}
	else if (m_aDelLights[key]->m_pShadowCube)
	{
		if (how >= 0 && how < 6)
			return m_aDelLights[key]->m_pShadowCube->getIdVisCalcObj(inid, how);
	}
	else if (m_aDelLights[key]->m_pShadowPSSM)
	{
		if (how >= 0 && how < 5)
			return m_aDelLights[key]->m_pShadowPSSM->getIdVisCalcObj(inid, how);
	}
	return(-1);
}