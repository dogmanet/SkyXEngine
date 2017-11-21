
/*
mtrlgreen_tree_base.ps
Базовый рендер деревьев/кустов
*/

#include <../struct.h>
#include <../green.h>

half4x4 WorldViewProjection;

void main(in vs_in_green IN, out vs_out_gcommon OUT) 
{
	OUT.Normal = (GreenComRotation(IN.Normal,IN.InstanceSinCosRot));
	
	OUT.Position = GreenTransformPos(
						GreenComRotation(IN.Position,IN.InstanceSinCosRot),
						IN.InstanceTrans.x,
						1,
						IN.InstancePos
					);
	
	OUT.Position = mul(OUT.Position, WorldViewProjection);
	
	OUT.Pos = OUT.Position;
	
	OUT.TexUV = IN.TexUV;
}