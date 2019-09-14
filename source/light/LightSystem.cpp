#include "LightSystem.h"

CLightSystem::~CLightSystem()
{
	mem_delete(m_pSun);
	mem_release(m_pShapeSphere);
	mem_release(m_pShapeCone);
}

IXLightSun *CLightSystem::createSun()
{
	if(m_pSun)
	{
		m_pSun->AddRef();
	}
	else
	{
		m_pSun = new CXLightSun(this);
		m_aLights.push_back(m_pSun);
	}
	return(m_pSun);
}

IXLightSun *CLightSystem::getSun()
{
	if(m_pSun)
	{
		m_pSun->AddRef();
	}
	return(m_pSun);
}

IXLightPoint *CLightSystem::createPoint()
{
	CXLightPoint *pLight = m_poolPoint.Alloc(this);
	m_aLights.push_back(pLight);
	return(pLight);
}
IXLightSpot *CLightSystem::createSpot()
{
	CXLightSpot *pLight = m_poolSpot.Alloc(this);
	m_aLights.push_back(pLight);
	return(pLight);
}

UINT CLightSystem::getCount()
{
	return(m_aLights.size());
}
IXLight *CLightSystem::getLight(ID id)
{
	assert(ID_VALID(id) && id < (ID)m_aLights.size());

	return(m_aLights[id]);
}

void CLightSystem::destroySun(IXLightSun *pLight)
{
	assert(m_pSun == pLight);
	_deleteLight(m_pSun);
	mem_delete(m_pSun);
}
void CLightSystem::destroyPoint(IXLightPoint *_pLight)
{
	CXLightPoint *pLight = dynamic_cast<CXLightPoint*>(_pLight);
	_deleteLight(pLight);
	m_poolPoint.Delete(pLight);
}
void CLightSystem::destroySpot(IXLightSpot *_pLight)
{
	CXLightSpot *pLight = dynamic_cast<CXLightSpot*>(_pLight);
	_deleteLight(pLight);
	m_poolSpot.Delete(pLight);
}

void CLightSystem::_deleteLight(CXLight *pLight)
{
	for(UINT i = 0, l = m_aLights.size(); i < l; ++i)
	{
		if(m_aLights[i] == pLight)
		{
			m_aLights.erase(i);
			break;
		}
	}
}

IMesh *CLightSystem::getShapeSphere()
{
	if(!m_pShapeSphere)
	{
		SGCore_FCreateSphere(1.0f, 16, 16, &m_pShapeSphere);
	}
	m_pShapeSphere->AddRef();
	return(m_pShapeSphere);
}

IMesh *CLightSystem::getShapeCone()
{
	if(!m_pShapeCone)
	{
		SGCore_FCreateCone(0.0f, 1.0f, 1.0f, &m_pShapeCone, 16);
	}
	m_pShapeCone->AddRef();
	return(m_pShapeCone);
}

void CLightSystem::updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax)
{
	for(UINT i = 0, l = m_aLights.size(); i < l; ++i)
	{
		if(m_aLights[i]->isEnabled())
		{
			m_aLights[i]->updateVisibility(pMainCamera, vLPVmin, vLPVmax);
		}
	}
}
