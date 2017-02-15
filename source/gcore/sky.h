
#ifndef __sky
#define __sky

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
	void ChangeTexture(const char *texture);
	inline void SetRotation(float angle);
	inline float GetRotation();
	inline void SetColor(float4_t* color);
	inline void GetColor(float4_t* color);
	void Render(float timeDelta,float3* pos,bool is_shadow);

	inline void SetStdPath(const char* path);
	inline void GetStdPath(char* path);

protected:

	IDirect3DVertexDeclaration9* VertexDeclarationSkyBox;
	char StdPath[1024];
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

	DWORD VS_RenderSkyBox;
	DWORD PS_RenderSkyBox;
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
	void ChangeTexture(const char *texture);

	inline void SetRotation(float angle);
	inline float GetRotation();
	inline void SetAlpha(float alpha);
	inline float GetAlpha();
	inline void SetColor(float4_t* color);
	inline void GetColor(float4_t* color);

	inline void SetStdPath(const char* path);
	inline void GetStdPath(char* path);

	void Render(DWORD timeDetlta,float3* pos,bool is_shadow);

private:

	IDirect3DVertexDeclaration9* VertexDeclarationClouds;
	char StdPath[1024];
	float Alpha;
	float RotaionY;
	float4x4 MatRotation;
	float4_t Color;

	float FactorBlend;
	float2_t WidthHeight;
	bool BFChange;
	bool BFChangeMainTex;

	float Bias;
	SkyCloudsVertex* Vertices;
	IDirect3DVertexBuffer9*	SkyCloudsVertices;
	IDirect3DIndexBuffer9*  SkyCloudsIndeces;

	IDirect3DTexture9*	SkyCloudsTex;
	IDirect3DTexture9*	SkyCloudsTex2;

	DWORD VS_RenderSkyClouds;
	DWORD PS_RenderSkyClouds;

	DWORD PS_RenderSkyCloudsShadow;
};

#endif