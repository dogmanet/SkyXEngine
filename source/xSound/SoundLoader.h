/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDLOADER_H
#define __SOUNDLOADER_H

#include <common/string.h>

#include "SoundTypes.h"

//##########################################################################

//! количество частей буфера потоковой загрузки
#define STREAM_DATA_COUNT_PARTS 4

//! минимальное количество частей для потоковой загрузки, чтобы была выбрана потоковая загрузка
#define STREAM_COUNT_MIN_PARTS 100

//! структура данных для потоковой загрузки
struct CStreamData
{
	//! порядковый номер текущей загрузки частей (для определения позиции в звуке)
	uint32_t uNumLoader = 0;

	//! размер одной части в байтах
	uint32_t uPartSize = 0;

	//! структура части
	struct CPart
	{
		uint32_t uStart = 0;		//!< стартовая позиция
		uint32_t uEnd = 0;			//!< конечная позиция
		bool isPlay = false;		//!< воспроизводится ли сейчас часть
		bool isLoaded = false;	//!< загружена ли эта часть
	};

	//! массив частей
	CPart aParts[STREAM_DATA_COUNT_PARTS];

	struct CRawData
	{
		BYTE *pData = 0;
		uint32_t uSize = 0;
	} oData;
};

//##########################################################################

class CSoundLoader
{
public:
	~CSoundLoader(){}

	void init(CSoundSystem *pSoundSystem);

	/*! загружает звук через подходящий декодер
	*/
	bool load(const char *szPath, AudioRawDesc *pDesc);
	size_t getPCM(void **ppData, uint32_t uLen, int32_t iPos = -1);
	void setPos(uint32_t uPos);
	int32_t getPos() const;

protected:

	bool can() const;
	IXAudioCodecTarget *m_pCodecTarget = NULL;
	CSoundSystem *m_pSoundSystem = NULL;

	String m_sPath = "";
};

#endif
