
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "level.h"

CLevel::CLevel()
{
	m_sAmbientSounds = "";
	m_sWeather = "";
	m_szName[0] = 0;

	loadParticles();
	m_pWeather = new CWeather();
	m_pAmbientSounds = new CAmbientSounds();
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
	m_sWeather = "";
	m_sAmbientSounds = "";

	SGeom_ModelsClear();
	SGeom_GreenClear();

	SML_LigthsClearIDArr();
	
	//SXGame_UnloadObjLevel();
	Core_0ConsoleExecCmd("ent_unload_level");
	SXPhysics_UnloadGeom();
	SAIG_Clear();
}

void CLevel::load(const char *szName, bool isGame)
{
	LibReport(REPORT_MSG_LEVEL_NOTICE, "load level '%s' ...\n", szName);
	sprintf(m_szName, "%s", szName);
	char tmppathlevel[1024];
	sprintf(tmppathlevel, "%s%s/%s.lvl", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
	if (!FileExistsFile(tmppathlevel))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of level '%s'", tmppathlevel);
		return;
	}

	ISXConfig* config = Core_OpConfig(tmppathlevel);
	if (config->keyExists("level", "geometry"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load geometry\n");
		char tmppath[1024];
		sprintf(tmppath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, config->getKey("level", "geometry"));
		if (FileExistsFile(tmppath))
			SGeom_ModelsLoad(tmppath);
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of geometry '%s'", tmppath);
		}
	}

	if (config->keyExists("level", "green"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load green\n");
		char tmppath[1024];
		sprintf(tmppath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, config->getKey("level", "green"));
		if (FileExistsFile(tmppath))
			SGeom_GreenLoad(tmppath);
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of green '%s'", tmppath);
		}
	}
	char tmppathEntity[1024];
	if (config->keyExists("level", "entity"))
	{
		//LibReport(REPORT_MSG_LEVEL_NOTICE, "  load entity\n");
		sprintf(tmppathEntity, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, config->getKey("level", "entity"));
		FileCanonizePath(tmppathEntity);
		if(FileExistsFile(tmppathEntity))
		{
			//SXGame_LoadEnts(tmppathEntity);
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of entity '%s'", tmppathEntity);
		}
	}

	if (config->keyExists("level", "physic"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load physic\n");
		char tmppath[1024];
		sprintf(tmppath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, config->getKey("level", "physic"));
		if (FileExistsFile(tmppath))
		{
			SXPhysics_ImportGeom(tmppath);
		}
		else
		{
			if (isGame)
			{
				SXPhysics_LoadGeom(tmppath);
				//SXPhysics_ExportGeom(tmppath);
			}
		}
	}
	else
	{
		if (isGame)
			SXPhysics_LoadGeom();
	}

	if (config->keyExists("level", "aigrid"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load aigrid\n");
		char tmppath[1024];
		sprintf(tmppath, "%s%s/%s", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, config->getKey("level", "aigrid"));
		if (FileExistsFile(tmppath))
			SAIG_GridLoad(tmppath);
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of aigrid '%s'", tmppath);
		}
	}

	//SGCore_LoadTexLoadTextures();

	if (config->keyExists("level", "ambient_sounds"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load ambient_sounds\n");
		m_sAmbientSounds = config->getKey("level", "ambient_sounds");

		if (isGame)
		{
			m_pAmbientSounds->clear();
			char tmpallpath[4096];
			sprintf(tmpallpath, "%s", m_sAmbientSounds.c_str());
			char* tmppath = strtok(tmpallpath, " ,|");
			if (tmppath)
				m_pAmbientSounds->add(tmppath);
			while (tmppath != NULL)
			{
				tmppath = strtok(NULL, " ,|");

				if (tmppath)
					m_pAmbientSounds->add(tmppath);
			}
		}
	}

	if (config->keyExists("level", "weather"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  load weather\n");
		m_sWeather = config->getKey("level", "weather");
		if (isGame)
		{
			char tmppath[1024];
			sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS), m_sWeather.c_str());

			if (FileExistsFile(tmppath))
				m_pWeather->load(tmppath);
			else
			{
				LibReport(REPORT_MSG_LEVEL_WARNING, "not found file of weather '%s'", tmppath);
			}
		}
	}

	if (config->keyExists("level", "type"))
	{
		LibReport(REPORT_MSG_LEVEL_NOTICE, "  init type\n");
		String str = config->getKey("level", "type");
		if (stricmp(str.c_str(), "outdoor") == 0)
		{
			SGCore_SkyBoxCr();
			SGCore_SkyCloudsCr();

			float3 vMin, vMax;
			SGeom_ModelsGetMinMax(&vMin, &vMax);
			SGCore_SkyCloudsSetWidthHeightPos((vMax.x - vMin.x) * 2.f, (vMax.z - vMin.z) * 2.f, &float3((vMax + vMin) * 0.5));

			ID gid = SML_LigthsCreatePoint(
				&float3(60, 60, 0),
				0,
				&float3(1, 1, 1),
				true,
				true);
			SML_LigthsSetEnable(gid, true);
			SML_LigthsSetName(gid, "sun");
		}
	}

	Core_0ConsoleExecCmd("ent_load_level \"%s\" \"%s\"", tmppathEntity, szName);

	//SXGame_OnLevelLoad(szName);

	mem_release(config);

	LibReport(REPORT_MSG_LEVEL_NOTICE, "level '%s' loaded!\n", szName);
}

void CLevel::save(const char *szName)
{
	sprintf(m_szName, "%s", szName);
	char tmppathlevel[1024];
	sprintf(tmppathlevel, "%s%s/%s.lvl", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);

	FILE* file = 0;
	file = fopen(tmppathlevel, "w");

	fprintf(file, "%s", "[level]\n");

	if (SGeom_ModelsGetCount() > 0)
	{
		fprintf(file, "geometry = %s.geom\n", szName);

		sprintf(tmppathlevel, "%s%s/%s.geom", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		SGeom_ModelsSave(tmppathlevel);
	}

	if (SGeom_GreenGetCount() > 0)
	{
		fprintf(file, "green = %s.green\n", szName);
		sprintf(tmppathlevel, "%s%s/%s.green", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		SGeom_GreenSave(tmppathlevel);
	}

	if (SML_LigthsGetGlobal() > 0)
		fprintf(file, "type = outdoor\n");
	else
		fprintf(file, "type = indoor\n");

	

	//if (SXGame_EntGetCount() > 0)
	{
		sprintf(tmppathlevel, "%s%s/%s.ent", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		fprintf(file, "entity = %s.ent\n", szName);
		FileCanonizePath(tmppathlevel);
		Core_0ConsoleExecCmd("ent_save_level \"%s\"", tmppathlevel);
		//SXGame_SaveEnts(tmppathlevel);
	}

	if (SAIG_GridGetCountSplits() > 0)
	{
		sprintf(tmppathlevel, "%s%s/%s.aigrid", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
		fprintf(file, "aigrid = %s.aigrid\n", szName);
		SAIG_GridSave(tmppathlevel);
	}

	SXPhysics_LoadGeom();
	sprintf(tmppathlevel, "%s%s/%s.phy", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), szName, szName);
	fprintf(file, "physic = %s.phy\n", szName);
	SXPhysics_ExportGeom(tmppathlevel);

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