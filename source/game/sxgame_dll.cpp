
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sxgame.h"

#include <core/sxcore.h>
#include <gcore/sxgcore.h>

#include "EntityManager.h"

#include "PathCorner.h"
#include "FuncTrain.h"

#include "GameData.h"

#if defined(_DEBUG)
#	pragma comment(lib, "sxcore_d.lib")
#	pragma comment(lib, "sxgcore_d.lib")
#	pragma comment(lib, "sxinput_d.lib")
#	pragma comment(lib, "sxphysics_d.lib")
#	pragma comment(lib, "sxdecals_d.lib")
#	pragma comment(lib, "sxanim_d.lib")
#	pragma comment(lib, "sxparticles_d.lib")
#else
#	pragma comment(lib, "sxcore.lib")
#	pragma comment(lib, "sxgcore.lib")
#	pragma comment(lib, "sxinput.lib")
#	pragma comment(lib, "sxphysics.lib")
#	pragma comment(lib, "sxdecals.lib")
#	pragma comment(lib, "sxparticles.lib")
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

GameData * g_pGameData = NULL;

#define SG_PRECOND(ret) if(!g_pGameData){reportf(-1, "%s - sxgame is not init", gen_msg_location);return ret;}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


void ccmd_cam_pt()
{
	static int count = 0;
	char buf[64];
	sprintf(buf, "cam_pt_%d", count);
	PathCorner * pc = (PathCorner*)CREATE_ENTITY("path_corner", GameData::m_pMgr);
	pc->SetKV("name", buf);
	pc->SetPos(GameData::m_pActiveCamera->GetPos());
	pc->SetOrient(GameData::m_pActiveCamera->GetOrient());

	sprintf(buf, "cam_pt_%d", count - 1);
	pc = (PathCorner*)GameData::m_pMgr->FindEntityByName(buf);
	sprintf(buf, "cam_pt_%d", count);
	pc->SetKV("next", buf);
	
	++count;
}

void ccmd_save_as(int argc, const char ** argv)
{
	if(argc > 1)
	{
		GameData::m_pMgr->Export(argv[1]);
	}
}

SX_LIB_API ISXCamera * SXGame_GetActiveCamera()
{
	SG_PRECOND(NULL);
	return(GameData::m_pActiveCamera->GetCamera());
}

SX_LIB_API void SXGame_0Create()
{
	if(g_pGameData)
	{
		reportf(-1, "%s - sxgame double init", gen_msg_location);
		return;
	}
	Core_SetOutPtr();

	g_pGameData = new GameData();

	//g_pPlayer->Spawn();


	Core_0RegisterConcmd("add_corner", ccmd_cam_pt);
	Core_0RegisterConcmdArg("ent_save", ccmd_save_as);
}
SX_LIB_API void SXGame_0Kill()
{
	SG_PRECOND(_VOID);
	mem_delete(g_pGameData);
}

SX_LIB_API void SXGame_Update(int thread)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->Update(thread);
}
SX_LIB_API void SXGame_UpdateSetThreadNum(int num)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->SetThreadNum(num);
}
SX_LIB_API void SXGame_Sync()
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->Sync();
}

SX_LIB_API void SXGame_Dbg_Set(report_func rf)
{
	reportf = rf;
}

SX_LIB_API void SXGame_LoadEnts(const char * file)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->Import(file);
}

SX_LIB_API void SXGame_SaveEnts(const char * file)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->Export(file);
}

SX_LIB_API void SXGame_PlayerSpawn()
{
	SG_PRECOND(_VOID);
	GameData::m_pPlayer->Spawn();
}

SX_LIB_API void SXGame_EditorRender()
{
	SG_PRECOND(_VOID);
	
	PathCorner * pStartPoint = (PathCorner*)(GameData::m_pMgr->FindEntityByName("cam_pt_0"));
	if(!pStartPoint)
	{
		return;
	}
	FuncTrain * pTrain = (FuncTrain*)(GameData::m_pMgr->FindEntityByClass("func_train"));
	if(!pTrain)
	{
		return;
	}
	SXpointCamera * cam = (SXpointCamera*)(GameData::m_pMgr->FindEntityByName("train_camera"));
	if(cam)
	{
		GameData::m_pActiveCamera = cam;
	}
	pTrain->Start();

	//pStartPoint = pStartPoint->GetNext();

	float len = 0;
	PathCorner * pCur = pStartPoint;
	do
	{
		len += pCur->GetLength();
	}
	while((pCur = pCur->GetNext())/* && false*/);


	const int npoints = 60;
	float3_t pts[npoints * 2 + 2];

	for(int i = 0; i < npoints; ++i)
	{
		pts[i * 2] = pStartPoint->GetPoint((float)i * (len / (float)npoints));
	}
	for(int i = 0; i < npoints - 1; ++i)
	{
		pts[i * 2 + 1] = pts[(i + 1) * 2];
	}
	pts[npoints * 2 - 1] = pStartPoint->GetPoint(len);

	pts[npoints * 2] = pTrain->GetPos();
	pts[npoints * 2 + 1] = (float3)(pTrain->GetPos() + (pTrain->GetOrient() * float3(0.0f, 1.0f, 0.0f)));

	SGCore_ShaderUnBind();

	SMMATRIX mView, mProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mProj);

	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&SMMatrixIdentity());
	SGCore_GetDXDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
	SGCore_GetDXDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj);

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFF);

	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ);

	SGCore_GetDXDevice()->SetTexture(0, 0);

	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, npoints + 1, &pts, sizeof(float3_t));

	
}

SX_LIB_API int SXGame_EntGetClassListCount()
{
	SG_PRECOND(-1);

	return(EntityFactoryMap::GetInstance()->GetListCount());
}

SX_LIB_API void SXGame_EntGetClassList(const char ** pszClassList, int count)
{
	SG_PRECOND(_VOID);

	EntityFactoryMap::GetInstance()->GetListing(pszClassList, count);
}

SX_LIB_API SXbaseEntity * SXGame_CreateEntity(const char * classname)
{
	SG_PRECOND(NULL);

	return(CREATE_ENTITY(classname, GameData::m_pMgr));
}

SX_LIB_API void SXGame_RemoveEntity(SXbaseEntity * pEnt)
{
	SG_PRECOND(_VOID);

	REMOVE_ENTITY(pEnt);
}

SX_LIB_API proptable_t * SXGame_EntGetProptable(const char * classname)
{
	SG_PRECOND(NULL);

	return(EntityFactoryMap::GetInstance()->GetPropTable(classname));
}

SX_LIB_API const char * SXGame_EntGetDefault(const char * classname, const char * key)
{
	SG_PRECOND(NULL);

	EntDefaultsMap * pMap = EntityFactoryMap::GetInstance()->GetDefaults(classname);
	if(!pMap)
	{
		return(NULL);
	}

	const EntDefaultsMap::Node * pNode;
	if(!pMap->KeyExists(AAString(key), &pNode))
	{
		return(NULL);
	}
	return(*pNode->Val);
}
