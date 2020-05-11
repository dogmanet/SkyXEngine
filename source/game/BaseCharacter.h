
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый класс персонажа
*/

/*! \ingroup cbaseanimating
@{
*/

#ifndef __BASE_CHARACTER_H
#define __BASE_CHARACTER_H

#include "BaseAnimating.h"
#include "LightDirectional.h"
#include "CharacterInventory.h"
#include "PointEntity.h"

class CBaseTool;

//! Типы движения игрока
enum PLAYER_MOVE
{
	PM_NONE = 0,
	PM_FORWARD = 0x01,   //!< вперед
	PM_BACKWARD = 0x02,  //!< назад
	PM_LEFT = 0x04,      //!< влево
	PM_RIGHT = 0x08,     //!< вправо
	PM_CROUCH = 0x10,    //!< присесть
	PM_JUMP = 0x20,      //!< прыгнуть
	PM_RUN = 0x40,       //!< бежать
	PM_CRAWL = 0x80,     //!< лежать
	PM_OBSERVER = 0x100, //!< наблюдатель

	PM_STOP = 0xFFFF
};

class CHUDcontroller;

//! Класс игрока  \ingroup cbaseanimating
class CBaseCharacter: public CBaseAnimating
{
	DECLARE_CLASS(CBaseCharacter, CBaseAnimating);
	DECLARE_PROPTABLE();
public:
	CBaseCharacter(CEntityManager * pMgr);
	~CBaseCharacter();

	//! Запускает/останавливает первичную атаку
	void attack(BOOL state);
	//! Запускает/останавливает вторичную атаку
	void attack2(BOOL state);
	//! Запускает перезарядку активного оружия
	void reload();
	//! Включает/выключает фонарь
	void toggleFlashlight();
	//! Переключает режим стрельбы активного оружия
	void nextFireMode();

	//! Находится ли игрок на земле
	bool onGround();

	//! Воспроизводит звук шагов с учетом материала на котором стоит игрок
	void playFootstepsSound();

	float getAimRange();

	void onInventoryChanged();

	//! Получает рассчитанный для текущего оружия коэффициент разброса (с учетом задержки сведения)
	float getCurrentSpread();

	btCollisionObject *getBtCollisionObject() const
	{
		return(m_pGhostObject);
	}
	void initHitboxes();
	void releaseHitboxes();
	void updateHitboxes();

	void onSync();

	void initPhysics();
	void releasePhysics();

	//! Устанавливает положение в мире
	void setPos(const float3 & pos);

	//void dispatchDamage(CTakeDamageInfo &takeDamageInfo);

	CCharacterInventory * getInventory()
	{
		return(m_pInventory);
	}

	virtual CHUDcontroller * getHUDcontroller();

	void onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor);

	CBaseEntity *getHead();

	virtual bool isBloody()
	{
		return(true);
	}

	bool isObserver();

	void use(bool start);

	IXResourceModelAnimated *getHandsResource()
	{
		return(m_pHandsModelResource);
	}

protected:
	//! Фонарик
	CLightDirectional* m_flashlight;

	//! Текущее движение
	UINT m_uMoveDir;

	//! Текущий инструмент в руках
	CBaseTool * m_pActiveTool;

	//! Для физики @{
	btCollisionShape * m_pCollideShape;
	btRigidBody * m_pRigidBody;
	btPairCachingGhostObject * m_pGhostObject;
	btKinematicCharacterController * m_pCharacter;
	btRigidBody ** m_pHitboxBodies;
	//! @}

	//! Углы вращения игрока
	float3_t m_vPitchYawRoll;

	//! Мгновенное значение коэффициента разброса
	float getMomentSpread();

	//! Задача обновления разброса
	ID m_idTaskSpread;

	//! Обновляет разброса значение
	virtual void updateSpread(float dt);

	//! Действующее значение разброса
	float m_fCurrentSpread;

	CCharacterInventory * m_pInventory;

	ID m_idQuadCurr;	//!< текущий квад аи сетки на котором стоит игрок
	ID m_idQuadLast;	//!< Последний валидный квад аи сетки на котором стоял игрок

	float m_fCapsHeight;
	float m_fCapsHeightCrouch;
	float m_fCapsRadius;

	CPointEntity * m_pHeadEnt;

	float m_fCurrentHeight;

	IXResourceModelAnimated *m_pHandsModelResource = NULL;
};

#endif

//! @}
