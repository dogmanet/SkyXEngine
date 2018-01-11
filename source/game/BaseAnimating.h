
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Анимированный игровой объект
*/

/*! \defgroup cbaseanimating Моделированные объекты
\ingroup cbaseentity
@{
*/

#ifndef _CBaseAnimating_H_
#define _CBaseAnimating_H_

#include "BaseEntity.h"
#include <anim/sxanim.h>

//! Анимированный игровой объект
class CBaseAnimating: public CBaseEntity
{
	DECLARE_CLASS(CBaseAnimating, CBaseEntity);
	DECLARE_PROPTABLE();
public:
	CBaseAnimating(CEntityManager * pMgr);
	~CBaseAnimating();

	void getMinMax(float3 * min, float3 * max);
	void getSphere(float3 * center, float * radius);

	bool setKV(const char * name, const char * value);

	void setModel(const char * mdl);

	float3 getAttachmentPos(int id);
	SMQuaternion getAttachmentRot(int id);

	void onSync();

	void playAnimation(const char * name, UINT iFadeTime = 0, UINT slot = 0);
	void playActivity(const char * name, UINT iFadeTime = 0, UINT slot = 0);


	bool playingAnimations(const char* name);
	void setPos(const float3 & pos);
	void setOrient(const SMQuaternion & q);

protected:
	IAnimPlayer * m_pAnimPlayer;
	const char * m_szModelFile;
	float m_fBaseScale;

	virtual void initPhysics();
	virtual void createPhysBody();
	void releasePhysics();
	virtual void removePhysBody();

	btCollisionShape * m_pCollideShape;
	btRigidBody * m_pRigidBody;
};

#endif

/*! @} cbaseanimating */
