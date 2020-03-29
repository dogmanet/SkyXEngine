#include "ShadowCache.h"
#include <core/sxcore.h>

class CRShadowSizeCvarListener: public IEventListener<XEventCvarChanged>
{
public:
	CRShadowSizeCvarListener(IXCore *pCore, CShadowCache *pShadowCache):
		m_pCore(pCore),
		m_pShadowCache(pShadowCache)
	{
	}

	virtual void onEvent(const XEventCvarChanged *pEvent)
	{
		static const float *s_pfRLSMQuality = GET_PCVAR_FLOAT("r_lsm_quality");
		static const float *s_pfRPSMQuality = GET_PCVAR_FLOAT("r_psm_quality");
		static const float *s_pfRPSSMQuality = GET_PCVAR_FLOAT("r_pssm_quality");
		if(pEvent->pCvar == s_pfRLSMQuality 
			|| pEvent->pCvar == s_pfRPSMQuality
			|| pEvent->pCvar == s_pfRPSSMQuality
			)
		{
			m_pShadowCache->dropCaches();
		}
	}

private:
	CShadowCache *m_pShadowCache;
	IXCore *m_pCore;
};

//##########################################################################

CShadowCache::CShadowCache(IXRenderPipeline *pRenderPipeline, IXMaterialSystem *pMaterialSystem):
	m_pRenderPipeline(pRenderPipeline),
	m_pMaterialSystem(pMaterialSystem)
{
	m_pShadowSizeCvarListener = new CRShadowSizeCvarListener(Core_GetIXCore(), this);
	Core_GetIXCore()->getEventChannel<XEventCvarChanged>(EVENT_CVAR_CHANGED_GUID)->addListener(m_pShadowSizeCvarListener);

	Core_0RegisterCVarFloat("r_lsm_quality", 0.5f, "Коэфициент размера карты глубины для направленных источников света [0.5,4] (низкое, высокое)", FCVAR_NOTIFY);
	Core_0RegisterCVarFloat("r_psm_quality", 0.5f, "Коэфициент размера карты глубины для точечных источников света [0.5,4] (низкое, высокое)", FCVAR_NOTIFY);
	Core_0RegisterCVarFloat("r_pssm_quality", 0.5f, "Коэфициент размера карты глубины для солнца [0.5,4] (низкое, высокое)", FCVAR_NOTIFY);
	Core_0RegisterCVarFloat("r_sm_max_memory", 0.15f, "Максимальный процент от доступной видеопамяти, отводимый под кэш теней");
	Core_0RegisterCVarInt("r_pssm_splits", 6, "Количество PSSM сплитов. Допустимые значения от 1 до 6");
	Core_0RegisterCVarFloat("r_pssm_max_distance", 800.0f, "Дальность прорисовки PSSM");
	
	const char *aszGSRequiredParams[] = {
		"vPosition",
		"vPos",
		NULL
	};
	GXMacro aCubemapDefines[] = {
		{"IS_CUBEMAP", "1"},
		GX_MACRO_END()
	};
	m_pCubemapGeometryShader = m_pMaterialSystem->registerGeometryShader("sm/rsmcube.gs", aszGSRequiredParams, aCubemapDefines);

	char tmp[16];
	for(int i = 0; i < PSSM_MAX_SPLITS; ++i)
	{
		sprintf(tmp, "%d", i + 1);
		GXMacro aPSSMDefines[] = {
			{"PSSM_MAX_SPLITS", tmp},
			GX_MACRO_END()
		};
		m_pPSSMGeometryShader[i] = m_pMaterialSystem->registerGeometryShader("sm/rsmcube.gs", aszGSRequiredParams, aPSSMDefines);
	}

	m_pRenderPassShadow = m_pMaterialSystem->getRenderPass("xShadow");

	GXRasterizerDesc rasterizerDesc;
	//rasterizerDesc.cullMode = GXCULL_FRONT;
	//rasterizerDesc.useConservativeRasterization = true;
	m_pRasterizerConservative = m_pRenderPipeline->getDevice()->createRasterizerState(&rasterizerDesc);

}
CShadowCache::~CShadowCache()
{
	Core_GetIXCore()->getEventChannel<XEventCvarChanged>(EVENT_CVAR_CHANGED_GUID)->removeListener(m_pShadowSizeCvarListener);
	mem_delete(m_pShadowSizeCvarListener);
	mem_delete(m_pShadowPSSM);
}

void CShadowCache::setLightsCount(UINT uPoints, UINT uSpots, bool hasGlobal)
{
	static const float *s_pfRLSMQuality = GET_PCVAR_FLOAT("r_lsm_quality");
	static const float *s_pfRPSMQuality = GET_PCVAR_FLOAT("r_psm_quality");
	static const float *s_pfRPSSMQuality = GET_PCVAR_FLOAT("r_pssm_quality");
	static const float *s_pfRSMMaxMemory = GET_PCVAR_FLOAT("r_sm_max_memory");
	const UINT uSpotShadowmapSize = (UINT)(512 * *s_pfRLSMQuality);
	const UINT uPointShadowmapSize = (UINT)(512 * *s_pfRPSMQuality);
	const UINT uSunShadowmapSize = (UINT)(512 * *s_pfRPSSMQuality);

	size_t stMaxMem = (size_t)(m_pRenderPipeline->getDevice()->getAdapterDesc()->sizeTotalMemory * *s_pfRSMMaxMemory);

	size_t stPointsMemory = uPoints * CShadowCubeMap::GetMapMemory(uPointShadowmapSize);
	size_t stSpotsMemory = uSpots * CShadowMap::GetMapMemory(uSpotShadowmapSize);
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
			uSpots = (UINT)(stSpotsMemory / CShadowMap::GetMapMemory(uSpotShadowmapSize));
			if(!uSpots)
			{
				uSpots = 1;
			}
		}
		if(uPoints)
		{
			uPoints = (UINT)(stPointsMemory / CShadowCubeMap::GetMapMemory(uPointShadowmapSize));
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
			m_aShadowMaps[i].map.init(m_pRenderPipeline->getDevice(), uSpotShadowmapSize);
		}
	}

	if(m_aShadowCubeMaps.size() != uPoints)
	{
		UINT i = m_aShadowCubeMaps.size();
		m_aShadowCubeMaps.resizeFast(uPoints);
		m_aShadowCubeMapsQueue.resizeFast(uPoints);
		for(; i < uPoints; ++i)
		{
			m_aShadowCubeMaps[i].map.init(m_pRenderPipeline->getDevice(), uPointShadowmapSize);
		}

		for(i = 0; i < uPoints; ++i)
		{
			m_aShadowCubeMapsQueue[i] = &m_aShadowCubeMaps[i];
		}
	}

	m_aReadyMaps.resizeFast(uSpots + uPoints + (hasGlobal ? 1 : 0));

	if(hasGlobal)
	{
		if(!m_pShadowPSSM)
		{
			m_pShadowPSSM = new ShadowPSSM();
			m_pShadowPSSM->map.init(m_pRenderPipeline->getDevice(), uSunShadowmapSize);
		}
	}
	else
	{
		mem_delete(m_pShadowPSSM);
	}
}

void CShadowCache::nextFrame()
{
	++m_uCurrentFrame;
	m_aFrameLights.clearFast();
	m_isFirstBunch = true;

	{
		ShadowCubeMap *pSM;
		for(UINT i = 0, l = m_aShadowCubeMaps.size(); i < l; ++i)
		{
			pSM = &m_aShadowCubeMaps[i];

			if(pSM->uLastUsed != UINT_MAX)
			{
				++pSM->uLastUsed;
			}
		}
	}
}

void CShadowCache::addLight(IXLight *pLight)
{
	m_aFrameLights.push_back(pLight);
}

UINT CShadowCache::processNextBunch()
{
	if(!m_aFrameLights.size())
	{
		return(0);
	}

	static const int *r_pssm_splits = GET_PCVAR_INT("r_pssm_splits");

	SMMATRIX mOldView, mOldProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &mOldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mOldProj);

	m_pRenderPipeline->getDevice()->getThreadContext()->setDepthStencilState(NULL);
	m_pRenderPipeline->getDevice()->getThreadContext()->setRasterizerState(m_pRasterizerConservative);

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

		if(false && m_pShadowPSSM && !m_pShadowPSSM->isDirty && (id = m_aFrameLights.indexOf(m_pShadowPSSM->pLight)) >= 0)
		{
			if(m_pShadowPSSM->pLight->isDirty())
			{
				m_pShadowPSSM->pLight->markClean();
				m_pShadowPSSM->isDirty = true;
			}
			m_aFrameLights.erase(id);
			m_pShadowPSSM->shouldProcess = true;
			isSomeFound = true;
		}

		m_isFirstBunch = false;
	}
	
	if(!isSomeFound)
	{
		UINT uPoints = 0, uSpots = 0;
		bool isFound = false;

		m_aShadowCubeMapsQueue.quickSort([](const ShadowCubeMap *a, const ShadowCubeMap *b){
			return(a->uLastUsed > b->uLastUsed);
		});

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
				if(m_aShadowCubeMapsQueue.size() > uPoints)
				{
					m_aShadowCubeMapsQueue[uPoints]->isDirty = true;
					m_aShadowCubeMapsQueue[uPoints]->shouldProcess = true;
					m_aShadowCubeMapsQueue[uPoints]->pLight = m_aFrameLights[i];
					++uPoints;
					m_aFrameLights.erase(i);
					isFound = true;
				}
				break;
			case LIGHT_TYPE_SUN:
				m_pShadowPSSM->isDirty = true;
				m_pShadowPSSM->shouldProcess = true;
				m_pShadowPSSM->pLight = m_aFrameLights[i];
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

	m_pMaterialSystem->bindGS(m_pPSSMGeometryShader[*r_pssm_splits - 1]);

	if(m_pShadowPSSM && m_pShadowPSSM->shouldProcess && m_pCamera)
	{
		m_pShadowPSSM->shouldProcess = false;

		if(m_pShadowPSSM->isDirty)
		{
			m_pShadowPSSM->map.setObserverCamera(m_pCamera);
			m_pShadowPSSM->map.setLight(m_pShadowPSSM->pLight);
			m_pShadowPSSM->map.process(m_pRenderPipeline);
			m_pShadowPSSM->isDirty = false;
		}

		m_aReadyMaps.push_back({&m_pShadowPSSM->map, m_pShadowPSSM->pLight});
	}

	m_pMaterialSystem->bindGS(m_pCubemapGeometryShader);

	m_pMaterialSystem->bindRenderPass(m_pRenderPassShadow, 1);
	{
		ShadowCubeMap *pSM;
		for(UINT i = 0, l = m_aShadowCubeMaps.size(); i < l; ++i)
		{
			pSM = &m_aShadowCubeMaps[i];
			if(pSM->shouldProcess)
			{
				pSM->uLastUsed = 0;
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

IXLight* CShadowCache::getLight(ID id)
{
	assert(ID_VALID(id) && m_aReadyMaps.size() > (UINT)id);
	return(m_aReadyMaps[id].pLight);
}

IBaseShadowMap* CShadowCache::getShadow(ID id)
{
	assert(ID_VALID(id) && m_aReadyMaps.size() > (UINT)id);
	return(m_aReadyMaps[id].pShadowMap);
}

void CShadowCache::setObserverCamera(ICamera *pCamera)
{
	m_pCamera = pCamera;
}

void CShadowCache::dropCaches()
{
	m_aShadowMaps.resizeFast(0);
	m_aShadowCubeMaps.resizeFast(0);
	m_aShadowCubeMapsQueue.resizeFast(0);
	mem_delete(m_pShadowPSSM);
}
