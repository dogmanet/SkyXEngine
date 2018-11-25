
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый класс npc
*/

/*! \ingroup cbaseanimating
@{
*/

#ifndef __NPCBASE_H
#define __NPCBASE_H

#include "BaseCharacter.h"
#include <aigrid/sxaigrid.h>

//! базовое направление для нпс
#define NPC_BASE_DIR float3(0,0,-1)

//время в млсек для плавной ориентации на проходящий путь
#define NPC_TIME_ORIENT_IN_PATH 250

#define NPC_STEP_HEIGHT_EPSILON	0.05f

#define NPC_QUAD_DIST_NOTBEYOND	AIGRID_QUAD_SIZE
#define NPC_QUAD_DIST_BEYOND	AIGRID_QUAD_SIZE*2.f

//! состояния пути
enum NPC_STATE_PATH
{
	NPC_STATE_PATH_NOTFOUND		= 0,	//!< не найден путь
	NPC_STATE_PATH_BITBEYOND	= 1,	//!< за пределами (возвращение допустимо)
	NPC_STATE_PATH_BEYOND		= 2,	//!< за пределами (возвращение не допустимо)
	NPC_STATE_PATH_LOST			= 3,	//!< сбился с пути
	NPC_STATE_PATH_FOUND		= 4,	//!< найден
};

//! Базовый класс npc
class CNPCBase: public CBaseCharacter
{
	DECLARE_CLASS(CNPCBase, CBaseCharacter);
	DECLARE_PROPTABLE();

public:

	SX_ALIGNED_OP_MEM

	CNPCBase(CEntityManager * pMgr);
	~CNPCBase();

	ID getAIQuad();	//!< id квада аи сетки на котором стоит нпс

	//! установка позиции сопровождается ориентацией на аи сетке
	void setPos(const float3 &pos);


	//! Остановить моторику!
	void stopMotion(bool runIdleAnim=true);

	void onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor);

	//! Степень видимости объекта pOther. 0 - не виден, 1 - полностью виден
	float canSee(CBaseEntity *pOther);
	float canSee(CBaseCharacter *pOther);

	void setModel(const char * mdl);

	void orientAtPoint(const float3 &vPoint);
	void stopOrientAt();

protected:
	void onSync();

	//void think(float fDelta);

	//! процедура проверки найденности пути
	void findPathThinker(float fDelta);
	ID m_idFindPathInterval;

	//! процедура ориентации
	void orientAtThinker(float fDelta);
	ID m_idOrientAtInterval;
	SMQuaternion m_qOldOrient;
	float m_fOrientAtDelta;

	//! процедура движения по имеющемуся пути
	void pathFollowThinker(float fDelta);
	ID m_idPathFollowInterval;

	SMQuaternion m_qOrientTo;
	float3 m_vLastFramePos;
	int m_iStuckCount;

	void goTo(ID idQuad, bool bRun=false);
	void goTo(const float3 &vPos, bool bRun = false);

	//bool pathFind(ID endq);	//!< поиск пути от текущего (на котором стоит нпс) до endq
	//void pathWalk();		//!< хождение по пути
	//void gridCheckBeyond();	//!< проверка на выход за пределы сетки, и если так то установка соответсвующего состояния

	//! ориентаци нпс на точку pos, ttime время в млсек за которое нпс будет повернут в/на точку
	//void orientAtPoint(const float3 &pos, DWORD ttime);	
	//void updateOrientLerp();//!< плавная интерполяция поворотов
	
	float m_fSpeedWalk;	//!< скорость движения при ходьбе
	float m_fSpeedRun;	//!< скорость движения при беге

	Array<ID> m_aPathQuads;		//!< массив с идентификаторами квадов пути на аи сетке
	int m_iCurrQuaidInPath;		//!< номер текущего квада из m_arr_path

	ID m_idQueueFindPath;       //!< номер запроса на поиск пути

	ID m_idQuadGoingTo;         //!< номер квада, к которому идем
	
	float3_t m_vLastPathPos;		//!< последняя позиция нпс на пути

	//float3_t m_vPosQuadInPathLast;	//!< позиция последнего квада на котором был нпс
	//float3_t m_vPosQuadInPathNext;	//!< следующая позици квада на которую стремится нпс
	
	//float m_fAngleYLast;		//!< предыдущий угол поворота
	//float m_fAngleYNext;		//!< следующий угол поворота
	//DWORD m_ulTimeAllRot;		//!< общее время текущей ориентации
	//DWORD m_ulTimeRot;			//!< время за которое должен быть ориентирован 

	float m_fStepHeight;		//!< высота шага
	//bool m_canJump;				//!< прыгать ли сейчас
	NPC_STATE_PATH m_statePath;	//!< состояние пути
	DWORD m_ulColor;				//!< случайный (скорее всего уникальный) цвет нпс для метки квадов сетки
	//DWORD m_ulTimeReturnInGrid;		//!< за сколько млсек нпс должен вернуться на аи сетку
	//DWORD m_ulTimeAllReturnInGrid;	//!< общее время возвращения нпс на аи сетку

	bool m_isGoing;
	bool m_isPathInterrupted;

	float m_fViewConeAngle;      //!< Половина угла конуса видимости, в радианах
	float m_fViewDistance;      //!< Максимальное расстояние видимости 

	bool m_bRunMode; //! < перемещение бегом или нет
};

#endif

//! @}
