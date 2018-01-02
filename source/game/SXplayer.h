
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

#include "CBaseCharacter.h"
#include "SXpointCamera.h"
#include "crosshair.h"

//! Класс игрока  \ingroup cbaseanimating
class SXplayer: public CBaseCharacter
{
	DECLARE_CLASS(SXplayer, CBaseCharacter);
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

protected:
	//! Камера
	SXpointCamera * m_pCamera;

	//! ID интервала обновления
	ID m_iUpdIval;

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

	//! Обновляет разброса значение
	virtual void updateSpread(float dt);
};

#endif

//! @}
