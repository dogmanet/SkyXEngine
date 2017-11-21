
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Класс игрока
*/

/*! \ingroup cbaseanimating
@{
*/

#ifndef _SXplayer_H_
#define _SXplayer_H_

#include "SXbaseAnimating.h"
#include "SXpointCamera.h"
#include "LightDirectional.h"
#include "crosshair.h"

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

class SXbaseTool;

//! Класс игрока  \ingroup cbaseanimating
class SXplayer: public SXbaseAnimating
{
	DECLARE_CLASS(SXplayer, SXbaseAnimating);
	DECLARE_PROPTABLE();
public:
	SXplayer(EntityManager * pMgr);
	~SXplayer();
	
	//! Возвращает камеру игрока
	SXpointCamera * GetCamera();

	//! Переключает в режим наблюдателя
	void Observe()
	{
		m_uMoveDir |= PM_OBSERVER;
		m_pCrosshair->Enable(false);
	}
	//! Помещает игрока в мир
	void Spawn();

	/*! Запускает/останавливает движение в заданном направлении
		@param [in] dir направление движения из #PLAYER_MOVE
	*/
	void Move(UINT dir, bool start);

	//! Обновляет инпут от игрока
	virtual void UpdateInput(float dt);

	void OnSync();

	//! Устанавливает положение в мире
	void SetPos(const float3 & pos);

	//! Запускает/останавливает первичную атаку
	void Attack(BOOL state);
	//! Запускает/останавливает вторичную атаку
	void Attack2(BOOL state);
	//! Запускает перезарядку активного оружия
	void Reload();
	//! Включает/выключает фонарь
	void ToggleFlashlight();
	//! Переключает режим стрельбы активного оружия
	void nextFireMode();

	/*! Возаращает мировую позицию для модели оружия
		\note Устарело?
	*/
	float3 GetWeaponOrigin();
	//! Получает смещения для задержки движения модели оружия при вращении игрока
	float3_t & GetWeaponDeltaAngles();

	//! Команды для подбора положения визуальной модели оружия на экране
	void _ccmd_slot_on(int argc, const char ** argv);
	void _ccmd_slot_off();

	//! Получает объект перекрестия
	Crosshair * GetCrosshair();

	//! Находится ли игрок на земле
	bool onGround();

	//! Воспроизводит звук шагов с учетом материала на котором стоит игрок
	void playFootstepsSound();

protected:
	//! Фонарик
	CLightDirectional* m_flashlight;
	//! Камера
	SXpointCamera * m_pCamera;

	//! Текущее движение
	UINT m_uMoveDir;

	//! ID интервала обновления
	ID m_iUpdIval;

	//! Углы вращения игрока
	float3_t m_vPitchYawRoll;

	//! Для физики @{
	btCollisionShape * m_pCollideShape;
	btRigidBody * m_pRigidBody;
	btPairCachingGhostObject * m_pGhostObject;
	btKinematicCharacterController * m_pCharacter;
	//! @}

	//! Текущий инструмент в руках
	SXbaseTool * m_pActiveTool;

	//! Может ли прыгать
	bool m_canJump;

	//! Для качания камеры @{
	float m_fViewbobStep;
	float m_fViewbobY;
	float3_t m_fViewbobStrafe;
	float3_t m_vWpnShakeAngles;
	//! @}

	int m_iDSM;

	//! Перекрестие
	Crosshair * m_pCrosshair;

	ID m_idQuadCurr;	//!< текущий квад аи сетки на котором стоит игрок
};

#endif

//! @}
