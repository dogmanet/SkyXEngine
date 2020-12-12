
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Дверь
*/

#ifndef __PROP_DOOR_H
#define __PROP_DOOR_H

#include "PropDynamic.h"

#define DOOR_START_LOCKED ENT_FLAG_0
#define DOOR_NO_USE ENT_FLAG_1
#define DOOR_START_OPENED ENT_FLAG_2
#define DOOR_AUTOCLOSE ENT_FLAG_3
#define DOOR_FORCE ENT_FLAG_4

//! базовое направление для двери
#define DOOR_BASE_DIR float3(0, 0, -1.0f)

#define DOOR_MAX_PENETRATION_DEPTH 0.16f

/*! Дверь
\ingroup cbaseanimating
*/
class CPropDoor: public CPropDynamic
{
	DECLARE_CLASS(CPropDoor, CPropDynamic);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CPropDoor();
	virtual void onPostLoad();

	void onUse(CBaseEntity *pUser);

	//! Устанавливает положение в мире
	void setPos(const float3 & pos);

protected:
	void inputOpen(inputdata_t * pInputdata);
	void inputClose(inputdata_t * pInputdata);
	void inputLock(inputdata_t * pInputdata);
	void inputUnlock(inputdata_t * pInputdata);
	void inputToggle(inputdata_t * pInputdata);

	void think(float fDT);
	ID m_idThinkInterval = -1;
	ID m_idAutoCloseTimeout = -1;

	void stop();

	bool testPenetration();
	bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);

	output_t m_onClose;
	output_t m_onClosed;
	output_t m_onOpen;
	output_t m_onOpened;
	output_t m_onUseLocked;

	virtual void createPhysBody();
	virtual void removePhysBody();

	float m_fAutoCloseTime = 0.0f;
	float m_fDistanceOverride = 0.0f;
	float m_fSpeed = 0.1f;
	SMQuaternion m_qAngle;
	float m_fBlockDamage = 100.0f;

	const char * m_szSndClose;
	IXSoundPlayer *m_pSndClose = NULL;
	const char * m_szSndOpen;
	IXSoundPlayer *m_pSndOpen = NULL;
	const char * m_szSndLocked;
	IXSoundPlayer *m_pSndLocked = NULL;

	bool m_isLocked = false;

	bool m_bState = false;

	btPairCachingGhostObject * m_pGhostObject = NULL;

private:
	float m_fDistance = 0.0f;

	float3_t m_vStartPos;
	float3_t m_vEndPos;

	float m_fPositionFrac = 0.0f;

	bool m_isOpening = false;
	bool m_isClosing = false;

	CBaseEntity *m_pInflictor = NULL;

	void close(float fDT);
};

#endif
