#include "ShadowCache.h"
#include <core/sxcore.h>
#include "shadow.h"

CShadowCache::CShadowCache()
{
	Core_0RegisterCVarFloat("r_sm_max_memory", 0.15f, "Максимальный процент от доступной видеопамяти, отводимый под кэш теней");
}
CShadowCache::~CShadowCache()
{
}

void CShadowCache::setLightsCount(UINT iPoints, UINT iSpots, bool hasGlobal)
{
	static const float *s_pfRLSMQuality = GET_PCVAR_FLOAT("r_lsm_quality");
	const UINT uDefaultShadowmapSize = 512;

	UINT uPointsMemory = 0;
	UINT uSpotsMemory = iSpots * CShadowMap::GetMapMemory(uDefaultShadowmapSize * *s_pfRLSMQuality);
	UINT uSunMemory = 0;


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
}
IXLight *CShadowCache::getLight(ID id)
{
}
