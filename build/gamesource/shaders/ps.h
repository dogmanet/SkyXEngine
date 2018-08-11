
//! обертка tex2Dlod
half4 tex2Dlod2(sampler2D oSampler, half2 vTexUV, int iLod)
{
	return tex2Dlod(oSampler, half4(vTexUV,0.f,iLod));
}