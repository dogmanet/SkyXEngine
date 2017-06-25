
#include <managed_render\\level.h>

void Level::Clear()
{
	Name[0] = 0;
	SGeom_ModelsClear();
	SGeom_GreenClear();
	SML_LigthsClear();
	SXPhysics_UnloadGeom();
	//! \todo добавить очистку игровых объектов в очистку уровня
}

void Level::Load(const char* name)
{
	sprintf(Name, "%s", name);
	char tmppathlevel[1024];
	sprintf(tmppathlevel, "%s%s\\%s.lvl", GData::Pathes::Levels, name, name);
	if (!Core_0FileExists(tmppathlevel))
	{

	}

	ISXLConfig* config = Core_OpLConfig(tmppathlevel);
	if (config->KeyExists("level", "geometry"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "geometry"));
		if (Core_0FileExists(tmppath))
			SGeom_ModelsLoad(tmppath);
		else
		{
			//error
		}
	}

	if (config->KeyExists("level", "green"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "green"));
		if (Core_0FileExists(tmppath))
			SGeom_GreenLoad(tmppath);
		else
		{
			//error
		}
	}

	if (config->KeyExists("level", "light"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "light"));
		if (Core_0FileExists(tmppath))
			SML_LigthsLoad(tmppath);
		else
		{
			//error
		}
	}

	if(config->KeyExists("level", "entity"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "entity"));
		if(Core_0FileExists(tmppath))
			SXGame_LoadEnts(tmppath);
		else
		{
			//error
		}
	}

	if (config->KeyExists("level", "physic"))
	{
		char tmppath[1024];
		sprintf(tmppath, "%s%s\\%s", GData::Pathes::Levels, name, config->GetKey("level", "physic"));
		if(Core_0FileExists(tmppath))
			SXPhysics_ImportGeom(tmppath);
		else
		{
			//error
		}
	}
	else
	{
#if defined(SX_GAME)
		SXPhysics_LoadGeom();
#endif
	}

	SGCore_LoadTexLoadTextures();
	mem_release(config);
}

void Level::Save(const char* name)
{
	sprintf(Name, "%s", name);
	char tmppathlevel[1024];
	sprintf(tmppathlevel, "%s%s\\%s.lvl", GData::Pathes::Levels, name, name);
	if (!Core_0FileExists(tmppathlevel))
	{

	}

	FILE* file = 0;
	file = fopen(tmppathlevel, "w");

	fprintf(file, "%s", "[level]\n");

	if (SGeom_ModelsGetCount() > 0)
	{
		fprintf(file, "geometry = %s.geom\n", name);

		sprintf(tmppathlevel, "%s%s\\%s.geom", GData::Pathes::Levels, name, name);
		SGeom_ModelsSave(tmppathlevel);
	}

	if (SGeom_GreenGetCount() > 0)
	{
		fprintf(file, "green = %s.green\n", name);
		sprintf(tmppathlevel, "%s%s\\%s.green", GData::Pathes::Levels, name, name);
		SGeom_GreenSave(tmppathlevel);
	}

	if (SML_LigthsGetCount() > 0)
	{
		sprintf(tmppathlevel, "%s%s\\%s.light", GData::Pathes::Levels, name, name);
		fprintf(file, "light = %s.light\n", name);
		SML_LigthsSave(tmppathlevel);
	}

	if (SXGame_EntGetCount() > 0)
	{
		sprintf(tmppathlevel, "%s%s\\%s.ent", GData::Pathes::Levels, name, name);
		fprintf(file, "entity = %s.ent\n", name);
		SXGame_SaveEnts(tmppathlevel);
	}

	SXPhysics_LoadGeom();
	sprintf(tmppathlevel, "%s%s\\%s.phy", GData::Pathes::Levels, name, name);
	fprintf(file, "physic = %s.phy\n", name);
	SXPhysics_ExportGeom(tmppathlevel);


	fclose(file);
}

void Level::LoadParticles()
{
	char tmppathsave[1024];
	sprintf(tmppathsave, "%seff.eff", GData::Pathes::GameSource);

	SPE_EffectLoad(tmppathsave);
}

void Level::SaveParticles()
{
	char tmppathsave[1024];
	sprintf(tmppathsave, "%seff.eff", GData::Pathes::GameSource);

	SPE_EffectSave(tmppathsave);
}