
/*
mtrl.h
Общие функции для материалов
*/

#ifndef __MTRL_H
#define __MTRL_H
 
//! матрица для трансформации координат текстуры отражения (над водой)
static const float4x4  MatrixReflection =
{	0.5,	0,		0,		0, 
    0,		0.5,	0,		0, 
    0,		0,		0.5,	0, 
    0.5,	0.5,	0.5,	1
};

//! матрица для трансофрмации координат текстуры преломления (под водой)
static const float4x4  MatrixRefraction =
{	0.5,	0,		0,		0, 
    0,		-0.5,	0,		0, 
    0,		0,		0.5,	0, 
    0.5,	0.5,	0.5,	1
};

//##########################################################################

//! отсечение пикселя по дистанции (при превышении)
void ClipFar(float fZ, float fFar)
{
	clip(fFar-fZ);
}

//##########################################################################

//! просчет цвета линейной глубины [0,1] на основании текущей позиции и плоскостей отсечения! 
float ComDepthByPos(float4 vPositionWVP, float2 vNearFar)
{
	// return ((vPositionWVP.z + vNearFar.x)/vNearFar.y);
	return (1.0f - vPositionWVP.z/vNearFar.x);
}

//! аналогично #ComDepthByPos, только возвращает вектор
float4 ComDepthByPosVec4(float4 vPositionWVP, float2 vNearFar)
{
	float fDepth = ComDepthByPos(vPositionWVP, vNearFar);
	return float4(fDepth, fDepth, fDepth,1);
}

#define GetDepthW ComDepthByPosVec4

//##########################################################################

//! возвращает номер лода для для отражений, на основании шероховатости материала [0, 1]
float GetTexLod4Ref(float fRoughness)
{
	return lerp(0.f, MTRL_REF_LOD_COUNT, fRoughness);
}

/*! возаращет нужный лод текстуры,
 \param fCountLods - количество лодов в текстуре
 \param vDX - ddx(vTexUV.x)
 \param vDY - ddx(vTexUV.y)
*/
float GetTextureLodEx(float fCountLods, float2 vDX, float2 vDY)
{
	return (fCountLods - 1.0)+log2(max(length(vDX), length(vDY)));
}

//! возвращает номер лода, подробности #GetTextureLodEx
float GetTextureLod(float fCountLods, float2 vTexUV)
{
	return GetTextureLodEx(fCountLods, ddx(vTexUV.x), ddx(vTexUV.y));
}

//! возвращает номер нужного лода на основании текстурных координат, для текстур сцены (текстуры для моделей)
float GetTextureLod4Scene(float2 vTexUV)
{
	return GetTextureLodEx(SCENE_COUNT_TEXTURE_LOD, ddx(vTexUV), ddy(vTexUV));
}

//##########################################################################

//! кодирование xyz нормали в xy
float3 NormalEncode(float3 vNormal)
{
	/*vNormal.xy = vNormal.xy * 0.5 + 0.5;
	
	[branch]if(vNormal.z >= 0.0)
		vNormal.z = 0.5 + fLayer * g_fUnit256;
	else
		vNormal.z = fLayer * g_fUnit256;*/
	
	return vNormal * 0.5 + 0.5;
}

//! декодирование нормали xy в xyz
float3 NormalDecode(float3 vNormal)
{
	/*float fLayer = 0;
	
	if(vNormal.z >= 0.5)
		fLayer = (floor((vNormal.z - 0.5) * 255.0 + 0.5)) * g_fUnit256;
	else
		fLayer = floor(vNormal.z * 255.0 + 0.5) * g_fUnit256;
	
	vNormal.xy = vNormal.xy * 2.0 - 1.0;
	vNormal.z = sign(vNormal.z * 2.0 - 1.0);
	vNormal.z = sqrt(1 - pow(vNormal.x, 2) - pow(vNormal.y, 2)) * vNormal.z;
	
	return float4(vNormal, fLayer);*/
	
	return vNormal * 2.0 - 1.0;
}

float LayerEncode(float fLayer, int iLighted)
{
	float fResult = 0;
	[branch]if(iLighted == MTLTYPE_LIGHT)
		fResult = 0.5 + fLayer * g_fUnit256;
	else
		fResult = fLayer * g_fUnit256;
	
	return fResult;
}


float2 LayerDecode(float fCode)
{
	float fLayer = 0;
	int iLighted = MTLTYPE_UNLIT;
	
	if(fCode >= 0.5)
	{
		fLayer = /*fCode - 0.5;//*/(floor((fCode - 0.5) * 255.0 + 0.5)) * g_fUnit256;
		iLighted = MTLTYPE_LIGHT;
	}
	else
	{
		fLayer = /*fCode;//*/floor(fCode * 255.0 + 0.5) * g_fUnit256;
		iLighted = MTLTYPE_UNLIT;
	}
	
	return float2(int(fLayer * LAYERS_COUNT_MAX), iLighted);
}

#define LayerDecodeLayer(fCode)(LayerDecode(fCode).x)
#define LayerDecodeType(fCode)(LayerDecode(fCode).y)

//! преобразвоание цвета в нормаль (если конечно цвет содержит нормаль)
float3 Color2Normal(float3 vColor)
{
	return (2.0 * vColor - 1.0);
}

//##########################################################################

/*! формирование структуры G буфера
 \param vColor - float4 цвет 
 \param vNormal - float3 нормаль 
 \param vParam - float4 параметры освещения (из текстуры парамтеров) 
 \param vWVPposition - float4 позиция пикселя (считается в вершинном шейдере как mul(vPos, g_mWVP))
 \param vNearFarLayers - float4, аналог g_vNearFarLayers (https://wiki.skyxengine.com/index.php?title=Организация_шейдеров_материалов#Константы) 
*/
PSO_Gbuffer CreateGbuffer(float4 vColor, float3 vNormal, float4 vParam, float4 vWVPposition, float4 vNearFarLayers)
{
	PSO_Gbuffer OUT;
	
	OUT.vColor = vColor;
	
	/*vNormal = normalize(vNormal);
	OUT.vNormal.xyz = NormalEncode(vNormal, vNearFarLayers.w);
	OUT.vNormal.w = vNearFarLayers.z;*/
	
	vNormal = normalize(vNormal);
	OUT.vNormal.xyz = NormalEncode(vNormal);
	OUT.vNormal.w = LayerEncode(vNearFarLayers.w, vNearFarLayers.z);
	
	OUT.vParam = vParam;
	
	OUT.vDepth = ComDepthByPosVec4(vWVPposition,vNearFarLayers.xy);
	
	return OUT;
}

//##########################################################################

//! возвращает фактор интерполяции [0, 1] для детальных/микрорельефных текстур, где чем меньше значение тем больше будет проявление детальности/микронормалей
float GetLerpFactorDetail(float fDistance)
{
	return saturate(fDistance/MTRL_LAND_DIST);
}

//! смешивание макронормали (модели) с микронормалью (из normal map)
/*float3 MixNormalMicro(float3 vMacroNormal, float3 vMicroNormal)
{
	return normalize((vMacroNormal + float3(vMicroNormal.xy,vMacroNormal.z)));
}*/

float3 MixNormalMicro(float3 vNormalMap, float3 vTangent, float3 vBinormal, float3 vNormal)
{
	float3x3 mTBN = float3x3(vTangent, vBinormal, vNormal);
	return normalize(mul(vNormalMap, mTBN));
}

//! смешивание 2 детальных текстур по маске, где r канал маски для первой детальной текстуры, а g для второй
float4 MixDetail2(float4 vMask, float4 vDetail1, float4 vDetail2, float fIntesity)
{
	return (vDetail1 * vMask.r + vDetail2 * vMask.g) * fIntesity * 2.0;
}

//! смешивание 4 детальных текстур по маске, где на каждую текстуру по каналу из маски, r канал для первой детальной, и т.д.
float4 MixDetail4(float4 vMask, float4 vDetail1, float4 vDetail2, float4 vDetail3, float4 vDetail4, float fIntesity)
{
	return (vDetail1 * vMask.r + vDetail2 * vMask.g + vDetail3 * vMask.b + vDetail4 * vMask.a) * fIntesity;
}

//! смешивание двух микронормалей (normal map)
float3 MixMicroNormal(float3 vMicroNormal1, float3 vMicroNormal2, float fIntesity)
{
	return (vMicroNormal1 * fIntesity + vMicroNormal2 * fIntesity);
}

//! смешивание 4 микронормалей (normal map) по маске, где на каждую текстуру по каналу из маски, r канал для vMicroNormal1, и т.д.
float3 MixMicroNormal4(float4 vMask, float3 vMicroNormal1, float3 vMicroNormal2, float3 vMicroNormal3, float3 vMicroNormal4, float fIntesity)
{
	return (vMicroNormal1 * vMask.r + vMicroNormal2 * vMask.g + vMicroNormal3 * vMask.b + vMicroNormal4 * vMask.a) * fIntesity;
}

//! смешивание цвета и детального цвета, на основе фактора интерполяции #GetLerpFactorDetail
float4 MixColorDetail(float4 vColor, float4 vDetail, float fLerpFactor)
{
	float4 vBlend = vColor * vDetail * (2.0 - vDetail);
	vColor.rgb = lerp(vBlend,vColor,fLerpFactor).rgb;
	return vColor;
}

#endif
