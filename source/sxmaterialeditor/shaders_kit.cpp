
#include <sxmaterialeditor/shaders_kit.h>

ShadersKit::ShadersKit()
{

}

ShadersKit::~ShadersKit()
{

}

void ShadersKit::Load(const char* path)
{
	ISXLConfig* config = Core_OpLConfig(path);

	if (!config)
		return;

	if (!config->SectionExists("shaders_kit"))
	{
		return;
	}

	if (!config->KeyExists("shaders_kit", "count"))
	{
		return;
	}

	int count = String(config->GetKey("shaders_kit", "count")).ToInt();
	char section_name[CONFIG_SECTION_MAX_LEN];

	for (int i = 0; i < count; ++i)
	{
		sprintf(section_name, "shader_%d", i);
		if (!config->SectionExists(section_name))
		{
			//reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect [%s], %s", gen_msg_location, eff_section_name, path);
			return;
		}

		if (!(config->KeyExists(section_name, "name") && config->KeyExists(section_name, "vs") && config->KeyExists(section_name, "ps")))
			continue;

		shaders* tmpshaders = new shaders();
		tmpshaders->name = config->GetKey(section_name, "name");
		tmpshaders->vs = config->GetKey(section_name, "vs");
		tmpshaders->ps = config->GetKey(section_name, "ps");

		Arr.push_back(tmpshaders);
	}
}

UINT ShadersKit::GetCount() const
{
	return Arr.size();
}

const char* ShadersKit::GetName(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->name.c_str();

	return 0;
}

const char* ShadersKit::GetVS(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->vs.c_str();
	
	return 0;
}

const char* ShadersKit::GetPS(ID id) const
{
	if (id >= 0 && Arr.size() > id && Arr[id])
		return Arr[id]->ps.c_str();
	
	return 0;
}

ID ShadersKit::Find(const char* vs, const char* ps) const
{
	if (!vs || !ps)
		return -1;

	for (int i = 0, il = Arr.size(); i < il; ++i)
	{
		if (stricmp(vs, Arr[i]->vs.c_str()) == 0 && stricmp(ps, Arr[i]->ps.c_str()) == 0)
			return i;
	}

	return -1;
}