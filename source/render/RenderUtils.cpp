#include "RenderUtils.h"
#include "GizmoRenderer.h"
#include <gcore/sxgcore.h>

void XMETHODCALLTYPE CRenderUtils::newGizmoRenderer(IXGizmoRenderer **ppOut) const
{
	*ppOut = new CGizmoRenderer(SGCore_GetDXDevice());
}
