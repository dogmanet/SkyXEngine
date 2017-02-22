
void SaveLevel(const char* name)
{
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

	fclose(file);
}