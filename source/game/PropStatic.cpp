
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

REGISTER_ENTITY(CPropStatic, prop_static, REC_MODEL_FIELD("model"));

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

		XRIDIGBODY_DESC rigidBodyDesc;
		rigidBodyDesc.pCollisionShape = m_pCollideShape;
		rigidBodyDesc.vStartWorldPosition = vPos;
		rigidBodyDesc.qStartWorldRotation = qRot;

		GetPhysics()->newRigidBody(rigidBodyDesc, &m_pRigidBody);
		
		//m_pRigidBody->setFriction(100.0f);
		m_pRigidBody->setUserPointer(this);
		m_pRigidBody->setUserTypeId(1);
		GetPhysWorld()->addCollisionObject(m_pRigidBody, CG_STATIC, CG_STATIC_MASK);

		m_pRigidBody->setLinearFactor(float3(0.0f, 0.0f, 0.0f));
		m_pRigidBody->setAngularFactor(float3(0.0f, 0.0f, 0.0f));
	}
}

void CPropStatic::removePhysBody()
{
	GetPhysWorld()->removeCollisionObject(m_pRigidBody);
	mem_release(m_pRigidBody);
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

	IXCompoundShape *pShape;
	GetPhysics()->newCompoundShape(&pShape, uShapesCount);

	auto pResource = m_pModel->getResource()->asStatic();
	UINT uUsedLod = pResource->getLodCount() - 1;
	for(UINT i = 0, l = pResource->getSubsetCount(uUsedLod); i < l; ++i)
	{
		IXTrimeshShape *pLocalShape = NULL;
		auto pSubset = pResource->getSubset(uUsedLod, i);

		GetPhysics()->newTrimeshShape(pSubset->iVertexCount, (float3_t*)pSubset->pVertices, pSubset->iIndexCount, pSubset->pIndices, &pLocalShape, sizeof(pSubset->pVertices[0]));

		if(pLocalShape)
		{
			pLocalShape->setLocalScaling(m_fBaseScale);
			pShape->addChildShape(pLocalShape, float3(), SMQuaternion());
		}
	}
	pShape->recalculateLocalAabb();

	m_pCollideShape = pShape;
	createPhysBody();
}

void CPropStatic::releasePhysics()
{
	removePhysBody();

	mem_release(m_pCollideShape);
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
