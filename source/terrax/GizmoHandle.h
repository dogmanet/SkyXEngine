#ifndef __GIZMO_HANDLE_H
#define __GIZMO_HANDLE_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CEditor;
class CGizmoHandle: public IXUnknownImplementation<IXEditorGizmoHandle>
{
public:
	CGizmoHandle(CEditor *pEditor);
	~CGizmoHandle();

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void XMETHODCALLTYPE lockInPlane(const float3_t &vPlaneNormal) override;
	void XMETHODCALLTYPE lockInDir(const float3_t &vDir) override;
	void XMETHODCALLTYPE unLock(const float3_t &vDir) override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);

private:
	CEditor *m_pEditor;

	float3_t m_vPos;
};

#endif
