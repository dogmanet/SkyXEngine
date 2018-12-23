
#include "shaders_kit.h"

CShadersKit::CShadersKit()
{

}

CShadersKit::~CShadersKit()
{

}

void CShadersKit::load(const char* path)
{
	ISXConfig* config = Core_OpConfig(path);

	if (!config)
		return;

	if (!config->sectionExists("shaders_kit"))
	{
		return;
	}

	if (!config->keyExists("shaders_kit", "count"))
	{
		return;
	}

	int count = String(config->getKey("shaders_kit", "count")).toInt();
	char section_name[CONFIG_SECTION_MAX_LEN];

	for (int i = 0; i < count; ++i)
	{
		sprintf(section_name, "shader_%d", i);
		if (!config->sectionExists(section_name))
		{
			//reportf(REPORT_MSG_LEVEL_ERROR, "%s - not found effect [%s], %s", gen_msg_location, eff_section_name, path);
			return;
		}

		if (!(config->keyExists(section_name, "name") && config->keyExists(section_name, "vs") && config->keyExists(section_name, "ps")))
			continue;

		CShaders* tmpshaders = new CShaders();
		tmpshaders->m_sName = config->getKey(section_name, "name");
		tmpshaders->m_sVS = config->getKey(section_name, "vs");
		tmpshaders->m_sPS = config->getKey(section_name, "ps");

		m_aShaders.push_back(tmpshaders);
	}
}

UINT CShadersKit::getCount() const
{
	return m_aShaders.size();
}

const char* CShadersKit::getName(ID id) const
{
	if (id >= 0 && m_aShaders.size() > id && m_aShaders[id])
		return m_aShaders[id]->m_sName.c_str();

	return 0;
}

const char* CShadersKit::getVS(ID id) const
{
	if (id >= 0 && m_aShaders.size() > id && m_aShaders[id])
		return m_aShaders[id]->m_sVS.c_str();
	
	return 0;
}

const char* CShadersKit::getPS(ID id) const
{
	if (id >= 0 && m_aShaders.size() > id && m_aShaders[id])
		return m_aShaders[id]->m_sPS.c_str();
	
	return 0;
}

ID CShadersKit::find(const char* vs, const char* ps) const
{
	if (!vs || !ps)
		return -1;

	for (int i = 0, il = m_aShaders.size(); i < il; ++i)
	{
		if (stricmp(vs, m_aShaders[i]->m_sVS.c_str()) == 0 && stricmp(ps, m_aShaders[i]->m_sPS.c_str()) == 0)
			return i;
	}

	return -1;
}