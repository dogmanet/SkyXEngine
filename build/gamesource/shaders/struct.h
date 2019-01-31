
/*
struct.h
Файл включения в шейдеры (вершинные и пиксельные)
Определены все структуры которые используются
*/

#include <../gdefines.h>

/* \name ПОСТПРОЦЕСС
@{*/

//! выходная структура вершинного шейдера, восстановление позиции по глубине
struct VSO_ResPos
{
	half4 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
	
	half3 vEyeRay	:TEXCOORD1;
	half3 vWorldRay	:TEXCOORD2;
};

//! стандартная структура постпроцесса, на вход врешинному шейдеру
struct VSI_PP
{
	half4 vPosition	:POSITION;
	half2 vTexUV	:TEXCOORD0;
};

//! стандартная структура постпроцесса, на выход врешинного шейдера и на вход пиксельному
struct VSO_PP
{
	half4 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
};

//!@}


//! union alpha, структура результата работы объединения полупрозрачных слоев
struct PSO_UnionAlpha
{
	half4 vColor	: COLOR0;
	half4 vDepth0	: COLOR1;
	half4 vDepth1	: COLOR2;
};


//! растительность
struct VSI_Green 
{
	half3 vPosition		:POSITION0;
	half3 vNormal		:NORMAL0;
	half2 vTexUV		:TEXCOORD0;
	half3 vInstPos		:TEXCOORD1;
	half3 vInstTrans	:TEXCOORD2;
	half2 vInstSinCosRot:TEXCOORD3;
};


//! статическая геометрия
struct VSI_Geometry
{
	half3 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
	half3 vNormal	:NORMAL0;
};


//! анимационная модель
struct VSI_Animation
{
	half3 vPosition	: POSITION0;
	half2 vTexUV	: TEXCOORD0;
	half3 vNormal	: NORMAL0;
	half3 vTangent	: TANGENT0;
	half3 vBinormal	: BINORMAL0;
	uint4 vBones	: BLENDINDICES0;
	half4 vWeights	: BLENDWEIGHT0;
};


//! вода
struct VSO_WaterPlane
{
	half4 vPosition:	POSITION;
	
	//! текстурные координаты для bump карты
	half2 vTexUV2:		TEXCOORD0;			
	
	//! текстурные координаты для du/dv карты
	half2 vTexUV3:		TEXCOORD1;			
	
	//! позиция (для расчета координат отражения)
	half4 vPos:			TEXCOORD2;			
	half3 ViewDir:		TEXCOORD3;
	
	//! нормальные текстурные координаты
	half2 vTexUV:		TEXCOORD4;			
	half3 vNormal:		TEXCOORD5;
};


//! общая структура выдаваемая вершинным шейдером (и передаваемая в пиксельный)
struct VSO_SceneCommon 
{
	half4 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
	half3 vNormal	:TEXCOORD1;
	half4 vPos		:TEXCOORD2;
};


//! выходная структура с для кубического отражения
struct VSO_RefCube 
{
	half4 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
	half3 vNormal	:TEXCOORD1;
	half4 vPos		:TEXCOORD2;
	half3 vCubePPos	:TEXCOORD3;
};


//! структура вывода всех данных для отложенного освещения при mrt
struct PSO_Gbuffer 
{
	half4 vColor	:COLOR0;
	half4 vNormal	:COLOR1;
	half4 vParam	:COLOR2;
	half4 vDepth	:COLOR3;
};


//! структура вывода базовой инфомрации при просчетах освещения (данные из #PSO_Gbuffer)
struct PSO_Lbuffer
{
	half4 vAmdient	:COLOR0;
	half4 vSpecular	:COLOR1;
};


/*! \name SkyBox
@{*/

struct VSI_SkyBox
{
	half4 vPosition	:POSITION0;
	half3 vTexUV	:TEXCOORD0;
};

struct VSO_SkyBox
{
	half4 vPosition	:POSITION0;
	half3 vTexUV	:TEXCOORD0;
};

//!@}


/*! \name SkyClouds
@{*/

struct VSI_SkyClouds
{
	half4 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
};

struct VSO_SkyClouds
{
	half4 vPosition	:POSITION;
	half2 vTexUV	:TEXCOORD0;
};

//!@}


/*! \name Particles
@{*/

struct VSI_Particles 
{
	half3 vPosition	:POSITION0;
	half2 vTexUV 	:TEXCOORD0;
	
	half3 vInstPos	:TEXCOORD1;
	half4 vInstTex	:TEXCOORD2;
	half vInstSize	:TEXCOORD3;
	half vInstAlpha	:TEXCOORD4;
	
	half2 vInstRot1	:TEXCOORD5;
	half vInstLight	:TEXCOORD6;
};

struct VSO_Particles 
{
	half4 vPosition	:POSITION0;
	half2 vTexUV 	:TEXCOORD0;
	half Alpha		:TEXCOORD1;
	half4 vPos		:TEXCOORD2;
	half Light		:TEXCOORD3;
	half2 vTexUV2	:TEXCOORD4;
};

//!@}


/*! \name AI grid
@{*/

struct VSI_AIgrid 
{
	half3 vPosition	:POSITION0;
	half2 vTexUV 	:TEXCOORD0;
	
	half3 vInstPos	:TEXCOORD1;
	half4 vInstTex	:TEXCOORD2;
	half4 vInstColor:COLOR0;
};

struct VSO_AIgrid 
{
	half4 vPosition	:POSITION0;
	half2 vTexUV	:TEXCOORD0;
	half4 vPos		:TEXCOORD1;
	half4 vColor	:TEXCOORD2;
};

//!@}

/*! \name LightBound
@{*/

struct VSI_LightBound
{
	half4 vPosition	:POSITION0;
};

struct VSO_LightBound
{
	half4 vPosition	:POSITION;
};

//!@}
