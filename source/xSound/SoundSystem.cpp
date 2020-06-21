
#include "SoundSystem.h"
#include "AudioConverter.h"

#include "SoundLayer.h"

#include <ctime>
#include <sys/stat.h>

#ifdef _MSC_VER
#include <direct.h>
#endif

//##########################################################################

CSoundSystem::CSoundSystem(IXCore *pXCore)
{
#ifdef _MSC_VER
	mkdir(SOUND_CACHE);
#else
	mkdir(SOUND_CACHE, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	m_pXCore = pXCore;

	auto pPluginManager = m_pXCore->getPluginManager();

	IXAudioCodec *pAudioCodec;
	UINT ic = 0;
	while ((pAudioCodec = (IXAudioCodec*)(pPluginManager->getInterface(IXAUDIOCODEC_GUID, ic++))))
	{
		if (pAudioCodec->getVersion() == IXAUDIOCODEC_VERSION)
		{
			LibReport(REPORT_MSG_LEVEL_NOTICE, "Registered audio codec: %s\n", pAudioCodec->getFormat());

			addCodec(pAudioCodec->getFormat(), pAudioCodec);
		}
	}

	m_pAudio = MitAl_InitAudio();
}

CSoundSystem::~CSoundSystem()
{
	mem_release(m_pMasterLayer);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundSystem::update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	std::lock_guard<std::mutex> guard(m_oMutexUpdate);

	if(m_pMasterLayer)
		m_pMasterLayer->update(vListenerPos, vListenerDir, vListenerUp);

	m_vObserverPos = vListenerPos; m_vObserverLook = vListenerDir; m_vObserverUp = vListenerUp;
}

//**************************************************************************

IXSoundLayer* XMETHODCALLTYPE CSoundSystem::createMasterLayer(const AudioRawDesc *pDesc, const char *szName)
{
	if(m_pMasterLayer)
		return m_pMasterLayer;

	if(!pDesc)
		return NULL;

	if (!supportedDesc(pDesc, AB_TYPE_PRIMARY))
	{
		printf("unsupported audio buffer description");
		return NULL;
	}

	CSoundLayer *pLayer = new CSoundLayer();
	pLayer->init(this, NULL, pDesc, szName);

	m_pMasterLayer = pLayer;

	return pLayer;
}

//**************************************************************************

IXCore* CSoundSystem::getCore() const
{
	return m_pXCore;
}

//**************************************************************************

IXAudioCodecTarget* CSoundSystem::getCodecTarget(const char *szName)
{
	if(!m_pMasterLayer)
		return NULL;

	if (!szName)
		return NULL;

	const char *szPath = szName;

	if (!m_pXCore->getFileSystem()->fileExists(szPath))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "File '%s' not found\n", szPath);
		return NULL;
	}

	IXAudioCodec *pCodec = NULL;
	IXAudioCodecTarget *pTarget = NULL;
	IXAudioCodecTarget *pTarget2 = NULL;

	for (mapcodec::Iterator i = m_mapCodecs.begin(); i != m_mapCodecs.end(); i++)
	{
		if (m_mapCodecs[i.first]->open(szPath, "", &pTarget, false))
		{
			pCodec = m_mapCodecs[i.first];
			break;
		}
	}

	if(!pCodec || !pTarget)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unsupported audio format '%s'\n", szPath);
		return NULL;
	}

	AudioRawDesc oDescMaster, oDescSnd, oDescCache;
	m_pMasterLayer->getDesc(&oDescMaster);
	pTarget->getDesc(&oDescSnd);

	//если звук соответствует требования мастер буфера, тогда не надо конвертировать, возвращает целевой кодек
	if(oDescSnd.uSampleRate == oDescMaster.uSampleRate)
		return pTarget;


	//если кодек может сохранять, тогда пересохраняем файл
	if(pCodec->canSave())
	{
		//выдергиваем pcm данные из таргета и удаляем его
		IXBuffer *pBufferIn = m_pXCore->newBuffer();
		IXBuffer *pBufferOut = m_pXCore->newBuffer();
		pBufferIn->alloc(oDescSnd.uSize);
		BYTE *pData = pBufferIn->get();
		pTarget->decode(0, pBufferIn->size(), (void**)&pData);
		pTarget->Release();

		//открываем файл на запись
		if(!pCodec->open(szPath, "", &pTarget, true))
			return NULL;

		//создаем нужное описание и конвертируем pcm данные
		AudioRawDesc oDescOut = oDescSnd;
		oDescOut.uSampleRate = oDescMaster.uSampleRate;
		oDescOut.calc();
		if (!AudioConverter(pData, &oDescSnd, pBufferOut, &oDescOut))
			return NULL;

		//записываем новые данные и сносим таргет
		pTarget->encode(pBufferOut, &oDescOut);
		pTarget->Release();

		//открываем файл на чтение
		if(!pCodec->open(szPath, "", &pTarget, false))
			return NULL;

		return pTarget;
	}

	//ищем первый попавшийся кодек который может записывать
	IXAudioCodec *pFullCodec = getCodecSave();
	if (!pFullCodec)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Not found codec for coding, file '%s' does not match of parameters master buffer\n", szPath);
		return NULL;
	}

	//создаем путь до файла в кэше
	String sCachePath = String(SOUND_CACHE) + "/"+ szPath;
	sCachePath = PathSetExt(sCachePath.c_str(), pFullCodec->getExt());

	//берем инфу о файлах (оригинал и кэш)
	struct stat stOrigin, stCache;
	stat(szPath, &stOrigin);
	stat(sCachePath.c_str(), &stCache);

	//нужно ли конвертировать файл
	bool canConvert = false;

	//если есть разница по времени (изменения/модификации), тогда надо конвертировать
#ifdef _MSC_VER
	if (stOrigin.st_ctime > stCache.st_ctime || stOrigin.st_mtime > stCache.st_mtime)
#else
	if(stOrigin.st_ctim.tv_sec > stCache.st_ctim.tv_sec || stOrigin.st_mtim.tv_sec > stCache.st_mtim.tv_sec)
#endif
		canConvert = true;
	else
	{
		//если не удается открыть файл кэша, то надо конвертировать
		if(!pFullCodec->open(sCachePath.c_str(), "", &pTarget2, false))
			canConvert = true;
		else
		{
			//если файл кэша не соответствует, тогда надо конвертировать
			pTarget2->getDesc(&oDescCache);
			if(oDescCache.uSampleRate != oDescMaster.uSampleRate)
				canConvert = true;
		}
	}

	//если конвертирование не требуется, возвращаем целевой кодек
	if(!canConvert)
		return pTarget2;

	mem_release(pTarget2);

	//выдергиваем pcm данные из таргета и удаляем его
	IXBuffer *pBufferIn = m_pXCore->newBuffer();
	IXBuffer *pBufferOut = m_pXCore->newBuffer();
	pBufferIn->alloc(oDescSnd.uSize);
	BYTE *pData = pBufferIn->get();
	pTarget->decode(0, pBufferIn->size(), (void**)&pData);
	pTarget->Release();

	//создаем нужное описание и конвертируем pcm данные
	AudioRawDesc oDescOut = oDescSnd;
	oDescOut.uSampleRate = oDescMaster.uSampleRate;
	oDescOut.calc();
	if (!AudioConverter(pData, &oDescSnd, pBufferOut, &oDescOut))
		return NULL;

	//открываем файл на запись
	if(!pFullCodec->open(sCachePath.c_str(), "", &pTarget2, true))
		return NULL;

	pTarget2->encode(pBufferOut, &oDescOut);
	pTarget2->Release();

	if(!pFullCodec->open(sCachePath.c_str(), "", &pTarget2, false))
		return NULL;

	return pTarget2;
}

//**************************************************************************

void CSoundSystem::addCodec(const char *szFmt, IXAudioCodec *pCodec)
{
	if(!szFmt || !pCodec)
		return;

	m_mapCodecs[szFmt] = pCodec;
}

//**************************************************************************

IAudioBufferEx* CSoundSystem::createMasterBuffer(AudioRawDesc *pDesc)
{
	return m_pAudio->createMasterBuffer(pDesc);
}

//**************************************************************************

bool CSoundSystem::supportedDesc(const AudioRawDesc *pDesc, AB_TYPE type)
{
	return m_pAudio->supportedDesc(pDesc, type);
}

//**************************************************************************

IXSoundLayer* XMETHODCALLTYPE CSoundSystem::findLayer(const char *szName)
{
	if(!szName)
		return NULL;

	if(strcasecmp(m_pMasterLayer->getName(), szName) == 0)
		return m_pMasterLayer;

	return m_pMasterLayer->findLayer(szName);
}

//**************************************************************************

IXAudioCodec* CSoundSystem::getCodecSave()
{
	for (mapcodec::Iterator i = m_mapCodecs.begin(); i != m_mapCodecs.end(); i++)
	{
		if (m_mapCodecs[i.first]->canSave())
			return m_mapCodecs[i.first];
	}

	return NULL;
}

