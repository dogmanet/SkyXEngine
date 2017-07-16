
/*
mtrl.h
Общие функции для материалов
*/

//отсечение пиксел¤ по дистанции (при превышении)
void ClipFar(half curr_z, half far)
{
	clip(far-curr_z);
}

//просчет цвета линейной глубины [0,1] на основании текущей позиции и плоскостей отсечени¤
half4 GetDepthW(half4 pos, half2 near_far)
{
	half depth = (pos.z + near_far.x)/near_far.y;
	return half4(depth,depth,depth,1);
}

//! debug просчет линейной глубины
half4 GetDepthLinearR(half depth, half2 near_far)
{
	return depth;
}

//просчет дистанци на основании линейной глубины и плоскостей отсечени¤
half4 GetDepthDistR(half depth, half2 near_far)
{
	return depth*near_far.y;
}