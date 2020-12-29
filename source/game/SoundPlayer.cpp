
#include "SoundPlayer.h"

//##########################################################################

/*! \skydocent sound_player
	
*/

//##########################################################################

BEGIN_PROPTABLE(CSoundPlayer)

	//! Звуковой файл
	DEFINE_FIELD_STRINGFN(m_szPathSound, 0, "file", "Sound file", setSound, EDITOR_SOUND)

	//! Громкость
	DEFINE_FIELD_FLOATFN(m_fVolume, 0, "volume", "Volume", setVolume, EDITOR_TEXTFIELD)

	//! Дистанция слышимости
	DEFINE_FIELD_FLOATFN(m_fDist, 0, "distance", "Hearing distance", setDistance, EDITOR_TEXTFIELD)

	//! Зацикливание
	DEFINE_FIELD_INTFN(m_iLoop, 0, "loop", "Loop", setLoop, EDITOR_COMBOBOX)
		COMBO_OPTION("None", "0")		//!< Нет
		COMBO_OPTION("Simple", "1")		//!< Простое (могут быть пустоты на стыках конца с началом)
		COMBO_OPTION("Seamless", "2")	//!< Непрерывное (пустот не будет, все будет заполнено звуком)
	EDITOR_COMBO_END()

	//! Включить
	DEFINE_INPUT(turnOn, "turnOn", "Turn On", PDF_NONE)
	//! Выключить
	DEFINE_INPUT(turnOff, "turnOff", "Turn Off", PDF_NONE)

	//! При включении
	DEFINE_OUTPUT(m_onTurnOn, "OnTurnOn", "On Turn On")
	//! При выключении
	DEFINE_OUTPUT(m_onTurnOff, "OnTurnOff", "On Turn Off")

	//! Изначально проигрывается
	DEFINE_FLAG(SND_PLAYER_START_PLAYED, "Play on")

	//! Тип фонового звука
	DEFINE_FLAG(SND_PLAYER_TYPE_AMBIENT, "Ambient")

END_PROPTABLE()

//**************************************************************************

REGISTER_ENTITY(CSoundPlayer, sound_player);

//##########################################################################

CSoundPlayer::~CSoundPlayer()
{
	mem_release(m_pPlayer);
}

//##########################################################################

void CSoundPlayer::setSound(const char *szSound)
{
	if(!m_pPlayer || fstrcmp(m_szPathSound, szSound) != 0)
	{
		_setStrVal(&m_szPathSound, szSound);

		mem_release(m_pPlayer);
		IXSoundSystem *pSound = (IXSoundSystem*)(Core_GetIXCore()->getPluginManager()->getInterface(IXSOUNDSYSTEM_GUID));
		IXSoundLayer *pGameLayer = pSound->findLayer("xGame");

		SOUND_SPACE space = (getFlags() & SND_PLAYER_TYPE_AMBIENT ? SOUND_SPACE_2D : SOUND_SPACE_3D);
		m_pPlayer = pGameLayer->newSoundPlayer(szSound, space);

		if(m_pPlayer)
		{
			m_pPlayer->setLoop((SOUND_LOOP)m_iLoop);
			m_pPlayer->setVolume(m_fVolume);
			m_pPlayer->setDistance(m_fDist);
			m_pPlayer->setWorldPos(getPos());

			if(getFlags() & SND_PLAYER_START_PLAYED)
				m_pPlayer->play();
		}
	}
}

//##########################################################################

void CSoundPlayer::setPos(const float3 &pos)
{
	BaseClass::setPos(pos);

	SAFE_CALL(m_pPlayer, setWorldPos, pos);
}

void CSoundPlayer::setVolume(float fVolume)
{
	m_fVolume = fVolume;

	SAFE_CALL(m_pPlayer, setVolume, fVolume);
}
float CSoundPlayer::getVolume()
{
	return(m_fVolume);
}

void CSoundPlayer::setDistance(float fDistance)
{
	m_fDist = fDistance;

	SAFE_CALL(m_pPlayer, setDistance, fDistance);
}
float CSoundPlayer::getDistance()
{
	return(m_fDist);
}

void CSoundPlayer::setLoop(int iLoop)
{
	m_iLoop = iLoop;
	SAFE_CALL(m_pPlayer, setLoop, (SOUND_LOOP)iLoop);
}
SOUND_LOOP CSoundPlayer::getLoop()
{
	return((SOUND_LOOP)m_iLoop);
}

//##########################################################################

void CSoundPlayer::turnOn(inputdata_t * pInputdata)
{
	if (m_pPlayer)
		m_pPlayer->play();
	FIRE_OUTPUT(m_onTurnOn, pInputdata->pInflictor);
}

void CSoundPlayer::turnOff(inputdata_t * pInputdata)
{
	if (m_pPlayer)
		m_pPlayer->stop();
	FIRE_OUTPUT(m_onTurnOff, pInputdata->pInflictor);
}

//##########################################################################

void CSoundPlayer::updateFlags()
{
	BaseClass::updateFlags();

	if (!m_pPlayer)
		return;

	if (getFlags() & SND_PLAYER_START_PLAYED)
		m_pPlayer->play();
	else
		m_pPlayer->stop();
	
	if (getFlags() & SND_PLAYER_TYPE_AMBIENT)
	{
		m_pPlayer->setSpace(SOUND_SPACE_2D);
		/*m_pPlayer->setPan(0.f);
		m_pPlayer->setVolume(m_fVolume);*/
	}
	else
		m_pPlayer->setSpace(SOUND_SPACE_3D);
}
