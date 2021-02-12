#include "GizmoRadius.h"
#include "Editor.h"

CGizmoRadius::CGizmoRadius(CEditor *pEditor):
	m_pEditor(pEditor)
{
	m_pEditor->setDirty();
}
CGizmoRadius::~CGizmoRadius()
{
	m_pEditor->onGizmoRemoved(this);

	m_pEditor->setDirty();
}

void XMETHODCALLTYPE CGizmoRadius::setPos(const float3_t &vPos)
{
	m_vPos = vPos;
	m_pEditor->setDirty();
}
const float3_t& XMETHODCALLTYPE CGizmoRadius::getPos()
{
	return(m_vPos);
}

void XMETHODCALLTYPE CGizmoRadius::setRadius(float fRadius)
{
	if(fRadius < m_fMinRadius)
	{
		fRadius = m_fMinRadius;
	}
	else if(fRadius > m_fMaxRadius)
	{
		fRadius = m_fMaxRadius;
	}

	m_fRadius = fRadius;

	m_pEditor->setDirty();
}
float XMETHODCALLTYPE CGizmoRadius::getRadius()
{
	return(m_fRadius);
}

void XMETHODCALLTYPE CGizmoRadius::setMinRadius(float fRadius)
{
	if(fRadius < 0.0f)
	{
		fRadius = 0.0f;
	}
	if(fRadius > m_fMaxRadius)
	{
		m_fMaxRadius = fRadius;
	}
	m_fMinRadius = fRadius;
	setRadius(m_fRadius);
}
void XMETHODCALLTYPE CGizmoRadius::setMaxRadius(float fRadius)
{
	if(fRadius < 0.0f)
	{
		fRadius = 0.0f;
	}
	if(fRadius < m_fMinRadius)
	{
		m_fMinRadius = fRadius;
	}
	m_fMaxRadius = fRadius;
	setRadius(m_fRadius);
}

void CGizmoRadius::draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D)
{
	pGRBoth->setColor(float4(1.0f, 1.0f, 0.0f, 0.1f));
	pGRBoth->setLineWidth(2.0f);
	pGRBoth->drawEllipsoid(m_vPos, float3(m_fRadius));

	float3 fHandleDist = m_fRadius;

	pGR2D->setPointMode(XGPM_SQUARE);
	pGR2D->setColor(float4(1.0f));
	pGR2D->setPointSize(10.0f);
	pGR2D->drawPoint(m_vPos + float3(0.0f, 0.0f, 1.0f) * fHandleDist);
	pGR2D->drawPoint(m_vPos + float3(0.0f, 0.0f, -1.0f) * fHandleDist);
	pGR2D->drawPoint(m_vPos + float3(0.0f, 1.0f, 0.0f) * fHandleDist);
	pGR2D->drawPoint(m_vPos + float3(0.0f, -1.0f, 0.0f) * fHandleDist);
	pGR2D->drawPoint(m_vPos + float3(1.0f, 0.0f, 0.0f) * fHandleDist);
	pGR2D->drawPoint(m_vPos + float3(-1.0f, 0.0f, 0.0f) * fHandleDist);
}
