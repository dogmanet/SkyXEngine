
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Главный файл редактора материалов material_editor
*/

#define SX_MATERIAL_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 

#include <SkyXEngine.h>
#include "material_editor.h"
#include "models.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SXGUIinit();
	srand((unsigned int)time(0));

	material_editor::InitAllElements();
	material_editor::Nulling();
	

	SkyXEngine_Init(material_editor::pWindowRender->getHWND(), material_editor::pJobWindow->getHWND());
	static int *r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int *r_win_height = (int*)GET_PCVAR_INT("r_win_height");
	SkyXEngine_RunGenPreview();
	Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);

	material_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
	material_editor::pCheckBoxTBRLighting->setCheck(true);

	material_editor::pCheckBoxTBCam->setCheck(true);
	SRender_EditorCameraSetMove(material_editor::pCheckBoxTBCam->getCheck());

	char shaderskitpath[1024];
	sprintf(shaderskitpath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_EXE), "\\shaders_kit.cfg");
	material_editor::pShaders = new CShadersKit();
	material_editor::pShaders->load(shaderskitpath);

	for (int i = 0; i < material_editor::pShaders->getCount(); ++i)
	{
		material_editor::pComboBoxShaders->addItem(material_editor::pShaders->getName(i));
	}

	material_editor::pComboBoxShaders->setSel(0);


	char paramlpath[1024];
	sprintf(paramlpath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_EXE), "\\paraml_kit.cfg");
	material_editor::pParamL = new CParamLigthKit();
	material_editor::pParamL->load(paramlpath);

	for (int i = 0; i < material_editor::pParamL->getCount(); ++i)
	{
		material_editor::pComboBoxParamL->addItem(material_editor::pParamL->getName(i));
	}

	material_editor::pComboBoxParamL->setSel(0);


	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	material_editor::pEditSkyBox->setText("sky_2_cube.dds");
		
	SRender_GetCamera()->setPosition(&float3(0, 0, -150));
	
	//этот код не надо удалять, при помощи него вывел данные буферов модели в текстовый файл и таким образом избавились от внешних моделей для редактора
	/*
	FILE *pFile = 0;
	pFile = fopen("D:/project/engine/source/sxmaterialeditor/models.cpp", "w");

	char szFullPath[1024];
	ISXDataStaticModel *pStaticModel = 0;
	sprintf(szFullPath, "%sme/sphere.dse", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));
	SGCore_StaticModelLoad(szFullPath, &pStaticModel);

	vertex_static_ex *pVertex;
	pStaticModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
	UINT *pIndex;
	pStaticModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);

	fprintf(pFile, "vertex_static_ex g_aModelsSphereVertex[] = {\n");
	for (int i = 0; i < pStaticModel->m_uiAllVertexCount; ++i)
	{
		fprintf(pFile, "\t{float3_t(%f, %f, %f), float2_t(%f, %f), float3_t(%f, %f, %f), float3_t(%f, %f, %f), float3_t(%f, %f, %f)}",
			pVertex[i].Pos.x, pVertex[i].Pos.y, pVertex[i].Pos.z, 
			pVertex[i].Tex.x, pVertex[i].Tex.y,
			pVertex[i].Norm.x, pVertex[i].Norm.y, pVertex[i].Norm.z,
			pVertex[i].Tangent.x, pVertex[i].Tangent.y, pVertex[i].Tangent.z,
			pVertex[i].Binorm.x, pVertex[i].Binorm.y, pVertex[i].Binorm.z);

		if (pStaticModel->m_uiAllVertexCount > i)
			fprintf(pFile, ",");

		fprintf(pFile, "\n");
	}
	fprintf(pFile, "};\n\n");

	fprintf(pFile, "UINT g_aModelsSphereIndex[] = {\n");
	for (int i = 0; i < pStaticModel->m_uiAllIndexCount; i+=3)
	{
		fprintf(pFile, "\t%d, %d, %d, \n", pIndex[i], pIndex[i+1], pIndex[i+2]);
	}
	fprintf(pFile, "};\n\n");

	pStaticModel->m_pIndexBuffer->Unlock();
	pStaticModel->m_pVertexBuffer->Unlock();

	fclose(pFile);
	*/

	SRender_SimModelAddModel(CreateModelSphere());
	SRender_SimModelAddModel(CreateModelCube());
	SRender_SimModelAddModel(CreateModelPlane());

	SLight_CreatePoint(
		&float3(110,110,-110),
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		true);
	SLight_SetEnable(SLight_GetCount() - 1, true);
	SLight_SetName(SLight_GetCount() - 1, "sun");

	material_editor::pEditLigthColorR->setText("1");
	material_editor::pEditLigthColorG->setText("1");
	material_editor::pEditLigthColorB->setText("1");
	material_editor::pStaticLigthColor->setColorBrush(RGB(255, 255, 255));


	SGCore_LoadTexAllLoad();

	material_editor::InitMtl(SRender_SimModelGetIDMtl());
	material_editor::idMat = SRender_SimModelGetIDMtl();

	SkyXEngine_PreviewKill();

	material_editor::pJobWindow->setVisible(true);
	SetWindowPos(material_editor::pJobWindow->getHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	
	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	material_editor::DeleteAllElements();
	return result;
}