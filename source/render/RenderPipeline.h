#ifndef __RENDERPIPELINE_H
#define __RENDERPIPELINE_H

#include <xcommon/IXRenderable.h>
#include <xcommon/IXRenderPipeline.h>
#include <common/array.h>
#include <light/IXLightSystem.h>
#include "ShadowCache.h"
#include <xUI/IXUI.h>
#include "OcclusionCuller.h"

#define MAX_TRANSPARENCY_CLIP_PANES 4
#define LPV_CASCADES_COUNT 3
#define LPV_GRID_SIZE 32
#define LPV_STEP_COUNT 6

class CRenderPipeline: public IXRenderPipeline
{
public:
	CRenderPipeline(IGXDevice *pDevice);
	~CRenderPipeline();

	void resize(UINT uWidth, UINT uHeight, bool isWindowed = true) override;

	void updateVisibility() override;

	void renderFrame() override;
	void endFrame() override;

	SX_ALIGNED_OP_MEM2();

	void renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility = NULL) override;

	IGXDevice *getDevice() override;

	void newVisData(IXRenderableVisibility **ppVisibility) override;

protected:

	void renderPrepare() override;
	void renderGBuffer() override;
	void renderShadows() override;
	void renderGI() override;
	void renderPostprocessMain() override;
	void renderTransparent() override;
	void renderPostprocessFinal() override;
	void renderEditor2D() override;

	void showGICubes();

protected:
	UINT getIndexForStage(X_RENDER_STAGE stage);

	void showTexture(IGXTexture2D *pTexture);
	void showFrameStats();

	IXUI* getXUI();

	struct _render_sys
	{
		IXRenderable *pRenderable;
		UINT uPriority;
		UINT uRenderableId;
	};

	struct _render_stage
	{
		Array<_render_sys> aSystems; // sorted list of render systems
	};

	Array<_render_stage> m_apRenderStages;
	Array<IXRenderable*> m_apRenderables;
	IGXDevice *m_pDevice;
	IGXSwapChain *m_pSwapChain = NULL;

	bool m_isWindowed = true;
	UINT m_uOutWidth;
	UINT m_uOutHeight;

	IXRenderableVisibility *m_pMainCameraVisibility = NULL;
	COcclusionCuller *m_pMainCameraOcclusionCuller = NULL;

	IXLightSystem *m_pLightSystem = NULL;

	IXUI *m_pXUI = NULL;

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

	//! Буфер тени
	IGXTexture2D *m_pShadow = NULL;

	IGXDepthStencilState *m_pDepthStencilStateDefault = NULL;
	IGXDepthStencilState *m_pDepthStencilStateNoZWrite = NULL;
	IGXDepthStencilState *m_pDepthStencilStateNoZ = NULL;
	IGXBlendState *m_pBlendStateAlpha = NULL;

	IXMaterialSystem *m_pMaterialSystem = NULL;

	struct
	{
		/*struct
		{
			SMMATRIX mP;
			float4 vTimeDelta;
			float4 vWinSize;
		} vs;*/
		float4 vNearFarInvWinSize;
	} m_sceneShaderData;
	//IGXConstantBuffer *m_pSceneShaderDataVS = NULL;
	IGXConstantBuffer *m_pSceneShaderDataPS = NULL;

	struct
	{
		struct
		{
			//SMMATRIX mV;
			SMMATRIX mVP;
			float3 vPosCam;
			SMMATRIX mInvVP;
		} vs;
		//float4 vNearFarLayers;
	} m_cameraShaderData;
	IGXConstantBuffer *m_pCameraShaderDataVS = NULL;
	//IGXConstantBuffer *m_pCameraShaderDataPS = NULL;

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

	struct XTransparentNodeSP
	{
		int iPSP = -1;
		XTransparentNodeSP *pNext = NULL;
		float fCamDist = -1.0f;
	};

	struct XTransparentNode
	{
		XTransparentObject obj;
		UINT uRenderable;
		UINT uObjectID;
		IXRenderableVisibility *pVisibility;
		// split plane list
		UINT uSplitPlanes = 0;
		XTransparentNodeSP *pSplitPlanes = NULL;
	};

	struct XTransparentPSP
	{
		SMPLANE psp;
		UINT uNode;
		bool useMe = false;
		int iBasePSP = -1; // номер похожей плоскости

		bool isRenderFront = false;
	};

	struct XTransparentBSPObject
	{
		XTransparentBSPObject *pNext = NULL;
		float fCamDist;
		UINT uNode;
	};
	struct XTransparentBSPNode
	{
		int iPSP = -1;
		XTransparentBSPObject *pObjects = NULL;
		XTransparentBSPNode *pBack = NULL;
		XTransparentBSPNode *pFront = NULL;
	};

	void buildTransparencyBSP(XTransparentPSP *pPSPs, UINT uPSPcount, UINT uStartPSP, XTransparentBSPNode *pNode, XTransparentBSPObject *pObjects, XTransparentNode *pObjectNodes, const float3 &vCamPos);
	void renderTransparencyBSP(XTransparentBSPNode *pNode, XTransparentPSP *pPSPs, XTransparentNode *pObjectNodes, _render_sys *pRenderSystems, const float3 &vCamPos, UINT *puPlanesStack, UINT uPlaneCount, const SMMATRIX &mTransInvVP);
	MemAlloc<XTransparentBSPNode> m_poolTransparencyBSPnodes;
	MemAlloc<XTransparentBSPObject> m_poolTransparencyBSPobjects;
	MemAlloc<XTransparentNodeSP> m_poolTransparencyBSPsplitPlanes;
	Array<XTransparentNode> m_aTmpNodes;
	Array<XTransparentPSP> m_aTmpPSPs;
	IGXConstantBuffer *m_pTransparencyShaderClipPlanes = NULL;
	IGXSamplerState *m_pRefractionScene = NULL;

	IGXTexture2D *m_pRefractiveTextures[2];
	int m_iRefractiveSource = -1;
	IGXTexture2D *m_pRefractiveTextureRead = NULL;
	IGXTexture2D *m_pRefractiveTextureWrite = NULL;

	IGXTexture2D *m_pSceneTexture = NULL;

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

	IGXRenderBuffer *m_pGICubesRB = NULL;
	UINT m_uGICubesCount = 0;
	ID m_idGICubesShader = -1;

	ID m_idLightBoundShader = -1;
	ID m_idLPVPropagateShader = -1;

	//###################################

	XRenderPassHandler *m_pRenderPassGBuffer = NULL;
	XRenderPassHandler *m_pRenderPassTransparency = NULL;
	XRenderPassHandler *m_pRenderPassIllumination = NULL;
	XRenderPassHandler *m_pRenderPassPostprocess = NULL;
	XRenderPassHandler *m_pRenderPassShadow = NULL;
};

#endif
