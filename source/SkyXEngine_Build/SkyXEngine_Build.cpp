
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS
#pragma once

#define SX_EXE
#define SX_GAME

#include <SkyXEngine.cpp>

#include <physics/sxphysics.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SkyXEngine_Init();
	SkyXEngine_PreviewKill();
	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyCloudsSetWidthHeightPos(2000, 2000, &float3(0, 0, 0));

	Level::Load("stalker_atp");

	SGeom_0SettGreenSetFreqGrass(30);

	/*for (int i = 0; i < 10; ++i)
	{
	for (int k = 0; k < 10; ++k)
	{
	ID tmpid = SPE_EffectGetByName("test");
	//SPE_EffectPosSet(tmpid, &float3(i, 0, k));
	//SPE_EffectEnableSet(tmpid, true);
	SPE_EffectPlayByID(tmpid, &float3(i, 0, k), 0);
	}
	}*/

	/*ID tmpid = SPE_EffectGetByName("test");
	SPE_EffectEnableSet(tmpid, true);*/

	
#if 0
	btBoxShape shape(btVector3(0.25, 0.9, 0.25));
	for(int x = -20; x <= 20; ++x)
	{
		for(int y = -20; y <= 20; ++y)
		{
			float3 fTopPos(x, 100, y);
			float3 fBottomPos(x, -100, y);

			float3 n;
			float3 tsBasis[3];
			SMMATRIX mBasis;
			SMMATRIX mBasisInv;
			float det;

			btDiscreteDynamicsWorld::ClosestConvexResultCallback cb(F3_BTVEC(fTopPos), F3_BTVEC(fBottomPos));
			btTransform xForm;
			xForm.setOrigin(F3_BTVEC(fTopPos));
			xForm.getBasis().setIdentity();
			btTransform xForm2;
			xForm2.setOrigin(F3_BTVEC(fBottomPos));
			xForm2.getBasis().setIdentity();
			SXPhysics_GetDynWorld()->convexSweepTest(&shape, xForm, xForm2, cb);
			//AllConvexResultCallback::part * part;
			if(cb.hasHit())
			{
				// Центр бокса: 
				// cb.m_hitPointWorld + btVector3(0, shape.getHalfExtentsWithoutMargin().getY(), 0)


				/* btTransform tr;
				tr.setOrigin(cb.m_hitPointWorld + btVector3(0, shape.getHalfExtentsWithoutMargin().getY(), 0));
				tr.getBasis().setIdentity();
				btDefaultMotionState motionState(tr);
				btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
					0,                  // mass
					&motionState,        // initial position
					&shape,    // collision shape of body
					btVector3(0, 0, 0)  // local inertia
					);
				btRigidBody * body = new btRigidBody(rigidBodyCI);

				//body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

				SXPhysics_AddShape(body); */
			}
		}
	}
#endif

	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_LoadTexLoadTextures();

	/*ID tmpids = SSCore_SndCreate3d("Exclusion_zone.ogg", false, 0, 100, 0.1);
	SSCore_SndPosCurrSet(tmpids, 20, SOUND_POS_SEC);
	SSCore_SndPlay(tmpids);*/
	/*ID tmpidsnd = SSCore_SndCreate2d("Exclusion_zone.ogg", false, 0);
	if (tmpidsnd >= 0)
	{
		SSCore_SndPosCurrSet(tmpidsnd, 20, SOUND_POS_SEC);
		SSCore_SndPlay(tmpidsnd);
		//ms->SoundPanSet(tmpid, DSBPAN_RIGHT, 0);
		//ms->SoundVolumeSet(tmpid, 100);
	}


	SSCore_SndInstancePlay2d(tmpidsnd);
	//SSCore_SndInstancePlay2d(tmpidsnd);*/

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}