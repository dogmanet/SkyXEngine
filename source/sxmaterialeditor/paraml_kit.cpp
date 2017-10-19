
#include "paraml_kit.h"

ParamLKit::ParamLKit()
{

}

ParamLKit::~ParamLKit()
{

}

void ParamLKit::Load(const char* path)
{
	ISXConfig* config = Core_OpConfig(path);

	if (!config)
		return;

	if (!config->sectionExists("paraml_kit"))
	{
		return;
	}

	if (!config->keyExists("paraml_kit", "count"))
	{
		return;
	}

	int count = String(config->getKey("paraml_kit", "count")).ToInt();
	char section_name[CONFIG_SECTION_MAX_LEN];

	for (int i = 0; i < count; ++i)
	{
		sprintf(section_name, "paraml_%d", i);
		if (!config->sectionExists(section_name))
		{
			//reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect [%s], %s", gen_msg_location, eff_section_name, path);
			return;
		}

		if (!(config->keyExists(section_name, "name") && config->keyExists(section_name, "thickness") && config->keyExists(section_name, "roughness") && config->keyExists(section_name, "f0")))
			continue;

		paraml* tmpparaml = new paraml();
		tmpparaml->name = config->getKey(section_name, "name");
		tmpparaml->thickness = String(config->getKey(section_name, "thickness")).ToDouble();
		tmpparaml->roughness = String(config->getKey(section_name, "roughness")).ToDouble();
		tmpparaml->f0 = String(config->getKey(section_name, "f0")).ToDouble();

		Arr.push_back(tmpparaml);
	}
}

UINT ParamLKit::GetCount() const
{
	return Arr.size();
}

const char* ParamLKit::GetName(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->name.c_str();

	return 0;
}

float ParamLKit::GetThickness(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->thickness;
	
	return 0;
}

float ParamLKit::GetRoughness(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->roughness;
	
	return 0;
}

float ParamLKit::GetF0(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->f0;

	return 0;
}

ID ParamLKit::Find(float thickness, float roughness, float f0) const
{
	for (int i = 0, il = Arr.size(); i < il; ++i)
	{
		if (thickness == Arr[i]->thickness && roughness == Arr[i]->roughness && f0 == Arr[i]->f0)
			return i;
	}

	return -1;
}