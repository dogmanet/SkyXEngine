
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

	m_idShadowMap = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap", 1);
	m_idShadowMap2 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap2", 1);

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
	
		CLight* tmplight = m_aLights[id];
		CLight* tmplight2 = new CLight();
		tmplight2->m_fAngle = tmplight->m_fAngle;
		tmplight2->m_pBoundVolume = SGCore_CrBound();
		IDirect3DVertexBuffer9* vertexbuf;
		tmplight->m_pMesh->GetVertexBuffer(&vertexbuf);
		tmplight->m_pBoundVolume->calcBound(vertexbuf, tmplight->m_pMesh->GetNumVertices(), tmplight->m_pMesh->GetNumBytesPerVertex());
		mem_release(vertexbuf);
		tmplight2->m_fPower = tmplight->m_fPower;
		tmplight2->m_fDist = tmplight->m_fDist;
		tmplight2->m_vColor = tmplight->m_vColor;
		tmplight2->m_fDistFor = -1;
		tmplight2->m_fGAngleX = tmplight->m_fGAngleX;
		tmplight2->m_fGAngleY = tmplight->m_fGAngleY;
		tmplight2->m_isEnable = tmplight->m_isEnable;
		tmplight2->m_isGlobal = tmplight->m_isGlobal;
		tmplight2->m_isVisible = tmplight->m_isVisible;
		tmplight2->m_isVisibleFor = false;
		sprintf(tmplight2->m_szName, "%s", tmplight->m_szName);
		tmplight2->m_vPosition = tmplight->m_vPosition;
		tmplight2->m_qQuaternion = tmplight->m_qQuaternion;
		tmplight2->m_vTopBottomRadius = tmplight->m_vTopBottomRadius;
		tmplight2->m_typeLight = tmplight->m_typeLight;
		tmplight2->m_mWorldMat = tmplight->m_mWorldMat;

		tmplight2->m_typeShadowed = tmplight->m_typeShadowed;
		tmplight2->m_iCountUpdate = tmplight->m_iCountUpdate;
		
		if (tmplight->m_pShadowPSSM)
		{
			tmplight2->m_pShadowPSSM = new PSSM();
			tmplight2->m_pShadowPSSM->init();
			tmplight2->m_fGAngleX = tmplight->m_fGAngleX;
			tmplight2->m_fGAngleY = tmplight->m_fGAngleY;
			tmplight2->m_pShadowPSSM->setPosition(&float3(tmplight->m_vPosition.x, tmplight->m_vPosition.y, tmplight->m_vPosition.z));
			memcpy((tmplight2->m_pShadowPSSM->m_aNearFar), (tmplight->m_pShadowPSSM->m_aNearFar), sizeof(float2)* 5);
			memcpy((tmplight2->m_pShadowPSSM->m_aIsUpdate), (tmplight->m_pShadowPSSM->m_aIsUpdate), sizeof(int)* 5);
		}
		else
			tmplight2->m_pShadowPSSM = 0;

		if (tmplight->m_pShadowSM)
		{
			tmplight2->m_pShadowSM = new ShadowMapTech();
			tmplight2->m_pShadowSM->init();
			tmplight2->m_pShadowSM->setPosition(&float3(tmplight->m_vPosition.x, tmplight->m_vPosition.y, tmplight->m_vPosition.z));
			tmplight2->m_pShadowSM->setDirection(&(tmplight->m_qQuaternion * LIGHTS_DIR_BASE));
			
			tmplight2->m_pShadowSM->setBias(tmplight->m_pShadowSM->getBias());
			tmplight2->m_pShadowSM->setBlurPixel(tmplight->m_pShadowSM->getBlurPixel());
			float3 anf;
			tmplight2->m_pShadowSM->getAngleNearFar(&anf);
			tmplight2->m_pShadowSM->setAngleNearFar(&anf);
		}
		else
			tmplight2->m_pShadowSM = 0;

		if (tmplight->m_pShadowCube)
		{
			tmplight2->m_pShadowCube = new ShadowMapCubeTech();
			tmplight2->m_pShadowCube->init();
			tmplight2->m_pShadowCube->setPosition(&float3(tmplight->m_vPosition.x, tmplight->m_vPosition.y, tmplight->m_vPosition.z));
			tmplight2->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, tmplight->m_fDist));
			tmplight2->m_pShadowCube->setNear(tmplight2->m_pShadowCube->getNear());
			tmplight2->m_pShadowCube->setFar(tmplight2->m_pShadowCube->getFar());
			tmplight2->m_pShadowCube->setBias(tmplight->m_pShadowCube->getBias());
			tmplight2->m_pShadowCube->setBlurPixel(tmplight->m_pShadowCube->getBlurPixel());

			for (int i = 0; i < 6; ++i)
			{
				tmplight2->m_pShadowCube->setEnableCubeEdge(i, tmplight2->m_pShadowCube->getEnableCubeEdge(i));
			}
		}
		else
			tmplight2->m_pShadowCube = 0;

		tmplight->m_pMesh->CloneMeshFVF(tmplight->m_pMesh->GetOptions(), tmplight->m_pMesh->GetFVF(), light_data::pDXDevice, &(tmplight2->m_pMesh));

	return addLight(tmplight);
}

CLights::CLight::CLight()
{
	m_typeLight = LTYPE_LIGHT_NONE;
	m_szName[0] = 0;

	m_fShadowCoef = 0.01;

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
		if (m_aLights[i]->m_pShadowSM)
			m_aLights[i]->m_pShadowSM->onLostDevice();
		else if (m_aLights[i]->m_pShadowCube)
			m_aLights[i]->m_pShadowCube->onLostDevice();
		else if (m_aLights[i]->m_pShadowPSSM)
			m_aLights[i]->m_pShadowPSSM->onLostDevice();
	}
}

void CLights::onResetDevice()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		if (m_aLights[i]->m_pShadowSM)
			m_aLights[i]->m_pShadowSM->onResetDevice();
		else if (m_aLights[i]->m_pShadowCube)
			m_aLights[i]->m_pShadowCube->onResetDevice();
		else if (m_aLights[i]->m_pShadowPSSM)
			m_aLights[i]->m_pShadowPSSM->onResetDevice();

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

void CLights::clearIDArr()
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		if (!m_aLights[i])
			continue;

		if (m_aLights[i]->m_pShadowSM)
		{
			for (int k = 0; k < m_aLights[i]->m_pShadowSM->getCountIDArrs(); ++k)
			{
				m_aLights[i]->m_pShadowSM->setIDArr(k, -1);
			}
		}
		else if (m_aLights[i]->m_pShadowCube)
		{
			for (int k = 0; k < m_aLights[i]->m_pShadowCube->getCountIDArrs(); ++k)
			{
				m_aLights[i]->m_pShadowCube->setIDArr(k, 0, -1);
				m_aLights[i]->m_pShadowCube->setIDArr(k, 1, -1);
				m_aLights[i]->m_pShadowCube->setIDArr(k, 2, -1);
				m_aLights[i]->m_pShadowCube->setIDArr(k, 3, -1);
				m_aLights[i]->m_pShadowCube->setIDArr(k, 4, -1);
				m_aLights[i]->m_pShadowCube->setIDArr(k, 5, -1);
			}
		}
		else if (m_aLights[i]->m_pShadowPSSM)
		{
			for (int k = 0; k < m_aLights[i]->m_pShadowPSSM->getCountIDArrs(); ++k)
			{
				m_aLights[i]->m_pShadowPSSM->setIDArr(k, 0, -1);
				m_aLights[i]->m_pShadowPSSM->setIDArr(k, 1, -1);
				m_aLights[i]->m_pShadowPSSM->setIDArr(k, 2, -1);
				m_aLights[i]->m_pShadowPSSM->setIDArr(k, 3, -1);
				m_aLights[i]->m_pShadowPSSM->setIDArr(k, 4, -1);
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

	if (m_aLights[id]->m_isGlobal)
		m_idGlobalLight = -1;


	m_aLights[id]->m_isEnable = false;
	m_aDelLights.push_back(m_aLights[id]);
	m_aLights[id] = 0;
	m_aFreeIDs.push_back(id);
}

char* CLights::getLightName(ID id)
{
	LIGHTS_PRE_COND_ID(id, 0);
	return m_aLights[id]->m_szName;
}

void CLights::setLightName(ID id, const char* name)
{
	LIGHTS_PRE_COND_ID(id, _VOID);
	sprintf(m_aLights[id]->m_szName, "%s", name);
}

ID CLights::createPoint(ID id, const float3* center, float dist, const float3* color, bool isglobal, bool is_shadow/*, const char* bound_volume*/)
{
	if (m_idGlobalLight != -1 && isglobal)
	{
		setLightPos(m_idGlobalLight, center);
		setLightColor(m_idGlobalLight, color);
		setLightTypeShadowed(m_idGlobalLight, (is_shadow ? LTYPE_SHADOW_DYNAMIC : LTYPE_SHADOW_NONE));
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - light: global light exists, you can not create 2 global light sources\n", GEN_MSG_LOCATION);
		return m_idGlobalLight;
	}

	CLight* tmplight = 0;// new CLight();

	if (id < 0)
		tmplight = new CLight();
	else
	{
		tmplight = new CLight();
		tmplight->m_id = m_aLights[id]->m_id;
		mem_delete(m_aLights[id]);
		m_aLights[id] = tmplight;
	}

	tmplight->m_isGlobal = isglobal;

	if (isglobal)
	{
		tmplight->m_fDist = LIGHTS_GLOBAL_STD_RADIUS;
	}
	else
	{
		tmplight->m_fDist = dist;
	}

	tmplight->m_typeLight = (isglobal ? LTYPE_LIGHT_GLOBAL : LTYPE_LIGHT_POINT);
		/*if(StrValidate(bound_volume))
		{
			
		}
		else
		{*/
			D3DXCreateSphere(light_data::pDXDevice, tmplight->m_fDist, 20, 20, &tmplight->m_pMesh, 0);
		//}

		if (isglobal && is_shadow)
		{
			tmplight->m_pShadowPSSM = new PSSM();
			tmplight->m_pShadowPSSM->init();
			tmplight->m_pShadowPSSM->setPosition(&float3(center->x, center->y, center->z));
		}
		else if (is_shadow)
		{
			tmplight->m_pShadowCube = new ShadowMapCubeTech();
			tmplight->m_pShadowCube->init();
			tmplight->m_pShadowCube->setPosition(&float3(center->x, center->y, center->z));
			tmplight->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, tmplight->m_fDist));
			
		}

		if (is_shadow)
			tmplight->m_typeShadowed = LTYPE_SHADOW_DYNAMIC;
		else
			tmplight->m_typeShadowed = LTYPE_SHADOW_NONE;
		
	ID tmpid = id;

	if (id == -1)
		tmpid = addLight(tmplight);

	setLightPos(tmpid, &float3(center->x, center->y, center->z));

	tmplight->m_vColor = *color;
	tmplight->m_isEnable = true;
	tmplight->m_pBoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->m_pMesh->GetVertexBuffer(&vertexbuf);
	tmplight->m_pBoundVolume->calcBound(vertexbuf, tmplight->m_pMesh->GetNumVertices(), tmplight->m_pMesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	if (tmplight->m_isGlobal)
		m_idGlobalLight = tmpid;

	return tmpid;
}

ID CLights::createDirection(ID id, const float3* pos, float dist, const float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	CLight* tmplight = 0;

	if (id < 0)
		tmplight = new CLight();
	else
	{
		tmplight = new CLight();
		tmplight->m_id = m_aLights[id]->m_id;
		mem_delete(m_aLights[id]);
		m_aLights[id] = tmplight;
	}

	tmplight->m_isGlobal = false;
	tmplight->m_typeLight = LTYPE_LIGHT_DIR;
	tmplight->m_fAngle = angle;
	float bottom_radius = top_radius + dist * tanf(angle / 2.f);
	tmplight->m_vTopBottomRadius.x = top_radius;
	tmplight->m_vTopBottomRadius.y = bottom_radius;

	/*if (StrValidate(bound_volume))
	{
		
	}
	else*/
		SGCore_FCreateCone(top_radius, bottom_radius, dist, &tmplight->m_pMesh, 32);

	tmplight->m_vTopBottomRadius = float2_t(top_radius, bottom_radius);
	tmplight->m_vColor = *color;
	
	if (orient)
		tmplight->m_qQuaternion = *orient;

	tmplight->m_fDist = dist;

	tmplight->m_isEnable = true;
	float4x4 mpos = SMMatrixTranslation(*pos);
	tmplight->m_mWorldMat = tmplight->m_qQuaternion.GetMatrix() * mpos;

	tmplight->m_vPosition = float3(pos->x, pos->y, pos->z);
	
	if (is_shadow)
	{
		tmplight->m_pShadowSM = new ShadowMapTech();
		tmplight->m_pShadowSM->init();
		tmplight->m_pShadowSM->setPosition(&float3(pos->x, pos->y, pos->z));
		tmplight->m_pShadowSM->setDirection(&(tmplight->m_qQuaternion * LIGHTS_DIR_BASE));
		tmplight->m_pShadowSM->setAngleNearFar(&float3(angle, LIGHTS_LOCAL_STD_NEAR, dist));
	}
	
	if (is_shadow)
		tmplight->m_typeShadowed = LTYPE_SHADOW_DYNAMIC;
	else
		tmplight->m_typeShadowed = LTYPE_SHADOW_NONE;

	tmplight->m_pBoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->m_pMesh->GetVertexBuffer(&vertexbuf);
	tmplight->m_pBoundVolume->calcBound(vertexbuf, tmplight->m_pMesh->GetNumVertices(), tmplight->m_pMesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	ID tmpid = id;

	if (id == -1)
		tmpid = addLight(tmplight);
	
	return tmpid;
}

void CLights::render(ID id, DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	float4x4 tmpwmat = m_aLights[id]->m_mWorldMat;
	light_data::pDXDevice->SetTransform(D3DTS_WORLD, &(m_aLights[id]->m_mWorldMat.operator D3DXMATRIX()));
	m_aLights[id]->m_pMesh->DrawSubset(0);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + (m_aLights[id]->m_pMesh->GetNumFaces() / 3));
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

void CLights::getLightColor(ID id, float3* vec) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	(*vec) = m_aLights[id]->m_vColor;
}

void CLights::setLightColor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_vColor = *vec;
}

void CLights::getLightPos(ID id, float3* vec, bool greal) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_isGlobal && !greal)
	{
		vec->x = m_aLights[id]->m_fGAngleX;
		vec->y = m_aLights[id]->m_fGAngleY;
		vec->z = 0;
	}
	else
	{
		vec->x = m_aLights[id]->m_vPosition.x;
		vec->y = m_aLights[id]->m_vPosition.y;
		vec->z = m_aLights[id]->m_vPosition.z;	
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

void CLights::setLightDist(ID id, float radius_height, bool is_create)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_fDist = radius_height;
	if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_DIR && is_create)
	{
		m_aLights[id]->m_vTopBottomRadius.y = m_aLights[id]->m_vTopBottomRadius.x + radius_height * tanf(m_aLights[id]->m_fAngle / 2.f);
		mem_release_del(m_aLights[id]->m_pMesh);
		SGCore_FCreateCone(m_aLights[id]->m_vTopBottomRadius.x, m_aLights[id]->m_vTopBottomRadius.y, radius_height, &m_aLights[id]->m_pMesh, 32);
	}

	if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->setAngleNearFar(&float3(m_aLights[id]->m_fAngle, 0.1, m_aLights[id]->m_fDist));

	if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_POINT && is_create)
	{
		mem_release_del(m_aLights[id]->m_pMesh);
		D3DXCreateSphere(light_data::pDXDevice, radius_height, 20, 20, &m_aLights[id]->m_pMesh, 0);
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

void CLights::setLightShadowCoef(ID id, float fShadowCoef)
{
	LIGHTS_PRE_COND_ID(id, _VOID);
	m_aLights[id]->m_fShadowCoef = fShadowCoef;
}

float CLights::getLightShadowCoef(ID id)
{
	return m_aLights[id]->m_fShadowCoef;
}

void CLights::setLightPos(ID id, const float3* vec, bool greal)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_isGlobal)
	{
		CLight* tmplight = m_aLights[id];
		tmplight->m_fGAngleX = vec->x;
		tmplight->m_fGAngleY = vec->y;

		if (tmplight->m_fGAngleX > 360 || tmplight->m_fGAngleX < 0)
			tmplight->m_fGAngleX = 0;

		if (tmplight->m_fGAngleY > 360 || tmplight->m_fGAngleY < 0)
			tmplight->m_fGAngleY = 0;


		float4x4 mat = SMMatrixRotationZ(-D3DXToRadian(tmplight->m_fGAngleX)) * SMMatrixRotationY(D3DXToRadian(tmplight->m_fGAngleY));
		tmplight->m_vPosition = SMVector3Transform(float3(-1, 0, 0), mat);


		tmplight->m_vPosition.x *= LIGHTS_POS_G_MAX;
		tmplight->m_vPosition.y *= LIGHTS_POS_G_MAX;
		tmplight->m_vPosition.z *= LIGHTS_POS_G_MAX;

		tmplight->m_mWorldMat = SMMatrixTranslation(tmplight->m_vPosition.x, tmplight->m_vPosition.y, tmplight->m_vPosition.z);
		if (tmplight->m_pShadowPSSM)
			tmplight->m_pShadowPSSM->setPosition(&float3(tmplight->m_vPosition.x, tmplight->m_vPosition.y, tmplight->m_vPosition.z));
	}
	else
	{
				m_aLights[id]->m_vPosition.x = (*vec).x;
				m_aLights[id]->m_vPosition.y = (*vec).y;
				m_aLights[id]->m_vPosition.z = (*vec).z;

				float4x4 mpos = SMMatrixTranslation(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z);
				m_aLights[id]->m_mWorldMat = m_aLights[id]->m_qQuaternion.GetMatrix() * mpos;

			if (m_aLights[id]->m_pShadowSM)
			{
				m_aLights[id]->m_pShadowSM->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
			}

			if (m_aLights[id]->m_pShadowCube)
			{
				m_aLights[id]->m_mWorldMat = SMMatrixTranslation(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z);
				m_aLights[id]->m_pShadowCube->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
			}
	}

	lightCountUpdateNull(id);
}

void CLights::getLightOrient(ID id, SMQuaternion* q) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	*q = m_aLights[id]->m_qQuaternion;
}

void CLights::setLightOrient(ID id, const SMQuaternion* q)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	m_aLights[id]->m_qQuaternion = *q;

	if (m_aLights[id]->m_pShadowSM)
		m_aLights[id]->m_pShadowSM->setDirection(&(m_aLights[id]->m_qQuaternion * LIGHTS_DIR_BASE));

	lightCountUpdateNull(id);
}

void CLights::setShadowBias(ID id, float val)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (m_aLights[id]->m_pShadowSM)
			m_aLights[id]->m_pShadowSM->setBias(val);
		else if (m_aLights[id]->m_pShadowCube)
			m_aLights[id]->m_pShadowCube->setBias(val);

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


bool CLights::comVisibleForFrustum(ID id, const IFrustum* frustum)
{
	LIGHTS_PRE_COND_ID(id, false);

	float3 tmpcenter;
	float tmpradius;
	m_aLights[id]->m_pBoundVolume->getSphere(&tmpcenter, &tmpradius);
	tmpcenter = SMVector3Transform(tmpcenter, m_aLights[id]->m_mWorldMat);

	return frustum->sphereInFrustum(&tmpcenter, tmpradius);
}

bool CLights::getVisibleForFrustum(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return m_aLights[id]->m_isVisibleFor;
}

float CLights::comDistFor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return SMVector3Distance((float3)m_aLights[id]->m_vPosition, *vec);
}

void CLights::comVisibleFrustumDistFor(const IFrustum* frustum, const float3* vec)
{
	for (int i = 0; i < m_aLights.size(); ++i)
	{
		if (m_aLights[i])
		{
			m_aLights[i]->m_fDistFor = SMVector3Distance((float3)m_aLights[i]->m_vPosition, *vec);
			m_aLights[i]->m_isVisibleFor = comVisibleForFrustum(m_aLights[i]->m_id, frustum);
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
	return m_aLights[id]->m_typeLight;
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

void CLights::shadowRenderPre(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->pre(cube);
	else if (m_aLights[id]->m_pShadowPSSM && cube >= 0 && cube < 5)
		m_aLights[id]->m_pShadowPSSM->preRender(cube);
}

void CLights::shadowRenderPost(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_pShadowCube)
		m_aLights[id]->m_pShadowCube->post(cube);
}

void CLights::initShaderOfTypeMaterial(ID id, int typemat, const float4x4* wmat)
{
	float4x4 mWorld = (wmat ? (*wmat) : SMMatrixIdentity());
	
	float4x4 tmpmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &tmpmat);
	tmpmat = SMMatrixTranspose(mWorld * tmpmat);
	float4x4 tmpwmat = SMMatrixTranspose(mWorld);
	if (m_aLights[id]->m_pShadowSM || m_aLights[id]->m_pShadowPSSM)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomPSSMDirect);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassPSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreePSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreePSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinPSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthSkinPSSMDirect);
		}
	}
	else if (m_aLights[id]->m_pShadowCube)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_mWVP", &tmpmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_vLightPos", &m_aLights[id]->m_vPosition);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomCube);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube, "g_mWVP", &tmpmat);
			//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube, "g_vLightPos", &m_aLights[id]->m_vPosition);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGrassCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube, "g_mWVP", &tmpmat);
			//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube, "g_vLightPos", &m_aLights[id]->m_vPosition);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthTreeCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube, "g_mWVP", &tmpmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthSkinCube, "g_vLightPos", &m_aLights[id]->m_vPosition);
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
				return m_aLights[id]->m_pShadowSM->Frustum;
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
				return m_aLights[id]->m_pShadowSM->getIDArr(inid);
		}
		else if (m_aLights[id]->m_pShadowCube)
		{
			if (how >= 0 && how < 6)
				return m_aLights[id]->m_pShadowCube->getIDArr(inid, how);
		}
		else if (m_aLights[id]->m_pShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return m_aLights[id]->m_pShadowPSSM->getIDArr(inid, how);
		}
	return(-1);
}

void CLights::setLightIDArr(ID id, ID inid, int how, ID id_arr)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (m_aLights[id]->m_pShadowSM)
		{
			if (how == 0)
				m_aLights[id]->m_pShadowSM->setIDArr(inid, id_arr);
		}
		else if (m_aLights[id]->m_pShadowCube)
		{
			if (how >= 0 && how < 6)
				m_aLights[id]->m_pShadowCube->setIDArr(inid, how, id_arr);
		}
		else if (m_aLights[id]->m_pShadowPSSM)
		{
			if (how >= 0 && how < 5)
				m_aLights[id]->m_pShadowPSSM->setIDArr(inid, how, id_arr);
		}
}

void CLights::setLightTypeShadowed(ID id, LTYPE_SHADOW type)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (m_aLights[id]->m_typeShadowed == type)
		return;

	m_aLights[id]->m_typeShadowed = type;

	if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_DIR)
	{
		if (!m_aLights[id]->m_pShadowSM)
		{
			m_aLights[id]->m_pShadowSM = new ShadowMapTech();
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
				m_aLights[id]->m_pShadowPSSM = new PSSM();
				m_aLights[id]->m_pShadowPSSM->init();
				m_aLights[id]->m_pShadowPSSM->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
			}
		}
	}
	else if (m_aLights[id]->m_typeLight == LTYPE_LIGHT_POINT)
	{
		if (!m_aLights[id]->m_pShadowCube)
		{
			m_aLights[id]->m_pShadowCube = new ShadowMapCubeTech();
			m_aLights[id]->m_pShadowCube->init();
			m_aLights[id]->m_pShadowCube->setPosition(&float3(m_aLights[id]->m_vPosition.x, m_aLights[id]->m_vPosition.y, m_aLights[id]->m_vPosition.z));
			m_aLights[id]->m_pShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, m_aLights[id]->m_fDist));
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
			return m_aDelLights[key]->m_pShadowSM->getIDArr(inid);
	}
	else if (m_aDelLights[key]->m_pShadowCube)
	{
		if (how >= 0 && how < 6)
			return m_aDelLights[key]->m_pShadowCube->getIDArr(inid, how);
	}
	else if (m_aDelLights[key]->m_pShadowPSSM)
	{
		if (how >= 0 && how < 5)
			return m_aDelLights[key]->m_pShadowPSSM->getIDArr(inid, how);
	}
	return(-1);
}