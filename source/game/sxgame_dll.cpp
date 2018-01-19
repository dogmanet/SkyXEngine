
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

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
#	pragma comment(lib, "sxscore_d.lib")
//#	pragma comment(lib, "sxpp_d.lib")
#	pragma comment(lib, "sxaigrid_d.lib")
#else
#	pragma comment(lib, "sxcore.lib")
#	pragma comment(lib, "sxgcore.lib")
#	pragma comment(lib, "sxinput.lib")
#	pragma comment(lib, "sxphysics.lib")
#	pragma comment(lib, "sxdecals.lib")
#	pragma comment(lib, "sxanim.lib")
#	pragma comment(lib, "sxparticles.lib")
#	pragma comment(lib, "sxscore.lib")
//#	pragma comment(lib, "sxpp.lib")
#	pragma comment(lib, "sxaigrid.lib")
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif


GameData * g_pGameData = NULL;
ID3DXMesh* g_pFigureBox = 0;

#define SG_PRECOND(ret) if(!g_pGameData){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgame is not init", GEN_MSG_LOCATION);return ret;}

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
	CPathCorner * pc = (CPathCorner*)CREATE_ENTITY("path_corner", GameData::m_pMgr);
	pc->setKV("name", buf);
	pc->setPos(GameData::m_pActiveCamera->getPos());
	pc->setOrient(GameData::m_pActiveCamera->getOrient());

	sprintf(buf, "cam_pt_%d", count - 1);
	pc = (CPathCorner*)GameData::m_pMgr->findEntityByName(buf);
	sprintf(buf, "cam_pt_%d", count);
	pc->setKV("next", buf);
	
	++count;
}

void ccmd_save_as(int argc, const char ** argv)
{
	if(argc > 1)
	{
		GameData::m_pMgr->exportList(argv[1]);
	}
}

SX_LIB_API ISXCamera * SXGame_GetActiveCamera()
{
	SG_PRECOND(NULL);
	return(GameData::m_pActiveCamera->getCamera());
}

SX_LIB_API void SXGame_0Create()
{
	if(g_pGameData)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgame double init", GEN_MSG_LOCATION);
		return;
	}
	Core_SetOutPtr();

	g_pGameData = new GameData();

	//g_pPlayer->spawn();
	D3DXCreateBox(SGCore_GetDXDevice(), 1, 1, 1, &g_pFigureBox, 0);

	Core_0RegisterConcmd("add_corner", ccmd_cam_pt);
	Core_0RegisterConcmdArg("ent_save", ccmd_save_as);
}
SX_LIB_API void SXGame_AKill()
{
	SG_PRECOND(_VOID);
	mem_delete(g_pGameData);
	mem_release(g_pFigureBox);
}

SX_LIB_API void SXGame_Update(int thread)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->update(thread);
	g_pGameData->update();
}
SX_LIB_API void SXGame_UpdateSetThreadNum(int num)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->setThreadNum(num);
}
SX_LIB_API void SXGame_Sync()
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->sync();
	g_pGameData->sync();
}

SX_LIB_API void SXGame_Render()
{
	SG_PRECOND(_VOID);
	//GameData::m_pMgr->render();
	g_pGameData->render();
}

SX_LIB_API void SXGame_RenderHUD()
{
	SG_PRECOND(_VOID);
	g_pGameData->renderHUD();
}


SX_LIB_API void SXGame_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SXGame_LoadEnts(const char * file)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->import(file);
}

SX_LIB_API void SXGame_UnloadObjLevel()
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->unloadObjLevel();
}

SX_LIB_API void SXGame_SaveEnts(const char * file)
{
	SG_PRECOND(_VOID);
	GameData::m_pMgr->exportList(file);
}

SX_LIB_API void SXGame_PlayerSpawn()
{
	SG_PRECOND(_VOID);
	GameData::m_pPlayer->spawn();
}

SX_LIB_API void SXGame_EditorRender(ID id, ID id_sel_tex)
{
	SG_PRECOND(_VOID);

	CBaseEntity* bEnt = SXGame_EntGet(id);
	if (!bEnt)
		return;

	SMMATRIX mView, mProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mProj);

	SGCore_GetDXDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
	SGCore_GetDXDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj);

	if (strcmp(bEnt->getClassName(), "path_corner") == 0)
	{
		CPathCorner * pStartPoint = (CPathCorner*)bEnt;
		if (!pStartPoint)
			return;

		/*CFuncTrain * pTrain = (CFuncTrain*)(GameData::m_pMgr->findEntityByClass("func_train"));
		if (pTrain)
		{
			pTrain->start();
		}

		CPointCamera * cam = (CPointCamera*)(GameData::m_pMgr->findEntityByName("train_camera"));
		if (cam)
		{
			GameData::m_pActiveCamera = cam;
			cam->setParent();
		}*/

		CPathCorner * pCur = pStartPoint;
		while ((pCur = pCur->GetPrev()))
		{
			pStartPoint = pCur;
		}

		float len = 0;
		int count = 0;
		pCur = pStartPoint;
		do
		{
			len += pCur->GetLength();
			++count;
			SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&SMMatrixTranslation(pCur->getPos()));

			if (id == pCur->getId())
				SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(id_sel_tex));
			else
				SGCore_GetDXDevice()->SetTexture(0, 0);

			g_pFigureBox->DrawSubset(0);
		} while ((pCur = pCur->GetNext()));


		int npoints = count * 10;
		static Array<float3_t> pts;

		if (pts.size() <= npoints)
		{
			pts.clear();
			pts.resize(npoints*2);
		}
		
		for (int i = 0; i < npoints; ++i)
		{
			pts[i * 2] = pStartPoint->getPoint((float)i * (len / (float)npoints));
		}
		for (int i = 0; i < npoints - 1; ++i)
		{
			pts[i * 2 + 1] = pts[(i + 1) * 2];
		}
		pts[npoints * 2 - 1] = pStartPoint->getPoint(len);

		//pts[npoints * 2] = pTrain->getPos();
		//pts[npoints * 2 + 1] = (float3)(pTrain->getPos() + (pTrain->getOrient() * float3(0.0f, 1.0f, 0.0f)));

		SGCore_ShaderUnBind();

		SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&SMMatrixIdentity());

		SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ);
		SGCore_GetDXDevice()->SetTexture(0, 0);
		SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, npoints, &(pts[0]), sizeof(float3_t));
	}
	else
	{
		float3 min, max;
		bEnt->getMinMax(&min, &max);

		if (min.x == 0.0f && min.y == 0.0f && min.z == 0.0f && max.x == 0.0f && max.y == 0.0f && max.z == 0.0f)
			max.x = max.y = max.z = 0.5f;
		SMMATRIX mView, mProj;
		Core_RMatrixGet(G_RI_MATRIX_VIEW, &mView);
		Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mProj);

		SGCore_GetDXDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
		SGCore_GetDXDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj);
		SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixScaling((max.x - min.x), (max.y - min.y), (max.z - min.z)) * SMMatrixTranslation(bEnt->getPos())));
		//SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&bEnt->getWorldTM());

		SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(id_sel_tex));
		g_pFigureBox->DrawSubset(0);
	}
}

SX_LIB_API int SXGame_EntGetClassListCount()
{
	SG_PRECOND(-1);

	return(CEntityFactoryMap::GetInstance()->getListCount());
}

SX_LIB_API void SXGame_EntGetClassList(const char ** pszClassList, int count)
{
	SG_PRECOND(_VOID);

	CEntityFactoryMap::GetInstance()->getListing(pszClassList, count);
}

SX_LIB_API CBaseEntity * SXGame_CreateEntity(const char * classname)
{
	SG_PRECOND(NULL);

	return(CREATE_ENTITY(classname, GameData::m_pMgr));
}

SX_LIB_API void SXGame_RemoveEntity(CBaseEntity * pEnt)
{
	SG_PRECOND(_VOID);

	REMOVE_ENTITY(pEnt);
}

SX_LIB_API proptable_t * SXGame_EntGetProptable(const char * classname)
{
	SG_PRECOND(NULL);

	return(CEntityFactoryMap::GetInstance()->getPropTable(classname));
}

SX_LIB_API const char * SXGame_EntGetDefault(const char * classname, const char * key)
{
	SG_PRECOND(NULL);

	EntDefaultsMap * pMap = CEntityFactoryMap::GetInstance()->getDefaults(classname);
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

SX_LIB_API int SXGame_EntGetCount()
{
	SG_PRECOND(0);
	return(GameData::m_pMgr->getCount());
}

SX_LIB_API CBaseEntity *SXGame_EntGet(ID id)
{
	SG_PRECOND(NULL);
	return(GameData::m_pMgr->getById(id));
}

SX_LIB_API CBaseEntity *SXGame_EntGetByName(const char *szName, ID idStart)
{
	SG_PRECOND(NULL);

	CBaseEntity *pEnt = 0;

	for (int i = 0, il = SXGame_EntGetCount(); i < il; ++i)
	{
		pEnt = SXGame_EntGet(i);
		if (pEnt && strcmp(szName, pEnt->getName()) == 0)
		{
			return pEnt;
		}
	}

	return 0;
}
