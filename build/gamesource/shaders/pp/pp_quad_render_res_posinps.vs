
#include <../struct.h>

float2	NearFar;
float3	ParamProj;

vs_out_res_pos main(vs_in_pp IN)
{
	vs_out_res_pos OUT;
	OUT.Position = IN.Position;
	OUT.TexUV = IN.TexUV;
	
	float tanHalfFOV = tan( ParamProj.z * 0.5f) ; 
	float aspectRatio = ParamProj.x / ParamProj.y; 
	OUT.FarXY.y = tanHalfFOV * NearFar.y; 
	OUT.FarXY.x = OUT.FarXY.y * aspectRatio; 
	
	OUT.Pos = OUT.Position.xy;
	
	return OUT;
}