#include "GizmoMove.h"
#include "Editor.h"

CGizmoMove::CGizmoMove(CEditor *pEditor):
	m_pEditor(pEditor)
{
}
CGizmoMove::~CGizmoMove()
{
	m_pEditor->onGizmoRemoved(this);
}

void XMETHODCALLTYPE CGizmoMove::setPos(const float3_t &vPos)
{
	m_vPos = vPos;
}
const float3_t& XMETHODCALLTYPE CGizmoMove::getPos()
{
	return(m_vPos);
}

void CGizmoMove::draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D)
{
}

