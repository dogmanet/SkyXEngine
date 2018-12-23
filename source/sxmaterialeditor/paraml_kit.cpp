
#include "paraml_kit.h"

CParamLigthKit::CParamLigthKit()
{

}

CParamLigthKit::~CParamLigthKit()
{

}

void CParamLigthKit::load(const char *szPath)
{
	ISXConfig* config = Core_OpConfig(szPath);

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

	int count = String(config->getKey("paraml_kit", "count")).toInt();
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

		CParamLigth* tmpparaml = new CParamLigth();
		tmpparaml->m_sName = config->getKey(section_name, "name");
		tmpparaml->m_fThickness = String(config->getKey(section_name, "thickness")).toDouble();
		tmpparaml->m_fRoughness = String(config->getKey(section_name, "roughness")).toDouble();
		tmpparaml->m_fF0 = String(config->getKey(section_name, "f0")).toDouble();

		m_aParameters.push_back(tmpparaml);
	}
}

UINT CParamLigthKit::getCount() const
{
	return m_aParameters.size();
}

const char* CParamLigthKit::getName(ID id) const
{
	if (id >= 0 && m_aParameters.size() > id && m_aParameters[id])
		return m_aParameters[id]->m_sName.c_str();

	return 0;
}

float CParamLigthKit::getThickness(ID id) const
{
	if (id >= 0 && m_aParameters.size() > id && m_aParameters[id])
		return m_aParameters[id]->m_fThickness;
	
	return 0;
}

float CParamLigthKit::getRoughness(ID id) const
{
	if (id >= 0 && m_aParameters.size() > id && m_aParameters[id])
		return m_aParameters[id]->m_fRoughness;
	
	return 0;
}

float CParamLigthKit::getF0(ID id) const
{
	if (id >= 0 && m_aParameters.size() > id && m_aParameters[id])
		return m_aParameters[id]->m_fF0;

	return 0;
}

ID CParamLigthKit::find(float fThickness, float fRoughness, float fF0) const
{
	for (int i = 0, il = m_aParameters.size(); i < il; ++i)
	{
		if (fThickness == m_aParameters[i]->m_fThickness && fRoughness == m_aParameters[i]->m_fRoughness && fF0 == m_aParameters[i]->m_fF0)
			return i;
	}

	return -1;
}