
#include <../struct.h>

half4x4 WorldViewProjection;
half2 TimeDelta;
half4 Param;
half4 PosCam;
half4x4 World;	

vs_out_water main(vs_in_geom IN)
{
	vs_out_water OUT;
	OUT.Position = mul(half4(IN.Position,1.f),WorldViewProjection);
	OUT.TexUV = IN.TexUV;
	half4 WorldPos = mul(IN.Position,World);
    OUT.ViewDir = PosCam - WorldPos;
	OUT.Pos = OUT.Position;
	
	OUT.Normal = mul(IN.Normal,World);

	half2 tmptex = IN.TexUV * Param.z * 100.f;
	OUT.Tex2.x = tmptex.x + sin(TimeDelta.x * Param.w*0.01)*Param.x;
	OUT.Tex2.y = tmptex.y - cos(TimeDelta.x * Param.w*0.01)*Param.y;
	OUT.Tex3.x = tmptex.x - sin(TimeDelta.x * Param.w*0.01)*Param.x;
	OUT.Tex3.y = tmptex.y + cos(TimeDelta.x * Param.w*0.01)*Param.y;
	return OUT;
}