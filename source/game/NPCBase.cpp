
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "NPCBase.h"

/*! \skydocent npc_base
Базовый класс для NPC
*/

BEGIN_PROPTABLE(CNPCBase)
// empty
	
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CNPCBase, npc_base);

CNPCBase::CNPCBase(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	m_fSpeedWalk = 0.07f;
	m_fSpeedRun = 0.12f;
	m_idCurrQuaidInPath = -1;
	m_fBaseScale = 0.01f;
	m_idCurrAiQuad = -1;
	m_stateMove = NPC_STATE_MOVE_IDLE_START;
	m_statePath = NPC_STATE_PATH_NOTFOUND;

	m_fAngleYLast = 0;
	m_fAngleYNext = 0;
	m_ulTimeAllRot = 0;
	m_ulTimeRot = 0;
	m_fStepHeight = 0.4f;
	m_canJump = false;
	m_ulColor = D3DCOLOR_ARGB(128, rand() % 255, rand() % 255, rand() % 255);
	m_ulTimeReturnInGrid = m_ulTimeAllReturnInGrid = 0;
}

CNPCBase::~CNPCBase()
{

}

void CNPCBase::setPos(const float3 &pos)
{
	float3 tpos = pos;
	m_idCurrAiQuad = SAIG_QuadGet(&tpos, true);

	if (m_idCurrAiQuad >= 0)
	{
		if (SAIG_QuadGetState(m_idCurrAiQuad) == AIQUAD_STATE_FREE)
		{
			SAIG_QuadSetState(m_idCurrAiQuad, AIQUAD_STATE_BUSY);
			SAIG_QuadSetStateWho(m_idCurrAiQuad, getId());
		}
		else
		{
			if (SAIG_QuadGetStateWho(m_idCurrAiQuad) != getId())
			{
				ID idquad = SAIG_QuadGetNear(&(float3)m_vPosition, true, 2);
				if (idquad >= 0)
				{
					m_idCurrAiQuad = idquad;
					SAIG_QuadSetState(m_idCurrAiQuad, AIQUAD_STATE_BUSY);
					SAIG_QuadSetStateWho(m_idCurrAiQuad, getId());
					SAIG_QuadGetPos(m_idCurrAiQuad, &tpos);
					tpos.y += 0.7f;
					setPos(tpos);
				}
			}
		}
	}

	BaseClass::setPos(tpos);
	m_pGhostObject->getWorldTransform().setOrigin(F3_BTVEC(tpos));
}

bool CNPCBase::setKV(const char *name, const char *value)
{
	if(stricmp("origin", name) == 0)
	{
		propdata_t * field = getField(name);
		if (!field)
		{
			return(false);
		}
		float3_t f3;

		if(3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
		{
			setPos(f3);
			return true;
		}
		else
			return false;
	}

	return BaseClass::setKV(name, value);
}

ID CNPCBase::getAIQuad()
{
	return m_idCurrAiQuad;
}

bool CNPCBase::pathFind(ID endq)
{
	if(m_idCurrAiQuad >= 0 && SAIG_GridFindPath(m_idCurrAiQuad, endq))
	{
		if (m_aPathQuads.size() > 0)
			SAIG_GridSetColorArr(&(m_aPathQuads[0]), 0, m_aPathQuads.size());
		m_statePath = NPC_STATE_PATH_FOUND;
		m_aPathQuads.resize(SAIG_GridGetSizePath());
		SAIG_GridGetPath(&(m_aPathQuads[0]), m_aPathQuads.size(), true);
		SAIG_GridSetColorArr(&(m_aPathQuads[0]), m_ulColor, m_aPathQuads.size());
		m_vLastPathPos = m_vPosition;
		return true;
	}
	
	m_statePath = NPC_STATE_PATH_NOTFOUND;
	return false;
}

void CNPCBase::onSync()
{
	BaseClass::onSync();

	btTransform trans;
	trans = m_pGhostObject->getWorldTransform();

	m_vPosition = (float3)(float3(trans.getOrigin().x(), trans.getOrigin().y()-0.9f, trans.getOrigin().z()));

	if (m_fHealth <= 0.f)
		return;

	//определяем текущий id квада аи сетки
	m_idCurrAiQuad = SAIG_QuadGet(&(float3)m_vPosition, true);

	//если квад найти не удалось и состояние пути не равно немного/полностью вышел за пределы
	if (m_idCurrAiQuad < 0 && (m_statePath != NPC_STATE_PATH_BITBEYOND || m_statePath != NPC_STATE_PATH_BEYOND))
	{
		//если мы не движемся по пути
		if (m_idCurrQuaidInPath < 0 || m_idCurrQuaidInPath >= m_aPathQuads.size())
		{
			gridCheckBeyond();
		}
		//иначе мы движемся по пути, проверяем как далеко лежат предыдущий и следующий квады
		else
		{
			//если это не первый квад на пути, и дистанция между центром предыдущего квада и позицией нпс лежит в пределах допустимого
			if (m_idCurrQuaidInPath > 0 && SMVector3Distance(m_vPosition, m_vPosQuadInPathLast) <= NPC_QUAD_DIST_NOTBEYOND)
				m_idCurrAiQuad = m_aPathQuads[m_idCurrQuaidInPath-1];
			//иначе если дистанция между центром следующего квада и позицией нпс лежит в пределах допустимого
			else if (SMVector3Distance(m_vPosition, m_vPosQuadInPathNext) <= NPC_QUAD_DIST_NOTBEYOND)
				m_idCurrAiQuad = m_aPathQuads[m_idCurrQuaidInPath];
			else //иначе что-то не так
			{
				gridCheckBeyond();
			}
		}
	}

	//если состояние пути немного вышел за пределы и установлено нужное время для прихода на сетку
	if (m_statePath == NPC_STATE_PATH_BITBEYOND && m_ulTimeReturnInGrid > 0)
	{
		int iTimeDelta = Core_RIntGet(G_RI_INT_TIME_DELTA);
		m_ulTimeAllReturnInGrid += iTimeDelta;

		//если общее время движения к сетке превысело предел
		if (m_ulTimeAllReturnInGrid >= m_ulTimeReturnInGrid)
		{
			//значит скорее всего нпс уже не вернется, устанавливаем состояние вышел за пределы
			m_statePath = NPC_STATE_PATH_BEYOND;
			m_ulTimeReturnInGrid = m_ulTimeAllReturnInGrid = 0;
			m_pCharacter->setWalkDirection(F3_BTVEC(float3(0, 0, 0)));
			//если был предыдущий путь
			if (m_aPathQuads.size() > 0)
				SAIG_GridSetColorArr(&(m_aPathQuads[0]), 0, m_aPathQuads.size());
		}
	}

	//если возвращение на сетку не допустимо
	if (m_statePath == NPC_STATE_PATH_BEYOND)
	{
		if (m_fHealth > 0.f)
		{
			//минусуем здоровье нпс, делаем -1 чтобы предки поняли что только что умер нпс
			m_fHealth = -1.f;
			return;
		}
	}
	
	updateOrientLerp();
}

void CNPCBase::gridCheckBeyond()
{
	//находим ближайший квад к текущей позиции нпс
	ID idq = SAIG_QuadGetNear(&(float3)m_vPosition);
	if (idq >= 0)
	{
		float3 tpos;
		SAIG_QuadGetPos(idq, &tpos);
		float dist = SMVector3Distance(m_vPosition, tpos);
		//если дистанция между текущей позицией нпс и позицией ближайшего квада лежит в допустимых пределах возвращения на сетку
		if (dist <= NPC_QUAD_DIST_BEYOND)
		{
			//просто сообщаем что вышли за пределы
			m_statePath = NPC_STATE_PATH_BITBEYOND;

			//если был предыдущий путь
			if (m_aPathQuads.size() > 0)
				SAIG_GridSetColorArr(&(m_aPathQuads[0]), 0, m_aPathQuads.size());

			//и устанавливаем путь возвращения
			m_aPathQuads.resize(1);
			m_aPathQuads[0] = idq;
			m_idCurrQuaidInPath = 0;
			SAIG_GridSetColorArr(&(m_aPathQuads[0]), m_ulColor, m_aPathQuads.size());

			//просчитываем время хождения по пути до сетки
			m_ulTimeReturnInGrid = (dist * m_fSpeedWalk) * 1000.f * 20.f;
			m_ulTimeAllReturnInGrid = 0;
		}
		else
			m_statePath = NPC_STATE_PATH_BEYOND;
	}
	else
		m_statePath = NPC_STATE_PATH_BEYOND;
}

void CNPCBase::pathWalk()
{
	//если текущий ключ квада пути не известен
	if (m_idCurrQuaidInPath >= m_aPathQuads.size() || m_idCurrQuaidInPath < 0)
	{
		m_idCurrQuaidInPath = -1;
		m_stateMove = NPC_STATE_MOVE_IDLE_START;
		return;
	}

	m_stateMove = NPC_STATE_MOVE_RUN;

	//если текущий квад больше нуля и текущий квад нпс не явлется предыдущим квадом пути либо следующим квадом пути, значит скорее всего нпс сбился с пути
	if (m_idCurrQuaidInPath > 0 && (m_idCurrAiQuad != m_aPathQuads[m_idCurrQuaidInPath] && m_idCurrAiQuad != m_aPathQuads[m_idCurrQuaidInPath - 1]))
	{
		//если текущий квад нпс не явлется соседом предыдущему и следующему квадам пути, значит нпс вышел за допустимые пределы, а значит сбился с пути
		if (!SAIG_QuadIs2Neighbors(m_idCurrAiQuad, m_aPathQuads[m_idCurrQuaidInPath], m_aPathQuads[m_idCurrQuaidInPath - 1]))
		{
			m_statePath = NPC_STATE_PATH_LOST;
			m_pCharacter->setWalkDirection(F3_BTVEC(float3(0,0,0)));
			return;
		}
		else //иначе нпс чуток отклонился от маршрута, поправляем
			orientAtPoint(&m_vPosQuadInPathNext, NPC_TIME_ORIENT_IN_PATH);
	}

	//если следующий квад аи сетки несвободен и занят не текущим нпс
	if (SAIG_QuadGetState(m_aPathQuads[m_idCurrQuaidInPath]) != AIQUAD_STATE_FREE && SAIG_QuadGetStateWho(m_aPathQuads[m_idCurrQuaidInPath]) != getId())
	{
		//значит он преградил путь и текущий нпс сбился с пути
		m_statePath = NPC_STATE_PATH_LOST;
		m_pCharacter->setWalkDirection(F3_BTVEC(float3(0, 0, 0)));
		return;
	}

	//занимаем следующий квад аи сетки за текущим нпс
	SAIG_QuadSetState(m_aPathQuads[m_idCurrQuaidInPath], AIQUAD_STATE_TEMPBUSY);
	SAIG_QuadSetStateWho(m_aPathQuads[m_idCurrQuaidInPath], getId());

	SAIG_QuadGetPos(m_aPathQuads[m_idCurrQuaidInPath], &m_vPosQuadInPathNext);

	//если текущая позиция нпс ниже чем позиция квада к которому он стремится, и их разница больше либо равна допустимой
	if (m_vPosQuadInPathNext.y > m_vPosition.y && abs(m_vPosQuadInPathNext.y - m_vPosition.y) >= m_fStepHeight - NPC_STEP_HEIGHT_EPSILON)
	{
		//говорим что надо прыгнуть
		m_canJump = true;
	}

	//если разрешен прыжок
	if (m_canJump)
	{
		if (m_pCharacter->canJump())
		{
				m_pCharacter->jump();
				m_canJump = false;
		}
	}

	if (m_idCurrQuaidInPath == 0)
		orientAtPoint(&m_vPosQuadInPathNext, NPC_TIME_ORIENT_IN_PATH);

	float r_curr = SMVector3Distance(m_vPosition, m_vPosQuadInPathNext);
	float r_last = SMVector3Distance(m_vLastPathPos, m_vPosQuadInPathNext);

	if (SMVector3Distance(m_vPosition, m_vPosQuadInPathNext) <= 0.05f || r_curr > r_last)
	{
		if (m_idCurrQuaidInPath + 1 < m_aPathQuads.size())
			SAIG_QuadSetState(m_aPathQuads[m_idCurrQuaidInPath], AIQUAD_STATE_FREE);

		++m_idCurrQuaidInPath;

		//если было состояние пути немного вышел за пределы
		if (m_statePath == NPC_STATE_PATH_BITBEYOND)
		{
			//устанавилваем состояние не найденный путь
			m_statePath = NPC_STATE_PATH_NOTFOUND;
			m_ulTimeReturnInGrid = m_ulTimeAllReturnInGrid = 0;
		}

		if (m_idCurrQuaidInPath < m_aPathQuads.size() && m_idCurrQuaidInPath > 0)
		{
			m_vPosQuadInPathLast = m_vPosQuadInPathNext;
			SAIG_QuadGetPos(m_aPathQuads[m_idCurrQuaidInPath], &m_vPosQuadInPathNext);
			orientAtPoint(&m_vPosQuadInPathNext, NPC_TIME_ORIENT_IN_PATH);
		}
		else
		{
			m_pCharacter->setWalkDirection(F3_BTVEC(float3(0,0,0)));
		}
	}

	m_vLastPathPos = m_vPosition;
}

void CNPCBase::orientAtPoint(const float3 * pos, DWORD ttime)
{
	float3 curr_pos = getPos();
	curr_pos.y = 0;
	float3 poscam = float3(pos->x, 0, pos->z);
	float3 dircam = poscam - curr_pos;
	dircam = SMVector3Normalize(dircam);
	
	float angle = (acosf(SMVector3Dot(NPC_BASE_DIR, dircam)) * sign2(SMVector3Cross(NPC_BASE_DIR, dircam).y));
	angle = (-angle);
	
	//просчитываем направление движения и скорость
	float3 dir = SMVector3Normalize(SMQuaternion(angle, 'y') * NPC_BASE_DIR);
	float speed = 0;
	if (m_stateMove == NPC_STATE_MOVE_WALK)
		speed = m_fSpeedWalk;
	else if (m_stateMove == NPC_STATE_MOVE_RUN)
		speed = m_fSpeedRun;
	m_pCharacter->setWalkDirection(F3_BTVEC(dir * speed));

	if (ttime > 0)
	{
		m_fAngleYNext = angle;
		m_ulTimeAllRot = 0;
		m_ulTimeRot = ttime;
	}
	else
	{
		m_fAngleYLast = angle;
		m_fAngleYNext = angle;
		m_ulTimeAllRot = 0;
		m_ulTimeRot = 0;
		setOrient(SMQuaternion(angle, 'y'));
	}
}

void CNPCBase::updateOrientLerp()
{
	if (m_ulTimeRot <= 0)
		return;

	int iTimeDelta = Core_RIntGet(G_RI_INT_TIME_DELTA);
	m_ulTimeAllRot += iTimeDelta;
	float fCurrAngle = lerpf(m_fAngleYLast, m_fAngleYNext, saturatef(float(m_ulTimeAllRot) / float(m_ulTimeRot)));
	setOrient(SMQuaternion(fCurrAngle, 'y'));

	if (m_ulTimeAllRot >= m_ulTimeRot)
	{
		m_fAngleYLast = m_fAngleYNext;
		m_ulTimeAllRot = 0;
		m_ulTimeRot = 0;
	}
}