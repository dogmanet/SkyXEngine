
#include <../struct.h>

half4x4	WorldViewProjection : register(c1);
half4x4	World : register(c5);

half4 g_BufferBoneWorld[128] : register(c16);

half3 Rotate(half4 q, half3 p)
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

vs_out_gcommon Transform(uint iBone, half weight, vs_in_anim v)
{
	vs_out_gcommon Output = (vs_out_gcommon)0;
	iBone *= 2;
	half4 bpos = g_BufferBoneWorld[iBone];
	half4 q = g_BufferBoneWorld[iBone + 1];
	Output.Position = half4((Rotate(q, v.Pos) + bpos) * weight, 1.0);
	Output.Normal = Rotate(q, v.Norm) * weight;

	return(Output);
}



vs_out_gcommon main(vs_in_anim input)
{
	vs_out_gcommon Output = (vs_out_gcommon)0;
	
	vs_out_gcommon o;
	
	//Bone0
    uint iBone = input.Bones.x;
    float fWeight = input.Weights.x;
	o = Transform(iBone, fWeight, input);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
    
    //Bone1
    iBone = input.Bones.y;
    fWeight = input.Weights.y;
	o = Transform(iBone, fWeight, input);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
    
    //Bone2
    iBone = input.Bones.z;
    fWeight = input.Weights.z;
	o = Transform(iBone, fWeight, input);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
    
    //Bone3
    iBone = input.Bones.w;
    fWeight = input.Weights.w;
	o = Transform(iBone, fWeight, input);
	Output.Position += o.Position;
	Output.Normal += o.Normal;
	
	Output.Position.w = 1.0;
	
	Output.Position = mul(Output.Position, WorldViewProjection);
	Output.Normal = mul(Output.Normal, World);
	Output.TexUV = input.Tex;
	
	Output.Pos = Output.Position;
    
	return(Output);
}
