#ifndef __GIZMO_ROTATE_H
#define __GIZMO_ROTATE_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CEditor;
class CGizmoRotate: public IXUnknownImplementation<IXEditorGizmoRotate>
{
public:
	CGizmoRotate(CEditor *pEditor);
	~CGizmoRotate();

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);

private:
	CEditor *m_pEditor;

	float3_t m_vPos;


};

#endif
