
#include <../struct.h>

half4x4 ViewInv;
half2 NearFar;
half3 ParamProj;

struct vs_in_pp2
{
	half4 Position	:POSITION;
	half3 TexUV		:TEXCOORD0;
};

vs_out_res_pos main(vs_in_pp2 IN)
{
	vs_out_res_pos OUT;
	OUT.Position = IN.Position;
	OUT.TexUV = IN.TexUV.xy;
	
	half tanHalfFOV = tan( ParamProj.z * 0.5f) ; 
	half aspectRatio = ParamProj.x / ParamProj.y; 
	half farY = tanHalfFOV * NearFar.y; 
	half farX = farY * aspectRatio; 
	
	OUT.EyeRay = half3(sign(OUT.Position.x) * farX, sign(OUT.Position.y) * farY, NearFar.y); 
	OUT.WorldRay = mul(half4(OUT.EyeRay, 0.0), ViewInv).xyz;
  
	return OUT;
}