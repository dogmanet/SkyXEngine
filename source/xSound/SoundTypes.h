/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDTYPES_H
#define __SOUNDTYPES_H

#include <xcommon/IXSoundSystem.h>
#include <xcommon/IXAudioCodec.h>

#include <mital.h>

//##########################################################################

class CStreamData;

class CSoundBase;
class CSoundLoader;
class CSoundEmitter;
class CSoundPlayer;
class CSoundLayer;
class CSoundSystem;

//##########################################################################

//! состояния воспроизведения звукового объекта
enum SOUND_STATE
{
	SOUND_STATE_STOP,		//!< остановлен
	SOUND_STATE_PAUSE,	//!< приостановлен
	SOUND_STATE_PLAY,		//!< проигрывается
};


#endif