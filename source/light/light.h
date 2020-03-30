
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __LIGHT_H
#define __LIGHT_H

#include <gdefines.h>

//#define SM_D3D_CONVERSIONS
#include <common/Math.h>
#include <common/Array.h>
#include <graphix/graphix.h>
#include <xcommon/IXMutationObserver.h>

#include "sxlight.h"
#include "ml_data.h"

class CLightSystem;
class CXLight: public virtual IXLight
{
public:
	CXLight(CLightSystem *pLightSystem);
	~CXLight();

	LIGHT_TYPE getType();

	float3 getColor();
	void setColor(const float3 &vColor);

	float3 getPosition();
	void setPosition(const float3 &vPosition);

	float getShadowIntencity();
	void setShadowIntencity(float fShadowIntencity);

	bool isEnabled();
	void setEnabled(bool isEnabled);

	bool isShadowDynamic();
	void setShadowDynamic(bool isDynamic);

	void drawShape(IGXDevice *pDevice);
	IGXConstantBuffer *getConstants(IGXDevice *pDevice);

	IXLightSpot *asSpot();
	IXLightSun *asSun();
	IXLightPoint *asPoint();

	float getMaxDistance();

	virtual void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax);
	IXRenderableVisibility *getVisibility() override;
	LIGHT_RENDER_TYPE getRenderType() override
	{
		return(m_renderType);
	}

	bool isDirty() override
	{
		if(m_isDirty)
		{
			m_pMutationObserver->reset();
		}
		return(m_isDirty || m_pMutationObserver->wasTriggered());
	}
	void markClean() override
	{
		m_isDirty = false;
		m_pMutationObserver->reset();
	}

protected:
	virtual SMMATRIX getWorldTM();
	virtual void updatePSConstants(IGXDevice *pDevice) = 0;
	virtual void updateFrustum()
	{
	}

	CLightSystem *m_pLightSystem = NULL;

	LIGHT_TYPE m_type = LIGHT_TYPE_NONE;
	float3_t m_vColor;
	bool m_isEnable = true;
	float3 m_vPosition;
	float m_fShadowIntensity = 1.0f;
	bool m_isShadowDynamic = true;

	IMesh *m_pShape = NULL;

	struct _base_light_data_ps
	{
		float4 vLightColor; //!< xyz: цвет, w: мощность
		float4 vLightPos; //!< xyz: позиция, w: непрозрачность тени
	};

	bool m_isVSDataDirty = true;
	IGXConstantBuffer *m_pVSData = NULL;
	bool m_isPSDataDirty = true;
	IGXConstantBuffer *m_pPSData = NULL;


	IFrustum *m_pFrustum = NULL;
	IXRenderableVisibility *m_pVisibility = NULL;
	LIGHT_RENDER_TYPE m_renderType = LRT_NONE;
	bool m_isDirty = true;

	IXMutationObserver *m_pMutationObserver = NULL;
};

#pragma warning(push)
#pragma warning(disable:4250)

class CXLightPoint: public CXLight, public virtual IXLightPoint
{
public:
	CXLightPoint(CLightSystem *pLightSystem);

	void XMETHODCALLTYPE Release();

	void updateFrustum() override;

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax) override;

protected:
	void updatePSConstants(IGXDevice *pDevice);

	_base_light_data_ps m_dataPS;
};

class CXLightSun: public CXLight, public virtual IXLightSun
{
public:
	SX_ALIGNED_OP_MEM2();

	CXLightSun(CLightSystem *pLightSystem);
	void XMETHODCALLTYPE Release();

	SMQuaternion getDirection();
	void setDirection(const SMQuaternion &qDirection);

	float getMaxDistance();

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax) override;

protected:
	void updatePSConstants(IGXDevice *pDevice);

	_base_light_data_ps m_dataPS;
	SMQuaternion m_qDirection;
};

class CXLightSpot: public CXLight, public virtual IXLightSpot
{
public:
	CXLightSpot(CLightSystem *pLightSystem);
	void XMETHODCALLTYPE Release();

	float getInnerAngle();
	void setInnerAngle(float fAngle);
	float getOuterAngle();
	void setOuterAngle(float fAngle);
	SMQuaternion getDirection();
	void setDirection(const SMQuaternion &qDirection);

	SMMATRIX getWorldTM();

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax) override;
protected:
	void updatePSConstants(IGXDevice *pDevice);
	void updateFrustum() override;

	struct _spot_light_data_ps
	{
		_base_light_data_ps baseData;
		float3 vDir;
		float2 vInnerOuterAngle;
	};
	_spot_light_data_ps m_dataPS;
	float m_fOuterAngle = SM_PIDIV2;
	float m_fInnerAngle = SM_PIDIV4;
	SMQuaternion m_qDirection;
};

#pragma warning(pop)

#endif