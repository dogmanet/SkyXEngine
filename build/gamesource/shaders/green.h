
/*
green.h
функции для трансформации растительности
*/

// просчет множителя для уменьшения масштаба травы
half GrassComMultiplier(half3 vertex_pos, half3 view_pos,half2 dist_bel)
{
	return 1.0 - saturate((distance(view_pos,vertex_pos) - dist_bel.x) / (dist_bel.y - dist_bel.x));
}

// поворот вектора
half3 GreenComRotation(half3 vec, half2 sincos)
{
	half3 tvec;
	tvec.x = vec.x * sincos.y + vec.z * sincos.x;
	tvec.z = -vec.x * sincos.x + vec.z * sincos.y;
	tvec.y = vec.y;
	return tvec;
}

// трансформация позиции на основании данных о масштабе и мировом положении
half4 GreenTransformPos(half3 pos,half scale,half multiplier, half3 translate)
{
	half4 tpos = half4(pos,1);
	tpos.xyz *= scale * multiplier;
	tpos.xyz += translate;
	return tpos;
}

// общий альфа тест растительности
half4 GreenAlphaTest(half4 vColor)
{
	clip(vColor.a - GREEN_ALPHATEST_VALUE);
	vColor.a = 1.f;
	return vColor;
}

// расчет затенености вершины, на основании отдаленности от центра ограничивающей сферы
half GreenComShadingBySphere(half4 vSphere, half3 vPosition)
{
	return pow(length(vSphere.xyz - vPosition) / vSphere.w, 2);
}
