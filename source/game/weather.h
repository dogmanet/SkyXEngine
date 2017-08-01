
#ifndef __weather_h
#define __weather_h

//рандомные звуки с 
class WeatherRndSnd
{
public:
	WeatherRndSnd();
	~WeatherRndSnd();

	void Clear();
	void ResetOld();
	void AddSound(const char* path);
	void SetParamPlayVolume(int bvol, int evol);
	void SetParamPlayPeriod(DWORD bper, DWORD eper);

	void Update();

	void Play();
	void Pause();
	inline bool IsPlaying();

protected:

	bool Playing;

	DWORD NextPlay;
	
	int BVolume, EVolume;
	DWORD BPeriod, EPeriod;

	Array<ID> CurrentSndIDs;
	ID CurrPlay;

	struct Snd
	{
		Snd(){ id = -1; }
		Snd(const char* _path, ID _id){ path = _path; id = _id; }
		String path;
		ID id;
	};

	Array<Snd> ArrSnd;
};

//#############################################################################

#define WEATHER_CONFIG_SECTION_KEY(name_key, key, file, section) \
{ \
	if (!name_key || strlen(name_key) != 2 || !isdigit(name_key[0]) || !isdigit(name_key[1])) \
		reportf(REPORT_MSG_LEVEL_ERROR,"%s - unresolved name of key '%s' \nfile '%s' \nsection '%s'",gen_msg_location, key, file, section); \
}
#define WEATHER_CONFIG_LACKS_COMPONENT(component, file, section, key) if(!component) {reportf(REPORT_MSG_LEVEL_ERROR,"%s - lacks component in key '%s' \nfile '%s' \nsection '%s'",gen_msg_location, key, file, section);}
#define WEATHER_CONFIG_LACKS_KEY(key, file, section) {reportf(REPORT_MSG_LEVEL_ERROR,"%s - lacks key '%s' \nfile '%s' \nsection '%s'",gen_msg_location, key, file, section);}

class Weather
{
public:
	Weather();
	~Weather();

	struct DataSection
	{
		char sky_tex[64];
		float sky_rotation;
		float4_t sky_color;

		char clouds_tex[64];
		float clouds_rotate;
		float clouds_transparency;
		float clouds_speed;
		float4_t clouds_color;

		char sun_tex[64];
		float2_t sun_pos;
		float3_t sun_color;

		float ffar;

		float rain_density;
		float4_t rain_color;

		bool thunderbolt;
		DWORD thunder_period;

		float3_t fog_color;
		float fog_density;

		struct SndRnd
		{
			SndRnd(){ period_b = period_e = 0; volume_b = volume_e = 0; }
			Array<String> arr_path;
			DWORD period_b, period_e;
			int volume_b, volume_e;
		};

		SndRnd Snds;
	};

	struct TimeSection
	{
		TimeSection(){ time = 0; section[0] = 0; }
		TimeSection(int _time, const char* sect){ time = _time; if (sect) strcpy(section, sect); }

		int time;
		char section[CONFIG_SECTION_MAX_LEN];
		DataSection Data;
	};

	void Load(const char* file);
	void SndPlay();
	void SndPause();
	inline bool SndIsPlaying();
	void Update();
	float GetCurrRainDensity();

protected:

	bool Playing;

	void UpdateRainSound();
	
	Array<TimeSection> ArrTimeSection;
	int OldSection;			//порядковый номер старой секции
	int CurrSection;		//порядковый номер текущей секции
	DWORD OldTimeMlsec;		//время при котором произошло обновление погоды (в прошлый раз)
	DWORD CurrTimeMlsec;	//время при котором произошло обновление погоды (нынче)
	bool HasUpdate;			//требуется ли интерпоялция данных между секциями
	WeatherRndSnd RndSnd;	//случайные звуки погоды

	ID EffIDRain;			//эффект дождя
	ID EffThunderbolt;		//эффект молнии
	ID SndRain;				//звук дождя
	ID SndThunder;			//звук грозы
	ID LightThunderbolt;	//свет от молнии

	DWORD TimeBoltLast;		//время предыдущей грозы
	DWORD TimeBoltNext;		//следующее время генерации грозы
	DWORD TimeBoltLight;	//время появления света от молнии

	float LevelMaxY;		//максимальная высота уровня

	//экстремумы для генерации молнии
	float3_t BoltMin, BoltMax;

	float3* ArrTrackPos;		//выделенный массив для записи позиций партиклов которые оставили след
	int CountArrTrackPos;		//количество элементов в ArrTrackPos
	DWORD TimeRainVolSndLast;	//предыдущее время просчетов громкости дождя

	float RainVolume;		//громкость дождя [0,1]
};

#endif