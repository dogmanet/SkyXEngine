#ifndef __IXMUTATIONOBSERVER_H
#define __IXMUTATIONOBSERVER_H

#include <gdefines.h>

//! UNSTABLE! DO NOT USE!

enum XOBSERVER_FLAGS
{
	XOBSERVER_FLAG_MOVEMENT = 0x00000001,
	XOBSERVER_FLAG_ANIMATION = 0x00000002,
	// XOBSERVER_FLAG_EXISTANCE = 0x00000004,
};
DEFINE_ENUM_FLAG_OPERATORS(XOBSERVER_FLAGS);

class IXMutationObserver: public IXUnknown
{
public:
	virtual float3 XMETHODCALLTYPE getPosition() = 0;
	virtual void XMETHODCALLTYPE setPosition(const float3 &vPosition) = 0;
	
	virtual SMQuaternion XMETHODCALLTYPE getRotation() = 0;
	virtual void XMETHODCALLTYPE setRotation(const SMQuaternion &qRotation) = 0;
	
	virtual void XMETHODCALLTYPE setEnabled(bool isEnabled) = 0;
	virtual bool XMETHODCALLTYPE isEnabled() = 0;
	
	virtual void XMETHODCALLTYPE setFlags(XOBSERVER_FLAGS flags) = 0;
	virtual XOBSERVER_FLAGS XMETHODCALLTYPE getFlags() = 0;
	
	virtual void XMETHODCALLTYPE reset() = 0;
	virtual bool XMETHODCALLTYPE wasTriggered() = 0;

	virtual void XMETHODCALLTYPE setHalfExtents(const float3 &vHalfExtents) = 0;

	
};

#endif
