
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Анимированный игровой объект
*/

/*! \defgroup cbaseanimating Моделированные объекты
\ingroup cbaseentity
@{
*/

#ifndef __BASE_ANIMATING_H
#define __BASE_ANIMATING_H

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

	virtual void setModel(const char * mdl);

	float3 getAttachmentPos(int id);
	SMQuaternion getAttachmentRot(int id);

	void onSync();

	void playAnimation(const char * name, UINT uFadeTime = 0, UINT slot = 0);
	void playActivity(const char * name, UINT uFadeTime = 0, UINT slot = 0);

	void playAnimationNext(const char * name, UINT uFadeTime = 0, UINT slot = 0);
	void playActivityNext(const char * name, UINT uFadeTime = 0, UINT slot = 0);

	void cancelNextAnimation(int iSlot = -1);

	bool playingAnimations(const char* name);
	void setPos(const float3 & pos);
	void setOrient(const SMQuaternion & q);

	void setSkin(int iSkin);
	
	void setCollisionGroup(COLLISION_GROUP group, COLLISION_GROUP mask = CG_ALL);
	COLLISION_GROUP getCollisionGroup();

protected:

	void inputPlayAnim(inputdata_t * pInputdata);
	void inputPlayAnimNext(inputdata_t * pInputdata);

	void inputPlayActivity(inputdata_t * pInputdata);
	void inputPlayActivityNext(inputdata_t * pInputdata);

	void inputSetSkin(inputdata_t * pInputdata);

	IAnimPlayer * m_pAnimPlayer;
	const char * m_szModelFile;
	float m_fBaseScale;
	bool m_isStatic;

	CBaseEntity *m_pEntColorRef = NULL;
	float3_t m_vGlowColor;

	virtual void initPhysics();
	virtual void releasePhysics();
	virtual void createPhysBody();
	virtual void removePhysBody();

	btCollisionShape * m_pCollideShape;
	btRigidBody * m_pRigidBody;

	virtual void _cleanup();

	virtual void onAnimationStateChanged(int slot, ANIM_STATE as);

	void onIsStaticChange(bool isStatic);

	int m_iSkin = 0;

	struct
	{
		char szName[MODEL_MAX_NAME];
		UINT uFadeTime;
		bool isActivity;
	} m_vNextAnim[BLEND_MAX];

private:
	COLLISION_GROUP m_collisionGroup;
	COLLISION_GROUP m_collisionMask = CG_ALL;
};

#endif

/*! @} cbaseanimating */
