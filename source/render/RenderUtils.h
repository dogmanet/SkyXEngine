#ifndef __RENDERUTILS_H
#define __RENDERUTILS_H

#include <xcommon/render/IXRenderUtils.h>

class CRenderUtils final: public IXUnknownImplementation<IXRenderUtils>
{
public:
	void XMETHODCALLTYPE newGizmoRenderer(IXGizmoRenderer **ppOut) const override;
};

#endif
