
/*
shadow.h
функции для теней
*/

//! альфа тест при записи глубины для теней
half4 ShadowDepthAlphaTest(half4 vColor)
{
	clip(vColor.a - SHADOW_DEPTH_ALPHATEST_VALUE);
	vColor.a = 1.f;
	return vColor;
}

//! смещение позиции по нормали
half3 ShadowPosScopeBias(half3 vPosition, half3 vNormal)
{
	return (vPosition.xyz + vNormal * SHADOW_SCOPE_BIAS);
}

/*! смещение на основании slope, источник: https://gamedev.stackexchange.com/questions/65739/use-depth-bias-for-shadows-in-deferred-shading
надо будет посмотреть http://developer.amd.com/wordpress/media/2012/10/Isidoro-ShadowMapping.pdf
*/
half3 ShadowSlopeBias(half fDepth)
{
	return max(abs(ddx(fDepth)),abs(ddy(fDepth))) * SHADOW_SLOPE_SCALE + SHADOW_SLOPE_BIAS;
}

//! расчет тени
half ShadowCalcShading(half fDepth0, half fDepth1, half fBias)
{
	return (((fDepth0 + fBias) < fDepth1) ? 0.0f : 1.0f);
}