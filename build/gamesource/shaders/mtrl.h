
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

half3 NormalEncode(half3 n)
{
    /*half p = sqrt(n.z*8+8);
    return half4(n.xy/p + 0.5,0,0);*/
	
	half2 enc = normalize(n.xy) * (sqrt(-n.z*0.5+0.5));
    enc = enc*0.5+0.5;
    return half3(enc, 0);
}

half3 NormalDecode(half2 enc)
{
   /* half2 fenc = enc*4-2;
    half f = dot(fenc,fenc);
    half g = sqrt(1-f/4);
    half3 n;
    n.xy = fenc*g;
    n.z = 1-f/2;
    return n;*/
	
	half4 nn = half4(enc, enc)*half4(2,2,0,0) + half4(-1,-1,1,-1);
    half l = dot(nn.xyz,-nn.xyw);
    nn.z = l;
    nn.xy *= sqrt(l);
    return nn.xyz * 2 + half3(0,0,-1);
}
