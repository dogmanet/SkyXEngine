
#ifndef __WEATHER_H
#define __WEATHER_H

#include <gdefines.h>
#include <GRegisterIndex.h>
#include <common/Array.h>
#include <common/String.h>

#include <time.h>

#include <core/sxcore.h>
#include <score/sxscore.h>
#include <particles/sxparticles.h>
#include <pp/sxpp.h>

#define WEATHER_RAIN_VOL_SND_UPDATE	1000	/*!< промежутки между обновлениями громоксти звука дождя (млсек) */
#define WEATHER_RAIN_VOL_SND_DIST	20.f	/*!< дистанция от наблюдателя, в течении которой будут слышны упавшие капли */
#define WEATHER_RAIN_MIN_Y_OBSERVER	5.f		/*!< на сколько ниже будет минимальная позиция бокса дождя относительно наблюдателя */
#define WEATHER_RAIN_RECREATE_COUNT	500		/*!< количество перерождаемых частиц при полной плотности дождя (должно быть оптимальнымтак как возможно при чрезмерно больших значениях соотношение дождя может быть неправильным) */
#define WEATHER_THUNDERBOLT_LIGHT_TIME	500	/*!< время свечения света при молнии (млсек)*/
#define WEATHER_THUNDERBOLT_WIDTH	100.f	/*!< ширина возможного пространства для молнии */
#define WEATHER_THUNDERBOLT_LENGTH	10.f	/*!< длина возможного пространства для молнии */
#define WEATHER_THUNDERBOLT_HEIGHT	100.f	/*!< высота возможного пространства для молнии */

extern report_func reportf;

//рандомные звуки с 
class CWeatherRndSnd
{
public:
	CWeatherRndSnd();
	~CWeatherRndSnd();

	void clear();
	void resetOld();
	void addSound(const char* path);
	void setParamPlayVolume(int bvol, int evol);
	void setParamPlayPeriod(DWORD bper, DWORD eper);

	void update();

	void play();
	void pause();
	inline bool getPlaying();

protected:

	bool m_isPlaying;

	DWORD m_ulNextPlay;
	
	int m_iVolumeB, m_iVolumeE;
	DWORD m_ulPeriodB, m_ulPeriodE;

	Array<ID> m_aCurrSndIDs;
	ID m_idCurrPlay;

	struct CSnd
	{
		CSnd(){ m_id = -1; }
		CSnd(const char* path, ID id){ m_sPath = path; m_id = id; }
		String m_sPath;
		ID m_id;
	};

	Array<CSnd> m_aArrSnds;
};

//#############################################################################

#define WEATHER_CONFIG_SECTION_KEY(name_key, key, file, section) \
{ \
	if (!name_key || strlen(name_key) != 2 || !isdigit(name_key[0]) || !isdigit(name_key[1])) \
		reportf(REPORT_MSG_LEVEL_ERROR,"%s - unresolved name of key '%s' \nfile '%s' \nsection '%s'",gen_msg_location, key, file, section); \
}
#define WEATHER_CONFIG_LACKS_COMPONENT(component, file, section, key) if(!component) {reportf(REPORT_MSG_LEVEL_ERROR,"%s - lacks component in key '%s' \nfile '%s' \nsection '%s'",gen_msg_location, key, file, section);}
#define WEATHER_CONFIG_LACKS_KEY(key, file, section) {reportf(REPORT_MSG_LEVEL_ERROR,"%s - lacks key '%s' \nfile '%s' \nsection '%s'",gen_msg_location, key, file, section);}

class CWeather
{
public:

	SX_ALIGNED_OP_MEM

	CWeather();
	~CWeather();

	struct CDataSection
	{
		char m_szSkyTex[64];
		float m_fSkyRotation;
		float4_t m_vSkyColor;

		char m_szCloudsTex[64];
		float m_fCloudsRotate;
		float m_fCloudsTransparency;
		float m_fCloudsSpeed;
		float4_t m_vCloudsColor;

		char m_szSunTex[64];
		float2_t m_vSunPos;
		float3_t m_vSunColor;

		float m_fFar;

		float m_fRainDensity;
		float4_t m_vRainColor;

		bool m_hasThunderbolt;
		DWORD m_ulThunderPeriod;

		float3_t m_vFogColor;
		float m_fFogDensity;

		struct CSndRnd
		{
			CSndRnd(){ period_b = period_e = 0; volume_b = volume_e = 0; }
			Array<String> arr_path;
			DWORD period_b, period_e;
			int volume_b, volume_e;
		};

		CSndRnd Snds;
	};

	struct CTimeSection
	{
		CTimeSection(){ m_iTime = 0; m_szSection[0] = 0; }
		CTimeSection(int iTime, const char *szSect){ m_iTime = iTime; if (szSect) strcpy(m_szSection, szSect); }

		int m_iTime;
		char m_szSection[CONFIG_SECTION_MAX_LEN];
		CDataSection m_DataSection;
	};

	void load(const char * szPath);
	void sndPlay();
	void sndPause();
	bool sndGetPlaying();
	void update();
	float getCurrRainDensity();

protected:

	bool m_isPlaying;

	void updateRainSound();
	
	Array<CTimeSection> m_aTimeSections;
	int m_iSectionOld;		//порядковый номер старой секции
	int m_iSectionCurr;		//порядковый номер текущей секции
	DWORD m_ulTimeMlsecOld;		//время при котором произошло обновление погоды (в прошлый раз)
	DWORD m_ulTimeMlsecCurr;	//время при котором произошло обновление погоды (нынче)
	bool m_hasUpdate;			//требуется ли интерпоялция данных между секциями
	CWeatherRndSnd m_RndSnd;	//случайные звуки погоды

	ID m_idEffRain;			//эффект дождя
	ID m_idEffThunderbolt;		//эффект молнии
	ID m_idSndRain;				//звук дождя
	ID m_idSndThunder;			//звук грозы
	ID m_idLightThunderbolt;	//свет от молнии

	DWORD m_ulTimeBoltLast;		//время предыдущей грозы
	DWORD m_ulTimeBoltNext;		//следующее время генерации грозы
	DWORD m_ulTimeBoltLight;	//время появления света от молнии

	float m_fLevelMaxY;			//максимальная высота уровня

	//экстремумы для генерации молнии
	float3_t m_vBoltMin, m_vBoltMax;

	float3* m_aTrackPos;		//выделенный массив для записи позиций партиклов которые оставили след
	int m_iTrackPosCount;		//количество элементов в ArrTrackPos
	DWORD m_ulTimeRainVolSndLast;	//предыдущее время просчетов громкости дождя

	float m_fRainVolume;		//громкость дождя [0,1]
};

#endif