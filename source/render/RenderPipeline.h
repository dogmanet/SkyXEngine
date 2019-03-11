#ifndef __RENDERPIPELINE_H
#define __RENDERPIPELINE_H

#include <xcommon/IXRenderable.h>
#include <xcommon/IXRenderPipeline.h>
#include <common/array.h>

class CRenderPipeline: public IXRenderPipeline
{
public:
	CRenderPipeline(IGXContext *pDevice);
	~CRenderPipeline();

	void resize(UINT uWidth, UINT uHeight, bool isWindowed = true);

	void renderFrame();
	void endFrame();

	SX_ALIGNED_OP_MEM2();

protected:
	void renderStage(X_RENDER_STAGE stage);

	void renderPrepare();
	void renderGBuffer();
	void renderShadows();
	void renderGI();
	void renderPostprocessMain();
	void renderTransparent();
	void renderPostprocessFinal();
	void renderEditor2D();

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

	IGXTexture2D *m_pGBufferColor = NULL;
	IGXTexture2D *m_pGBufferNormals = NULL;
	IGXTexture2D *m_pGBufferParams = NULL;
	IGXTexture2D *m_pGBufferDepth = NULL;

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
	}
	m_sceneShaderData;
	IGXConstantBuffer *m_pSceneShaderDataVS = NULL;
	IGXConstantBuffer *m_pSceneShaderDataPS = NULL;
};

#endif
