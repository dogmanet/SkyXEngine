#ifndef __GIZMO_ROTATE_H
#define __GIZMO_ROTATE_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CEditor;
class CGizmoRotate final: public IXUnknownImplementation<IXEditorGizmoRotate>
{
public:
	CGizmoRotate(CEditor *pEditor);
	~CGizmoRotate();

	void XMETHODCALLTYPE enable(bool yesNo) override;
	bool XMETHODCALLTYPE isEnabled() override
	{
		return(m_isEnabled);
	}

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);

	bool wantHandle(const float3 &vRayOrigin, const float3 &vRayDir);

	void setWorldRay(const float3 &vRayOrigin, const float3 &vRayDir);
	void setTracking(bool isTracking);

	void XMETHODCALLTYPE setDeltaAngle(float fAngle) override;
	float XMETHODCALLTYPE getDeltaAngle() override
	{
		return(m_fCurrentRotation);
	}

	void XMETHODCALLTYPE setOrient(const SMQuaternion &q) override
	{
		m_qBaseRotation = q;
	}
	SMQuaternion XMETHODCALLTYPE getOrient() override;

	void XMETHODCALLTYPE setCallback(IXEditorGizmoRotateCallback *pCallback) override
	{
		m_pCallback = pCallback;
	}

private:
	void drawArc(IXGizmoRenderer *pGR3D, const float3 &vCenter, const float3 &vCamDir, const float3 &vDir, float fRadius);

	bool intersectMove(const float3 &vStart, const float3 &vRayDir);

private:
	CEditor *m_pEditor;
	bool m_isEnabled = true;

	float3_t m_vPos;

	SMQuaternion m_qBaseRotation;

	float3_t m_vLockAxis;
	bool m_isAxisLocked = false;

	float3_t m_vBasePoint;
	float3_t m_vLastPoint;

	bool m_isTracking = false;
	float3_t m_vCurDir;

	float m_fCurrentRotation = 0.0f;

	enum HANDLER_AXE
	{
		HANDLER_AXE_NONE = 0,
		HANDLER_AXE_X = 1,
		HANDLER_AXE_Y = 2,
		HANDLER_AXE_Z = 3,
		HANDLER_AXE_CUSTOM = 4
	};

	HANDLER_AXE m_currentAxe = HANDLER_AXE_NONE;
	HANDLER_AXE m_lastAxe = HANDLER_AXE_NONE;
	
	IXEditorGizmoRotateCallback *m_pCallback = NULL;
};

#endif
