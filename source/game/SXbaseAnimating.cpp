#include "SXbaseAnimating.h"
#include "gcore/sxgcore.h"

/*! \skydocent base_animating
Базовый класс для объектов, имеющих объем в игровом мире
*/

BEGIN_PROPTABLE(SXbaseAnimating)
	//! Файл модели. Поддерживаются статические и анимированные модели
	DEFINE_FIELD_STRING(m_szModelFile, 0, "model", "Model file", EDITOR_FILEFIELD)
		FILE_OPTION("Model file (*.dse)", "*.dse")
	EDITOR_FILE_END()

	//! Масштаб модели
	DEFINE_FIELD_FLOAT(m_fBaseScale, 0, "scale", "Scale", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseAnimating, base_animating);

SXbaseAnimating::SXbaseAnimating(EntityManager * pMgr):
	BaseClass(pMgr),
	m_pAnimPlayer(NULL),
	m_fBaseScale(1.0f),
	m_pCollideShape(NULL),
	m_pRigidBody(NULL)
{
}

SXbaseAnimating::~SXbaseAnimating()
{
	ReleasePhysics();
	mem_release(m_pAnimPlayer);
}

void SXbaseAnimating::GetMinMax(float3 * min, float3 * max)
{
	if (m_pAnimPlayer)
	{
		const ISXBound * bound = m_pAnimPlayer->GetBound();
		bound->GetMinMax(min, max);
	}
}

void SXbaseAnimating::GetSphere(float3 * center, float * radius)
{
	if (m_pAnimPlayer)
	{
		const ISXBound * bound = m_pAnimPlayer->GetBound();
		bound->GetSphere(center, radius);
	}
}

bool SXbaseAnimating::SetKV(const char * name, const char * value)
{
	if(!BaseClass::SetKV(name, value))
	{
		return(false);
	}
	if(!strcmp(name, "model"))
	{
		SetModel(value);
	}
	else if(!strcmp(name, "scale"))
	{
		ReleasePhysics();
		m_pAnimPlayer->SetScale(m_fBaseScale);
		InitPhysics();
	}
	return(true);
}

void SXbaseAnimating::SetModel(const char * mdl)
{
	_SetStrVal(&m_szModelFile, mdl);
	ReleasePhysics();
	if(!mdl[0] && m_pAnimPlayer)
	{
		mem_release(m_pAnimPlayer);
		return;
	}
	if(!m_pAnimPlayer)
	{
		m_pAnimPlayer = SXAnim_CreatePlayer(mdl);
	}
	else
	{
		m_pAnimPlayer->SetModel(mdl);
	}
	InitPhysics();
}

float3 SXbaseAnimating::GetAttachmentPos(int id)
{
	float3 pos;
	if(!m_pAnimPlayer && id >= 0)
	{
		pos = m_pAnimPlayer->GetBoneTransformPos(id);
	}

	return(GetOrient() * pos + GetPos());
}

SMQuaternion SXbaseAnimating::GetAttachmentRot(int id)
{
	SMQuaternion rot;
	if(!m_pAnimPlayer && id >= 0)
	{
		rot = m_pAnimPlayer->GetBoneTransformRot(id);
	}

	return(GetOrient() * rot);
}

void SXbaseAnimating::OnSync()
{
	BaseClass::OnSync();
	if(!m_pParent && m_pRigidBody)
	{
		SetPos(BTVEC_F3(m_pRigidBody->getWorldTransform().getOrigin()));
		SetOrient(BTQUAT_Q4(m_pRigidBody->getWorldTransform().getRotation()));
	}
	else if(m_pRigidBody)
	{
	//	m_pRigidBody->getWorldTransform().setOrigin(F3_BTVEC(GetPos()));
	//	m_pRigidBody->getWorldTransform().setRotation(Q4_BTQUAT(GetOrient()));
	}
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->SetScale(m_fBaseScale);
		m_pAnimPlayer->SetPos(GetPos());
		m_pAnimPlayer->SetOrient(GetOrient());
	}
}

void SXbaseAnimating::PlayAnimation(const char * name, UINT iFadeTime, UINT slot)
{
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->Play(name, iFadeTime, slot);
	}
}

bool SXbaseAnimating::PlayingAnimations(const char* name)
{
	if(m_pAnimPlayer)
	{
		return(m_pAnimPlayer->PlayingAnimations(name));
	}
	return(false);
}

void SXbaseAnimating::PlayActivity(const char * name, UINT iFadeTime, UINT slot)
{
	if(m_pAnimPlayer)
	{
		m_pAnimPlayer->StartActivity(name, iFadeTime, slot);
	}
}

void SXbaseAnimating::InitPhysics()
{
	if(!m_pAnimPlayer)
	{
		return;
	}
	int32_t iShapeCount;
	HITBOX_TYPE * phTypes;
	float3_t ** ppfData;
	int32_t * pfDataLen;

	//m_pAnimPlayer->SetScale(m_fBaseScale);

	m_pAnimPlayer->GetPhysData(&iShapeCount, &phTypes, &ppfData, &pfDataLen);

	for(int i = 0; i < iShapeCount; ++i)
	{
		if(phTypes[i] == HT_CONVEX)
		{
			m_pCollideShape = new btConvexHullShape((float*)ppfData[i], pfDataLen[i], sizeof(ppfData[0][0]));
		}
		break;
	}

	m_pAnimPlayer->FreePhysData(iShapeCount, phTypes, ppfData, pfDataLen);


	CreatePhysBody();
}

void SXbaseAnimating::CreatePhysBody()
{
	if(m_pCollideShape)
	{

		btVector3 vInertia;
		const float fMass = 1.0f;
		m_pCollideShape->calculateLocalInertia(fMass, vInertia);

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(Q4_BTQUAT(m_vOrientation), F3_BTVEC(m_vPosition)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			fMass,                  // mass
			motionState,        // initial position
			m_pCollideShape,    // collision shape of body
			vInertia  // local inertia
			);
		m_pRigidBody = new btRigidBody(rigidBodyCI);

		//m_pRigidBody->setFriction(100.0f);
		m_pRigidBody->setUserPointer(this);
		SXPhysics_AddShape(m_pRigidBody);
	}
}

void SXbaseAnimating::RemovePhysBody()
{
	SXPhysics_RemoveShape(m_pRigidBody);
	mem_delete(m_pRigidBody);
}

void SXbaseAnimating::ReleasePhysics()
{
	RemovePhysBody();
	mem_delete(m_pCollideShape);
}

void SXbaseAnimating::SetPos(const float3 & pos)
{
	BaseClass::SetPos(pos);
	if(m_pRigidBody)
	{
		m_pRigidBody->getWorldTransform().setOrigin(F3_BTVEC(pos));
	}
}

void SXbaseAnimating::SetOrient(const SMQuaternion & q)
{
	BaseClass::SetOrient(q);
	if(m_pRigidBody)
	{
		m_pRigidBody->getWorldTransform().setRotation(Q4_BTQUAT(q));
	}
}
