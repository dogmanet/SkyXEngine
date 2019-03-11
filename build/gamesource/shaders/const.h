
/*
struct.h
Файл включения в шейдеры (вершинные и пиксельные)
Определены все структуры которые используются
*/

struct CDataScene
{
	float4x4 mP;
	float4 vTimeDelta;
	float4 vWinSize;
	float4 vNearFarLayers;
};
struct CDataCamera
{
	float4x4 mV;
	float4 vPosCam;
};
cbuffer CDataObject: register(b1)
{
	float4x4 g_mW;
	float4x4 g_mWV;
	float4x4 g_mWVP;
};
cbuffer CDataMaterial: register(b0)
{
	float4 g_vUserVS;
	float4 g_vUserPS;
	float4 g_vDestColor;
	float4 g_vNearFarLayers;
};










