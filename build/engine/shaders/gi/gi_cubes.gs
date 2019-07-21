
/*
terrax_colored.vs

*/

#include "../struct.h"

//##########################################################################

cbuffer perFrame: register(b1)
{
	half4x4 g_mVP;
	half4x4 g_mViewInv;
	half2 g_vNearFar;
	half3 g_vParamProj;
};

//##########################################################################

[maxvertexcount(14)]
void main(point VSO_GICubes input[1], inout TriangleStream<GSO_GICubes> OutputStream)
// void main(point VSO_GICubes input[1], inout LineStream<VSO_GICubes> OutputStream)
// void main(point VSO_GICubes input[1], inout PointStream<VSO_GICubes> OutputStream)
{
	const float fSize = 0.05;
	const float fNormal = 0.57735027;
	
	float3 vCenter = input[0].vPosition.xyz;
	
	const GSO_GICubes vertices[8] = {
		{mul(input[0].vPosition + float4(fSize, fSize, fSize, 0), g_mVP),    float3(fNormal, fNormal, fNormal),    vCenter},// 0
		{mul(input[0].vPosition + float4(fSize, -fSize, fSize, 0), g_mVP),   float3(fNormal, -fNormal, fNormal),   vCenter},// 1
		{mul(input[0].vPosition + float4(fSize, fSize, -fSize, 0), g_mVP),   float3(fNormal, fNormal, -fNormal),   vCenter},// 2
		{mul(input[0].vPosition + float4(fSize, -fSize, -fSize, 0), g_mVP),  float3(fNormal, -fNormal, -fNormal),  vCenter},// 3
		{mul(input[0].vPosition + float4(-fSize, -fSize, -fSize, 0), g_mVP), float3(-fNormal, -fNormal, -fNormal), vCenter},// 4
		{mul(input[0].vPosition + float4(-fSize, -fSize, fSize, 0), g_mVP),  float3(-fNormal, -fNormal, fNormal),  vCenter},// 5
		{mul(input[0].vPosition + float4(-fSize, fSize, fSize, 0), g_mVP),   float3(-fNormal, fNormal, fNormal),   vCenter},// 6
		{mul(input[0].vPosition + float4(-fSize, fSize, -fSize, 0), g_mVP),  float3(-fNormal, fNormal, -fNormal),  vCenter}// 7
	};

	OutputStream.Append(vertices[0]);
	OutputStream.Append(vertices[1]);
	OutputStream.Append(vertices[2]);
	OutputStream.Append(vertices[3]);
	OutputStream.Append(vertices[4]);
	OutputStream.Append(vertices[1]);
	OutputStream.Append(vertices[5]);
	OutputStream.Append(vertices[0]);
	OutputStream.Append(vertices[6]);
	OutputStream.Append(vertices[2]);
	OutputStream.Append(vertices[7]);
	OutputStream.Append(vertices[4]);
	OutputStream.Append(vertices[6]);
	OutputStream.Append(vertices[5]);
	
	// OutputStream.RestartStrip();
}
