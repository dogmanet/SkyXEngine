
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PropDoor.h"

/*! \skydocent prop_door
Дверь
*/

BEGIN_PROPTABLE(CPropDoor)
	//! Открыть
	DEFINE_INPUT(inputOpen, "open", "Open", PDF_NONE)
	//! Закрыть
	DEFINE_INPUT(inputClose, "close", "Close", PDF_NONE)
	//! Заблокировать
	DEFINE_INPUT(inputLock, "lock", "Lock", PDF_NONE)
	//! Разблокировать
	DEFINE_INPUT(inputUnlock, "unlock", "Unlock", PDF_NONE)
	//! Переключить
	DEFINE_INPUT(inputToggle, "toggle", "Toggle", PDF_NONE)

	//! При начале закрытия
	DEFINE_OUTPUT(m_onClose, "OnClose", "On close")
	//! При завершении закрытия
	DEFINE_OUTPUT(m_onClosed, "OnClosed", "On closed")
	//! При начале открытия
	DEFINE_OUTPUT(m_onOpen, "OnOpen", "On open")
	//! При завершении открытия
	DEFINE_OUTPUT(m_onOpened, "OnOpened", "On opened")
	//! При попытке использовать заблокированную
	DEFINE_OUTPUT(m_onUseLocked, "OnUseLocked", "On use locked")

	//! Время до автозакрытия
	DEFINE_FIELD_FLOAT(m_fAutoCloseTime, PDFF_NONE, "autoclose_time", "Autoclose time", EDITOR_TIMEFIELD)
	//! Повреждение при блокировке
	DEFINE_FIELD_FLOAT(m_fBlockDamage, PDFF_NONE, "block_damage", "Block damage", EDITOR_TEXTFIELD)
	//! Величина смещение (0-авто)
	DEFINE_FIELD_FLOAT(m_fDistanceOverride, PDFF_NONE, "distance_override", "Distance override", EDITOR_TEXTFIELD)
	//! Скорость, м/с
	DEFINE_FIELD_FLOAT(m_fSpeed, PDFF_NONE, "speed", "Speed", EDITOR_TEXTFIELD)
	//! Направление открытия
	DEFINE_FIELD_ANGLES(m_qAngle, PDFF_NONE, "open_angle", "Open angle", EDITOR_ANGLES)

	DEFINE_FIELD_STRING(m_szSndClose, PDFF_NONE, "snd_close", "Close sound", EDITOR_FILEFIELD)
		FILE_OPTION("Select sound", "ogg")
		EDITOR_FILE_END()
	DEFINE_FIELD_STRING(m_szSndOpen, PDFF_NONE, "snd_open", "Open sound", EDITOR_FILEFIELD)
		FILE_OPTION("Select sound", "ogg")
		EDITOR_FILE_END()
	DEFINE_FIELD_STRING(m_szSndLocked, PDFF_NONE, "snd_locked", "Locked sound", EDITOR_FILEFIELD)
		FILE_OPTION("Select sound", "ogg")
		EDITOR_FILE_END()

	//! Изначально заблокирована
	DEFINE_FLAG(DOOR_START_LOCKED, "Start locked")
	//! Запрет открытия игроком
	DEFINE_FLAG(DOOR_NO_USE, "Disable player USE")
	//! Изначально открыта
	DEFINE_FLAG(DOOR_START_OPENED, "Start opened")
	//! Автозакрытие по таймеру
	DEFINE_FLAG(DOOR_AUTOCLOSE, "Autoclose")
	//! Форсированное закрытие двери
	DEFINE_FLAG(DOOR_FORCE, "Force close")
END_PROPTABLE()

REGISTER_ENTITY(CPropDoor, prop_door);

CPropDoor::~CPropDoor()
{
	releasePhysics();

	mem_release(m_pSndClose);
	mem_release(m_pSndOpen);
	mem_release(m_pSndLocked);
}

void CPropDoor::onPostLoad()
{
	m_isLocked = (getFlags() & DOOR_START_LOCKED) != 0;
	m_bState = (getFlags() & DOOR_START_OPENED) != 0;

	BaseClass::onPostLoad();

	IXSoundSystem *pSound = (IXSoundSystem*)(Core_GetIXCore()->getPluginManager()->getInterface(IXSOUNDSYSTEM_GUID));
	if(pSound)
	{
		IXSoundLayer *pGameLayer = pSound->findLayer("xGame");

		if(m_szSndClose[0])
		{
			m_pSndClose = pGameLayer->newSoundPlayer(m_szSndClose, SOUND_SPACE_3D);
		}
		if(m_szSndOpen[0])
		{
			m_pSndOpen = pGameLayer->newSoundPlayer(m_szSndOpen, SOUND_SPACE_3D);
		}
		if(m_szSndLocked[0])
		{
			m_pSndLocked = pGameLayer->newSoundPlayer(m_szSndLocked, SOUND_SPACE_3D);
		}
	}
}

void CPropDoor::inputOpen(inputdata_t * pInputdata)
{
	if(m_isLocked)
	{
		FIRE_OUTPUT(m_onUseLocked, pInputdata->pInflictor);

		SAFE_CALL(m_pSndLocked, setWorldPos, getPos());
		SAFE_CALL(m_pSndLocked, play);
		return;
	}
	if(m_bState)
	{
		return;
	}
	m_bState = true;
	if(m_isClosing)
	{
		stop();
	}

	m_pInflictor = pInputdata->pInflictor;
	FIRE_OUTPUT(m_onOpen, pInputdata->pInflictor);

	SAFE_CALL(m_pSndOpen, setWorldPos, getPos());
	SAFE_CALL(m_pSndOpen, play);

	m_isOpening = true;
	m_idThinkInterval = SET_INTERVAL(think, 0);
}
void CPropDoor::inputClose(inputdata_t * pInputdata)
{
	if(m_isLocked)
	{
		FIRE_OUTPUT(m_onUseLocked, pInputdata->pInflictor);

		SAFE_CALL(m_pSndLocked, setWorldPos, getPos());
		SAFE_CALL(m_pSndLocked, play);
		return;
	}
	if(!m_bState)
	{
		return;
	}
	m_bState = false;
	if(m_isOpening)
	{
		stop();
	}

	if(ID_VALID(m_idAutoCloseTimeout))
	{
		CLEAR_TIMEOUT(m_idAutoCloseTimeout);
		m_idAutoCloseTimeout = -1;
	}

	m_pInflictor = pInputdata->pInflictor;
	FIRE_OUTPUT(m_onClose, pInputdata->pInflictor);

	SAFE_CALL(m_pSndClose, setWorldPos, getPos());
	SAFE_CALL(m_pSndClose, play);

	m_isClosing = true;
	m_idThinkInterval = SET_INTERVAL(think, 0);
}
void CPropDoor::inputToggle(inputdata_t * pInputdata)
{
	if(m_bState)
	{
		inputClose(pInputdata);
	}
	else
	{
		inputOpen(pInputdata);
	}
}
void CPropDoor::inputLock(inputdata_t * pInputdata)
{
	m_isLocked = true;
}
void CPropDoor::inputUnlock(inputdata_t * pInputdata)
{
	m_isLocked = false;
}

void CPropDoor::onUse(CBaseEntity *pUser)
{
	if(getFlags() & DOOR_NO_USE)
	{
		return;
	}

	inputdata_t inputdata;
	memset(&inputdata, 0, sizeof(inputdata_t));
	inputdata.pActivator = pUser;
	inputdata.pInflictor = pUser;
	inputdata.type = PDF_NONE;

	if(getFlags() & DOOR_AUTOCLOSE)
	{
		inputOpen(&inputdata);
	}
	else
	{
		inputToggle(&inputdata);
	}

	BaseClass::onUse(pUser);
}

void CPropDoor::createPhysBody()
{
	if(m_pCollideShape)
	{
		float3 vPos = getPos();
		SMQuaternion qRot = getOrient();

		GetPhysics()->newGhostObject(&m_pGhostObject, true);
		m_pGhostObject->setPosition(vPos);
		m_pGhostObject->setRotation(qRot);
		m_pGhostObject->setCollisionShape(m_pCollideShape);
		m_pGhostObject->setCollisionFlags(XCF_KINEMATIC_OBJECT);
		m_pGhostObject->setUserPointer(this);
		m_pGhostObject->setUserTypeId(1);
		
		GetPhysWorld()->addCollisionObject(m_pGhostObject, CG_DOOR, CG_CHARACTER | CG_DEFAULT);

		//m_pGhostObject->activate();
		//m_pGhostObject->setActivationState(DISABLE_DEACTIVATION);

		float3 vDir = m_qAngle * DOOR_BASE_DIR;

		if(m_fDistanceOverride > 0.0f)
		{
			m_fDistance = m_fDistanceOverride;
		}
		else
		{
			SMAABB aabb = m_pGhostObject->getAABB();

			aabb.vMin += float3(m_pCollideShape->getMargin());
			aabb.vMax -= float3(m_pCollideShape->getMargin());

			float4 v0(
				SMVector3Dot(float3(aabb.vMin.x, aabb.vMin.y, aabb.vMin.z), vDir),
				SMVector3Dot(float3(aabb.vMin.x, aabb.vMin.y, aabb.vMax.z), vDir),
				SMVector3Dot(float3(aabb.vMin.x, aabb.vMax.y, aabb.vMin.z), vDir),
				SMVector3Dot(float3(aabb.vMin.x, aabb.vMax.y, aabb.vMax.z), vDir)
			);
			float4 v1(
				SMVector3Dot(float3(aabb.vMax.x, aabb.vMin.y, aabb.vMin.z), vDir),
				SMVector3Dot(float3(aabb.vMax.x, aabb.vMin.y, aabb.vMax.z), vDir),
				SMVector3Dot(float3(aabb.vMax.x, aabb.vMax.y, aabb.vMin.z), vDir),
				SMVector3Dot(float3(aabb.vMax.x, aabb.vMax.y, aabb.vMax.z), vDir)
			);
			float4 v4Min = SMVectorMin(v0, v1);
			float4 v4Max = SMVectorMax(v0, v1);

			float fMin = min(min(v4Min.x, v4Min.y), min(v4Min.z, v4Min.w));
			float fMax = max(max(v4Max.x, v4Max.y), max(v4Max.z, v4Max.w));

			m_fDistance = fMax - fMin;
		}

		m_vStartPos = vPos;
		m_vEndPos = (float3)(m_vStartPos + vDir * m_fDistance);

		if(m_bState)
		{
			setPos(m_vEndPos);
			m_fPositionFrac = 1.0f;
		}
	}
}
void CPropDoor::removePhysBody()
{
	if(m_pGhostObject)
	{
		GetPhysWorld()->removeCollisionObject(m_pGhostObject);
		mem_release(m_pGhostObject);
	}

	BaseClass::removePhysBody();
}

void CPropDoor::think(float fDT)
{
	testPenetration();

	if(m_isOpening)
	{
		m_fPositionFrac += fDT * m_fSpeed / m_fDistance;
		if(m_fPositionFrac >= 1.0f)
		{
			m_fPositionFrac = 1.0f;
			FIRE_OUTPUT(m_onOpened, m_pInflictor);
			stop();

			if(getFlags() & DOOR_AUTOCLOSE)
			{
				m_idAutoCloseTimeout = SET_TIMEOUT(close, m_fAutoCloseTime);
			}
		}
	}
	else if(m_isClosing)
	{
		m_fPositionFrac -= fDT * m_fSpeed / m_fDistance;
		if(m_fPositionFrac <= 0.0f)
		{
			m_fPositionFrac = 0.0f;
			FIRE_OUTPUT(m_onClosed, m_pInflictor);
			stop();
		}
	}

	float2 p1(0.0f, 0.0f);
	float2 p2(0.42f, 0.0f);
	float2 p3(0.58f, 1.0f);
	float2 p4(1.0f, 1.0f);
	float t = m_fPositionFrac;
	float2 p = powf(1.0f - t, 3.0f) * p1 + 3.0f * powf(1.0f - t, 2.0f) * t * p2 + 3.0f * (1.0f - t) * powf(t, 2.0f) * p3 + powf(t, 3.0f) * p4;
	t = p.y;

	setPos(SMVectorLerp(m_vStartPos, m_vEndPos, t));
}

void CPropDoor::stop()
{
	if(ID_VALID(m_idThinkInterval))
	{
		CLEAR_INTERVAL(m_idThinkInterval);
		m_idThinkInterval = -1;
	}
	m_isOpening = false;
	m_isClosing = false;

	//@TODO: Stop sound
}

void CPropDoor::setPos(const float3 & pos)
{
	BaseClass::setPos(pos);
	if(m_pGhostObject)
	{
		m_pGhostObject->setPosition(pos);
		//m_pGhostObject->setInterpolationLinearVelocity(btVector3(-0.02f, 0.0f, 0.0f));
	}
}

bool CPropDoor::testPenetration()
{
	bool hasContact = false;

	for(UINT i = 0, l = m_pGhostObject->getOverlappingPairCount(); i < l; ++i)
	{

		IXCollisionPair *collisionPair = m_pGhostObject->getOverlappingPair(i);
		
		IXCollisionObject* obj0 = collisionPair->getObject0();
		IXCollisionObject* obj1 = collisionPair->getObject1();

		if((obj0 && !obj0->hasContactResponse()) || (obj1 && !obj1->hasContactResponse()))
			continue;

		if(!needsCollision(obj0, obj1))
			continue;

		for(UINT j = 0, jl = collisionPair->getContactManifoldCount(); j < jl; ++j)
		{
			IXContactManifold *manifold = collisionPair->getContactManifold(j);
			//btScalar directionSign = manifold->getBody0() == m_pGhostObject ? btScalar(-1.0) : btScalar(1.0);
			for(UINT p = 0, pl = manifold->getContactCount(); p<pl; ++p)
			{
				const IXContactManifoldPoint *pt = manifold->getContact(p);

				float dist = pt->getDistance();

				if(dist < -DOOR_MAX_PENETRATION_DEPTH)
				{
					hasContact = true;
					const IXCollisionObject *pObject = (obj0 == m_pGhostObject) ? obj1 : obj0;

					if(pObject->getUserPointer() && pObject->getUserTypeId() == 1)
					{
						CBaseEntity *pEnt = (CBaseEntity*)pObject->getUserPointer();
						if(pEnt)
						{
							CTakeDamageInfo takeDamageInfo(this, m_fBlockDamage);
							takeDamageInfo.m_pInflictor = this;

							pEnt->dispatchDamage(takeDamageInfo);
						}
					}
				}
			}

			//manifold->clearManifold();
		}
	}

	if(hasContact)
	{
		if(!(getFlags() & DOOR_FORCE))
		{
			inputdata_t inputdata;
			memset(&inputdata, 0, sizeof(inputdata_t));
			inputdata.pActivator = this;
			inputdata.pInflictor = m_pInflictor;
			inputdata.type = PDF_NONE;
			inputToggle(&inputdata);
			return(false);
		}
		else
		{
			return(true);
		}
	}
	return(false);
}

bool CPropDoor::needsCollision(const IXCollisionObject *body0, const IXCollisionObject *body1)
{
	bool collides = (body0->getFilterGroup() & body1->getFilterMask()) != 0;
	collides = collides && (body1->getFilterGroup() & body0->getFilterMask());
	return(collides);
}

void CPropDoor::close(float fDT)
{
	m_idAutoCloseTimeout = -1;

	inputdata_t inputdata;
	memset(&inputdata, 0, sizeof(inputdata_t));
	inputdata.pActivator = this;
	inputdata.pInflictor = m_pInflictor;
	inputdata.type = PDF_NONE;
	inputClose(&inputdata);
}
