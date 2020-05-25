/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDSYSTEM_H
#define __SOUNDSYSTEM_H

#include <common/assotiativearray.h>
#include <common/string.h>
#include <common/path_utils.h>
#include <common/math.h>

#include <xcommon/IXCore.h>
#include <xcommon/IPluginManager.h>
#include <xcommon/IXSoundSystem.h>
#include <xcommon/IXAudioCodec.h>

#include "SoundTypes.h"

//##########################################################################

inline float Com3DPan(float3 &vSnd, float3 &vListenerPos, float3 &vListenerDir, float3 &vListenerUp)
{
	float3 side = SMVector3Cross(vListenerUp, vListenerDir);
	SMVector3Normalize(side);
	float x = SMVector3Dot(vSnd - vListenerPos, side);
	float z = SMVector3Dot(vSnd - vListenerPos, vListenerDir);
	float angle = atan2(x, z);
	float pan = sin(angle);
	return pan;
}

//##########################################################################

class CSoundSystem: public IXUnknownImplementation<IXSoundSystem>
{
public:

	CSoundSystem(IXCore *pXCore);
	~CSoundSystem();

	virtual IXSoundLayer* XMETHODCALLTYPE createMasterLayer(const AudioRawDesc *pDesc, const char *szName) override;
	virtual IXSoundLayer* XMETHODCALLTYPE findLayer(const char *szName) override;

	virtual void XMETHODCALLTYPE update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp) override;

	IXAudioCodecTarget* getCodecTarget(const char *szPath);

	IXCore* getCore() const;

protected:

	friend CSoundLayer;

	IAudioBufferEx* createMasterBuffer(AudioRawDesc *pDesc);
	bool supportedDesc(const AudioRawDesc *pDesc, AB_TYPE type);

	void addCodec(const char *szFmt, IXAudioCodec *pCodec);

	IXCore *m_pXCore = NULL;

	IAudio *m_pAudio = NULL;

	IXAudioCodec* getCodecSave();

	CSoundLayer *m_pMasterLayer = NULL;

	typedef AssotiativeArray<String, IXAudioCodec*> mapcodec;
	mapcodec m_mapCodecs;
};

#endif
