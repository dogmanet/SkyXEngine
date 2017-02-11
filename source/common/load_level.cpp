
void LoadLevel(const char* name)
{
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

	SGCore_LoadTexLoadTextures();
	mem_release(config);

	for (long i = 0; i < SML_LigthsGetCount(); ++i)
	{
		if (SML_LigthsGetType(i) == LIGHTS_TYPE_DIRECTION)
			SML_LigthsSetIDArr(i, 0, 0, SGeom_ModelsAddArrForCom());
		else if (SML_LigthsGetType(i) == LIGHTS_TYPE_POINT)
		{
			for (int k = 0; k < 6; ++k)
			{
				SML_LigthsSetIDArr(i, 0, k, SGeom_ModelsAddArrForCom());
			}
		}
		else if (SML_LigthsGetType(i) == LIGHTS_TYPE_GLOBAL)
		{
			for (int k = 0; k < 5; ++k)
			{
				SML_LigthsSetIDArr(i, 0, k, SGeom_ModelsAddArrForCom());
			}
		}
	}
}