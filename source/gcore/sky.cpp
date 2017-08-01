
#include <gcore\sky.h>

SkyBox::SkyBox()
{
	TexActive[0] = 0;
	TexSecond[0] = 0;

	D3DVERTEXELEMENT9 layoutskybox[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	DXDevice->CreateVertexDeclaration(layoutskybox, &VertexDeclarationSkyBox);

	VS_RenderSkyBox = SGCore_ShaderLoad(ShaderType::st_vertex, "sky_box.vs", "sky_box", ShaderCheckDouble::scd_name);
	PS_RenderSkyBox = SGCore_ShaderLoad(ShaderType::st_pixel, "sky_box.ps", "sky_box", ShaderCheckDouble::scd_name);

	StdPath[0] = 0;
	Color = float4(0,0,0,0);
	RotaionY = 0.f;
	MatRotation = SMMatrixIdentity();
	DXDevice->CreateVertexBuffer(
								8 * sizeof(SkyBoxVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&Vertices,
								0
								);

	DXDevice->CreateIndexBuffer(
               36 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
               &Indeces,
               0);

	float X = 1.0f * 200;
	float Y = 1.0f * 200;
	float Z = 1.0f * 200;
	float tmpy = 45;

	SkyBoxVertex* tmpVertices;
	Vertices->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxVertex( X,  Y-tmpy, Z, 1.0f, 1.0f, 1.0f);
	tmpVertices[1] = SkyBoxVertex(-X,  Y-tmpy, Z,-1.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxVertex( X, -tmpy, Z, 1.0f,-1.0f, 1.0f);

	tmpVertices[3] = SkyBoxVertex( X,  Y-tmpy,-Z, 1.0f, 1.0f,-1.0f);
	tmpVertices[4] = SkyBoxVertex(-X, -tmpy, Z,-1.0f,-1.0f, 1.0f);
	tmpVertices[5] = SkyBoxVertex( X, -tmpy,-Z, 1.0f,-1.0f,-1.0f);

	tmpVertices[6] = SkyBoxVertex(-X,  Y-tmpy,-Z,-1.0f, 1.0f,-1.0f);
	tmpVertices[7] = SkyBoxVertex(-X, -tmpy,-Z,-1.0f,-1.0f,-1.0f);

	Vertices->Unlock();


	WORD indices_tmp[] =
    {
        4,1,0,
        2,4,0,

        7,5,3,
        6,7,3,

        2,0,3,
        5,2,3,

		7,6,1,
		4,7,1,

		6,3,0,
		1,6,0,

		7,4,2,
		5,7,2,
    };

	WORD* indices = 0;
	Indeces->Lock(0, 0, (void**)&indices, 0);

	memcpy(indices,indices_tmp,36 * sizeof(WORD));

	Indeces->Unlock();

	Tex = 0;
	Tex2 = 0;

	BFChange = false;
	BFChangeMainTex = false;
	FactorBlend = 0.0f;
};

SkyBox::~SkyBox()
{
	mem_release_del(Vertices);
	mem_release_del(Indeces);

	mem_release_del(Tex);
	mem_release_del(Tex2);

	mem_release_del(VertexDeclarationSkyBox);
}

void SkyBox::SetStdPath(const char* path)
{
	if (path)
		sprintf(StdPath, "%s", path);
}

void SkyBox::GetStdPath(char* path)
{
	if (path)
		sprintf(path, "%s", StdPath);
}

void SkyBox::LoadTextures(const char *texture)
{
	mem_release_del(Tex);
	if (!texture)
	{
		mem_release_del(Tex2);

		BFChange = false;
		BFChangeMainTex = false;
		FactorBlend = 0.0f;

		return;
	}

	char tmppath[1024];
	sprintf(tmppath, "%s%s", StdPath, texture);
	if (FAILED(D3DXCreateCubeTextureFromFile(DXDevice, tmppath, &Tex)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "[SGCORE] %s - failed load cube texture '%s'", gen_msg_location, tmppath);
	}
}

bool SkyBox::IsLoadTex()
{
	return (Tex != 0 || Tex2 != 0);
}

void SkyBox::ChangeTexture(const char *texture)
{
	mem_release_del((BFChangeMainTex ? Tex : Tex2));
	char tmpsb1[1024];
	sprintf(tmpsb1,"%s%s",StdPath,texture);
		if(!FAILED(D3DXCreateCubeTextureFromFile(DXDevice,tmpsb1,(BFChangeMainTex ? &Tex : &Tex2))))
			BFChange = true;
		else
		{
			reportf(REPORT_MSG_LEVEL_ERROR, "[SGCORE] %s - failed load cube texture '%s'", gen_msg_location, tmpsb1);
		}
}

void SkyBox::GetActiveTexture(char *texture)
{
	if (texture)
		strcpy(texture, TexActive);
}

void SkyBox::GetSecondTexture(char *texture)
{
	if (texture)
		strcpy(texture, TexSecond);
}

inline void SkyBox::SetRotation(float angle)
{
	RotaionY = angle;
	MatRotation = SMMatrixRotationY(RotaionY);
}

inline float SkyBox::GetRotation()
{
	return RotaionY;
}

inline void SkyBox::SetColor(float4_t* color)
{
	Color = *color;
}

inline void SkyBox::GetColor(float4_t* color)
{
	*color = Color;
}

void SkyBox::Render(float timeDelta,float3* pos,bool is_shadow)
{
		if(BFChange)
			FactorBlend += timeDelta * 0.001f * 0.2f;

	float4x4 World = MatRotation * SMMatrixTranslation(pos->x, pos->y, pos->z);

	//D3DXMATRIX tmpdxView, tmpdxProjection;
	//DXDevice->GetTransform(D3DTS_PROJECTION, &tmpdxProjection);
	//DXDevice->GetTransform(D3DTS_VIEW, &tmpdxView);
	float4x4 View;// = float4x4(tmpdxView);
	float4x4 Proj;// = float4x4(tmpdxProjection);

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &View);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &Proj);

	float4x4 WVP = World * View * Proj;

	WVP = SMMatrixTranspose(WVP);

		if(BFChange && FactorBlend >= 1.0)
		{
			FactorBlend = 0.0;
			BFChangeMainTex = !BFChangeMainTex;
			BFChange = false;
		}

		if(BFChangeMainTex)
		{
			DXDevice->SetTexture(0,Tex2);
			DXDevice->SetTexture(1,Tex);
		}
		else
		{
			DXDevice->SetTexture(0,Tex);
			DXDevice->SetTexture(1,Tex2);
		}

	SGCore_ShaderSetVRF(ShaderType::st_vertex, VS_RenderSkyBox, "WorldViewProjection", &WVP);
	SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyBox, "Color", &Color);
	SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyBox, "BlendFactor", &FactorBlend);
	SGCore_ShaderBind(ShaderType::st_vertex, VS_RenderSkyBox);
	SGCore_ShaderBind(ShaderType::st_pixel, PS_RenderSkyBox);

	DXDevice->SetStreamSource(0, Vertices, 0, sizeof(SkyBoxVertex));
	DXDevice->SetIndices(Indeces);
	DXDevice->SetVertexDeclaration(VertexDeclarationSkyBox);
	DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	SGCore_ShaderUnBind();
};

/////

SkyClouds::SkyClouds()
{
	D3DVERTEXELEMENT9 layoutclouds[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	DXDevice->CreateVertexDeclaration(layoutclouds, &VertexDeclarationClouds);

	VS_RenderSkyClouds = SGCore_ShaderLoad(ShaderType::st_vertex, "sky_clouds.vs", "sky_clouds", ShaderCheckDouble::scd_name);
	PS_RenderSkyClouds = SGCore_ShaderLoad(ShaderType::st_pixel, "sky_clouds.ps", "sky_clouds", ShaderCheckDouble::scd_name);

	D3DXMACRO Defines_SHADOW[] = { { "SHADOW", "" }, { 0, 0 } };
	PS_RenderSkyCloudsShadow = SGCore_ShaderLoad(ShaderType::st_pixel, "sky_clouds.ps", "sky_clouds_shadow", ShaderCheckDouble::scd_name, Defines_SHADOW);

	StdPath[0] = 0;
	RotaionY = 0;
	Alpha = 1.f;
	Color = float4_t(0,0,0,0);
	MatRotation = SMMatrixIdentity();
	DXDevice->CreateVertexBuffer(
								4 * sizeof(SkyCloudsVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&SkyCloudsVertices,
								0
								);

	DXDevice->CreateIndexBuffer(
               6 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
               &SkyCloudsIndeces,
               0);

	float X = 800;
	float Y = 0;
	float Z = 800;

	SkyCloudsVertices->Lock(0, 0, (void**)&Vertices, 0);

	Vertices[3] = SkyCloudsVertex(-X,  Y, -Z, 0.0f, 2.0f);
	Vertices[2] = SkyCloudsVertex(-X,  Y,  Z, 0.0f, 0.0f);
	Vertices[1] = SkyCloudsVertex( X,  Y,  Z, 2.0f,0.0f);
	Vertices[0] = SkyCloudsVertex( X,  Y, -Z, 2.0f, 2.0f);

	SkyCloudsVertices->Unlock();


	WORD indices_tmp[] =
    {
        0,1,2,
        0,2,3
    };


	WORD* indices = 0;
	SkyCloudsIndeces->Lock(0, 0, (void**)&indices, 0);

	memcpy(indices,indices_tmp,6 * sizeof(WORD));

	SkyCloudsIndeces->Unlock();

	Bias = 0.f;
	Speed = 0.01f;

	SkyCloudsTex = 0;
	SkyCloudsTex2 = 0;

	BFChange = false;
	BFChangeMainTex = false;
	FactorBlend = 0.0f;
}

SkyClouds::~SkyClouds()
{
	mem_release_del(SkyCloudsTex);
	mem_release_del(SkyCloudsTex2);

	mem_release_del(SkyCloudsVertices);
	mem_release_del(SkyCloudsIndeces);

	mem_release_del(VertexDeclarationClouds);
}

void SkyClouds::ChangeTexture(const char *texture)
{
	mem_release_del((BFChangeMainTex ? SkyCloudsTex : SkyCloudsTex2));
	char tmpsb1[1024];
	sprintf(tmpsb1,"%s%s",StdPath,texture);
		if(!FAILED(D3DXCreateTextureFromFile(DXDevice,tmpsb1,(BFChangeMainTex ? &SkyCloudsTex : &SkyCloudsTex2))))
			BFChange = true;
		else
		{
			reportf(REPORT_MSG_LEVEL_ERROR, "[SGCORE] %s - failed load texture '%s'", gen_msg_location, tmpsb1);
		}
}

void SkyClouds::SetWidthHeightPos(float width,float height,float3* pos)
{
	SkyCloudsVertices->Lock(0, 0, (void**)&Vertices, 0);

	float X = pos->x + (width*0.5f);
	float Y = pos->y;
	float Z = pos->z + (height*0.5f);

	WidthHeight.x = width;
	WidthHeight.y = height;

	//float2_t tmpwh = float2_t(width*0.5f,height*0.5f);

	Vertices[3] = SkyCloudsVertex(-X,  Y, -Z, 0.0f, 2.0f);
	Vertices[2] = SkyCloudsVertex(-X,  Y,  Z, 0.0f, 0.0f);
	Vertices[1] = SkyCloudsVertex( X,  Y,  Z, 2.0f,0.0f);
	Vertices[0] = SkyCloudsVertex( X,  Y, -Z, 2.0f, 2.0f);

	SkyCloudsVertices->Unlock();
}

inline void SkyClouds::SetRotation(float angle)
{
	RotaionY = angle;
	MatRotation = SMMatrixRotationY(RotaionY);
}

inline float SkyClouds::GetRotation()
{
	return RotaionY;
}

inline void SkyClouds::SetAlpha(float alpha)
{
	Alpha = alpha;
}

inline float SkyClouds::GetAlpha()
{
	return Alpha;
}

inline void SkyClouds::SetColor(float4_t* color)
{
	Color = *color;
}

inline void SkyClouds::GetColor(float4_t* color)
{
	*color = Color;
}

void SkyClouds::SetSpeed(float speed)
{
	Speed = speed;
}

float SkyClouds::GetSpeed()
{
	return Speed;
}

void SkyClouds::SetStdPath(const char* path)
{
	if (path)
		sprintf(StdPath, "%s", path);
}

void SkyClouds::GetStdPath(char* path)
{
	if (path)
		sprintf(path, "%s", StdPath);
}

void SkyClouds::LoadTextures(const char *texture)
{
	mem_release_del(SkyCloudsTex);
	if (!texture)
	{
		mem_release_del(SkyCloudsTex2);

		BFChange = false;
		BFChangeMainTex = false;
		FactorBlend = 0.0f;

		return;
	}

	char tmppath[1024];
	sprintf(tmppath,"%s%s",StdPath,texture);
	if (FAILED(D3DXCreateTextureFromFile(DXDevice, tmppath, &SkyCloudsTex)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "[SGCORE] %s - failed load texture '%s'", gen_msg_location, tmppath);
	}
}

bool SkyClouds::IsLoadTex()
{
	return (SkyCloudsTex != 0 || SkyCloudsTex2 != 0);
}

void SkyClouds::Render(DWORD timeDelta,float3* pos,bool is_shadow)
{
		if(BFChange)
			FactorBlend += timeDelta * 0.001f * 0.2f;

	Bias += timeDelta * 0.001f * Speed;

		if(Bias >= 2.f)
			Bias = 0.f;

	//float4x4 World = SMMatrixTranslation(Core::Data::ConstCurrentCameraPosition.x, Core::Data::ConstCurrentCameraPosition.y+50, Core::Data::ConstCurrentCameraPosition.z);
	//float4x4 World = SMMatrixTranslation(pos->x, pos->y+50, pos->z);
	float4x4 World = SMMatrixTranslation(0, pos->y, 0);

		if(BFChange && FactorBlend >= 1.0)
		{
			FactorBlend = 0.0;
			BFChangeMainTex = !BFChangeMainTex;
			BFChange = false;
		}

		if(BFChangeMainTex)
		{
			DXDevice->SetTexture(0,SkyCloudsTex2);
			DXDevice->SetTexture(1,SkyCloudsTex);
		}
		else
		{
			DXDevice->SetTexture(0,SkyCloudsTex);
			DXDevice->SetTexture(1,SkyCloudsTex2);
		}

		if(!is_shadow)
		{
			/*D3DXMATRIX tmpdxView, tmpdxProjection;
			DXDevice->GetTransform(D3DTS_PROJECTION, &tmpdxProjection);
			DXDevice->GetTransform(D3DTS_VIEW, &tmpdxView);*/
			float4x4 View;// = float4x4(tmpdxView);
			float4x4 Proj;// = float4x4(tmpdxProjection);

			Core_RMatrixGet(G_RI_MATRIX_VIEW, &View);
			Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &Proj);

			float4x4 WVP = (MatRotation * World) * View * Proj;

			WVP = SMMatrixTranspose(WVP);

			SGCore_ShaderSetVRF(ShaderType::st_vertex, VS_RenderSkyClouds, "WorldViewProjection", &WVP);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyClouds, "BlendFactorBias", &float2(FactorBlend, Bias));
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyClouds, "Color", &Color);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyClouds, "Alpha", &Alpha);
			SGCore_ShaderBind(ShaderType::st_vertex, VS_RenderSkyClouds);
			SGCore_ShaderBind(ShaderType::st_pixel, PS_RenderSkyClouds);
		}
		else
		{
			//D3DXMATRIX tmpdxViewProj;
			//DXDevice->GetTransform(D3DTS_WORLD1, &tmpdxViewProj);
			float4x4 ViewProj;// = float4x4(tmpdxViewProj);
			Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &ViewProj);
			float4x4 WVP = (MatRotation * World) * ViewProj;

			WVP = SMMatrixTranspose(WVP);

			SGCore_ShaderSetVRF(ShaderType::st_vertex, VS_RenderSkyClouds, "WorldViewProjection", &WVP);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyCloudsShadow, "BlendFactorBias", &float2(FactorBlend, Bias));
			//SGCore_ShaderSetVRF(1, PS_RenderSkyCloudsShadow, "Color", &Color);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PS_RenderSkyCloudsShadow, "Alpha", &Alpha);
			SGCore_ShaderBind(ShaderType::st_vertex, VS_RenderSkyClouds);
			SGCore_ShaderBind(ShaderType::st_pixel, PS_RenderSkyCloudsShadow);
		}
	
	DXDevice->SetStreamSource(0, SkyCloudsVertices, 0, sizeof(SkyCloudsVertex));
	DXDevice->SetIndices(this->SkyCloudsIndeces);
	DXDevice->SetVertexDeclaration(VertexDeclarationClouds);

	DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	SGCore_ShaderUnBind();
}