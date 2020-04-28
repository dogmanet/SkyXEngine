
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "light.h"
#include "LightSystem.h"
#include <xcommon/IXRenderable.h>
#include <physics/sxphysics.h>


//##########################################################################


CXLight::CXLight(CLightSystem *pLightSystem):
	m_pLightSystem(pLightSystem)
{
	IXRenderPipeline *pPipeline;
	Core_GetIXCore()->getRenderPipeline(&pPipeline);
	pPipeline->newVisData(&m_pVisibility);
	mem_release(pPipeline);

	m_pFrustum = SGCore_CrFrustum();
	m_pMutationObserver = SPhysics_NewMutationObserver();
}
CXLight::~CXLight()
{
	mem_release(m_pFrustum);
	mem_release(m_pShape);
	mem_release(m_pVSData);
	mem_release(m_pPSData);
	mem_release(m_pVisibility);
	mem_release(m_pMutationObserver);
}

LIGHT_TYPE CXLight::getType()
{
	return(m_type);
}

float3 CXLight::getColor()
{
	return(m_vColor);
}
void CXLight::setColor(const float3 &vColor)
{
	if((float3)m_vColor == vColor)
	{
		return;
	}
	m_vColor = vColor;
	m_pMutationObserver->setHalfExtents(float3(getMaxDistance() * 0.5f));
	m_isVSDataDirty = true;
	m_isPSDataDirty = true;
	m_dirty = LRT_ALL;
}

float3 CXLight::getPosition()
{
	return(m_vPosition);
}
void CXLight::setPosition(const float3 &vPosition)
{
	if(m_vPosition == vPosition)
	{
		return;
	}
	m_vPosition = vPosition;
	m_pMutationObserver->setPosition(vPosition);
	m_isVSDataDirty = true;
	m_isPSDataDirty = true;
	m_dirty = LRT_ALL;
}

float CXLight::getShadowIntencity()
{
	return(m_fShadowIntensity);
}
void CXLight::setShadowIntencity(float fShadowIntencity)
{
	if(m_fShadowIntensity == fShadowIntencity)
	{
		return;
	}
	m_fShadowIntensity = fShadowIntencity;
	m_isPSDataDirty = true;
}

bool CXLight::isEnabled()
{
	return(m_isEnable);
}
void CXLight::setEnabled(bool isEnabled)
{
	if(m_isEnable == isEnabled)
	{
		return;
	}
	m_isEnable = isEnabled;
	m_pMutationObserver->setEnabled(isEnabled);
	m_dirty = LRT_ALL;
}

bool CXLight::isShadowDynamic()
{
	return(m_isShadowDynamic);
}
void CXLight::setShadowDynamic(bool isDynamic)
{
	m_isShadowDynamic = isDynamic;
}

void CXLight::drawShape(IGXDevice *pDevice)
{
	assert(pDevice);

	if(m_pShape)
	{
		if(m_isVSDataDirty)
		{
			if(!m_pVSData)
			{
				m_pVSData = pDevice->createConstantBuffer(sizeof(SMMATRIX));
			}
			m_pVSData->update(&SMMatrixTranspose(getWorldTM()));
			m_isVSDataDirty = false;
		}

		IGXContext *pCtx = pDevice->getThreadContext();

		pCtx->setVSConstant(m_pVSData, SCR_OBJECT);

		m_pShape->draw();
	}
}

IGXConstantBuffer* CXLight::getConstants(IGXDevice *pDevice)
{
	if(m_isPSDataDirty)
	{
		updatePSConstants(pDevice);
		m_isPSDataDirty = false;
	}
	m_pPSData->AddRef();
	return(m_pPSData);
}

SMMATRIX CXLight::getWorldTM()
{
	return(SMMatrixScaling(float3(getMaxDistance())) * SMMatrixTranslation(m_vPosition));
}

IXLightSpot* CXLight::asSpot()
{
	if(m_type == LIGHT_TYPE_SPOT)
	{
		return((CXLightSpot*)this);
	}
	return(NULL);
}
IXLightSun* CXLight::asSun()
{
	if(m_type == LIGHT_TYPE_SUN)
	{
		return((CXLightSun*)this);
	}
	return(NULL);
}
IXLightPoint* CXLight::asPoint()
{
	if(m_type == LIGHT_TYPE_POINT)
	{
		return((CXLightPoint*)this);
	}
	return(NULL);
}

float CXLight::getMaxDistance()
{
	return(SMVector3Length2(m_vColor));
}

void CXLight::updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax)
{
	updateFrustum();

	if(m_pFrustum->boxInFrustum(&vLPVmin, &vLPVmax))
	{
		m_renderType |= LRT_LPV;
	}

	if(m_renderType != LRT_NONE)
	{
		m_pVisibility->updateForFrustum(m_pFrustum);
	}
}

IXRenderableVisibility *CXLight::getVisibility()
{
	return(m_pVisibility);
}

//##########################################################################

CXLightPoint::CXLightPoint(CLightSystem *pLightSystem):
	CXLight(pLightSystem)
{
	m_type = LIGHT_TYPE_POINT;
	m_pShape = pLightSystem->getShapeSphere();
}

void CXLightPoint::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pLightSystem->destroyPoint(this);
	}
}

void CXLightPoint::updatePSConstants(IGXDevice *pDevice)
{
	if(!m_pPSData)
	{
		m_pPSData = pDevice->createConstantBuffer(sizeof(m_dataPS));
	}
	m_dataPS.vLightColor = float4(m_vColor, SMVector3Length2(m_vColor));
	m_dataPS.vLightPos = float4(m_vPosition, m_fShadowIntensity);
	m_pPSData->update(&m_dataPS);
}

void CXLightPoint::updateFrustum()
{
	//! @todo optimize me!
	float3 vPos = getPosition();
	float fRadius = getMaxDistance();

	// ax + by + cz + d = 0

	SMPLANE planes[] = {
		SMPLANE(-1.0f,  0.0f,  0.0f, fRadius + vPos.x),
		SMPLANE( 1.0f,  0.0f,  0.0f, fRadius - vPos.x),
		SMPLANE( 0.0f, -1.0f,  0.0f, fRadius + vPos.y),
		SMPLANE( 0.0f,  1.0f,  0.0f, fRadius - vPos.y),
		SMPLANE( 0.0f,  0.0f, -1.0f, fRadius + vPos.z),
		SMPLANE( 0.0f,  0.0f,  1.0f, fRadius - vPos.z),
	};

	m_pFrustum->update(planes, true);
}

void CXLightPoint::updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax)
{
	m_renderType = LRT_NONE;
	
	float3 vOrigin = getPosition();
	if(pMainCamera->getFrustum()->sphereInFrustum(&vOrigin, getMaxDistance()))
	{
		m_renderType |= LRT_LIGHT;
	}

	CXLight::updateVisibility(pMainCamera, vLPVmin, vLPVmax);
}

//##########################################################################

CXLightSun::CXLightSun(CLightSystem *pLightSystem):
	CXLight(pLightSystem)
{
	m_type = LIGHT_TYPE_SUN;
}

void CXLightSun::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pLightSystem->destroySun(this);
	}
}

SMQuaternion CXLightSun::getDirection()
{
	return(m_qDirection);
}
void CXLightSun::setDirection(const SMQuaternion &qDirection)
{
	if(m_qDirection == qDirection)
	{
		return;
	}
	m_qDirection = qDirection;
	m_isVSDataDirty = true;
	m_dirty = LRT_ALL;
}

void CXLightSun::updatePSConstants(IGXDevice *pDevice)
{
	if(!m_pPSData)
	{
		m_pPSData = pDevice->createConstantBuffer(sizeof(m_dataPS));
	}
	m_dataPS.vLightColor = float4(m_vColor, SMVector3Length2(m_vColor));
	m_dataPS.vLightPos = float4(m_qDirection * -LIGHTS_DIR_BASE, m_fShadowIntensity);
	m_pPSData->update(&m_dataPS);
}

float CXLightSun::getMaxDistance()
{
	return(m_fMaxDistance);
}

void CXLightSun::setMaxDistance(float fMax)
{
	m_fMaxDistance = fMax;
}

void CXLightSun::updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax)
{
	m_renderType = LRT_LIGHT | LRT_LPV;
#if 0
	m_renderType = LRT_NONE;

	float3 vOrigin = getPosition();
	if(pMainCamera->getFrustum()->sphereInFrustum(&vOrigin, getMaxDistance()))
	{
		m_renderType |= LRT_LIGHT;
	}
#endif
	CXLight::updateVisibility(pMainCamera, vLPVmin, vLPVmax);
}

//##########################################################################

CXLightSpot::CXLightSpot(CLightSystem *pLightSystem):CXLight(pLightSystem)
{
	m_type = LIGHT_TYPE_SPOT;
	m_pShape = pLightSystem->getShapeCone();
}

void CXLightSpot::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pLightSystem->destroySpot(this);
	}
}

float CXLightSpot::getInnerAngle()
{
	return(m_fInnerAngle);
}
void CXLightSpot::setInnerAngle(float fAngle)
{
	if(m_fInnerAngle == fAngle)
	{
		return;
	}
	m_fInnerAngle = fAngle;
	m_isPSDataDirty = true;
}
float CXLightSpot::getOuterAngle()
{
	return(m_fOuterAngle);
}
void CXLightSpot::setOuterAngle(float fAngle)
{
	if(m_fOuterAngle == fAngle)
	{
		return;
	}
	m_fOuterAngle = fAngle;
	m_isVSDataDirty = true;
	m_isPSDataDirty = true;
	m_dirty = LRT_ALL;
}
SMQuaternion CXLightSpot::getDirection()
{
	return(m_qDirection);
}
void CXLightSpot::setDirection(const SMQuaternion &qDirection)
{
	if(m_qDirection == qDirection)
	{
		return;
	}
	m_qDirection = qDirection;
	m_isVSDataDirty = true;
	m_isPSDataDirty = true;
	m_dirty = LRT_ALL;
}

void CXLightSpot::updatePSConstants(IGXDevice *pDevice)
{
	if(!m_pPSData)
	{
		m_pPSData = pDevice->createConstantBuffer(sizeof(m_dataPS));
	}
	m_dataPS.baseData.vLightColor = float4(m_vColor, SMVector3Length2(m_vColor));
	m_dataPS.baseData.vLightPos = float4(m_vPosition, m_fShadowIntensity);
	m_dataPS.vInnerOuterAngle = float2(cosf(m_fInnerAngle * 0.5f), cosf(m_fOuterAngle * 0.5f));
	m_dataPS.vDir = m_qDirection * LIGHTS_DIR_BASE;
	m_pPSData->update(&m_dataPS);
}

SMMATRIX CXLightSpot::getWorldTM()
{
	float fAngleScale = tanf(m_fOuterAngle * 0.5f);

	return(SMMatrixScaling(float3(fAngleScale, 1.0f, fAngleScale)) * SMMatrixScaling(float3(getMaxDistance())) * m_qDirection.GetMatrix() * SMMatrixTranslation(m_vPosition));
}

void CXLightSpot::updateFrustum()
{
	//! @todo optimize me!
	float3 vPos = getPosition();
	float3 vDir = getDirection() * LIGHTS_DIR_BASE;
	float3 vUp = getDirection() * float3(0.0f, 0.0f, 1.0f);

	SMMATRIX mView = SMMatrixLookAtLH(vPos, vPos + vDir, vUp);
	SMMATRIX mProj = SMMatrixPerspectiveFovLH(getOuterAngle(), 1.0f, 0.025f, getMaxDistance());

	m_pFrustum->update(&mView, &mProj);
}

void CXLightSpot::updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax)
{
	m_renderType = LRT_NONE;

	updateFrustum();
	float3 vOrigin = getPosition();
	if(pMainCamera->getFrustum()->frustumInFrustum(m_pFrustum))
	{
		m_renderType |= LRT_LIGHT;
	}

	CXLight::updateVisibility(pMainCamera, vLPVmin, vLPVmax);
}
