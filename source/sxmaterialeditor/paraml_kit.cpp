
#include <sxmaterialeditor/paraml_kit.h>

ParamLKit::ParamLKit()
{

}

ParamLKit::~ParamLKit()
{

}

void ParamLKit::Load(const char* path)
{
	ISXLConfig* config = Core_OpLConfig(path);

	if (!config)
		return;

	if (!config->SectionExists("paraml_kit"))
	{
		return;
	}

	if (!config->KeyExists("paraml_kit", "count"))
	{
		return;
	}

	int count = String(config->GetKey("paraml_kit", "count")).ToInt();
	char section_name[CONFIG_SECTION_MAX_LEN];

	for (int i = 0; i < count; ++i)
	{
		sprintf(section_name, "paraml_%d", i);
		if (!config->SectionExists(section_name))
		{
			//reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect [%s], %s", gen_msg_location, eff_section_name, path);
			return;
		}

		if (!(config->KeyExists(section_name, "name") && config->KeyExists(section_name, "thickness") && config->KeyExists(section_name, "roughness") && config->KeyExists(section_name, "f0")))
			continue;

		paraml* tmpparaml = new paraml();
		tmpparaml->name = config->GetKey(section_name, "name");
		tmpparaml->thickness = String(config->GetKey(section_name, "thickness")).ToDouble();
		tmpparaml->roughness = String(config->GetKey(section_name, "roughness")).ToDouble();
		tmpparaml->f0 = String(config->GetKey(section_name, "f0")).ToDouble();

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