/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDSYSTEM_H
#define __SOUNDSYSTEM_H


#include <xcommon/IXCore.h>
#include <xcommon/IPluginManager.h>
#include <xcommon/IXSoundSystem.h>
#include <xcommon/IXAudioCodec.h>

#include <common/queue.h>

#include "SoundTypes.h"

//##########################################################################

inline float Com3DPan(const float3 &vSnd, const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	float3 side = SMVector3Cross(vListenerUp, vListenerDir);
	SMVector3Normalize(side);
	float x = SMVector3Dot(vSnd - vListenerPos, side);
	float z = SMVector3Dot(vSnd - vListenerPos, vListenerDir);
	float angle = atan2(x, z);
	float pan = sin(angle);
	return pan;
}

inline void Com3D(IAudioBuffer *pAB, float fDist, float m_fVolume, const float3 &vSnd, const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	float fPan = Com3DPan(vSnd, vListenerPos, vListenerDir, vListenerUp);
	float fVolume = 1.f - saturatef(SMVector3Distance(vSnd, vListenerPos) / fDist);
	fPan = lerpf(fPan, 0.f, fVolume);
	pAB->setPan(fPan);
	pAB->setVolume(fVolume*m_fVolume);
}

//##########################################################################

class CSoundSystem: public IXUnknownImplementation<IXSoundSystem>
{
public:
	SX_ALIGNED_OP_MEM();

	CSoundSystem(IXCore *pXCore);
	~CSoundSystem();

	virtual IXSoundLayer* XMETHODCALLTYPE createMasterLayer(const AudioRawDesc *pDesc, const char *szName) override;
	virtual IXSoundLayer* XMETHODCALLTYPE findLayer(const char *szName) override;

	virtual void XMETHODCALLTYPE update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp) override;

	//########################################################################

	IXAudioCodecTarget* getCodecTarget(const char *szName);

	IXCore* getCore() const;

	void addMessage(SndQueueMsg &oMsg);

protected:

	friend CSoundLayer;

	IAudioBufferEx* createMasterBuffer(AudioRawDesc *pDesc);
	bool supportedDesc(const AudioRawDesc *pDesc, AB_TYPE type);

	void addCodec(const char *szFmt, IXAudioCodec *pCodec);

	//########################################################################

	Queue<SndQueueMsg> m_queue;

	IXCore *m_pXCore = NULL;

	IAudio *m_pAudio = NULL;

	IXAudioCodec* getCodecSave();

	CSoundLayer *m_pMasterLayer = NULL;

	typedef AssotiativeArray<String, IXAudioCodec*> MapCodec;
	MapCodec m_mapCodecs;

	float3 m_vObserverPos, m_vObserverLook, m_vObserverUp;
	
	SpinLock m_oSpinLockUpdate;
};

#endif
