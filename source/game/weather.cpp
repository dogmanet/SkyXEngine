
#include <game/weather.h>

WeatherRndSnd::WeatherRndSnd()
{
	NextPlay = 0;
	BVolume = 50;
	EVolume = 90;

	BPeriod = 1000;
	EPeriod = 10000;

	CurrPlay = -1;
	Playing = false;
}

WeatherRndSnd::~WeatherRndSnd()
{
	Clear();
}

void WeatherRndSnd::Clear()
{
	ResetOld();

	for (int i = 0; i < ArrSnd.size(); ++i)
	{
		if (ArrSnd[i].id >= 0)
		{
			SSCore_SndStop(ArrSnd[i].id);
			SSCore_SndDelete(ArrSnd[i].id);
		}
	}
	ArrSnd.clear();
}

void WeatherRndSnd::ResetOld()
{
	for (int i = 0; i < CurrentSndIDs.size(); ++i)
	{
		if (CurrentSndIDs[i] >= 0)
			SSCore_SndStop(CurrentSndIDs[i]);
	}
	CurrentSndIDs.clear();
}

void WeatherRndSnd::SetParamPlayVolume(int bvol, int evol)
{
	BVolume = bvol;
	EVolume = evol;
}

void WeatherRndSnd::SetParamPlayPeriod(DWORD bper, DWORD eper)
{
	BPeriod = bper;
	EPeriod = eper;
}

void WeatherRndSnd::AddSound(const char* path)
{
	bool isunic = true;
	ID tmpid = -1;
	for (int i = 0; i<ArrSnd.size(); i++)
	{
		if (stricmp(ArrSnd[i].path.c_str(), path) == 0)
		{
			isunic = false;
			tmpid = ArrSnd[i].id;
			break;
		}
	}

	if (isunic)
	{
		ID tmpid = SSCore_SndCreate2d(path);
		ArrSnd.push_back(Snd(path, tmpid));
		CurrentSndIDs.push_back(tmpid);
	}
	else
	{
		CurrentSndIDs.push_back(tmpid);
	}
}

void WeatherRndSnd::Update()
{
	if (!Playing)
		return;

	static const float * weather_snd_volume = GET_PCVAR_FLOAT("weather_snd_volume");

	if (CurrentSndIDs.size() > 0 && (NextPlay == 0 || NextPlay < TimeGetMls(G_Timer_Render_Scene)))
	{
		int tmpkeysnd = rand() % (CurrentSndIDs.size());
		if (CurrentSndIDs.size() > tmpkeysnd && CurrentSndIDs[tmpkeysnd] && CurrentSndIDs[tmpkeysnd] >= 0)
		{
			CurrPlay = CurrentSndIDs[tmpkeysnd];
			SSCore_SndPosCurrSet(CurrPlay, 0);
			int tmprndvol = (rand() % (EVolume - BVolume)) + BVolume;
			SSCore_SndVolumeSet(CurrPlay, (weather_snd_volume ? (float)tmprndvol * (*weather_snd_volume) : tmprndvol), SOUND_VOL_PCT);
			SSCore_SndPlay(CurrPlay);

			DWORD tmprnd = (rand() % (EPeriod - BPeriod)) + BPeriod;
			NextPlay = TimeGetMls(G_Timer_Render_Scene) + tmprnd + ((SSCore_SndLengthSecGet(CurrPlay) + 1) * 1000);
		}
	}
}

void WeatherRndSnd::Play()
{
	if (Playing)
		return;

	Playing = true;

	if (CurrPlay >= 0 && SSCore_SndStateGet(CurrPlay) == SoundObjState::sos_pause)
		SSCore_SndPlay(CurrPlay);
}

void WeatherRndSnd::Pause()
{
	if (!Playing)
		return;

	Playing = false;

	if (CurrPlay >= 0 && SSCore_SndStateGet(CurrPlay) == SoundObjState::sos_play)
		SSCore_SndPlay(sos_pause);
}

bool WeatherRndSnd::IsPlaying()
{
	return Playing;
}

//#############################################################################

Weather::Weather()
{
	Playing = false;
	OldSection = -1;
	CurrSection = -1;
	HasUpdate = false;
	TimeRainVolSndLast = 0;
	OldTimeMlsec = CurrTimeMlsec = 0;
	LevelMaxY = 20.f;

	EffIDRain = SPE_EffectGetByName("rain");

	CountArrTrackPos = 0;
	ArrTrackPos = 0;
	if (EffIDRain >= 0)
	{
		CountArrTrackPos = SPE_EmitterCountGet(EffIDRain,0);
		ArrTrackPos = new float3[CountArrTrackPos];
	}
	else
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect 'rain'", gen_msg_location);
	}

	EffThunderbolt = SPE_EffectGetByName("thunderbolt");
	if (EffThunderbolt < 0)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect 'thunderbolt'", gen_msg_location);
	}
	TimeBoltNext = TimeBoltLast = 0;

	LightThunderbolt = SML_LigthsCreatePoint(&float3(0, 0, 0), 200, &float3(1, 1, 1), false, true);
	SML_LigthsSetEnable(LightThunderbolt, false);
	SndRain = SSCore_SndCreate2d("nature\\rain.ogg",true);
	SndThunder = SSCore_SndCreate2d("nature\\thunder.ogg");

	RainVolume = 0;
}

Weather::~Weather()
{
	ArrTimeSection.clear();
	SML_LigthsDeleteLight(LightThunderbolt);
	SSCore_SndDelete(SndRain);
	SSCore_SndDelete(SndThunder);
}

void Weather::Load(const char* file)
{
	if (file == 0)
	{
		ArrTimeSection.clear();
		OldSection = CurrSection = -1;
		OldTimeMlsec = CurrTimeMlsec = 0;
		HasUpdate = false;
		Playing = false;
		RndSnd.Pause();
		RndSnd.ResetOld();

		SGCore_SkyBoxLoadTex(0);
		SGCore_SkyBoxSetRot(0);
		SGCore_SkyCloudsLoadTex(0);
		SGCore_SkyCloudsSetSpeed(0);
		SGCore_SkyCloudsSetRot(0);

		
		SPE_EffectEnableSet(EffIDRain, false);
		SSCore_SndStop(SndRain);

		SPE_EffectEnableSet(EffThunderbolt, false);
		SML_LigthsSetEnable(LightThunderbolt, false);
		SSCore_SndStop(SndThunder);

		return;
	}

	ISXLConfig* config = Core_OpLConfig(file);

	if (!config->SectionExists("sections"))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found section 'sections' \nfile '%s'", gen_msg_location, file);
		mem_release_del(config);
		return;
	}

	int key_count = config->GetKeyCount("sections");

	for (int i = 0; i < key_count; ++i)
	{
		int time = 0;
		const char* str = config->GetKeyName("sections", i);

		if (strlen(str) != 8)
		{
			reportf(REPORT_MSG_LEVEL_ERROR, "%s - unresolved name of key '%s' \nfile '%s' \nsection '%s'", gen_msg_location, str, file, "sections");
			mem_release_del(config);
			return;
		}

		char tmpall[32];
		sprintf(tmpall, str);

		char* tmpstr = strtok(tmpall, ":");
		WEATHER_CONFIG_SECTION_KEY(tmpstr, str, file, "sections");
		time = atoi(tmpstr) * 10000;

		tmpstr = strtok(NULL, ":");
		WEATHER_CONFIG_SECTION_KEY(tmpstr, str, file, "sections");
		time += atoi(tmpstr) * 100;

		tmpstr = strtok(NULL, ":");
		WEATHER_CONFIG_SECTION_KEY(tmpstr, str, file, "sections");
		time += atoi(tmpstr);

		ArrTimeSection.push_back(TimeSection(time, config->GetKey("sections", str)));

		int qwerty = 0;
	}

	Array<TimeSection> tmpts = ArrTimeSection;
	ArrTimeSection.clear();
	for (int i = 0, il = tmpts.size(); i < il; ++i)
	{
		int num = 0;
		for (int k = 0, kl = tmpts.size(); k < kl; ++k)
		{
			if (i != k && tmpts[i].time > tmpts[k].time)
				++num;
		}

		ArrTimeSection[num] = tmpts[i];
	}

	tmpts.clear();

	for (int i = 0, il = ArrTimeSection.size(); i < il; ++i)
	{
		//reportf(0, "%d\n", ArrTimeSection[i].time);

		if (config->KeyExists(ArrTimeSection[i].section, "sky_texture"))
			sprintf(ArrTimeSection[i].Data.sky_tex, "%s", config->GetKey(ArrTimeSection[i].section, "sky_texture"));
		else
			WEATHER_CONFIG_LACKS_KEY("sky_texture", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "clouds_texture"))
			sprintf(ArrTimeSection[i].Data.clouds_tex, "%s", config->GetKey(ArrTimeSection[i].section, "clouds_texture"));
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_texture", file, ArrTimeSection[i].section);


		if (config->KeyExists(ArrTimeSection[i].section, "sun_texture"))
			sprintf(ArrTimeSection[i].Data.sun_tex, "%s", config->GetKey(ArrTimeSection[i].section, "sun_texture"));
		else
			WEATHER_CONFIG_LACKS_KEY("sun_texture", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "sun_pos"))
		{
			const char* text_sp = config->GetKey(ArrTimeSection[i].section, "sun_pos");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_pos", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sun_pos.x = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_pos", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sun_pos.y = atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("sun_pos", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "sky_rotation"))
			ArrTimeSection[i].Data.sky_rotation = String(config->GetKey(ArrTimeSection[i].section, "sky_rotation")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("sky_rotation", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "sky_color"))
		{
			const char* text_sp = config->GetKey(ArrTimeSection[i].section, "sky_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sky_color.x = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sky_color.y = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sky_color.z = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sky_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sky_color.w = atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("sky_color", file, ArrTimeSection[i].section);


		if (config->KeyExists(ArrTimeSection[i].section, "clouds_color"))
		{
			const char* text_sp = config->GetKey(ArrTimeSection[i].section, "clouds_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.clouds_color.x = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.clouds_color.y = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.clouds_color.z = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "clouds_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.clouds_color.w = atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_color", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "clouds_rotate"))
			ArrTimeSection[i].Data.clouds_rotate = String(config->GetKey(ArrTimeSection[i].section, "clouds_rotate")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_rotate", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "clouds_transparency"))
			ArrTimeSection[i].Data.clouds_transparency = String(config->GetKey(ArrTimeSection[i].section, "clouds_transparency")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_transparency", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "clouds_speed"))
			ArrTimeSection[i].Data.clouds_speed = String(config->GetKey(ArrTimeSection[i].section, "clouds_speed")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("clouds_speed", file, ArrTimeSection[i].section);


		if (config->KeyExists(ArrTimeSection[i].section, "sun_color"))
		{
			const char* text_sp = config->GetKey(ArrTimeSection[i].section, "sun_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sun_color.x = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sun_color.y = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sun_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.sun_color.z = atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("sun_color", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "far"))
			ArrTimeSection[i].Data.ffar = String(config->GetKey(ArrTimeSection[i].section, "far")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("far", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "rain_density"))
			ArrTimeSection[i].Data.rain_density = String(config->GetKey(ArrTimeSection[i].section, "rain_density")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("rain_density", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "rain_color"))
		{
			const char* text_sp = config->GetKey(ArrTimeSection[i].section, "rain_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.rain_color.x = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.rain_color.y = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.rain_color.z = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "rain_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.rain_color.w = atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("rain_color", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "fog_density"))
			ArrTimeSection[i].Data.fog_density = String(config->GetKey(ArrTimeSection[i].section, "fog_density")).ToDouble();
		else
			WEATHER_CONFIG_LACKS_KEY("fog_density", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "fog_color"))
		{
			const char* text_sp = config->GetKey(ArrTimeSection[i].section, "fog_color");
			char text_sp2[64];
			strcpy(text_sp2, text_sp);

			char* tmpstr = strtok(text_sp2, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "fog_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.fog_color.x = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "fog_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.fog_color.y = atof(tmpstr);

			tmpstr = strtok(NULL, ",");
			WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "fog_color", file, ArrTimeSection[i].section);
			ArrTimeSection[i].Data.fog_color.z = atof(tmpstr);
		}
		else
			WEATHER_CONFIG_LACKS_KEY("fog_color", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "thunderbolt"))
			ArrTimeSection[i].Data.thunderbolt = String(config->GetKey(ArrTimeSection[i].section, "thunderbolt")).ToBool();
		else
			WEATHER_CONFIG_LACKS_KEY("thunderbolt", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "thunder_period"))
			ArrTimeSection[i].Data.thunder_period = String(config->GetKey(ArrTimeSection[i].section, "thunder_period")).ToUnsLongInt();
		else
			WEATHER_CONFIG_LACKS_KEY("thunder_period", file, ArrTimeSection[i].section);

		if (config->KeyExists(ArrTimeSection[i].section, "env_ambient"))
		{
			const char* text_env = config->GetKey(ArrTimeSection[i].section, "env_ambient");

			if (!config->SectionExists(text_env))
			{
				reportf(REPORT_MSG_LEVEL_ERROR, "%s - lacks env_ambient section '%s' \nfile '%s' \nsection '%s'", gen_msg_location, text_env, file, ArrTimeSection[i].section);
				return;
			}

			if (config->KeyExists(text_env, "period"))
			{
				const char* text_sp = config->GetKey(text_env, "period");
				char text_sp2[64];
				strcpy(text_sp2, text_sp);

				char* tmpstr = strtok(text_sp2, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "period", file, text_env);
				ArrTimeSection[i].Data.Snds.period_b = atol(tmpstr);

				tmpstr = strtok(NULL, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "period", file, text_env);
				ArrTimeSection[i].Data.Snds.period_e = atol(tmpstr);
			}
			else
				WEATHER_CONFIG_LACKS_KEY("period", file, text_env);

			if (config->KeyExists(text_env, "volume"))
			{
				const char* text_sp = config->GetKey(text_env, "volume");
				char text_sp2[64];
				strcpy(text_sp2, text_sp);

				char* tmpstr = strtok(text_sp2, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "volume", file, text_env);
				ArrTimeSection[i].Data.Snds.volume_b = atol(tmpstr);

				tmpstr = strtok(NULL, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "volume", file, text_env);
				ArrTimeSection[i].Data.Snds.volume_e = atol(tmpstr);
			}
			else
				WEATHER_CONFIG_LACKS_KEY("volume", file, text_env);

			if (config->KeyExists(text_env, "sounds"))
			{
				const char* text_sp = config->GetKey(text_env, "sounds");
				char text_sp2[4096];
				strcpy(text_sp2, text_sp);

				char* tmpstr = strtok(text_sp2, ",");
				WEATHER_CONFIG_LACKS_COMPONENT(tmpstr, "sounds", file, text_env);
				ArrTimeSection[i].Data.Snds.arr_path.push_back(tmpstr);

				while (tmpstr != NULL)
				{
					tmpstr = strtok(NULL, " ,");

					if (tmpstr)
						ArrTimeSection[i].Data.Snds.arr_path.push_back(tmpstr);
				}

			}
			else
				WEATHER_CONFIG_LACKS_KEY("sounds", file, text_env);
		}
	}

	mem_release_del(config);
}

void Weather::Update()
{
	if (Playing)
		RndSnd.Update();

	ID gid = SML_LigthsGetGlobal();

	static const float * weather_snd_volume = GET_PCVAR_FLOAT("weather_snd_volume");

	//если есть дождь то обновл€ем его позицию и громкость
	if (CurrSection >= 0 && ArrTimeSection.size() > CurrSection && ArrTimeSection[CurrSection].Data.rain_density > 0.f)
	{
		static float3 campos;
		Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &campos);
		SPE_EffectPosSet(EffIDRain, &float3(campos.x, campos.y - WEATHER_RAIN_MIN_Y_OBSERVER, campos.z));
		UpdateRainSound();
	}

	static float * main_rain_density = (float*)GET_PCVAR_FLOAT("main_rain_density");
	static float main_rain_density_old = 1.f;

	if (main_rain_density && (*main_rain_density) != main_rain_density_old)
	{
		main_rain_density_old = *main_rain_density;
		if (ArrTimeSection[CurrSection].Data.rain_density > 0.f)
			SPE_EmitterSet(EffIDRain, 0, ReCreateCount, main_rain_density_old * ArrTimeSection[CurrSection].Data.rain_density * float(WEATHER_RAIN_RECREATE_COUNT));
	}

	//получаем текущую игровую дату
	tm g_tm;
	time_t g_time = Core_TimeUnixCurrGet(G_Timer_Game);
	localtime_s(&g_tm, &g_time);

	//цифровое представление времени суток
	int ltime = g_tm.tm_hour * 10000 + g_tm.tm_min * 100 + g_tm.tm_sec;
	
	//представление времени суток в млсекундах
	DWORD ltime_mlsec = (g_tm.tm_sec * 1000) + (g_tm.tm_min * 60 * 1000) + (g_tm.tm_hour * 60 * 60 * 1000);
	int curr_section = -1;

	//находим секцию дл€ текущего времени суток
	for (int i = 0, il = ArrTimeSection.size(); i < il; ++i)
	{
		if (ltime >= ArrTimeSection[i].time)
			curr_section = i;
	}

	//если только что обновл€емс€ либо сменилась секци€
	if (curr_section >= 0 && curr_section != CurrSection)
	{
		OldSection = CurrSection;
		OldTimeMlsec = CurrTimeMlsec;
		if (OldSection < 0)
		{
			OldSection = curr_section;
			OldTimeMlsec = ltime_mlsec;
		}
		
		CurrSection = curr_section;
		CurrTimeMlsec = ltime_mlsec;

		SGCore_SkyBoxChangeTex(ArrTimeSection[CurrSection].Data.sky_tex);
		SGCore_SkyBoxSetRot(ArrTimeSection[CurrSection].Data.sky_rotation);
		SGCore_SkyCloudsChangeTex(ArrTimeSection[CurrSection].Data.clouds_tex);
		SGCore_SkyCloudsSetSpeed(ArrTimeSection[CurrSection].Data.clouds_speed);
		SGCore_SkyCloudsSetRot(ArrTimeSection[CurrSection].Data.clouds_rotate);
		
		if (ArrTimeSection[CurrSection].Data.sun_tex[0] != '0')
			SPP_ChangeTexSun(ArrTimeSection[CurrSection].Data.sun_tex);
		
		if (gid >= 0)
			SML_LigthsSetEnable(gid, ArrTimeSection[CurrSection].Data.sun_tex[0] != '0');

		HasUpdate = true;

		//если плотность дожд€ больше нул€ тогда включаем дождь
		if (ArrTimeSection[CurrSection].Data.rain_density > 0.f)
		{
			SPE_EmitterSet(EffIDRain, 0, Color, ArrTimeSection[CurrSection].Data.rain_color);
			SPE_EmitterSet(EffIDRain, 0, ReCreateCount, main_rain_density_old * ArrTimeSection[CurrSection].Data.rain_density * float(WEATHER_RAIN_RECREATE_COUNT));
			SPE_EffectEnableSet(EffIDRain, true);

			SSCore_SndPosCurrSet(SndRain, 0);
			SSCore_SndVolumeSet(SndRain, 0, SOUND_VOL_PCT);

			if (Playing)
				SSCore_SndPlay(SndRain);
			else
				SSCore_SndPause(SndRain);
		}
		//иначе выключаем
		else
		{
			SPE_EffectEnableSet(EffIDRain, false);
			SSCore_SndStop(SndRain);
		}

		//очищаем старые звуки
		RndSnd.ResetOld();

		//если есть пути до новых случайных звуков
		if (ArrTimeSection[CurrSection].Data.Snds.arr_path.size() > 0)
		{
			//загружаем эти звуки
			DataSection::SndRnd Snds = ArrTimeSection[CurrSection].Data.Snds;

			for (int i = 0, il = Snds.arr_path.size(); i < il; ++i)
			{
				RndSnd.AddSound(Snds.arr_path[i].c_str());
				RndSnd.SetParamPlayPeriod(Snds.period_b, Snds.period_e);
				RndSnd.SetParamPlayVolume(Snds.volume_b, Snds.volume_e);
			}
		}
	}

	
	
	if (HasUpdate)
	{
		//расчет коэфициента интерпол€ции
		float lerp_factor = 1.f;
			
		if (CurrTimeMlsec - OldTimeMlsec > 0)
			lerp_factor = float(ltime_mlsec - CurrTimeMlsec) / float(CurrTimeMlsec - OldTimeMlsec);

		//если коэфициент интерпол€ции больше либо равен 1 то значит интерпол€ци€ больше не нужна на текущей секции
		if (lerp_factor >= 1.f)
			HasUpdate = false;

		//цвет скайбокса
		float4_t tmp_sky_color;
		tmp_sky_color.x = lerpf(ArrTimeSection[OldSection].Data.sky_color.x, ArrTimeSection[CurrSection].Data.sky_color.x, lerp_factor);
		tmp_sky_color.y = lerpf(ArrTimeSection[OldSection].Data.sky_color.y, ArrTimeSection[CurrSection].Data.sky_color.y, lerp_factor);
		tmp_sky_color.z = lerpf(ArrTimeSection[OldSection].Data.sky_color.z, ArrTimeSection[CurrSection].Data.sky_color.z, lerp_factor);
		tmp_sky_color.w = lerpf(ArrTimeSection[OldSection].Data.sky_color.w, ArrTimeSection[CurrSection].Data.sky_color.w, lerp_factor);
		SGCore_SkyBoxSetColor(&tmp_sky_color);

		//цвет облаков
		float4_t tmp_clouds_color;
		tmp_clouds_color.x = lerpf(ArrTimeSection[OldSection].Data.clouds_color.x, ArrTimeSection[CurrSection].Data.clouds_color.x, lerp_factor);
		tmp_clouds_color.y = lerpf(ArrTimeSection[OldSection].Data.clouds_color.y, ArrTimeSection[CurrSection].Data.clouds_color.y, lerp_factor);
		tmp_clouds_color.z = lerpf(ArrTimeSection[OldSection].Data.clouds_color.z, ArrTimeSection[CurrSection].Data.clouds_color.z, lerp_factor);
		tmp_clouds_color.w = lerpf(ArrTimeSection[OldSection].Data.clouds_color.w, ArrTimeSection[CurrSection].Data.clouds_color.w, lerp_factor);
		SGCore_SkyCloudsSetColor(&tmp_sky_color);

		//прозрачность облаков
		float tmp_clouds_transparency = lerpf(ArrTimeSection[OldSection].Data.clouds_transparency, ArrTimeSection[CurrSection].Data.clouds_transparency, lerp_factor);
		SGCore_SkyCloudsSetAlpha(tmp_clouds_transparency);

		if (gid >= 0)
		{
			//цвет солнца
			float3 tmp_scolor;
			tmp_scolor.x = lerpf(ArrTimeSection[OldSection].Data.sun_color.x, ArrTimeSection[CurrSection].Data.sun_color.x, lerp_factor);
			tmp_scolor.y = lerpf(ArrTimeSection[OldSection].Data.sun_color.y, ArrTimeSection[CurrSection].Data.sun_color.y, lerp_factor);
			tmp_scolor.z = lerpf(ArrTimeSection[OldSection].Data.sun_color.z, ArrTimeSection[CurrSection].Data.sun_color.z, lerp_factor);
			SML_LigthsSetColor(gid, &tmp_scolor);

			//позици€ солнца
			float3 tmp_spos;
			tmp_spos.x = lerpf(ArrTimeSection[OldSection].Data.sun_pos.x, ArrTimeSection[CurrSection].Data.sun_pos.x, lerp_factor);
			tmp_spos.y = lerpf(ArrTimeSection[OldSection].Data.sun_pos.y, ArrTimeSection[CurrSection].Data.sun_pos.y, lerp_factor);
			tmp_spos.z = 0;
			SML_LigthsSetPos(gid, &tmp_spos, false);
		}

		//дальность видимости
		float tmpfar = lerpf(ArrTimeSection[OldSection].Data.ffar, ArrTimeSection[CurrSection].Data.ffar, lerp_factor);
		Core_0SetCVarFloat("p_far", tmpfar);

		//плотность тумана
		static float * pp_fog_density = (float*)GET_PCVAR_FLOAT("pp_fog_density");
		if (pp_fog_density)
			*pp_fog_density = lerpf(ArrTimeSection[OldSection].Data.fog_density, ArrTimeSection[CurrSection].Data.fog_density, lerp_factor);

		//цвет тумана
		static const int * e_pp_fog_color = GET_PCVAR_INT("e_pp_fog_color");
		if (e_pp_fog_color && *e_pp_fog_color)
		{
			float3_t* tmp_fog_color2 = (float3_t*)(*e_pp_fog_color);
			tmp_fog_color2->x = lerpf(ArrTimeSection[OldSection].Data.fog_color.x, ArrTimeSection[CurrSection].Data.fog_color.x, lerp_factor);
			tmp_fog_color2->y = lerpf(ArrTimeSection[OldSection].Data.fog_color.y, ArrTimeSection[CurrSection].Data.fog_color.y, lerp_factor);
			tmp_fog_color2->z = lerpf(ArrTimeSection[OldSection].Data.fog_color.z, ArrTimeSection[CurrSection].Data.fog_color.z, lerp_factor);
		}
		else
			reportf(REPORT_MSG_LEVEL_WARRNING, "cvar e_pp_fog_color is not init");
	}

	//если в текущей секции есть частота молнии
	if (CurrSection >= 0 && ArrTimeSection[CurrSection].Data.thunder_period > 0)
	{
		//если следующее врем€ грозы и предыдущее нулевые, тогда генерируем следующее врем€
		if (TimeBoltNext == 0 && TimeBoltLast == 0)
			TimeBoltNext = TimeGetMls(G_Timer_Render_Scene) + (rand() % ArrTimeSection[CurrSection].Data.thunder_period);
		//иначе если предыдущее врем€ грозы нулевое и врем€ следующей грозы наступило
		else if (TimeBoltLast == 0 && TimeGetMls(G_Timer_Render_Scene) >= TimeBoltNext)
		{
			static const bool * main_thunderbolt = GET_PCVAR_BOOL("main_thunderbolt");
			//если предусмотерна молни€ то показываем
			if (ArrTimeSection[CurrSection].Data.thunderbolt && (!main_thunderbolt || (main_thunderbolt && *main_thunderbolt)))
			{
				static float3 campos;
				Core_RFloat3Set(G_RI_FLOAT3_OBSERVER_POSITION, &campos);

				BoltMin = float3_t(campos.x - WEATHER_THUNDERBOLT_WIDTH * 0.5f, LevelMaxY, campos.z - WEATHER_THUNDERBOLT_LENGTH * 0.5f);
				BoltMax = float3_t(campos.x + WEATHER_THUNDERBOLT_WIDTH * 0.5f, LevelMaxY + WEATHER_THUNDERBOLT_HEIGHT, campos.z + WEATHER_THUNDERBOLT_LENGTH * 0.5f);

				float3 tpos = float3(randf(BoltMin.x, BoltMax.x), randf(BoltMin.y, BoltMax.y), randf(BoltMin.z, BoltMax.z));
				SPE_EffectPosSet(EffThunderbolt, &tpos);
				SPE_EffectEnableSet(EffThunderbolt, true);
				SML_LigthsSetPos(LightThunderbolt, &tpos, false);
				SML_LigthsSetEnable(LightThunderbolt, true);
				TimeBoltLight = TimeGetMls(G_Timer_Render_Scene);
			}
			else
			{
				SSCore_SndPosCurrSet(SndThunder, 0);
				SSCore_SndVolumeSet(SndThunder, (weather_snd_volume ? (*weather_snd_volume) : 1.f) * 100.f, SOUND_VOL_PCT);

				if (Playing)
					SSCore_SndPlay(SndThunder);
				else
					SSCore_SndPause(SndThunder);
			}

			TimeBoltNext = 0;	//обнул€ем следующее врем€

			//и устанавливаем прошлому времени грозы текущее значение времени
			TimeBoltLast = TimeGetMls(G_Timer_Render_Scene);
			
		}
		//иначе если предыдцщее врем€ грозы не нулевое и оно уже было достаточно давно чтобы дать возможность генерации следующей грозы
		else if (TimeBoltLast > 0 && TimeGetMls(G_Timer_Render_Scene) - TimeBoltLast >= ArrTimeSection[CurrSection].Data.thunder_period)
			TimeBoltLast = 0;

		//если врем€ света от молнии не нулевое и прошло достаточно времени чтобы выключить свет
		if (TimeBoltLight > 0 && TimeGetMls(G_Timer_Render_Scene) - TimeBoltLight > WEATHER_THUNDERBOLT_LIGHT_TIME)
		{
			TimeBoltLight = 0;
			SML_LigthsSetEnable(LightThunderbolt, false);

			//и заодно проиграть звук молнии
			SSCore_SndPosCurrSet(SndThunder, 0);
			SSCore_SndVolumeSet(SndThunder, clampf(RainVolume*2.f*100.f,0.f,100.f), SOUND_VOL_PCT);
			
			if (Playing)
				SSCore_SndPlay(SndThunder);
			else
				SSCore_SndPause(SndThunder);
		}
	}
	else
	{
		TimeBoltLast = 0;
		TimeBoltNext = 0;
		TimeBoltLight = 0;
	}
}

void Weather::UpdateRainSound()
{
	if (EffIDRain < 0 || !Playing)
		return;

	static const float * weather_snd_volume = GET_PCVAR_FLOAT("weather_snd_volume");

	//если пришло врем€ обновл€ть громкость
	if (TimeRainVolSndLast == 0 || TimeGetMls(G_Timer_Render_Scene) - TimeRainVolSndLast >= WEATHER_RAIN_VOL_SND_UPDATE)
		TimeRainVolSndLast = TimeGetMls(G_Timer_Render_Scene);
	else
		return;

	//если внезапно количество оставл€ющих след стало больше чем выделено
	if (SPE_EmitterTrackCountGet(EffIDRain, 0) > CountArrTrackPos)
	{
		mem_delete(ArrTrackPos);
		CountArrTrackPos = SPE_EmitterCountGet(EffIDRain, 0);
		ArrTrackPos = new float3[CountArrTrackPos];
	}

	//получаем массив следов
	int tmpcount = SPE_EmitterTrackPosGet(EffIDRain, 0, &ArrTrackPos, CountArrTrackPos);
	RainVolume = 0;
	float biger = 0.f;

	static float3 campos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &campos);

	for (int i = 0; i < tmpcount; ++i)
	{
		float dist2 = 1.f - SMVector3Distance(campos, ArrTrackPos[i]) / WEATHER_RAIN_VOL_SND_DIST;
		RainVolume += clampf(dist2, 0.f, 1.f);
	}

	RainVolume /= tmpcount / 4;
	SSCore_SndVolumeSet(SndRain, (weather_snd_volume ? (*weather_snd_volume) : 1.f) *  RainVolume * 100.f, SOUND_VOL_PCT);
}

float Weather::GetCurrRainDensity()
{
	if (CurrSection >= 0 && ArrTimeSection.size() > CurrSection)
		return ArrTimeSection[CurrSection].Data.rain_density;
	
	return 0.f;
}

void Weather::SndPlay()
{
	if (Playing)
		return;

	Playing = true;
	RndSnd.Play();

	if (SndRain >= 0 && SSCore_SndStateGet(SndRain) == SoundObjState::sos_pause)
		SSCore_SndPlay(SndRain);

	if (SndThunder >= 0 && SSCore_SndStateGet(SndThunder) == SoundObjState::sos_pause)
		SSCore_SndPlay(SndThunder);
}

void Weather::SndPause()
{
	if (!Playing)
		return;

	Playing = false;
	RndSnd.Pause();

	if (SndRain >= 0 && SSCore_SndStateGet(SndRain) == SoundObjState::sos_play)
		SSCore_SndPause(SndRain);

	if (SndThunder >= 0 && SSCore_SndStateGet(SndThunder) == SoundObjState::sos_play)
		SSCore_SndPause(SndThunder);
}

bool Weather::SndIsPlaying()
{
	return Playing;
}