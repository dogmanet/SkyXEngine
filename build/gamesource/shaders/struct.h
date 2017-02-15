
struct vs_out_res_pos
{
	half4 Position	:POSITION0;
	half2 TexUV		:TEXCOORD0;
	
	half3 EyeRay	:TEXCOORD1;
	half3 WorldRay	:TEXCOORD2;
};

struct vs_in_pp
{
	float4 Position	:POSITION;
	float2 TexUV	:TEXCOORD0;
};

struct vs_out_pp
{
	float4 Position	:POSITION0;
	float2 TexUV	:TEXCOORD0;
};

struct vs_in_green 
{
	float3 Position :POSITION0;
	float3 Normal	:NORMAL0;
	float2 TexUV : TEXCOORD0;
	float3 InstancePos :TEXCOORD1;
	float3 InstanceTrans :TEXCOORD2;
};

struct vs_out_green 
{
	float4 Position	:POSITION0;
	float2 TexUV	:TEXCOORD0;
	float3 Normal	:TEXCOORD1;
	float4 Pos		:TEXCOORD2;
};

struct vs_in_geom
{
	float3 Position	:POSITION0;
	float2 TexUV	:TEXCOORD0;
	float3 Normal	:NORMAL0;
};

struct ps_out_ds_mrt 
{
	float4 Color		:COLOR0;
	float4 Normal		:COLOR1;
	float4 Param		:COLOR2;
	float4 Depth		:COLOR3;
};

struct ps_out_com_lighting
{
	float4 Amdient	:COLOR0;
	float4 Spec	:COLOR1;
};

struct vs_in_skybox
{
	float4 Position	:POSITION0;
	float3 TexUV	:TEXCOORD0;
};

struct vs_out_skybox
{
	float4 Position	:POSITION0;
	float3 TexUV	:TEXCOORD0;
};

struct vs_in_skyclouds
{
	float4 Position	:POSITION0;
	float2 TexUV	:TEXCOORD0;
};

struct vs_out_skyclouds
{
	float4 Position	:POSITION;
	float2 TexUV	:TEXCOORD0;
};