
/*
mtrl.h
Общие функции для материалов
*/

//матрица для трансформации координат текстуры отражени¤ (над водой)
static const half4x4  MatrixReflection =
{	0.5,	0,		0,		0, 
    0,		0.5,	0,		0, 
    0,		0,		0.5,	0, 
    0.5,	0.5,	0.5,	1
};

//матрица дл¤ трансофрмации координат текстуры преломлени¤ (под водой)
static const half4x4  MatrixRefraction =
{	0.5,	0,		0,		0, 
    0,		-0.5,	0,		0, 
    0,		0,		0.5,	0, 
    0.5,	0.5,	0.5,	1
};

//отсечение пиксел¤ по дистанции (при превышении)
void ClipFar(half curr_z, half far)
{
	clip(far-curr_z);
}

//просчет цвета линейной глубины [0,1] на основании текущей позиции и плоскостей отсечени¤
half4 GetDepthW(half4 pos, half2 near_far)
{
	half depth = /*pos.z / pos.w;//*/(pos.z + near_far.x)/near_far.y;
	return half4(depth,depth,depth,1);
}

//! debug просчет линейной глубины
/*half4 GetDepthLinearR(half depth, half2 near_far)
{
	return depth;
}*/

//просчет дистанци на основании линейной глубины и плоскостей отсечени¤
/*half4 GetDepthDistR(half depth, half2 near_far)
{
	return depth*near_far.y;
}*/

half GetTexLod4Ref(half roughness)
{
	return lerp(0.f, MTRL_REF_LOD_COUNT, roughness);
}

/*half LinearizeDepth(half depth, half2 NearFar)
{
	return depth;//(NearFar.x / (NearFar.y + NearFar.x - depth * (NearFar.y - NearFar.x)));
}*/
