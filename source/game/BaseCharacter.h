
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Базовый класс персонажа
*/

/*! \ingroup cbaseanimating
@{
*/

#ifndef _CBaseCharacter_H_
#define _CBaseCharacter_H_

#include "BaseAnimating.h"
#include "LightDirectional.h"

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


	//! Получает рассчитанный для текущего оружия коэффициент разброса (с учетом задержки сведения)
	float getCurrentSpread();

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
};

#endif

//! @}
