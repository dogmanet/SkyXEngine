
/*
mtrlgeom_reflect_cube.vs
Рендер геометрии с объемным (кубическим) отражением
*/

#include <../struct.h>
#include <../mtrl.h>

//##########################################################################

half4x4	g_mWVP;
half4x4	g_mWV;
half4x4	g_mV;
half4x4	g_mW;

//##########################################################################

VSO_RefCube main(VSI_Geometry IN)
{
	VSO_RefCube OUT;

	OUT.vPosition = mul(half4(IN.vPosition,1), g_mWVP);
	OUT.vNormal	= mul(IN.vNormal, g_mW);
	OUT.vTexUV = IN.vTexUV;
	OUT.vPos = OUT.vPosition;
	
	half3 vView= mul(IN.vPosition, g_mWV);
	OUT.vCubePPos = mul((half3x3)(g_mV),reflect(vView, mul(OUT.vNormal, (half3x3)(g_mV))));
	OUT.vCubePPos.y = -OUT.vCubePPos.y;
	return OUT;
}