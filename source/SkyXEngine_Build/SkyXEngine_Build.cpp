
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
	SkyXEngine_Init(0, 0, lpCmdLine);
	SkyXEngine_PreviewKill();

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	//SGCore_SkyCloudsSetWidthHeightPos(1000, 1000, &float3(0, 0, 0));

	//SGCore_OC_SetEnable(false);

	//SLevel_Load("stalker_atp", true);
	//SLevel_Load("sga", true);

	SGreen_0SettSetFreqGrass(100);

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

	

	//SGeom_ModelsAddModel("tt.dse", "tt.dse", 0, false);
	/*SGeom_ModelsAddModel("stalker_atp.dse", "stalker_atp.dse", "stalker_atp.dse", "stalker_atp.dse", true);
	SGeom_ModelsMsetScale(0, &float3(0.1, 0.1, 0.1));
	SGeom_ModelsMsetRotation(0, &float3(0, 1, 0));
	SGeom_ModelsMsetPosition(0, &float3(10, -11, 10));

	ID idModel = SGeom_ModelsCopy(0);
	SGeom_ModelsMsetScale(idModel, &float3(0.1, 0.1, 0.1));
	SGeom_ModelsMsetRotation(idModel, &float3(0, 1, 0));
	SGeom_ModelsMsetPosition(idModel, &float3(50, 11, 50));
	//SGeom_ModelsReComBound(0);
	ID idGlobal = SLight_CreatePoint(
		&float3(60, 60, 0),
		0,
		&float3(1, 1, 1),
		true,
		true);

	//SGeom_ModelsDelModel(0);

	SXPhysics_LoadGeom();
	//SXGame_OnLevelLoad("stalker_atp");
	Core_0ConsoleExecCmd("ent_load_level \"%s\" \"%s\"", "D:\\project\\engine\\build\\gamesource\\levels\\stalker_atp\\stalker_atp.ent", "stalker_atp");
	SLevel_Save("new_test");

	SLight_SetEnable(idGlobal, true);

	//SLevel_Load("new_test", false);*/

	SGCore_ShaderAllLoad();
	SGCore_LoadTexAllLoad();

	SetWindowPos(SGCore_GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	SLevel_AmbientSndPlay();
	SLevel_WeatherSndPlay();

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}