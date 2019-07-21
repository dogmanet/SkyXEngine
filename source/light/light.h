
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
#include <common/String.h>
#include <graphix/graphix.h>

#include "sxlight.h"
#include "ml_data.h"

#define LIGHTS_PRE_COND_ID(id,stdval) \
if (!(id >= 0 && id < (ID)m_aLights.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved index of access '%d'", GEN_MSG_LOCATION, id); return stdval; }\
	else if (!m_aLights[id]) \
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: light is deleted '%d'", GEN_MSG_LOCATION, id); return stdval; }

#define LIGHTS_PRE_COND_KEY_DEL(key,stdval) \
if (!(key >= 0 && key < (ID)m_aDelLights.size()))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: unresolved key of access '%d'", GEN_MSG_LOCATION, key); return stdval; }

#include "shadow.h"


class CLights
{
public:

	CLights();
	~CLights();

	ID createCopy(ID id);

	IXLight *getIXLight(ID id);
#if 0
	void onLostDevice();
	void onResetDevice();
#endif

	int getCountLights() const;
	void clear();

	void resetUpdate4Local();

	//очистка массивов идентификацторов (просчетов видимости)

	void clearIDArr();

	bool getExists(ID id) const;

	ID createPoint(ID id, const  float3 *pCenter, float fDist, const  float3 *pColor, bool isGlobal, bool isShadow);
	ID createDirection(ID id, const float3 *pPos, float fDist, const  float3 *pColor, const SMQuaternion *pOrient, float fTopRadius, float fAngle, bool isShadow);

	void deleteLight(ID id);

	void render(ID id, DWORD timeDelta);

	ID getLightGlobal() const;

	bool getCastGlobalShadow() const;
	void setCastGlobalShadow(bool isShadowed);

	//-----------------------------------------------------------------------------

	char* getLightName(ID id);
	void setLightName(ID id, const char *szName);

	void getLightColor(ID id, float3 *pVec) const;
	void setLightColor(ID id, const float3 *pVec);

	void getLightPos(ID id, float3 *pVec, bool isGreal = false) const;
	void setLightPos(ID id, const float3 *pVec, bool isGreal = false);

	float getLightPower(ID id) const;

	float getLightDist(ID id) const;
	void setLightDist(ID id, float fRadiusHeight, bool isCreate);

	void setLightShadowIntensity(ID id, float fShadowCoef);
	float getLightShadowIntensity(ID id);

	//-----------------------------------------------------------------------------
	
	void getLightOrient(ID id, SMQuaternion *pQ) const;
	void setLightOrient(ID id, const  SMQuaternion *pQ);

	float getLightTopRadius(ID id) const;
	void setLightTopRadius(ID id, float fTopRadius);

	float getLightAngle(ID id) const;
	void setLightAngle(ID id, float fAngle, bool isCreate);

	void setLightCubeEdgeEnable(ID id, int iEdge, bool isEnable);
	bool getLightCubeEdgeEnable(ID id, int iEdge) const;
	
	//-----------------------------------------------------------------------------
	
	void setShadowBias(ID id, float fVal);
	float getShadowBias(ID id) const;
	
	void setShadowBlurPixel(ID id, float fBlurPixel);
	float getShadowBlurPixel(ID id) const;

	void setShadowLocalNear(ID id, float fSlnear);
	float getShadowLocalNear(ID id) const;

	void setShadowLocalFar(ID id, float fSlfar);
	float getShadowLocalFar(ID id) const;

	//-----------------------------------------------------------------------------
	
	bool comVisibleForFrustum(ID id, const IFrustum *pFrustum);
	bool getVisibleForFrustum(ID id);
	float comDistFor(ID id, const float3 *pVec);
	float getDistFor(ID id);
	void comVisibleFrustumDistFor(const IFrustum *pFrustum, const float3 *pVec);

	//-----------------------------------------------------------------------------
#if 0
	bool getLightEnable(ID id) const;
	void setLightEnable(ID id, bool val);
	bool getLightShadowed(ID id) const;
	LTYPE_LIGHT getLightType(ID id) const;
	void setLightTypeShadowed(ID id, LTYPE_SHADOW typeShadow);
	LTYPE_SHADOW getLightTypeShadowed(ID id) const;
#endif

	//-----------------------------------------------------------------------------

	IGXTexture2D* getShadow2();

	void shadowRenderBegin(ID id);
	void shadowRenderEnd(ID id);
	void shadowRenderPre(ID id, int iCube);
	void shadowRenderPost(ID id, int iCube);

	//-----------------------------------------------------------------------------

	void initShaderOfTypeMaterial(ID id, int iTypeMat, const float4x4 *pWorldMat);

	//-----------------------------------------------------------------------------

	IFrustum* getLightFrustum(ID id, int iHow) const;
	IFrustum* getLightFrustumG(ID id, int iSplit) const;

	void updateLightGFrustums(ID id, int iSplit, const float3 *pPos, const float3 *pDir);

	//-----------------------------------------------------------------------------

	ID getLightIDArr(ID id, ID inid, int iHow);
	void setLightIDArr(ID id, ID inid, int iHow, ID idArr);

	//-----------------------------------------------------------------------------

	bool lightCountUpdateUpdate(ID id, const float3 *pViewPos, int iGhow = -1);
	bool lightCountUpdateAllowed(ID id, int iGhow = -1) const;
	void lightCountUpdateNull(ID id);

	//-----------------------------------------------------------------------------

	void toneMappingCom(DWORD timeDelta, float fFactorAdapted = 10);

	//-----------------------------------------------------------------------------

	void set4Or3Splits(ID id, bool is4);
	bool get4Or3Splits(ID id);

	//-----------------------------------------------------------------------------

	void shadowGen(ID id);
	void shadowNull();
	void shadowGen2(ID id);
	void shadowSoft(bool useRandomSam, float fSize, bool isFirst = false);

	//-----------------------------------------------------------------------------

	int delGetCount();
	LTYPE_LIGHT delGetType(ID idKey);
	void delDel(ID idKey);
	ID delGetIDArr(ID idKey, ID inid, int iHow);

	//-----------------------------------------------------------------------------

	struct CLight
	{
		CLight();
		~CLight();

		SX_ALIGNED_OP_MEM;

		ID m_id;

		IXLight *m_pIXLight = NULL;
		
		LTYPE_LIGHT m_typeLight;
		char m_szName[OBJECT_NAME_MAX_LEN];

		bool m_isVisible;
		bool m_isEnable;
		bool m_isGlobal;

		LTYPE_SHADOW m_typeShadowed;
		int m_iCountUpdate;
		
		IMesh *m_pMesh;

		float3 m_vPosition;
		float m_fPower;
		float m_fDist;
		float m_fGAngleX, m_fGAngleY; //sun pos
		SMQuaternion m_qQuaternion;
		float3 m_vColor;

		float2_t m_vTopBottomRadius;
		float m_fAngle;

		float m_fShadowIntensity;

		ISXBound *m_pBoundVolume;
		float4x4 m_mWorldMat;
		bool m_isVSDataDirty = false;
		IGXConstantBuffer *m_pVSData = NULL;
		
		bool m_isVisibleFor;
		float m_fDistFor;

		//PSSM *m_pShadowPSSM;
		//ShadowMapTech *m_pShadowSM;
		//ShadowMapCubeTech *m_pShadowCube;

	};

protected:
	ID addLight(CLight *pObj);

	//! массив всех элементов, основанный на id
	Array<CLight*> m_aLights;	

	//! массив света который надо удалить
	Array<CLight*> m_aDelLights;	

	//! 
	Array<ID> m_aFreeIDs;		

	ID m_idShadowMap;
	ID m_idShadowMap2;
	int m_iHowShadow;

	ID m_idGlobalLight;
	bool m_isCastGlobalShadow;
};


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

	void drawShape(IGXContext *pDevice);
	IGXConstantBuffer *getConstants(IGXContext *pDevice);

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
protected:
	virtual SMMATRIX getWorldTM();
	virtual void updatePSConstants(IGXContext *pDevice) = 0;
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
	void updatePSConstants(IGXContext *pDevice);

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

protected:
	void updatePSConstants(IGXContext *pDevice);

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
	void updatePSConstants(IGXContext *pDevice);
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