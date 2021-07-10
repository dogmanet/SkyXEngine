#if 0

#include "DSbase.h"
#include "DSHCplayer.h"

#include "DSwpnTAR.h"

/*
Known bugs:
	1. There is no time linking in Advance method. So some operations may be faster or slower
*/

float * g_fHitFraction;
float * g_fFriction;
float * g_fMass;

DSbaseTool * g_pWpn[2];
DSbaseAnimating * g_pTbl;

void FlatChunk(std::string arg)
{
	CChunk * pc = DS::pPlayer->m_pInChunk;
	if(pc)
	{
		for(int x = 0; x < HC_CHUNK_SIZE_X; x++)
		{
			for(int z = 0; z < HC_CHUNK_SIZE_Z; z++)
			{
				for(int y = 0; y < 64; y++)
				{
					pc->SetBlock(x, z, y, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_GRASS), false);
				}
				for(int y = 64; y < HC_CHUNK_SIZE_Y; y++)
				{
					pc->SetBlock(x, z, y, HC_MKTYPE(HC_BLOCK_TYPE_NONE, HC_BLOCK_AIR), false);
				}
			}
		}
		pc->SetBlock(HC_CHUNK_SIZE_X - 1, HC_CHUNK_SIZE_Z - 1, HC_CHUNK_SIZE_Y - 1, HC_MKTYPE(HC_BLOCK_TYPE_NONE, HC_BLOCK_AIR), true);
	}
}

DSHCplayer::DSHCplayer():m_pLVB(NULL),m_bIsReady(false), m_uMoveDir(0), m_bOnGround(false), m_pInChunk(NULL), m_iXblock(0), m_iYblock(0), m_iZblock(0),
m_fXblock(0.0f), m_fYblock(0.0f), m_fZblock(0.0f), m_pActiveTool(NULL), m_pBlockCollideShape(NULL), m_pBlockRigidBody(NULL)
{
#undef new
	DS_REGISTER_CONCMD("flat", FlatChunk);
	m_pCamera = new DScamera();

	m_vPosition = XMFLOAT3(16.0f, 494.0f, 35.0f);
	//m_vPosition = XMFLOAT3(1.6f, 102.4f, 1.6f);

	/*m_pCollideShape = new btCapsuleShape(HC_PR, HC_PH);

	btVector3 localInertia(0, 0, 0);
	DS_REGISTER_CVAR("phy_mass", 75.0f, "");
	g_fMass = DS_GET_PCVAR_FLOAT("phy_mass");
	m_pCollideShape->calculateLocalInertia(*g_fMass, localInertia);

	btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(16.0f, 1024.0f, 16.0f)));
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		*g_fMass,                  // mass
		motionState,        // initial position
		m_pCollideShape,    // collision shape of body
		localInertia  // local inertia
		);

	m_pRigidBody = new btRigidBody(rigidBodyCI);

	DS_REGISTER_CVAR("phy_hitfraction", 0.3f, "");
	DS_REGISTER_CVAR("phy_friction", 100.0f, "");

	g_fHitFraction = DS_GET_PCVAR_FLOAT("phy_hitfraction");
	g_fFriction = DS_GET_PCVAR_FLOAT("phy_friction");

	//m_pRigidBody->setFriction(*g_fFriction);
	m_pRigidBody->setFriction(0.0f);
	//m_pRigidBody->setHitFraction(*g_fHitFraction);
	m_pRigidBody->setCcdMotionThreshold(1.0f);
	m_pRigidBody->setCcdSweptSphereRadius(HC_PH);

	m_pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	//m_pRigidBody->set

	

	DSphysics::AddShape(m_pRigidBody);*/
	/////////////////////////////////////////////////////
	m_iFlags |= DS_ENT_RENDERABLE;
	btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setOrigin (btVector3(0.0, 4.0, 0.0));
	startTransform.setOrigin(XM3_BTVEC(m_vPosition));


	m_pGhostObject = new btPairCachingGhostObject();
	m_pGhostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	m_pCollideShape = new btCapsuleShape(HC_PR, HC_PH - HC_PR * 3.0f);
	m_pGhostObject->setCollisionShape(m_pCollideShape);
	m_pGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

	DS_REGISTER_CVAR("phy_stepheight", 4.5f, "Max height that player can step");//@@TODO: Add changes listener for this
	//DS_REGISTER_CVAR("phy_stepheight", 0.45f, "Max height that player can step");//@@TODO: Add changes listener for this
	//g_fMass = DS_GET_PCVAR_FLOAT("phy_stepheight");

	btScalar stepHeight = DScVarSystem::getf("phy_stepheight");
	m_pCharacter = new btKinematicCharacterController(m_pGhostObject, (btConvexShape*)m_pCollideShape, stepHeight);
	m_pCharacter->setMaxJumpHeight(HC_H * 1.60f);
	m_pCharacter->setJumpSpeed(35.0f);
	//m_pCharacter->setJumpSpeed(3.5f);
	m_pCharacter->setGravity(100.0f);
	//m_pCharacter->setGravity(1.0f);
	m_pCharacter->setFallSpeed(300.0f);
	//m_pCharacter->setFallSpeed(30.0f);

	///only collide with static for now (no interaction with dynamic objects)
	DSphysics::m_pDynamicsWorld->addCollisionObject(m_pGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

	m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	DSphysics::m_pDynamicsWorld->addAction(m_pCharacter);
#define new DS_NEW

	{
		DSmaterialSystem::vertex v[36];
		v[0].Pos = XMFLOAT3(-HC_R, 0.0f, 0.0f);
		v[1].Pos = XMFLOAT3(-HC_R / 2.0f, 0.0f, Rcos30);
		v[2].Pos = XMFLOAT3(-HC_R / 2.0f, 0.0f, Rcos30);
		v[3].Pos = XMFLOAT3(HC_R / 2.0f, 0.0f, Rcos30);
		v[4].Pos = XMFLOAT3(HC_R / 2.0f, 0.0f, Rcos30);
		v[5].Pos = XMFLOAT3(HC_R, 0.0f, 0.0f);
		v[6].Pos = XMFLOAT3(HC_R, 0.0f, 0.0f);
		v[7].Pos = XMFLOAT3(HC_R / 2.0f, 0.0f, -Rcos30);
		v[8].Pos = XMFLOAT3(HC_R / 2.0f, 0.0f, -Rcos30);
		v[9].Pos = XMFLOAT3(-HC_R / 2.0f, 0.0f, -Rcos30);
		v[10].Pos = XMFLOAT3(-HC_R / 2.0f, 0.0f, -Rcos30);
		v[11].Pos = XMFLOAT3(-HC_R, 0.0f, 0.0f);

		v[12].Pos = XMFLOAT3(-HC_R, 0.0f, 0.0f);
		v[13].Pos = XMFLOAT3(-HC_R, HC_H, 0.0f);
		v[14].Pos = XMFLOAT3(-HC_R / 2.0f, 0.0f, Rcos30);
		v[15].Pos = XMFLOAT3(-HC_R / 2.0f, HC_H, Rcos30);
		v[16].Pos = XMFLOAT3(HC_R / 2.0f, 0.0f, Rcos30);
		v[17].Pos = XMFLOAT3(HC_R / 2.0f, HC_H, Rcos30);
		v[18].Pos = XMFLOAT3(HC_R, 0.0f, 0.0f);
		v[19].Pos = XMFLOAT3(HC_R, HC_H, 0.0f);
		v[20].Pos = XMFLOAT3(HC_R / 2.0f, 0.0f, -Rcos30);
		v[21].Pos = XMFLOAT3(HC_R / 2.0f, HC_H, -Rcos30);
		v[22].Pos = XMFLOAT3(-HC_R / 2.0f, 0.0f, -Rcos30);
		v[23].Pos = XMFLOAT3(-HC_R / 2.0f, HC_H, -Rcos30);

		v[24].Pos = XMFLOAT3(-HC_R, HC_H, 0.0f);
		v[25].Pos = XMFLOAT3(-HC_R / 2.0f, HC_H, Rcos30);
		v[26].Pos = XMFLOAT3(-HC_R / 2.0f, HC_H, Rcos30);
		v[27].Pos = XMFLOAT3(HC_R / 2.0f, HC_H, Rcos30);
		v[28].Pos = XMFLOAT3(HC_R / 2.0f, HC_H, Rcos30);
		v[29].Pos = XMFLOAT3(HC_R, HC_H, 0.0f);
		v[30].Pos = XMFLOAT3(HC_R, HC_H, 0.0f);
		v[31].Pos = XMFLOAT3(HC_R / 2.0f, HC_H, -Rcos30);
		v[32].Pos = XMFLOAT3(HC_R / 2.0f, HC_H, -Rcos30);
		v[33].Pos = XMFLOAT3(-HC_R / 2.0f, HC_H, -Rcos30);
		v[34].Pos = XMFLOAT3(-HC_R / 2.0f, HC_H, -Rcos30);
		v[35].Pos = XMFLOAT3(-HC_R, HC_H, 0.0f);

#undef new
		m_pBlockCollideShape = new btConvexHullShape();
		for(UINT i = 0; i < 36; i++)
		{
			m_pBlockCollideShape->addPoint(XM3_BTVEC(v[i].Pos));
		}
		if(m_pBlockCollideShape->getNumPoints())
		{
			btVector3 vInertia;
			const float fMass = 1.0f;
			m_pBlockCollideShape->calculateLocalInertia(fMass, vInertia);

			btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), XM3_BTVEC(m_vPosition)));
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
				fMass,                  // mass
				motionState,        // initial position
				m_pBlockCollideShape,    // collision shape of body
				vInertia  // local inertia
				);
			m_pBlockRigidBody = new btRigidBody(rigidBodyCI);
			m_pBlockRigidBody->setGravity(btVector3(0.0f, 0.0f, 0.0f));
			m_pBlockRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
			m_pBlockRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
#define new DS_NEW
			//m_pBlockRigidBody->setCollisionFlags(m_pBlockRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			DSphysics::AddShape(m_pBlockRigidBody);
			//m_pBlockRigidBody->set
			//gContactAddedCallback = 
		}

		

		DS::CreateBuffers(v, 36, NULL, 0, &m_pLVB, NULL);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_pActiveTool = new DSwpnTAR();
	m_pActiveTool = CBlockManager::CreateTool(HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_GRASS));
	m_pActiveTool->SetOwner(this);
	m_pActiveTool->Deploy();

	m_bIsReady = true;

//	g_pWpn[0] = new DSwpnTAR(true);
//	g_pWpn[1] = new DSwpnTAR(true);
//	g_pWpn[0]->SetPos(XMFLOAT3(30.0f, 600.0f, 30.0f));
//	g_pWpn[1]->SetPos(XMFLOAT3(35.0f, 610.0f, 30.0f));

	//g_pTbl = new DSbaseAnimating();
//	g_pTbl->SetModel("/models/props/table");
	//g_pTbl->SetModel("/models/darkvalley");
	//g_pTbl->SetPos(XMFLOAT3(30.0f, 550.0f, 30.0f));
	//g_pWpn->SetPos(XMFLOAT3(0.3f, 10.24f, 0.3f));
	//g_pWpn->SetOrient(DSquaternion(90.0f, 'y'));
	//g_pWpn->SetOrient(DSquaternion(0.0f, 1.0f, 0.0f, 1.0f));
}

void DSHCplayer::Render()
{
	//btTransform trans;
	//m_pRigidBody->getMotionState()->getWorldTransform(trans);
	
	//DS::UTDrawLine(XMFLOAT3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()), XMFLOAT3());
	if(pAimPos.iAimType)
	{
		UINT stride = sizeof(DSmaterialSystem::vertex);
		UINT offset = 0;
		DS::g_pImmediateContext()->IASetVertexBuffers(0, 1, &m_pLVB, &stride, &offset);
		DS::g_pImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		static DSMATERIAL pMat = NULL;
		if(!pMat)
		{
			DSmaterialSystem::bindMaterial("/dev/white");
			pMat = DSmaterialSystem::GetMaterial("/dev/white");
		}
		else
		{
			DSmaterialSystem::bindMaterial(pMat);
		}
		DSmaterialSystem::XMSetWord(XMMatrixTranslation(pAimPos.vBPos.x, pAimPos.vBPos.y, pAimPos.vBPos.z));
		DSmaterialSystem::PreRender();
		DS::g_pImmediateContext()->Draw(36, 0);
	}
}

void DSHCplayer::Move(UINT dir, bool start)
{
	if(start)
	{
		m_uMoveDir |= dir;
	}
	else
	{
		m_uMoveDir &= ~dir;
	}
}

void DSHCplayer::Advance(DS_TIME time)
{
	if(m_bIsReady)
	{




		static bool * bFreeCam = DS_GET_PCVAR_BOOL("cam_free");
		m_pCharacter->setGravity(*bFreeCam ? 0.0f : 100.0f);
		if(*bFreeCam)
		{
			//m_pCharacter->setS
		}
		//m_pRigidBody->setFriction(*g_fFriction);
		//m_pRigidBody->setHitFraction(*g_fHitFraction);
		//btVector3 localInertia(0, 0, 0);
		//m_pCollideShape->calculateLocalInertia(*g_fMass, localInertia);
		//m_pRigidBody->setMassProps(*g_fMass, localInertia);

		//btTransform trans;
		//m_pRigidBody->getMotionState()->getWorldTransform(trans);

		//DS::UTDrawLine(XMFLOAT3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()), XMFLOAT3());


		float dt = (time - m_iCurTime) / 10.0f;
		m_iCurTime = time;
		int te;
		int ts0 = te = GetTickCount();

		XMFLOAT3 movespeed = XMFLOAT3(0.0f, 0.0f, 0.0f);

		int ts1 = GetTickCount();

		if(m_uMoveDir & HC_PLAYER_MOVE_FORWARD)
		{
			movespeed.x += m_uMoveDir & HC_PLAYER_MOVE_CROUCH ? 1.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 10.0f : 5.5f;
			//vel.setX(vel.getX() + (m_uMoveDir & HC_PLAYER_MOVE_CROUCH) ? 0.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 1.0f : 50.0f);
		}
		if(m_uMoveDir & HC_PLAYER_MOVE_BACKWARD)
		{
			movespeed.x -= m_uMoveDir & HC_PLAYER_MOVE_CROUCH ? 1.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 10.0f : 5.5f;
			//vel.setX(vel.getX() - (m_uMoveDir & HC_PLAYER_MOVE_CROUCH) ? 0.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 1.0f : 50.0f);
		}
		if(m_uMoveDir & HC_PLAYER_MOVE_LEFT)
		{
			movespeed.z -= m_uMoveDir & HC_PLAYER_MOVE_CROUCH ? 1.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 10.0f : 5.5f;
			//vel.setZ(vel.getZ() - (m_uMoveDir & HC_PLAYER_MOVE_CROUCH) ? 0.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 1.0f : 50.0f);
			//btMatrix3x3 orn = m_pGhostObject->getWorldTransform().getBasis();
			//orn *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), 0.01));
			//m_pGhostObject->getWorldTransform().setBasis(orn);
		}
		if(m_uMoveDir & HC_PLAYER_MOVE_RIGHT)
		{
			movespeed.z += m_uMoveDir & HC_PLAYER_MOVE_CROUCH ? 1.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 10.0f : 5.5f;
			//vel.setZ(vel.getZ() + (m_uMoveDir & HC_PLAYER_MOVE_CROUCH) ? 0.1f : m_uMoveDir & HC_PLAYER_MOVE_RUN ? 1.0f : 50.0f);
		}
		if(m_uMoveDir & HC_PLAYER_MOVE_JUMP)
		{
			//btVector3 vel = m_pRigidBody->getLinearVelocity();
			if(*bFreeCam)
			{
				movespeed.y += m_uMoveDir & HC_PLAYER_MOVE_RUN ? 10.0f : 5.5f;
			}
			else
			{
				if(m_pCharacter->canJump())
				{
					m_pCharacter->jump();
					//m_pRigidBody->applyCentralImpulse(btVector3(0.0f, 1000.0f, 0.0f));
				}
			}
		}

		if(m_uMoveDir & HC_PLAYER_MOVE_CROUCH)
		{
			if(*bFreeCam)
			{
				movespeed.y -= m_uMoveDir & HC_PLAYER_MOVE_RUN ? 10.0f : 5.5f;
			}
		}

		int ts2 = GetTickCount();
		//movespeed = movespeed * m_pCamera->GetViewDir();
		m_vSpeed = m_pCamera->TransformSpeed(movespeed);
		//btVector3 vel = m_pRigidBody->getLinearVelocity();
		//vel.setX(0.0f);
		//vel.setZ(0.0f);
		//m_pRigidBody->setLinearVelocity(vel + XM3_BTVEC(m_vSpeed));
		//m_pRigidBody->activate();

		btVector3 sp = XM3_BTVEC(m_vSpeed);

		//if(sp.length())
		{
			m_pCharacter->setWalkDirection(sp / 10.0f);
		}

		int ts3 = GetTickCount();

		//m_vPosition = m_vPosition + m_vSpeed * (float)dt;
		float fX = 0;
		float fZ = 0;
		XMFLOAT3 vec = DS::pCamera->GetViewDir();
		XMFLOAT3 pos = DS::pCamera->GetPos();

		if(DS::pDSHCCM)
		{
			m_pInChunk = DS::pDSHCCM->FindRenderableChunkByXZ(m_vPosition.x, m_vPosition.z);
			if(m_pInChunk)
			{
				m_pInChunk->FindUserInChunk(this, (UINT*)&m_iXblock, (UINT*)&m_iZblock, (UINT*)&m_iYblock, &fX, &fZ, &m_fYblock);
				m_fXblock = fX;
				m_fZblock = fZ;
				//DS::pDSHCCM->FindAimingBlock(pos, vec, 10.0f, m_pInChunk);
			}
		}
		int ts4 = GetTickCount();
		int64_t iX = 0;
		int64_t iZ = 0;

		if(m_pInChunk)
		{
			m_pInChunk->GetPos(&iX, &iZ);
		}
		int ts5 = GetTickCount();
		float p = 0.0f;
		if(DS::pDSHCCM)
		{
			//m_vPosition = DS::pDSHCCM->HCfixPosition(m_vPosition);
			//DS::pDSHCCM->HCfixPosition(this);
			//		p = DS::pDSHCCM->IsOnGround(this);
		}

		int ts6 = GetTickCount();


		if(p >= 0.0f)
		{
			m_bOnGround = true;
			m_vPosition.y += p;
		}
		else
		{
			m_bOnGround = false;
		}
		int ts7 = GetTickCount();
		if(DS::pDSHCCM)
		{
			m_pInChunk = DS::pDSHCCM->FindRenderableChunkByXZ(m_vPosition.x, m_vPosition.z);
			if(m_pInChunk)
			{
				m_pInChunk->FindUserInChunk(this, (UINT*)&m_iXblock, (UINT*)&m_iZblock, (UINT*)&m_iYblock, &fX, &fZ, &m_fYblock);
				m_fXblock = fX;
				m_fZblock = fZ;
				DS::pDSHCCM->FindAimingBlock(pos, vec, 10.0f, m_pInChunk, &pAimPos);
			}
		}
		int ts8 = GetTickCount();
		static float fHeight = 5.0f;
		if(m_uMoveDir & HC_PLAYER_MOVE_CROUCH)
		{
			if(fHeight > 2.0f)
			{
				fHeight -= 0.5f;
			}
		}
		else if(fHeight < 5.0f)
		{
			fHeight += 0.5f;
		}

		btTransform trans;
		//look at the vehicle
		trans = m_pGhostObject->getWorldTransform();
		//btVector3 up = characterWorldTrans.getBasis()[1];
		//btVector3 backward = -characterWorldTrans.getBasis()[2];
		//up.normalize();
		//backward.normalize();

		//m_cameraTargetPosition = characterWorldTrans.getOrigin();
		//m_cameraPosition = m_cameraTargetPosition + up * 10.0 + backward * 12.0;
		m_vPosition = XMFLOAT3(trans.getOrigin().getX(), trans.getOrigin().getY() + fHeight, trans.getOrigin().getZ());


		//g_pWpn->SetPos(m_vPosition);
		/*if(DS::pKeyBoard->KeyState(DSKEY_R))
		{
			((DSbaseWeapon*)m_pActiveTool)->Reload();
		}*/

		/*if(DS::pKeyBoard->KeyState(DSKEY_LBUTTON))
		{
		((DSbaseWeapon*)g_pWpn)->PrimaryAction();
		}*/
		//DS_WAIT(DS::pDSHCCM->m
		//if(!*bFreeCam)
	{
		//m_pCamera->SetPos(m_vPosition + XMFLOAT3(0.0f, fHeight, 0.0f));
		m_pCamera->SetPos(m_vPosition);
	}
	/*else
	{
	m_vPosition = m_pCamera->GetPos();
	m_vSpeed = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}*/
	int ts9 = GetTickCount();
	te = GetTickCount() - te;

	/*DS::pDSGUI->GetElementById("position")->SetText(L"x: " + stringw(floatToStr(m_vPosition.x)) + L'\n'+
		L"y: " + stringw(floatToStr(m_vPosition.y)) + L'\n' +
		L"z: " + stringw(floatToStr(m_vPosition.z)) + L'\n' +
		L"ChunkX: " + stringw(numToStr(iX)) + L'\n' +
		L"ChunkZ: " + stringw(numToStr(iZ)) + L'\n' +
		L'\n' +
		L"BlockX: " + stringw(numToStr(m_iXblock)) + L'\n' +
		L"BlockY: " + stringw(numToStr(m_iYblock)) + L'\n' +
		L"BlockZ: " + stringw(numToStr(m_iZblock)) + L'\n' +
		L'\n' +
		L"BlockWX: " + stringw(floatToStr(fX)) + L'\n' +
		L"BlockWZ: " + stringw(floatToStr(fZ)) + L'\n' +
		L'\n' +
		L"x: " + stringw(floatToStr(vec.x)) + L'\n' +
		L"y: " + stringw(floatToStr(vec.y)) + L'\n' +
		L"z: " + stringw(floatToStr(vec.z)) + L'\n'
		);*/
	if(DS::pDSHCCM)
	{
		DSGUIBase * el = DS::pDSGUI->GetElementById("position");
		if(el)
		{
			el->SetText(L"x: " + stringw(floatToStr(m_vPosition.x)) + L'\n' +
				L"y: " + stringw(floatToStr(m_vPosition.y)) + L'\n' +
				L"z: " + stringw(floatToStr(m_vPosition.z)) + L'\n' +
				L"Physics: " + stringw(numToStr(DSphysics::m_pAdvTime)) + L'\n' +
				L"ChunkRendered: " + stringw(numToStr(DS::pDSHCCM->m_iChunkCountR))
				);
		}
	}


	/*if(m_vPosition.z > 32.0f)
	{
	_asm
	{
	int 3;
	}
	}
	if(m_vPosition.z > 16.0f)
	{
	DSconsole::write("%f\n", m_vPosition.z);
	}*/
	//DS::sleep(50);

	//XMFLOAT3 a(-5.0f, 1.0f, 3.0f);
	//XMFLOAT3 b(4.0f, 5.0f, -1.0f);
	//XMFLOAT3 c = 4 * a - 5 * b;
	//XMFLOAT3 d = 7 * b + 2 * a;
	//D3DXVECTOR4_16F a;

	//float f = 1.0f;
	//WORD w = *((WORD*)&f);



	//w = 15360;
	//f = *((float*)&w);

	//f = *D3DXFloat16To32Array(
	}
	BaseClass::Advance(time);
}

bool DSHCplayer::IsOnGround()
{
	return(m_bOnGround);
}

DSHCplayer::~DSHCplayer()
{
	DS_SAFE_DELETE(m_pCamera);
	DS_SAFE_RELEASE_DELETE(m_pActiveTool);
	DS_SAFE_RELEASE(m_pLVB);
}

DScamera * DSHCplayer::GetCamera()
{
	return(m_pCamera);
}

CChunk * DSHCplayer::GetChunk()
{
	return(m_pInChunk);
}

void DSHCplayer::GetBlockIn(int * pX, int * pY, int * pZ)
{
	if(pX)
	{
		*pX = m_iXblock;
	}
	if(pY)
	{
		*pY = m_iYblock;
	}
	if(pZ)
	{
		*pZ = m_iZblock;
	}
}

void DSHCplayer::GetBlockInPos(float * pX, float * pY, float * pZ)
{
	if(pX)
	{
		*pX = m_fXblock;
	}
	if(pY)
	{
		*pY = m_fYblock;
	}
	if(pZ)
	{
		*pZ = m_fZblock;
	}
}

void DSHCplayer::PrimaryAttack(bool st)
{
	if(m_pActiveTool)
	{
		m_pActiveTool->PrimaryAction(st);
	}
}

void DSHCplayer::SecondaryAttack(bool st)
{
	if(m_pActiveTool)
	{
		m_pActiveTool->SecondaryAction(st);
	}
}

void DSHCplayer::PlaceBlock(UINT type)
{
	if(pAimPos.pChunk && pAimPos.iAimType)
	{
		btTransform tr = m_pBlockRigidBody->getWorldTransform();
		XMFLOAT3 vBPos = pAimPos.pChunk->GetBlockPos(pAimPos.iNewPosX, pAimPos.iNewPosY, pAimPos.iNewPosZ);
		tr.getOrigin().setX(vBPos.x);
		tr.getOrigin().setY(vBPos.y);
		tr.getOrigin().setZ(vBPos.z);
		m_pBlockRigidBody->setWorldTransform(tr);
		m_pBlockRigidBody->activate();
		//DSconsole::write("%f, %f, %f\n", vBPos.x, vBPos.y, vBPos.z);
		//m_pBlockRigidBody->is
		pAimPos.pChunk->SetBlock(pAimPos.iNewPosX, pAimPos.iNewPosY, pAimPos.iNewPosZ, type);
	}
	/*else
	{
		btTransform tr = m_pBlockRigidBody->getWorldTransform();
		tr.getOrigin().setX(0.0f);
		tr.getOrigin().setY(-HC_H);
		tr.getOrigin().setZ(0.0f);
		m_pBlockRigidBody->setWorldTransform(tr);
	}*/
}

void DSHCplayer::RemoveBlock()
{
	if(pAimPos.pChunk && pAimPos.iAimType)
	{
		pAimPos.pChunk->RemoveBlock(pAimPos.iAimPosX, pAimPos.iAimPosY, pAimPos.iAimPosZ);
	}
}

#endif
