
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS
#pragma once

#define SX_EXE
#define SX_GAME

#include <SkyXEngine.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	InitOutLog();
	srand((unsigned int)time(0));
	
	GData::InitWin("SkyXEngine", "SkyXEngine");

	SkyXEngine_Init();
	
	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyCloudsSetWidthHeightPos(2000, 2000, &float3(0, 0, 0));

	Level::Load("stalker_atp");
	SXPhysics_LoadGeom();

	SGeom_0SettGreenSetFreqGrass(30);

	IAnimPlayer * pl;
	pl = SXAnim_CreatePlayer("models/stalker_zombi/stalker_zombi_a.dse");
	pl->SetPos(float3(-17.18, -1.38f, -32.3));
	pl->Play("reload");
	
	//TestEffect = new Effect();



	char tmppathsave[1024];
	sprintf(tmppathsave, "%seff.eff", GData::Pathes::GameSource);

	SPE_EffectLoad(tmppathsave);

	//ID tmpeffid = SPE_EffectAdd("test");
	//ID tmppartid = SPE_ParticlesAdd(tmpeffid, &data);

	//SPE_ParticlesTextureSet(0, 0, "pfx_smoke_c.dds");
	//SPE_ParticlesCreate(0, 0, 0);
	//SPE_EffectEnableSet(0, false);

	/*for (int i = 0; i < 20; ++i)
	{
		for (int k = 0; k < 20; ++k)
		{
			ID tmpid = SPE_EffectCopyName("test");
			SPE_EffectPosSet(tmpid, &float3(i, 0, k));
			SPE_EffectEnableSet(tmpid, true);
		}
	}*/

	ID tmpid = SPE_EffectGetByName("test");
	SPE_EffectEnableSet(tmpid, true);

	/*SPE_EffectCopyName("test");
	SPE_EffectCopyName("test");*/


	//SPE_EffectSave(tmppathsave);

	/*SPE_ParticlesSet(tmpeffid, tmppartid, SizeParticle, float2_t(2, 2));*/
	//SPE_ParticlesSet(tmpeffid, tmppartid, TransparencyCoef, 1);
	//SPE_ParticlesSet(tmpeffid, tmppartid, FigureCountQuads, 10);
	//SPE_ParticlesSet(tmpeffid, tmppartid, FigureRotRand, true);
	//SPE_ParticlesSet(tmpeffid, tmppartid, FigureTapX, true);
	//SPE_ParticlesSet(tmpeffid, tmppartid, FigureTapZ, true);

	/*SGeom_ModelsAddModel("stalker_atp.dse", 0, "stalker_atp.dse");

	SGeom_GreenAddGreen("tree_topol", "terrain_mp_atp_mask_tree.dds", 0.3, "green\\tree_topol_lod0.dse", "green\\tree_topol_lod1.dse", "green\\tree_topol_lod2.dse", "green\\tree_topol_lod2.dse");
	SGeom_GreenAddGreen("trava_green_det2", "terrain_mp_atp_mask_grass.dds", 0.2, "green\\trava_green_det2.dse", 0, 0, 0);


	SML_LigthsCreatePoint(
		&float3(60,60,0),
		LIGHTS_GLOBAL_MAX_POWER,
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		true);
		SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
		SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");
		
	Level::Save("stalker_atp"); */

	char tmppathexe[1024];
	char tmppath[1024];
	GetModuleFileName(NULL, tmppath, 1024);
	int len = strlen(tmppath);
	while (tmppath[len--] != '\\')
	{
		if (tmppath[len - 1] == '\\')
		{
			len--;
			memcpy(tmppathexe, tmppath, len);
			tmppathexe[len] = 0;
		}
	}

	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_LoadTexLoadTextures();

	/*ID tmpids = SSCore_SndCreate3d("Exclusion_zone.ogg", false, 0, 100, 0.1);
	SSCore_SndPosCurrSet(tmpids, 20, SOUND_POS_SEC);
	SSCore_SndPlay(tmpids);*/
	ID tmpidsnd = SSCore_SndCreate3d("Exclusion_zone.ogg", false, 0, 100, 0.1);
	if (tmpidsnd >= 0)
	{
		SSCore_SndPosCurrSet(tmpidsnd, 20, SOUND_POS_SEC);
		SSCore_SndPlay(tmpidsnd);
	//ms->SoundPanSet(tmpid, DSBPAN_RIGHT, 0);
	//ms->SoundVolumeSet(tmpid, 100);
	}

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static DWORD lastTime = GetTickCount();
	static DWORD TimeCCadr = 0;

	while (msg.message != WM_QUIT && IsWindow(GData::Handle3D))
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			if (SSInput_GetKeyEvents(SIK_T) == InputEvents::iv_k_first)
			{
				for (int i = 0; i < 1; ++i)
				{
					ID tmpid = SPE_EffectIdOfKey(i);
					//SPE_EffectPosSet(tmpid, &float3(i, 0, k));
					SPE_EffectAlifeSet(tmpid, !SPE_EffectAlifeGet(tmpid));
				}
			}
			static DWORD TimeStart = 0;
			DWORD TimeThis = GetTickCount();

			DWORD currTime = GetTickCount();
			DWORD timeDelta = (currTime - lastTime);

			Core_0ConsoleUpdate();
#ifdef SX_GAME
			GData::ObjCamera = SXGame_GetActiveCamera();
#endif
			if (GetActiveWindow() == GData::Handle3D)
			{
				SGCore_LoadTexLoadTextures();
				SXRenderFunc::MainRender(timeDelta);
			}

			TimeCCadr = timeDelta;
			TimeStart = TimeThis;

			lastTime = currTime;
		}
	}

	/*
	SXGame_0Kill();
	SXPhysics_0Kill();
	SXAnim_0Kill();
	mem_release(GData::ObjCamera);
	SGeom_0CreateKill();
	SML_0Kill();
	SGCore_0Kill();
	Core_AKill();*/

	SkyXEngine_Kill();

	return msg.wParam;
}