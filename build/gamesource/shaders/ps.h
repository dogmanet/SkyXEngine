
//! обертка tex2Dlod
float4 tex2Dlod2(sampler2D oSampler, float2 vTexUV, int iLod)
{
	return tex2Dlod(oSampler, float4(vTexUV,0.f,iLod));
}
