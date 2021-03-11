#include "GizmoHandle.h"
#include "Editor.h"

CGizmoHandle::CGizmoHandle(CEditor *pEditor):
	m_pEditor(pEditor)
{
	m_pEditor->setDirty();
}
CGizmoHandle::~CGizmoHandle()
{
	m_pEditor->onGizmoRemoved(this);

	m_pEditor->setDirty();
}

void XMETHODCALLTYPE CGizmoHandle::setPos(const float3_t &vPos)
{
	m_vPos = vPos;

	m_pEditor->setDirty();
}
const float3_t& XMETHODCALLTYPE CGizmoHandle::getPos()
{
	return(m_vPos);
}

void XMETHODCALLTYPE CGizmoHandle::lockInPlane(const float3_t &vPlaneNormal)
{
}
void XMETHODCALLTYPE CGizmoHandle::lockInDir(const float3_t &vDir)
{
}
void XMETHODCALLTYPE CGizmoHandle::unLock(const float3_t &vDir)
{
}

void CGizmoHandle::draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D)
{
	pGRBoth->setPointMode(XGPM_SQUARE);
	pGRBoth->setColor(float4(1.0f));
	pGRBoth->setPointSize(getOnscreenSize());
	pGRBoth->drawPoint(m_vPos);

	drawGrid(pGR3D);
}

void CGizmoHandle::setWorldRay(const float3 &vRayOrigin, const float3 &vRayDir)
{
	float3 vDir = vRayDir * 1000.0f;
	float3 vPoint;
	if(m_planeBase.intersectLine(&vPoint, vRayOrigin - vDir, vRayOrigin + vDir))
	{
		setPos(vPoint);
	}
}
void CGizmoHandle::setTracking(bool isTracking)
{
	m_isTracking = isTracking;

	if(isTracking)
	{
		m_planeBase = SMPLANE(m_vBestPlaneNormal, m_vPos);


	}

	m_pEditor->setDirty();
}

void CGizmoHandle::drawGrid(IXGizmoRenderer *pGR3D)
{
	if(!m_isTracking)
	{
		return;
	}

	float3 vT, vB, vN = m_vBestPlaneNormal;
	if(fabsf(SMVector3Dot(vN, float3(0.0f, 1.0f, 0.0f))) > 0.9f)
	{
		vT = float3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		vT = float3(0.0f, 1.0f, 0.0f);
	}
	vB = SMVector3Cross(vT, vN);
	vT = SMVector3Cross(vN, vB);

	pGR3D->jumpTo(m_vPos);
	pGR3D->lineTo(m_vPos + vT);
	pGR3D->jumpTo(m_vPos);
	pGR3D->lineTo(m_vPos + vB);
	pGR3D->jumpTo(m_vPos);
	pGR3D->lineTo(m_vPos - vB);
	pGR3D->jumpTo(m_vPos);
	pGR3D->lineTo(m_vPos - vT);
}
