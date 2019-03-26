#include "ShadowCache.h"
#include <core/sxcore.h>
#include <mtrl/IXMaterialSystem.h>

CShadowCache::CShadowCache(IXRenderPipeline *pRenderPipeline):
	m_pRenderPipeline(pRenderPipeline)
{
	Core_0RegisterCVarFloat("r_sm_max_memory", 0.15f, "Максимальный процент от доступной видеопамяти, отводимый под кэш теней");
	m_idRSMPixelShader = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_rsm_generic.ps");
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

	m_aShadowMaps.resizeFast(uSpots);
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

	IXMaterialSystem *pMaterialSystem = NULL;
	pMaterialSystem->overridePixelShader(m_idRSMPixelShader);

	for(UINT i = 0, l = m_aFrameLights.size(); i < l; ++i)
	{
		if(m_aFrameLights[i]->getType() == LIGHT_TYPE_SPOT)
		{
			m_aShadowMaps[0].isDirty = true;
			m_aShadowMaps[0].pLight = m_aFrameLights[i];
			break;
		}
	}

	m_aReadyMaps.clearFast();
	// render shadows
	_shadow_map_s *pSM;
	for(UINT i = 0, l = m_aShadowMaps.size(); i < l; ++i)
	{
		pSM = &m_aShadowMaps[i];
		if(pSM->isDirty)
		{
			pSM->map.setLight(pSM->pLight);
			pSM->map.process(m_pRenderPipeline);
			pSM->isDirty = false;

			m_aReadyMaps.push_back({&pSM->map, pSM->pLight});
		}
	}

	pMaterialSystem->overridePixelShader(-1);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &mOldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &mOldProj);

	return(m_aReadyMaps.size());
}

IXLight *CShadowCache::getLight(ID id)
{
	assert(ID_VALID(id) && m_aReadyMaps.size() > (UINT)id);
	return(m_aReadyMaps[id].pLight);
}
