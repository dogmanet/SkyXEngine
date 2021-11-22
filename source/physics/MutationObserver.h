#ifndef __MUTATIONOBSERVER_H
#define __MUTATIONOBSERVER_H

#include <xcommon/physics/IXMutationObserver.h>
#include <xcommon/XEvents.h>
#include <xcommon/IXCore.h>

class CMutationObserver;
class CModelChangedEventListener: public IEventListener<XEventModelChanged>
{
public:
	CModelChangedEventListener(CMutationObserver *pObserver);
	void onEvent(const XEventModelChanged *pData) override;

protected:
	CMutationObserver *m_pObserver;
};

class CMutationObserver: public IXUnknownImplementation<IXMutationObserver>
{
public:
	CMutationObserver(IXCore *pCore);
	~CMutationObserver();

	float3 XMETHODCALLTYPE getPosition() override;
	void XMETHODCALLTYPE setPosition(const float3 &vPosition) override;

	SMQuaternion XMETHODCALLTYPE getRotation() override;
	void XMETHODCALLTYPE setRotation(const SMQuaternion &qRotation) override;

	void XMETHODCALLTYPE setEnabled(bool isEnabled) override;
	bool XMETHODCALLTYPE isEnabled() override;

	void XMETHODCALLTYPE setFlags(XOBSERVER_FLAGS flags) override;
	XOBSERVER_FLAGS XMETHODCALLTYPE getFlags() override;

	void XMETHODCALLTYPE reset() override;
	bool XMETHODCALLTYPE wasTriggered() override;

	void XMETHODCALLTYPE setHalfExtents(const float3 &vHalfExtents) override;

	void onModelChanged(const XEventModelChanged *pEvent);
private:
	IXCore *m_pCore;
	CModelChangedEventListener *m_pModelChangedEventListener;

	float3_t m_vPosition;
	SMQuaternion m_qRotation;

	float3_t m_vHalfExtents;
	bool m_isEnabled = true;

	bool m_wasTriggered = false;
};

#endif
