
#include "CrosshairManager.h"

AssotiativeArray<AAString, ID> CCrosshairManager::m_mIndex;
Array<CCrosshairManager::CrosshairDesc> CCrosshairManager::m_vCrosshairList;

extern report_func reportf;

void CCrosshairManager::loadCrosshair(ID id, CCrosshair * pCCrosshair)
{
	if(id < 0 || id >= (ID)m_vCrosshairList.size())
	{
		return;
	}
	CrosshairDesc * desc = &m_vCrosshairList[id];
	pCCrosshair->setAngle(desc->fAngle);
	pCCrosshair->setFixedRadius(desc->fRadius);
	pCCrosshair->setNumSegmens(desc->iNumSegments);
	pCCrosshair->setStyle(desc->style);
	pCCrosshair->setTexInfo(desc->f2TexOffset, desc->f2TexSize);
	pCCrosshair->setTexture(desc->idTexture);
}

void CCrosshairManager::loadConfig(const char * szFile)
{
	ISXConfig * config = Core_OpConfig(szFile);
	int sections = config->getSectionCount();
	const char * sect;
	
	CrosshairDesc cd;

	for(int i = 0; i < sections; ++i)
	{
		sect = config->getSectionName(i);

		float f;
		if(config->keyExists(sect, "angle") && sscanf(config->getKey(sect, "angle"), "%f", &f))
		{
			cd.fAngle = f;
		}
		else
		{
			cd.fAngle = 0.0f;
		}
		if(config->keyExists(sect, "fixed_radius") && sscanf(config->getKey(sect, "fixed_radius"), "%f", &f))
		{
			cd.fRadius = f;
		}
		else
		{
			cd.fRadius = 0.0f;
		}
		int id;
		if(config->keyExists(sect, "segments") && sscanf(config->getKey(sect, "segments"), "%d", &id))
		{
			cd.iNumSegments = id;
		}
		else
		{
			cd.iNumSegments = 4;
		}

		const char * str;
		if(config->keyExists(sect, "style"))
		{
			str = config->getKey(sect, "style");
		}
		else
		{
			str = "split";
		}
		if(strcmpi(str, "split_move") == 0)
		{
			cd.style = CCrosshair::SPLIT_MOVE;
		}
		else if(strcmpi(str, "scaled") == 0)
		{
			cd.style = CCrosshair::SCALED;
		}
		else
		{
			reportf(REPORT_MSG_LEVEL_WARNING, "Unknown CCrosshair style '%s' for '%s'\n", str, sect);
			continue;
		}

		const char * tex;
		if(config->keyExists(sect, "tex"))
		{
			tex = config->getKey(sect, "tex");
			cd.idTexture = SGCore_LoadTexAddName(tex, ltt_const);
		}
		else
		{
			reportf(REPORT_MSG_LEVEL_WARNING, "Unable to read CCrosshair tex '%s'\n", sect);
			continue;
		}
		int x, y;
		if(config->keyExists(sect, "tex_offset") && sscanf(config->getKey(sect, "tex_offset"), "[%d,%d]", &x, &y) == 2)
		{
			cd.f2TexOffset = float2_t((float)x, (float)y);
		}
		else
		{
			reportf(REPORT_MSG_LEVEL_WARNING, "Unable to read CCrosshair tex_offset '%s'. Expected format: '[x,y]'\n", sect);
			continue;
		}
		if(config->keyExists(sect, "tex_size") && sscanf(config->getKey(sect, "tex_size"), "[%d,%d]", &x, &y) == 2)
		{
			cd.f2TexSize = float2_t((float)x, (float)y);
		}
		else
		{
			reportf(REPORT_MSG_LEVEL_WARNING, "Unable to read CCrosshair tex_size '%s'. Expected format: '[x,y]'\n", sect);
			continue;
		}
		AAString aas;
		aas.SetName(sect);
		m_mIndex[aas] = m_vCrosshairList.size();
		m_vCrosshairList.push_back(cd);
	}

	mem_release(config);
}

ID CCrosshairManager::getCrosshairID(const char * szName)
{
	const AssotiativeArray<AAString, ID>::Node * pNode;
	if(m_mIndex.KeyExists(AAString(szName), &pNode))
	{
		return(*pNode->Val);
	}
	return(-1);
}
