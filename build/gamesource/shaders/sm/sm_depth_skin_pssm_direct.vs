
/*
sm_depth_skin_pssm_direct.vs
Рендер глубины сцены (анимационные модели) с позиции направленного/глобального источника света
*/

#include <../struct.h>
#include <../skin.h>

half4x4	WorldViewProjection;
half4x4	World;

vs_out_gcommon main(vs_in_anim input)
{
	vs_out_gcommon Output = SkinAllTransform(input);
	
	Output.Position = mul(Output.Position, WorldViewProjection);
	Output.TexUV = input.Tex;
	
	Output.Pos = Output.Position;
    
	return(Output);
}
