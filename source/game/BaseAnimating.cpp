
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseAnimating.h"
#include "gcore/sxgcore.h"

#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/resource/IXResourceModel.h>

/*! \skydocent base_animating
Базовый класс для объектов, имеющих объем в игровом мире
*/

BEGIN_PROPTABLE(CBaseAnimating)
	//! Файл модели. Поддерживаются статические и анимированные модели
	DEFINE_FIELD_STRINGFN(m_szModelFile, 0, "model", "Model file", setModel, EDITOR_MODEL)

	//! Масштаб модели
	DEFINE_FIELD_FLOATFN(m_fBaseScale, 0, "scale", "Scale", setScale, EDITOR_TEXTFIELD)

	//! Объект референса для цвета свечения
	DEFINE_FIELD_ENTITY(m_pEntColorRef, 0, "glow_color_ref", "Glow color reference", EDITOR_TEXTFIELD)
	//! Цвет свечения
	DEFINE_FIELD_VECTOR(m_vGlowColor, 0, "glow_color", "Glow color", EDITOR_TEXTFIELD)

	DEFINE_FIELD_BOOLFN(m_isStatic, 0, "is_static", "Is static", onIsStaticChange, EDITOR_YESNO)

	DEFINE_FIELD_INTFN(m_iSkin, 0, "skin", "Skin", setSkin, EDITOR_TEXTFIELD)

	DEFINE_FIELD_BOOLFN(m_useAutoPhysbox, 0, "auto_physbox", "Auto generate physbox", onSetUseAutoPhysbox, EDITOR_YESNO)

	DEFINE_INPUT(inputPlayAnim, "playAnim", "Play animation", PDF_STRING)
	DEFINE_INPUT(inputPlayAnimNext, "playAnimNext", "Play animation next", PDF_STRING)

	DEFINE_INPUT(inputPlayActivity, "playActivity", "Play activity", PDF_STRING)
	DEFINE_INPUT(inputPlayActivityNext, "playActivityNext", "Play activity next", PDF_STRING)

	DEFINE_INPUT(inputSetSkin, "setSkin", "Set skin", PDF_STRING)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseAnimating, base_animating);

//##########################################################################

class CAnimationCallback: public IAnimationCallback
{
public:
	CAnimationCallback(CBaseAnimating *pBaseAnimating):
		m_pBaseAnimating(pBaseAnimating)
	{
	}

	void onPlay(UINT uLayer) override
	{
		m_pBaseAnimating->onAnimationStart(uLayer);
	}
	void onStop(UINT uLayer) override
	{
		m_pBaseAnimating->onAnimationStop(uLayer);
	}
	void onLoop(UINT uLayer) override
	{
		m_pBaseAnimating->onAnimationLoop(uLayer);
	}

	void onProgress(UINT uLayer, float fProgress) override
	{
		m_pBaseAnimating->onAnimationProgress(uLayer, fProgress);
	}

private:
	CBaseAnimating *m_pBaseAnimating;
};

//##########################################################################

CBaseAnimating::CBaseAnimating(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	memset(m_vNextAnim, 0, sizeof(m_vNextAnim));
	m_pAnimationCallback = new CAnimationCallback(this);
}

CBaseAnimating::~CBaseAnimating()
{
	releasePhysics();
	mem_release(m_pModel);
	mem_delete(m_pAnimationCallback);
}

void CBaseAnimating::getMinMax(float3 * min, float3 * max)
{
	if(m_pModel)
	{
		*min = m_pModel->getLocalBoundMin();
		*max = m_pModel->getLocalBoundMax();
	}
}

/*void CBaseAnimating::getSphere(float3 * center, float * radius)
{
	if(m_pAnimPlayer)
	{
		const ISXBound * bound = m_pAnimPlayer->getBound();
		bound->getSphere(center, radius);
	}
}*/

void CBaseAnimating::onSetUseAutoPhysbox(bool use)
{
	if(m_useAutoPhysbox != use)
	{
		m_useAutoPhysbox = use;
		releasePhysics();
		initPhysics();
	}
}

void CBaseAnimating::setModel(const char * mdl)
{
	_setStrVal(&m_szModelFile, mdl);
	releasePhysics();
	mem_release(m_pModel);
	if(!mdl[0] /*&& m_pAnimPlayer*/)
	{
		return;
	}

	IXResourceManager *pResourceManager = Core_GetIXCore()->getResourceManager();
	IXModelProvider *pProvider = (IXModelProvider*)Core_GetIXCore()->getPluginManager()->getInterface(IXMODELPROVIDER_GUID);
	
	IXResourceModel *pResource;
	if(pResourceManager->getModel(mdl, &pResource))
	{
		IXDynamicModel *pModel;
		if(pProvider->createDynamicModel(pResource, &pModel))
		{
			m_pModel = pModel;
			m_pModel->setSkin(m_iSkin);
			m_pModel->setScale(m_fBaseScale);
			m_pModel->setPosition(getPos());
			m_pModel->setOrientation(getOrient());
			
			auto pAnimating = m_pModel->asAnimatedModel();
			if(pAnimating)
			{
				pAnimating->setCallback(m_pAnimationCallback);
				pAnimating->play("idle");
			}
		}
		mem_release(pResource);
	}

	initPhysics();
}

void CBaseAnimating::setScale(float fScale)
{
	m_fBaseScale = fScale;

	releasePhysics();
	if(m_pModel)
	{
		m_pModel->setScale(m_fBaseScale);
	}
	initPhysics();
}

float3 CBaseAnimating::getAttachmentPos(int id)
{
	float3 pos;
	if(m_pModel && m_pModel->asAnimatedModel() && id >= 0)
	{
		pos = m_pModel->asAnimatedModel()->getBoneTransformPos(id);
	}

	return(/*getOrient() * */pos/* + getPos()*/);
}

SMQuaternion CBaseAnimating::getAttachmentRot(int id)
{
	SMQuaternion rot;
	if(m_pModel && m_pModel->asAnimatedModel() && id >= 0)
	{
		rot = m_pModel->asAnimatedModel()->getBoneTransformRot(id);
	}

	return(/*getOrient() * */rot);
}

void CBaseAnimating::onSync()
{
	BaseClass::onSync();
	if(!m_pParent && m_pRigidBody && !m_isStatic)
	{
		btVector3 &v = m_pRigidBody->getWorldTransform().getOrigin();
		setPos(BTVEC_F3(v));
		btQuaternion &q = m_pRigidBody->getWorldTransform().getRotation();
		setOrient(BTQUAT_Q4(q));
	}
	else if(m_pRigidBody)
	{
	//	m_pRigidBody->getWorldTransform().setOrigin(F3_BTVEC(getPos()));
	//	m_pRigidBody->getWorldTransform().setRotation(Q4_BTQUAT(getOrient()));
	}
	if(m_pModel)
	{
		m_pModel->setPosition(getPos());
		m_pModel->setOrientation(getOrient());

		float3_t vGlowColor = m_vGlowColor;
		//bool isGlowEnabled = m_pEntColorRef ? m_pEntColorRef->getMainColor(&vGlowColor) : m_vGlowColor.x != 0.0f || m_vGlowColor.y != 0.0f || m_vGlowColor.z != 0.0f;
		m_pModel->setColor(float4(vGlowColor));
		//m_pAnimPlayer->setGlowEnabled(isGlowEnabled);
	}
}

void CBaseAnimating::playAnimation(const char * name, UINT iFadeTime, UINT slot)
{
	if(m_pModel && m_pModel->asAnimatedModel())
	{
		m_pModel->asAnimatedModel()->play(name, iFadeTime, slot);
	}
}

bool CBaseAnimating::playingAnimations(const char* name)
{
	if(m_pModel && m_pModel->asAnimatedModel())
	{
		return(m_pModel->asAnimatedModel()->isPlayingAnimation(name));
	}
	return(false);
}

void CBaseAnimating::playActivity(const char * name, UINT iFadeTime, UINT slot)
{
	if(m_pModel && m_pModel->asAnimatedModel())
	{
		m_pModel->asAnimatedModel()->startActivity(name, iFadeTime, slot);
	}
}

void CBaseAnimating::initPhysics()
{
	if(!m_pModel)
	{
		return;
	}

	UINT uShapesCount = m_pModel->getPhysboxCount();


	btCompoundShape *pShape = new btCompoundShape(true, uShapesCount);
	for(UINT i = 0; i < uShapesCount; ++i)
	{
		auto pPhysbox = m_pModel->getPhysBox(i);
		btCollisionShape *pLocalShape = NULL;
		switch(pPhysbox->getType())
		{
		case XPBT_BOX:
			pLocalShape = new btBoxShape(F3_BTVEC(pPhysbox->asBox()->getSize()) * m_fBaseScale);
			break;
		case XPBT_SPHERE:
			pLocalShape = new btSphereShape(pPhysbox->asSphere()->getRadius() * m_fBaseScale);
			break;
		case XPBT_CAPSULE:
			pLocalShape = new btCapsuleShape(pPhysbox->asCapsule()->getRadius() * m_fBaseScale, pPhysbox->asCapsule()->getHeight() * m_fBaseScale);
			break;
		case XPBT_CYLINDER:
			pLocalShape = new btCylinderShape(btVector3(pPhysbox->asCylinder()->getRadius(), pPhysbox->asCylinder()->getHeight() * 0.5f, pPhysbox->asCylinder()->getRadius()) * m_fBaseScale);
			break;
		case XPBT_CONVEX:
			{
				auto pConvex = pPhysbox->asConvex();
				btConvexHullShape tmpShape((float*)pConvex->getData(), pConvex->getVertexCount(), sizeof(float3_t));
				tmpShape.setMargin(0);
				btVector3 *pData;
				int iVertexCount;
				SPhysics_BuildHull(&tmpShape, &pData, &iVertexCount);
				for(int i = 0; i < iVertexCount; ++i)
				{
					pData[i] *= m_fBaseScale;
				}
				pLocalShape = new btConvexHullShape((float*)pData, iVertexCount, sizeof(btVector3));
				SPhysics_ReleaseHull(pData, iVertexCount);
			}
			break;
		}

		if(pLocalShape)
		{
			btTransform localTransform(Q4_BTQUAT(pPhysbox->getOrientation()), F3_BTVEC(pPhysbox->getPosition()) * m_fBaseScale);
			pShape->addChildShape(localTransform, pLocalShape);
		}
	}
	if(!uShapesCount && m_useAutoPhysbox)
	{
		{
			auto pResource = m_pModel->getResource()->asStatic();
			if(pResource)
			{
				UINT uUsedLod = pResource->getLodCount() - 1;
				for(UINT i = 0, l = pResource->getSubsetCount(uUsedLod); i < l; ++i)
				{
					btCollisionShape *pLocalShape = NULL;
					auto pSubset = pResource->getSubset(uUsedLod, i);

					btConvexHullShape tmpShape((float*)pSubset->pVertices, pSubset->iVertexCount, sizeof(pSubset->pVertices[0]));
					tmpShape.setMargin(0);
					btVector3 *pData;
					int iVertexCount;
					SPhysics_BuildHull(&tmpShape, &pData, &iVertexCount);
					for(int i = 0; i < iVertexCount; ++i)
					{
						pData[i] *= m_fBaseScale;
					}
					pLocalShape = new btConvexHullShape((float*)pData, iVertexCount, sizeof(btVector3));
					SPhysics_ReleaseHull(pData, iVertexCount);


					if(pLocalShape)
					{
						btTransform localTransform;
						localTransform.setIdentity();
						pShape->addChildShape(localTransform, pLocalShape);
					}
				}
			}
		}
		{
			auto pResource = m_pModel->getResource()->asAnimated();
			if(pResource)
			{
				UINT uUsedLod = pResource->getLodCount() - 1;
				for(UINT i = 0, l = pResource->getSubsetCount(uUsedLod); i < l; ++i)
				{
					btCollisionShape *pLocalShape = NULL;
					auto pSubset = pResource->getSubset(uUsedLod, i);

					btConvexHullShape tmpShape((float*)pSubset->pVertices, pSubset->iVertexCount, sizeof(pSubset->pVertices[0]));
					tmpShape.setMargin(0);
					btVector3 *pData;
					int iVertexCount;
					SPhysics_BuildHull(&tmpShape, &pData, &iVertexCount);
					for(int i = 0; i < iVertexCount; ++i)
					{
						pData[i] *= m_fBaseScale;
					}
					pLocalShape = new btConvexHullShape((float*)pData, iVertexCount, sizeof(btVector3));
					SPhysics_ReleaseHull(pData, iVertexCount);


					if(pLocalShape)
					{
						btTransform localTransform;
						localTransform.setIdentity();
						pShape->addChildShape(localTransform, pLocalShape);
					}
				}
			}
		}
	}
	m_pCollideShape = pShape;
	createPhysBody();
}

void CBaseAnimating::createPhysBody()
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
		//m_pRigidBody->getInvMass();

		//m_pRigidBody->setFriction(100.0f);
		m_pRigidBody->setUserPointer(this);
		int colGroup = m_collisionGroup;
		int colMask = m_collisionMask;
		if(m_isStatic)
		{
			colGroup = CG_STATIC;
			colMask = CG_STATIC_MASK;
		}
		SPhysics_AddShapeEx(m_pRigidBody, colGroup, colMask);

		if(m_isStatic)
		{
			m_pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
			m_pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
		}
	}
}

void CBaseAnimating::removePhysBody()
{
	SPhysics_RemoveShape(m_pRigidBody);
	mem_delete(m_pRigidBody);
}

void CBaseAnimating::releasePhysics()
{
	removePhysBody();
	if(m_pCollideShape)
	{
		btCompoundShape *pShape = (btCompoundShape*)m_pCollideShape;
		for(UINT i = 0, l = pShape->getNumChildShapes(); i < l; ++i)
		{
			delete pShape->getChildShape(i);
		}
		mem_delete(m_pCollideShape);
	}
}

void CBaseAnimating::setCollisionGroup(COLLISION_GROUP group, COLLISION_GROUP mask)
{
	if(m_collisionGroup == group && m_collisionMask == mask)
	{
		return;
	}
	m_collisionGroup = group;
	m_collisionMask = mask;
	if(m_pRigidBody)
	{
		SPhysics_RemoveShape(m_pRigidBody);
		SPhysics_AddShapeEx(m_pRigidBody, m_collisionGroup, m_collisionMask);
	}
}
COLLISION_GROUP CBaseAnimating::getCollisionGroup()
{
	return(m_collisionGroup);
}

void CBaseAnimating::setPos(const float3 & pos)
{
	BaseClass::setPos(pos);
	if(m_pRigidBody)
	{
		m_pRigidBody->getWorldTransform().setOrigin(F3_BTVEC(pos));

		SPhysics_GetDynWorld()->updateSingleAabb(m_pRigidBody);
	}
}

void CBaseAnimating::setOrient(const SMQuaternion & q)
{
	BaseClass::setOrient(q);
	if(m_pRigidBody)
	{
		m_pRigidBody->getWorldTransform().setRotation(Q4_BTQUAT(q));

		SPhysics_GetDynWorld()->updateSingleAabb(m_pRigidBody);
	}
}

void CBaseAnimating::_cleanup()
{
	releasePhysics();

	BaseClass::_cleanup();
}

void CBaseAnimating::onAnimationStart(UINT uLayer)
{
}
void CBaseAnimating::onAnimationStop(UINT uLayer)
{
	if(m_vNextAnim[uLayer].szName[0] != 0)
	{
		if(m_vNextAnim[uLayer].isActivity)
		{
			playActivity(m_vNextAnim[uLayer].szName, m_vNextAnim[uLayer].uFadeTime, uLayer);
		}
		else
		{
			playAnimation(m_vNextAnim[uLayer].szName, m_vNextAnim[uLayer].uFadeTime, uLayer);
		}

		m_vNextAnim[uLayer].szName[0] = 0;
	}
}
void CBaseAnimating::onAnimationLoop(UINT uLayer)
{
}
void CBaseAnimating::onAnimationProgress(UINT uLayer, float fProgress)
{
}

void CBaseAnimating::playAnimationNext(const char * name, UINT uFadeTime, UINT slot)
{
	assert(slot < BLEND_MAX);

	strncpy(m_vNextAnim[slot].szName, name, XMODEL_MAX_NAME);
	m_vNextAnim[slot].szName[XMODEL_MAX_NAME - 1] = 0;
	m_vNextAnim[slot].isActivity = false;
	m_vNextAnim[slot].uFadeTime = uFadeTime;
}

void CBaseAnimating::playActivityNext(const char * name, UINT uFadeTime, UINT slot)
{
	assert(slot < BLEND_MAX);

	strncpy(m_vNextAnim[slot].szName, name, XMODEL_MAX_NAME);
	m_vNextAnim[slot].szName[XMODEL_MAX_NAME - 1] = 0;
	m_vNextAnim[slot].isActivity = true;
	m_vNextAnim[slot].uFadeTime = uFadeTime;
}

void CBaseAnimating::cancelNextAnimation(int iSlot)
{
	assert(iSlot < BLEND_MAX);

	if(iSlot >= 0)
	{
		m_vNextAnim[iSlot].szName[0] = 0;
	}
	else
	{
		for(int i = 0; i < BLEND_MAX; ++i)
		{
			m_vNextAnim[i].szName[0] = 0;
		}
	}
}

void CBaseAnimating::onIsStaticChange(bool isStatic)
{
	if(m_pRigidBody && m_isStatic != isStatic)
	{
		if(isStatic)
		{
			m_pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
			m_pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

			SPhysics_RemoveShape(m_pRigidBody);
			SPhysics_AddShapeEx(m_pRigidBody, CG_STATIC, CG_STATIC_MASK);
		}
		else
		{
			m_pRigidBody->setLinearFactor(btVector3(1.0f, 1.0f, 1.0f));
			m_pRigidBody->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));

			SPhysics_RemoveShape(m_pRigidBody);
			SPhysics_AddShapeEx(m_pRigidBody, m_collisionGroup, m_collisionMask);
		}
	}
	m_isStatic = isStatic;
}

void CBaseAnimating::inputPlayAnim(inputdata_t * pInputdata)
{
	if(pInputdata->type != PDF_STRING)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CBaseAnimating::inputPlayAnim() expected parameter type string");
		return;
	}
	playAnimation(pInputdata->parameter.str);
}

void CBaseAnimating::inputPlayAnimNext(inputdata_t * pInputdata)
{
	if(pInputdata->type != PDF_STRING)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CBaseAnimating::inputPlayAnimNext() expected parameter type string");
		return;
	}
	playAnimationNext(pInputdata->parameter.str);
}


void CBaseAnimating::inputPlayActivity(inputdata_t * pInputdata)
{
	if(pInputdata->type != PDF_STRING)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CBaseAnimating::inputPlayActivity() expected parameter type string");
		return;
	}
	playActivity(pInputdata->parameter.str);
}

void CBaseAnimating::inputPlayActivityNext(inputdata_t * pInputdata)
{
	if(pInputdata->type != PDF_STRING)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "CBaseAnimating::inputPlayActivityNext() expected parameter type string");
		return;
	}
	playActivityNext(pInputdata->parameter.str);
}

void CBaseAnimating::inputSetSkin(inputdata_t * pInputdata)
{
	int iSkin;
	switch(pInputdata->type)
	{
	case PDF_INT:
		iSkin = pInputdata->parameter.i;
		break;

	case PDF_FLOAT:
		iSkin = (int)pInputdata->parameter.f;
		break;

	case PDF_BOOL:
		iSkin = pInputdata->parameter.b ? 1 : 0;
		break;

	case PDF_STRING:
		if(sscanf(pInputdata->parameter.str, "%d", &iSkin))
		{
			break;
		}

	default:
		LibReport(REPORT_MSG_LEVEL_WARNING, "CBaseAnimating::inputSetSkin() expected parameter type int");
		return;
	}

	setSkin(iSkin);
}

void CBaseAnimating::setSkin(int iSkin)
{
	if(m_pModel && m_pModel->asAnimatedModel())
	{
		m_pModel->asAnimatedModel()->setSkin(iSkin);
	}
	m_iSkin = iSkin;
}

void CBaseAnimating::_initEditorBoxes()
{
	// do nothing
}

void CBaseAnimating::_releaseEditorBoxes()
{
	// do nothing
}

void CBaseAnimating::renderEditor(bool is3D)
{
	if(m_pModel)
	{
		m_pModel->render(0, false);
		m_pModel->render(0, true);
	}
}
