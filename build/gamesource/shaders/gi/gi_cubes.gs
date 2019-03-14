
/*
terrax_colored.vs

*/

#include <../struct.h>

//##########################################################################

/*cbuffer perFrame: register(b1)
{
	half4x4 g_mVP;
	half4x4 g_mViewInv;
	half2 g_vNearFar;
	half3 g_vParamProj;
};*/

//##########################################################################

[maxvertexcount(1)]
// void main(point VSO_GICubes input[1], inout TriangleStream<VSO_GICubes> OutputStream)
// void main(point VSO_GICubes input[1], inout LineStream<VSO_GICubes> OutputStream)
void main(point VSO_GICubes input[1], inout PointStream<VSO_GICubes> OutputStream)
{   
//vPosition
    VSO_GICubes output = (VSO_GICubes)0;

	output.vPosition = input[0].vPosition;
	OutputStream.Append(output);
	output.vPosition = half4(0, 0, 0, 0);
	OutputStream.Append(output);
	// output.vPosition = input[0].vPosition + half4(0.1, -0.1, 0.1, 0.0);
	// OutputStream.Append(output);
	
	// OutputStream.RestartStrip();
}
