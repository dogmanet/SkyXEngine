
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SHADOW_H
#define __SHADOW_H

#include <gdefines.h>

#include <common/Math.h>
#include <common/Array.h>
#include <graphix/graphix.h>
#include <gcore/sxgcore.h>
#include <light/IXLight.h>

class IXTexture;
class IBaseShadowMap
{
public:
	virtual void genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals) = 0;
};

//##########################################################################

class IBaseReflectiveShadowMap
{
public:
	virtual void genLPV(bool isDebug = false) = 0;
};

//##########################################################################

class CShadowMap: public IBaseShadowMap
{
public:
	CShadowMap();
	~CShadowMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXDevice *pContext, UINT uSize);

	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genShadow(IGXTexture2D *shadowmap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals) override;

private:
	IGXDevice *m_pDevice = NULL;

	static IGXDepthStencilSurface *ms_pDepthStencilSurface;
	static UINT ms_uDepthStencilSurfaceRefCount;

	static IGXSamplerState *ms_pSamplerPointWrap;
	static IGXSamplerState *ms_pSamplerPointClamp;
	static IGXSamplerState *ms_pSamplerLinearClamp;
	static IGXSamplerState *ms_pSamplerComparisonLinearClamp;

	static IGXTexture2D *ms_pRandomTexture;

	static void InitDepthStencilSurface(IGXDevice *pContext, UINT uSize);
	static void ReleaseDepthStencilSurface();

	IGXTexture2D *m_pDepthMap = NULL;

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.0001f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	IXLight *m_pLight = NULL;

	float4x4 m_mView;
	float4x4 m_mProj;

	struct
	{
		struct
		{
			SMMATRIX mMatrixTexture;
			float3 vPixelMapSizeBias;
		} ps;
	} m_shaderData;
	IGXConstantBuffer *m_pShaderDataPS = NULL;
	IGXConstantBuffer *m_pShaderDataInjectVS = NULL;

	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;
};

//##########################################################################

class CReflectiveShadowMap: public IBaseReflectiveShadowMap
{
public:
	CReflectiveShadowMap();
	~CReflectiveShadowMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXDevice *pContext, UINT uSize);

	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genLPV(bool isDebug = false) override;

private:
	IGXDevice *m_pDevice = NULL;

	static IGXDepthStencilSurface *ms_pDepthStencilSurface32;
	static UINT ms_uDepthStencilSurfaceRefCount;

	static IGXSamplerState *ms_pSamplerPointWrap;
	static IGXSamplerState *ms_pSamplerPointClamp;
	static IGXSamplerState *ms_pSamplerLinearClamp;

	static ID ms_idInjectShader32;
	static ID ms_idInjectDebugShader32;

	static void InitDepthStencilSurface(IGXDevice *pContext, UINT uSize);
	static void ReleaseDepthStencilSurface();

	IGXTexture2D *m_pDepthMap32 = NULL;
	IGXTexture2D *m_pNormalMap32 = NULL;
	IGXTexture2D *m_pFluxMap32 = NULL;

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.0001f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	IXLight *m_pLight = NULL;

	float4x4 m_mView;
	float4x4 m_mProj;

	struct
	{
		struct
		{
			SMMATRIX mMatrixTexture;
			float3 vPixelMapSizeBias;
		} ps;
	} m_shaderData;
	IGXConstantBuffer *m_pShaderDataPS = NULL;
	IGXConstantBuffer *m_pShaderDataInjectVS = NULL;

	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;
};

//##########################################################################

#define PSSM_MAX_SPLITS 6
class CShadowPSSM: public IBaseShadowMap
{
public:
	CShadowPSSM();
	~CShadowPSSM();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXDevice *pContext, UINT uSize);

	void setObserverCamera(ICamera *pCamera);
	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genShadow(IGXTexture2D *shadowmap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals) override;

protected:
	void updateFrustums();
private:
	IGXDevice *m_pDevice = NULL;
	ICamera *m_pCamera = NULL;

	IGXDepthStencilSurface *m_pDepthStencilSurface = NULL;

	IGXSamplerState *m_pSamplerPointWrap = NULL;
	IGXSamplerState *m_pSamplerPointClamp = NULL;
	IGXSamplerState *m_pSamplerLinearClamp = NULL;
	IGXSamplerState *m_pSamplerComparisonLinearClamp = NULL;

	IGXTextureCube *m_pDepthMap = NULL;

	struct Split
	{
		SX_ALIGNED_OP_MEM2();

		float2 vNearFar;
		float4x4 mView;
		float4x4 mProj;
	};
	Split m_splits[PSSM_MAX_SPLITS];

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.0001f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	IXLight *m_pLight = NULL;

	struct
	{
		struct
		{
			SMMATRIX mVP[PSSM_MAX_SPLITS];
		} gs;
		struct
		{
			SMMATRIX mMatrixTextureV[PSSM_MAX_SPLITS];
			SMMATRIX mMatrixTextureP[PSSM_MAX_SPLITS];
			float4 vSizeBoundNearFar;
		} ps;
	} m_shaderData;
	IGXConstantBuffer *m_pShaderDataPS = NULL;
	IGXConstantBuffer *m_pShaderDataGS = NULL;

	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;
};

//##########################################################################

class CReflectiveShadowPSSM: public IBaseShadowMap
{
public:
	CReflectiveShadowPSSM();
	~CReflectiveShadowPSSM();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXDevice *pContext, UINT uSize);

	void setObserverCamera(ICamera *pCamera);
	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	//	void genLPV(bool isDebug = false) override;

private:
	IGXDevice *m_pDevice = NULL;
	ICamera *m_pCamera = NULL;

	IGXDepthStencilSurface *m_pDepthStencilSurface = NULL;

	IGXSamplerState *m_pSamplerPointWrap = NULL;
	IGXSamplerState *m_pSamplerPointClamp = NULL;
	IGXSamplerState *m_pSamplerLinearClamp = NULL;
	IGXSamplerState *m_pSamplerComparisonLinearClamp = NULL;

	ID m_idShader = -1;
	ID m_idInjectShader = -1;
	ID m_idInjectDebugShader = -1;

	struct Split
	{
		SX_ALIGNED_OP_MEM2();

		IGXTexture2D *pDepthMap = NULL;
		IGXTexture2D *pNormalMap = NULL;
		IGXTexture2D *pFluxMap = NULL;

		float4x4 mView;
		float4x4 mProj;
	};
	Split m_splits[PSSM_MAX_SPLITS];

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.0001f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	IXLight *m_pLight = NULL;

	struct
	{
		struct
		{
			SMMATRIX mMatrixTexture[PSSM_MAX_SPLITS];
			float3 vPixelMapSizeBias;
		} ps;
	} m_shaderData;
	IGXConstantBuffer *m_pShaderDataPS = NULL;
	IGXConstantBuffer *m_pShaderDataInjectVS = NULL;

	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;
};

//##########################################################################

class CShadowCubeMap: public IBaseShadowMap
{
public:
	CShadowCubeMap();
	~CShadowCubeMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXDevice *pDevice, UINT uSize);

	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals) override;

private:
	IGXDevice *m_pDevice = NULL;

	static IGXDepthStencilSurface *ms_pDepthStencilSurface;
	static UINT ms_uDepthStencilSurfaceRefCount;

	static IGXSamplerState *ms_pSamplerPointWrap;
	static IGXSamplerState *ms_pSamplerPointClamp;
	static IGXSamplerState *ms_pSamplerLinearClamp;
	static IGXSamplerState *ms_pSamplerComparisonLinearClamp;

	static void InitDepthStencilSurface(IGXDevice *pContext, UINT uSize);
	static void ReleaseDepthStencilSurface();

	IGXTextureCube *m_pDepthMap = NULL;

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.005f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	IXLight *m_pLight = NULL;

	float4x4 m_mView[6];
	float4x4 m_mProj;

	struct
	{
		struct
		{
			SMMATRIX mVP[6];
		} gs;
		struct
		{
			float2 vPixelSizeBias;
		} ps;
	} m_shaderData;
	IGXConstantBuffer *m_pShaderDataPS = NULL;
	IGXConstantBuffer *m_pShaderDataGS = NULL;



	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;


	struct
	{
		struct
		{
			SMMATRIX mVP;
			SMMATRIX mViewInv;
			float2 vNearFar;
			float3 vParamProj;
		} vs;
	} m_lightingShaderData;
	IGXConstantBuffer *m_pLightingShaderDataVS = NULL;
};

//##########################################################################

class CReflectiveShadowCubeMap: public IBaseReflectiveShadowMap
{
public:
	CReflectiveShadowCubeMap();
	~CReflectiveShadowCubeMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXDevice *pDevice, UINT uSize);

	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genLPV(bool isDebug = false) override;

private:
	IGXDevice *m_pDevice = NULL;

	static IGXDepthStencilSurface *ms_pDepthStencilSurface32;
	static UINT ms_uDepthStencilSurfaceRefCount;

	static IGXSamplerState *ms_pSamplerPointWrap;
	static IGXSamplerState *ms_pSamplerPointClamp;
	static IGXSamplerState *ms_pSamplerLinearClamp;

	static ID ms_idInjectShader32;
	static ID ms_idInjectDebugShader32;

	static void InitDepthStencilSurface(IGXDevice *pContext, UINT uSize);
	static void ReleaseDepthStencilSurface();

	IGXTextureCube *m_pDepthMap32 = NULL;
	IGXTextureCube *m_pNormalMap32 = NULL;
	IGXTextureCube *m_pFluxMap32 = NULL;

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.005f;
	float m_fBlurPixel = 0.5f;

	float m_fSize;

	IXLight *m_pLight = NULL;

	float4x4 m_mView[6];
	float4x4 m_mProj;

	struct
	{
		struct
		{
			SMMATRIX mVP[6];
		} gs;
		struct
		{
			float2 vPixelSizeBias;
		} ps;
	} m_shaderData;
	IGXConstantBuffer *m_pShaderDataPS = NULL;
	IGXConstantBuffer *m_pShaderDataGS = NULL;



	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;


	struct
	{
		struct
		{
			SMMATRIX mVP;
			SMMATRIX mViewInv;
			float2 vNearFar;
			float3 vParamProj;
		} vs;
	} m_lightingShaderData;
	IGXConstantBuffer *m_pLightingShaderDataVS = NULL;
};

#endif
