
#pragma once

namespace SkyXEngine
{
namespace Graphics
{
namespace ThreeD
{


Object::Object()
{
	Scale = float3(1,1,1);
}

Object::~Object()
{

}

void Object::CalculateWorld(float4x4* world)
{
	World = SMMatrixScaling(Scale) * /*Rotation.GetMatrix()*/SMMatrixRotationX(Rotation.x) * SMMatrixRotationY(Rotation.y) * SMMatrixRotationZ(Rotation.z) * SMMatrixTranslation(Position);
		if(world)
			*world = World;
}

////////////////////

Bound::Bound()
{

}

Bound::~Bound()
{

}

void Bound::CalculateBound(IDirect3DVertexBuffer9* vertex_buffer,DWORD count_vert,DWORD bytepervert)
{
	BYTE *V = 0;
	HRESULT hr = 0;
	
		if(SUCCEEDED(vertex_buffer->Lock(0,0, (void **)&V,0)))
		{
			float3_t tmppos = *(float3_t*)((char*)(V) + bytepervert * 0);
			Max = tmppos;
			Min = tmppos;

				for(DWORD i=0;i<count_vert;i++)
				{
					float3_t pos = *(float3_t*)((char*)(V) + bytepervert * i);

						if(pos.x > Max.x)
							Max.x = pos.x;

						if(pos.y > Max.y)
							Max.y = pos.y;

						if(pos.z > Max.z)
							Max.z = pos.z;


						if(pos.x < Min.x)
							Min.x = pos.x;

						if(pos.y < Min.y)
							Min.y = pos.y;

						if(pos.z < Min.z)
							Min.z = pos.z;
				}
			vertex_buffer->Unlock();
		}

	Center = (Min + Max) * 0.5f;
	Radius = SMVector3Length(Center-Max);
}

void Bound::GetPosBBScreen(PosBBScreen *res,float3* campos,float3* sizemapdepth,float4x4* mat)
{
	float3 max,min;
	max = Max;
	min = Min;

	float2 mins,maxs,minmaxdepth;

	float4 point0 = float4(max.x,max.y,max.z,1.0f);
	float4 point1 = float4(max.x,max.y,min.z,1.0f);
	float4 point2 = float4(max.x,min.y,max.z,1.0f);
	float4 point3 = float4(min.x,max.y,max.z,1.0f);
	float4 point4 = float4(max.x,min.y,min.z,1.0f);
	float4 point5 = float4(min.x,min.y,max.z,1.0f);
	float4 point6 = float4(min.x,max.y,min.z,1.0f);
	float4 point7 = float4(min.x,min.y,min.z,1.0f);

	point0 = SMVector4Transform(point0,*mat);
	TRANSFORM_COORD_SCREEN2(point0,sizemapdepth);

	point1 = SMVector4Transform(point1,*mat);
	TRANSFORM_COORD_SCREEN2(point1,sizemapdepth);

	point2 = SMVector4Transform(point2,*mat);
	TRANSFORM_COORD_SCREEN2(point2,sizemapdepth);

	point3 = SMVector4Transform(point3,*mat);
	TRANSFORM_COORD_SCREEN2(point3,sizemapdepth);

	point4 = SMVector4Transform(point4,*mat);
	TRANSFORM_COORD_SCREEN2(point4,sizemapdepth);

	point5 = SMVector4Transform(point5,*mat);
	TRANSFORM_COORD_SCREEN2(point5,sizemapdepth);

	point6 = SMVector4Transform(point6,*mat);
	TRANSFORM_COORD_SCREEN2(point6,sizemapdepth);

	point7 = SMVector4Transform(point7,*mat);
	TRANSFORM_COORD_SCREEN2(point7,sizemapdepth);

	maxs.x = point0.x;
	maxs.y = point0.y;

	mins.x = point0.x;
	mins.y = point0.y;

	minmaxdepth.x = point0.z;
	minmaxdepth.y = point0.z;

		if(point1.x > maxs.x)
			maxs.x = point1.x;
		if(point2.x > maxs.x)
			maxs.x = point2.x;
		if(point3.x > maxs.x)
			maxs.x = point3.x;
		if(point4.x > maxs.x)
			maxs.x = point4.x;
		if(point5.x > maxs.x)
			maxs.x = point5.x;
		if(point6.x > maxs.x)
			maxs.x = point6.x;
		if(point7.x > maxs.x)
			maxs.x = point7.x;

		if(point1.y > maxs.y)
			maxs.y = point1.y;
		if(point2.y > maxs.y)
			maxs.y = point2.y;
		if(point3.y > maxs.y)
			maxs.y = point3.y;
		if(point4.y > maxs.y)
			maxs.y = point4.y;
		if(point5.y > maxs.y)
			maxs.y = point5.y;
		if(point6.y > maxs.y)
			maxs.y = point6.y;
		if(point7.y > maxs.y)
			maxs.y = point7.y;




		if(point1.x < mins.x)
			mins.x = point1.x;
		if(point2.x < mins.x)
			mins.x = point2.x;
		if(point3.x < mins.x)
			mins.x = point3.x;
		if(point4.x < mins.x)
			mins.x = point4.x;
		if(point5.x < mins.x)
			mins.x = point5.x;
		if(point6.x < mins.x)
			mins.x = point6.x;
		if(point7.x < mins.x)
			mins.x = point7.x;

		if(point1.y < mins.y)
			mins.y = point1.y;
		if(point2.y < mins.y)
			mins.y = point2.y;
		if(point3.y < mins.y)
			mins.y = point3.y;
		if(point4.y < mins.y)
			mins.y = point4.y;
		if(point5.y < mins.y)
			mins.y = point5.y;
		if(point6.y < mins.y)
			mins.y = point6.y;
		if(point7.y < mins.y)
			mins.y = point7.y;


		if(point1.z > minmaxdepth.y)
			minmaxdepth.y = point1.z;
		if(point2.z > minmaxdepth.y)
			minmaxdepth.y = point2.z;
		if(point3.z > minmaxdepth.y)
			minmaxdepth.y = point3.z;
		if(point4.z > minmaxdepth.y)
			minmaxdepth.y = point4.z;
		if(point5.z > minmaxdepth.y)
			minmaxdepth.y = point5.z;
		if(point6.z > minmaxdepth.y)
			minmaxdepth.y = point6.z;
		if(point7.z > minmaxdepth.y)
			minmaxdepth.y = point7.z;

		if(point1.z < minmaxdepth.x)
			minmaxdepth.x = point1.z;
		if(point2.z < minmaxdepth.x)
			minmaxdepth.x = point2.z;
		if(point3.z < minmaxdepth.x)
			minmaxdepth.x = point3.z;
		if(point4.z < minmaxdepth.x)
			minmaxdepth.x = point4.z;
		if(point5.z < minmaxdepth.x)
			minmaxdepth.x = point5.z;
		if(point6.z < minmaxdepth.x)
			minmaxdepth.x = point6.z;
		if(point7.z < minmaxdepth.x)
			minmaxdepth.x = point7.z;

	mins.x = (mins.x);
	mins.y = (mins.y);

	maxs.x = (maxs.x);
	maxs.y = (maxs.y);

		if(mins.x < 0.f)
			mins.x = 0.f;

		if(mins.y < 0.f)
			mins.y = 0.f;

		if(maxs.x > sizemapdepth->x)
			maxs.x = sizemapdepth->x;

		if(maxs.y > sizemapdepth->y)
			maxs.y = sizemapdepth->y;

	res->x = (mins.x);
	res->y = (mins.y);

	res->width = (maxs.x-mins.x);
	res->height = (maxs.y-mins.y);


	res->mindepth = minmaxdepth.x;
	res->maxdepth = minmaxdepth.y;

		if(res->mindepth < 0.f)
			res->mindepth = 0.f;

	res->IsVisible = true;
	res->IsIn = false;

		/*if(campos->x > Min.x && campos->y > Min.y && campos->z > Min.z   &&   campos->x < Max.x && campos->y < Max.y && campos->z < Max.z)
			res->IsIn = true;*/
}

void Bound::CalculateWorldAndTrans(float4x4* world)
{
	CalculateWorld(world);

	Max = SMVector3Transform(Max, World);
	Min = SMVector3Transform(Min, World);

	float3 Center = (Min + Max) * 0.5f;

	float Radius = SMVector3Length(Center-Max);
}


inline void Bound::SetMinMax(float3* min,float3* max)
{
	Min = *min; 
	Max = *max;

	/*float3 vec = (Max - Min) * 0.5f;
	Radius = sqrt(vec.x * vec.x + vec.y * vec.y + vec.x * vec.z);*/

	Center = (Min + Max) * 0.5f;
	Radius = SMVector3Length(Center-Max);
};

inline void Bound::GetMinMax(float3* min,float3* max)
{
	*min = Min; *max = Max;
};

inline void Bound::SetSphere(float3* center,float* radius)
{
	Center = *center;
	Radius = *radius;

	Min = Center - float3(Radius, Radius, Radius);
	Max = Center + float3(Radius, Radius, Radius);
};

inline void Bound::GetSphere(float3* center,float* radius)
{
	*center = Center;
	*radius = Radius;
};

inline bool Bound::IsPointInSphere(float3* point)
{
	float distsqr = SMVector3Dot(Center - *point);
		if(distsqr <= Radius*Radius)
			return true;
		else
			return false;
}

inline bool Bound::IsPointInBox(float3* point)
{
		if(point->x >= Min.x && point->y >= Min.y && point->z >= Min.z && point->x <= Max.x && point->y <= Max.y && point->z <= Max.z)
			return true;
		else
			return false;
}


/////////////////////

SkyBox::SkyBox()
{
	Color = float4(0,0,0,0);
	RotaionY = 0.f;
	MatRotation = SMMatrixIdentity();
	SkyXEngine::Core::Data::Device->CreateVertexBuffer(
								8 * sizeof(SkyBoxVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&Vertices,
								0
								);

	SkyXEngine::Core::Data::Device->CreateIndexBuffer(
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
	ndx_release(Vertices);
	ndx_release(Indeces);

	ndx_release(Tex);
	ndx_release(Tex2);
}

void SkyBox::LoadTextures(const char *texture)
{
	char tmppath[1024];
	sprintf(tmppath,"%s%s",SkyXEngine::Core::Data::Pathes::Textures,texture);
	D3DXCreateCubeTextureFromFile(SkyXEngine::Core::Data::Device,tmppath,&Tex);
};

void SkyBox::ChangeTexture(const char *texture)
{
	ndx_release((BFChangeMainTex ? Tex : Tex2));
	char tmpsb1[1024];
	sprintf(tmpsb1,"%s%s",SkyXEngine::Core::Data::Pathes::Textures,texture);
		if(!FAILED(D3DXCreateCubeTextureFromFile(SkyXEngine::Core::Data::Device,tmpsb1,(BFChangeMainTex ? &Tex : &Tex2))))
			BFChange = true;
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

	D3DXMATRIX tmpdxView, tmpdxProjection;
	SkyXEngine::Core::Data::Device->GetTransform(D3DTS_PROJECTION, &tmpdxProjection);
	SkyXEngine::Core::Data::Device->GetTransform(D3DTS_VIEW, &tmpdxView);
	float4x4 View = float4x4(tmpdxView);
	float4x4 Proj = float4x4(tmpdxProjection);

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
			SkyXEngine::Core::Data::Device->SetTexture(0,Tex2);
			SkyXEngine::Core::Data::Device->SetTexture(1,Tex);
		}
		else
		{
			SkyXEngine::Core::Data::Device->SetTexture(0,Tex);
			SkyXEngine::Core::Data::Device->SetTexture(1,Tex2);
		}

	SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(0,SkyXEngine::Core::Data::EngineID::VS_RenderSkyBox,"WorldViewProjection",&WVP);
	SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,SkyXEngine::Core::Data::EngineID::PS_RenderSkyBox,"Color",&Color);
	SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,SkyXEngine::Core::Data::EngineID::PS_RenderSkyBox,"BlendFactor",&FactorBlend);
	SkyXEngine::Core::Data::ShadersManager->Bind(0,SkyXEngine::Core::Data::EngineID::VS_RenderSkyBox);
	SkyXEngine::Core::Data::ShadersManager->Bind(1,SkyXEngine::Core::Data::EngineID::PS_RenderSkyBox);

	SkyXEngine::Core::Data::Device->SetStreamSource(0, Vertices, 0, sizeof(SkyBoxVertex));
	SkyXEngine::Core::Data::Device->SetIndices(Indeces);
	SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationSkyBox);
	SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	SkyXEngine::Core::Data::ShadersManager->UnBind();
};

////////////////////////////

SkyBoxCreator::SkyBoxCreator()
{
		for(int i=0;i<6;i++)
		{
			TexCube[i] = 0;
			TexOrient[i] = 0;
			Vertices[i] = 0;
			Indeces[i] = 0;
			ArrPathCube[i][0] = 0;
		}
}

SkyBoxCreator::~SkyBoxCreator()
{
		
}

void SkyBoxCreator::Init()
{
	CubeSurface[0] = 0;
	CubeSurface[1] = 0;
	CubeSurface[2] = 0;
	CubeSurface[3] = 0;
	CubeSurface[4] = 0;
	CubeSurface[5] = 0;

		for(int i=0;i<6;i++)
		{
				for(int k=0;k<4;k++)
				{
					Value[i][k].x = Value[i][k].y = 1;
					Value[i][k].z = 1.57f;
					Value[i][k].w = 0.5f;
				}
		}

	/*IsUse[4][0].y = 1;
	StartLerp[4][0].w = 0.2;

	IsUse[4][1].x = 1;
	StartLerp[4][1].x = 0.8;

	IsUse[4][2].y = 1;
	StartLerp[4][2].z = 0.8;

	IsUse[4][3].x = 1;
	StartLerp[4][3].y = 0.2;

	//

	IsUse[5][0].x = 1;
	StartLerp[5][0].w = 0.2;

	IsUse[5][1].x = 1;
	StartLerp[5][1].x = 0.8;

	IsUse[5][2].x = 1;
	StartLerp[5][2].z = 0.8;

	IsUse[5][3].x = 1;
	StartLerp[5][3].y = 0.2;

	//

	IsUse[0][0].x = 1;
	IsUse[0][0].y = 1;
	IsUse[0][0].z = 1;
	StartLerp[0][0].w = 0.2;

	IsUse[0][1].x = 1;
	StartLerp[0][1].x = 0.8;

	IsUse[0][2].y = 1;
	IsUse[0][2].z = 1;
	StartLerp[0][2].z = 0.8;

	IsUse[0][3].x = 1;
	StartLerp[0][3].y = 0.2;

	//

	//IsUse[1][0].x = 1;
	//IsUse[1][0].y = 1;
	IsUse[1][0].z = 1;
	StartLerp[1][0].w = 0.2;

	IsUse[1][1].x = 1;
	StartLerp[1][1].x = 0.8;

	IsUse[1][2].x = 1;
	IsUse[1][2].z = 1;
	StartLerp[1][2].z = 0.8;

	IsUse[1][3].x = 1;
	StartLerp[1][3].y = 0.2;

	///

	IsUse[3][0].y = 1;
	StartLerp[3][0].w = 0.2;

	IsUse[3][1].y = 1;
	IsUse[3][1].z = 1;
	StartLerp[3][1].x = 0.8;

	IsUse[3][2].x = 1;
	StartLerp[3][2].z = 0.8;

	IsUse[3][3].x = 1;
	IsUse[3][3].z = 1;
	StartLerp[3][3].y = 0.2;

	///

	IsUse[2][0].x = 1;
	StartLerp[2][0].w = 0.2;

	IsUse[2][1].x = 1;
	IsUse[2][1].z = 1;
	StartLerp[2][1].x = 0.8;

	IsUse[2][2].y = 1;
	StartLerp[2][2].z = 0.8;

	//IsUse[2][3].x = 1;
	IsUse[2][3].z = 1;
	StartLerp[2][3].y = 0.2;*/



	/*IsUse[4][0].y = 1;
	StartLerp[4][0].w = 0.2;

	IsUse[4][1].x = 1;
	StartLerp[4][1].x = 0.8;

	IsUse[4][2].y = 1;
	StartLerp[4][2].z = 0.8;

	IsUse[4][3].x = 1;
	StartLerp[4][3].y = 0.2;

	//

	IsUse[5][0].x = 1;
	StartLerp[5][0].w = 0.2;

	IsUse[5][1].x = 1;
	StartLerp[5][1].x = 0.8;

	IsUse[5][2].x = 1;
	StartLerp[5][2].z = 0.8;

	IsUse[5][3].x = 1;
	StartLerp[5][3].y = 0.2;

	//

	IsUse[0][0].x = 1;
	IsUse[0][0].y = 1;
	IsUse[0][0].z = 1;
	StartLerp[0][0].w = 0.2;

	IsUse[0][1].x = 1;
	StartLerp[0][1].x = 0.8;

	//IsUse[0][2].y = 1;
	IsUse[0][2].z = 1;
	StartLerp[0][2].z = 0.8;

	IsUse[0][3].x = 1;
	StartLerp[0][3].y = 0.2;

	//

	//IsUse[1][0].x = 1;
	//IsUse[1][0].y = 1;
	IsUse[1][0].z = 1;
	StartLerp[1][0].w = 0.2;

	IsUse[1][1].x = 1;
	StartLerp[1][1].x = 0.8;

	IsUse[1][2].x = 1;
	IsUse[1][2].y = 1;
	IsUse[1][2].z = 1;
	StartLerp[1][2].z = 0.8;

	IsUse[1][3].x = 1;
	StartLerp[1][3].y = 0.2;

	///

	IsUse[3][0].y = 1;
	StartLerp[3][0].w = 0.2;

	//IsUse[3][1].y = 1;
	IsUse[3][1].z = 1;
	StartLerp[3][1].x = 0.8;

	IsUse[3][2].x = 1;
	StartLerp[3][2].z = 0.8;

	IsUse[3][3].x = 1;
	IsUse[3][3].z = 1;
	StartLerp[3][3].y = 0.2;

	///

	IsUse[2][0].x = 1;
	StartLerp[2][0].w = 0.2;

	IsUse[2][1].x = 1;
	IsUse[2][1].y = 1;
	IsUse[2][1].z = 1;
	StartLerp[2][1].x = 0.8;

	IsUse[2][2].y = 1;
	StartLerp[2][2].z = 0.8;

	//IsUse[2][3].x = 1;
	IsUse[2][3].z = 1;
	StartLerp[2][3].y = 0.2;*/

	//D3DXCreateCubeTexture(Core::Data::Device, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &CubeMap);
	Core::Data::Device->CreateTexture(1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &(ArrCubeMap[0]), NULL);
	Core::Data::Device->CreateTexture(1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &(ArrCubeMap[1]), NULL);
	Core::Data::Device->CreateTexture(1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &(ArrCubeMap[2]), NULL);
	Core::Data::Device->CreateTexture(1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &(ArrCubeMap[3]), NULL);
	Core::Data::Device->CreateTexture(1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &(ArrCubeMap[4]), NULL);
	Core::Data::Device->CreateTexture(1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &(ArrCubeMap[5]), NULL);

	D3DXCreateCubeTexture(Core::Data::Device, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &CubeMap);

		for(int i=0;i<6;i++)
		{
	SkyXEngine::Core::Data::Device->CreateVertexBuffer(
								4 * sizeof(SkyBoxCreatorVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&Vertices[i],
								0
								);
		}

		for(int i=0;i<6;i++)
		{
	SkyXEngine::Core::Data::Device->CreateIndexBuffer(
               6 * sizeof(WORD),
               0,
               D3DFMT_INDEX16,
               D3DPOOL_MANAGED,
               &Indeces[i],
               0);
		}

	float X = 1.0f * 1;
	float Y = 1.0f * 1;
	float Z = 1.0f * 1;
	float tmpy = 0;

	SkyBoxCreatorVertex* tmpVertices;
	Vertices[4]->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxCreatorVertex( X,  Y, Z, 1.0f, 0.0f, 1.0f, 0.0f);
	tmpVertices[1] = SkyBoxCreatorVertex( X, -Y, Z, 1.0f, 1.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxCreatorVertex(-X, -Y, Z, 0.0f, 1.0f, 0.0f, 1.0f);
	tmpVertices[3] = SkyBoxCreatorVertex(-X,  Y, Z, 0.0f, 0.0f, 0.0f, 0.0f);

	Vertices[4]->Unlock();


	Vertices[5]->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxCreatorVertex( X,  Y, -Z, 0.0f, 0.0f, 1.0f, 0.0f);
	tmpVertices[1] = SkyBoxCreatorVertex( X, -Y, -Z, 0.0f, 1.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxCreatorVertex(-X, -Y, -Z, 1.0f, 1.0f, 0.0f, 1.0f);
	tmpVertices[3] = SkyBoxCreatorVertex(-X,  Y, -Z, 1.0f, 0.0f, 0.0f, 0.0f);

	Vertices[5]->Unlock();


	Vertices[2]->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxCreatorVertex( X,  Y,  Z, 1.0f, 1.0f, 1.0f, 0.0f);
	tmpVertices[1] = SkyBoxCreatorVertex( X,  Y, -Z, 1.0f, 0.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxCreatorVertex(-X,  Y, -Z, 0.0f, 0.0f, 0.0f, 1.0f);
	tmpVertices[3] = SkyBoxCreatorVertex(-X,  Y,  Z, 0.0f, 1.0f, 0.0f, 0.0f);

	Vertices[2]->Unlock();


	Vertices[3]->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxCreatorVertex( X, -Y,  Z, 1.0f, 0.0f, 1.0f, 0.0f);
	tmpVertices[1] = SkyBoxCreatorVertex( X, -Y, -Z, 1.0f, 1.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxCreatorVertex(-X, -Y, -Z, 0.0f, 1.0f, 0.0f, 1.0f);
	tmpVertices[3] = SkyBoxCreatorVertex(-X, -Y,  Z, 0.0f, 0.0f, 0.0f, 0.0f);

	Vertices[3]->Unlock();


	Vertices[0]->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxCreatorVertex( X,  Y, -Z, 1.0f, 0.0f, 1.0f, 0.0f);
	tmpVertices[1] = SkyBoxCreatorVertex( X, -Y, -Z, 1.0f, 1.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxCreatorVertex( X, -Y,  Z, 0.0f, 1.0f, 0.0f, 1.0f);
	tmpVertices[3] = SkyBoxCreatorVertex( X,  Y,  Z, 0.0f, 0.0f, 0.0f, 0.0f);

	Vertices[0]->Unlock();


	Vertices[1]->Lock(0, 0, (void**)&tmpVertices, 0);

	tmpVertices[0] = SkyBoxCreatorVertex(-X,  Y, -Z, 0.0f, 0.0f, 1.0f, 0.0f);
	tmpVertices[1] = SkyBoxCreatorVertex(-X, -Y, -Z, 0.0f, 1.0f, 1.0f, 1.0f);
	tmpVertices[2] = SkyBoxCreatorVertex(-X, -Y,  Z, 1.0f, 1.0f, 0.0f, 1.0f);
	tmpVertices[3] = SkyBoxCreatorVertex(-X,  Y,  Z, 1.0f, 0.0f, 0.0f, 0.0f);

	Vertices[1]->Unlock();




	WORD indices_tmp[] =
    {
        0,1,2,
		0,2,3
    };

		for(int i=0;i<6;i++)
		{
			WORD* indices = 0;
			Indeces[i]->Lock(0, 0, (void**)&indices, 0);

			memcpy(indices,indices_tmp,6 * sizeof(WORD));

			Indeces[i]->Unlock();
		}






float rad = 100;
int seg = 256;

	float ath=180.f/float(seg/2.0f); //шаг приращения угла "зета"
  float aph=360.f/float(seg); //шаг приращения угла "фи"

  
 Array<SkyBoxCreatorVertex2> av;//объявляем массив вертексов
 float3_t tmppos,tmpnorm;
  for(int i=0;i<seg+1;i++)//цикл по углу фи
  {
    for(int j=0;j<seg/2+1;j++)//цикл по углу зета
    {
		SkyBoxCreatorVertex2 v;// объявляем вертекс
      //считаем нормаль
      v._nz=-(float)(sin(j*ath*D3DX_PI/180.0f)*cos(i*aph*D3DX_PI/180.0f-D3DX_PI));
      v._nx=(float)(sin(j*ath*D3DX_PI/180.0f)*sin(i*aph*D3DX_PI/180.0f-D3DX_PI));
      v._ny=-(float)cosf(j*ath*D3DX_PI/180.0f);
      // умножая нормаль на радиус получаем координаты вертекса
		tmppos = float3_t(rad*float3(v._nx,v._ny,v._nz));
      v._x=tmppos.x;
	   v._y=tmppos.y;
	    v._z=tmppos.z;
      //считаем текс. координаты (заметь здесь я кое что исправил по сравнению со старым постом)
      v._u0=float(i)/float(seg+1); 
	  v._v0=1.0f-float(j)/float(seg/2+1);
      av.push_back(v); //добавляем посчитаный вертекс в массив
    }
  }

  // здесь мне нечего сказать кроме того что здесь я считаю индексы
  // см. IndexBuffer
  int nFaces=seg*(seg/2)*2;
  int nInds=nFaces*3;
  int st=seg/2+1;
  //DWORD *pInds=new DWORD[nInds];
  Array<DWORD> pInds;
  int k=0;
  for(int i=0;i<nFaces/2;i++)
  {
    pInds[i*6+0]=k+1;
    pInds[i*6+1]=k;
    pInds[i*6+2]=k+st;

    pInds[i*6+3]=k+st;
    pInds[i*6+4]=k+st+1;
    pInds[i*6+5]=k+1;
    if(k>0)
      if((k+2)%st==0) k++;
    k++;
  }


	SkyXEngine::Core::Data::Device->CreateVertexBuffer(
								av.size() * sizeof(SkyBoxCreatorVertex2),
								0,
								0,
								D3DPOOL_MANAGED,
								&Vertices2,
								0
								);


	Vertices2->Lock(0, 0, (void**)&tmpVertices, 0);

	memcpy(tmpVertices,&(av[0]),av.size() * sizeof(SkyBoxCreatorVertex2));

	Vertices2->Unlock();


	SkyXEngine::Core::Data::Device->CreateIndexBuffer(
               pInds.size() * sizeof(DWORD),
               0,
               D3DFMT_INDEX32,
               D3DPOOL_MANAGED,
               &Indeces2,
               0);

	CountVert = av.size();
	CountInd = pInds.size();


	DWORD* indices = 0;
	Indeces2->Lock(0, 0, (void**)&indices, 0);
	memcpy(indices,&(pInds[0]),pInds.size() * sizeof(DWORD));
	Indeces2->Unlock();
}

void SkyBoxCreator::LoadOrientTex(int cube,const char *texture)
{
		if(cube >= 0 && cube < 6)
		{
			ndx_release(TexOrient[cube]);
				if(FAILED(D3DXCreateTextureFromFile(SkyXEngine::Core::Data::Device,texture,&(TexOrient[cube]))))
					Core::InError("not found texture [%s]\n",texture);
		}
}

void SkyBoxCreator::LoadTextures(int cube,const char *texture)
{
		if(cube >= 0 && cube < 6)
		{
			ndx_release(TexCube[cube]);
			sprintf(ArrPathCube[cube],"%s",texture);
				if(FAILED(D3DXCreateTextureFromFile(SkyXEngine::Core::Data::Device,texture,&TexCube[cube])))
					Core::InError("not found texture [%s]\n",texture);
		}
}

struct int2
{
	int2(){}
	int2(int _x,int _y){this->x = _x;this->y = _y;}
	int x,y;
};

void SkyBoxCreator::CreateCubeMap()
{
	RenderInRT();
	LPDIRECT3DSURFACE9 BackBuf;
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	
		for(int i=0;i<6;i++)
		{
			float4x4 tmpview = SMMatrixLookToLH(float3(0,0,0), Core::Data::OrientedCube[i], Core::Data::UpVectorsCube[i]);

			float4x4 tmpproj = SMMatrixPerspectiveFovLH(SM_PIDIV2,1.f,0.25f,400.f);

			float4x4 wvp = SMMatrixTranspose(tmpview * tmpproj);

			
			
			CubeMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) i, 0, &CubeSurface[i] );
			//CubeMap[i]->GetSurfaceLevel(0, &RenderSurf);
			Core::Data::Device->SetRenderTarget(0, CubeSurface[i]);
			
			Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

			//Core::Data::Device->SetTransform(D3DTS_VIEW,&(View[cube].operator D3DXMATRIX()));
			//Core::Data::Device->SetTransform(D3DTS_PROJECTION,&(Proj[cube].operator D3DXMATRIX()));

			SkyXEngine::Core::Data::ShadersManager->Bind(0,"skyboxcreator");
			SkyXEngine::Core::Data::ShadersManager->Bind(1,"skyboxcreator");
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(0,"skyboxcreator","WorldViewProjection",&wvp);

				if(Vertices[i] && Indeces[i])
				{	
					SkyXEngine::Core::Data::Device->SetTexture(0,TexOrient[i]);
					SkyXEngine::Core::Data::Device->SetTexture(1,ArrCubeMap[i]);

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value1",&(Value[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value2",&(Value[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value3",&(Value[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value4",&(Value[i][3]));

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse1",&(IsUse[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse2",&(IsUse[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse3",&(IsUse[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse4",&(IsUse[i][3]));

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp1",&(StartLerp[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp2",&(StartLerp[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp3",&(StartLerp[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp4",&(StartLerp[i][3]));

						if(i == 0)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[4]);
						}
						else if(i == 1)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[5]);
						}
						else if(i == 2)
						{
							
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[1]);
						}
						else if(i == 3)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[1]);
						}
						else if(i == 4)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[1]);
						}
						else if(i == 5)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[1]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[0]);
						}

					SkyXEngine::Core::Data::Device->SetStreamSource(0, Vertices[i], 0, sizeof(SkyBoxCreatorVertex));
					SkyXEngine::Core::Data::Device->SetIndices(Indeces[i]);
					SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationSkyBoxCreator);
					SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}
			
			ndx_release(CubeSurface[i]);
		}

	Core::Data::Device->SetRenderTarget(0, BackBuf);
	ndx_release(BackBuf);

	SkyXEngine::Core::Data::ShadersManager->UnBind();
}

void SkyBoxCreator::CreateCubeMap2()
{
	LPDIRECT3DSURFACE9 BackBuf;
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	
		for(int i=0;i<6;i++)
		{
			float4x4 tmpview = SMMatrixLookToLH(float3(0,0,0), Core::Data::OrientedCube[i], Core::Data::UpVectorsCube[i]);

			float4x4 tmpproj = SMMatrixPerspectiveFovLH(SM_PIDIV2,1.f,0.25f,400.f);

			float4x4 wvp = SMMatrixTranspose(tmpview * tmpproj);

			SkyXEngine::Core::Data::Device->SetTransform(D3DTS_VIEW, &(tmpview.operator D3DXMATRIX()));
			SkyXEngine::Core::Data::Device->SetTransform(D3DTS_PROJECTION, &(tmpproj.operator D3DXMATRIX()));
			
			CubeMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) i, 0, &CubeSurface[i] );
			//CubeMap[i]->GetSurfaceLevel(0, &RenderSurf);
			Core::Data::Device->SetRenderTarget(0, CubeSurface[i]);
			
			Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

			Render2(&float3(0,0,0));
			
			ndx_release(CubeSurface[i]);
		}

	Core::Data::Device->SetRenderTarget(0, BackBuf);
	ndx_release(BackBuf);

	SkyXEngine::Core::Data::ShadersManager->UnBind();
}

void SkyBoxCreator::SaveCubeMap(const char* path)
{
	D3DXSaveTextureToFile(path,D3DXIFF_DDS,CubeMap,0);
}

void SkyBoxCreator::RenderInRT()
{
	//Core::Render::SetSamplerAddress(0,16,D3DTADDRESS_WRAP);
		for(int i=0;i<6;i++)
		{
			float4x4 tmpview = SMMatrixLookToLH(float3(0,0,0), Core::Data::OrientedCube[i], Core::Data::UpVectorsCube[i]);

			float4x4 tmpproj = SMMatrixPerspectiveFovLH(SM_PIDIV2,1.f,0.25f,400.f);

			float4x4 wvp = SMMatrixTranspose(tmpview * tmpproj);

			
			LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
			Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
			//CubeMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) i, 0, &CubeSurface[i] );
			ArrCubeMap[i]->GetSurfaceLevel(0, &RenderSurf);
			Core::Data::Device->SetRenderTarget(0, RenderSurf);
			
			Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

			//Core::Data::Device->SetTransform(D3DTS_VIEW,&(View[cube].operator D3DXMATRIX()));
			//Core::Data::Device->SetTransform(D3DTS_PROJECTION,&(Proj[cube].operator D3DXMATRIX()));

			SkyXEngine::Core::Data::ShadersManager->Bind(0,"skyboxcreator");
			SkyXEngine::Core::Data::ShadersManager->Bind(1,"skyboxcreator");
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(0,"skyboxcreator","WorldViewProjection",&wvp);

				if(Vertices[i] && Indeces[i])
				{	
					SkyXEngine::Core::Data::Device->SetTexture(0,TexOrient[i]);
					SkyXEngine::Core::Data::Device->SetTexture(1,TexCube[i]);

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value1",&(Value[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value2",&(Value[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value3",&(Value[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value4",&(Value[i][3]));

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse1",&(IsUse[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse2",&(IsUse[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse3",&(IsUse[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse4",&(IsUse[i][3]));

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp1",&(StartLerp[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp2",&(StartLerp[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp3",&(StartLerp[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp4",&(StartLerp[i][3]));

						if(i == 0)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[4]);
						}
						else if(i == 1)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[5]);
						}
						else if(i == 2)
						{
							
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[1]);
						}
						else if(i == 3)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[1]);
						}
						else if(i == 4)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,ArrCubeMap[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,ArrCubeMap[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[1]);
						}
						else if(i == 5)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,ArrCubeMap[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[1]);
							SkyXEngine::Core::Data::Device->SetTexture(4,ArrCubeMap[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[0]);
						}

					SkyXEngine::Core::Data::Device->SetStreamSource(0, Vertices[i], 0, sizeof(SkyBoxCreatorVertex));
					SkyXEngine::Core::Data::Device->SetIndices(Indeces[i]);
					SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationSkyBoxCreator);
					SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}
			Core::Data::Device->SetRenderTarget(0, BackBuf);
			ndx_release(BackBuf);
			ndx_release(RenderSurf);
		}

	SkyXEngine::Core::Data::ShadersManager->UnBind();
}

void SkyBoxCreator::Render2(float3* pos)
{
	float4x4 World = SMMatrixTranslation(pos->x, pos->y, pos->z);
	SkyXEngine::Core::Data::Device->SetTransform(D3DTS_WORLD, &(World.operator D3DXMATRIX()));
	SkyXEngine::Core::Data::ShadersManager->UnBind();

					SkyXEngine::Core::Data::Device->SetTexture(0,TexCube[0]);
					SkyXEngine::Core::Data::Device->SetTexture(1,TexCube[0]);

					SkyXEngine::Core::Data::Device->SetStreamSource(0, Vertices2, 0, sizeof(SkyBoxCreatorVertex2));
					SkyXEngine::Core::Data::Device->SetIndices(Indeces2);
					SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationSkyBoxCreator2);
					SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, CountVert, 0, CountInd/3);

	SkyXEngine::Core::Data::ShadersManager->UnBind();
}
	
void SkyBoxCreator::Render(float3* pos)
{
	RenderInRT();
	//RenderInRT2();
	float4x4 World = SMMatrixTranslation(pos->x, pos->y, pos->z);
	SkyXEngine::Core::Data::Device->SetTransform(D3DTS_WORLD, &(World.operator D3DXMATRIX()));
	SkyXEngine::Core::Data::ShadersManager->UnBind();

	/*D3DXMATRIX tmpdxView, tmpdxProjection;
	SkyXEngine::Core::Data::Device->GetTransform(D3DTS_PROJECTION, &tmpdxProjection);
	SkyXEngine::Core::Data::Device->GetTransform(D3DTS_VIEW, &tmpdxView);
	float4x4 View = float4x4(tmpdxView);
	float4x4 Proj = float4x4(tmpdxProjection);

	float4x4 WVP = World * View * Proj;

	WVP = SMMatrixTranspose(WVP);


	SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(0,"skyboxcreator","WorldViewProjection",&WVP);

	SkyXEngine::Core::Data::ShadersManager->Bind(0,"skyboxcreator");
	SkyXEngine::Core::Data::ShadersManager->Bind(1,"skyboxcreator");*/

	//Core::Render::SetSamplerAddress(0,16,D3DTADDRESS_WRAP);
		for(int i=0;i<6;i++)
		{
				if(Vertices[i] && Indeces[i])
				{	
					SkyXEngine::Core::Data::Device->SetTexture(0,ArrCubeMap[i]);
						//if(!GetAsyncKeyState(VK_NUMPAD7))
							SkyXEngine::Core::Data::Device->SetTexture(1,ArrCubeMap[i]);
						/*else
							SkyXEngine::Core::Data::Device->SetTexture(1,TexCube[i]);*/

					/*SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value1",&(Value[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value2",&(Value[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value3",&(Value[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","Value4",&(Value[i][3]));

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse1",&(IsUse[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse2",&(IsUse[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse3",&(IsUse[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","IsUse4",&(IsUse[i][3]));

					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp1",&(StartLerp[i][0]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp2",&(StartLerp[i][1]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp3",&(StartLerp[i][2]));
					SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"skyboxcreator","StartLerp4",&(StartLerp[i][3]));

						if(i == 0)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[4]);
						}
						else if(i == 1)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,TexCube[5]);
						}
						else if(i == 2)
						{
							
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[1]);
						}
						else if(i == 3)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,TexCube[4]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,TexCube[5]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[1]);
						}
						else if(i == 4)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,ArrCubeMap[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[0]);
							SkyXEngine::Core::Data::Device->SetTexture(4,ArrCubeMap[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[1]);
						}
						else if(i == 5)
						{
							SkyXEngine::Core::Data::Device->SetTexture(2,ArrCubeMap[2]);
							SkyXEngine::Core::Data::Device->SetTexture(3,ArrCubeMap[1]);
							SkyXEngine::Core::Data::Device->SetTexture(4,ArrCubeMap[3]);
							SkyXEngine::Core::Data::Device->SetTexture(5,ArrCubeMap[0]);
						}*/

					SkyXEngine::Core::Data::Device->SetStreamSource(0, Vertices[i], 0, sizeof(SkyBoxCreatorVertex));
					SkyXEngine::Core::Data::Device->SetIndices(Indeces[i]);
					SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationSkyBoxCreator);
					SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
				}
		}

	SkyXEngine::Core::Data::ShadersManager->UnBind();
}

////////////////////////////

SkyClouds::SkyClouds()
{
	RotaionY = 0;
	Alpha = 1.f;
	Color = float4_t(0,0,0,0);
	MatRotation = SMMatrixIdentity();
	SkyXEngine::Core::Data::Device->CreateVertexBuffer(
								4 * sizeof(SkyCloudsVertex),
								0,
								0,
								D3DPOOL_MANAGED,
								&SkyCloudsVertices,
								0
								);

	SkyXEngine::Core::Data::Device->CreateIndexBuffer(
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

	SkyCloudsTex = 0;
	SkyCloudsTex2 = 0;

	BFChange = false;
	BFChangeMainTex = false;
	FactorBlend = 0.0f;
}

SkyClouds::~SkyClouds()
{
	ndx_release(SkyCloudsTex);
	ndx_release(SkyCloudsTex2);

	ndx_release(SkyCloudsVertices);
	ndx_release(SkyCloudsIndeces);
}

void SkyClouds::ChangeTexture(const char *texture)
{
	ndx_release((BFChangeMainTex ? SkyCloudsTex : SkyCloudsTex2));
	char tmpsb1[1024];
	sprintf(tmpsb1,"%s%s",SkyXEngine::Core::Data::Pathes::Textures,texture);
		if(!FAILED(D3DXCreateTextureFromFile(SkyXEngine::Core::Data::Device,tmpsb1,(BFChangeMainTex ? &SkyCloudsTex : &SkyCloudsTex2))))
			BFChange = true;
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

void SkyClouds::LoadTextures(const char *texture)
{
	char tmppath[1024];
	sprintf(tmppath,"%s%s",SkyXEngine::Core::Data::Pathes::Textures,texture);
	D3DXCreateTextureFromFile(SkyXEngine::Core::Data::Device,tmppath,&SkyCloudsTex);
};

void SkyClouds::Render(DWORD timeDelta,float3* pos,bool is_shadow)
{
		if(BFChange)
			FactorBlend += timeDelta * 0.001f * 0.2f;

	Bias += timeDelta * 0.001f * 0.01;

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
			SkyXEngine::Core::Data::Device->SetTexture(0,SkyCloudsTex2);
			SkyXEngine::Core::Data::Device->SetTexture(1,SkyCloudsTex);
		}
		else
		{
			SkyXEngine::Core::Data::Device->SetTexture(0,SkyCloudsTex);
			SkyXEngine::Core::Data::Device->SetTexture(1,SkyCloudsTex2);
		}

		if(!is_shadow)
		{
			D3DXMATRIX tmpdxView, tmpdxProjection;
			SkyXEngine::Core::Data::Device->GetTransform(D3DTS_PROJECTION, &tmpdxProjection);
			SkyXEngine::Core::Data::Device->GetTransform(D3DTS_VIEW, &tmpdxView);
			float4x4 View = float4x4(tmpdxView);
			float4x4 Proj = float4x4(tmpdxProjection);

			float4x4 WVP = (MatRotation * World) * View * Proj;

			WVP = SMMatrixTranspose(WVP);

			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(0,"clouds","WorldViewProjection",&WVP);
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"clouds","BlendFactorBias",&float2(FactorBlend,Bias));
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"clouds","Color",&Color);
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"clouds","Alpha",&Alpha);
			SkyXEngine::Core::Data::ShadersManager->Bind(0,"clouds");
			SkyXEngine::Core::Data::ShadersManager->Bind(1,"clouds");
		}
		else
		{
			D3DXMATRIX tmpdxViewProj;
			SkyXEngine::Core::Data::Device->GetTransform(D3DTS_WORLD1, &tmpdxViewProj);
			float4x4 ViewProj = float4x4(tmpdxViewProj);

			float4x4 WVP = (MatRotation * World) * ViewProj;

			WVP = SMMatrixTranspose(WVP);

			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(0,"cloudsshadow","WorldViewProjection",&WVP);
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"cloudsshadow","BlendFactorBias",&float2(FactorBlend,Bias));
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"cloudsshadow","Color",&Color);
			SkyXEngine::Core::Data::ShadersManager->SetValueRegisterF(1,"cloudsshadow","Alpha",&Alpha);
			SkyXEngine::Core::Data::ShadersManager->Bind(0,"cloudsshadow");
			SkyXEngine::Core::Data::ShadersManager->Bind(1,"cloudsshadow");
		}
	
	SkyXEngine::Core::Data::Device->SetStreamSource(0, SkyCloudsVertices, 0, sizeof(SkyCloudsVertex));
	SkyXEngine::Core::Data::Device->SetIndices(this->SkyCloudsIndeces);
	SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationClouds);

	SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	SkyXEngine::Core::Data::ShadersManager->UnBind();
}

///////////////////////////

Model::Model()
{
	Lod = 0;
	IsLoded = false;
	ArrReflection = 0;

	TextureID = 0;
	ArrMaterials = 0;
	Data = 0;

	Name[0] = 0;
	//PathForModel[0] = "0\0";
	//PathForLod[0] = "0\0";
	sprintf(PathForModel,"%s","0");
	sprintf(PathForLod,"%s","0");
	IsSegmentationModel = false;
	//MatWorld = MatRot = MatPos = SMMatrixIdentity();
}

Model::Model(Model& mm)
{
	sprintf(Name,"%s",mm.Name);
	sprintf(PathForModel,"%s",mm.PathForModel);
	sprintf(PathForLod,"%s",mm.PathForLod);
	IsOpen = mm.IsOpen;
	IsLoded = mm.IsLoded;

		if(mm.Lod)
			Lod = new Model(*(mm.Lod));
		else
			Lod = 0;

	Data = new Graphics::ThreeD::DataStaticModel(*Data);

	TextureID = new DWORD[mm.Data->SubsetCount];
	memcpy(TextureID,mm.TextureID,sizeof(DWORD) * mm.Data->SubsetCount);
	AllCountPoly = mm.AllCountPoly;
	AllCountVertex = mm.AllCountVertex;

	ArrMaterials = new DWORD[mm.Data->SubsetCount];
	memcpy(ArrMaterials,mm.ArrMaterials,sizeof(DWORD) * mm.Data->SubsetCount);

	ArrReflection = new SkyXEngine::Graphics::ThreeD::RenderReflection*[mm.Data->SubsetCount];
		for(DWORD i=0;i<mm.Data->SubsetCount;i++)
		{
				if(mm.ArrReflection[i])
					ArrReflection[i] = new SkyXEngine::Graphics::ThreeD::RenderReflection(*(mm.ArrReflection[i]));
		}
}

Model::~Model()
{
	nmem_delete_a(TextureID);
	nmem_delete_a(ArrMaterials);

		for(DWORD i=0;i<Data->SubsetCount;i++)
		nmem_delete(ArrReflection[i]);

	nmem_delete_a(ArrReflection);

	nmem_delete(Data);
}

void Model::SetName(const char* name)
{
	sprintf(Name,"%s",name);
}

void Model::GetName(char* name)
{
	sprintf(name,"%s",Name);
}

void Model::OnLostDevice()
{
		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
				if(ArrReflection[i])
					ArrReflection[i]->OnLostDevice();
		}
}

void Model::OnResetDevice()
{
		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
				if(ArrReflection[i])
					ArrReflection[i]->OnResetDevice();
		}
}

bool Model::GetOpen()
{
	return IsOpen;
}

void Model::SetOpen(bool is_open)
{
	IsOpen = is_open;
}

/*inline void Model::GetMatPosition(float3* pos)
{
	*pos = Position;
}

inline void Model::GetMatRotation(float3* rot)
{
	*rot = Rotation;
}

inline void Model::SetMatPosition(float3* pos)
{
	Position = *pos;
	MatPos = SMMatrixTranslation(Position);

	MatWorld = MatRot * MatPos;
}

inline void Model::SetMatRotation(float3* rot)
{
	Rotation = *rot;
	MatRot = SMMatrixRotationX(Rotation.x) * SMMatrixRotationY(Rotation.y) * SMMatrixRotationZ(Rotation.z);

	MatWorld = MatRot * MatPos;
}

void Model::ComMatrixTrans()
{
	MatWorld = MatRot * MatPos;
}*/

void Model::Load(const char* path,bool is_open)
{
	IsOpen = is_open;
	Data = new Graphics::ThreeD::DataStaticModel;

	LoadStaticModel(path, Data);

	sprintf(PathForModel,"%s",path);

	TextureID = new DWORD[Data->SubsetCount];
	ArrMaterials = new DWORD[Data->SubsetCount];
	ArrReflection = new RenderReflection*[Data->SubsetCount];
	AllCountPoly = 0;
	AllCountVertex = 0;

	Graphics::ThreeD::vertex_static* tmpvertex;
	float3 tmpMin,tmpMax,tmppos;

		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
			char tmptex[256];
			sprintf(tmptex,"%s.dds",Data->ArrTextures[i]);
			TextureID[i] = SkyXEngine::Core::Data::LoadedTextures->AddName(tmptex);
			//Core::InLog("TextureID[%d] = %d, name = %s\n",i,TextureID[i],tmptex);
			ArrMaterials[i] = SkyXEngine::Core::Data::MaterialsManager->Load(tmptex);
			Material* mtrl = SkyXEngine::Core::Data::MaterialsManager->GetMaterial(ArrMaterials[i]);

				if(mtrl->LightParam.TypeReflect > 0)
				{
					ArrReflection[i] = new RenderReflection();

					ArrReflection[i]->Init(mtrl->LightParam.TypeReflect-1);

					Data->VertexBuffer->Lock(0,0,(void**)&tmpvertex,0);
					float3_t tmppos = tmpvertex[Data->StartVertex[i]].Pos;
					tmpMax = tmppos;
					tmpMin = tmppos;
						for(DWORD k=0;k<Data->VertexCount[i];k++)
						{
							tmppos = tmpvertex[Data->StartVertex[i]+k].Pos;	

								if(tmppos.x > tmpMax.x)
									tmpMax.x = tmppos.x;

								if(tmppos.y > tmpMax.y)
									tmpMax.y = tmppos.y;

								if(tmppos.z > tmpMax.z)
									tmpMax.z = tmppos.z;


								if(tmppos.x < tmpMin.x)
									tmpMin.x = tmppos.x;

								if(tmppos.y < tmpMin.y)
									tmpMin.y = tmppos.y;

								if(tmppos.z < tmpMin.z)
									tmpMin.z = tmppos.z;
						}

					DWORD* indeces;
					Data->IndexBuffer->Lock(0,0,(void **)&indeces,0);

					float3 tmpMM = SMVectorLerp(tmpMax,tmpMin,0.5f);
					
					//D3DXPlaneFromPoints(&ArrReflection[i]->Plane,&D3DXVECTOR3(tmpMax.x,tmpMax.y,tmpMax.z),&D3DXVECTOR3(tmpMM.x,tmpMM.y,tmpMM.z),&D3DXVECTOR3(tmpMin.x,tmpMin.y,tmpMin.z));
					/*D3DXPlaneFromPoints(&ArrReflection[i]->Plane,
										&D3DXVECTOR3(tmpvertex[indeces[Data->StartIndex[i]+2]].Pos.x,tmpvertex[indeces[Data->StartIndex[i]+2]].Pos.y,tmpvertex[indeces[Data->StartIndex[i]+2]].Pos.z),
										&D3DXVECTOR3(tmpvertex[indeces[Data->StartIndex[i]+1]].Pos.x,tmpvertex[indeces[Data->StartIndex[i]+1]].Pos.y,tmpvertex[indeces[Data->StartIndex[i]+1]].Pos.z),
										&D3DXVECTOR3(tmpvertex[indeces[Data->StartIndex[i]+0]].Pos.x,tmpvertex[indeces[Data->StartIndex[i]+0]].Pos.y,tmpvertex[indeces[Data->StartIndex[i]+0]].Pos.z));*/

						if(mtrl->LightParam.TypeReflect==1)
					D3DXPlaneFromPoints(&ArrReflection[i]->Plane,
										&D3DXVECTOR3(tmpvertex[indeces[Data->StartIndex[i]+0]].Pos.x,tmpvertex[indeces[Data->StartIndex[i]+0]].Pos.y,tmpvertex[indeces[Data->StartIndex[i]+0]].Pos.z),
										&D3DXVECTOR3(tmpvertex[indeces[Data->StartIndex[i]+1]].Pos.x,tmpvertex[indeces[Data->StartIndex[i]+1]].Pos.y,tmpvertex[indeces[Data->StartIndex[i]+1]].Pos.z),
										&D3DXVECTOR3(tmpvertex[indeces[Data->StartIndex[i]+2]].Pos.x,tmpvertex[indeces[Data->StartIndex[i]+2]].Pos.y,tmpvertex[indeces[Data->StartIndex[i]+2]].Pos.z));
					Data->VertexBuffer->Unlock();
					Data->IndexBuffer->Unlock();

					ArrReflection[i]->Position = (tmpMax + tmpMin) * 0.5;
					ArrReflection[i]->PosMin = tmpMin;
					ArrReflection[i]->PosMax = tmpMax;
				}
				else
					ArrReflection[i] = 0;

			AllCountPoly += Data->IndexCount[i] / 3;
			AllCountVertex += Data->VertexCount[i];
		}

		/*if(!IsSegmentationModel)
		{
	DWORD* tmpArrPoly = new DWORD[AllCountPoly * 3];

	DWORD* tmpindeces22 = 0;
	Data->IndexBuffer->Lock(0, 0, (void**)&tmpindeces22, 0);
	DWORD tmpCountNewPoly = 0;
		for(DWORD i=0;i<Data->SubsetCount;i++)
		{
				for(DWORD k=0;k<Data->IndexCount[i];k++)
				{
					tmpArrPoly[tmpCountNewPoly] = tmpindeces22[Data->StartIndex[i]+k] + Data->StartIndex[i];
					tmpCountNewPoly++;
				}
		}
	memcpy(tmpindeces22,tmpArrPoly,AllCountPoly * 3 * sizeof(DWORD));
	Data->IndexBuffer->Unlock();
		}*/

	SkyXEngine::Core::Data::LoadedTextures->LoadTextures();
}

void Model::InitReflection(DWORD group,int type_reflect)
{
		if(type_reflect > 0 && group >= 0 && group < Data->SubsetCount)
		{
			Graphics::ThreeD::vertex_static* tmpvertex;
			float3 tmpMin,tmpMax;
			Material* mtrl = SkyXEngine::Core::Data::MaterialsManager->GetMaterial(ArrMaterials[group]);
			mtrl->LightParam.TypeReflect = type_reflect;
			ArrReflection[group] = new RenderReflection();
			ArrReflection[group]->Init(mtrl->LightParam.TypeReflect-1);

			Data->VertexBuffer->Lock(0,0,(void**)&tmpvertex,0);
			float3_t tmppos = tmpvertex[Data->StartVertex[group]].Pos;
			tmpMax = tmppos;
			tmpMin = tmppos;
				for(DWORD k=0;k<Data->VertexCount[group];k++)
				{
					tmppos = tmpvertex[Data->StartVertex[group]+k].Pos;	

						if(tmppos.x > tmpMax.x)
							tmpMax.x = tmppos.x;

						if(tmppos.y > tmpMax.y)
							tmpMax.y = tmppos.y;

						if(tmppos.z > tmpMax.z)
							tmpMax.z = tmppos.z;


						if(tmppos.x < tmpMin.x)
							tmpMin.x = tmppos.x;

						if(tmppos.y < tmpMin.y)
							tmpMin.y = tmppos.y;

						if(tmppos.z < tmpMin.z)
							tmpMin.z = tmppos.z;
				}

			DWORD* indeces;
			Data->IndexBuffer->Lock(0,0,(void **)&indeces,0);

			float3 tmpMM = SMVectorLerp(tmpMax,tmpMin,0.5f);
					
			//D3DXPlaneFromPoints(&ArrReflection[i]->Plane,&D3DXVECTOR3(tmpMax.x,tmpMax.y,tmpMax.z),&D3DXVECTOR3(tmpMM.x,tmpMM.y,tmpMM.z),&D3DXVECTOR3(tmpMin.x,tmpMin.y,tmpMin.z));
			D3DXPlaneFromPoints(&ArrReflection[group]->Plane,
								&D3DXVECTOR3(tmpvertex[indeces[2]].Pos.x,tmpvertex[indeces[2]].Pos.y,tmpvertex[indeces[2]].Pos.z),
								&D3DXVECTOR3(tmpvertex[indeces[1]].Pos.x,tmpvertex[indeces[1]].Pos.y,tmpvertex[indeces[1]].Pos.z),
								&D3DXVECTOR3(tmpvertex[indeces[0]].Pos.x,tmpvertex[indeces[0]].Pos.y,tmpvertex[indeces[0]].Pos.z));
			Data->VertexBuffer->Unlock();
			Data->IndexBuffer->Unlock();

			ArrReflection[group]->Position = (tmpMax + tmpMin) * 0.5;
			ArrReflection[group]->PosMin = tmpMin;
			ArrReflection[group]->PosMax = tmpMax;
		}
}

void Model::LoadLod(const char* path)
{
		if(StrValidate(path))
		{
			sprintf(PathForLod,"%s",path);
				if(Lod==0)
				{
					Lod = new Model();
				}
				else
				{
					nmem_delete(Lod);
					Lod = new Model();
				}
			Lod->Load(path,IsOpen);
			IsLoded = true;
		}
		else
			IsLoded = false;
}

void Model::Render(int lod,DWORD timeDelta,int ismaterial,DWORD tex_selected)
{
		if(lod == 0)
		{
			//ComMatrixTrans();
			this->CalculateWorld();
			DWORD tmpcountVert = 0;
			SkyXEngine::Core::Data::Device->SetStreamSource(0, Data->VertexBuffer, 0, sizeof(Graphics::ThreeD::vertex_static));
			SkyXEngine::Core::Data::Device->SetIndices(Data->IndexBuffer);
			SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationStatic);
				
				for(DWORD i=0;i<Data->SubsetCount;i++)
				{
						if(ismaterial == 1)
							SkyXEngine::Core::Data::MaterialsManager->Render(this->ArrMaterials[i],&World,0);
						else
						{
								/*if(ismaterial == 2)
									SkyXEngine::Core::Data::MaterialsManager->Render(SkyXEngine::Core::Data::EngineID::ID_MtrlBase,&World,0);
								else*/
									Core::Data::Device->SetTransform(D3DTS_WORLD,&(World.operator D3DXMATRIX()));

							SkyXEngine::Core::Data::MaterialsManager->SetMainTexture(0,this->ArrMaterials[i]);
						}
				
						if(tex_selected != -1)
							SkyXEngine::Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(tex_selected));

						/*if(!IsSegmentationModel)
							SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, Data->StartVertex[i], 0, Data->VertexCount[i], Data->StartIndex[i], Data->IndexCount[i] / 3);
						else*/
							SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, AllCountVertex, tmpcountVert, Data->IndexCount[i] / 3);
					tmpcountVert += Data->IndexCount[i];
					Core::Data::Statistics::CountPoly += Data->IndexCount[i] / 3;
				}
		}
		else if(lod == 1 && IsLoded && Lod)
			Lod->Render(0,timeDelta,ismaterial,tex_selected);
}

////////////////////////

Light::Light()
{
	ShadowPSSM = 0; 
	ShadowSM = 0; 
	ShadowCube = 0;
	CountRenderInSec = 0;
	CountRenderShadowInSec = 0;
	IsShadow = false;

	PathRays[0];
	
	SourceLight = 0;

	PosLigthMaxValue = 1000;
	AngleY = 90;
	AngleX = 45;

	IsEnable = true;
	PathVolume[0] = 0;
	Name[0] = 0;

	Mesh = 0;
	MeshRays = 0;
	TypeLight = -1;

	Angle = 0;

	TopBottomRadius = float2_t(0,0);

	IDEffect = -1;
}

Light::Light(Light& ll)
{
	CountRenderInSec = ll.CountRenderInSec;
	CountRenderShadowInSec = ll.CountRenderShadowInSec;
	Color = ll.Color;

	TopBottomRadius = ll.TopBottomRadius;
	Angle = ll.Angle;
	TypeLight = ll.TypeLight;
	sprintf(Name,"%s",ll.Name);
	sprintf(PathVolume,"%s",ll.PathVolume);
	sprintf(PathRays,"%s",ll.PathRays);
	IsShadow = ll.IsShadow;
	IsGlobal = ll.IsGlobal;

		if(ll.Mesh)
			ll.Mesh->CloneMeshFVF(ll.Mesh->GetOptions(),ll.Mesh->GetFVF(),SkyXEngine::Core::Data::Device,&Mesh);
		else
			Mesh = 0;

		if(ll.MeshRays)
			MeshRays = new Model(*(ll.MeshRays));
		else
			MeshRays = 0;

	WorldMat = ll.WorldMat;
	DistForCamera = ll.DistForCamera;

		if(ll.SourceLight)
			SourceLight = new Model(*(ll.SourceLight));
		else
			SourceLight = 0;
	IDEffect = ll.IDEffect;

	PosLigthMaxValue = ll.PosLigthMaxValue;
	AngleY = ll.AngleY;
	AngleX = ll.AngleX;

	MatRot = ll.MatRot;

	IsTransformation = ll.IsTransformation;
	IsVisible = ll.IsVisible;
	
	IsEnable = ll.IsEnable;
	Position = ll.Position;
	Direction = ll.Direction;
	Rotation = ll.Rotation;

	Center2 = ll.Center2;

		if(ll.ShadowPSSM)
		{
			ShadowPSSM = new PSSM();
			ShadowPSSM->Init();
		}
		else
			ShadowPSSM = 0;

		if(ll.ShadowSM)
		{
			ShadowSM = new ShadowMapTech();
			ShadowSM->Init();
			ShadowSM->SetPosition(&float3(Position.x,Position.y,Position.z));
			ShadowSM->SetDirection(&Direction);
			ShadowSM->SetAngleNearFar(&float3(Angle,0.1,Position.w));
			ShadowSM->SetStatic(ll.ShadowCube->GetStatic());
		}
		else
			ShadowSM = 0;

		if(ll.ShadowCube)
		{
			ShadowCube = new ShadowMapCubeTech();
			ShadowCube->Init();
			ShadowCube->SetPosition(&float3(Position.x,Position.y,Position.z));
			ShadowCube->SetNearFar(&float2(0.1f,Position.w));
			ShadowCube->SetStatic(ll.ShadowCube->GetStatic());
		}
		else
			ShadowCube = 0;
}

Light::~Light()
{
	nmem_delete(ShadowPSSM);
	nmem_delete(ShadowSM);
	nmem_delete(ShadowCube);

	ndx_release(Mesh);
	nmem_delete(MeshRays);
}

void Light::OnLostDevice()
{
		if(ShadowPSSM)
			ShadowPSSM->OnLostDevice();
		else if(ShadowSM)
			ShadowSM->OnLostDevice();
		else if(ShadowCube)
			ShadowCube->OnLostDevice();
}

void Light::OnResetDevice()
{
		if(ShadowPSSM)
			ShadowPSSM->OnResetDevice();
		else if(ShadowSM)
			ShadowSM->OnResetDevice();
		else if(ShadowCube)
			ShadowCube->OnResetDevice();
}

inline void Light::SetEnableCubeEdge(int edge,bool enable)
{
		if(ShadowCube)
			ShadowCube->SetEnableCubeEdge(edge,enable);

	SetNullUpdateShadow();
}

inline bool Light::GetEnableCubeEdge(int edge)
{
		if(ShadowCube)
			return ShadowCube->GetEnableCubeEdge(edge);
}

inline void Light::SetBias(float bias)
{
		if(ShadowCube)
			ShadowCube->SetBias(bias);
		else if(ShadowSM)
			ShadowSM->SetBias(bias);

	SetNullUpdateShadow();
}

inline float Light::GetBias()
{
		if(ShadowCube)
			return ShadowCube->GetBias();
		else if(ShadowSM)
			return ShadowSM->GetBias();
}

inline void Light::SetShadowLocalFar(float slfar)
{
		if(ShadowCube)
		{
			float2 tmpnf;
			ShadowCube->GetNearFar(&tmpnf);
			tmpnf.y = slfar;
			ShadowCube->SetNearFar(&tmpnf);
		}
		else if(ShadowSM)
			ShadowSM->SetFar(slfar);
	SetNullUpdateShadow();
}

inline float Light::GetShadowLocalFar()
{
		if(ShadowCube)
		{
			float2 tmpnf;
			ShadowCube->GetNearFar(&tmpnf);
			return tmpnf.y;
		}
		else if(ShadowSM)
			return ShadowSM->GetFar();
		else
			return Position.w;
}

inline void Light::SetBlurPixel(float blur_pixel)
{
		if(ShadowCube)
			ShadowCube->SetBlurPixel(blur_pixel);
		else if(ShadowSM)
			ShadowSM->SetBlurPixel(blur_pixel);
		else if(ShadowPSSM)
			ShadowPSSM->SetBlurPixel(blur_pixel);

	SetNullUpdateShadow();
}

inline float Light::GetBlurPixel()
{
		if(ShadowCube)
			return ShadowCube->GetBlurPixel();
		else if(ShadowSM)
			return ShadowSM->GetBlurPixel();
		else if(ShadowPSSM)
			return ShadowPSSM->GetBlurPixel();
}

inline void Light::SetRenderGreen(bool is_render_green)
{
		if(ShadowCube)
			ShadowCube->IsRenderGreen = is_render_green;
		else if(ShadowSM)
			ShadowSM->IsRenderGreen = is_render_green;

	SetNullUpdateShadow();
}

inline bool Light::GetRenderGreen()
{
		if(ShadowCube)
			return ShadowCube->IsRenderGreen;
		else if(ShadowSM)
			return ShadowSM->IsRenderGreen;
}

inline void Light::SetRays(const char* model_rays)
{

	nmem_delete(MeshRays);
		if(StrValidate(model_rays))
		{
			MeshRays = new Model();
			MeshRays->Load(model_rays,true);
			MeshRays->SetPosition(&float3(Position.x,Position.y,Position.z));
			MeshRays->SetRotation(&Rotation);
			sprintf(PathRays,"%s",model_rays);
		}
		else
			PathRays[0] = 0;
}

void Light::SetShadowCube()
{
		if(ShadowCube)
		{
			ShadowCube = new SkyXEngine::Graphics::ThreeD::ShadowMapCubeTech();
			ShadowCube->Init();
			ShadowCube->SetPosition(&float3(Position.x,Position.y,Position.z));
			ShadowCube->SetNearFar(&float2(0.1f,Position.w));
			ShadowCube->SetStatic(false);
		}
}

void Light::SetShadowSM()
{
		if(ShadowSM)
		{
			ShadowSM = new SkyXEngine::Graphics::ThreeD::ShadowMapTech();
			ShadowSM->Init();
			ShadowSM->SetPosition(&float3(Position.x,Position.y,Position.z));
			ShadowSM->SetDirection(&Direction);
			ShadowSM->SetAngleNearFar(&float3(Angle,0.1,Position.w));
			ShadowSM->SetStatic(false);
		}
}

void Light::CreatePoint(float3* center,float radius,bool isglobal,bool is_shadowed,const char* bound_volume)
{
	IsGlobal = isglobal;
	TypeLight = 0;
		if(StrValidate(bound_volume))
		{
			//char tmppath[1024];
			//sprintf(tmppath,"%s%s",Core::Data::Pathes::Meshes,bound_volume);
			D3DXLoadMeshFromX(
            bound_volume,
            D3DXMESH_MANAGED,
			Core::Data::Device,
			0,
            0,
            0,
            0,
            &Mesh);
			sprintf(PathVolume,"%s",bound_volume);
		}
		else
		{
			D3DXCreateSphere(Core::Data::Device,radius,20,20,&Mesh,0);
		}

	//Position = float4(center->x,center->y,center->z,radius);
	

	WorldMat = SMMatrixTranslation(*center);

		if(isglobal && is_shadowed)
		{
			ShadowPSSM = new SkyXEngine::Graphics::ThreeD::PSSM();
			ShadowPSSM->Init();
			IsShadow = true;
			
		}
		else if(is_shadowed)
		{
			ShadowCube = new SkyXEngine::Graphics::ThreeD::ShadowMapCubeTech();
			ShadowCube->Init();
			ShadowCube->SetPosition(center);
			ShadowCube->SetNearFar(&float2(0.1f,radius));
			ShadowCube->SetStatic(false);
			IsShadow = true;
			
		}
		else
			IsShadow = false;
	
		if(isglobal)
			SetPosition(&float4(AngleX,AngleY,0,radius));
		else
			SetPosition(&float4(center->x,center->y,center->z,radius));
	
	
	IDirect3DVertexBuffer9* vertexbuf;
	Mesh->GetVertexBuffer(&vertexbuf);
	CalculateBound(vertexbuf,Mesh->GetNumVertices(),Mesh->GetNumBytesPerVertex());
	ndx_release(vertexbuf);
}

void Light::CreateDirection(float3* pos,float3* dir,float top_radius,float angle,float height,bool is_shadow,const char* bound_volume)
{
	IsGlobal = false;
	TypeLight = 1;
	Angle = angle;
	float bottom_radius = top_radius + height * tanf(angle/2.f);
	TopBottomRadius.x = top_radius;
	TopBottomRadius.y = bottom_radius;

		if(StrValidate(bound_volume))
		{
			//char tmppath[1024];
			//sprintf(tmppath,"%s%s",Core::Data::Pathes::Meshes,bound_volume);
			D3DXLoadMeshFromX(
            bound_volume,
            D3DXMESH_MANAGED,
			Core::Data::Device,
			0,
            0,
            0,
            0,
            &Mesh);
			sprintf(PathVolume,"%s",bound_volume);
		}
		else
			Core::WorkModel::CreateCone(top_radius,bottom_radius,height,&Mesh,Core::Data::Device);

	TopBottomRadius = float2_t(top_radius,bottom_radius);

	Direction = *dir;
	float3 f(0,-1,0);
	float3 a = SMVector3Cross(f,*dir);
	float ang = acosf(SMVector3Dot(f,*dir));
	MatRot = SMMatrixRotationAxis(a,ang);

	Rotation = SMMatrixToEuler(MatRot);

	float4x4 mpos = SMMatrixTranslation(*pos);
	WorldMat = MatRot * mpos;

	Position = float4(pos->x,pos->y,pos->z,height);
	Direction = *dir;
		if(is_shadow)
		{
			ShadowSM = new SkyXEngine::Graphics::ThreeD::ShadowMapTech();
			ShadowSM->Init();
			ShadowSM->SetPosition(pos);
			ShadowSM->SetDirection(dir);
			ShadowSM->SetAngleNearFar(&float3(angle,0.1,height));
			ShadowSM->SetStatic(false);
		}
	IsShadow = is_shadow;

	IDirect3DVertexBuffer9* vertexbuf;
	Mesh->GetVertexBuffer(&vertexbuf);
	CalculateBound(vertexbuf,Mesh->GetNumVertices(),Mesh->GetNumBytesPerVertex());
	ndx_release(vertexbuf);
}

bool Light::GetVisibleForFrustum(Core::ControllMoving::Frustum* frustum)
{
	Center2 = SMVector3Transform(this->Center,WorldMat);

		if(frustum->SphereInFrustum(&Center2,this->Radius))
			IsVisible =  true;
		else
			IsVisible = false;
	return IsVisible;
}

void Light::GetWVP(float4x4* wvp)
{
		if(ShadowSM)
		{
			*wvp = WorldMat * Core::Data::View * Core::Data::Projection;
		}
		else if(ShadowCube)
		{
			*wvp = WorldMat * Core::Data::View * Core::Data::Projection;
		}
}

IDirect3DTexture9* Light::GetShadow()
{
		if(CountRenderInSec < 4)
		{
				if(ShadowSM)
					return ShadowSM->GetShadow();
				else if(ShadowCube)
					return ShadowCube->GetShadow();
		}

		if(ShadowPSSM)
			return ShadowPSSM->GetShadow();

	return 0;
}

inline void Light::GetPosition(float4* pos)
{
		if(IsGlobal)
		{
			pos->x = AngleX;
			pos->y = AngleY;
			pos->w = Position.w;
		}
		else
			*pos = Position;
}

inline void Light::GetPositionSys(float4* pos)
{
	*pos = Position;
}

inline void Light::SetPosition(float4* pos)
{
		if(IsGlobal)
		{
			AngleX = pos->x;
			AngleY = pos->y;

			/*float OneAngle = PosLigthMaxValue / 180.0;

				if(AngleX > 360 || AngleX < 0)
					AngleX = 0;

				if(AngleY > 360 || AngleY < 0)
					AngleY = 0;

				if(AngleX <= 180 && AngleX >= 0)
					Position.x = lerp(1,-1,AngleX / 180.f);
				else
					Position.x = lerp(-1,1,(AngleX - 180.f)/180.f);
			
				if(AngleY <= 180 && AngleY >= 0)
					Position.z = lerp(1,-1,AngleY/180.f);
				else
					Position.z = lerp(-1,1,(AngleY - 180.f)/180.f);

			//Position.y = 1.f - abs((abs(Position.x) + abs(Position.z)) * 0.5f);

				/*if(AngleX > 180 || AngleY > 180)
				{
						if(AngleX > 180)
					Position.y = -Position.y;
				}*/

				if(AngleX > 360 || AngleX < 0)
					AngleX = 0;

				if(AngleY > 360 || AngleY < 0)
					AngleY = 0;

				/*if(AngleX <= 180 && AngleX >= 0)
					Position.x = lerp(1,-1,AngleX / 180.f);
				else
					Position.x = lerp(-1,1,(AngleX - 180.f)/180.f);
			
				if(AngleY <= 180 && AngleY >= 0)
					Position.z = lerp(1,-1,AngleY/180.f);
				else
					Position.z = lerp(-1,1,(AngleY - 180.f)/180.f);*/

			float4x4 mat = SMMatrixRotationZ(-D3DXToRadian(AngleX)) * SMMatrixRotationY(D3DXToRadian(AngleY));
			Position = SMVector3Transform(float3(-1,0,0),mat);


			Position.x *= PosLigthMaxValue;
			Position.y *= PosLigthMaxValue;
			Position.z *= PosLigthMaxValue;

				/*if(AngleX < 90 && AngleX > 0)
					Position.y += lerp(PosLigthMaxValue,0,90/AngleX);


				if(AngleY < 90 && AngleY > 0)
					Position.y += lerp(PosLigthMaxValue,0,90/AngleY);*/

				/*if(AngleX >= 0 && AngleX <= 90)
				{
					Position.x = OneAngle * AngleX;
					Position.z = PosLigthMaxValue - OneAngle * AngleX;
				}
				else if(AngleX >= 90 && AngleX <= 180)
				{
					Position.x = OneAngle * (180.0 - AngleX);
					Position.z = -(PosLigthMaxValue-OneAngle * (180.0 - AngleX));
				}
				else if(AngleX >= 180 && AngleX <= 270)
				{
					Position.x = -(PosLigthMaxValue-OneAngle * (270.0 - AngleX));
					Position.z = -OneAngle * (270.0 - AngleX);
				}
				else if(AngleX >= 270 && AngleX <= 360)
				{
					Position.x = -(OneAngle * (360.0 - AngleX));
					Position.z = PosLigthMaxValue - OneAngle * (360.0 - AngleX);
				}


				if(AngleY >= 0 && AngleY <= 90)
				{
					Position.y = OneAngle * AngleY;

					float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * AngleY);
					Position.x *= 1.0 - coef_ck;
					Position.z *= 1.0 - coef_ck;
				}
				else if(AngleY >= 90 && AngleY <= 180)
				{
					Position.y = OneAngle * (180.0 - AngleY);

					float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * (180.0 - AngleY));
					Position.x *= coef_ck - 1.0;
					Position.z *= coef_ck - 1.0;
				}
				else if(AngleY >= 180 && AngleY <= 270)
				{
					Position.y = -(PosLigthMaxValue-OneAngle * (270.0 - AngleY));

					float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * (270.0 - AngleY));
					Position.x *= - coef_ck;
					Position.z *= - coef_ck;
				}
				else if(AngleY >= 270 && AngleY <= 360)
				{
					Position.y = -(OneAngle * (360.0 - AngleY));

					float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * (360.0 - AngleY));
					Position.x *= 1.0 - (coef_ck);
					Position.z *= 1.0 - (coef_ck);
				}*/

			Position.w = pos->w;
			WorldMat = SMMatrixTranslation(Position.x,Position.y,Position.z);
				if(ShadowPSSM)
					ShadowPSSM->SetPosition(&float3(Position.x,Position.y,Position.z));
		}
		else
		{
			
			Position.x = pos->x;
			Position.y = pos->y;
			Position.z = pos->z;
			Position.w = pos->w;

			float4x4 mpos = SMMatrixTranslation(Position.x,Position.y,Position.z);
			WorldMat = MatRot * mpos;

				if(ShadowSM)
				{
					ShadowSM->SetPosition(&float3(Position.x,Position.y,Position.z));
				}

				if(ShadowCube)
				{
					WorldMat = SMMatrixTranslation(Position.x,Position.y,Position.z);
					ShadowCube->SetPosition(&float3(Position.x,Position.y,Position.z));
				}

				if(SourceLight)
				{
					SourceLight->SetPosition(&float3(Position.x,Position.y,Position.z));
				}

				if(MeshRays)
				{
					MeshRays->SetPosition(&float3(Position.x,Position.y,Position.z));
				}

			SetNullUpdateShadow();
		}
}

inline void Light::GetRotation(float3* rot)
{
	*rot = Rotation;
}

inline void Light::SetRotation(float3* rot)
{
	Rotation = *rot;

	Direction = SMEulerToVec(Rotation);

	float3 f(0,-1,0);
	float3 a = SMVector3Cross(f,Direction);
	float ang = acosf(SMVector3Dot(f,Direction));
	MatRot =  SMMatrixRotationAxis(a,ang);

	float4x4 mpos = SMMatrixTranslation(Position.x,Position.y,Position.z);
	WorldMat = MatRot * mpos;
	
		if(ShadowSM)
			ShadowSM->SetDirection(&Direction);

		if(SourceLight)
			SourceLight->SetRotation(&Rotation);

		if(MeshRays)
			MeshRays->SetRotation(&Rotation);

	SetNullUpdateShadow();
}

inline void Light::GetDirection(float3* dir)
{
	*dir = Direction; 
}

inline void Light::SetDirection(float3* dir)
{
	Direction = *dir;

	float3 f(0,-1,0);
	float3 a = SMVector3Cross(f,Direction);
	float ang = acosf(SMVector3Dot(f,Direction));
	MatRot =  SMMatrixRotationAxis(a,ang);

	Rotation = SMMatrixToEuler(MatRot);

	float4x4 mpos = SMMatrixTranslation(Position.x,Position.y,Position.z);
	WorldMat = MatRot * mpos;

		if(ShadowSM)
			ShadowSM->SetDirection(&Direction);

		if(SourceLight)
			SourceLight->SetRotation(&Rotation);

		if(MeshRays)
			MeshRays->SetRotation(&Rotation);

	SetNullUpdateShadow();
}

void Light::LoadSource(const char* path)
{
	nmem_delete(SourceLight);

		if(StrValidate(path))
		{
			SourceLight = new Model();
			SourceLight->Load(path,true);

			/*float4 Position;
			float3 Direction;*/
			SourceLight->SetPosition(&float3(Position.x,Position.y,Position.z));
			SourceLight->SetRotation(&Rotation);
		}
}

void Light::Render()
{
	Core::Data::Device->SetTransform(D3DTS_WORLD,&(WorldMat.operator D3DXMATRIX()));
	Mesh->DrawSubset(0);
}

void Light::RenderSource(bool is_material)
{
		if(SourceLight)
		{
				if(!is_material)
				{
					SourceLight->CalculateWorld();
					float4x4 wvp = SourceLight->World * Core::Data::View * Core::Data::ProjectionStencil;
					Core::Data::ShadersManager->SetValueRegisterF(0,"lightsource","WorldViewProjection",&SMMatrixTranspose(wvp));
					Core::Data::ShadersManager->SetValueRegisterF(0,"lightsource","World",&SMMatrixTranspose(SourceLight->World));
					Core::Data::ShadersManager->SetValueRegisterF(1,"lightsource","Color",&this->Color);
				}
			SourceLight->Render(0,0,(is_material ? 1 : 2),-1);
		}
}

void Light::ChangeVolume(const char* path)
{
		if(StrValidate(path))
		{
			ndx_release(Mesh);
			//char tmppath[1024];
			//sprintf(tmppath,"%s",path);
			D3DXLoadMeshFromX(
            path,
            D3DXMESH_MANAGED,
			Core::Data::Device,
			0,
            0,
            0,
            0,
            &Mesh);
			sprintf(PathVolume,"%s",path);

			
		}
		else
		{
				if(TypeLight == 0)
				{
					D3DXCreateSphere(Core::Data::Device,Position.w,20,20,&Mesh,0);
				}
				else
				{
					Core::WorkModel::CreateCone(this->TopBottomRadius.x,this->TopBottomRadius.y,Position.w,&Mesh,Core::Data::Device);
				}
			PathVolume[0] = 0;
		}

		if(Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			Mesh->GetVertexBuffer(&vertexbuf);
			CalculateBound(vertexbuf,Mesh->GetNumVertices(),Mesh->GetNumBytesPerVertex());
			ndx_release(vertexbuf);
		}

	SetNullUpdateShadow();
}

void Light::ChangeAngle(float angle,bool is_create)
{
	Angle = angle;
		if(TypeLight == 1 && is_create)
		{
			ndx_release(Mesh);
			TopBottomRadius.y = TopBottomRadius.x + Position.w * tanf(angle/2.f);
			Angle = angle;
			Core::WorkModel::CreateCone(TopBottomRadius.x,TopBottomRadius.y,Position.w,&Mesh,Core::Data::Device);
			PathVolume[0] = 0;
		}

		if(Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			Mesh->GetVertexBuffer(&vertexbuf);
			CalculateBound(vertexbuf,Mesh->GetNumVertices(),Mesh->GetNumBytesPerVertex());
			ndx_release(vertexbuf);
		}

		if(TypeLight == 1 && ShadowSM)
			ShadowSM->SetAngleNearFar(&float3(angle,0.1,Position.w));

	SetNullUpdateShadow();
}

void Light::ChangeTopRadius(float top_radius)
{
		if(TypeLight == 1)
		{
			ndx_release(Mesh);
			TopBottomRadius.x = top_radius;
			Core::WorkModel::CreateCone(top_radius,TopBottomRadius.y,Position.w,&Mesh,Core::Data::Device);
			PathVolume[0] = 0;
		}

		if(Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			Mesh->GetVertexBuffer(&vertexbuf);
			CalculateBound(vertexbuf,Mesh->GetNumVertices(),Mesh->GetNumBytesPerVertex());
			ndx_release(vertexbuf);
		}

	SetNullUpdateShadow();
}

void Light::ChangeRadiusHeight(float radius_height,bool is_create)
{
	Position.w = radius_height;
		if(TypeLight == 1 && is_create)
		{
			TopBottomRadius.y = TopBottomRadius.x + radius_height * tanf(Angle/2.f);
			ndx_release(Mesh);
			Core::WorkModel::CreateCone(TopBottomRadius.x,TopBottomRadius.y,radius_height,&Mesh,Core::Data::Device);
			PathVolume[0] = 0;
		}

		if(ShadowSM)
			ShadowSM->SetAngleNearFar(&float3(Angle,0.1,Position.w));
		
		if(TypeLight == 0 && is_create)
		{
			ndx_release(Mesh);
			D3DXCreateSphere(Core::Data::Device,radius_height,20,20,&Mesh,0);
			PathVolume[0] = 0;
		}

		if(ShadowCube)
			ShadowCube->SetNearFar(&float2(0.1,Position.w));

		if(Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			Mesh->GetVertexBuffer(&vertexbuf);
			CalculateBound(vertexbuf,Mesh->GetNumVertices(),Mesh->GetNumBytesPerVertex());
			ndx_release(vertexbuf);
		}

	SetNullUpdateShadow();
}

void Light::ChangeShadow(bool is_shadow)
{
	IsShadow = is_shadow;
		if(TypeLight == 1)
		{
				if(!ShadowSM && IsShadow)
				{
					ShadowSM = new SkyXEngine::Graphics::ThreeD::ShadowMapTech();
					ShadowSM->Init();
					ShadowSM->SetPosition(&float3(Position.x,Position.y,Position.z));
					ShadowSM->SetDirection(&Direction);
					ShadowSM->SetAngleNearFar(&float3(Angle,0.1,Position.w));
					ShadowSM->SetStatic(false);
				}
		}
		else if(TypeLight == 0)
		{
				if(IsGlobal)
				{
						if(!ShadowPSSM && IsShadow)
						{
							ShadowPSSM = new SkyXEngine::Graphics::ThreeD::PSSM();
							ShadowPSSM->Init();
							ShadowPSSM->SetPosition(&float3(Position.x,Position.y,Position.z));
						}
				}
				else
				{		
						if(!ShadowCube && IsShadow)
						{
							ShadowCube = new SkyXEngine::Graphics::ThreeD::ShadowMapCubeTech();
							ShadowCube->Init();
							ShadowCube->SetPosition(&float3(Position.x,Position.y,Position.z));
							ShadowCube->SetNearFar(&float2(0.1f,Position.w));
							ShadowCube->SetStatic(false);
						}
				}
		}
	SetNullUpdateShadow();
}

inline void Light::SetNullUpdateShadow()
{
		if(ShadowSM)
			ShadowSM->SetUpdate(false);
		else if(ShadowCube)
			ShadowCube->SetUpdate(false);
}

//////////////

ManagerLight::ManagerLight()
{
	NumSelected = -1;
}

inline void ManagerLight::SetEnable(DWORD id,bool enable)
{
		if(id >= 0 && Arr.size() > id)
		{
			Arr[id]->SetEnable(enable);
		}
}

void ManagerLight::RenderSelected()
{
		if(NumSelected >= 0 && Arr.size() > NumSelected && SkyXEngine::Core::Data::Settings::EditorsIsSelectedLight)
		{
			SkyXEngine::Core::Data::Device->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&(Arr[NumSelected]->WorldMat));
			SkyXEngine::Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(SkyXEngine::Core::Data::EngineID::Tex_FreeSelectedMesh));
			Arr[NumSelected]->Mesh->DrawSubset(0);
		}
}

inline bool ManagerLight::GetEnable(DWORD id)
{
		if(id >= 0 && Arr.size() > id)
		{
			return Arr[id]->GetEnable();
		}
}

inline void ManagerLight::SetSelected(DWORD id)
{
	NumSelected = id;
}

inline DWORD ManagerLight::GetSelected()
{
	return NumSelected;
}

inline DWORD ManagerLight::GetCount()
{
	return Arr.size();
}	

ManagerLight::~ManagerLight()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			nmem_delete(Arr[i]);
		}
	Arr.clear();
}

void ManagerLight::OnLostDevice()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->OnLostDevice();
		}
}

void ManagerLight::OnResetDevice()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->OnResetDevice();
		}
}

void ManagerLight::AddPoint(float3* center,float4* color,float radius,bool isglobal,bool is_shadowed,const char* bound_volume)
{
	Light* tmpLight = new SkyXEngine::Graphics::ThreeD::Light();
	tmpLight->CreatePoint(center,radius,isglobal,is_shadowed,bound_volume);
	tmpLight->Color = *color;
	Arr.push_back(tmpLight);
}

void ManagerLight::AddDirection(float3* pos,float3* dir,float4* color,float top_radius,float angle,float height,bool is_shadow,const char* bound_volume)
{
	Light* tmpLight = new SkyXEngine::Graphics::ThreeD::Light();
	tmpLight->CreateDirection(pos,dir,top_radius,angle,height,is_shadow,bound_volume);
	tmpLight->Color = *color;
	Arr.push_back(tmpLight);
}

void ManagerLight::Render()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->Render();
		}
}

void ManagerLight::Delete(DWORD id)
{
		if(id >= 0 && Arr.size() > id)
		{
			nmem_delete(Arr[id]);
			Arr.erase(id);
		}
}

inline void ManagerLight::SetName(DWORD id,const char* text)
{
		if(id >= 0 && Arr.size() > id)
		{
			sprintf(Arr[id]->Name,"%s",text);
		}
}

inline void ManagerLight::GetName(DWORD id,char* text)
{
		if(id >= 0 && Arr.size() > id)
			sprintf(text,"%s",Arr[id]->Name);
}

inline DWORD ManagerLight::GetType(DWORD id)
{
		if(id >= 0 && Arr.size() > id)
			return Arr[id]->TypeLight;
}

inline void ManagerLight::SetVolume(DWORD id,const char* text)
{
		if(id >= 0 && Arr.size() > id)
			Arr[id]->ChangeVolume(text);
}

inline void ManagerLight::GetVolume(DWORD id,char* text)
{
		if(id >= 0 && Arr.size() > id)
			sprintf(text,"%s",Arr[id]->PathVolume);
}

void ManagerLight::RenderSource(bool is_material,bool is_enable)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			if(Arr[i]->GetEnable() == is_enable)
				Arr[i]->RenderSource(is_material);
		}
}

bool ManagerLight::TraceBeam(const float3 & start, const float3 & dir, float3 * _res,DWORD* idlight)
{
	triangle tmptri;
	float dist;
	bool tmpiscom = true;
	float3 ip;
	float3 res = start + float3(10000.0f, 10000.0f, 10000.0f);
	bool found = false;
	triangle arrtri[12];

		for(DWORD i = 0; i < Arr.size() && tmpiscom; i++)
		{
			float3 tmpmin,tmpmax;
			Arr[i]->GetMinMax(&tmpmin,&tmpmax);
			tmpmin = SMVector3Transform(tmpmin,Arr[i]->WorldMat);
			tmpmax = SMVector3Transform(tmpmax,Arr[i]->WorldMat);
			arrtri[0] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z));
			arrtri[1] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmin.z));

			arrtri[2] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmax.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z));
			arrtri[3] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z));

			arrtri[4] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z));
			arrtri[5] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z),float3_t(tmpmin.x,tmpmin.y,tmpmax.z));

			arrtri[6] = triangle(float3_t(tmpmax.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z));
			arrtri[7] = triangle(float3_t(tmpmax.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z));

			arrtri[8] = triangle(float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmin.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z));
			arrtri[9] = triangle(float3_t(tmpmin.x,tmpmax.y,tmpmin.z),float3_t(tmpmax.x,tmpmax.y,tmpmax.z),float3_t(tmpmax.x,tmpmax.y,tmpmin.z));

			arrtri[10] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmin.x,tmpmin.y,tmpmax.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z));
			arrtri[11] = triangle(float3_t(tmpmin.x,tmpmin.y,tmpmin.z),float3_t(tmpmax.x,tmpmin.y,tmpmax.z),float3_t(tmpmax.x,tmpmin.y,tmpmin.z));

				for(int k=0;k<12;k++)
				{
						if(arrtri[k].IntersectLine(start, dir * 10000.0f, &ip))
						{
								if(SMVector3Length2(start - res) > SMVector3Length2(start - ip))
								{
									res = ip;
									found = true;
										if(idlight)
											*idlight = i;
								}
						}
				}
		}
		if(found && _res)
		{
			*_res = res;
		}
	return(found);
}

};
};
};