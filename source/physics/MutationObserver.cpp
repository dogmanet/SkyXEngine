
#include <xcommon/resource/IXModel.h>
#include "MutationObserver.h"

CMutationObserver::CMutationObserver(IXCore *pCore):
	m_pCore(pCore)
{
	m_pModelChangedEventListener = new CModelChangedEventListener(this);
	pCore->getEventChannel<XEventModelChanged>(EVENT_MODEL_CHANGED_GUID)->addListener(m_pModelChangedEventListener);
}
CMutationObserver::~CMutationObserver()
{
	m_pCore->getEventChannel<XEventModelChanged>(EVENT_MODEL_CHANGED_GUID)->removeListener(m_pModelChangedEventListener);
	mem_delete(m_pModelChangedEventListener);
}

float3 XMETHODCALLTYPE CMutationObserver::getPosition()
{
	return(m_vPosition);
}
void XMETHODCALLTYPE CMutationObserver::setPosition(const float3 &vPosition)
{
	m_vPosition = vPosition;
}

SMQuaternion XMETHODCALLTYPE CMutationObserver::getRotation()
{
	return(m_qRotation);
}
void XMETHODCALLTYPE CMutationObserver::setRotation(const SMQuaternion &qRotation)
{
	m_qRotation = qRotation;
}

void XMETHODCALLTYPE CMutationObserver::setEnabled(bool isEnabled)
{
	m_isEnabled = isEnabled;
	reset();
}
bool XMETHODCALLTYPE CMutationObserver::isEnabled()
{
	return(m_isEnabled);
}

void XMETHODCALLTYPE CMutationObserver::setFlags(XOBSERVER_FLAGS flags)
{
}
XOBSERVER_FLAGS XMETHODCALLTYPE CMutationObserver::getFlags()
{
	return((XOBSERVER_FLAGS)0);
}

void XMETHODCALLTYPE CMutationObserver::reset()
{
	m_wasTriggered = false;
}
bool XMETHODCALLTYPE CMutationObserver::wasTriggered()
{
	return(m_wasTriggered);
}

void XMETHODCALLTYPE CMutationObserver::setHalfExtents(const float3 &vHalfExtents)
{
	m_vHalfExtents = vHalfExtents;
}

void CMutationObserver::onModelChanged(const XEventModelChanged *pEvent)
{
	float3 vPos = pEvent->pModel->getPosition();
	if(SMAABBIntersectAABB(SMAABB(m_vPosition - m_vHalfExtents, m_vPosition + m_vHalfExtents), pEvent->pModel->getLocalBound() + vPos))
	{
		m_wasTriggered = true;
	}
}


//##########################################################################


CModelChangedEventListener::CModelChangedEventListener(CMutationObserver *pObserver):
m_pObserver(pObserver)
{}
void CModelChangedEventListener::onEvent(const XEventModelChanged *pData)
{
	m_pObserver->onModelChanged(pData);
}
