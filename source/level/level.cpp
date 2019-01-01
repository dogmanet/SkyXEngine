
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "level.h"
#include <decals/sxdecals.h>

CLevel::CLevel(bool isServerMode)
{
	m_sAmbientSounds = "";
	m_sWeather = "";
	m_szName[0] = 0;
	m_sLocalName = "";
	m_isServerMode = isServerMode;

	if(!isServerMode)
	{
		loadParticles();
		m_pWeather = new CWeather();
		m_pAmbientSounds = new CAmbientSounds();
	}
}

CLevel::~CLevel()
{
	clear();

	mem_delete(m_pWeather);
	mem_delete(m_pAmbientSounds);
}

void CLevel::clear()
{
	m_szName[0] = 0;
	m_sLocalName = "";
	m_sWeather = "";
	m_sAmbientSounds = "";

	SGeom_Clear();
	SGreen_Clear();

	if(!m_isServerMode)
	{
		SLight_ClearIDArr();
	}
	
	//SGame_UnloadObjLevel();
	Core_0ConsoleExecCmd("ent_unload_level");
	SPhysics_UnloadGeom();
	SAIG_Clear();
	if(!m_isServerMode)
	{
		SXDecals_Clear();
	}
}

void CLevel::load(const char *szName, bool isGame)
{
	clear();

	LibReport(REPORT_MSG_LEVEL_NOTICE, "load level '%s' ...\n", szName);
	sprintf(m_szName, "%s", szName);

	char szFullPathEntity[1024];
	szFullPathEntity[0] = 0;
	
	char szFullPath[1024];
	sprintf(szFullPath, "%s%s/%s.lvl", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
	if (!FileExistsFile(szFullPath))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of level '%s'", szFullPath);
		return;
	}

	ISXConfig *pConfig = Core_OpConfig(szFullPath);

	if (pConfig->keyExists("level", "local_name"))
		m_sLocalName = pConfig->getKey("level", "local_name");

	if (pConfig->keyExists("level", "geometry"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load geometry\n");
		sprintf(szFullPath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, pConfig->getKey("level", "geometry"));
		if (FileExistsFile(szFullPath))
			SGeom_Load(szFullPath);
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of geometry '%s'", szFullPath);
		}
	}

	if (pConfig->keyExists("level", "green"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load green\n");
		sprintf(szFullPath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, pConfig->getKey("level", "green"));
		if (FileExistsFile(szFullPath))
			SGreen_Load(szFullPath);
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of green '%s'", szFullPath);
		}
	}
	
	if (pConfig->keyExists("level", "entity"))
	{
		//LibReport(REPORT_MSG_LEVEL_NOTICE, "  load entity\n");
		sprintf(szFullPathEntity, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, pConfig->getKey("level", "entity"));
		FileCanonizePath(szFullPathEntity);
		if (FileExistsFile(szFullPathEntity))
		{
			//SGame_LoadEnts(szFullPathEntity);
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of entity '%s'", szFullPathEntity);
		}
	}

	if (pConfig->keyExists("level", "physic"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load physic\n");
		
		sprintf(szFullPath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, pConfig->getKey("level", "physic"));
		if (FileExistsFile(szFullPath))
		{
			SPhysics_ImportGeom(szFullPath);
		}
		else
		{
			if (isGame)
			{
				SPhysics_LoadGeom(szFullPath);
				//SPhysics_ExportGeom(tmppath);
			}
		}
	}
	else
	{
		if (isGame)
			SPhysics_LoadGeom();
	}

	if (pConfig->keyExists("level", "aigrid"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load aigrid\n");
		
		sprintf(szFullPath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, pConfig->getKey("level", "aigrid"));
		if (FileExistsFile(szFullPath))
			SAIG_GridLoad(szFullPath);
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of aigrid '%s'", szFullPath);
		}
	}

	if(!m_isServerMode && pConfig->keyExists("level", "ambient_sounds"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load ambient_sounds\n");
		m_sAmbientSounds = pConfig->getKey("level", "ambient_sounds");

		if (isGame)
		{
			m_pAmbientSounds->clear();
			char szFullStr[4096];
			sprintf(szFullStr, "%s", m_sAmbientSounds.c_str());
			char *szStr = strtok(szFullStr, " ,|");

			if (szStr)
				m_pAmbientSounds->add(szStr);

			while (szStr != NULL)
			{
				szStr = strtok(NULL, " ,|");

				if (szStr)
					m_pAmbientSounds->add(szStr);
			}
		}
	}

	if(!m_isServerMode && pConfig->keyExists("level", "weather"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load weather\n");
		m_sWeather = pConfig->getKey("level", "weather");
		if (isGame)
		{
			sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS), m_sWeather.c_str());

			if (FileExistsFile(szFullPath))
				m_pWeather->load(szFullPath);
			else
			{
				LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of weather '%s'", szFullPath);
			}
		}
	}

	if(!m_isServerMode && pConfig->keyExists("level", "type"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  init type\n");
		String sStr = pConfig->getKey("level", "type");
		if (stricmp(sStr.c_str(), "outdoor") == 0)
		{
			SGCore_SkyBoxCr();
			SGCore_SkyCloudsCr();

			SGCore_SkyBoxSetUse(true);
			SGCore_SkyCloudsSetUse(true);

			float3 vMin, vMax;
			SGeom_GetMinMax(&vMin, &vMax);
			float fWidth = (vMax.x - vMin.x) * 2.f;
			float fHeight = (vMax.z - vMin.z) * 2.f;
			fWidth = (fWidth >= SXGC_SKYCLOUDS_MIN_SIZE ? fWidth : SXGC_SKYCLOUDS_MIN_SIZE);
			fHeight = (fHeight >= SXGC_SKYCLOUDS_MIN_SIZE ? fHeight : SXGC_SKYCLOUDS_MIN_SIZE);
			SGCore_SkyCloudsSetWidthHeightPos(fWidth, fHeight, &float3((vMax + vMin) * 0.5));

			ID gid = SLight_CreatePoint(
				&float3(60, 60, 0),
				0,
				&float3(1, 1, 1),
				true,
				true);
			SLight_SetEnable(gid, true);
			SLight_SetName(gid, "sun");
		}
		else
		{
			SGCore_SkyBoxSetUse(false);
			SGCore_SkyCloudsSetUse(false);
		}
	}

	Core_0ConsoleExecCmd("ent_load_level \"%s\" \"%s\"", szFullPathEntity, szName);

	//SGame_OnLevelLoad(szName);

	mem_release(pConfig);

	LibReport(REPORT_MSG_LEVEL_NOTICE, "level '%s' loaded!\n", szName);
}

void CLevel::save(const char *szName)
{
	sprintf(m_szName, "%s", szName);
	char szPathLevel[1024], szPathDirLevel[1024];
	sprintf(szPathDirLevel, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName);
	sprintf(szPathLevel, "%s%s.lvl", szPathDirLevel, szName);

	if (!FileExistsDir(szPathDirLevel))
		FileCreateDir(szPathDirLevel);

	char szFullPath[1024];

	FILE* file = 0;
	file = fopen(szPathLevel, "w");

	fprintf(file, "%s", "[level]\n");

	if (m_sLocalName.length() > 0)
		fprintf(file, "local_name = %s\n", m_sLocalName.c_str());

	if (SGeom_GetCountModels() > 0)
	{
		fprintf(file, "geometry = %s.geom\n", szName);

		sprintf(szFullPath, "%s%s/%s.geom", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		SGeom_Save(szFullPath);
	}

	if (SGreen_GetCount() > 0)
	{
		fprintf(file, "green = %s.green\n", szName);
		sprintf(szFullPath, "%s%s/%s.green", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		SGreen_Save(szFullPath);
	}

	if (SLight_GetGlobal() > 0)
		fprintf(file, "type = outdoor\n");
	else
		fprintf(file, "type = indoor\n");

	

	//if (SGame_EntGetCount() > 0)
	{
		sprintf(szFullPath, "%s%s/%s.ent", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		fprintf(file, "entity = %s.ent\n", szName);
		FileCanonizePath(szFullPath);
		Core_0ConsoleExecCmd("ent_save_level \"%s\"", szFullPath);
		//SGame_SaveEnts(tmppathlevel);
	}

	if (SAIG_GridGetCountSplits() > 0)
	{
		sprintf(szFullPath, "%s%s/%s.aigrid", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		fprintf(file, "aigrid = %s.aigrid\n", szName);
		SAIG_GridSave(szFullPath);
	}

	SPhysics_LoadGeom();
	sprintf(szFullPath, "%s%s/%s.phy", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
	fprintf(file, "physic = %s.phy\n", szName);
	SPhysics_ExportGeom(szFullPath);

	if (m_sAmbientSounds[0])
	{
		fprintf(file, "ambient_sounds = %s\n", m_sAmbientSounds.c_str());
	}

	if (m_sWeather[0])
	{
		fprintf(file, "weather = %s\n", m_sWeather.c_str());
	}

	fprintf(file, "\n");
	fclose(file);
}

const char* CLevel::getName(char *szName)
{
	if (szName)
		strcpy(szName, m_szName);

	return m_szName;
}

void CLevel::loadParticles()
{
	char szPathLoad[1024];
	sprintf(szPathLoad, "%seff.eff", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE));

	SPE_EffectLoad(szPathLoad);
}

void CLevel::saveParticles()
{
	char szPathSave[1024];
	sprintf(szPathSave, "%seff.eff", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE));

	SPE_EffectSave(szPathSave);
}