
#include <../struct.h>
#include <../skin.h>

half4x4	WorldViewProjection;
half4x4	World;

vs_out_gcommon main(vs_in_anim input)
{
	vs_out_gcommon Output = SkinAllTransform(input);
	
	Output.Position = mul(Output.Position, WorldViewProjection);
	Output.Normal = mul(Output.Normal, World);
	Output.TexUV = input.Tex;
	
	Output.Pos = Output.Position;
    
	return(Output);
}
