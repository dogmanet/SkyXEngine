
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <gcore/sxgcore.h>

#include "BaseTrigger.h"

/*! \skydocent trigger
Базовый класс триггера. Триггер - область, задаваемая моделью, при входе/выходе из которой игрока или объектов, вызывает заданные события
*/

BEGIN_PROPTABLE(CBaseTrigger)
	//! Срабатывает при начале контакта
	DEFINE_OUTPUT(m_onTouchStart, "OnTouchStart", "On touch start")
	//! Срабатывает при прекращении контакта
	DEFINE_OUTPUT(m_onTouchEnd, "OnTouchEnd", "On touch end")
	//! Срабатывает, когда не осталось контактирующих объектов
	DEFINE_OUTPUT(m_onTouchEndAll, "OnTouchEndAll", "On touch end all")

	//! Включает триггер
	DEFINE_INPUT(inEnable, "enable", "Enable", PDF_NONE)
	//! Выключает триггер
	DEFINE_INPUT(inDisable, "disable", "Disable", PDF_NONE)
	//! Переключает состояние триггера
	DEFINE_INPUT(inToggle, "toggle", "Toggle", PDF_NONE)
END_PROPTABLE()

REGISTER_ENTITY(CBaseTrigger, trigger);

CBaseTrigger::~CBaseTrigger()
{
	removePhysBody();
	CLEAR_INTERVAL(m_idUpdateInterval);
}

void CBaseTrigger::onPostLoad()
{
	BaseClass::onPostLoad();

	assert(!ID_VALID(m_idUpdateInterval));
	m_idUpdateInterval = SET_INTERVAL(update, 0);

	if(m_pModel)
	{
		//m_pModel->enable(false);
		//m_pAnimPlayer->setOverrideMaterial("dev_trigger.dds");
	}
}

void CBaseTrigger::enable()
{
	if(!m_bEnabled)
	{
		m_bEnabled = true;
		m_idUpdateInterval = SET_INTERVAL(update, 0);
		if(m_pGhostObject)
		{
			SPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, CG_TRIGGER, CG_CHARACTER);
		}
	}
}
void CBaseTrigger::disable()
{
	if(m_bEnabled)
	{
		m_bEnabled = false;
		CLEAR_INTERVAL(m_idUpdateInterval);
		if(m_pGhostObject)
		{
			SPhysics_GetDynWorld()->removeCollisionObject(m_pGhostObject);
		}
	}
}
void CBaseTrigger::toggle()
{
	if(m_bEnabled)
	{
		disable();
	}
	else
	{
		enable();
	}
}

void CBaseTrigger::inEnable(inputdata_t * pInputdata)
{
	enable();
}
void CBaseTrigger::inDisable(inputdata_t * pInputdata)
{
	disable();
}
void CBaseTrigger::inToggle(inputdata_t * pInputdata)
{
	toggle();
}

void CBaseTrigger::createPhysBody()
{
	if(m_pCollideShape)
	{
		m_pGhostObject = new btPairCachingGhostObject();
		m_pGhostObject->setWorldTransform(btTransform(Q4_BTQUAT(m_vOrientation), F3_BTVEC(m_vPosition)));
		m_pGhostObject->setUserPointer(this);
		m_pGhostObject->setCollisionShape(m_pCollideShape);
		m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() ^ btCollisionObject::CF_NO_CONTACT_RESPONSE);

		SPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, CG_TRIGGER, CG_CHARACTER);
	}
}

void CBaseTrigger::removePhysBody()
{
	if(m_pGhostObject)
	{
		SPhysics_GetDynWorld()->removeCollisionObject(m_pGhostObject);
		mem_delete(m_pGhostObject);
	}
}

void CBaseTrigger::onTouchStart(CBaseEntity *pActivator)
{
	FIRE_OUTPUT(m_onTouchStart, pActivator);
}
void CBaseTrigger::onTouchEnd(CBaseEntity *pActivator)
{
	FIRE_OUTPUT(m_onTouchEnd, pActivator);
}
void CBaseTrigger::onTouchEndAll(CBaseEntity *pActivator)
{
	FIRE_OUTPUT(m_onTouchEndAll, pActivator);
}

void CBaseTrigger::onSync()
{
	BaseClass::onSync();

	static const bool *dev_show_triggers = GET_PCVAR_BOOL("dev_show_triggers");

	if(m_pModel && *dev_show_triggers != m_isModelEnabled)
	{
		m_isModelEnabled = *dev_show_triggers;
		m_pModel->enable(m_isModelEnabled);
	}

	if(!m_pGhostObject || !m_bEnabled)
	{
		return;
	}
	m_aNewTouches.clearFast();

	btManifoldArray manifoldArray;
	btBroadphasePairArray &pairArray = m_pGhostObject->getOverlappingPairCache()->getOverlappingPairArray();
	int iTouches = pairArray.size();
	for(int i = 0; i < iTouches; ++i)
	{
		const btBroadphasePair &pair = pairArray[i];
		btBroadphasePair *pCollisionPair = SPhysics_GetDynWorld()->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
		
		if(pCollisionPair && pCollisionPair->m_algorithm)
		{
			manifoldArray.resize(0);
			pCollisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

			for(int j = 0, jl = manifoldArray.size(); j < jl; ++j)
			{
				if(manifoldArray[j]->getNumContacts() > 0)
				{
					const btCollisionObject *pObject = (manifoldArray[0]->getBody0() == m_pGhostObject)
						? manifoldArray[0]->getBody1()
						: manifoldArray[0]->getBody0();

					CBaseEntity *pEnt = (CBaseEntity*)pObject->getUserPointer();
					if(pEnt)
					{
						m_aNewTouches.push_back(pEnt);
						//printf("touched %s\n", pEnt->getClassName());
					}
					break;
				}
			}
			//printf("m=%d ", manifoldArray.size());
		}
	}
	//m_pGhostObject->getOverlappingObject(0);
	//printf("%d\n", m_iTouches);
}

void CBaseTrigger::update(float dt)
{
	if(!m_bEnabled)
	{
		return;
	}

	
	bool bFound;
	for(int i = 0, l = m_aNewTouches.size(); i < l; ++i)
	{
		bFound = false;
		for(int j = 0, jl = m_aTouches.size(); j < jl; ++j)
		{
			if(m_aNewTouches[i] == m_aTouches[j])
			{
				m_aTouches[j] = NULL;
				bFound = true;
				break;
			}
		}
		if(!bFound)
		{
			onTouchStart(m_aNewTouches[i]);
		}
	}
	CBaseEntity * pLastTouch = NULL;
	for(int j = 0, jl = m_aTouches.size(); j < jl; ++j)
	{
		if(m_aTouches[j])
		{
			onTouchEnd(m_aTouches[j]);
			pLastTouch = m_aTouches[j];
		}
	}
	if(pLastTouch && !m_aNewTouches.size())
	{
		onTouchEndAll(m_aTouches[0]);
	}
	m_aTouches.swap(m_aNewTouches);
}


void CBaseTrigger::setPos(const float3 & pos)
{
	BaseClass::setPos(pos);
	if(m_pGhostObject)
	{
		m_pGhostObject->getWorldTransform().setOrigin(F3_BTVEC(pos));

		SPhysics_GetDynWorld()->updateSingleAabb(m_pGhostObject);
	}
}

void CBaseTrigger::setOrient(const SMQuaternion & q)
{
	BaseClass::setOrient(q);
	if(m_pGhostObject)
	{
		m_pGhostObject->getWorldTransform().setRotation(Q4_BTQUAT(q));

		SPhysics_GetDynWorld()->updateSingleAabb(m_pGhostObject);
	}
}
