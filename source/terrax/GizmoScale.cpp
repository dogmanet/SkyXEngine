#include "GizmoScale.h"
#include "Editor.h"

CGizmoScale::CGizmoScale(CEditor *pEditor):
	m_pEditor(pEditor)
{
}
CGizmoScale::~CGizmoScale()
{
	m_pEditor->onGizmoRemoved(this);
}

void XMETHODCALLTYPE CGizmoScale::setPos(const float3_t &vPos)
{
	m_vPos = vPos;
}
const float3_t& XMETHODCALLTYPE CGizmoScale::getPos()
{
	return(m_vPos);
}

void CGizmoScale::draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D)
{
}


