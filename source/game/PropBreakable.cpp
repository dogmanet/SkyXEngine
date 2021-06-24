
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PropBreakable.h"

#include <common/file_utils.h>

#include "PropDebris.h"

/*! \skydocent prop_breakable
Разбиваемый объект
*/

BEGIN_PROPTABLE(CPropBreakable)
	//! Разбить
	DEFINE_INPUT(breakInput, "break", "Break", PDF_NONE)

	//! При разбивании
	DEFINE_OUTPUT(m_onBroken, "OnBroken", "On broken")

	DEFINE_FIELD_FLOAT(m_fHealth, PDFF_NONE, "health", "Health", EDITOR_TEXTFIELD)
	DEFINE_FIELD_STRING(m_szSndBreak, PDFF_NONE, "snd_break", "Break sound", EDITOR_FILEFIELD)
		FILE_OPTION("Select sound", "ogg")
	EDITOR_FILE_END()
END_PROPTABLE()

REGISTER_ENTITY(CPropBreakable, prop_breakable);

CPropBreakable::~CPropBreakable()
{
	mem_release(m_pSndBreak);
}

void CPropBreakable::onPostLoad()
{
	BaseClass::onPostLoad();

	IXSoundSystem *pSound = (IXSoundSystem*)(Core_GetIXCore()->getPluginManager()->getInterface(IXSOUNDSYSTEM_GUID));
	if(pSound)
	{
		IXSoundLayer *pGameLayer = pSound->findLayer("xGame");
		if(m_szSndBreak[0])
		{
			m_pSndBreak = pGameLayer->newSoundPlayer(m_szSndBreak, SOUND_SPACE_3D);
		}
	}
}

void CPropBreakable::breakInput(inputdata_t * pInputdata)
{
	takeHealth(m_fHealth, pInputdata->pInflictor, pInputdata->pActivator);
}

void CPropBreakable::onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor)
{
	FIRE_OUTPUT(m_onBroken, pInflictor);

	SAFE_CALL(m_pSndBreak, setWorldPos, getPos());
	SAFE_CALL(m_pSndBreak, play);

	if(m_szModelFile)
	{
		int len = (int)strlen(m_szModelFile);
		char *szMask = (char*)alloca(len + 2);
		strcpy(szMask, m_szModelFile);

		// someModel.dse00
		//          432101 
		szMask[len - 4] = '_';
		szMask[len - 3] = 'g';
		szMask[len - 2] = 'i';
		szMask[len - 1] = 'b';
		szMask[len - 0] = '*';
		szMask[len + 1] = 0;

		Array<String> aFiles = FileGetListFiles(szMask);

		dirname(szMask);

		char szTmp[MAX_PATH];

		for(int i = 0, l = aFiles.size(); i < l; ++i)
		{
			sprintf(szTmp, "%s%s", szMask, aFiles[i].c_str());

			CPropDebris *pEnt = (CPropDebris*)CREATE_ENTITY("prop_debris", m_pMgr);
			pEnt->setFlags(pEnt->getFlags() | EF_LEVEL);
			pEnt->setPos(getPos());
			pEnt->setOrient(getOrient());
			pEnt->setSkin(m_iSkin);
			pEnt->setKV("is_static", "0");
			pEnt->setModel(szTmp);
			pEnt->sheduleRemove();
		}
	}
	

	REMOVE_ENTITY(this);
}
