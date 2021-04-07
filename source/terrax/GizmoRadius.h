#ifndef __GIZMO_RADIUS_H
#define __GIZMO_RADIUS_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CGizmoRadius;
class CHandleCallback final: public IXEditorGizmoHandleCallback
{
public:
	CHandleCallback(CGizmoRadius *pGizmo);
	void XMETHODCALLTYPE moveTo(const float3 &vNewPos, IXEditorGizmoHandle *pHandle) override;

private:
	CGizmoRadius *m_pGizmo;
};

class CEditor;
class CGizmoRadius final: public IXUnknownImplementation<IXEditorGizmoRadius>
{
	friend class CHandleCallback;
public:
	CGizmoRadius(CEditor *pEditor);
	~CGizmoRadius();

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void XMETHODCALLTYPE setRadius(float fRadius) override;
	void setRadius(float fRadius, bool runCallback);
	float XMETHODCALLTYPE getRadius() override;

	void XMETHODCALLTYPE setMinRadius(float fRadius) override;
	void XMETHODCALLTYPE setMaxRadius(float fRadius) override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);

	void XMETHODCALLTYPE setCallback(IXEditorGizmoRadiusCallback *pCallback) override
	{
		m_pCallback = pCallback;
	}
private:
	CEditor *m_pEditor;

	float3_t m_vPos;

	float m_fRadius = 0.0f;
	float m_fMinRadius = 0.0f;
	float m_fMaxRadius = FLT_MAX;

	IXEditorGizmoHandle *m_apHandles[6];

	CHandleCallback m_handleCallback;

	IXEditorGizmoRadiusCallback *m_pCallback = NULL;
};

#endif
