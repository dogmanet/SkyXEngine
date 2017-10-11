
#include "sxrender.h"

//##########################################################################

//! пространство имен для хранения данных цикла рендера
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

	//////

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

//#########################################################################

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

//##########################################################################

SX_LIB_API void ComDeviceLost()
{
	if (GData::ReSize != 2)
	{
		//получаем текущий размер окна в которое рисовали
		RECT rect_scene;
		GetClientRect(GData::Handle3D, &rect_scene);


		GData::WinSize.x = rect_scene.right;
		GData::WinSize.y = rect_scene.bottom;
	}

	//сбрасываем все что необходимо для восстановления устройства
	SGCore_OnLostDevice();
	SGeom_OnLostDevice();
	SML_OnLostDevice();
	SPE_OnLostDevice();

	bool bf = SGCore_OnDeviceReset(GData::WinSize.x, GData::WinSize.y, GData::IsWindowed);
	if (bf)
	{
		//если все-таки функция зашла сюда значит чтото было неосвобождено
		g_fnReportf(REPORT_MSG_LEVEL_ERROR, "reset device is failed ... \n");
	}
	else
	{
		GData::InitAllMatrix();
		GData::ReSize = 0;
		SGCore_OnResetDevice();
		SML_OnResetDevice();
		SGeom_OnResetDevice();
		SPE_OnResetDevice();


		GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}