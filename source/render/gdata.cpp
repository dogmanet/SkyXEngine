
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "gdata.h"

namespace GData
{
	IDirect3DDevice9* DXDevice = 0;

	HWND HandleParent3D = 0;
	HWND Handle3D = 0;	

	//bool IsWindowed = true;


	DS_RT FinalImage = DS_RT_SCENELIGHT;
	ICamera* ObjCamera = 0;
	ID IDSelectTex = -1;
	
	float2_t NearFar = float2_t(0.025, 400);
	float ProjFov = SM_PIDIV4;	

	ID DefaultGeomIDArr = -1;
	ID DefaultGreenIDArr = -1;
	ID DefaultAnimIDArr = -1;

	float3 ConstCurrCamPos;
	float3 ConstCurrCamDir;

	float4_t CamWalkParamEditor = float4_t(
		10.f,	//!< простое движенеи вперед
		5.f,	//!< коэфициент ускорения
		0.7f,	//!< коэфициент от основного движения в стороны 
		0.5f	//!< коэфициент от основного движения назад
		);

	//матрицы
	void InitAllMatrix();
	float4x4 MCamView;
	float4x4 MCamProj;
	float4x4 MLightProj;
	float4x4 MRefPlaneSkyProj;
	float4x4 MRefCubeSkyProj;

	//**********************************************************************

	//! пространство имен с идентификаторами шейдеров
	namespace IDsShaders
	{
		//! загрузка всех необходимых шейдеров
		void InitAllShaders();

		//! вершинные шейдеры
		namespace VS
		{
			 ID ScreenOut;
			 ID ResPos;
		};

		//! пиксельные шейдеры
		namespace PS
		{
			 ID ScreenOut;
			 ID ComLightingNonShadow;
			 ID ComLightingShadow;

			 ID BlendAmbientSpecDiffColor;

			//ID ToneMapping;

			 ID StencilStr;
			 ID StencilColumn;
			 ID StencilStrColumn;
			 ID UnionAlpha;
		};
	};

	namespace Editors
	{
		bool MoveCamera = true;
		ModelSim* SimModel = 0;
		CGrid* ObjGrid = 0;
		CAxesStatic* ObjAxesStatic = 0;

		bool RenderGrid = false;
		bool RenderAxesStatic = false;
	}
};

//##########################################################################

void GData::InitAllMatrix()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static float2_t vWinSize;
	vWinSize = float2(*r_win_width, *r_win_height);

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");
	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	GData::NearFar.x = *r_near;
	GData::NearFar.y = *r_far;
	GData::ProjFov = *r_default_fov;

	GData::MCamProj = SMMatrixPerspectiveFovLH(GData::ProjFov, vWinSize.x / vWinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(GData::ProjFov, vWinSize.x / vWinSize.y, GData::NearFar.x, G_DATA_LIGHT_FAR);

	GData::MRefPlaneSkyProj = SMMatrixPerspectiveFovLH(GData::ProjFov, vWinSize.x / vWinSize.y, GData::NearFar.x, G_DATA_LIGHT_FAR);
	GData::MRefCubeSkyProj = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, GData::NearFar.x, G_DATA_LIGHT_FAR);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &GData::MCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &GData::MLightProj);

	/*Core_RFloatSet(G_RI_FLOAT_OBSERVER_NEAR, GData::NearFar.x);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FAR, GData::NearFar.y);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FOV, GData::ProjFov);*/
}

void GData::IDsShaders::InitAllShaders()
{
	GData::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs",SHADER_CHECKDOUBLE_PATH);
	GData::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps",SHADER_CHECKDOUBLE_PATH);

	//GData::IDsShaders::PS::ToneMapping = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_tone_mapping.ps", "lighting_tone_mapping.ps",SHADER_CHECKDOUBLE_PATH);

	GData::IDsShaders::VS::ResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs",SHADER_CHECKDOUBLE_PATH);


	GData::IDsShaders::PS::ComLightingNonShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_nonshadow.ps", SHADER_CHECKDOUBLE_NAME);
	D3DXMACRO Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	GData::IDsShaders::PS::ComLightingShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_shadow.ps", SHADER_CHECKDOUBLE_NAME, Defines_IS_SHADOWED);
	GData::IDsShaders::PS::BlendAmbientSpecDiffColor = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_blend.ps", "lighting_blend.ps",SHADER_CHECKDOUBLE_PATH);

	GData::IDsShaders::PS::UnionAlpha = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_union_alpha.ps", "pp_union_alpha.ps",SHADER_CHECKDOUBLE_PATH);

	D3DXMACRO Defines_STR[] = { { "_STR_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilStr = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str.ps", SHADER_CHECKDOUBLE_NAME, Defines_STR);
	D3DXMACRO Defines_COLUMN[] = { { "_COLUMN_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_column.ps", SHADER_CHECKDOUBLE_NAME, Defines_COLUMN);
	D3DXMACRO Defines_COLUMN_STR[] = { { "_COLUMN_STR_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilStrColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str_column.ps", SHADER_CHECKDOUBLE_NAME, Defines_COLUMN_STR);
}
