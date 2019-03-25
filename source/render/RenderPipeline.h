#ifndef __RENDERPIPELINE_H
#define __RENDERPIPELINE_H

#include <xcommon/IXRenderable.h>
#include <xcommon/IXRenderPipeline.h>
#include <common/array.h>
#include <light/IXLightSystem.h>
#include "ShadowCache.h"

class CRenderPipeline: public IXRenderPipeline
{
public:
	CRenderPipeline(IGXContext *pDevice);
	~CRenderPipeline();

	void resize(UINT uWidth, UINT uHeight, bool isWindowed = true);

	void renderFrame();
	void endFrame();

	SX_ALIGNED_OP_MEM2();

	void renderStage(X_RENDER_STAGE stage);

protected:

	void renderPrepare();
	void renderGBuffer();
	void renderShadows();
	void renderGI();
	void renderPostprocessMain();
	void renderTransparent();
	void renderPostprocessFinal();
	void renderEditor2D();

	void showGICubes();

protected:
	UINT getIndexForStage(X_RENDER_STAGE stage);

	void showTexture(IGXTexture2D *pTexture);
	void showFrameStats();

	struct _render_sys
	{
		IXRenderable *pRenderable;
		UINT uPriority;
	};

	struct _render_stage
	{
		Array<_render_sys> aSystems; // sorted list of render systems
	};

	Array<_render_stage> m_apRenderStages;
	IGXContext *m_pDevice;
	IGXSwapChain *m_pSwapChain = NULL;

	bool m_isWindowed = true;
	UINT m_uOutWidth;
	UINT m_uOutHeight;

	IXLightSystem *m_pLightSystem = NULL;

	//! G-Buffer
	IGXTexture2D *m_pGBufferColor = NULL;
	IGXTexture2D *m_pGBufferNormals = NULL;
	IGXTexture2D *m_pGBufferParams = NULL;
	IGXTexture2D *m_pGBufferDepth = NULL;

	//! Буфер освещения
	IGXTexture2D *m_pLightAmbientDiffuse = NULL;
	IGXTexture2D *m_pLightSpecular = NULL;

	//! Буфер освещения
	IGXTexture2D *m_pLightTotal = NULL;

	IGXDepthStencilState *m_pDepthStencilStateNoZ = NULL;

	struct
	{
		struct
		{
			SMMATRIX mP;
			float4 vTimeDelta;
			float4 vWinSize;
		} vs;
		float4 vNearFarLayers;
	} m_sceneShaderData;
	IGXConstantBuffer *m_pSceneShaderDataVS = NULL;
	IGXConstantBuffer *m_pSceneShaderDataPS = NULL;

	struct
	{
		struct
		{
			SMMATRIX mVP;
			SMMATRIX mViewInv;
			float2 vNearFar;
			float3 vParamProj;
		} vs;
		struct
		{
			float3 vViewPos;
		} ps;
	} m_lightingShaderData;
	IGXConstantBuffer *m_pLightingShaderDataVS = NULL;
	IGXConstantBuffer *m_pLightingShaderDataPS = NULL;

	//###################################
	
	CShadowCache *m_pShadowCache = NULL;

	//###################################

	IGXTexture3D *m_pGIAccumRed = NULL;
	IGXTexture3D *m_pGIAccumGreen = NULL;
	IGXTexture3D *m_pGIAccumBlue = NULL;

	IGXRenderBuffer *m_pGICubesRB = NULL;
	UINT m_uGICubesCount = 0;
	ID m_idGICubesShader = -1;

	ID m_idLightBoundShader = -1;
};

#endif
