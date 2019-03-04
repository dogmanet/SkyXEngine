#ifndef __RENDERPIPELINE_H
#define __RENDERPIPELINE_H

#include <xcommon/IXRenderable.h>
#include <common/array.h>

class CRenderPipeline
{
public:
	CRenderPipeline(IGXContext *pDevice);
	~CRenderPipeline();

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
};

#endif
