
#include "gdata.h"

namespace GData
{
	 IDirect3DDevice9* DXDevice = 0;	//!< dx устройство

	 HWND HandleParent3D = 0;//!< хэндл окна родителя окна рендера, на случай редакторов
	 HWND Handle3D = 0;		//!< хэндл окна рендера

	// float2_t WinSize = float2_t(800, 600);	//!< размер окна рендера (области рендера)
	 bool IsWindowed = true;					//!<использовать ли оконный режим рендера?


	 DS_RT FinalImage = DS_RT_SCENELIGHT;//!< финальное изображение
	 ISXCamera* ObjCamera = 0;	//!< камера для которой будет рендер
	 ID IDSelectTex = -1;
	
	 float2_t NearFar = float2_t(0.025, 400);	//!< значение дальней и ближней плоскостей отсечения
	 float ProjFov = SM_PIDIV4;				//!< fov камеры

	 ID DefaultGeomIDArr = -1;
	 ID DefaultGreenIDArr = -1;
	 ID DefaultAnimIDArr = -1;

	 float3 ConstCurrCamPos;	//!< позиция камеры
	 float3 ConstCurrCamDir;	//!< направление взгляда камеры

	//! Параметры перемещения камеры по умолчанию
	 float4_t CamWalkParamEditor = float4_t(
		10.f,	//!< простое движенеи вперед
		5.f,	//!< коэфициент ускорения
		0.7f,	//!< коэфициент от основного движения в стороны 
		0.5f	//!< коэфициент от основного движения назад
		);

	//матрицы
	void InitAllMatrix();		//!< инициализация матриц
	 float4x4 MCamView;			//!< матрица вида камеры
	 float4x4 MCamProj;			//!< матрица проекции камеры
	 float4x4 MLightProj;		//!< матрица проекции аналогична¤ камере, только дальна¤¤ плоскость дальше
	 float4x4 MRefPlaneSkyProj;	//!< матрица проекции дл¤ рендера skybox и sky clouds, дл¤ плоских отражений, аналогична MCamProj
	 float4x4 MRefCubeSkyProj;	//!< матрица проекции дл¤ рендера skybox и sky clouds, дл¤ куибческих отражений

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

			 ID ToneMapping;

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
		Grid* ObjGrid = 0;
		AxesStatic* ObjAxesStatic = 0;
		/*ID3DXMesh* FigureBox = 0;
		float3_t vFigureBoxParam;
		ID3DXMesh* FigureSphere = 0;
		float3_t vFigureSphereParam;
		ID3DXMesh* FigureCone = 0;
		float3_t vFigureConeParam;
		float3 vFigurePos;*/

		bool RenderGrid = false;
		bool RenderAxesStatic = false;
		/*bool RenderBoundBox = false;
		bool RenderBoundSphere = false;
		bool RenderBoundCone = false;*/
	}
};



void GData::InitAllMatrix()
{
	static const int *winr_width = GET_PCVAR_INT("winr_width");
	static const int *winr_height = GET_PCVAR_INT("winr_height");
	static float2_t vWinSize;
	vWinSize = float2(*winr_width, *winr_height);

	static const float *p_near = GET_PCVAR_FLOAT("p_near");
	static const float *p_far = GET_PCVAR_FLOAT("p_far");
	static const float *cam_fov = GET_PCVAR_FLOAT("cam_fov");

	GData::NearFar.x = *p_near;
	GData::NearFar.y = *p_far;
	GData::ProjFov = *cam_fov;

	GData::MCamProj = SMMatrixPerspectiveFovLH(GData::ProjFov, vWinSize.x / vWinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(GData::ProjFov, vWinSize.x / vWinSize.y, GData::NearFar.x, G_DATA_LIGHT_FAR);

	GData::MRefPlaneSkyProj = SMMatrixPerspectiveFovLH(GData::ProjFov, vWinSize.x / vWinSize.y, GData::NearFar.x, G_DATA_LIGHT_FAR);
	GData::MRefCubeSkyProj = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, GData::NearFar.x, G_DATA_LIGHT_FAR);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &GData::MCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &GData::MLightProj);

	Core_RFloatSet(G_RI_FLOAT_OBSERVER_NEAR, GData::NearFar.x);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FAR, GData::NearFar.y);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FOV, GData::ProjFov);
}
/*
void GData::Pathes::InitAllPathes()
{
	char tmppath[1024];
	GetModuleFileName(NULL, tmppath, 1024);
	int len = strlen(tmppath);
	while (tmppath[len--] != '\\')
	{
		if (tmppath[len - 1] == '\\')
		{
			len--;
			memcpy(GData::Pathes::ForExe, tmppath, len);
			GData::Pathes::ForExe[len] = 0;
		}
	}

	sprintf(GData::Pathes::WorkingTex, "%s%s", GData::Pathes::ForExe, "\\worktex\\");
	sprintf(GData::Pathes::GameSource, "%s%s", GData::Pathes::ForExe, "\\gamesource\\");
	sprintf(GData::Pathes::Screenshots, "%s%s", GData::Pathes::ForExe, "\\screenshots\\");
	sprintf(GData::Pathes::Config, "%s%s", GData::Pathes::GameSource, "config\\");
	sprintf(GData::Pathes::ConfigWeather, "%s%s", GData::Pathes::Config, "weather\\");
	sprintf(GData::Pathes::Levels, "%s%s", GData::Pathes::GameSource, "levels\\");
	sprintf(GData::Pathes::Meshes, "%s%s", GData::Pathes::GameSource, "meshes\\");
	sprintf(GData::Pathes::Models, "%s%s", GData::Pathes::GameSource, "models\\");
	sprintf(GData::Pathes::Shaders, "%s%s", GData::Pathes::GameSource, "shaders\\");
	sprintf(GData::Pathes::Sounds, "%s%s", GData::Pathes::GameSource, "sounds\\");
	sprintf(GData::Pathes::Scripts, "%s%s", GData::Pathes::GameSource, "scripts\\");

	sprintf(GData::Pathes::Textures, "%s%s", GData::Pathes::GameSource, "textures\\");
	sprintf(GData::Pathes::TexturesDetail, "%s%s", GData::Pathes::GameSource, "textures\\detail\\");
	sprintf(GData::Pathes::TexturesSkyBoxes, "%s%s", GData::Pathes::Textures, "sky\\");
	sprintf(GData::Pathes::Materials, "%s%s", GData::Pathes::GameSource, "materials\\");
	sprintf(GData::Pathes::GUIresources, "%s%s", GData::Pathes::GameSource, "resource\\");

	SetCurrentDirectoryA(GData::Pathes::GameSource);
}*/

void GData::IDsShaders::InitAllShaders()
{
	GData::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render",SHADER_CHECKDOUBLE_PATH);
	GData::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render",SHADER_CHECKDOUBLE_PATH);

	GData::IDsShaders::PS::ToneMapping = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_tone_mapping.ps", "lighting_tone_mapping",SHADER_CHECKDOUBLE_PATH);

	GData::IDsShaders::VS::ResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos",SHADER_CHECKDOUBLE_PATH);


	GData::IDsShaders::PS::ComLightingNonShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_nonshadow", SHADER_CHECKDOUBLE_NAME);
	D3DXMACRO Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	GData::IDsShaders::PS::ComLightingShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_shadow", SHADER_CHECKDOUBLE_NAME, Defines_IS_SHADOWED);
	GData::IDsShaders::PS::BlendAmbientSpecDiffColor = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_blend.ps", "lighting_blend",SHADER_CHECKDOUBLE_PATH);

	GData::IDsShaders::PS::UnionAlpha = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_union_alpha.ps", "pp_union_alpha",SHADER_CHECKDOUBLE_PATH);

	D3DXMACRO Defines_STR[] = { { "_STR_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilStr = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str", SHADER_CHECKDOUBLE_NAME, Defines_STR);
	D3DXMACRO Defines_COLUMN[] = { { "_COLUMN_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_column", SHADER_CHECKDOUBLE_NAME, Defines_COLUMN);
	D3DXMACRO Defines_COLUMN_STR[] = { { "_COLUMN_STR_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilStrColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str_column", SHADER_CHECKDOUBLE_NAME, Defines_COLUMN_STR);
}


#if defined(SX_PARTICLES_EDITOR)
void GData::Editors::ParticlesEditorUpdate()
{
	bool whyplay = false;
	for (int i = 0; i < SPE_EffectCountGet(); ++i)
	{
		if (SPE_EffectEnableGet(SPE_EffectIdOfKey(i)))
		{
			SXParticlesEditor::CheckBoxTBPlay->SetCheck(true);
			SXParticlesEditor::CheckBoxTBPause->SetCheck(false);
			SXParticlesEditor::CheckBoxTBStop->SetCheck(false);
			whyplay = true;
			break;
		}
	}

	if(!whyplay)
	{
		SXParticlesEditor::CheckBoxTBPlay->SetCheck(false);
		SXParticlesEditor::CheckBoxTBPause->SetCheck(false);
		SXParticlesEditor::CheckBoxTBStop->SetCheck(true);
	}

	int emitters_count = 0;
	int emitters_all_count = 0;
	int particles_life_count = 0;
	int particles_all_count = 0;
	//ID effid = -1;
	/*for (int i = 0; i < SPE_EffectCountGet(); ++i)
	{
		effid = SPE_EffectIdOfKey(i);
		if (SPE_EffectEnableGet(effid))
		{
			for (int k = 0; k < SPE_EmitterSCountGet(effid); ++k)
			{
				if (SPE_EmitterEnableGet(effid, k))
					++emitters_count;

				particles_all_count += SPE_EmitterCountGet(effid, k);
				particles_life_count += SPE_EmitterCountLifeGet(effid, k);
			}
			break;
		}
		effid = -1;
	}*/

	if (SXParticlesEditor::SelEffID >= 0)
	{
		emitters_all_count = SPE_EmitterSCountGet(SXParticlesEditor::SelEffID);
		if (SXParticlesEditor::SelEmitterID < 0)
		{
			for (int k = 0; k < SPE_EmitterSCountGet(SXParticlesEditor::SelEffID); ++k)
			{
				if (SPE_EmitterEnableGet(SXParticlesEditor::SelEffID, k))
					++emitters_count;

				particles_all_count += SPE_EmitterCountGet(SXParticlesEditor::SelEffID, k);
				particles_life_count += SPE_EmitterCountLifeGet(SXParticlesEditor::SelEffID, k);
			}
		}

		if (SXParticlesEditor::SelEmitterID >= 0)
		{
			emitters_count = SPE_EmitterEnableGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
			particles_all_count += SPE_EmitterCountGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
			particles_life_count += SPE_EmitterCountLifeGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
		}
	}

	char ttext[256];
	
	sprintf(ttext, "Playing emitters: %d/%d", emitters_all_count, emitters_count);
	SXParticlesEditor::StatusBar1->SetTextParts(0, ttext);

	sprintf(ttext, "Living particles: %d/%d", particles_all_count, particles_life_count);
	SXParticlesEditor::StatusBar1->SetTextParts(1, ttext);
}
#endif

#if defined(SX_LEVEL_EDITOR)
void GData::Editors::LevelEditorUpdate()
{
	long count_poly_green = 0;
	for (int i = 0; i < SGeom_GreenGetCount(); ++i)
	{
		count_poly_green += SGeom_GreenMGetCountGen(i) * SGeom_GreenMGetCountPoly(i);
	}

	long count_poly_geom = 0;
	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		count_poly_geom += SGeom_ModelsMGetCountPoly(i);
	}

	char text[256];
	sprintf(text, "%s%d", EDITORS_LEVEL_STATUSBAR_LEVEL_POLY, count_poly_geom + count_poly_green);
	SXLevelEditor::StatusBar1->SetTextParts(0, text);

	sprintf(text, "%s%d", EDITORS_LEVEL_STATUSBAR_GEOM_POLY, count_poly_geom);
	SXLevelEditor::StatusBar1->SetTextParts(1, text);

	sprintf(text, "%s%d", EDITORS_LEVEL_STATUSBAR_GREEN_POLY, count_poly_green);
	SXLevelEditor::StatusBar1->SetTextParts(2, text);

	sprintf(text, "%s%d", EDITORS_LEVEL_STATUSBAR_GAME_COUNT, SXGame_EntGetCount());
	SXLevelEditor::StatusBar1->SetTextParts(3, text);

	if(SXLevelEditor::IdMtl >= 0)
	{
		//sprintf(text, "%s", EDITORS_LEVEL_STATUSBAR_GAME_COUNT, SXGame_EntGetCount());
		SML_MtlGetTexture(SXLevelEditor::IdMtl,text);
		SXLevelEditor::StatusBar1->SetTextParts(4, text);
	}
}

#endif

#if defined(SX_MATERIAL_EDITOR)
void GData::Editors::MaterialEditorUpdate()
{
	ID skit = -1;

	if(!SXMaterialEditor::ComboBoxShaders->Focus())
	{
		char vs[1024];
		char ps[1024];

		SML_MtlGetVS(SXMaterialEditor::IDMat, vs);
		SML_MtlGetPS(SXMaterialEditor::IDMat, ps);

		if ((skit = SXMaterialEditor::Shaders->Find(vs, ps)) >= 0)
			SXMaterialEditor::ComboBoxShaders->SetSel(skit + 1);
		else 
			SXMaterialEditor::ComboBoxShaders->SetSel(0);
	}

	if(!SXMaterialEditor::ComboBoxParamL->Focus())
	{
		float thikcness = SML_MtlGetThickness(SXMaterialEditor::IDMat);
		float roughness = SML_MtlGetRoughness(SXMaterialEditor::IDMat);
		float f0 = SML_MtlGetF0(SXMaterialEditor::IDMat);

		skit = -1;
		if ((skit = SXMaterialEditor::ParamL->Find(thikcness, roughness, f0)) >= 0)
			SXMaterialEditor::ComboBoxParamL->SetSel(skit + 1);
		else
			SXMaterialEditor::ComboBoxParamL->SetSel(0);
	}
}
#endif