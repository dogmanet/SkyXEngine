#ifndef __LIGHTSYSTEM_H
#define __LIGHTSYSTEM_H

#include "IXLightSystem.h"
#include <common/memalloc.h>
#include "light.h"
#include "ShadowCache.h"

#define LPV_CASCADES_COUNT 3
#define LPV_GRID_SIZE 32
#define LPV_STEP_COUNT 6
#define LUMINANCE_BUFFER_SIZE 1024

class CLevelLoadListener;
class CLightSystem: public IXLightSystem
{
public:
	CLightSystem(IXCore *pCore);
	~CLightSystem();

	SX_ALIGNED_OP_MEM2();

	IXLightSun* XMETHODCALLTYPE newSun() override;
	IXLightSun* XMETHODCALLTYPE getSun() override;
	void destroySun(IXLightSun *pLight);

	IXLightPoint* XMETHODCALLTYPE newPoint() override;
	void destroyPoint(IXLightPoint *pLight);
	IXLightSpot* XMETHODCALLTYPE newSpot() override;
	void destroySpot(IXLightSpot *pLight);

	UINT XMETHODCALLTYPE getCount() override;
	IXLight* XMETHODCALLTYPE getLight(ID id) override;

	IMesh* getShapeSphere();
	IMesh* getShapeCone();

	void XMETHODCALLTYPE updateVisibility() override;

	void XMETHODCALLTYPE setFrameObserverCamera(ICamera *pMainCamera) override;

	void setLevelSize(const float3 &vMin, const float3 &vMax);

	void XMETHODCALLTYPE setGBuffer(IGXTexture2D *pColor, IGXTexture2D *pNormals, IGXTexture2D *pParams, IGXTexture2D *pDepth) override;
	void XMETHODCALLTYPE setRenderPipeline(IXRenderPipeline *pRenderPipeline) override;
	void XMETHODCALLTYPE renderGI(IGXTexture2D *pLightTotal, IGXTexture2D *pTempBuffer) override;
	void XMETHODCALLTYPE renderToneMapping(IGXTexture2D *pSourceLight) override;
	void XMETHODCALLTYPE renderDebug() override;

protected:
	void showGICubes();

protected:
	CXLightSun *m_pSun = NULL;
	MemAlloc<CXLightPoint, 256, 16, 16> m_poolPoint;
	MemAlloc<CXLightSpot, 256, 16, 16> m_poolSpot;

	IXCore *m_pCore;
	IXRenderPipeline *m_pRenderPipeline = NULL;
	IXMaterialSystem *m_pMaterialSystem = NULL;
	IGXDevice *m_pDevice = NULL;

	CShadowCache *m_pShadowCache = NULL;

	ICamera *m_pMainCamera = NULL;

	float3_t m_vLevelMin, m_vLevelMax;
	float m_fLevelDimensions = 0.0f;

	IEventChannel<XEventLevel> *m_pLevelChannel = NULL;
	CLevelLoadListener *m_pLevelListener = NULL;

	Array<CXLight*> m_aLights;

	void _deleteLight(CXLight *pLight);

	IMesh *m_pShapeSphere = NULL;
	IMesh *m_pShapeCone = NULL;


	IGXRenderBuffer *m_pGICubesRB = NULL;
	UINT m_uGICubesCount = 0;
	ID m_idGICubesShader = -1;

	//! G-Buffer
	IGXTexture2D *m_pGBufferColor = NULL;
	IGXTexture2D *m_pGBufferNormals = NULL;
	IGXTexture2D *m_pGBufferParams = NULL;
	IGXTexture2D *m_pGBufferDepth = NULL;

	//###################################

	IGXDepthStencilState *m_pDepthStencilStateDefault = NULL;
	IGXSamplerState *m_pSamplerPointClamp = NULL;
	IGXSamplerState *m_pSamplerLinearWrap = NULL;
	IGXSamplerState *m_pSamplerLinearClamp = NULL;
	IGXSamplerState *m_pSamplerLinearBorder = NULL;

	IGXRasterizerState *m_pRasterizerCullNone = NULL;
	IGXRasterizerState *m_pRasterizerConservative = NULL;

	IGXBlendState *m_pBlendRed = NULL;
	IGXBlendState *m_pBlendAlphaOneOne = NULL;

	IGXDepthStencilState *m_pDepthStencilStateNoZ = NULL;
	IGXDepthStencilState *m_pDepthStencilStateLightBound = NULL;
	IGXDepthStencilState *m_pDepthStencilStateLightClear = NULL;
	IGXDepthStencilState *m_pDepthStencilStateLightShadowNonGlobal = NULL;
	IGXDepthStencilState *m_pDepthStencilStateLightShadowGlobal = NULL;

	//###################################

	ID m_idBlendAmbientSpecDiffColor = -1;
	ID m_idComLightingShadow = -1;
	ID m_idComLightingSpotShadow = -1;
	ID m_idComLightingPSSMShadow = -1;
	ID m_idComLightingGI = -1;

	ID m_idHDRinitLuminance = -1;
	ID m_idHDRAdaptLuminance = -1;
	ID m_idHDRToneMapping = -1;

	//###################################

	struct LPVcascade
	{
		IGXTexture3D *pGIAccumRed = NULL;
		IGXTexture3D *pGIAccumGreen = NULL;
		IGXTexture3D *pGIAccumBlue = NULL;

		IGXTexture3D *pGIAccumRed2 = NULL;
		IGXTexture3D *pGIAccumGreen2 = NULL;
		IGXTexture3D *pGIAccumBlue2 = NULL;
	} m_aLPVs[3];

	ID m_idLightBoundShader = -1;
	ID m_idLPVPropagateShader = -1;
	ID m_idLuminanceReductionShader = -1;
	struct
	{
		struct
		{
			float4 vCenterSize[LPV_CASCADES_COUNT]; // xyz: center / size; w: world size of cell in meters
		} vs;
		//float4 vNearFarLayers;
	} m_lpvCentersShaderData;
	IGXConstantBuffer *m_pLPVcentersShaderData = NULL;
	IGXConstantBuffer *m_pLPVcurrentCascadeShaderData = NULL;

	struct
	{
		float fAdaptationSpeed;
		float fBaseValue;
		float _padding[2];
	} m_toneMappingShaderData;
	IGXConstantBuffer *m_pToneMappingShaderData = NULL;

	struct
	{
		struct
		{
			SMMATRIX mVP; // dummy
			SMMATRIX mViewInv;
			float2 vNearFar;
			float3 vParamProj;
		} vs;
	} m_shadowShaderData;
	IGXConstantBuffer *m_pShadowShaderDataVS = NULL;

	//###################################

	//! Буфер яркости
	IGXTexture2D *m_pLightLuminance = NULL;
	IGXTexture2D *m_pLightLuminance32 = NULL;
	IGXTexture2D *m_pLightLuminance1 = NULL;
	IGXTexture2D *m_pAdaptedLuminance[2];
	UINT m_uCurrAdaptedLuminanceTarget = 0;
};

#endif
