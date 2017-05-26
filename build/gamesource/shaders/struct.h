
/*
Имя файла - struct.h
Файл включения в шейдеры (вершинные и пиксельные)
Определены все структуры которые используются
*/

#include <../gdefines.h>

//ПОСТПРОЦЕСС
//{
//выходная структура вершинного шейдера, восстановление позиции по глубине
struct vs_out_res_pos
{
	half4 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
	
	half3 EyeRay	:TEXCOORD1;
	half3 WorldRay	:TEXCOORD2;
};

//стандартная структура постпроцесса, на вход врешинному шейдеру
struct vs_in_pp
{
	half4 Position	:POSITION;
	half2 TexUV	:TEXCOORD0;
};

//стандартная структура постпроцесса, на выход врешинного шейдера и на вход пиксельному
struct vs_out_pp
{
	half4 Position	:POSITION0;
	half2 TexUV	:TEXCOORD0;
};
//}

//РАСТИТЕЛЬНОСТЬ
struct vs_in_green 
{
	half3 Position	:POSITION0;
	half3 Normal	:NORMAL0;
	half2 TexUV		:TEXCOORD0;
	half3 InstancePos	:TEXCOORD1;
	half3 InstanceTrans	:TEXCOORD2;
	half2 InstanceSinCosRot	:TEXCOORD3;
};

//СТАТИЧЕСКАЯ ГЕОМЕТРИЯ
struct vs_in_geom
{
	half3 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
	half3 Normal	:NORMAL0;
};

//АНИМАЦИОННАЯ МОДЕЛЬ
struct vs_in_anim
{
	half3 Pos : POSITION0;
	half2 Tex : TEXCOORD0;
	half3 Norm : NORMAL0;
	uint4 Bones : BLENDINDICES0;
	half4 Weights : BLENDWEIGHT0;
};

//ВОДА
struct vs_out_water
{
	half4 Position:	POSITION;
	half2 Tex2:		TEXCOORD0;			//текстурные координаты для bump карты
	half2 Tex3:		TEXCOORD1;			//текстурные координаты для du/dv карты
	half4 Pos:		TEXCOORD2;			//позиция (для расчета координат отражения)
	half3 ViewDir:	TEXCOORD3;
	half2 TexUV:	TEXCOORD4;			//нормальные текстурные координаты для текстуры мусора
	half3 Normal:	TEXCOORD5;
};

//ОБЩАЯ СТРУКТУРА ВЫДАВАЕМАЯ ВЕРШИННЫМ ШЕЙДЕРОМ (И ПЕРЕДАВАЕМАЯ В ПИКСЕЛЬНЫЙ)
struct vs_out_gcommon 
{
	half4 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
	half3 Normal	:TEXCOORD1;
	half4 Pos		:TEXCOORD2;
};

struct vs_out_refcube 
{
	half4 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
	half3 Normal	:TEXCOORD1;
	half4 Pos		:TEXCOORD2;
	half3 CubePPos	:TEXCOORD3;
};

//СТРУКТУРА ВЫВОДА ВСЕХ ДАННЫХ ДЛЯ ОТЛОЖЕННОГО ОСВЕЩЕНИЯ ПРИ MRT
struct ps_out_ds_mrt 
{
	half4 Color		:COLOR0;
	half4 Normal	:COLOR1;
	half4 Param		:COLOR2;
	half4 Depth		:COLOR3;
};

//СТРУКТУРА ВЫВОДА БАЗОВОЙ ИНФОМРАЦИИ ПРИ ПРОСЧЕТАХ ОСВЕЩЕНИЯ (ДАННЫЕ ИЗ ps_out_ds_mrt)
struct ps_out_com_lighting
{
	half4 Amdient	:COLOR0;
	half4 Spec		:COLOR1;
};


//SKYBOX
//{
struct vs_in_skybox
{
	half4 Position	:POSITION0;
	half3 TexUV		:TEXCOORD0;
};

struct vs_out_skybox
{
	half4 Position	:POSITION0;
	half3 TexUV		:TEXCOORD0;
};
//}

//SKYCLOUDS
//{
struct vs_in_skyclouds
{
	half4 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
};

struct vs_out_skyclouds
{
	half4 Position	:POSITION;
	half2 TexUV		:TEXCOORD0;
};
//}

//PARTICLES
//{
struct vs_in_particles 
{
	float3 Position :POSITION0;
	float2 TexUV : TEXCOORD0;
	
	float3 InstancePos :TEXCOORD1;
	float4 InstanceTex :TEXCOORD2;
	float InstanceSize :TEXCOORD3;
	float InstanceAlpha :TEXCOORD4;
	
	float2 InstanceRot1 :TEXCOORD5;
	float InstanceLight :TEXCOORD6;
};

struct vs_out_particles 
{
	float4 Position :POSITION0;
	float2 TexUV 	:TEXCOORD0;
	float Alpha		:TEXCOORD1;
	float4 Pos		:TEXCOORD2;
	float Light	:TEXCOORD3;
	float2 TexUV2	:TEXCOORD4;
};
//}
