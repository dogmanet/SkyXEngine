#include "GizmoRadius.h"
#include "Editor.h"

CGizmoRadius::CGizmoRadius(CEditor *pEditor):
	m_pEditor(pEditor),
	m_handleCallback(this)
{
	m_pEditor->setDirty();

	for(UINT i = 0; i < ARRAYSIZE(m_apHandles); ++i)
	{
		pEditor->newGizmoHandle(&m_apHandles[i]);
		m_apHandles[i]->setCallback(&m_handleCallback);
	}

	m_apHandles[0]->lockInDir(float3(0.0f, 0.0f,  1.0f));
	m_apHandles[1]->lockInDir(float3(0.0f, 0.0f, -1.0f));
	m_apHandles[2]->lockInDir(float3(0.0f, 1.0f,  0.0f));
	m_apHandles[3]->lockInDir(float3(0.0f, -1.0f, 0.0f));
	m_apHandles[4]->lockInDir(float3(1.0f, 0.0f,  0.0f));
	m_apHandles[5]->lockInDir(float3(-1.0f, 0.0f, 0.0f));

	setRadius(1.0f);
}
CGizmoRadius::~CGizmoRadius()
{
	for(UINT i = 0; i < ARRAYSIZE(m_apHandles); ++i)
	{
		mem_release(m_apHandles[i]);
	}

	m_pEditor->onGizmoRemoved(this);

	m_pEditor->setDirty();
}

void XMETHODCALLTYPE CGizmoRadius::enable(bool yesNo)
{
	if(m_isEnabled != yesNo)
	{
		m_isEnabled = yesNo;
		m_pEditor->setDirty();

		for(UINT i = 0; i < ARRAYSIZE(m_apHandles); ++i)
		{
			m_apHandles[i]->enable(yesNo);
		}
	}
}

void XMETHODCALLTYPE CGizmoRadius::setPos(const float3_t &vPos)
{
	m_vPos = vPos;

	setRadius(m_fRadius);

	m_pEditor->setDirty();
}
const float3_t& XMETHODCALLTYPE CGizmoRadius::getPos()
{
	return(m_vPos);
}

void CGizmoRadius::setRadius(float fRadius, bool runCallback)
{
	if(fRadius < m_fMinRadius)
	{
		fRadius = m_fMinRadius;
	}
	else if(fRadius > m_fMaxRadius)
	{
		fRadius = m_fMaxRadius;
	}

	if(runCallback)
	{
		SAFE_CALL(m_pCallback, onChange, fRadius, this)
		else
		{
			setRadius(fRadius);
		}
		return;
	}

	m_fRadius = fRadius;

	float3 fHandleDist = m_fRadius;

	m_apHandles[0]->setPos((float3)(m_vPos + float3(0.0f, 0.0f, 1.0f) * fHandleDist));
	m_apHandles[1]->setPos((float3)(m_vPos + float3(0.0f, 0.0f, -1.0f) * fHandleDist));
	m_apHandles[2]->setPos((float3)(m_vPos + float3(0.0f, 1.0f, 0.0f) * fHandleDist));
	m_apHandles[3]->setPos((float3)(m_vPos + float3(0.0f, -1.0f, 0.0f) * fHandleDist));
	m_apHandles[4]->setPos((float3)(m_vPos + float3(1.0f, 0.0f, 0.0f) * fHandleDist));
	m_apHandles[5]->setPos((float3)(m_vPos + float3(-1.0f, 0.0f, 0.0f) * fHandleDist));

	m_pEditor->setDirty();
}

void XMETHODCALLTYPE CGizmoRadius::setRadius(float fRadius)
{
	setRadius(fRadius, false);
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
}

//##########################################################################

CHandleCallback::CHandleCallback(CGizmoRadius *pGizmo):
	m_pGizmo(pGizmo)
{
}
void XMETHODCALLTYPE CHandleCallback::moveTo(const float3 &vNewPos, IXEditorGizmoHandle *pHandle)
{
	m_pGizmo->setRadius(SMVector3Length(m_pGizmo->m_vPos - vNewPos), true);
}

void XMETHODCALLTYPE CHandleCallback::onStart(IXEditorGizmoHandle *pHandle)
{
	SAFE_CALL(m_pGizmo->m_pCallback, onStart, m_pGizmo);
}
void XMETHODCALLTYPE CHandleCallback::onEnd(IXEditorGizmoHandle *pHandle)
{
	SAFE_CALL(m_pGizmo->m_pCallback, onEnd, m_pGizmo);
}
