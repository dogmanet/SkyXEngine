
/*
skin.h
Функции/данные трансформации анимационных моделей
*/

half4 g_BufferBoneWorld[128] : register(ANIM_R_DATA);

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

VSO_SceneCommon SkinBoneTransform(uint iBone, half weight, VSI_Animation v)
{
	VSO_SceneCommon Output = (VSO_SceneCommon)0;
	iBone *= 2;
	half4 bpos = g_BufferBoneWorld[iBone];
	half4 q = g_BufferBoneWorld[iBone + 1];
	Output.vPosition = half4((SkinRotateVec(q, v.vPosition) + bpos) * weight, 1.0);
	Output.vNormal = SkinRotateVec(q, v.vNormal) * weight;

	return(Output);
}


VSO_SceneCommon SkinAllTransform(VSI_Animation v)
{
	VSO_SceneCommon Output = (VSO_SceneCommon)0;
	VSO_SceneCommon o = (VSO_SceneCommon)0;
	
	//Bone0
    uint iBone = v.vBones.x;
    float fWeight = v.vWeights.x;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.vPosition += o.vPosition;
	Output.vNormal += o.vNormal;
    
    //Bone1
    iBone = v.vBones.y;
    fWeight = v.vWeights.y;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.vPosition += o.vPosition;
	Output.vNormal += o.vNormal;
    
    //Bone2
    iBone = v.vBones.z;
    fWeight = v.vWeights.z;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.vPosition += o.vPosition;
	Output.vNormal += o.vNormal;
    
    //Bone3
    iBone = v.vBones.w;
    fWeight = v.vWeights.w;
	o = SkinBoneTransform(iBone, fWeight, v);
	Output.vPosition += o.vPosition;
	Output.vNormal += o.vNormal;

	Output.vPosition.w = 1.0;
	
	return(Output);
}