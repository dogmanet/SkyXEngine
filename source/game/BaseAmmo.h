
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Патроны
*/

#ifndef __BASE_AMMO_H
#define __BASE_AMMO_H

#include "BaseSupply.h"
#include "BaseCharacter.h"

#define BULLET_MAX_JUMPS 32
#define BULLET_STEP_DISTANCE 5.0f /* 5 метров */

/*! Базовый класс патронов
\ingroup cbaseitem
*/
class CBaseAmmo: public CBaseSupply
{
	DECLARE_CLASS(CBaseAmmo, CBaseSupply);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	//! Стреляет пулю
	void fire(const float3 &vStart, const float3 &vDir, CBaseCharacter *pAttacker=NULL);

	bool isAmmo() const
	{
		return(true);
	}

	float getStartSpeed() const;

protected:

	//! Начальная скорость
	float m_fStartSpeed;
	//! Масса пули
	float m_fBulletMass;
	//! Бронебойность
	float m_fArmorPiercing;
	//! Экспансивная?
	bool m_isExpansive;
	//! Разрывная?
	bool m_isBursting;

	struct HitPoint
	{
		float3_t vPosition;
		float3_t vNormal;
		const btCollisionObject *pCollisionObject;
		float fFraction;
		bool isExit;
		btCollisionWorld::LocalShapeInfo shapeInfo;
	};

	//! Рисует декаль в точке попадания
	void shootDecal(const float3 &vPos, const float3 &vNormal, ID idMtl);
	void shootBlood(const float3 &vPos, const float3 &vNormal);

	//! Определяет, был ли рикошет
	bool shouldRecochet(const float3 &vPos, const float3 &vNormal, const float3 &vDir, ID idMtl, float fSpeed, float3 *pvNewDir, float *pfNewSpeed);

	//! Определяет, может ли пуля пробить препятствие
	bool canHole(float fDurability, float fCurrentSpeed, float *pfNewSpeed);

	//! Хранит толщину препятствия во время расчета выстрела. Изменяется во время полета от препятствия к препятствию
	float m_fNextBarrierDepth;

	void fire(const float3 &vStart, const float3 &vDir, CBaseCharacter *pAttacker, float fSpeed);
};

#endif
