
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "ml_data.h"

namespace mtrl_data
{
	void Init();
	IGXContext *pDXDevice = 0;


	float2_t vSizeTexReflection = float2_t(MTL_REF_TEX_SIZE, MTL_REF_TEX_SIZE);

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	float3 vOrientedCube[6] = { float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1) };
	float3 vUpVectorsCube[6] = { float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0) };

	float4x4 mRefProjPlane;
	float4x4 mRefProjCube;

	namespace shader_id
	{
		namespace vs
		{
			ID idResPosDepth;
			
			ID idScreenOut;

			ID idStdGeom;
			ID idStdTree;
			ID idStdGrass;
			ID idStdSkin;
		};

		namespace ps
		{
			ID idScreenOut;

			ID idStdGeom;
			ID idStdGeomCP;
			ID idStdGreen;
			ID idStdGreenCP;
			ID idStdSkin;
			ID idStdSkinCP;
		};
	};
};

void mtrl_data::Init()
{
	mtrl_data::pDXDevice = SGCore_GetDXDevice();

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	mtrl_data::shader_id::vs::idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);

	mtrl_data::shader_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	mtrl_data::shader_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);


	mtrl_data::shader_id::vs::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom.vs", SHADER_CHECKDOUBLE_NAME);
	mtrl_data::shader_id::ps::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom.ps", SHADER_CHECKDOUBLE_NAME);

	GXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	mtrl_data::shader_id::ps::idStdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	GXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	mtrl_data::shader_id::vs::idStdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	mtrl_data::shader_id::vs::idStdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree.vs", SHADER_CHECKDOUBLE_NAME);

	mtrl_data::shader_id::ps::idStdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green.ps", SHADER_CHECKDOUBLE_NAME);
	mtrl_data::shader_id::ps::idStdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);


	mtrl_data::shader_id::vs::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin.vs", SHADER_CHECKDOUBLE_NAME);
	mtrl_data::shader_id::ps::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin.ps", SHADER_CHECKDOUBLE_NAME);

	mtrl_data::shader_id::ps::idStdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	mtrl_data::mRefProjPlane = SMMatrixPerspectiveFovLH(*r_default_fov, float(*r_win_width) / float(*r_win_height), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
	mtrl_data::mRefProjCube = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
}