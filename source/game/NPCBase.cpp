
#include "NPCBase.h"

BEGIN_PROPTABLE(NPCBase)
// empty
	
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(NPCBase, npc_base);

NPCBase::NPCBase(EntityManager * pMgr) :
	BaseClass(pMgr)
{
	m_health = 1.f;
	m_speed_walk = 0.07f;
	m_speed_run = 0.12f;
	m_immunity_weapon = 1.f;
	m_curr_quaid_in_path = -1;
	m_fBaseScale = 0.01f;
	m_curr_aiquad = -1;
	m_move_state = NPC_MOVE_STATE_IDLE;

	m_angle_y_last = 0;
	m_angle_y_next = 0;
	m_alltime_rot = 0;
	m_time_rot = 0;
}

NPCBase::~NPCBase()
{

}

void NPCBase::InitPhysics()
{
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(F3_BTVEC(m_vPosition));
	//startTransform.setOrigin(btVector3(0, 12, 10));

	m_pGhostObject = new btPairCachingGhostObject();
	m_pGhostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	m_pCollideShape = new btCapsuleShape(0.3f, 1.4f);
	m_pGhostObject->setCollisionShape(m_pCollideShape);
	m_pGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	btScalar stepHeight = 0.4f;
	m_pCharacter = new btKinematicCharacterController(m_pGhostObject, (btConvexShape*)m_pCollideShape, stepHeight, btVector3(0.0f, 1.0f, 0.0f));
	m_pCharacter->setMaxJumpHeight(0.60f);
	m_pCharacter->setJumpSpeed(3.50f);
	m_pCharacter->setGravity(btVector3(0, -10.0f, 0));
	m_pCharacter->setFallSpeed(300.0f);

	SXPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::DebrisFilter);

	//m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() /*| btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT*/);

	SXPhysics_GetDynWorld()->addAction(m_pCharacter);
}

void NPCBase::SetPos(const float3 & pos)
{
	float3 tpos = pos;
	m_curr_aiquad = SAIG_QuadGet(&tpos, true);

	if (m_curr_aiquad >= 0)
	{
		if (SAIG_QuadGetState(m_curr_aiquad) == AIQUAD_STATE_FREE)
		{
			SAIG_QuadSetState(m_curr_aiquad, AIQUAD_STATE_BUSY);
			SAIG_QuadSetStateWho(m_curr_aiquad, this->GetId());
		}
		else
		{
			if (SAIG_QuadGetStateWho(m_curr_aiquad) != this->GetId())
			{
				ID idquad = SAIG_QuadGetNear(&(float3)m_vPosition, true, 2);
				if (idquad >= 0)
				{
					m_curr_aiquad = idquad;
					SAIG_QuadSetState(m_curr_aiquad, AIQUAD_STATE_BUSY);
					SAIG_QuadSetStateWho(m_curr_aiquad, this->GetId());
					SAIG_QuadGetPos(m_curr_aiquad, &tpos);
					tpos.y += 0.7f;
					SetPos(tpos);
				}
			}
		}
	}

	BaseClass::SetPos(tpos);
	m_pGhostObject->getWorldTransform().setOrigin(F3_BTVEC(tpos));
}

bool NPCBase::SetKV(const char * name, const char * value)
{
	if (stricmp("origin", name) == 0)
	{
		propdata_t * field = GetField(name);
		if (!field)
		{
			return(false);
		}
		float3_t f3;

		if (3 == sscanf(value, "%f %f %f", &f3.x, &f3.y, &f3.z))
		{
			SetPos(f3);
			return true;
		}
		else
			return false;
	}

	return BaseClass::SetKV(name, value);
}

ID NPCBase::GetAIQuad()
{
	return m_curr_aiquad;
}

bool NPCBase::PathFind(ID endq)
{
	if (m_curr_aiquad >= 0 && SAIG_GridFindPath(m_curr_aiquad, endq))
	{
		m_arr_path.resize(SAIG_GridGetSizePath());
		SAIG_GridGetPath(&(m_arr_path[0]), m_arr_path.size(), true);
		SAIG_GridSetColorArr(&(m_arr_path[0]), D3DCOLOR_ARGB(128, 0, 255, 0), m_arr_path.size());
		m_last_path_pos = m_vPosition;
		return true;
	}

	return false;
}

void NPCBase::OnSync()
{
	BaseClass::OnSync();

	btTransform trans;
	trans = m_pGhostObject->getWorldTransform();

	m_vPosition = (float3)(float3(trans.getOrigin().x(), trans.getOrigin().y()-0.9, trans.getOrigin().z()));

	//определяем текущий id квада аи сетки
	m_curr_aiquad = SAIG_QuadGet(&(float3)m_vPosition, false);

	//если квад найти не удалось
	if (m_curr_aiquad < 0)
	{
		//если мы не движемся по пути
		if (m_curr_quaid_in_path < 0 || m_curr_quaid_in_path >= m_arr_path.size())
		{
			if (m_health > 0.f)
			{
				//минусуем здоровье нпс, делаем -1 чтобы предки поняли что только что умер нпс
				m_health = -1.f;
			}
			return;
		}
		//иначе мы движемся по пути, присваиваем id текущего квада к котором стремится нпс
		else
			m_curr_aiquad = m_arr_path[m_curr_quaid_in_path];
	}
	
	UpdateOrientLerp();
}

void NPCBase::PathWalk()
{
	//если текущий ключ квада пути не известен
	if (m_curr_quaid_in_path >= m_arr_path.size() || m_curr_quaid_in_path < 0)
	{
		m_curr_quaid_in_path = -1;
		m_move_state = NPC_MOVE_STATE_IDLE_START;
		return;
	}

	SAIG_QuadGetPos(m_arr_path[m_curr_quaid_in_path], &m_next_path_pos);

	if (m_curr_quaid_in_path == 0)
		OrientAtPoint(&m_next_path_pos, NPC_TIME_ORIENT_IN_PATH);

	float r_curr = SMVector3Distance(m_vPosition, m_next_path_pos);
	float r_last = SMVector3Distance(m_last_path_pos, m_next_path_pos);

	if (SMVector3Distance(m_vPosition, m_next_path_pos) <= 0.05f || r_curr > r_last)
	{
		++m_curr_quaid_in_path;

		if (m_curr_quaid_in_path < m_arr_path.size() && m_curr_quaid_in_path > 0)
		{
			SAIG_QuadGetPos(m_arr_path[m_curr_quaid_in_path], &m_next_path_pos);
			OrientAtPoint(&m_next_path_pos, NPC_TIME_ORIENT_IN_PATH);
		}
		else
		{
			m_pCharacter->setWalkDirection(F3_BTVEC(float3(0,0,0)));
		}
	}

	m_last_path_pos = m_vPosition;
}

void NPCBase::OrientAtPoint(const float3 * pos, DWORD ttime)
{
	float3 curr_pos = GetPos();
	curr_pos.y = 0;
	float3 poscam = float3(pos->x, 0, pos->z);
	float3 dircam = poscam - curr_pos;
	dircam = SMVector3Normalize(dircam);
	
	float angle = (acosf(SMVector3Dot(NPC_BASE_DIR, dircam)) * sign2(SMVector3Cross(NPC_BASE_DIR, dircam).y));
	angle = (-angle);
	float3 dir = SMVector3Normalize(SMQuaternion(angle, 'y') * NPC_BASE_DIR);
	m_pCharacter->setWalkDirection(F3_BTVEC(dir * m_speed_walk));
	if (ttime > 0)
	{
		m_angle_y_next = angle;
		m_alltime_rot = 0;
		m_time_rot = ttime;
	}
	else
	{
		m_angle_y_last = angle;
		m_angle_y_next = angle;
		m_alltime_rot = 0;
		m_time_rot = 0;
		SetOrient(SMQuaternion(angle, 'y'));
	}
}

void NPCBase::UpdateOrientLerp()
{
	if (m_time_rot <= 0)
		return;

	int timeDelta = Core_RIntGet(G_RI_INT_TIME_DELTA);
	m_alltime_rot += timeDelta;
	float curr_angle = lerpf(m_angle_y_last, m_angle_y_next, saturatef(float(m_alltime_rot) / float(m_time_rot)));
	SetOrient(SMQuaternion(curr_angle, 'y'));

	if (m_alltime_rot >= m_time_rot)
	{
		m_angle_y_last = m_angle_y_next;
		m_alltime_rot = 0;
		m_time_rot = 0;
	}
}