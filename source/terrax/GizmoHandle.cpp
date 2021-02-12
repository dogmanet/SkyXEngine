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
	pGRBoth->setPointSize(10.0f);
	pGRBoth->drawPoint(m_vPos);
}
