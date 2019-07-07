
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
	virtual void genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth) = 0;
	
	virtual void genLPV() = 0;
};

class CShadowMap: public IBaseShadowMap
{
public:
	CShadowMap();
	~CShadowMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXContext *pContext, UINT uSize);
	
	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genShadow(IGXTexture2D *shadowmap, IGXTexture2D *pGBufferDepth);
	void genLPV();
	
private:
	IGXContext *m_pDevice = NULL;

	static IGXDepthStencilSurface *ms_pDepthStencilSurface;
	static UINT ms_uDepthStencilSurfaceRefCount;

	static IGXSamplerState *ms_pSamplerPointWrap;
	static IGXSamplerState *ms_pSamplerPointClamp;
	static IGXSamplerState *ms_pSamplerLinearClamp;

	static ID ms_idShader;
	static ID ms_idInjectShader;
	static IGXTexture2D *ms_pRandomTexture;

	static void InitDepthStencilSurface(IGXContext *pContext, UINT uSize);
	static void ReleaseDepthStencilSurface();

	IGXTexture2D *m_pDepthMap = NULL;
	IGXTexture2D *m_pNormalMap = NULL;
	IGXTexture2D *m_pFluxMap = NULL;

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

class CShadowCubeMap: public IBaseShadowMap
{
public:
	CShadowCubeMap();
	~CShadowCubeMap();

	SX_ALIGNED_OP_MEM2();

	static UINT GetMapMemory(UINT uSize);

	void init(IGXContext *pContext, UINT uSize);

	void setLight(IXLight *pLight);
	void process(IXRenderPipeline *pRenderPipeline);
	void genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth);
	void genLPV()
	{
	}

private:
	IGXContext *m_pDevice = NULL;

	static IGXDepthStencilSurface *ms_pDepthStencilSurface;
	static UINT ms_uDepthStencilSurfaceRefCount;

	static IGXSamplerState *ms_pSamplerPointWrap;
	static IGXSamplerState *ms_pSamplerPointClamp;
	static IGXSamplerState *ms_pSamplerLinearClamp;

	static ID ms_idShader;

	static void InitDepthStencilSurface(IGXContext *pContext, UINT uSize);
	static void ReleaseDepthStencilSurface();

	IGXTextureCube *m_pDepthMap = NULL;
	IGXTextureCube *m_pNormalMap = NULL;
	IGXTextureCube *m_pFluxMap = NULL;

	float4x4 m_mScaleBiasMat;
	float m_fBias = 0.0001f;
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
};

#endif
