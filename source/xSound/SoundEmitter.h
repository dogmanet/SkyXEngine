/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDEMITTER_H
#define __SOUNDEMITTER_H

#include <xcommon/IXSoundSystem.h>
#include "SoundBase.h"
#include <mital.h>

#include "SoundTypes.h"

//##########################################################################

class CSoundEmitter : public CSoundBase, public virtual IXSoundEmitter
{
public:
	~CSoundEmitter();
	virtual void XMETHODCALLTYPE play() override;

	void update();

protected:

	friend CSoundLayer;

	bool create(CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget);

	Array<IAudioBuffer*> m_aInstances;
};

#endif
