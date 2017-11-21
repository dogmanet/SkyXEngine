
#ifndef __SKY_H
#define __SKY_H

#include <gdefines.h>
#include <d3d9.h>
#include <common/array.h>
#include <common/String.h>
#include "sxgcore.h"

extern report_func g_fnReportf;
extern IDirect3DDevice9* DXDevice;
extern D3DPRESENT_PARAMETERS D3DAPP;

struct SkyBoxVertex
{
	SkyBoxVertex(float x,float y,float z,float u,float v,float w)
	{
		_x  = x;  _y  = y;  _z  = z; _u = u; _v = v; _w = w;
	}
	float _x, _y, _z;
	float _u, _v, _w;
};

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void LoadTextures(const char *texture);
	bool IsLoadTex();
	void ChangeTexture(const char *texture);
	void GetActiveTexture(char *texture);
	void GetSecondTexture(char *texture);
	void SetRotation(float angle);
	float GetRotation();
	void SetColor(float4_t* color);
	void GetColor(float4_t* color);
	void Render(float timeDelta,float3* pos,bool is_shadow);

	
	SX_ALIGNED_OP_MEM

protected:

	IDirect3DVertexDeclaration9* VertexDeclarationSkyBox;
	
	char TexActive[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	char TexSecond[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	float RotaionY;
	float4x4 MatRotation;
	float4_t Color;
	float FactorBlend;

	bool BFChange;
	bool BFChangeMainTex;

	IDirect3DCubeTexture9*	Tex;
	IDirect3DCubeTexture9*	Tex2;
	IDirect3DVertexBuffer9*	Vertices;
	IDirect3DIndexBuffer9*  Indeces;

	ID VS_RenderSkyBox;
	ID PS_RenderSkyBox;
};


struct SkyCloudsVertex
{
	SkyCloudsVertex(float x,float y,float z,float u,float v)
	{
	_x  = x;  _y  = y;  _z  = z; _u = u; _v = v;
	}
	float _x, _y, _z;
	float _u, _v;
};


class SkyClouds
{
public:
	SkyClouds();
	~SkyClouds();

	//установить параметры облаков
	//если облака отбрасывают тень, то надо шобы облака покрывали почти весь уровень
	void SetWidthHeightPos(float width,float height,float3* pos);
	void LoadTextures(const char *texture);
	bool IsLoadTex();
	void ChangeTexture(const char *texture);

	void SetRotation(float angle);
	float GetRotation();
	void SetAlpha(float alpha);
	float GetAlpha();
	void SetColor(float4_t* color);
	void GetColor(float4_t* color);

	void SetSpeed(float speed);
	float GetSpeed();

	void Render(DWORD timeDetlta,float3* pos,bool is_shadow);

	SX_ALIGNED_OP_MEM
private:

	IDirect3DVertexDeclaration9* VertexDeclarationClouds;
	float Alpha;
	float RotaionY;
	float4x4 MatRotation;
	float4_t Color;

	float FactorBlend;
	float2_t WidthHeight;
	bool BFChange;
	bool BFChangeMainTex;

	float Speed;
	float Bias;
	SkyCloudsVertex* Vertices;
	IDirect3DVertexBuffer9*	SkyCloudsVertices;
	IDirect3DIndexBuffer9*  SkyCloudsIndeces;

	IDirect3DTexture9*	SkyCloudsTex;
	IDirect3DTexture9*	SkyCloudsTex2;

	ID VS_RenderSkyClouds;
	ID PS_RenderSkyClouds;

	ID PS_RenderSkyCloudsShadow;
};

#endif
