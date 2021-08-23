#ifndef __OUTLINE_H
#define __OUTLINE_H

#include <xcommon/editor/IXEditorGizmo.h>
#include <xcommon/editor/IXEditor.h>
#include <xcommon/render/IXRenderUtils.h>
//#include <xcommon/editor/IXEditable.h>
//#include <mtrl/IXMaterialSystem.h>
//#include <xcommon/IXCore.h>
#include <gdefines.h>

class COutline;
class COutlinePointCallback: public IXEditorGizmoHandleCallback
{
public:
	COutlinePointCallback(COutline *pOutline):
		m_pOutline(pOutline)
	{
	}

	void XMETHODCALLTYPE moveTo(const float3 &vNewPos, IXEditorGizmoHandle *pGizmo) override;
	void XMETHODCALLTYPE onStart(IXEditorGizmoHandle *pGizmo) override;
	void XMETHODCALLTYPE onEnd(IXEditorGizmoHandle *pGizmo) override;

private:
	COutline *m_pOutline;
	bool m_isMoved = false;
	bool m_isFirstMov = false;
};

//##########################################################################

class COutline
{
public:
	COutline(IXEditor *pEditor, IXGizmoRenderer *pRenderer);
	~COutline();

	void addPoint();

	void setMouse(const float3_t &vPoint);

	void setPlane(const float3_t &vNormal);

	void render(bool is3D);

	void activate();
	void deactivate();

	void onHandleClick(IXEditorGizmoHandle *pHandle);
	void onHandleMove(const float3 &vNewPos, IXEditorGizmoHandle *pHandle);

private:
	void closePath();

	bool isValid();

private:
	
	struct PathPoint
	{
		float3_t vPos;
		bool isSelected;
		IXEditorGizmoHandle *pHandle;
	};

	IXEditor *m_pEditor = NULL;
	IXGizmoRenderer *m_pRenderer = NULL;

	Array<PathPoint> m_aPoints;

	float3_t m_vNormal;

	float3_t m_vNewPoint;

	bool m_isClosed = false;
	bool m_isDirty = false;

	COutlinePointCallback m_callBack;
};

#endif
