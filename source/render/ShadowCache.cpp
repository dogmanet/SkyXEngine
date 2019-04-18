#include "ShadowCache.h"
#include <core/sxcore.h>

CShadowCache::CShadowCache(IXRenderPipeline *pRenderPipeline, IXMaterialSystem *pMaterialSystem):
	m_pRenderPipeline(pRenderPipeline),
	m_pMaterialSystem(pMaterialSystem)
{
	Core_0RegisterCVarFloat("r_sm_max_memory", 0.15f, "Максимальный процент от доступной видеопамяти, отводимый под кэш теней");
	m_idRSMPixelShader = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_rsm_generic.ps");

	GXMACRO aDefsCube[] = {
		{"IS_CUBEMAP", "1"},
		{NULL, NULL}
	};
	m_idRSMCubePixelShader = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_rsm_generic.ps", "sm_rsm_cube.ps", aDefsCube);

	GXMACRO aDefsSpot[] = {
		{"IS_SPOT", "1"},
		{NULL, NULL}
	};
	m_idRSMPixelShaderSpot = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_rsm_generic.ps", "sm_rsm_spot.ps", aDefsSpot);

	m_idRSMCubeGeometryShader = SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "sm_rsm_cube.gs");
}
CShadowCache::~CShadowCache()
{
}

void CShadowCache::setLightsCount(UINT uPoints, UINT uSpots, bool hasGlobal)
{
	static const float *s_pfRLSMQuality = GET_PCVAR_FLOAT("r_lsm_quality");
	static const float *s_pfRSMMaxMemory = GET_PCVAR_FLOAT("r_sm_max_memory");
	const UINT uDefaultShadowmapSize = (UINT)(512 * *s_pfRLSMQuality);

	size_t stMaxMem = (size_t)(m_pRenderPipeline->getDevice()->getAdapterDesc()->uTotalGPUMemory * *s_pfRSMMaxMemory);

	size_t stPointsMemory = 0;
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
			uSpots = stSpotsMemory / CShadowMap::GetMapMemory(uDefaultShadowmapSize);
			if(!uSpots)
			{
				uSpots = 1;
			}
		}
		if(uPoints)
		{
			// uPoints = stPointsMemory / CShadowCubeMap::GetMapMemory(uDefaultShadowmapSize);
			if(!uPoints)
			{
				uPoints = 1;
			}
		}
	}

	if(m_aShadowMaps.size() != uSpots)
	{
		m_aShadowMaps.resizeFast(uSpots);
		for(UINT i = 0, l = m_aShadowMaps.size(); i < l; ++i)
		{
			m_aShadowMaps[i].map.init(m_pRenderPipeline->getDevice(), uDefaultShadowmapSize);
		}
	}

	if(m_aShadowCubeMaps.size() != uPoints)
	{
		m_aShadowCubeMaps.resizeFast(uPoints);
		for(UINT i = 0, l = m_aShadowCubeMaps.size(); i < l; ++i)
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

	UINT uPoints = 0, uSpots = 0;
	for(UINT i = 0, l = m_aFrameLights.size(); i < l; ++i)
	{
		switch(m_aFrameLights[i]->getType())
		{
		case LIGHT_TYPE_SPOT:
			m_aShadowMaps[uSpots].isDirty = true;
			m_aShadowMaps[uSpots].pLight = m_aFrameLights[i];
			++uSpots;
			break;
		case LIGHT_TYPE_POINT:
			m_aShadowCubeMaps[uPoints].isDirty = true;
			m_aShadowCubeMaps[uPoints].pLight = m_aFrameLights[i];
			++uPoints;
			break;
		}
	}

	m_pMaterialSystem->overridePixelShader(m_idRSMPixelShader);
	m_aReadyMaps.clearFast();
	// render shadows
	{
		_shadow_map_s *pSM;
		for(UINT i = 0, l = m_aShadowMaps.size(); i < l; ++i)
		{
			pSM = &m_aShadowMaps[i];
			if(pSM->isDirty)
			{
				/*if(pSM->pLight->getType() == LIGHT_TYPE_SPOT)
				{
				m_pMaterialSystem->overridePixelShader(m_idRSMPixelShaderSpot);
				}
				else
				{
				m_pMaterialSystem->overridePixelShader(m_idRSMPixelShader);
				}*/
				pSM->map.setLight(pSM->pLight);
				pSM->map.process(m_pRenderPipeline);
				pSM->isDirty = false;

				m_aReadyMaps.push_back({&pSM->map, pSM->pLight});
			}
		}
	}

	m_pMaterialSystem->overrideGeometryShader(m_idRSMCubeGeometryShader);
	m_pMaterialSystem->overridePixelShader(m_idRSMCubePixelShader);
	{
		_shadow_cube_map_s *pSM;
		for(UINT i = 0, l = m_aShadowCubeMaps.size(); i < l; ++i)
		{
			pSM = &m_aShadowCubeMaps[i];
			if(pSM->isDirty)
			{
				pSM->map.setLight(pSM->pLight);
				pSM->map.process(m_pRenderPipeline);
				pSM->isDirty = false;

				m_aReadyMaps.push_back({&pSM->map, pSM->pLight});
			}
		}
	}

	m_pMaterialSystem->overrideGeometryShader(-1);
	m_pMaterialSystem->overridePixelShader(-1);

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

