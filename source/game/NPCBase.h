
#ifndef _NPCBase_H_
#define _NPCBase_H_

#include "SXbaseAnimating.h"
#include <aigrid/sxaigrid.h>

//! базовое направление для нпс
#define NPC_BASE_DIR float3(0,0,-1)

//время в млсек для плавной ориентации на проходящий путь
#define NPC_TIME_ORIENT_IN_PATH 250

//! состяния движения
enum NPC_MOVE_STATE
{
	NPC_MOVE_STATE_IDLE_START = 0x0,	//!< остановился
	NPC_MOVE_STATE_IDLE = 0x1,	//!< стоит
	NPC_MOVE_STATE_WALK = 0x2,	//!< идет
	NPC_MOVE_STATE_RUN = 0x4,	//!< бежит
};

//! состояния опасности
enum NPC_DANGER_STATE
{
	NPC_DANGER_STATE_CALM = 0,		//!< спокоен
	NPC_DANGER_STATE_UNVISIBLE = 1,	//!< беспокоен, но не знает где опасность
	NPC_DANGER_STATE_VISIBLE = 2,	//!< беспокоен и знает где опасность
};

class NPCBase : public SXbaseAnimating
{
	DECLARE_CLASS(NPCBase, SXbaseAnimating);
	DECLARE_PROPTABLE();

public:

	SX_ALIGNED_OP_MEM

	NPCBase(EntityManager * pMgr);
	~NPCBase();

	ID GetAIQuad();	//id квада аи сетки на котором стоит нпс

	//установка позиции сопровождается ориентацией на аи сетке
	void SetPos(const float3 & pos);
	bool SetKV(const char * name, const char * value);
	void OnSync();

protected:

	virtual void InitPhysics();

	btPairCachingGhostObject * m_pGhostObject;
	btKinematicCharacterController * m_pCharacter;

	bool PathFind(ID endq);	//!< поиск пути от текущего (на котором стоит нпс) до endq
	void PathWalk();		//!< хождение по пути

	//! ориентаци нпс на точку pos, ttime время в млсек за которое нпс будет повернут в на точку
	void OrientAtPoint(const float3 * pos, DWORD ttime);	
	void UpdateOrientLerp();//!< плавная интерполяция поворотов

	float m_health;	//здоровье [0,1]

	float m_speed_walk;	//скорость движения при ходьбе
	float m_speed_run;	//скорость движения при беге

	float m_immunity_weapon;	//коэфициент иммунитета к хиту от оружия

	ID m_curr_aiquad;			//текущий id квада на котором находится нпс

	Array<ID> m_arr_path;		//массив с идентификаторами квадов пути на аи сетке
	ID m_curr_quaid_in_path;	//номер текущего квада из m_arr_path
	
	float3 m_last_path_pos;		//позиция последнего квада на котором был нпс
	float3 m_next_path_pos;		//следующая позици квада на которую стремится нпс
	
	float m_angle_y_last;		//предыдущий угол поворота
	float m_angle_y_next;		//следующий угол поворота
	DWORD m_alltime_rot;		//общее время текущей ориентации
	DWORD m_time_rot;			//время за которое должен быть ориентирован 

	NPC_MOVE_STATE m_move_state;
};

#endif