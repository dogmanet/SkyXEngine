
#include <../struct.h>

float4x4 ViewInv;
float2	NearFar;
float3	ParamProj;

vs_out_res_pos main(vs_in_pp IN)
{
	vs_out_res_pos OUT;
	OUT.Position = IN.Position;
	OUT.TexUV = IN.TexUV;
	
	float tanHalfFOV = tan( ParamProj.z * 0.5f) ; 
	float aspectRatio = ParamProj.x / ParamProj.y; 
	float farY = tanHalfFOV * NearFar.y; 
	float farX = farY * aspectRatio; 
	
	OUT.EyeRay = float3(OUT.Position.x * farX, OUT.Position.y * farY, NearFar.y); 
	OUT.WorldRay = mul(float4(OUT.EyeRay, 0.0), ViewInv).xyz;
	
	return OUT;
}