
#include "Weather.h"

CWeatherRndSnd::CWeatherRndSnd()
{
	m_ulNextPlay = 0;
	m_iVolumeB = 50;
	m_iVolumeE = 90;

	m_ulPeriodB = 1000;
	m_ulPeriodE = 10000;

	m_idCurrPlay = -1;
	m_isPlaying = false;
}

CWeatherRndSnd::~CWeatherRndSnd()
{
	clear();
}

void CWeatherRndSnd::clear()
{
	resetOld();

	for (UINT i = 0; i < m_aArrSnds.size(); ++i)
	{
		if (m_aArrSnds[i].m_id >= 0)
		{
			SSCore_SndStop(m_aArrSnds[i].m_id);
			SSCore_SndDelete(m_aArrSnds[i].m_id);
		}
	}
	m_aArrSnds.clear();
}

void CWeatherRndSnd::resetOld()
{
	for (UINT i = 0; i < m_aCurrSndIDs.size(); ++i)
	{
		if (m_aCurrSndIDs[i] >= 0)
			SSCore_SndStop(m_aCurrSndIDs[i]);
	}
	m_aCurrSndIDs.clear();
}

void CWeatherRndSnd::setParamPlayVolume(int iBVol, int iEVol)
{
	m_iVolumeB = iBVol;
	m_iVolumeE = iEVol;
}

void CWeatherRndSnd::setParamPlayPeriod(DWORD ulBPer, DWORD ulEPer)
{
	m_ulPeriodB = ulBPer;
	m_ulPeriodE = ulEPer;
}

void CWeatherRndSnd::addSound(const char *szPath)
{
	bool isunic = true;
	ID tmpid = -1;
	for (UINT i = 0; i<m_aArrSnds.size(); i++)
	{
		if (stricmp(m_aArrSnds[i].m_sPath.c_str(), szPath) == 0)
		{
			isunic = false;
			tmpid = m_aArrSnds[i].m_id;
			break;
		}
	}

	if (isunic)
	{
		ID tmpid = SSCore_SndCreate2d(szPath);
		m_aArrSnds.push_back(CSnd(szPath, tmpid));
		m_aCurrSndIDs.push_back(tmpid);
	}
	else
	{
		m_aCurrSndIDs.push_back(tmpid);
	}
}

void CWeatherRndSnd::update()
{
	if (!m_isPlaying)
		return;

	static const float * weather_snd_volume = GET_PCVAR_FLOAT("weather_snd_volume");

	if (m_aCurrSndIDs.size() > 0 && (m_ulNextPlay == 0 || m_ulNextPlay < TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER))))
	{
		int tmpkeysnd = rand() % (m_aCurrSndIDs.size());
		if((int)m_aCurrSndIDs.size() > tmpkeysnd && m_aCurrSndIDs[tmpkeysnd] && m_aCurrSndIDs[tmpkeysnd] >= 0)
		{
			m_idCurrPlay = m_aCurrSndIDs[tmpkeysnd];
			SSCore_SndPosCurrSet(m_idCurrPlay, 0);
			int tmprndvol = (rand() % (m_iVolumeE - m_iVolumeB)) + m_iVolumeB;
			SSCore_SndVolumeSet(m_idCurrPlay, (long)(weather_snd_volume ? (float)tmprndvol * (*weather_snd_volume) : tmprndvol), SOUND_VOL_PCT);
			SSCore_SndPlay(m_idCurrPlay);

			DWORD tmprnd = (rand() % (m_ulPeriodE - m_ulPeriodB)) + m_ulPeriodB;
			m_ulNextPlay = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + tmprnd + ((SSCore_SndLengthSecGet(m_idCurrPlay) + 1) * 1000);
		}
	}
}

void CWeatherRndSnd::play()
{
	if (m_isPlaying)
		return;

	m_isPlaying = true;

	if (m_idCurrPlay >= 0 && SSCore_SndStateGet(m_idCurrPlay) == SOUND_OBJSTATE_PAUSE)
		SSCore_SndPlay(m_idCurrPlay);
}

void CWeatherRndSnd::pause()
{
	if (!m_isPlaying)
		return;

	m_isPlaying = false;

	if (m_idCurrPlay >= 0 && SSCore_SndStateGet(m_idCurrPlay) == SOUND_OBJSTATE_PLAY)
		SSCore_SndPlay(SOUND_OBJSTATE_PAUSE);
}

bool CWeatherRndSnd::getPlaying()
{
	return m_isPlaying;
}

//#############################################################################

CWeather::CWeather()
{
	m_isPlaying = false;
	m_iSectionOld = -1;
	m_iSectionCurr = -1;
	m_hasUpdate = false;
	m_ulTimeRainVolSndLast = 0;
	m_ulTimeMlsecOld = m_ulTimeMlsecCurr = 0;

	float3 vGeomMin, vGeomMax;
	SGeom_ModelsGetMinMax(&vGeomMin, &vGeomMax);
	m_fLevelMaxY = vGeomMax.y + 10.f;

	m_idEffRain = SPE_EffectGetByName("rain");

	m_iTrackPosCount = 0;
	m_aTrackPos = 0;
	if (m_idEffRain >= 0)
	{
		m_iTrackPosCount = SPE_EmitterCountGet(m_idEffRain,0);
		m_aTrackPos = new float3[m_iTrackPosCount];
	}
	else
	{
		g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect 'rain'", gen_msg_location);
	}

	m_idEffThunderbolt = SPE_EffectGetByName("thunderbolt");
	if (m_idEffThunderbolt < 0)
	{
		g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect 'thunderbolt'", gen_msg_location);
	}
	m_ulTimeBoltNext = m_ulTimeBoltLast = 0;

	m_idLightThunderbolt = SML_LigthsCreatePoint(&float3(0, 0, 0), 200, &float3(1, 1, 1), false, true);
	SML_LigthsSetEnable(m_idLightThunderbolt, false);
	m_idSndRain = SSCore_SndCreate2d("nature/rain.ogg",true);
	m_idSndThunder = SSCore_SndCreate2d("nature/thunder.ogg");

	m_fRainVolume = 0;
}

CWeather::~CWeather()
{
	m_aTimeSections.clear();
	SML_LigthsDeleteLight(m_idLightThunderbolt);
	SSCore_SndDelete(m_idSndRain);
	SSCore_SndDelete(m_idSndThunder);
}

void CWeather::load(const char *szPath)
{
	float3 vGeomMin, vGeomMax;
	SGeom_ModelsGetMinMax(&vGeomMin, &vGeomMax);
	m_fLevelMaxY = vGeomMax.y + 10.f;

	if (szPath == 0 || m_aTimeSections.size() > 0)
	{
		m_aTimeSections.clear();
		m_iSectionOld = m_iSectionCurr = -1;
		m_ulTimeMlsecOld = m_ulTimeMlsecCurr = 0;
		m_hasUpdate = false;
		m_isPlaying = false;
		m_RndSnd.pause();
		m_RndSnd.resetOld();

		SGCore_SkyBoxLoadTex(0);
		SGCore_SkyBoxSetRot(0);
		SGCore_SkyCloudsLoadTex(0);
		SGCore_SkyCloudsSetSpeed(0);
		SGCore_SkyCloudsSetRot(0);

		
		SPE_EffectEnableSet(m_idEffRain, false);
		SSCore_SndStop(m_idSndRain);

		SPE_EffectEnableSet(m_idEffThunderbolt, false);
		SML_LigthsSetEnable(m_idLightThunderbolt, false);
		SSCore_SndStop(m_idSndThunder);

		m_aTimeSections.clear();

		if (szPath == 0)
			return;
	}

	ISXConfig* config = Core_OpConfig(szPath);

	if (!config->sectionExists("sections"))
	{
		g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - not found section 'sections' \nfile '%s'", gen_msg_location, szPath);
		mem_release_del(config);
		return;
	}

	int key_count = config->getKeyCount("sections");

	for (int i = 0; i < key_count; ++i)
	{
		int time = 0;
		const char* str = config->getKeyName("sections", i);

		if (strlen(str) != 8)
		{
			g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - unresolved name of key '%s' \nfile '%s' \nsection '%s'", gen_msg_location, str, szPath, "sections");
			mem_release_del(config);
			return;
		}

		char tmpall[32];
		sprintf(tmpall, str);

		char* tmpstr = strtok(tmpall, ":");
		WEATHER_CONFIG_SECTION_KEY(tmpstr, str, szPath, "sections");
		time = atoi(tmpstr) * 10000;

		tmpstr = strtok(NULL, ":");
		WEATHER_CONFIG_SECTION_KEY(tmpstr, str, szPath, "sections");
		time += atoi(tmpstr) * 100;

		tmpstr = strtok(NULL, ":");
		WEATHER_CONFIG_SECTION_KEY(tmpstr, str, szPath, "sections");
		time += atoi(tmpstr);

		m_aTimeSections.push_back(CTimeSection(time, config->getKey("sections", str)));

		int qwerty = 0;
	}

	Array<CTimeSection> tmpts = m_aTimeSections;
	m_aTimeSections.clear();
	for (int i = 0, il = tmpts.size(); i < il; ++i)
	{
		int num = 0;
		for (int k = 0, kl = tmpts.size(); k < kl; ++k)
		{
			if (i != k && tmpts[i].m_iTime > tmpts[k].m_iTime)
				++num;
		}

		m_aTimeSections[num] = tmpts[i];
	}

	tmpts.clear();

	for (int i = 0, il = m_aTimeSections.size(); i < il; ++i)
	{
		//g_fnReportf(0, "%d\n", m_aTimeSections[i].time);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "sky_texture"))
			sprintf(m_aTimeSections[i].m_DataSection.m_szSkyTex, "%s", config->getKey(m_aTimeSections[i].m_szSection, "sky_texture"));
		else
			WEATHER_CONFIG_LACKS_KEY("sky_texture", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "clouds_texture"))
			sprintf(m_aTimeSections[i].m_DataSection.m_szCloudsTex, "%s", config->getKey(m_aTimeSections[i].m_szSection, "clouds_texture"));
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_texture", szPath, m_aTimeSections[i].m_szSection);


		if (config->keyExists(m_aTimeSections[i].m_szSection, "sun_texture"))
			sprintf(m_aTimeSections[i].m_DataSection.m_szSunTex, "%s", config->getKey(m_aTimeSections[i].m_szSection, "sun_texture"));
		else
			WEATHER_CONFIG_LACKS_KEY("sun_texture", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "sun_pos"))
		{
			const char* text_sp = config->getKey(m_aTimeSections[i].m_szSection, "sun_pos");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_pos", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSunPos.x = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_pos", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSunPos.y = (float)atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("sun_pos", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "sky_rotation"))
			m_aTimeSections[i].m_DataSection.m_fSkyRotation = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "sky_rotation")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("sky_rotation", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "sky_color"))
		{
			const char* text_sp = config->getKey(m_aTimeSections[i].m_szSection, "sky_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSkyColor.x = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSkyColor.y = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSkyColor.z = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSkyColor.w = (float)atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("sky_color", szPath, m_aTimeSections[i].m_szSection);


		if (config->keyExists(m_aTimeSections[i].m_szSection, "clouds_color"))
		{
			const char* text_sp = config->getKey(m_aTimeSections[i].m_szSection, "clouds_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vCloudsColor.x = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vCloudsColor.y = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vCloudsColor.z = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vCloudsColor.w = (float)atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_color", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "clouds_rotate"))
			m_aTimeSections[i].m_DataSection.m_fCloudsRotate = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "clouds_rotate")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_rotate", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "clouds_transparency"))
			m_aTimeSections[i].m_DataSection.m_fCloudsTransparency = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "clouds_transparency")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_transparency", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "clouds_speed"))
			m_aTimeSections[i].m_DataSection.m_fCloudsSpeed = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "clouds_speed")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_speed", szPath, m_aTimeSections[i].m_szSection);


		if (config->keyExists(m_aTimeSections[i].m_szSection, "sun_color"))
		{
			const char* text_sp = config->getKey(m_aTimeSections[i].m_szSection, "sun_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSunColor.x = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSunColor.y = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vSunColor.z = (float)atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("sun_color", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "far"))
			m_aTimeSections[i].m_DataSection.m_fFar = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "far")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("far", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "rain_density"))
			m_aTimeSections[i].m_DataSection.m_fRainDensity = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "rain_density")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("rain_density", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "rain_color"))
		{
			const char* text_sp = config->getKey(m_aTimeSections[i].m_szSection, "rain_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vRainColor.x = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vRainColor.y = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vRainColor.z = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vRainColor.w = (float)atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("rain_color", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "fog_density"))
			m_aTimeSections[i].m_DataSection.m_fFogDensity = (float)String(config->getKey(m_aTimeSections[i].m_szSection, "fog_density")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("fog_density", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "fog_color"))
		{
			const char* text_sp = config->getKey(m_aTimeSections[i].m_szSection, "fog_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "fog_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vFogColor.x = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "fog_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vFogColor.y = (float)atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "fog_color", szPath, m_aTimeSections[i].m_szSection);
			m_aTimeSections[i].m_DataSection.m_vFogColor.z = (float)atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("fog_color", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "thunderbolt"))
			m_aTimeSections[i].m_DataSection.m_hasThunderbolt = String(config->getKey(m_aTimeSections[i].m_szSection, "thunderbolt")).ToBool();
		else
			WEATHER_CONFIG_LACKS_KEY("thunderbolt", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "thunder_period"))
			m_aTimeSections[i].m_DataSection.m_ulThunderPeriod = String(config->getKey(m_aTimeSections[i].m_szSection, "thunder_period")).ToUnsLongInt();
		else
			WEATHER_CONFIG_LACKS_KEY("thunder_period", szPath, m_aTimeSections[i].m_szSection);

		if (config->keyExists(m_aTimeSections[i].m_szSection, "env_ambient"))
		{
			const char* text_env = config->getKey(m_aTimeSections[i].m_szSection, "env_ambient");

			if (!config->sectionExists(text_env))
			{
				g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - lacks env_ambient section '%s' \nszPath '%s' \nsection '%s'", gen_msg_location, text_env, szPath, m_aTimeSections[i].m_szSection);
				return;
			}

			if (config->keyExists(text_env, "period"))
			{
				const char* text_sp = config->getKey(text_env, "period");
				char text_sp2[64];
				strcpy(text_sp2, text_sp);

				char* tmpstr = strtok(text_sp2, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "period", szPath, text_env);
				m_aTimeSections[i].m_DataSection.Snds.period_b = atol(tmpstr);

				tmpstr = strtok(NULL, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "period", szPath, text_env);
				m_aTimeSections[i].m_DataSection.Snds.period_e = atol(tmpstr);
			}
			else
				WEATHER_CONFIG_LACKS_KEY("period", szPath, text_env);

			if (config->keyExists(text_env, "volume"))
			{
				const char* text_sp = config->getKey(text_env, "volume");
				char text_sp2[64];
				strcpy(text_sp2, text_sp);

				char* tmpstr = strtok(text_sp2, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "volume", szPath, text_env);
				m_aTimeSections[i].m_DataSection.Snds.volume_b = atol(tmpstr);

				tmpstr = strtok(NULL, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "volume", szPath, text_env);
				m_aTimeSections[i].m_DataSection.Snds.volume_e = atol(tmpstr);
			}
			else
				WEATHER_CONFIG_LACKS_KEY("volume", szPath, text_env);

			if (config->keyExists(text_env, "sounds"))
			{
				const char* text_sp = config->getKey(text_env, "sounds");
				char text_sp2[4096];
				strcpy(text_sp2, text_sp);

				char* tmpstr = strtok(text_sp2, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sounds", szPath, text_env);
				m_aTimeSections[i].m_DataSection.Snds.arr_path.push_back(tmpstr);

				while (tmpstr != NULL)
				{
					tmpstr = strtok(NULL, " ,");

					if (tmpstr)
						m_aTimeSections[i].m_DataSection.Snds.arr_path.push_back(tmpstr);
				}

			}
			else
				WEATHER_CONFIG_LACKS_KEY("sounds", szPath, text_env);
		}
	}

	mem_release_del(config);
}

void CWeather::update()
{
	if (m_isPlaying)
		m_RndSnd.update();

	ID gid = SML_LigthsGetGlobal();

	static const float * weather_snd_volume = GET_PCVAR_FLOAT("weather_snd_volume");

	//если есть дождь то обновл¤ем его позицию и громкость
	if (m_iSectionCurr >= 0 && (int)m_aTimeSections.size() > m_iSectionCurr && m_aTimeSections[m_iSectionCurr].m_DataSection.m_fRainDensity > 0.f)
	{
		static float3 campos;
		Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &campos);
		SPE_EffectPosSet(m_idEffRain, &float3(campos.x, campos.y - WEATHER_RAIN_MIN_Y_OBSERVER, campos.z));
		updateRainSound();
	}

	static float * main_rain_density = (float*)GET_PCVAR_FLOAT("main_rain_density");
	static float main_rain_density_old = 1.f;

	if (main_rain_density && (*main_rain_density) != main_rain_density_old)
	{
		main_rain_density_old = *main_rain_density;
		if (m_aTimeSections[m_iSectionCurr].m_DataSection.m_fRainDensity > 0.f)
			SPE_EmitterSet(m_idEffRain, 0, ReCreateCount, main_rain_density_old * m_aTimeSections[m_iSectionCurr].m_DataSection.m_fRainDensity * float(WEATHER_RAIN_RECREATE_COUNT));
	}

	//получаем текущую игровую дату
	tm g_tm;
	time_t g_time = Core_TimeUnixCurrGet(Core_RIntGet(G_RI_INT_TIMER_GAME));
	localtime_s(&g_tm, &g_time);

	//цифровое представление времени суток
	int ltime = g_tm.tm_hour * 10000 + g_tm.tm_min * 100 + g_tm.tm_sec;
	
	//представление времени суток в млсекундах
	DWORD ltime_mlsec = (g_tm.tm_sec * 1000) + (g_tm.tm_min * 60 * 1000) + (g_tm.tm_hour * 60 * 60 * 1000);
	int curr_section = -1;

	//находим секцию дл¤ текущего времени суток
	for (int i = 0, il = m_aTimeSections.size(); i < il; ++i)
	{
		if (ltime >= m_aTimeSections[i].m_iTime)
			curr_section = i;
	}

	//если только что обновл¤емс¤ либо сменилась секци¤
	if (curr_section >= 0 && curr_section != m_iSectionCurr)
	{
		m_iSectionOld = m_iSectionCurr;
		m_ulTimeMlsecOld = m_ulTimeMlsecCurr;
		if (m_iSectionOld < 0)
		{
			m_iSectionOld = curr_section;
			m_ulTimeMlsecOld = ltime_mlsec;
		}
		
		m_iSectionCurr = curr_section;
		m_ulTimeMlsecCurr = ltime_mlsec;

		SGCore_SkyBoxChangeTex(m_aTimeSections[m_iSectionCurr].m_DataSection.m_szSkyTex);
		SGCore_SkyBoxSetRot(m_aTimeSections[m_iSectionCurr].m_DataSection.m_fSkyRotation);
		SGCore_SkyCloudsChangeTex(m_aTimeSections[m_iSectionCurr].m_DataSection.m_szCloudsTex);
		SGCore_SkyCloudsSetSpeed(m_aTimeSections[m_iSectionCurr].m_DataSection.m_fCloudsSpeed);
		SGCore_SkyCloudsSetRot(m_aTimeSections[m_iSectionCurr].m_DataSection.m_fCloudsRotate);
		
		if (m_aTimeSections[m_iSectionCurr].m_DataSection.m_szSunTex[0] != '0')
			SPP_ChangeTexSun(m_aTimeSections[m_iSectionCurr].m_DataSection.m_szSunTex);
		
		if (gid >= 0)
			SML_LigthsSetEnable(gid, m_aTimeSections[m_iSectionCurr].m_DataSection.m_szSunTex[0] != '0');

		m_hasUpdate = true;

		//если плотность дожд¤ больше нул¤ тогда включаем дождь
		if (m_aTimeSections[m_iSectionCurr].m_DataSection.m_fRainDensity > 0.f)
		{
			SPE_EmitterSet(m_idEffRain, 0, Color, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vRainColor);
			SPE_EmitterSet(m_idEffRain, 0, ReCreateCount, main_rain_density_old * m_aTimeSections[m_iSectionCurr].m_DataSection.m_fRainDensity * float(WEATHER_RAIN_RECREATE_COUNT));
			SPE_EffectEnableSet(m_idEffRain, true);

			SSCore_SndPosCurrSet(m_idSndRain, 0);
			SSCore_SndVolumeSet(m_idSndRain, 0, SOUND_VOL_PCT);

			if (m_isPlaying)
				SSCore_SndPlay(m_idSndRain);
			else
				SSCore_SndPause(m_idSndRain);
		}
		//иначе выключаем
		else
		{
			SPE_EffectEnableSet(m_idEffRain, false);
			SSCore_SndStop(m_idSndRain);
		}

		//очищаем старые звуки
		m_RndSnd.resetOld();

		//если есть пути до новых случайных звуков
		if (m_aTimeSections[m_iSectionCurr].m_DataSection.Snds.arr_path.size() > 0)
		{
			//загружаем эти звуки
			CDataSection::CSndRnd Snds = m_aTimeSections[m_iSectionCurr].m_DataSection.Snds;

			for (int i = 0, il = Snds.arr_path.size(); i < il; ++i)
			{
				m_RndSnd.addSound(Snds.arr_path[i].c_str());
				m_RndSnd.setParamPlayPeriod(Snds.period_b, Snds.period_e);
				m_RndSnd.setParamPlayVolume(Snds.volume_b, Snds.volume_e);
			}
		}
	}

	
	
	if (m_hasUpdate)
	{
		//расчет коэфициента интерпол¤ции
		float lerp_factor = 1.f;
			
		if (m_ulTimeMlsecCurr - m_ulTimeMlsecOld > 0)
			lerp_factor = float(ltime_mlsec - m_ulTimeMlsecCurr) / float(m_ulTimeMlsecCurr - m_ulTimeMlsecOld);

		//если коэфициент интерпол¤ции больше либо равен 1 то значит интерпол¤ци¤ больше не нужна на текущей секции
		if (lerp_factor >= 1.f)
			m_hasUpdate = false;

		//цвет скайбокса
		float4_t tmp_sky_color;
		tmp_sky_color.x = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSkyColor.x, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSkyColor.x, lerp_factor);
		tmp_sky_color.y = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSkyColor.y, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSkyColor.y, lerp_factor);
		tmp_sky_color.z = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSkyColor.z, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSkyColor.z, lerp_factor);
		tmp_sky_color.w = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSkyColor.w, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSkyColor.w, lerp_factor);
		SGCore_SkyBoxSetColor(&tmp_sky_color);

		//цвет облаков
		float4_t tmp_clouds_color;
		tmp_clouds_color.x = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vCloudsColor.x, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vCloudsColor.x, lerp_factor);
		tmp_clouds_color.y = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vCloudsColor.y, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vCloudsColor.y, lerp_factor);
		tmp_clouds_color.z = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vCloudsColor.z, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vCloudsColor.z, lerp_factor);
		tmp_clouds_color.w = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vCloudsColor.w, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vCloudsColor.w, lerp_factor);
		SGCore_SkyCloudsSetColor(&tmp_sky_color);

		//прозрачность облаков
		float tmp_clouds_transparency = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_fCloudsTransparency, m_aTimeSections[m_iSectionCurr].m_DataSection.m_fCloudsTransparency, lerp_factor);
		SGCore_SkyCloudsSetAlpha(tmp_clouds_transparency);

		if (gid >= 0)
		{
			//цвет солнца
			float3 tmp_scolor;
			tmp_scolor.x = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSunColor.x, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSunColor.x, lerp_factor);
			tmp_scolor.y = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSunColor.y, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSunColor.y, lerp_factor);
			tmp_scolor.z = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSunColor.z, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSunColor.z, lerp_factor);
			SML_LigthsSetColor(gid, &tmp_scolor);

			//позици¤ солнца
			float3 tmp_spos;
			tmp_spos.x = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSunPos.x, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSunPos.x, lerp_factor);
			tmp_spos.y = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vSunPos.y, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vSunPos.y, lerp_factor);
			tmp_spos.z = 0;
			SML_LigthsSetPos(gid, &tmp_spos, false);
		}

		//дальность видимости
		float tmpfar = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_fFar, m_aTimeSections[m_iSectionCurr].m_DataSection.m_fFar, lerp_factor);
		Core_0SetCVarFloat("p_far", tmpfar);

		//плотность тумана
		static float * pp_fog_density = (float*)GET_PCVAR_FLOAT("pp_fog_density");
		if (pp_fog_density)
			*pp_fog_density = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_fFogDensity, m_aTimeSections[m_iSectionCurr].m_DataSection.m_fFogDensity, lerp_factor);

		//цвет тумана
		static const UINT_PTR *pp_fog_color = GET_PCVAR_POINTER("pp_fog_color");
		if (pp_fog_color && *pp_fog_color)
		{
			float3_t* tmp_fog_color2 = (float3_t*)(*pp_fog_color);
			tmp_fog_color2->x = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vFogColor.x, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vFogColor.x, lerp_factor);
			tmp_fog_color2->y = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vFogColor.y, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vFogColor.y, lerp_factor);
			tmp_fog_color2->z = lerpf(m_aTimeSections[m_iSectionOld].m_DataSection.m_vFogColor.z, m_aTimeSections[m_iSectionCurr].m_DataSection.m_vFogColor.z, lerp_factor);
		}
		else
			g_fnReportf(REPORT_MSG_LEVEL_WARNING, "cvar pp_fog_color is not init");
	}

	//если в текущей секции есть частота молнии
	if (m_iSectionCurr >= 0 && m_aTimeSections[m_iSectionCurr].m_DataSection.m_ulThunderPeriod > 0)
	{
		//если следующее врем¤ грозы и предыдущее нулевые, тогда генерируем следующее врем¤
		if (m_ulTimeBoltNext == 0 && m_ulTimeBoltLast == 0)
			m_ulTimeBoltNext = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + (rand() % m_aTimeSections[m_iSectionCurr].m_DataSection.m_ulThunderPeriod);
		//иначе если предыдущее врем¤ грозы нулевое и врем¤ следующей грозы наступило
		else if (m_ulTimeBoltLast == 0 && TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) >= m_ulTimeBoltNext)
		{
			static const bool * main_thunderbolt = GET_PCVAR_BOOL("main_thunderbolt");
			//если предусмотерна молни¤ то показываем
			if (m_aTimeSections[m_iSectionCurr].m_DataSection.m_hasThunderbolt && (!main_thunderbolt || (main_thunderbolt && *main_thunderbolt)))
			{
				static float3 campos;
				Core_RFloat3Set(G_RI_FLOAT3_OBSERVER_POSITION, &campos);

				m_vBoltMin = float3_t(campos.x - WEATHER_THUNDERBOLT_WIDTH * 0.5f, m_fLevelMaxY, campos.z - WEATHER_THUNDERBOLT_LENGTH * 0.5f);
				m_vBoltMax = float3_t(campos.x + WEATHER_THUNDERBOLT_WIDTH * 0.5f, m_fLevelMaxY + WEATHER_THUNDERBOLT_HEIGHT, campos.z + WEATHER_THUNDERBOLT_LENGTH * 0.5f);

				float3 tpos = float3(randf(m_vBoltMin.x, m_vBoltMax.x), randf(m_vBoltMin.y, m_vBoltMax.y), randf(m_vBoltMin.z, m_vBoltMax.z));
				SPE_EffectPosSet(m_idEffThunderbolt, &tpos);
				SPE_EffectEnableSet(m_idEffThunderbolt, true);
				SML_LigthsSetPos(m_idLightThunderbolt, &tpos, false);
				SML_LigthsSetEnable(m_idLightThunderbolt, true);
				m_ulTimeBoltLight = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
			}
			else
			{
				SSCore_SndPosCurrSet(m_idSndThunder, 0);
				SSCore_SndVolumeSet(m_idSndThunder, (weather_snd_volume ? (*weather_snd_volume) : 1.f) * 100.f, SOUND_VOL_PCT);

				if (m_isPlaying)
					SSCore_SndPlay(m_idSndThunder);
				else
					SSCore_SndPause(m_idSndThunder);
			}

			m_ulTimeBoltNext = 0;	//обнул¤ем следующее врем¤

			//и устанавливаем прошлому времени грозы текущее значение времени
			m_ulTimeBoltLast = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
			
		}
		//иначе если предыдцщее врем¤ грозы не нулевое и оно уже было достаточно давно чтобы дать возможность генерации следующей грозы
		else if (m_ulTimeBoltLast > 0 && TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_ulTimeBoltLast >= m_aTimeSections[m_iSectionCurr].m_DataSection.m_ulThunderPeriod)
			m_ulTimeBoltLast = 0;

		//если врем¤ света от молнии не нулевое и прошло достаточно времени чтобы выключить свет
		if (m_ulTimeBoltLight > 0 && TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_ulTimeBoltLight > WEATHER_THUNDERBOLT_LIGHT_TIME)
		{
			m_ulTimeBoltLight = 0;
			SML_LigthsSetEnable(m_idLightThunderbolt, false);

			//и заодно проиграть звук молнии
			SSCore_SndPosCurrSet(m_idSndThunder, 0);
			SSCore_SndVolumeSet(m_idSndThunder, clampf(m_fRainVolume*2.f*100.f,0.f,100.f), SOUND_VOL_PCT);
			
			if (m_isPlaying)
				SSCore_SndPlay(m_idSndThunder);
			else
				SSCore_SndPause(m_idSndThunder);
		}
	}
	else
	{
		m_ulTimeBoltLast = 0;
		m_ulTimeBoltNext = 0;
		m_ulTimeBoltLight = 0;
	}
}

void CWeather::updateRainSound()
{
	if (m_idEffRain < 0 || !m_isPlaying)
		return;

	static const float * weather_snd_volume = GET_PCVAR_FLOAT("weather_snd_volume");

	//если пришло врем¤ обновл¤ть громкость
	if (m_ulTimeRainVolSndLast == 0 || TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - m_ulTimeRainVolSndLast >= WEATHER_RAIN_VOL_SND_UPDATE)
		m_ulTimeRainVolSndLast = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	else
		return;

	//если внезапно количество оставл¤ющих след стало больше чем выделено
	if (SPE_EmitterTrackCountGet(m_idEffRain, 0) > m_iTrackPosCount)
	{
		mem_delete(m_aTrackPos);
		m_iTrackPosCount = SPE_EmitterCountGet(m_idEffRain, 0);
		m_aTrackPos = new float3[m_iTrackPosCount];
	}

	//получаем массив следов
	int tmpcount = SPE_EmitterTrackPosGet(m_idEffRain, 0, &m_aTrackPos, m_iTrackPosCount);
	m_fRainVolume = 0;
	float biger = 0.f;

	static float3 campos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &campos);

	for (int i = 0; i < tmpcount; ++i)
	{
		float dist2 = 1.f - SMVector3Distance(campos, m_aTrackPos[i]) / WEATHER_RAIN_VOL_SND_DIST;
		m_fRainVolume += clampf(dist2, 0.f, 1.f);
	}

	m_fRainVolume /= tmpcount / 4;
	SSCore_SndVolumeSet(m_idSndRain, (weather_snd_volume ? (*weather_snd_volume) : 1.f) *  m_fRainVolume * 100.f, SOUND_VOL_PCT);
}

float CWeather::getCurrRainDensity()
{
	if (m_iSectionCurr >= 0 && m_aTimeSections.size() > m_iSectionCurr)
		return m_aTimeSections[m_iSectionCurr].m_DataSection.m_fRainDensity;
	
	return 0.f;
}

void CWeather::sndPlay()
{
	if (m_isPlaying)
		return;

	m_isPlaying = true;
	m_RndSnd.play();

	if (m_idSndRain >= 0 && SSCore_SndStateGet(m_idSndRain) == SOUND_OBJSTATE_PAUSE)
		SSCore_SndPlay(m_idSndRain);

	if (m_idSndThunder >= 0 && SSCore_SndStateGet(m_idSndThunder) == SOUND_OBJSTATE_PAUSE)
		SSCore_SndPlay(m_idSndThunder);
}

void CWeather::sndPause()
{
	if (!m_isPlaying)
		return;

	m_isPlaying = false;
	m_RndSnd.pause();

	if (m_idSndRain >= 0 && SSCore_SndStateGet(m_idSndRain) == SOUND_OBJSTATE_PLAY)
		SSCore_SndPause(m_idSndRain);

	if (m_idSndThunder >= 0 && SSCore_SndStateGet(m_idSndThunder) == SOUND_OBJSTATE_PLAY)
		SSCore_SndPause(m_idSndThunder);
}

bool CWeather::sndGetPlaying()
{
	return m_isPlaying;
}
