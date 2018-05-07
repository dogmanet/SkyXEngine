
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл level - пространства имен для управления уровнем
*/

/*! \defgroup managed_render_level level - пространство имен для управления уровнем
 \ingroup managed_render
@{*/

#ifndef __LEVEL_H
#define __LEVEL_H

#include <gdefines.h>
#include <common/string.h>
#include <common/file_utils.h>
#include <level/sxlevel.h>

#include "ambientsounds.h"
#include "weather.h"

class CLevel
{
public:
	CLevel();
	~CLevel();

	//! очистка уровня
	void clear();					

	//! Загрузка уровня с именем name
	void load(const char *szName, bool isGame);	

	//! Загрузка уровня с именем name
	void save(const char *szName);

	//! возвращает и записывает имя уровня
	const char* getName(char *szName=0);


	//! загрузка файла с инфой о частицах
	void loadParticles();			

	//! сохранение файла с инфой о частицах
	void saveParticles();			

	//**********************************************************************

	//! добавить звук
	void ambientSndAdd(const char *szPath){ m_pAmbientSounds->add(szPath); }

	//! возвращает общее количество загруженных звуков
	UINT ambientSndGetCount(){ return m_pAmbientSounds->getCount(); }

	//! в path записывает путь до звука по его id
	void ambientSndGet(ID id, char *szPath){ m_pAmbientSounds->get(id, szPath); }

	//! очистка списка звуков
	void ambientSndClear(){ m_pAmbientSounds->clear(); }

	//! начать проигрывание звуков
	void ambientSndPlay(){ m_pAmbientSounds->play(); }

	//! обновление проигрывания
	void ambientSndUpdate(){ m_pAmbientSounds->update(); }

	//! пауза проигрывания
	void ambientSndPause(){ m_pAmbientSounds->pause(); }

	//! возвращает "проигрываются ли звуки"
	bool ambientSndGetPlaying(){ return m_pAmbientSounds->getPlaying(); }

	//**********************************************************************

	//! загрузка конфига погоды
	void weatherLoad(const char *szPath){ m_pWeather->load(szPath); }

	//! обновление погоды
	void weatherUpdate(){ m_pWeather->update(); }

	//! возвращает текущую плотность дождя
	float weatherGetCurrRainDensity(){ return m_pWeather->getCurrRainDensity(); }

	//! начать воспроизведение звуков погоды
	void weatherSndPlay(){ m_pWeather->sndPlay(); }

	//! приостановить воспроизведение звуков погоды
	void weatherSndPause(){ m_pWeather->sndPause(); }

	//! возвращает "воспроизводятся ли звуки погоды"
	bool weatherSndGetPlaying(){ return m_pWeather->sndGetPlaying(); }

	//**********************************************************************

protected:

	//! текущее имя уровня
	char m_szName[OBJECT_NAME_MAX_LEN];	

	String m_sAmbientSounds;
	String m_sWeather;

	CWeather* m_pWeather;
	CAmbientSounds* m_pAmbientSounds;
};

#endif

//!@} managed_render_level
