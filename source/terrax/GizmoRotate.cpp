#include "GizmoRotate.h"
#include "Editor.h"

CGizmoRotate::CGizmoRotate(CEditor *pEditor):
	m_pEditor(pEditor)
{
}
CGizmoRotate::~CGizmoRotate()
{
	m_pEditor->onGizmoRemoved(this);
}

void XMETHODCALLTYPE CGizmoRotate::setPos(const float3_t &vPos)
{
	m_vPos = vPos;
}
const float3_t& XMETHODCALLTYPE CGizmoRotate::getPos()
{
	return(m_vPos);
}

void CGizmoRotate::draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D)
{
}
