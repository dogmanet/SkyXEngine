#ifndef __RENDERUTILS_H
#define __RENDERUTILS_H

#include <xcommon/render/IXRenderUtils.h>

class CRenderUtils final: public IXUnknownImplementation<IXRenderUtils>
{
public:
	~CRenderUtils();

	void XMETHODCALLTYPE newGizmoRenderer(IXGizmoRenderer **ppOut) override;
	void XMETHODCALLTYPE newLineRenderer(IXLineRenderer **ppOut) override;
	void XMETHODCALLTYPE getQuadIndexBuffer(UINT uQuads, IGXIndexBuffer **ppIndexBuffer) override;

private:
	UINT m_uQuadIBSize = 0;
	IGXIndexBuffer *m_pQuadIB = NULL;
};

#endif
