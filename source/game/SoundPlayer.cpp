
#include "SoundPlayer.h"

/*! \skydocent sound_player
	
*/

BEGIN_PROPTABLE(CSoundPlayer)

DEFINE_FIELD_STRINGFN(m_szPathSound, 0, "file", "Sound file", setSound, EDITOR_SOUND)

DEFINE_FIELD_FLOAT(m_fVolume, 0, "volume", "Volume", EDITOR_TEXTFIELD)

DEFINE_FIELD_FLOAT(m_fDist, 0, "distance", "Hearing distance", EDITOR_TEXTFIELD)

DEFINE_FIELD_INT(m_iLoop, 0, "loop", "Loop", EDITOR_COMBOBOX)
	COMBO_OPTION("None", "0")			//!< Нет
	COMBO_OPTION("Simple", "1")		//!< Простое (могут быть пустоты на стыках конца с началом)
	COMBO_OPTION("Seamless", "2")	//!< Непрерывное (пустот не будет, все будет заполнено звуком)
EDITOR_COMBO_END()

DEFINE_FIELD_INT(m_iType, 0, "type", "Type", EDITOR_COMBOBOX)
	COMBO_OPTION("2D", "0")
	COMBO_OPTION("3D", "1")
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

END_PROPTABLE()

REGISTER_ENTITY(CSoundPlayer, sound_player);

//##########################################################################

CSoundPlayer::CSoundPlayer(CEntityManager *pMgr):
	BaseClass(pMgr),
	m_pPlayer(NULL)
{
	
}

void CSoundPlayer::setSound(const char *szSound)
{
	if (!m_pPlayer || strcasecmp(m_pPlayer->getName(), szSound) != 0)
	{
		mem_release(m_pPlayer);
		IXSoundSystem *pSound = (IXSoundSystem*)(Core_GetIXCore()->getPluginManager()->getInterface(IXSOUNDSYSTEM_GUID));
		IXSoundLayer *pMasterLayer = pSound->findLayer("master");
		m_pPlayer = pMasterLayer->newSoundPlayer(szSound, (SOUND_DTYPE)m_iType);

		if (m_pPlayer)
		{
			m_szPathSound = szSound;
			m_pPlayer->setLoop((SOUND_LOOP)m_iLoop);
			m_pPlayer->setVolume(m_fVolume);

			if (getFlags() & SND_PLAYER_START_PLAYED)
				m_pPlayer->play();
		}
	}
}

void CSoundPlayer::onSync()
{
	CPointEntity::onSync();
	if (!m_pPlayer)
		return;

	float3 vPos = getPos();
	m_pPlayer->setWorldPos(vPos);

	if (m_pPlayer->getLoop() != (SOUND_LOOP)m_iLoop)
		m_pPlayer->setLoop((SOUND_LOOP)m_iLoop);

	if (m_pPlayer->getVolume() != m_fVolume)
		m_pPlayer->setVolume(m_fVolume);

	if (m_iType != m_pPlayer->getType())
	{
		m_pPlayer->setType((SOUND_DTYPE)m_iType);
		m_pPlayer->setPan(0.f);
	}
}

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

//**************************************************************************

void CSoundPlayer::updateFlags()
{
	BaseClass::updateFlags();

	if (getFlags() & SND_PLAYER_START_PLAYED)
	{
		if (m_pPlayer)
			m_pPlayer->play();
	}
	else
	{
		if (m_pPlayer)
			m_pPlayer->stop();
	}
}