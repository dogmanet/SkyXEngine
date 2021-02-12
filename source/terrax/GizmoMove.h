#ifndef __GIZMO_MOVE_H
#define __GIZMO_MOVE_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CEditor;
class CGizmoMove: public IXUnknownImplementation<IXEditorGizmoMove>
{
public:
	CGizmoMove(CEditor *pEditor);
	~CGizmoMove();

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);

private:
	CEditor *m_pEditor;

	float3_t m_vPos;


};

#endif
