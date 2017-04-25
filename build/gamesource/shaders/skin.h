
//

half4 g_BufferBoneWorld[128] : register(c16);

half3 SkinRotateVec(half4 q, half3 p)
{
	half xxzz = q.x*q.x - q.z*q.z;
	half wwyy = q.w*q.w - q.y*q.y;
	half xw2 = q.x*q.w*2.0;
	half xy2 = q.x*q.y*2.0;
	half xz2 = q.x*q.z*2.0;
	half yw2 = q.y*q.w*2.0;
	half yz2 = q.y*q.z*2.0;
	half zw2 = q.z*q.w*2.0;
	half3 oout = half3((xxzz + wwyy)*p.x		+ (xy2 + zw2)*p.y		+ (xz2 - yw2)*p.z,
					(xy2 - zw2)*p.x			+ (q.y*q.y+q.w*q.w-q.x*q.x-q.z*q.z)*p.y	+ (yz2 + xw2)*p.z,
					(xz2 + yw2)*p.x			+ (yz2 - xw2)*p.y		+ (wwyy - xxzz)*p.z);
	return(oout);
}

vs_out_gcommon SkinBoneTransform(uint iBone, half weight, vs_in_anim v)
{
	vs_out_gcommon Output = (vs_out_gcommon)0;
	iBone *= 2;
	half4 bpos = g_BufferBoneWorld[iBone];
	half4 q = g_BufferBoneWorld[iBone + 1];
	Output.Position = half4((SkinRotateVec(q, v.Pos) + bpos) * weight, 1.0);
	Output.Normal = SkinRotateVec(q, v.Norm) * weight;

	return(Output);
}


vs_out_gcommon SkinAllTransform(vs_in_anim v)
{
	vs_out_gcommon Output = (vs_out_gcommon)0;
	vs_out_gcommon o = (vs_out_gcommon)0;
	
	//Bone0
    uint iBone = v.Bones.x;
    float fWeight = v.Weights.x;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
    
    //Bone1
    iBone = v.Bones.y;
    fWeight = v.Weights.y;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
    
    //Bone2
    iBone = v.Bones.z;
    fWeight = v.Weights.z;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
    
    //Bone3
    iBone = v.Bones.w;
    fWeight = v.Weights.w;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.Position += o.Position;
	Output.Normal += o.Normal;

	Output.Position.w = 1.0;
	
	return(Output);
}