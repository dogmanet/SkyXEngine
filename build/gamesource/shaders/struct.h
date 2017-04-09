
/*
Имя файла - struct.h
Файл включения в шейдеры (вершинные и пиксельные)
Определены все структуры которые используются
*/

#define GREEN_ALPHA_VAL_CLIP 	0.5
#define PSSM_SHADING_LAST 		0.1
#define SHADOWGEN_JITTER_SCALE 1024.0

#define PI 3.141592654f

#define NOTLIGHTING_NOTTRANSPARENCY 0.0
#define LIGHTING_NOTTRANSPARENCY 	0.333333
#define NOTLIGHTING_TRANSPARENCY 	0.666667
#define LIGHTING_TRANSPARENCY 		1.0

half4 GetDepthW(half4 pos, half2 near_far)
{
	half depth = (pos.z + near_far.x)/near_far.y;
	return half4(depth,depth,depth,1);
}

half4 GetDepthLinearR(half depth, half2 near_far)
{
	return depth;
}

half4 GetDepthDistR(half depth, half2 near_far)
{
	return depth*near_far.y;
}


half4 bilinear(sampler2D tex, half2 texCoord, int texSize) 
{
   half2 trTexCoord = texCoord*texSize;
   half2 texf = floor(trTexCoord);
   half2 ratio = trTexCoord - texf;
   half2 opposite = 1.0 - ratio;
   half4 result = (tex2D(tex, texf/texSize) * opposite.x  + tex2D(tex, (texf+half2(1, 0))/texSize)   * ratio.x) * opposite.y + 
                   (tex2D(tex, (texf+half2(0, 1))/texSize) * opposite.x + tex2D(tex, (texf+half2(1, 1))/texSize) * ratio.x) * ratio.y;
   return result;
}

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
