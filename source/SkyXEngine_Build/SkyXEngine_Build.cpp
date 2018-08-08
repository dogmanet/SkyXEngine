
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <SkyXEngine.h>
//#include <common/string_func.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SkyXEngine_Init();
	SkyXEngine_PreviewKill();

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyCloudsSetWidthHeightPos(1000, 1000, &float3(0, 0, 0));

	SLevel_Load("stalker_atp", true);

	SGeom_0SettGreenSetFreqGrass(30);

	/*NPCBase* npc = new NPCBase();
	npc->setModel("models/stalker_zombi/stalker_zombi_a.dse");
	npc->setPos(&float3(0, 0, 0));*/

	/*for (int i = 0; i < 10; ++i)
	{
		CBaseEntity* bEnt = SXGame_CreateEntity("npc_zombie");
		bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);
		char pos[256];
		//sprintf(pos, "%f %f %f", randf(-0.1, 0.1), randf(-0.1, 0.1), randf(-0.1, 0.1));
		//bEnt->setKV("origin", "14.90 0.36 25.56");
		bEnt->setKV("origin", "0 0 0");
	}*/

	/*for (int i = 0; i < 100; ++i)
	{
		CBaseEntity* bEnt = SXGame_CreateEntity("npc_zombie");
		bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);
		bEnt->setKV("origin", "0 1 0");
	}*/
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

	//SGCore_LoadTexLoadTextures();

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

	SLevel_AmbientSndPlay();
	SLevel_WeatherSndPlay();

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}