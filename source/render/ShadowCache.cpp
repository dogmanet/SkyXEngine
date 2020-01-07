#include "ShadowCache.h"
#include <core/sxcore.h>

CShadowCache::CShadowCache(IXRenderPipeline *pRenderPipeline, IXMaterialSystem *pMaterialSystem):
	m_pRenderPipeline(pRenderPipeline),
	m_pMaterialSystem(pMaterialSystem)
{
	//! @todo implement handling r_lsm_quality change
	Core_0RegisterCVarFloat("r_lsm_quality", 0.5f, "Коэфициент размера карты глубины для локальных источников света относительно размеров окна рендера [0.5,4] (низкое, высокое)", FCVAR_READONLY);
	Core_0RegisterCVarFloat("r_sm_max_memory", 0.15f, "Максимальный процент от доступной видеопамяти, отводимый под кэш теней");
	
	const char *aszGSRequiredParams[] = {
		"vPosition",
		"vPos",
		NULL
	};
	m_pRSMGeometryShader = m_pMaterialSystem->registerGeometryShader("sm/rsmcube.gs", aszGSRequiredParams);

	m_pRenderPassShadow = m_pMaterialSystem->getRenderPass("xShadow");
}
CShadowCache::~CShadowCache()
{
}

void CShadowCache::setLightsCount(UINT uPoints, UINT uSpots, bool hasGlobal)
{
	static const float *s_pfRLSMQuality = GET_PCVAR_FLOAT("r_lsm_quality");
	static const float *s_pfRSMMaxMemory = GET_PCVAR_FLOAT("r_sm_max_memory");
	const UINT uDefaultShadowmapSize = (UINT)(512 * *s_pfRLSMQuality);

	size_t stMaxMem = (size_t)(m_pRenderPipeline->getDevice()->getAdapterDesc()->sizeTotalMemory * *s_pfRSMMaxMemory);

	size_t stPointsMemory = uPoints * CShadowCubeMap::GetMapMemory(uDefaultShadowmapSize);
	size_t stSpotsMemory = uSpots * CShadowMap::GetMapMemory(uDefaultShadowmapSize);
	size_t stSunMemory = 0;

	size_t stTotal = stPointsMemory + stSpotsMemory;
	stMaxMem -= stSunMemory;

	if(stTotal > stMaxMem)
	{
		float fCoeff = (float)stMaxMem / (float)stTotal;

		stPointsMemory = (size_t)(stPointsMemory * fCoeff);
		stSpotsMemory = (size_t)(stSpotsMemory * fCoeff);

		if(uSpots)
		{
			uSpots = (UINT)(stSpotsMemory / CShadowMap::GetMapMemory(uDefaultShadowmapSize));
			if(!uSpots)
			{
				uSpots = 1;
			}
		}
		if(uPoints)
		{
			uPoints = (UINT)(stPointsMemory / CShadowCubeMap::GetMapMemory(uDefaultShadowmapSize));
			if(!uPoints)
			{
				uPoints = 1;
			}
		}
	}

	if(m_aShadowMaps.size() != uSpots)
	{
		UINT i = m_aShadowMaps.size();
		m_aShadowMaps.resizeFast(uSpots);
		for(; i < uSpots; ++i)
		{
			m_aShadowMaps[i].map.init(m_pRenderPipeline->getDevice(), uDefaultShadowmapSize);
		}
	}

	if(m_aShadowCubeMaps.size() != uPoints)
	{
		UINT i = m_aShadowCubeMaps.size();
		m_aShadowCubeMaps.resizeFast(uPoints);
		for(; i < uPoints; ++i)
		{
			m_aShadowCubeMaps[i].map.init(m_pRenderPipeline->getDevice(), uDefaultShadowmapSize);
		}
	}

	m_aReadyMaps.resizeFast(uSpots + uPoints + (hasGlobal ? 1 : 0));
	//m_aShadowCubeMaps.resizeFast(uPoints);
	//@TODO: Init/deinit sun!
}

void CShadowCache::nextFrame()
{
	++m_uCurrentFrame;
	m_aFrameLights.clearFast();
	m_isFirstBunch = true;
}

void CShadowCache::addLight(IXLight *pLight)
{
	m_aFrameLights.push_back(pLight);
}

UINT CShadowCache::processNextBunch()
{
	SMMATRIX mOldView, mOldProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &mOldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mOldProj);

	m_pRenderPipeline->getDevice()->getThreadContext()->setDepthStencilState(NULL);

	bool isSomeFound = false;
	if(m_isFirstBunch)
	{
		ID id;
		{
			ShadowMap *pSM;
			for(UINT i = 0, l = m_aShadowMaps.size(); i < l; ++i)
			{
				pSM = &m_aShadowMaps[i];
				if(!pSM->isDirty && (id = m_aFrameLights.indexOf(pSM->pLight)) >= 0)
				{
					if(pSM->pLight->isDirty())
					{
						pSM->pLight->markClean();
						pSM->isDirty = true;
					}
					m_aFrameLights.erase(id);
					pSM->shouldProcess = true;
					isSomeFound = true;
				}
			}
		}

		{
			ShadowCubeMap *pSM;
			for(UINT i = 0, l = m_aShadowCubeMaps.size(); i < l; ++i)
			{
				pSM = &m_aShadowCubeMaps[i];
				if(!pSM->isDirty && (id = m_aFrameLights.indexOf(pSM->pLight)) >= 0)
				{
					if(pSM->pLight->isDirty())
					{
						pSM->pLight->markClean();
						pSM->isDirty = true;
					}
					m_aFrameLights.erase(id);
					pSM->shouldProcess = true;
					isSomeFound = true;
				}
			}
		}

		if(!m_shadowPSSM.isDirty && (id = m_aFrameLights.indexOf(m_shadowPSSM.pLight)) >= 0)
		{
			if(m_shadowPSSM.pLight->isDirty())
			{
				m_shadowPSSM.pLight->markClean();
				m_shadowPSSM.isDirty = true;
			}
			m_aFrameLights.erase(id);
			m_shadowPSSM.shouldProcess = true;
			isSomeFound = true;
		}

		m_isFirstBunch = false;
	}
	
	if(!isSomeFound)
	{
		UINT uPoints = 0, uSpots = 0;
		bool isFound = false;
		for(int i = (int)m_aFrameLights.size() - 1; i >= 0; --i)
		{
			switch(m_aFrameLights[i]->getType())
			{
			case LIGHT_TYPE_SPOT:
				if(m_aShadowMaps.size() > uSpots)
				{
					m_aShadowMaps[uSpots].isDirty = true;
					m_aShadowMaps[uSpots].shouldProcess = true;
					m_aShadowMaps[uSpots].pLight = m_aFrameLights[i];
					++uSpots;
					m_aFrameLights.erase(i);
					isFound = true;
				}
				break;
			case LIGHT_TYPE_POINT:
				if(m_aShadowCubeMaps.size() > uPoints)
				{
					m_aShadowCubeMaps[uPoints].isDirty = true;
					m_aShadowCubeMaps[uPoints].shouldProcess = true;
					m_aShadowCubeMaps[uPoints].pLight = m_aFrameLights[i];
					++uPoints;
					m_aFrameLights.erase(i);
					isFound = true;
				}
				break;
			case LIGHT_TYPE_SUN:
				m_shadowPSSM.isDirty = true;
				m_shadowPSSM.shouldProcess = true;
				m_shadowPSSM.pLight = m_aFrameLights[i];
				m_aFrameLights.erase(i);
				isFound = true;
				break;
			}
			if(!isFound)
			{
				break;
			}
		}
	}
	m_aReadyMaps.clearFast();

	// render shadows
	m_pMaterialSystem->bindRenderPass(m_pRenderPassShadow);
	{
		ShadowMap *pSM;
		for(UINT i = 0, l = m_aShadowMaps.size(); i < l; ++i)
		{
			pSM = &m_aShadowMaps[i];
			if(pSM->shouldProcess)
			{
				pSM->shouldProcess = false;
				if(pSM->isDirty)
				{
					pSM->map.setLight(pSM->pLight);
					pSM->map.process(m_pRenderPipeline);
					pSM->isDirty = false;
				}

				m_aReadyMaps.push_back({&pSM->map, pSM->pLight});
			}
		}
	}

	if(m_shadowPSSM.shouldProcess)
	{
		m_shadowPSSM.shouldProcess = false;

		if(m_shadowPSSM.isDirty)
		{
			m_shadowPSSM.map.setLight(m_shadowPSSM.pLight);
			m_shadowPSSM.map.process(m_pRenderPipeline);
			m_shadowPSSM.isDirty = false;
		}

		m_aReadyMaps.push_back({&m_shadowPSSM.map, m_shadowPSSM.pLight});
	}

	m_pMaterialSystem->bindGS(m_pRSMGeometryShader);
	m_pMaterialSystem->bindRenderPass(m_pRenderPassShadow, 1);
	{
		ShadowCubeMap *pSM;
		for(UINT i = 0, l = m_aShadowCubeMaps.size(); i < l; ++i)
		{
			pSM = &m_aShadowCubeMaps[i];
			if(pSM->shouldProcess)
			{
				pSM->shouldProcess = false;
				if(pSM->isDirty)
				{
					pSM->map.setLight(pSM->pLight);
					pSM->map.process(m_pRenderPipeline);
					pSM->isDirty = false;
				}

				m_aReadyMaps.push_back({&pSM->map, pSM->pLight});
			}
		}
	}

	m_pMaterialSystem->bindGS(NULL);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &mOldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &mOldProj);

	return(m_aReadyMaps.size());
}

IXLight *CShadowCache::getLight(ID id)
{
	assert(ID_VALID(id) && m_aReadyMaps.size() > (UINT)id);
	return(m_aReadyMaps[id].pLight);
}

IBaseShadowMap *CShadowCache::getShadow(ID id)
{
	assert(ID_VALID(id) && m_aReadyMaps.size() > (UINT)id);
	return(m_aReadyMaps[id].pShadowMap);
}

