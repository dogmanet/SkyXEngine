
/*
Имя файла - struct.h
Файл включения в шейдеры (вершинные и пиксельные)
Определены все структуры которые используются
*/

float4 GetDepthW(float4 pos, float2 near_far)
{
	float depth = (pos.z + near_far.x)/near_far.y;
	return float4(depth,depth,depth,1);
}

float4 GetDepthLinearR(float depth, float2 near_far)
{
	return depth;
}

float4 GetDepthDistR(float depth, float2 near_far)
{
	return depth*near_far.y;
}


float4 bilinear(sampler2D tex, float2 texCoord, int texSize) 
{
   float2 trTexCoord = texCoord*texSize;
   float2 texf = floor(trTexCoord);
   float2 ratio = trTexCoord - texf;
   float2 opposite = 1.0 - ratio;
   float4 result = (tex2D(tex, texf/texSize) * opposite.x  + tex2D(tex, (texf+float2(1, 0))/texSize)   * ratio.x) * opposite.y + 
                   (tex2D(tex, (texf+float2(0, 1))/texSize) * opposite.x + tex2D(tex, (texf+float2(1, 1))/texSize) * ratio.x) * ratio.y;
   return result;
}

//ПОСТПРОЦЕСС
//{
//выходная структура вершинного шейдера, восстановление позиции по глубине
struct vs_out_res_pos
{
	float4 Position	:POSITION0;
	float2 TexUV		:TEXCOORD0;
	
	float3 EyeRay	:TEXCOORD1;
	float3 WorldRay	:TEXCOORD2;
};

//стандартная структура постпроцесса, на вход врешинному шейдеру
struct vs_in_pp
{
	float4 Position	:POSITION;
	float2 TexUV	:TEXCOORD0;
};

//стандартная структура постпроцесса, на выход врешинного шейдера и на вход пиксельному
struct vs_out_pp
{
	float4 Position	:POSITION0;
	float2 TexUV	:TEXCOORD0;
};
//}

//РАСТИТЕЛЬНОСТЬ
struct vs_in_green 
{
	float3 Position	:POSITION0;
	float3 Normal	:NORMAL0;
	float2 TexUV		:TEXCOORD0;
	float3 InstancePos	:TEXCOORD1;
	float3 InstanceTrans	:TEXCOORD2;
	float2 InstanceSinCosRot	:TEXCOORD3;
};

//СТАТИЧЕСКАЯ ГЕОМЕТРИЯ
struct vs_in_geom
{
	float3 Position	:POSITION0;
	float2 TexUV		:TEXCOORD0;
	float3 Normal	:NORMAL0;
};

//ВОДА
struct vs_out_water
{
	float4 Position:	POSITION;
	float2 Tex2:		TEXCOORD0;			//текстурные координаты для bump карты
	float2 Tex3:		TEXCOORD1;			//текстурные координаты для du/dv карты
	float4 Pos:		TEXCOORD2;			//позиция (для расчета координат отражения)
	float3 ViewDir:		TEXCOORD3;
	float2 TexUV:		TEXCOORD4;			//нормальные текстурные координаты для текстуры мусора
	float3 Normal:		TEXCOORD5;
};

//ОБЩАЯ СТРУКТУРА ВЫДАВАЕМАЯ ВЕРШИННЫМ ШЕЙДЕРОМ (И ПЕРЕДАВАЕМАЯ В ПИКСЕЛЬНЫЙ)
struct vs_out_gcommon 
{
	float4 Position	:POSITION0;
	float2 TexUV		:TEXCOORD0;
	float3 Normal	:TEXCOORD1;
	float4 Pos		:TEXCOORD2;
};

//СТРУКТУРА ВЫВОДА ВСЕХ ДАННЫХ ДЛЯ ОТЛОЖЕННОГО ОСВЕЩЕНИЯ ПРИ MRT
struct ps_out_ds_mrt 
{
	float4 Color		:COLOR0;
	float4 Normal	:COLOR1;
	float4 Param		:COLOR2;
	float4 Depth		:COLOR3;
};

//СТРУКТУРА ВЫВОДА БАЗОВОЙ ИНФОМРАЦИИ ПРИ ПРОСЧЕТАХ ОСВЕЩЕНИЯ (ДАННЫЕ ИЗ ps_out_ds_mrt)
struct ps_out_com_lighting
{
	float4 Amdient	:COLOR0;
	float4 Spec		:COLOR1;
};


//SKYBOX
//{
struct vs_in_skybox
{
	float4 Position	:POSITION0;
	float3 TexUV		:TEXCOORD0;
};

struct vs_out_skybox
{
	float4 Position	:POSITION0;
	float3 TexUV		:TEXCOORD0;
};
//}

//SKYCLOUDS
//{
struct vs_in_skyclouds
{
	float4 Position	:POSITION0;
	float2 TexUV		:TEXCOORD0;
};

struct vs_out_skyclouds
{
	float4 Position	:POSITION;
	float2 TexUV		:TEXCOORD0;
};
//}
