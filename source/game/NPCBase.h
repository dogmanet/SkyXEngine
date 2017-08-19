
#ifndef __NPCBASE_H_
#define __NPCBASE_H_

#include "SXbaseAnimating.h"
#include <aigrid/sxaigrid.h>

//! базовое направление дл€ нпс
#define NPC_BASE_DIR float3(0,0,-1)

//врем€ в млсек дл€ плавной ориентации на проход€щий путь
#define NPC_TIME_ORIENT_IN_PATH 250

#define NPC_STEP_HEIGHT_EPSILON	0.05f

#define NPC_QUAD_DIST_NOTBEYOND	AIGRID_QUAD_SIZE
#define NPC_QUAD_DIST_BEYOND	AIGRID_QUAD_SIZE*2.f

//! сост€ни€ движени€
enum NPC_STATE_MOVE
{
	NPC_STATE_MOVE_IDLE_START = 0,	//!< остановилс€
	NPC_STATE_MOVE_IDLE = 1,		//!< стоит
	NPC_STATE_MOVE_WALK = 2,		//!< идет
	NPC_STATE_MOVE_RUN = 4,			//!< бежит
};

//! состо€ни€ опасности
enum NPC_STATE_DANGER
{
	NPC_STATE_DANGER_CALM		= 0,	//!< спокоен
	NPC_STATE_DANGER_UNVISIBLE	= 1,	//!< беспокоен, но не знает где опасность
	NPC_STATE_DANGER_VISIBLE	= 2,	//!< беспокоен и знает где опасность
};

//! состо€ни€ пути
enum NPC_STATE_PATH
{
	NPC_STATE_PATH_NOTFOUND		= 0,	//!< не найден путь
	NPC_STATE_PATH_BITBEYOND	= 1,	//!< за пределами (возвращение допустимо)
	NPC_STATE_PATH_BEYOND		= 2,	//!< за пределами (возвращение не допустимо)
	NPC_STATE_PATH_LOST			= 3,	//!< сбилс€ с пути
	NPC_STATE_PATH_FOUND		= 4,	//!< найден
};

class CNPCBase : public SXbaseAnimating
{
	DECLARE_CLASS(CNPCBase, SXbaseAnimating);
	DECLARE_PROPTABLE();

public:

	SX_ALIGNED_OP_MEM

	CNPCBase(EntityManager * pMgr);
	~CNPCBase();

	ID getAIQuad();	//id квада аи сетки на котором стоит нпс

	//установка позиции сопровождаетс€ ориентацией на аи сетке
	void SetPos(const float3 &pos);
	bool SetKV(const char *name, const char *value);
	void OnSync();

protected:

	virtual void InitPhysics();

	btPairCachingGhostObject * m_pGhostObject;
	btKinematicCharacterController * m_pCharacter;

	bool pathFind(ID endq);	//!< поиск пути от текущего (на котором стоит нпс) до endq
	void pathWalk();		//!< хождение по пути
	void gridCheckBeyond();	//!< проверка на выход за пределы сетки, и если так то установка соответсвующего состо€ни€

	//! ориентаци нпс на точку pos, ttime врем€ в млсек за которое нпс будет повернут в/на точку
	void orientAtPoint(const float3 *pos, DWORD ttime);	
	void updateOrientLerp();//!< плавна€ интерпол€ци€ поворотов

	float m_fHealth;	//здоровье [0,1]

	float m_fSpeedWalk;	//скорость движени€ при ходьбе
	float m_fSpeedRun;	//скорость движени€ при беге

	ID m_idCurrAiQuad;			//текущий id квада на котором находитс€ нпс

	Array<ID> m_aPathQuads;		//массив с идентификаторами квадов пути на аи сетке
	ID m_idCurrQuaidInPath;		//номер текущего квада из m_arr_path
	
	float3 m_vLastPathPos;		//последн€€ позици€ нпс на пути

	float3 m_vPosQuadInPathLast;	//позици€ последнего квада на котором был нпс
	float3 m_vPosQuadInPathNext;	//следующа€ позици квада на которую стремитс€ нпс
	
	float m_fAngleYLast;		//предыдущий угол поворота
	float m_fAngleYNext;		//следующий угол поворота
	DWORD m_ulTimeAllRot;		//общее врем€ текущей ориентации
	DWORD m_ulTimeRot;			//врем€ за которое должен быть ориентирован 

	float m_fStepHeight;		//высота шага
	bool m_canJump;				//прыгать ли сейчас
	NPC_STATE_MOVE m_stateMove;	//состо€ние движени€
	NPC_STATE_PATH m_statePath;	//состо€ние пути
	DWORD m_ulColor;				//случайный (скорее всего уникальный) цвет нпс дл€ метки квадов сетки
	DWORD m_ulTimeReturnInGrid;		//за сколько млсек нпс должен вернутьс€ на аи сетку
	DWORD m_ulTimeAllReturnInGrid;	//общее врем€ возвращени€ нпс на аи сетку
};

#endif