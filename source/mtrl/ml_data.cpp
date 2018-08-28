
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "ml_data.h"

namespace MLSet
{
	void MLInit();
	IDirect3DDevice9* DXDevice = 0;


	float2_t SizeTexReflection = float2_t(MTL_REF_TEX_SIZE, MTL_REF_TEX_SIZE);

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	float3 OrientedCube[6] = { float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1) };
	float3 UpVectorsCube[6] = { float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0) };

	float4x4 RefMProjPlane;
	float4x4 RefMProjCube;

	namespace IDsShaders
	{
		namespace VS
		{
			ID ResPosDepth;
			
			ID ScreenOut;

			ID StdGeom;
			ID StdTree;
			ID StdGrass;
			ID StdSkin;
		};

		namespace PS
		{
			ID ScreenOut;

			ID StdGeom;
			ID StdGeomCP;
			ID StdGreen;
			ID StdGreenCP;
			ID StdSkin;
			ID StdSkinCP;
		};
	};

	namespace IDsTexs
	{
		ID Tex_NoiseTex;
	};
};

void MLSet::MLInit()
{
	MLSet::DXDevice = SGCore_GetDXDevice();

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	//MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexAddName("noise_rottex.dds", LOAD_TEXTURE_TYPE_CONST);

	D3DLOCKED_RECT LockedRect;
	uint32_t* tmpOldColor;
	IDirect3DTexture9 *pRnsSampler;
	MLSet::DXDevice->CreateTexture(4, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRnsSampler, NULL);
	
	uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

	for (int i = 0; i < 16; ++i)
		aRndColors[i] = D3DCOLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

	pRnsSampler->LockRect(0, &LockedRect, 0, 0);
	memcpy(LockedRect.pBits, aRndColors, sizeof(uint32_t));
	pRnsSampler->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexCreate("noise_rottex__", pRnsSampler);
	

	MLSet::IDsShaders::VS::ResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);

	MLSet::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);


	MLSet::IDsShaders::VS::StdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom.vs", SHADER_CHECKDOUBLE_NAME);
	MLSet::IDsShaders::PS::StdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::StdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	MLSet::IDsShaders::VS::StdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	MLSet::IDsShaders::VS::StdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree.vs", SHADER_CHECKDOUBLE_NAME);

	MLSet::IDsShaders::PS::StdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green.ps", SHADER_CHECKDOUBLE_NAME);
	MLSet::IDsShaders::PS::StdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);


	MLSet::IDsShaders::VS::StdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin.vs", SHADER_CHECKDOUBLE_NAME);
	MLSet::IDsShaders::PS::StdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin.ps", SHADER_CHECKDOUBLE_NAME);

	MLSet::IDsShaders::PS::StdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);


	

	MLSet::RefMProjPlane = SMMatrixPerspectiveFovLH(*r_default_fov, float(*r_win_width) / float(*r_win_height), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
	MLSet::RefMProjCube = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
}