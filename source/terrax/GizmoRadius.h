#ifndef __GIZMO_RADIUS_H
#define __GIZMO_RADIUS_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CEditor;
class CGizmoRadius: public IXUnknownImplementation<IXEditorGizmoRadius>
{
public:
	CGizmoRadius(CEditor *pEditor);
	~CGizmoRadius();

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void XMETHODCALLTYPE setRadius(float fRadius) override;
	float XMETHODCALLTYPE getRadius() override;

	void XMETHODCALLTYPE setMinRadius(float fRadius) override;
	void XMETHODCALLTYPE setMaxRadius(float fRadius) override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);

private:
	CEditor *m_pEditor;

	float3_t m_vPos;

	float m_fRadius = 1.0f;
	float m_fMinRadius = 0.0f;
	float m_fMaxRadius = FLT_MAX;

};

#endif
