
/*
particles_main.vs
Рендер партиклов
*/

#include <../struct.h>

//##########################################################################

half4x4 ViewProjection;
half4x4 WorldViewProjection;
half4x4 CamRot;

half3 PosCam;

//##########################################################################

void main(in VSI_Particles IN, out VSO_Particles OUT) 
{
	half2 tvec;
	tvec.x = IN.vPosition.x * IN.vInstRot1.y + IN.vPosition.y * IN.vInstRot1.x;
	tvec.y = -IN.vPosition.x * IN.vInstRot1.x + IN.vPosition.y * IN.vInstRot1.y;
	IN.vPosition.xy = tvec;
	
	IN.vPosition.xyz *= IN.vInstSize;
	
	IN.vPosition = mul(IN.vPosition, CamRot);
	
	OUT.vPosition.xyz = IN.vPosition + IN.vInstPos;
	OUT.vPosition.w = 1.f;
	
	OUT.Alpha = IN.vInstAlpha;
	OUT.Light = IN.vInstLight;
	
	OUT.vPosition = mul(OUT.vPosition, WorldViewProjection);
	OUT.vPos = OUT.vPosition;
	OUT.vTexUV2 = IN.vTexUV;
	OUT.vTexUV = (IN.vTexUV * IN.vInstTex.xy)+IN.vInstTex.zw;
}