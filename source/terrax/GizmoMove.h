#ifndef __GIZMO_MOVE_H
#define __GIZMO_MOVE_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/render/IXRenderUtils.h>
//#include "terrax.h"

class CGizmoMove;
class CMoveHandleCallback: public IXEditorGizmoHandleCallback
{
public:
	CMoveHandleCallback(CGizmoMove *pMove);
	void XMETHODCALLTYPE moveTo(const float3 &vNewPos, IXEditorGizmoHandle *pGizmo) override;

private:
	CGizmoMove *m_pGizmo;
};

class CEditor;
class CGizmoHandle;
class CGizmoMove final: public IXUnknownImplementation<IXEditorGizmoMove>
{
public:
	CGizmoMove(CEditor *pEditor);
	~CGizmoMove();

	void XMETHODCALLTYPE setPos(const float3_t &vPos) override;
	const float3_t& XMETHODCALLTYPE getPos() override;

	void draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D);
	
	bool wantHandle(const float3 &vRayOrigin, const float3 &vRayDir);

	void setWorldRay(const float3 &vRayOrigin, const float3 &vRayDir);
	void setTracking(bool isTracking);

	void onHandlerMove(const float3 &vNewPos);

	void XMETHODCALLTYPE setCallback(IXEditorGizmoMoveCallback *pCallback) override
	{
		m_pCallback = pCallback;
	};

private:
	bool intersectMove(const float3 &vRayOrigin, const float3 &vRayDir);

private:
	CEditor *m_pEditor;

	CGizmoHandle *m_pHandle;

	CMoveHandleCallback m_handleCallback;

	IXEditorGizmoMoveCallback *m_pCallback = NULL;

	float3_t m_vPos;

	bool m_isTracking = false;
	bool m_isFirstMov = false;
	float3_t m_vMovDelta;

	float m_fFixedScale = -1.0f;

	struct vert
	{
		float3_t pos;
		DWORD color;
	};

	enum HANDLER_AXE
	{
		HANDLER_AXE_NONE = 0x00,
		HANDLER_AXE_X = 0x01,
		HANDLER_AXE_Y = 0x02,
		HANDLER_AXE_XY = 0x03,
		HANDLER_AXE_Z = 0x04,
		HANDLER_AXE_XZ = 0x05,
		HANDLER_AXE_YZ = 0x06,
		HANDLER_AXE_XYZ = 0x07
	};

	HANDLER_AXE m_currentAxe = HANDLER_AXE_NONE;
	HANDLER_AXE m_lastAxe = HANDLER_AXE_NONE;
};

#endif
