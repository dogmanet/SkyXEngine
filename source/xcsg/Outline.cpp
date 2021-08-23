#include "Outline.h"

COutline::COutline(IXEditor *pEditor, IXGizmoRenderer *pRenderer):
	m_pEditor(pEditor),
	m_pRenderer(pRenderer),
	m_callBack(this)
{
	
}

COutline::~COutline()
{
	for(UINT i = 0, l = m_aPoints.size(); i < l; ++i)
	{
		mem_release(m_aPoints[i].pHandle);
	}
}

void COutline::addPoint()
{
	//if(isValid())
	if(!m_isClosed)
	{
		m_isDirty = true;
		IXEditorGizmoHandle *pHandle;
		m_pEditor->newGizmoHandle(&pHandle);
		pHandle->setPos(m_vNewPoint);
		pHandle->lockInPlane(m_vNormal);
		pHandle->setCallback(&m_callBack);
		//pHandle->setCallback();

		m_aPoints.push_back({m_vNewPoint, false, pHandle});
	}
}

void COutline::setPlane(const float3_t &vNormal)
{
	m_vNormal = vNormal;
}

void COutline::closePath()
{
	m_isDirty = true;
	m_isClosed = true;
}

void COutline::render(bool is3D)
{
	if(m_isDirty)
	{
		m_pRenderer->reset();

		if(m_aPoints.size())
		{
			m_pRenderer->setColor(isValid() ? float4_t(0.6f, 0.8f, 1.0f, 1.0f) : float4_t(1.0f, 0.2f, 0.2f, 1.0f));
			m_pRenderer->jumpTo(m_aPoints[0].vPos);
			for(UINT i = 1, l = m_aPoints.size(); i < l; ++i)
			{
				m_pRenderer->lineTo(m_aPoints[i].vPos);
			}

			if(!m_isClosed)
			{
				m_pRenderer->setColor(isValid() ? float4_t(1.0f, 1.0f, 0.0f, 0.8f) : float4_t(1.0f, 0.2f, 0.2f, 0.8f));

				m_pRenderer->lineTo(m_vNewPoint);
			}

			m_pRenderer->lineTo(m_aPoints[0].vPos);
		}

		m_isDirty = false;
	}

	m_pRenderer->render(!is3D);
}

static bool SegmentIntersectSegment(const float3 &n, const float3 &a0, const float3 &a1, const float3 &b0, const float3 &b1)
{
	SMPLANE p0(SMVector3Cross(a0 - a1, n), a0);
	bool f0 = SMVector4Dot(p0, float4(b0, 1.0f)) > 0.0f;
	bool f1 = SMVector4Dot(p0, float4(b1, 1.0f)) > 0.0f;
	if(f0 == f1)
	{
		return(false);
	}

	SMPLANE p1(SMVector3Cross(b0 - b1, n), b0);
	f0 = SMVector4Dot(p1, float4(a0, 1.0f)) > 0.0f;
	f1 = SMVector4Dot(p1, float4(a1, 1.0f)) > 0.0f;
	return(f0 != f1);
}

bool COutline::isValid()
{
	// Committed outlines are always valid
	if(m_isClosed || m_aPoints.size() <= 2)
	{
		return(true);
	}

	float3 va0 = m_vNewPoint;
	float3 va1 = m_aPoints[0].vPos;

	float3 vb0, vb1;
	for(UINT i = 2, l = m_aPoints.size(); i < l; ++i)
	{
		vb0 = m_aPoints[i - 1].vPos;
		vb1 = m_aPoints[i].vPos;
		if(SegmentIntersectSegment(m_vNormal, va0, va1, vb0, vb1))
		{
			return(false);
		}
	}

	va1 = m_aPoints[m_aPoints.size() - 1].vPos;
	for(UINT i = 0, l = m_aPoints.size() - 1; i < l; ++i)
	{
		vb0 = m_aPoints[i].vPos;
		vb1 = m_aPoints[i + 1].vPos;
		if(SegmentIntersectSegment(m_vNormal, va0, va1, vb0, vb1))
		{
			return(false);
		}
	}

	return(true);
}

void COutline::activate()
{
	for(UINT i = 0, l = m_aPoints.size(); i < l; ++i)
	{
		m_aPoints[i].pHandle->enable(true);
	}
}

void COutline::deactivate()
{
	for(UINT i = 0, l = m_aPoints.size(); i < l; ++i)
	{
		m_aPoints[i].pHandle->enable(false);
	}
}

void COutline::onHandleClick(IXEditorGizmoHandle *pHandle)
{
	int idx = m_aPoints.indexOf(pHandle, [](const PathPoint &p, IXEditorGizmoHandle *h){
		return(p.pHandle == h);
	});
	assert(idx >= 0);

	if(!m_isClosed && idx == 0)
	{
		closePath();
	}

	bool isCtrl = m_pEditor->getKeyState(SIK_CONTROL);
	
	if(isCtrl)
	{
		m_aPoints[idx].isSelected = !m_aPoints[idx].isSelected;
		m_aPoints[idx].pHandle->setColor(float4_t(1.0f, 1.0f, m_aPoints[idx].isSelected ? 0.0f : 1.0f, 1.0f));
	}
	else
	{
		for(UINT i = 0, l = m_aPoints.size(); i < l; ++i)
		{
			m_aPoints[i].isSelected = false;
			m_aPoints[i].pHandle->setColor(float4_t(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_aPoints[idx].isSelected = true;
		m_aPoints[idx].pHandle->setColor(float4_t(1.0f, 1.0f, 0.0f, 1.0f));
	}


	m_isDirty = true;
}
void COutline::onHandleMove(const float3 &vNewPos, IXEditorGizmoHandle *pHandle)
{
	int idx = m_aPoints.indexOf(pHandle, [](const PathPoint &p, IXEditorGizmoHandle *h){
		return(p.pHandle == h);
	});
	assert(idx >= 0);

	float3 vDelta = vNewPos - m_aPoints[idx].vPos;

	if(!m_aPoints[idx].isSelected)
	{
		for(UINT i = 0, l = m_aPoints.size(); i < l; ++i)
		{
			m_aPoints[i].isSelected = false;
			m_aPoints[i].pHandle->setColor(float4_t(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_aPoints[idx].isSelected = true;
		m_aPoints[idx].pHandle->setColor(float4_t(1.0f, 1.0f, 0.0f, 1.0f));
	}

	m_aPoints[idx].vPos = vNewPos;
	pHandle->setPos(vNewPos);

	for(UINT i = 0, l = m_aPoints.size(); i < l; ++i)
	{
		if(i != idx && m_aPoints[i].isSelected)
		{
			m_aPoints[i].vPos = (float3)(m_aPoints[i].vPos + vDelta);
			m_aPoints[i].pHandle->setPos(m_aPoints[i].vPos);
		}
	}
	
	m_isDirty = true;
}

void COutline::setMouse(const float3_t &vPoint)
{
	if(!m_isClosed)
	{
		m_isDirty = true;
		m_vNewPoint = vPoint;
	}
}

//##########################################################################

void XMETHODCALLTYPE COutlinePointCallback::moveTo(const float3 &vNewPos, IXEditorGizmoHandle *pGizmo)
{
	if(m_isFirstMov)
	{
		m_isFirstMov = false;
	}
	else
	{
		m_isMoved = true;
		m_pOutline->onHandleMove(vNewPos, pGizmo);
	}
}
void XMETHODCALLTYPE COutlinePointCallback::onStart(IXEditorGizmoHandle *pGizmo)
{
	m_isMoved = false;
	m_isFirstMov = true;
}
void XMETHODCALLTYPE COutlinePointCallback::onEnd(IXEditorGizmoHandle *pGizmo)
{
	if(!m_isMoved)
	{
		m_pOutline->onHandleClick(pGizmo);
	}
}
