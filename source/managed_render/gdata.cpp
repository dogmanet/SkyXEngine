
#include "gdata.h"
/*
namespace GData
{
	IDirect3DDevice9* DXDevice = 0;	//!< dx устройство

	HWND HandlePreview = 0;	//!< хэндл превью окна
	HWND HandleParent3D = 0;//!< хэндл окна родителя окна рендера, на случай редакторов
	HWND Handle3D = 0;		//!< хэндл окна рендера

	float2_t WinSize = float2_t(800, 600);	//!< размер окна рендера (области рендера)
	bool IsWindowed = true;					//!<использовать ли оконный режим рендера?


	DS_RT FinalImage = DS_RT::ds_rt_scene_light_com;//!< финальное изображение
	ISXCamera* ObjCamera = 0;	//!< камера для которой будет рендер
	ID IDSelectTex = -1;
	int ReSize = 0;				//!< 0 - ничего не меняли, 1 - ресайз, 2 - переход между фуллскрин и окном

	float2_t NearFar = float2_t(0.025, 400);	//!< значение дальней и ближней плоскостей отсечения
	float ProjFov = SM_PIDIV4;				//!< fov камеры

	ID DefaultGeomIDArr = -1;
	ID DefaultGreenIDArr = -1;
	ID DefaultAnimIDArr = -1;

	//! Параметры перемещения камеры по умолчанию
	float4_t CamWalkParamEditor = float4_t(
		10.f,	//!< простое движенеи вперед
		5.f,	//!< коэфициент ускорения
		0.7f,	//!< коэфициент от основного движения в стороны 
		0.5f	//!< коэфициент от основного движения назад
		);
};*/

namespace GData
{
	 IDirect3DDevice9* DXDevice = 0;	//!< dx устройство

	//! функция обработчик оконных сообщений (окна рендера)
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//! инициализация окна рендера
	void InitWin(
		const char* name,	//!< имя окна (только латиница)
		const char* caption	//!< название окна
		);

	 HWND HandlePreview = 0;	//!< хэндл превью окна
	 HWND HandleParent3D = 0;//!< хэндл окна родителя окна рендера, на случай редакторов
	 HWND Handle3D = 0;		//!< хэндл окна рендера

	 float2_t WinSize = float2_t(800, 600);	//!< размер окна рендера (области рендера)
	 bool IsWindowed = true;					//!<использовать ли оконный режим рендера?


	 DS_RT FinalImage = DS_RT::ds_rt_scene_light_com;//!< финальное изображение
	 ISXCamera* ObjCamera = 0;	//!< камера для которой будет рендер
	 ID IDSelectTex = -1;
	 int ReSize = 0;				//!< 0 - ничего не меняли, 1 - ресайз, 2 - переход между фуллскрин и окном

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
};



void GData::InitAllMatrix()
{
	GData::MCamProj = SMMatrixPerspectiveFovLH(GData::ProjFov, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(GData::ProjFov, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, G_DATA_LIGHT_FAR);

	GData::MRefPlaneSkyProj = SMMatrixPerspectiveFovLH(GData::ProjFov, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, G_DATA_LIGHT_FAR);
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
	GData::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render", ShaderCheckDouble::scd_path);
	GData::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render", ShaderCheckDouble::scd_path);

	GData::IDsShaders::PS::ToneMapping = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_tone_mapping.ps", "lighting_tone_mapping", ShaderCheckDouble::scd_path);

	GData::IDsShaders::VS::ResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos", ShaderCheckDouble::scd_path);


	GData::IDsShaders::PS::ComLightingNonShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_nonshadow", ShaderCheckDouble::scd_name);
	D3DXMACRO Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	GData::IDsShaders::PS::ComLightingShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_shadow", ShaderCheckDouble::scd_name, Defines_IS_SHADOWED);
	GData::IDsShaders::PS::BlendAmbientSpecDiffColor = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_blend.ps", "lighting_blend", ShaderCheckDouble::scd_path);

	GData::IDsShaders::PS::UnionAlpha = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_union_alpha.ps", "pp_union_alpha", ShaderCheckDouble::scd_path);

	D3DXMACRO Defines_STR[] = { { "_STR_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilStr = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str", ShaderCheckDouble::scd_name, Defines_STR);
	D3DXMACRO Defines_COLUMN[] = { { "_COLUMN_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_column", ShaderCheckDouble::scd_name, Defines_COLUMN);
	D3DXMACRO Defines_COLUMN_STR[] = { { "_COLUMN_STR_", "" }, { 0, 0 } };
	GData::IDsShaders::PS::StencilStrColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str_column", ShaderCheckDouble::scd_name, Defines_COLUMN_STR);
}

LRESULT CALLBACK GData::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	IMSG msg;
	msg.lParam = lParam;
	msg.wParam = wParam;
	msg.message = message;

	SSInput_AddMsg(msg);

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

void GData::InitWin(const char* name, const char* caption)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GData::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_LOGO));
	wcex.hCursor = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = name;
	wcex.hIconSm = wcex.hIcon;

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, GData::WinSize.x, GData::WinSize.y };
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int posx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int posy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	GData::Handle3D = CreateWindowEx(
		0,
		name,
		caption,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		posx, posy,
		width, height,
		0, 0,
		GetModuleHandle(0),
		0);

	ShowWindow(GData::Handle3D, SW_HIDE);
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