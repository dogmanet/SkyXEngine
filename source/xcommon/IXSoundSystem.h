/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __IXSOUNDSYSTEM_H
#define __IXSOUNDSYSTEM_H

#include <gdefines.h>
#include <common/math.h>
#include <common/AudioRawDesc.h>

//##########################################################################

// {8FF4F913-5EA1-4EE6-8C06-73D8A49EBC22}
#define IXSOUNDSYSTEM_GUID DEFINE_XGUID(0x8ff4f913, 0x5ea1, 0x4ee6, 0x8c, 0x6, 0x73, 0xd8, 0xa4, 0x9e, 0xbc, 0x22)

#define IXSOUNDSYSTEM_VERSION 2

//##########################################################################

#define SOUND_CACHE "cache"

enum SOUND_DTYPE
{
	SOUND_DTYPE_2D,
	SOUND_DTYPE_3D
};

//! тип зацикленности звука
enum SOUND_LOOP
{
	SOUND_LOOP_NONE = 0,
	SOUND_LOOP_SIMPLE = 1,
	SOUND_LOOP_SEAMLESS = 2
};

//##########################################################################

class IXSoundBase: public IXUnknown
{
public:
	virtual SOUND_DTYPE XMETHODCALLTYPE getType() = 0;

	virtual void XMETHODCALLTYPE play() = 0;

	virtual void XMETHODCALLTYPE setVolume(float fVolume) = 0;
	virtual float XMETHODCALLTYPE getVolume() const = 0;

	virtual void XMETHODCALLTYPE setPan(float fPan) = 0;
	virtual float XMETHODCALLTYPE getPan() const = 0;

	virtual const float3& XMETHODCALLTYPE getWorldPos() const = 0;
	virtual void XMETHODCALLTYPE setWorldPos(const float3 &vPos) = 0;

	virtual float XMETHODCALLTYPE getDistance() const = 0;
	virtual void XMETHODCALLTYPE setDistance(float fDist) = 0;
};

//**************************************************************************

class IXSoundEmitter: public virtual IXSoundBase
{
};

//**************************************************************************

class IXSoundPlayer: public virtual IXSoundBase
{
public:
	virtual void XMETHODCALLTYPE resume() = 0;
	virtual void XMETHODCALLTYPE pause() = 0;
	virtual void XMETHODCALLTYPE stop() = 0;

	virtual bool XMETHODCALLTYPE isPlaying() const = 0;

	virtual SOUND_LOOP XMETHODCALLTYPE getLoop() const = 0;
	virtual void XMETHODCALLTYPE setLoop(SOUND_LOOP loop) = 0;

	virtual float XMETHODCALLTYPE getTime() const = 0;
	virtual void XMETHODCALLTYPE setTime(float fTime) = 0;

	virtual float XMETHODCALLTYPE getLength() const = 0;
};

//##########################################################################

class IXSoundLayer: public IXUnknown
{
public:

	virtual void XMETHODCALLTYPE play(bool canPlay) = 0;
	virtual bool XMETHODCALLTYPE isPlaying() const = 0;
	virtual const char* XMETHODCALLTYPE getName() const = 0;

	virtual IXSoundLayer* XMETHODCALLTYPE findLayer(const char *szName) = 0;

	virtual IXSoundLayer* XMETHODCALLTYPE newSoundLayer(const AudioRawDesc *pDesc, const char *szName) = 0;
	virtual IXSoundEmitter* XMETHODCALLTYPE newSoundEmitter(const char *szName, SOUND_DTYPE dtype) = 0;
	virtual IXSoundPlayer* XMETHODCALLTYPE newSoundPlayer(const char *szName, SOUND_DTYPE dtype) = 0;

	virtual void XMETHODCALLTYPE getDesc(AudioRawDesc *pDesc) const = 0;
};

//**************************************************************************

class IXSoundSystem: public IXUnknown
{
public:

	virtual IXSoundLayer* XMETHODCALLTYPE createMasterLayer(const AudioRawDesc *pDesc, const char *szName) = 0;
	virtual IXSoundLayer* XMETHODCALLTYPE findLayer(const char *szName) = 0;

	virtual void XMETHODCALLTYPE update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp) = 0;
};

#endif
