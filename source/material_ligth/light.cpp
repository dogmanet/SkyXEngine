
#include <material_ligth\\light.h>

Lights::Lights()
{
	
}

Lights::~Lights()
{

}

long Lights::CreateCopy(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		Light* tmplight = ArrLights[id];
		Light* tmplight2 = new Light();

		tmplight2->Angle = tmplight->Angle;
		tmplight2->BoundVolume = SGCore_CrBound();
		IDirect3DVertexBuffer9* vertexbuf;
		tmplight->Mesh->GetVertexBuffer(&vertexbuf);
		tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
		mem_release(vertexbuf);
		tmplight2->Color = tmplight->Color;
		tmplight2->Direction = tmplight->Direction;
		tmplight2->DistFor = -1;
		tmplight2->GAngleX = tmplight->GAngleX;
		tmplight2->GAngleY = tmplight->GAngleY;
		tmplight2->IsEnable = tmplight->IsEnable;
		tmplight2->IsGlobal = tmplight->IsGlobal;
		tmplight2->IsShadow = tmplight->IsShadow;
		tmplight2->IsVisible = tmplight->IsVisible;
		tmplight2->IsVisibleFor = false;
		tmplight2->MatRot = tmplight->MatRot;
		sprintf(tmplight2->Name, "%s", tmplight->Name);
		tmplight2->Position = tmplight->Position;
		tmplight2->Rotation = tmplight->Rotation;
		tmplight2->TopBottomRadius = tmplight->TopBottomRadius;
		tmplight2->TypeLight = tmplight->TypeLight;
		tmplight2->WorldMat = tmplight->WorldMat;

		if (tmplight->ShadowPSSM)
		{
			tmplight2->ShadowPSSM = new PSSM();
			tmplight2->ShadowPSSM->Init();
			tmplight2->GAngleX = tmplight->GAngleX;
			tmplight2->GAngleY = tmplight->GAngleY;
			tmplight2->ShadowPSSM->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			memcpy((tmplight2->ShadowPSSM->NearFar), (tmplight->ShadowPSSM->NearFar), sizeof(float2)* 5);
			memcpy((tmplight2->ShadowPSSM->IsUpdate), (tmplight->ShadowPSSM->IsUpdate), sizeof(int) * 5);
		}
		else
			tmplight2->ShadowPSSM = 0;

		if (tmplight->ShadowSM)
		{
			tmplight2->ShadowSM = new ShadowMapTech();
			tmplight2->ShadowSM->Init();
			tmplight2->ShadowSM->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowSM->SetDirection(&tmplight->Direction);
			//tmplight2->ShadowSM->SetAngleNearFar(&float3(tmplight->Angle, 0.1, tmplight->Position.w));
			tmplight2->ShadowSM->SetBias(tmplight->ShadowSM->GetBias());
			tmplight2->ShadowSM->SetBlurPixel(tmplight->ShadowSM->GetBlurPixel());
			float3 anf;
			tmplight2->ShadowSM->GetAngleNearFar(&anf);
			tmplight2->ShadowSM->SetAngleNearFar(&anf);
			//tmplight2->ShadowSM->SetStatic(ll.ShadowCube->GetStatic());
		}
		else
			tmplight2->ShadowSM = 0;

		if (tmplight->ShadowCube)
		{
			tmplight2->ShadowCube = new ShadowMapCubeTech();
			tmplight2->ShadowCube->Init();
			tmplight2->ShadowCube->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowCube->SetNearFar(&float2(0.1f, tmplight->Position.w));
			tmplight2->ShadowCube->SetNear(tmplight2->ShadowCube->GetNear());
			tmplight2->ShadowCube->SetFar(tmplight2->ShadowCube->GetFar());
			tmplight2->ShadowCube->SetBias(tmplight->ShadowCube->GetBias());
			tmplight2->ShadowCube->SetBlurPixel(tmplight->ShadowCube->GetBlurPixel());

			for (int i = 0; i < 6; ++i)
			{
				tmplight2->ShadowCube->SetEnableCubeEdge(i, tmplight2->ShadowCube->GetEnableCubeEdge(i));
			}
			//tmplight->ShadowCube->SetStatic(ll.ShadowCube->GetStatic());
		}
		else
			tmplight2->ShadowCube = 0;

		tmplight->Mesh->CloneMeshFVF(tmplight->Mesh->GetOptions(), tmplight->Mesh->GetFVF(), MLSet::DXDevice, &(tmplight2->Mesh));
	}
	return -1;
}

Lights::Light::Light()
{
	TypeLight = LIGHTS_TYPE_NONE;
	Name[0] = 0;

	IsVisible = IsEnable = IsShadow = IsGlobal = false;

	Mesh = 0;

	GAngleX = GAngleY = 0;

	Angle = 0;

	BoundVolume = 0;
	WorldMat;
	MatRot;

	IsVisibleFor = false;
	DistFor = -1;

	ShadowPSSM = 0;
	ShadowSM = 0;
	ShadowCube = 0;
}

Lights::Light::~Light()
{
	mem_release_del(Mesh);

	mem_release_del(BoundVolume);
	
	mem_delete(ShadowPSSM);
	mem_delete(ShadowSM);
	mem_delete(ShadowCube);
}

void Lights::OnLostDevice()
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->OnLostDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->OnLostDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->OnLostDevice();
	}
}

void Lights::OnResetDevice()
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->OnResetDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->OnResetDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->OnResetDevice();
	}
}

inline long Lights::AddLight(Light* obj)
{
	long idadd = -1;
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i] == 0)
		{
			ArrLights[i] = obj;
			idadd = i;
			break;
		}
	}

	if (idadd == -1)
	{
		ArrLights.push_back(obj);
		idadd = ArrLights.size() - 1;
	}

	return idadd;
}

inline long Lights::GetCountLights()
{
	return ArrLights.size();
}

void Lights::DeleteLight(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		mem_delete(ArrLights[id]);
	}
}

char* Lights::GetLightName(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		return ArrLights[id]->Name;
	}
}

void Lights::SetLightName(long id, const char* name)
{
	if (id >= 0 && id < ArrLights.size())
	{
		sprintf(ArrLights[id]->Name,"%s", name);
	}
}

long Lights::CreatePoint(float4* center, float3* color, bool isglobal, bool is_shadowed, const char* bound_volume)
{
	Light* tmplight = new Light();
	tmplight->IsGlobal = isglobal;
	tmplight->TypeLight = (isglobal ? LIGHTS_TYPE_GLOBAL : LIGHTS_TYPE_POINT);
		if(StrValidate(bound_volume))
		{
			
		}
		else
		{
			D3DXCreateSphere(MLSet::DXDevice, center->w, 20, 20, &tmplight->Mesh, 0);
		}

		if (isglobal && is_shadowed)
		{
			tmplight->ShadowPSSM = new PSSM();
			tmplight->ShadowPSSM->Init();
			tmplight->IsShadow = true;
			tmplight->ShadowPSSM->SetPosition(&float3(center->x, center->y, center->z));
		}
		else if (is_shadowed)
		{
			tmplight->ShadowCube = new ShadowMapCubeTech();
			tmplight->ShadowCube->Init();
			tmplight->ShadowCube->SetPosition(&float3(center->x, center->y, center->z));
			tmplight->ShadowCube->SetNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, center->w));
			tmplight->ShadowCube->SetStatic(false);
			tmplight->IsShadow = true;

		}
		else
			tmplight->IsShadow = false;

	long tmpid = AddLight(tmplight);

	SetLightPos(ArrLights.size()-1, &float3(center->x, center->y, center->z));

	tmplight->Position.w = center->w;
	tmplight->Color = *color;
	tmplight->WorldMat = SMMatrixTranslation(float3(center->x, center->y, center->z));
	tmplight->IsEnable = true;
	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	return tmpid;
}

long Lights::CreateDirection(float4* pos, float3* color, float3* dir, float top_radius, float angle, bool is_shadow, const char* bound_volume)
{
	Light* tmplight = new Light();
	tmplight->IsGlobal = false;
	tmplight->TypeLight = LIGHTS_TYPE_DIRECTION;
	tmplight->Angle = angle;
	float bottom_radius = top_radius + pos->w * tanf(angle / 2.f);
	tmplight->TopBottomRadius.x = top_radius;
	tmplight->TopBottomRadius.y = bottom_radius;

	if (StrValidate(bound_volume))
	{
		
	}
	else
		SGCore_FCreateCone(top_radius, bottom_radius, pos->w, &tmplight->Mesh,32);

	tmplight->TopBottomRadius = float2_t(top_radius, bottom_radius);
	tmplight->Color = *color;
	tmplight->Direction = *dir;
	float3 f(0, -1, 0);
	float3 a = SMVector3Cross(f, *dir);
	float ang = acosf(SMVector3Dot(f, *dir));
	tmplight->MatRot = SMMatrixRotationAxis(a, ang);

	tmplight->Rotation = SMMatrixToEuler(tmplight->MatRot);
	tmplight->IsEnable = true;
	float4x4 mpos = SMMatrixTranslation(*pos);
	tmplight->WorldMat = tmplight->MatRot * mpos;

	tmplight->Position = float4(pos->x, pos->y, pos->z, pos->w);
	tmplight->Direction = *dir;
	
	if (is_shadow)
	{
		tmplight->ShadowSM = new ShadowMapTech();
		tmplight->ShadowSM->Init();
		tmplight->ShadowSM->SetPosition(&float3(pos->x, pos->y, pos->z));
		tmplight->ShadowSM->SetDirection(dir);
		tmplight->ShadowSM->SetAngleNearFar(&float3(angle, LIGHTS_LOCAL_STD_NEAR, pos->w));
		tmplight->ShadowSM->SetStatic(false);
	}
	tmplight->IsShadow = is_shadow;

	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);
	
	return AddLight(tmplight);
}

void Lights::Render(long id,DWORD timeDelta)
{
	if (id >= 0 && id < ArrLights.size())
	{
		MLSet::DXDevice->SetTransform(D3DTS_WORLD, &(ArrLights[id]->WorldMat.operator D3DXMATRIX()));
		ArrLights[id]->Mesh->DrawSubset(0);
	}
}

void Lights::GetLightColor(long id,float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		(*vec) = ArrLights[id]->Color;
	}
}

void Lights::SetLightColor(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->Color = *vec;
	}
}

void Lights::GetLightPosW(long id, float4* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		(*vec) = ArrLights[id]->Position;
	}
}

void Lights::GetLightPos(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		vec->x = ArrLights[id]->Position.x;
		vec->y = ArrLights[id]->Position.y;
		vec->z = ArrLights[id]->Position.z;
	}
}

void Lights::SetLightPos(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->IsGlobal)
		{
			ArrLights[id]->GAngleX = vec->x;
			ArrLights[id]->GAngleY = vec->y;

			if (ArrLights[id]->GAngleX > 360 || ArrLights[id]->GAngleX < 0)
				ArrLights[id]->GAngleX = 0;

			if (ArrLights[id]->GAngleY > 360 || ArrLights[id]->GAngleY < 0)
				ArrLights[id]->GAngleY = 0;


			float4x4 mat = SMMatrixRotationZ(-D3DXToRadian(ArrLights[id]->GAngleX)) * SMMatrixRotationY(D3DXToRadian(ArrLights[id]->GAngleY));
			ArrLights[id]->Position = SMVector3Transform(float3(-1, 0, 0), mat);


			ArrLights[id]->Position.x *= LIGHTS_POS_G_MAX;
			ArrLights[id]->Position.y *= LIGHTS_POS_G_MAX;
			ArrLights[id]->Position.z *= LIGHTS_POS_G_MAX;

			//Position.w = pos->w;
			ArrLights[id]->WorldMat = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
			if (ArrLights[id]->ShadowPSSM)
				ArrLights[id]->ShadowPSSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
		}
		else
		{

			ArrLights[id]->Position.x = vec->x;
			ArrLights[id]->Position.y = vec->y;
			ArrLights[id]->Position.z = vec->z;
			//Position.w = pos->w;

			float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
			ArrLights[id]->WorldMat = ArrLights[id]->MatRot * mpos;

			if (ArrLights[id]->ShadowSM)
			{
				ArrLights[id]->ShadowSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}

			if (ArrLights[id]->ShadowCube)
			{
				ArrLights[id]->WorldMat = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
				ArrLights[id]->ShadowCube->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}

			/*if (SourceLight)
			{
				SourceLight->SetPosition(&float3(Position.x, Position.y, Position.z));
			}

			if (MeshRays)
			{
				MeshRays->SetPosition(&float3(Position.x, Position.y, Position.z));
			}

			SetNullUpdateShadow();*/
		}
	}
}


void Lights::GetLightRot(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		*vec = ArrLights[id]->Rotation;
	}
}

void Lights::SetLightRot(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->Rotation = *vec;

		ArrLights[id]->Direction = SMEulerToVec(ArrLights[id]->Rotation);

		float3 f(0, -1, 0);
		float3 a = SMVector3Cross(f, ArrLights[id]->Direction);
		float ang = acosf(SMVector3Dot(f, ArrLights[id]->Direction));
		ArrLights[id]->MatRot = SMMatrixRotationAxis(a, ang);

		float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
		ArrLights[id]->WorldMat = ArrLights[id]->MatRot * mpos;

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetDirection(&ArrLights[id]->Direction);

		/*if (SourceLight)
			SourceLight->SetRotation(&Rotation);

			if (MeshRays)
			MeshRays->SetRotation(&Rotation);

			SetNullUpdateShadow();*/
	}
}

void Lights::GetLightDir(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		*vec = ArrLights[id]->Direction;
	}
}

void Lights::SetLightDir(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->Direction = *vec;

		float3 f(0, -1, 0);
		float3 a = SMVector3Cross(f, ArrLights[id]->Direction);
		float ang = acosf(SMVector3Dot(f, ArrLights[id]->Direction));
		ArrLights[id]->MatRot = SMMatrixRotationAxis(a, ang);

		ArrLights[id]->Rotation = SMMatrixToEuler(ArrLights[id]->MatRot);

		float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
		ArrLights[id]->WorldMat = ArrLights[id]->MatRot * mpos;

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetDirection(&ArrLights[id]->Direction);

		/*if (SourceLight)
			SourceLight->SetRotation(&Rotation);

		if (MeshRays)
			MeshRays->SetRotation(&Rotation);

		SetNullUpdateShadow();*/
	}
}


bool Lights::ComVisibleForFrustum(long id, ISXFrustum* frustum)
{
	if (id >= 0 && id < ArrLights.size())
	{
		float3 tmpcenter;
		float tmpradius;
		ArrLights[id]->BoundVolume->GetSphere(&tmpcenter, &tmpradius);
		tmpcenter = SMVector3Transform(tmpcenter, ArrLights[id]->WorldMat);

		return frustum->SphereInFrustum(&tmpcenter, tmpradius);
	}
	return false;
}

bool Lights::GetVisibleForFrustum(long id, ISXFrustum* frustum)
{
	if (id >= 0 && id < ArrLights.size())
	{
		return ArrLights[id]->IsVisibleFor;
	}
}

float Lights::ComDistFor(long id, float3* vec)
{
	if (id >= 0 && id < ArrLights.size())
	{
		return SMVector3Distance((float3)ArrLights[id]->Position, *vec);
	}
}

void Lights::ComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec)
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		ArrLights[i]->DistFor = SMVector3Distance((float3)ArrLights[i]->Position, *vec);
		ArrLights[i]->IsVisibleFor = ComVisibleForFrustum(i, frustum);
	}
}

float Lights::GetDistFor(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		return ArrLights[id]->DistFor;
	}
}

IDirect3DTexture9* Lights::GetShadow(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetShadow();
		else if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->GetShadow();

		if (ArrLights[id]->ShadowPSSM)
			return ArrLights[id]->ShadowPSSM->GetShadow();
	}

	return 0;
}

bool Lights::IsEnable(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		return ArrLights[id]->IsEnable;
	}
	return false;
}

void Lights::SetEnable(long id, bool val)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->IsEnable = val;
	}
}

bool Lights::IsShadow(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		return ArrLights[id]->IsShadow;
	}
	return false;
}

int Lights::GetType(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowPSSM)
			return LIGHTS_TYPE_GLOBAL;
		else if (ArrLights[id]->ShadowSM)
			return LIGHTS_TYPE_DIRECTION;
		else if (ArrLights[id]->ShadowCube)
			return LIGHTS_TYPE_POINT;
	}
	return -1;
}

void Lights::InRenderBegin(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->Begin();
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->Begin();
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->Begin();
	}
}

void Lights::InRenderEnd(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->End();
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->End();
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->End();
	}
}

void Lights::InRenderPre(long id, int cube)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->Pre(cube);
	}
}

void Lights::InRenderPost(long id, int cube)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->Post(cube);
	}
}

ISXFrustum* Lights::GetFrustum(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->Frustum;
		else if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->Frustum;
		/*else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->End();*/
	}
	return 0;
}

ISXFrustum* Lights::GetFrustumG(long id, int split)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowPSSM && split >= 0 && split < 4 && ArrLights[id]->ShadowPSSM->Frustums[split])
			return ArrLights[id]->ShadowPSSM->Frustums[split];
	}
	return 0;
}

void Lights::UpdateG(long id, int split, float3* pos, float3* dir)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowPSSM && split >= 0 && split < 4)
			ArrLights[id]->ShadowPSSM->Update(split,pos,dir);
	}
}

void Lights::GenShadow(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->GenShadow();
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->GenShadow();
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->GenShadow();
	}
}

void Lights::SoftShadow(long id, bool randomsam, float size)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SoftShadow(randomsam, size);
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SoftShadow(randomsam, size);
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->SoftShadow(randomsam, size);
	}
}

void Lights::ChangeAngle(long id, float angle, bool is_create)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->Angle = angle;
		if (ArrLights[id]->TypeLight == 1 && is_create)
		{
			mem_release_del(ArrLights[id]->Mesh);
			ArrLights[id]->TopBottomRadius.y = ArrLights[id]->TopBottomRadius.x + ArrLights[id]->Position.w * tanf(angle / 2.f);
			ArrLights[id]->Angle = angle;
			SGCore_FCreateCone(ArrLights[id]->TopBottomRadius.x, ArrLights[id]->TopBottomRadius.y, ArrLights[id]->Position.w, &ArrLights[id]->Mesh, 32);
			//ArrLights[id]->PathVolume[0] = 0;
		}

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->CalcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release(vertexbuf);
		}

		if (ArrLights[id]->TypeLight == 1 && ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(angle, 0.1, ArrLights[id]->Position.w));

		//SetNullUpdateShadow();
	}
}

void Lights::ChangeTopRadius(long id, float top_radius)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->TypeLight == 1)
		{
			mem_release_del(ArrLights[id]->Mesh);
			ArrLights[id]->TopBottomRadius.x = top_radius;
			SGCore_FCreateCone(top_radius, ArrLights[id]->TopBottomRadius.y, ArrLights[id]->Position.w, &ArrLights[id]->Mesh, 32);
			//ArrLights[id]->PathVolume[0] = 0;
		}

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->CalcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		//SetNullUpdateShadow();
	}
}

void Lights::ChangeRadiusHeight(long id, float radius_height, bool is_create)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->Position.w = radius_height;
		if (ArrLights[id]->TypeLight == 1 && is_create)
		{
			ArrLights[id]->TopBottomRadius.y = ArrLights[id]->TopBottomRadius.x + radius_height * tanf(ArrLights[id]->Angle / 2.f);
			mem_release_del(ArrLights[id]->Mesh);
			SGCore_FCreateCone(ArrLights[id]->TopBottomRadius.x, ArrLights[id]->TopBottomRadius.y, radius_height, &ArrLights[id]->Mesh, 32);
			//PathVolume[0] = 0;
		}

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Position.w));

		if (ArrLights[id]->TypeLight == 0 && is_create)
		{
			mem_release_del(ArrLights[id]->Mesh);
			D3DXCreateSphere(MLSet::DXDevice, radius_height, 20, 20, &ArrLights[id]->Mesh, 0);
			//PathVolume[0] = 0;
		}

		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetNearFar(&float2(0.1, ArrLights[id]->Position.w));

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->CalcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		//SetNullUpdateShadow();
	}
}

void Lights::ChangeShadow(long id, bool is_shadow)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->IsShadow = is_shadow;
		if (ArrLights[id]->TypeLight == 1)
		{
			if (!ArrLights[id]->ShadowSM && ArrLights[id]->IsShadow)
			{
				ArrLights[id]->ShadowSM = new ShadowMapTech();
				ArrLights[id]->ShadowSM->Init();
				ArrLights[id]->ShadowSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
				ArrLights[id]->ShadowSM->SetDirection(&ArrLights[id]->Direction);
				ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Position.w));
				ArrLights[id]->ShadowSM->SetStatic(false);
			}
		}
		else if (ArrLights[id]->TypeLight == 0)
		{
			if (ArrLights[id]->IsGlobal)
			{
				if (!ArrLights[id]->ShadowPSSM && ArrLights[id]->IsShadow)
				{
					ArrLights[id]->ShadowPSSM = new PSSM();
					ArrLights[id]->ShadowPSSM->Init();
					ArrLights[id]->ShadowPSSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
				}
			}
			else
			{
				if (!ArrLights[id]->ShadowCube && ArrLights[id]->IsShadow)
				{
					ArrLights[id]->ShadowCube = new ShadowMapCubeTech();
					ArrLights[id]->ShadowCube->Init();
					ArrLights[id]->ShadowCube->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
					ArrLights[id]->ShadowCube->SetNearFar(&float2(0.1f, ArrLights[id]->Position.w));
					ArrLights[id]->ShadowCube->SetStatic(false);
				}
			}
		}
		//SetNullUpdateShadow();
	}
}

void Lights::SetBlurPixel(long id, float blur_pixel)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->SetBlurPixel(blur_pixel);

		//SetNullUpdateShadow();
	}
}

float Lights::GetBlurPixel(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->GetBlurPixel();
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetBlurPixel();
		else if (ArrLights[id]->ShadowPSSM)
			return ArrLights[id]->ShadowPSSM->GetBlurPixel();
	}
}

void Lights::SetShadowLocalNear(long id, float slnear)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetNear(slnear);
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetNear(slnear);
	}
}

float Lights::GetShadowLocalNear(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->GetNear();
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetNear();
	}
}

void Lights::SetShadowLocalFar(long id, float slfar)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
		{
			float2 tmpnf;
			ArrLights[id]->ShadowCube->GetNearFar(&tmpnf);
			tmpnf.y = slfar;
			ArrLights[id]->ShadowCube->SetNearFar(&tmpnf);
		}
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetFar(slfar);
		//SetNullUpdateShadow();
	}
}

float Lights::GetShadowLocalFar(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
		{
			float2 tmpnf;
			ArrLights[id]->ShadowCube->GetNearFar(&tmpnf);
			return tmpnf.y;
		}
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetFar();
		else
			return ArrLights[id]->Position.w;
	}
}

void Lights::SetEnableCubeEdge(long id, int edge, bool enable)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetEnableCubeEdge(edge, enable);

		//SetNullUpdateShadow();
	}
}

bool Lights::GetEnableCubeEdge(long id, int edge)
{
	if (id >= 0 && id < ArrLights.size())
	{
		if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->GetEnableCubeEdge(edge);
	}
}
