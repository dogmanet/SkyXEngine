
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

#include <gcore/sxgcore.h>

#include "IXLight.h"

#define PSSM_MAX_SPLITS 6
#define PSSM_LIGHT_NEAR 0.1f
#define PSSM_LIGHT_FAR 5000.0f

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

	virtual void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax, bool useLPV);
	IXRenderableVisibility *getVisibility() override;
	LIGHT_RENDER_TYPE getRenderType()
	{
		return(m_renderType);
	}

	void testDirty()
	{
		if(m_pMutationObserver->wasTriggered())
		{
			m_pMutationObserver->reset();
			m_dirty = LRT_ALL;
		}
	}
	bool isDirty(LIGHT_RENDER_TYPE type)
	{
		return((m_dirty & type) != 0);
	}
	void markClean(LIGHT_RENDER_TYPE type)
	{
		m_dirty &= ~type;
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
	LIGHT_RENDER_TYPE m_dirty = LRT_ALL;

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

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax, bool useLPV) override;

protected:
	void updatePSConstants(IGXDevice *pDevice);

	_base_light_data_ps m_dataPS;
};

class CXLightSun: public CXLight, public virtual IXLightSun
{
public:
	SX_ALIGNED_OP_MEM2();

	CXLightSun(CLightSystem *pLightSystem);
	~CXLightSun();

	SMQuaternion getDirection();
	void setDirection(const SMQuaternion &qDirection);

	float getMaxDistance();
	void setMaxDistance(float fMax);

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax, bool useLPV) override;

	void updateFrustum() override;

	void setCamera(ICamera *pCamera);

	const SMMATRIX* getPSSMVPs() const;
	void getReflectiveVP(SMMATRIX *pView, SMMATRIX *pProj) const;

	IXRenderableVisibility* getReflectiveVisibility();

	struct Split
	{
		SX_ALIGNED_OP_MEM2();

		float2 vNearFar;
		float4x4 mView;
		float4x4 mProj;
	};

	const Split* getPSSMsplits() const;

protected:
	void updatePSConstants(IGXDevice *pDevice);

	_base_light_data_ps m_dataPS;
	SMQuaternion m_qDirection;

	float m_fMaxDistance = 1000.0f;

	ICamera *m_pCamera = NULL;

	Split m_splits[PSSM_MAX_SPLITS];

	SMMATRIX m_mVPs[PSSM_MAX_SPLITS];
	SMMATRIX m_mReflectiveView;
	SMMATRIX m_mReflectiveProj;

	IFrustum *m_pPSSMFrustum[PSSM_MAX_SPLITS - 1];
	IFrustum *m_pReflectiveFrustum = NULL;
	IXRenderableVisibility *m_pReflectiveVisibility = NULL;
	IXRenderableVisibility *m_pTempVisibility = NULL;
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

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax, bool useLPV) override;
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