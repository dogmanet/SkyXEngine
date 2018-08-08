
/*
particles_main.vs
Рендер партиклов
*/

#include <../struct.h>

//##########################################################################

half4x4 WorldViewProjection;

//##########################################################################

half3 RotateVec(half4 q, half3 p)
{
	half xxzz = q.x*q.x - q.z*q.z;
	half wwyy = q.w*q.w - q.y*q.y;
	half xw2 = q.x*q.w*2.0;
	half xy2 = q.x*q.y*2.0;
	half xz2 = q.x*q.z*2.0;
	half yw2 = q.y*q.w*2.0;
	half yz2 = q.y*q.z*2.0;
	half zw2 = q.z*q.w*2.0;
	half3 oout = half3((xxzz + wwyy)*p.x		+ (xy2 + zw2)*p.y		+ (xz2 - yw2)*p.z,
					(xy2 - zw2)*p.x			+ (q.y*q.y+q.w*q.w-q.x*q.x-q.z*q.z)*p.y	+ (yz2 + xw2)*p.z,
					(xz2 + yw2)*p.x			+ (yz2 - xw2)*p.y		+ (wwyy - xxzz)*p.z);
	return(oout);
}

//##########################################################################

void main(in VSI_Particles IN, out VSO_Particles OUT) 
{
	half2 tvec;
	IN.vPosition.xyz *= IN.vInstSize;
	IN.vPosition.xyz = RotateVec(IN.vInstTex,IN.vPosition.xyz);
	OUT.vPosition.xyz = IN.vPosition + IN.vInstPos;
	OUT.vPosition.w = 1.f;
	
	OUT.vPosition = mul(OUT.vPosition, WorldViewProjection);
	OUT.vPos = OUT.vPosition;
	OUT.vTexUV = IN.vTexUV;
	OUT.Alpha = IN.vInstAlpha;
}