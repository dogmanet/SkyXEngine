
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PropStatic.h"

/*! \skydocent prop_static
Модель
*/

BEGIN_PROPTABLE(CPropStatic)
	DEFINE_FIELD_INTFN(m_iTrimeshPhysics, 0, "use_trimesh", "Use trimesh physics", onSetUseTrimesh, EDITOR_COMBOBOX)
		COMBO_OPTION("No", "0")
		COMBO_OPTION("Yes", "1")
		COMBO_OPTION("Force", "2")
	EDITOR_COMBO_END()
END_PROPTABLE()

REGISTER_ENTITY(CPropStatic, prop_static);

CPropStatic::CPropStatic()
{
	m_isStatic = true;
}

CPropStatic::~CPropStatic()
{
	removePhysBody();
}

void CPropStatic::createPhysBody()
{
	if(m_pCollideShape)
	{
		float3 vPos = getPos();
		SMQuaternion qRot = getOrient();

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(Q4_BTQUAT(qRot), F3_BTVEC(vPos)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass
			motionState,        // initial position
			m_pCollideShape,    // collision shape of body
			btVector3(0, 0, 0)  // local inertia
			);
		m_pRigidBody = new btRigidBody(rigidBodyCI);
		//m_pRigidBody->getInvMass();

		//m_pRigidBody->setFriction(100.0f);
		m_pRigidBody->setUserPointer(this);
		int colGroup = CG_STATIC;
		int colMask = CG_STATIC_MASK;
		SPhysics_AddShapeEx(m_pRigidBody, colGroup, colMask);

		m_pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
		m_pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	}
}

void CPropStatic::removePhysBody()
{
	SPhysics_RemoveShape(m_pRigidBody);
	mem_delete(m_pRigidBody);
}

void CPropStatic::initPhysics()
{
	if(!m_pModel)
	{
		return;
	}

	UINT uShapesCount = m_pModel->getPhysboxCount();

	float3 vSize = m_pModel->getLocalBoundMax() - m_pModel->getLocalBoundMin();
	bool useTrimesh = (m_iTrimeshPhysics == 2) || ((m_iTrimeshPhysics == 1) && vSize.x > 1.0f && vSize.z > 1.0f && vSize.y > 1.8f);

	if(uShapesCount || !useTrimesh)
	{
		BaseClass::initPhysics();
		return;
	}

	if(!m_useAutoPhysbox)
	{
		return;
	}

	btCompoundShape *pShape = new btCompoundShape(true, uShapesCount);

	auto pResource = m_pModel->getResource()->asStatic();
	UINT uUsedLod = pResource->getLodCount() - 1;
	for(UINT i = 0, l = pResource->getSubsetCount(uUsedLod); i < l; ++i)
	{
		btCollisionShape *pLocalShape = NULL;
		auto pSubset = pResource->getSubset(uUsedLod, i);

		pLocalShape = SPhysics_CreateTrimeshShape(pSubset);
		
		if(pLocalShape)
		{
			pLocalShape->setLocalScaling(btVector3(m_fBaseScale, m_fBaseScale, m_fBaseScale));
			btTransform localTransform;
			localTransform.setIdentity();
			pShape->addChildShape(localTransform, pLocalShape);
		}
	}

	m_pCollideShape = pShape;
	createPhysBody();
}

void CPropStatic::releasePhysics()
{
	removePhysBody();
	if(m_pCollideShape)
	{
		btCompoundShape *pShape = (btCompoundShape*)m_pCollideShape;
		for(UINT i = 0, l = pShape->getNumChildShapes(); i < l; ++i)
		{
			auto pChild = pShape->getChildShape(i);
			if(pChild->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
			{

			}
			else
			{
				mem_delete(pChild);
			}
		}
		mem_delete(m_pCollideShape);
	}
}

void CPropStatic::onSetUseTrimesh(int iVal)
{
	if(m_iTrimeshPhysics != iVal)
	{
		m_iTrimeshPhysics = iVal;
		releasePhysics();
		initPhysics();
	}
}
