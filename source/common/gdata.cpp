
#include <common/gdata.h>

void GData::Pathes::InitAllPathes()
{
	char tmppath[1024];
	GetModuleFileName(NULL, tmppath, 1024);
	int len = strlen(tmppath);
	while (tmppath[len--] != '\\')
	{
		if (tmppath[len - 1] == '\\')
		{
			len--;
			memcpy(GData::Pathes::ForExe, tmppath, len);
			GData::Pathes::ForExe[len] = 0;
		}
	}

	sprintf(GData::Pathes::GameSource, "%s%s", GData::Pathes::ForExe, "\\gamesource\\");
	sprintf(GData::Pathes::Screenshots, "%s%s", GData::Pathes::ForExe, "\\screenshots\\");
	sprintf(GData::Pathes::Configs, "%s%s", GData::Pathes::GameSource, "configs\\");
	sprintf(GData::Pathes::Levels, "%s%s", GData::Pathes::GameSource, "levels\\");
	sprintf(GData::Pathes::Meshes, "%s%s", GData::Pathes::GameSource, "meshes\\");
	sprintf(GData::Pathes::Models, "%s%s", GData::Pathes::GameSource, "models\\");
	sprintf(GData::Pathes::Shaders, "%s%s", GData::Pathes::GameSource, "shaders\\");
	sprintf(GData::Pathes::Sounds, "%s%s", GData::Pathes::GameSource, "sounds\\");
	sprintf(GData::Pathes::Scripts, "%s%s", GData::Pathes::GameSource, "scripts\\");

	sprintf(GData::Pathes::Textures, "%s%s", GData::Pathes::GameSource, "textures\\");
	sprintf(GData::Pathes::TexturesDetail, "%s%s", GData::Pathes::GameSource, "textures\\detail\\");
	sprintf(GData::Pathes::TexturesSkyBoxes, "%s%s", GData::Pathes::Textures, "sky\\");
	sprintf(GData::Pathes::Materials, "%s%s", GData::Pathes::GameSource, "materials\\");
	sprintf(GData::Pathes::GUIresources, "%s%s", GData::Pathes::GameSource, "resource\\");
}