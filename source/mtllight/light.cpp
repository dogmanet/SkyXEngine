
#include "light.h"

Lights::Lights()
{
	HowShadow = 0;
	ShadowMap = SGCore_RTAdd(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap", 1);
	ShadowMap2 = SGCore_RTAdd(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap2", 1);

	GlobalLight = -1;
}

Lights::~Lights()
{
	for (int i = 0; i < ArrKeyLights.size(); ++i)
	{
		mem_delete(ArrKeyLights[i]);
	}

	ArrKeyLights.clear();
	ArrIDLights.clear();

	SGCore_RTDelete(ShadowMap);
	SGCore_RTDelete(ShadowMap2);
}

ID Lights::CreateCopy(ID id)
{
	LIGHTS_PRE_COND_ID(id,-1);
	
		Light* tmplight = ArrIDLights[id];
		Light* tmplight2 = new Light();

		tmplight2->Angle = tmplight->Angle;
		tmplight2->BoundVolume = SGCore_CrBound();
		IDirect3DVertexBuffer9* vertexbuf;
		tmplight->Mesh->GetVertexBuffer(&vertexbuf);
		tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
		mem_release(vertexbuf);
		tmplight2->Power = tmplight->Power;
		tmplight2->Dist = tmplight->Dist;
		tmplight2->Color = tmplight->Color;
		tmplight2->DistFor = -1;
		tmplight2->GAngleX = tmplight->GAngleX;
		tmplight2->GAngleY = tmplight->GAngleY;
		tmplight2->IsEnable = tmplight->IsEnable;
		tmplight2->IsGlobal = tmplight->IsGlobal;
		tmplight2->IsVisible = tmplight->IsVisible;
		tmplight2->IsVisibleFor = false;
		sprintf(tmplight2->Name, "%s", tmplight->Name);
		tmplight2->Position = tmplight->Position;
		tmplight2->Quaternion = tmplight->Quaternion;
		tmplight2->TopBottomRadius = tmplight->TopBottomRadius;
		tmplight2->TypeLight = tmplight->TypeLight;
		tmplight2->WorldMat = tmplight->WorldMat;

		tmplight2->TypeShadowed = tmplight->TypeShadowed;
		tmplight2->CountUpdate = tmplight->CountUpdate;
		
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
			tmplight2->ShadowSM->SetDirection(&(tmplight->Quaternion * LIGHTS_DIR_BASE));
			
			tmplight2->ShadowSM->SetBias(tmplight->ShadowSM->GetBias());
			tmplight2->ShadowSM->SetBlurPixel(tmplight->ShadowSM->GetBlurPixel());
			float3 anf;
			tmplight2->ShadowSM->GetAngleNearFar(&anf);
			tmplight2->ShadowSM->SetAngleNearFar(&anf);
		}
		else
			tmplight2->ShadowSM = 0;

		if (tmplight->ShadowCube)
		{
			tmplight2->ShadowCube = new ShadowMapCubeTech();
			tmplight2->ShadowCube->Init();
			tmplight2->ShadowCube->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowCube->SetNearFar(&float2(0.1f, tmplight->Dist));
			tmplight2->ShadowCube->SetNear(tmplight2->ShadowCube->GetNear());
			tmplight2->ShadowCube->SetFar(tmplight2->ShadowCube->GetFar());
			tmplight2->ShadowCube->SetBias(tmplight->ShadowCube->GetBias());
			tmplight2->ShadowCube->SetBlurPixel(tmplight->ShadowCube->GetBlurPixel());

			for (int i = 0; i < 6; ++i)
			{
				tmplight2->ShadowCube->SetEnableCubeEdge(i, tmplight2->ShadowCube->GetEnableCubeEdge(i));
			}
		}
		else
			tmplight2->ShadowCube = 0;

		tmplight->Mesh->CloneMeshFVF(tmplight->Mesh->GetOptions(), tmplight->Mesh->GetFVF(), MLSet::DXDevice, &(tmplight2->Mesh));

	return AddLight(tmplight);
}

Lights::Light::Light()
{
	TypeLight = LightsTypeLight::ltl_none;
	Name[0] = 0;

	Id = -1;
	Key = -1;
	
	TypeShadowed = LightsTypeShadow::lts_static;
	CountUpdate = 0;

	IsVisible = IsEnable = IsGlobal = false;

	Mesh = 0;
	Power = Dist = 0;
	GAngleX = GAngleY = 0;

	Angle = 0;

	BoundVolume = 0;

	IsVisibleFor = false;
	DistFor = -1;
	Quaternion = SMQuaternion(-SM_PI, 'z');

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
	for (int i = 0; i < ArrKeyLights.size(); ++i)
	{
		if (ArrKeyLights[i]->ShadowSM)
			ArrKeyLights[i]->ShadowSM->OnLostDevice();
		else if (ArrKeyLights[i]->ShadowCube)
			ArrKeyLights[i]->ShadowCube->OnLostDevice();
		else if (ArrKeyLights[i]->ShadowPSSM)
			ArrKeyLights[i]->ShadowPSSM->OnLostDevice();
	}
}

void Lights::OnResetDevice()
{
	for (int i = 0; i < ArrKeyLights.size(); ++i)
	{
		if (ArrKeyLights[i]->ShadowSM)
			ArrKeyLights[i]->ShadowSM->OnResetDevice();
		else if (ArrKeyLights[i]->ShadowCube)
			ArrKeyLights[i]->ShadowCube->OnResetDevice();
		else if (ArrKeyLights[i]->ShadowPSSM)
			ArrKeyLights[i]->ShadowPSSM->OnResetDevice();

		LightCountUpdateNull(i);
	}
}

ID Lights::AddLight(Light* obj)
{
	ID idadd = -1;
	for (int i = 0; i < ArrIDLights.size(); ++i)
	{
		if (ArrIDLights[i] == 0)
		{
			ArrIDLights[i] = obj;
			idadd = i;
			break;
		}
	}

	if (idadd == -1)
	{
		ArrIDLights.push_back(obj);
		idadd = ArrIDLights.size() - 1;
	}

	obj->Id = idadd;
	ArrKeyLights.push_back(obj);
	obj->Key = ArrKeyLights.size() - 1;
	return idadd;
}

int Lights::GetCountLights() const 
{
	return ArrKeyLights.size();
}

void Lights::Clear()
{
	for (int i = 0; i < ArrKeyLights.size(); ++i)
	{
		mem_delete(ArrKeyLights[i]);
	}

	ArrKeyLights.clear();
	ArrIDLights.clear();
	GlobalLight = -1;
}

void Lights::DeleteLight(ID id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->IsGlobal)
		GlobalLight = -1;

	for (int i = ArrIDLights[id]->Key + 1; i < ArrKeyLights.size(); ++i)
	{
		--(ArrKeyLights[i]->Key);
	}

	ArrIDLights[id]->IsEnable = false;
	ArrKeyDelLights.push_back(ArrIDLights[id]);
	ArrKeyLights.erase(ArrIDLights[id]->Key);
	ArrIDLights[id] = 0;
}

char* Lights::GetLightName(ID id)
{
	LIGHTS_PRE_COND_ID(id, 0);
	return ArrIDLights[id]->Name;
}

void Lights::SetLightName(ID id, const char* name)
{
	LIGHTS_PRE_COND_ID(id);
	sprintf(ArrIDLights[id]->Name, "%s", name);
}

ID Lights::CreatePoint(ID id, const float3* center, float dist, const float3* color, bool isglobal, bool is_shadow, const char* bound_volume)
{
	if (GlobalLight != -1 && isglobal)
	{
		g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - light: global light exists, you can not create 2 global light sources", gen_msg_location);
		return -1;
	}

	Light* tmplight = 0;// new Light();

	if (id < 0)
		tmplight = new Light();
	else
	{
		tmplight = new Light();
		tmplight->Id = ArrIDLights[id]->Id;
		tmplight->Key = ArrIDLights[id]->Key;
		mem_delete(ArrIDLights[id]);
		ArrIDLights[id] = tmplight;
	}

	tmplight->IsGlobal = isglobal;

	if (isglobal)
	{
		tmplight->Dist = LIGHTS_GLOBAL_STD_RADIUS;
	}
	else
	{
		tmplight->Dist = dist;
	}

	tmplight->TypeLight = (isglobal ? LightsTypeLight::ltl_global : LightsTypeLight::ltl_point);
		if(StrValidate(bound_volume))
		{
			
		}
		else
		{
			D3DXCreateSphere(MLSet::DXDevice, tmplight->Dist, 20, 20, &tmplight->Mesh, 0);
		}

		if (isglobal && is_shadow)
		{
			tmplight->ShadowPSSM = new PSSM();
			tmplight->ShadowPSSM->Init();
			tmplight->ShadowPSSM->SetPosition(&float3(center->x, center->y, center->z));
		}
		else if (is_shadow)
		{
			tmplight->ShadowCube = new ShadowMapCubeTech();
			tmplight->ShadowCube->Init();
			tmplight->ShadowCube->SetPosition(&float3(center->x, center->y, center->z));
			tmplight->ShadowCube->SetNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, tmplight->Dist));
			
		}

		if (is_shadow)
			tmplight->TypeShadowed = LightsTypeShadow::lts_dynamic;
		else
			tmplight->TypeShadowed = LightsTypeShadow::lts_none;
		
	ID tmpid = id;

	if (id == -1)
		tmpid = AddLight(tmplight);

	SetLightPos(tmpid, &float3(center->x, center->y, center->z));

	tmplight->Color = *color;
	tmplight->IsEnable = true;
	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	if (tmplight->IsGlobal)
		GlobalLight = tmpid;

	return tmpid;
}

ID Lights::CreateDirection(ID id, const float3* pos, float dist, const float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow, const char* bound_volume)
{
	Light* tmplight = 0;

	if (id < 0)
		tmplight = new Light();
	else
	{
		tmplight = new Light();
		tmplight->Id = ArrIDLights[id]->Id;
		tmplight->Key = ArrIDLights[id]->Key;
		mem_delete(ArrIDLights[id]);
		ArrIDLights[id] = tmplight;
	}

	tmplight->IsGlobal = false;
	tmplight->TypeLight = LightsTypeLight::ltl_direction;
	tmplight->Angle = angle;
	float bottom_radius = top_radius + dist * tanf(angle / 2.f);
	tmplight->TopBottomRadius.x = top_radius;
	tmplight->TopBottomRadius.y = bottom_radius;

	if (StrValidate(bound_volume))
	{
		
	}
	else
		SGCore_FCreateCone(top_radius, bottom_radius, dist, &tmplight->Mesh, 32);

	tmplight->TopBottomRadius = float2_t(top_radius, bottom_radius);
	tmplight->Color = *color;
	
	if (orient)
		tmplight->Quaternion = *orient;

	tmplight->Dist = dist;

	tmplight->IsEnable = true;
	float4x4 mpos = SMMatrixTranslation(*pos);
	tmplight->WorldMat = tmplight->Quaternion.GetMatrix() * mpos;

	tmplight->Position = float3(pos->x, pos->y, pos->z);
	
	if (is_shadow)
	{
		tmplight->ShadowSM = new ShadowMapTech();
		tmplight->ShadowSM->Init();
		tmplight->ShadowSM->SetPosition(&float3(pos->x, pos->y, pos->z));
		tmplight->ShadowSM->SetDirection(&(tmplight->Quaternion * LIGHTS_DIR_BASE));
		tmplight->ShadowSM->SetAngleNearFar(&float3(angle, LIGHTS_LOCAL_STD_NEAR, dist));
	}
	
	if (is_shadow)
		tmplight->TypeShadowed = LightsTypeShadow::lts_dynamic;
	else
		tmplight->TypeShadowed = LightsTypeShadow::lts_none;

	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	ID tmpid = id;

	if (id == -1)
		tmpid = AddLight(tmplight);
	
	return tmpid;
}

void Lights::Render(ID id, DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id);

	float4x4 tmpwmat = ArrIDLights[id]->WorldMat;
	MLSet::DXDevice->SetTransform(D3DTS_WORLD, &(ArrIDLights[id]->WorldMat.operator D3DXMATRIX()));
	ArrIDLights[id]->Mesh->DrawSubset(0);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + (ArrIDLights[id]->Mesh->GetNumFaces() / 3));
}

ID Lights::GetLightGlobal() const
{
	return GlobalLight;
}

void Lights::GetLightColor(ID id, float3* vec) const
{
	LIGHTS_PRE_COND_ID(id);

	(*vec) = ArrIDLights[id]->Color;
}

void Lights::SetLightColor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id);

	ArrIDLights[id]->Color = *vec;
}

void Lights::GetLightPos(ID id, float3* vec, bool greal) const
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->IsGlobal && !greal)
	{
		vec->x = ArrIDLights[id]->GAngleX;
		vec->y = ArrIDLights[id]->GAngleY;
		vec->z = 0;
	}
	else
	{
		vec->x = ArrIDLights[id]->Position.x;
		vec->y = ArrIDLights[id]->Position.y;
		vec->z = ArrIDLights[id]->Position.z;	
	}
}

float Lights::GetLightPower(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	float power = ArrIDLights[id]->Color.x;

	if (power < ArrIDLights[id]->Color.y)
		power = ArrIDLights[id]->Color.y;

	if (power < ArrIDLights[id]->Color.z)
		power = ArrIDLights[id]->Color.z;

	if (ArrIDLights[id]->IsGlobal)
		return power;
	else
	{
		return power * (ArrIDLights[id]->Dist / LIGHTS_LOCAL_MAX_DIST);
	}
}

float Lights::GetLightDist(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrIDLights[id]->Dist;
}

void Lights::SetLightDist(ID id, float radius_height, bool is_create)
{
	LIGHTS_PRE_COND_ID(id);

	ArrIDLights[id]->Dist = radius_height;
	if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_direction && is_create)
	{
		ArrIDLights[id]->TopBottomRadius.y = ArrIDLights[id]->TopBottomRadius.x + radius_height * tanf(ArrIDLights[id]->Angle / 2.f);
		mem_release_del(ArrIDLights[id]->Mesh);
		SGCore_FCreateCone(ArrIDLights[id]->TopBottomRadius.x, ArrIDLights[id]->TopBottomRadius.y, radius_height, &ArrIDLights[id]->Mesh, 32);
	}

	if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrIDLights[id]->Angle, 0.1, ArrIDLights[id]->Dist));

	if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_point && is_create)
	{
		mem_release_del(ArrIDLights[id]->Mesh);
		D3DXCreateSphere(MLSet::DXDevice, radius_height, 20, 20, &ArrIDLights[id]->Mesh, 0);
	}

	if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->SetNearFar(&float2(0.1, ArrIDLights[id]->Dist));

	if (ArrIDLights[id]->Mesh)
	{
		IDirect3DVertexBuffer9* vertexbuf;
		ArrIDLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
		ArrIDLights[id]->BoundVolume->CalcBound(vertexbuf, ArrIDLights[id]->Mesh->GetNumVertices(), ArrIDLights[id]->Mesh->GetNumBytesPerVertex());
		mem_release_del(vertexbuf);
	}

	LightCountUpdateNull(id);
}

void Lights::SetLightPos(ID id, const float3* vec, bool greal)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->IsGlobal)
	{
		Light* tmplight = ArrIDLights[id];
		tmplight->GAngleX = vec->x;
		tmplight->GAngleY = vec->y;

		if (tmplight->GAngleX > 360 || tmplight->GAngleX < 0)
			tmplight->GAngleX = 0;

		if (tmplight->GAngleY > 360 || tmplight->GAngleY < 0)
			tmplight->GAngleY = 0;


		float4x4 mat = SMMatrixRotationZ(-D3DXToRadian(tmplight->GAngleX)) * SMMatrixRotationY(D3DXToRadian(tmplight->GAngleY));
		tmplight->Position = SMVector3Transform(float3(-1, 0, 0), mat);


		tmplight->Position.x *= LIGHTS_POS_G_MAX;
		tmplight->Position.y *= LIGHTS_POS_G_MAX;
		tmplight->Position.z *= LIGHTS_POS_G_MAX;

		tmplight->WorldMat = SMMatrixTranslation(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z);
		if (tmplight->ShadowPSSM)
			tmplight->ShadowPSSM->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
	}
	else
	{
				ArrIDLights[id]->Position.x = (*vec).x;
				ArrIDLights[id]->Position.y = (*vec).y;
				ArrIDLights[id]->Position.z = (*vec).z;

				float4x4 mpos = SMMatrixTranslation(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z);
				ArrIDLights[id]->WorldMat = ArrIDLights[id]->Quaternion.GetMatrix() * mpos;

			if (ArrIDLights[id]->ShadowSM)
			{
				ArrIDLights[id]->ShadowSM->SetPosition(&float3(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z));
			}

			if (ArrIDLights[id]->ShadowCube)
			{
				ArrIDLights[id]->WorldMat = SMMatrixTranslation(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z);
				ArrIDLights[id]->ShadowCube->SetPosition(&float3(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z));
			}
	}

	LightCountUpdateNull(id);
}

void Lights::GetLightOrient(ID id, SMQuaternion* q) const
{
	LIGHTS_PRE_COND_ID(id);

	*q = ArrIDLights[id]->Quaternion;
}

void Lights::SetLightOrient(ID id, const SMQuaternion* q)
{
	LIGHTS_PRE_COND_ID(id);

	ArrIDLights[id]->Quaternion = *q;

	if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->SetDirection(&(ArrIDLights[id]->Quaternion * LIGHTS_DIR_BASE));

	LightCountUpdateNull(id);
}

void Lights::SetShadowBias(ID id, float val)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrIDLights[id]->ShadowSM)
			ArrIDLights[id]->ShadowSM->SetBias(val);
		else if (ArrIDLights[id]->ShadowCube)
			ArrIDLights[id]->ShadowCube->SetBias(val);

		LightCountUpdateNull(id);
}

float Lights::GetShadowBias(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrIDLights[id]->ShadowSM)
		return ArrIDLights[id]->ShadowSM->GetBias();
	else if (ArrIDLights[id]->ShadowCube)
		return ArrIDLights[id]->ShadowCube->GetBias();
}

float Lights::GetLightTopRadius(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrIDLights[id]->ShadowSM)
		return ArrIDLights[id]->TopBottomRadius.x;
}

float Lights::GetLightAngle(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrIDLights[id]->ShadowSM)
		return ArrIDLights[id]->Angle;
}


bool Lights::ComVisibleForFrustum(ID id, const ISXFrustum* frustum)
{
	LIGHTS_PRE_COND_ID(id, false);

	float3 tmpcenter;
	float tmpradius;
	ArrIDLights[id]->BoundVolume->GetSphere(&tmpcenter, &tmpradius);
	tmpcenter = SMVector3Transform(tmpcenter, ArrIDLights[id]->WorldMat);

	return frustum->SphereInFrustum(&tmpcenter, tmpradius);
}

bool Lights::GetVisibleForFrustum(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrIDLights[id]->IsVisibleFor;
}

float Lights::ComDistFor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return SMVector3Distance((float3)ArrIDLights[id]->Position, *vec);
}

void Lights::ComVisibleFrustumDistFor(const ISXFrustum* frustum, const float3* vec)
{
	for (int i = 0; i < ArrKeyLights.size(); ++i)
	{
		ArrKeyLights[i]->DistFor = SMVector3Distance((float3)ArrKeyLights[i]->Position, *vec);
		ArrKeyLights[i]->IsVisibleFor = ComVisibleForFrustum(ArrKeyLights[i]->Id, frustum);
	}
}

float Lights::GetDistFor(ID id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrIDLights[id]->DistFor;
}

IDirect3DTexture9* Lights::GetShadow2()
{
	
	return SGCore_RTGetTexture((HowShadow == 1 ? ShadowMap2 : ShadowMap));
}

bool Lights::GetLightEnable(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrIDLights[id]->IsEnable;
}

void Lights::SetLightEnable(ID id, bool val)
{
	LIGHTS_PRE_COND_ID(id);

	ArrIDLights[id]->IsEnable = val;
	LightCountUpdateNull(id);
}

bool Lights::GetLightShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return (ArrIDLights[id]->TypeShadowed != LightsTypeShadow::lts_none);
}

LightsTypeLight Lights::GetLightType(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LightsTypeLight::ltl_none);
	return ArrIDLights[id]->TypeLight;
}

void Lights::ShadowRenderBegin(ID id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->Begin();
	else if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->Begin();
	else if (ArrIDLights[id]->ShadowPSSM)
		ArrIDLights[id]->ShadowPSSM->Begin();
}

void Lights::ShadowRenderEnd(ID id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->End();
	else if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->End();
	else if (ArrIDLights[id]->ShadowPSSM)
		ArrIDLights[id]->ShadowPSSM->End();
}

void Lights::ShadowRenderPre(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->Pre(cube);
	else if (ArrIDLights[id]->ShadowPSSM && cube >= 0 && cube < 5)
		ArrIDLights[id]->ShadowPSSM->PreRender(cube);
}

void Lights::ShadowRenderPost(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->Post(cube);
}

void Lights::InitShaderOfTypeMaterial(ID id, int typemat, float4x4* wmat)
{
	if (!wmat)
		wmat = &SMMatrixIdentity();
	
	float4x4 tmpmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &tmpmat);
	tmpmat = SMMatrixTranspose((*wmat) * tmpmat);
	float4x4 tmpwmat = SMMatrixTranspose(*wmat);
	if (ArrIDLights[id]->ShadowSM || ArrIDLights[id]->ShadowPSSM)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthTreePSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthTreePSSMDirect);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthSkinPSSMDirect);
		}
	}
	else if (ArrIDLights[id]->ShadowCube)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGeomCube, "WorldViewProjection", &tmpmat);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGeomCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGeomCube, "LightPos", &ArrIDLights[id]->Position);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGeomCube);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthGeomCube);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGrassCube, "WorldViewProjection", &tmpmat);
			//SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGrassCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGrassCube, "LightPos", &ArrIDLights[id]->Position);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthGrassCube);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthTreeCube, "WorldViewProjection", &tmpmat);
			//SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthTreeCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthTreeCube, "LightPos", &ArrIDLights[id]->Position);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthTreeCube);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthSkinCube, "WorldViewProjection", &tmpmat);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthSkinCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthSkinCube, "LightPos", &ArrIDLights[id]->Position);
			SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::SMDepthSkinCube);
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SMDepthSkinCube);
		}
	}
}

ISXFrustum* Lights::GetLightFrustum(ID id, int how) const
{
	LIGHTS_PRE_COND_ID(id, 0);

		if (ArrIDLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrIDLights[id]->ShadowSM->Frustum;
		}
		else if (ArrIDLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrIDLights[id]->ShadowCube->Frustums[how];
		}
		else if (ArrIDLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrIDLights[id]->ShadowPSSM->Frustums[how];
		}
}

ISXFrustum* Lights::GetLightFrustumG(ID id, int split) const
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (ArrIDLights[id]->ShadowPSSM && split >= 0 && split < 4 && ArrIDLights[id]->ShadowPSSM->Frustums[split])
		return ArrIDLights[id]->ShadowPSSM->Frustums[split];
}

void Lights::UpdateLightGFrustums(ID id, int split, const float3* pos, const float3* dir)
{
	LIGHTS_PRE_COND_ID(id);

	ArrIDLights[id]->ShadowPSSM->UpdateFrustums(split, pos, dir);
}

void Lights::ShadowGen2(ID id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->GenShadow2(SGCore_RTGetTexture(ShadowMap));
	else if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->GenShadow2(SGCore_RTGetTexture(ShadowMap));
	else if (ArrIDLights[id]->ShadowPSSM)
		ArrIDLights[id]->ShadowPSSM->GenShadow2(SGCore_RTGetTexture(ShadowMap));
}

void Lights::ShadowNull()
{
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	SGCore_RTGetTexture(ShadowMap)->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

	HowShadow = 0;
}

void Lights::SetLightAngle(ID id, float angle, bool is_create)
{
	LIGHTS_PRE_COND_ID(id);

		ArrIDLights[id]->Angle = angle;
		if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_direction && is_create)
		{
			mem_release_del(ArrIDLights[id]->Mesh);
			ArrIDLights[id]->TopBottomRadius.y = ArrIDLights[id]->TopBottomRadius.x + ArrIDLights[id]->Dist * tanf(angle / 2.f);
			ArrIDLights[id]->Angle = angle;
			SGCore_FCreateCone(ArrIDLights[id]->TopBottomRadius.x, ArrIDLights[id]->TopBottomRadius.y, ArrIDLights[id]->Dist, &ArrIDLights[id]->Mesh, 32);
		}

		if (ArrIDLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrIDLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrIDLights[id]->BoundVolume->CalcBound(vertexbuf, ArrIDLights[id]->Mesh->GetNumVertices(), ArrIDLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release(vertexbuf);
		}

		if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_direction && ArrIDLights[id]->ShadowSM)
			ArrIDLights[id]->ShadowSM->SetAngleNearFar(&float3(angle, 0.1, ArrIDLights[id]->Dist));

		LightCountUpdateNull(id);
}

void Lights::SetLightTopRadius(ID id, float top_radius)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrIDLights[id]->TypeLight == 1)
		{
			mem_release_del(ArrIDLights[id]->Mesh);
			ArrIDLights[id]->TopBottomRadius.x = top_radius;
			SGCore_FCreateCone(top_radius, ArrIDLights[id]->TopBottomRadius.y, ArrIDLights[id]->Dist, &ArrIDLights[id]->Mesh, 32);
		}

		if (ArrIDLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrIDLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrIDLights[id]->BoundVolume->CalcBound(vertexbuf, ArrIDLights[id]->Mesh->GetNumVertices(), ArrIDLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		LightCountUpdateNull(id);
}

void Lights::SetShadowBlurPixel(ID id, float blur_pixel)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrIDLights[id]->ShadowCube)
			ArrIDLights[id]->ShadowCube->SetBlurPixel(blur_pixel);
		else if (ArrIDLights[id]->ShadowSM)
			ArrIDLights[id]->ShadowSM->SetBlurPixel(blur_pixel);
		else if (ArrIDLights[id]->ShadowPSSM)
			ArrIDLights[id]->ShadowPSSM->SetBlurPixel(blur_pixel);

		LightCountUpdateNull(id);
}

float Lights::GetShadowBlurPixel(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrIDLights[id]->ShadowCube)
			return ArrIDLights[id]->ShadowCube->GetBlurPixel();
		else if (ArrIDLights[id]->ShadowSM)
			return ArrIDLights[id]->ShadowSM->GetBlurPixel();
		else if (ArrIDLights[id]->ShadowPSSM)
			return ArrIDLights[id]->ShadowPSSM->GetBlurPixel();
}

void Lights::SetShadowLocalNear(ID id, float slnear)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->SetNear(slnear);
	else if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->SetNear(slnear);

	LightCountUpdateNull(id);
}

float Lights::GetShadowLocalNear(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrIDLights[id]->ShadowCube)
		return ArrIDLights[id]->ShadowCube->GetNear();
	else if (ArrIDLights[id]->ShadowSM)
		return ArrIDLights[id]->ShadowSM->GetNear();
}

void Lights::SetShadowLocalFar(ID id, float slfar)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowCube)
	{
		float2 tmpnf;
		ArrIDLights[id]->ShadowCube->GetNearFar(&tmpnf);
		tmpnf.y = slfar;
		ArrIDLights[id]->ShadowCube->SetNearFar(&tmpnf);
	}
	else if (ArrIDLights[id]->ShadowSM)
		ArrIDLights[id]->ShadowSM->SetFar(slfar);

	LightCountUpdateNull(id);
}

float Lights::GetShadowLocalFar(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrIDLights[id]->ShadowCube)
	{
		float2 tmpnf;
		ArrIDLights[id]->ShadowCube->GetNearFar(&tmpnf);
		return tmpnf.y;
	}
	else if (ArrIDLights[id]->ShadowSM)
		return ArrIDLights[id]->ShadowSM->GetFar();
	else
		return ArrIDLights[id]->Dist;
}

void Lights::SetLightCubeEdgeEnable(ID id, int edge, bool enable)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowCube)
		ArrIDLights[id]->ShadowCube->SetEnableCubeEdge(edge, enable);

	LightCountUpdateNull(id);
}

bool Lights::GetLightCubeEdgeEnable(ID id, int edge) const
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrIDLights[id]->ShadowCube)
		return ArrIDLights[id]->ShadowCube->GetEnableCubeEdge(edge);
}

ID Lights::GetLightIDArr(ID id, ID inid, int how)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrIDLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrIDLights[id]->ShadowSM->GetIDArr(inid);
		}
		else if (ArrIDLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrIDLights[id]->ShadowCube->GetIDArr(inid, how);
		}
		else if (ArrIDLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrIDLights[id]->ShadowPSSM->GetIDArr(inid, how);
		}
}

void Lights::SetLightIDArr(ID id, ID inid, int how, ID id_arr)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrIDLights[id]->ShadowSM)
		{
			if (how == 0)
				ArrIDLights[id]->ShadowSM->SetIDArr(inid, id_arr);
		}
		else if (ArrIDLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				ArrIDLights[id]->ShadowCube->SetIDArr(inid, how, id_arr);
		}
		else if (ArrIDLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				ArrIDLights[id]->ShadowPSSM->SetIDArr(inid, how, id_arr);
		}
}

void Lights::SetLightTypeShadowed(ID id, LightsTypeShadow type)
{
	LIGHTS_PRE_COND_ID(id);

	ArrIDLights[id]->TypeShadowed = type;

	if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_direction)
	{
		if (!ArrIDLights[id]->ShadowSM)
		{
			ArrIDLights[id]->ShadowSM = new ShadowMapTech();
			ArrIDLights[id]->ShadowSM->Init();
			ArrIDLights[id]->ShadowSM->SetPosition(&float3(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z));
			ArrIDLights[id]->ShadowSM->SetDirection(&(ArrIDLights[id]->Quaternion * LIGHTS_DIR_BASE));
			ArrIDLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrIDLights[id]->Angle, 0.1, ArrIDLights[id]->Dist));
		}
	}
	else if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_global)
	{
		if (ArrIDLights[id]->IsGlobal)
		{
			if (!ArrIDLights[id]->ShadowPSSM)
			{
				ArrIDLights[id]->ShadowPSSM = new PSSM();
				ArrIDLights[id]->ShadowPSSM->Init();
				ArrIDLights[id]->ShadowPSSM->SetPosition(&float3(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z));
			}
		}
	}
	else if (ArrIDLights[id]->TypeLight == LightsTypeLight::ltl_point)
	{
		if (!ArrIDLights[id]->ShadowCube)
		{
			ArrIDLights[id]->ShadowCube = new ShadowMapCubeTech();
			ArrIDLights[id]->ShadowCube->Init();
			ArrIDLights[id]->ShadowCube->SetPosition(&float3(ArrIDLights[id]->Position.x, ArrIDLights[id]->Position.y, ArrIDLights[id]->Position.z));
			ArrIDLights[id]->ShadowCube->SetNearFar(&float2(0.1f, ArrIDLights[id]->Dist));
		}
	}

	LightCountUpdateNull(id);
}

LightsTypeShadow Lights::GetLightTypeShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LightsTypeShadow::lts_none);

	return ArrIDLights[id]->TypeShadowed;
}


bool Lights::LightCountUpdateUpdate(ID id, const float3* viewpos, int ghow)
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			Light* tmpl = ArrIDLights[id];
			if (tmpl->ShadowPSSM)
			{
				if (ghow >= 0 && ghow < 4)
				{
					if (tmpl->ShadowPSSM->IsUpdate[ghow] >= ghow*LIGHTS_UPDATE_PSSM_SPLIT)
						tmpl->ShadowPSSM->IsUpdate[ghow] = -1;
				}
				else
				{
					if (tmpl->ShadowPSSM->IsUpdate[ghow] >= LIGHTS_UPDATE_PSSM_SPLIT_5)
						tmpl->ShadowPSSM->IsUpdate[ghow] = -1;
					
				}

				++(tmpl->ShadowPSSM->IsUpdate[ghow]);
				
				return (tmpl->ShadowPSSM->IsUpdate[ghow] == 0);
			}
		}

		else if (ArrIDLights[id]->TypeShadowed == LightsTypeShadow::lts_static)
		{
			if (ArrIDLights[id]->CountUpdate < LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
			{
				++(ArrIDLights[id]->CountUpdate);
				return true;
			}
			else
				return false;
		}
		else
		{
			Light* tmpl = ArrIDLights[id];

			float3 tmpcenter;
			float tmpradius;

			tmpl->BoundVolume->GetSphere(&tmpcenter, &tmpradius);
			float dist = SMVector3Distance(tmpl->Position, (*viewpos)) - tmpradius;
			if (dist < LIGHTS_UPDATE_L0_DIST)
				tmpl->CountUpdate = -1;
			else if (dist < LIGHTS_UPDATE_L1_DIST && dist > LIGHTS_UPDATE_L0_DIST)
			{
				if (tmpl->CountUpdate > 1)
					tmpl->CountUpdate = -1;
			}
			else if (dist < LIGHTS_UPDATE_L2_DIST && dist > LIGHTS_UPDATE_L1_DIST)
			{
				if (tmpl->CountUpdate > 2)
					tmpl->CountUpdate = -1;
			}
			else if (dist > LIGHTS_UPDATE_L2_DIST)
			{
				tmpl->CountUpdate = 4;
			}

			++(tmpl->CountUpdate);

			return (tmpl->CountUpdate == 0);
		}
}

bool Lights::LightCountUpdateAllowed(ID id, int ghow) const
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			if (ArrIDLights[id]->ShadowPSSM)
			{
				return (ArrIDLights[id]->ShadowPSSM->IsUpdate[ghow] == 0);
			}
		}
		else if (ArrIDLights[id]->TypeShadowed == LightsTypeShadow::lts_static)
		{
			if (ArrIDLights[id]->CountUpdate >= LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
				return false;
		}
		else
		{
			return (ArrIDLights[id]->CountUpdate == 0);
		}
}

void Lights::LightCountUpdateNull(ID id)
{
	if (id >= 0 && id < ArrIDLights.size())
	{
		ArrIDLights[id]->CountUpdate = 0;
	}
	else if (id < 0)
	{
		for (int i = 0; i < ArrKeyLights.size(); ++i)
		{
			ArrKeyLights[i]->CountUpdate = 0;
		}
	}
}

void Lights::ShadowSoft(bool randomsam, float size, bool isfirst)
{
	if (isfirst)
		HowShadow = 0;

	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			SGCore_RTGetTexture(ShadowMap2)->GetSurfaceLevel(0, &RenderSurf);
		else
			SGCore_RTGetTexture(ShadowMap)->GetSurfaceLevel(0, &RenderSurf);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);

	MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene));
	
		if(HowShadow == 0)
			MLSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(ShadowMap));
		else
			MLSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(ShadowMap2));
	
		SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::ScreenOut);

		if(randomsam)
		{
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
			MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise);
		}
		else
			SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::PPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "PixelSize", &float2_t(size / Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), size / Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT)));
			SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "NearFar", &float2_t(Core_RFloatGet(G_RI_FLOAT_OBSERVER_NEAR), Core_RFloatGet(G_RI_FLOAT_OBSERVER_FAR)));
		}
		else
		{
			SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::PPBlurDepthBased, "PixelSize", &float2_t(size / Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), size / Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT)));
			SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::PPBlurDepthBased, "NearFar", &float2_t(Core_RFloatGet(G_RI_FLOAT_OBSERVER_NEAR), Core_RFloatGet(G_RI_FLOAT_OBSERVER_FAR)));
		}

	
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0,BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

		if(HowShadow == 1)
			HowShadow = 0;
		else
			HowShadow = 1;
}

void Lights::ToneMappingCom(DWORD timeDelta, float factor_adapted)
{
	MLSet::GetArrDownScale4x4(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), MLSet::HDRSampleOffsets);

	LPDIRECT3DSURFACE9 SurfSceneScale, BackBuf;

	SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthComScaled)->GetSurfaceLevel(0, &SurfSceneScale);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, SurfSceneScale);

	SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SampleLumIterative);
	SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::SampleLumIterative, "ArrOffsets", &(MLSet::HDRSampleOffsets));

	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthCom));
	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(SurfSceneScale);

	MLSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);
	int CurrTexture = MLSet::IDsRenderTargets::CountArrToneMaps - 1;

	for (int i = 0; i < MLSet::IDsRenderTargets::CountArrToneMaps; i++)
	{
		IDirect3DTexture9* tmptex = SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[i]);
		IDirect3DSurface9* tmpsurf = MLSet::IDsRenderTargets::SurfToneMap[i];
		SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[i])->GetSurfaceLevel(0, &MLSet::IDsRenderTargets::SurfToneMap[i]);
		int qwert = 0;
	}

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[CurrTexture])->GetLevelDesc(0, &desc);

	MLSet::GetArrDownScale4x4(desc.Width, desc.Height, MLSet::HDRSampleOffsets);

	MLSet::DXDevice->SetRenderTarget(0, MLSet::IDsRenderTargets::SurfToneMap[CurrTexture]);
	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthComScaled));
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SampleLumInit);
	SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::SampleLumInit, "ArrOffsets", &(MLSet::HDRSampleOffsets));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(MLSet::IDsRenderTargets::SurfToneMap[CurrTexture]);

	--CurrTexture;

	

	while (CurrTexture >= 0)
	{
		SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[CurrTexture + 1])->GetLevelDesc(0, &desc);
		MLSet::GetArrDownScale4x4(desc.Width, desc.Height, MLSet::HDRSampleOffsets);

		MLSet::DXDevice->SetRenderTarget(0, MLSet::IDsRenderTargets::SurfToneMap[CurrTexture]);
		MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

		SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::SampleLumIterative);
		SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::SampleLumIterative, "ArrOffsets", &(MLSet::HDRSampleOffsets));

		MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[CurrTexture + 1]));
		SGCore_ScreenQuadDraw();

		SGCore_ShaderUnBind();
		CurrTexture--;
	}

	IDirect3DTexture9* tmptex = SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[3]);

	for (int i = 0; i < MLSet::IDsRenderTargets::CountArrToneMaps-1; i++)
	{
		IDirect3DSurface9* tmpsurf = MLSet::IDsRenderTargets::SurfToneMap[i];
		mem_release(MLSet::IDsRenderTargets::SurfToneMap[i]);
	}

	tmptex = SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[3]);
	
	MLSet::IDsRenderTargets::IncrAdaptedLum();
	LPDIRECT3DSURFACE9 SurfAdaptedLum = NULL;
	SGCore_RTGetTexture(MLSet::IDsRenderTargets::GetCurrAdaptedLum())->GetSurfaceLevel(0, &SurfAdaptedLum);

	MLSet::DXDevice->SetRenderTarget(0, SurfAdaptedLum);
	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::GetLastAdaptedLum()));
	MLSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[0]));
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	SGCore_ShaderBind(ShaderType::st_vertex, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, MLSet::IDsShaders::PS::CalcAdaptedLum);

	float ElapsedTime = float(timeDelta) * 0.001f * (factor_adapted * 1000.f);
	SGCore_ShaderSetVRF(ShaderType::st_pixel, MLSet::IDsShaders::PS::CalcAdaptedLum, "ElapsedTime", &(ElapsedTime));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(SurfAdaptedLum);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	MLSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	
}

void Lights::Set4Or3Splits(ID id, bool is4)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrIDLights[id]->ShadowPSSM)
		ArrIDLights[id]->ShadowPSSM->Set4Or3Splits(is4);
}

bool Lights::Get4Or3Splits(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrIDLights[id]->ShadowPSSM)
		return ArrIDLights[id]->ShadowPSSM->Get4Or3Splits();

	return false;
}

ID Lights::GetIdByKey(int key)
{
	LIGHTS_PRE_COND_KEY(key, -1);

	return ArrKeyLights[key]->Id;
}

///////////////

int Lights::DelGetCount()
{
	return ArrKeyDelLights.size();
}

LightsTypeLight Lights::DelGetType(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, LightsTypeLight::ltl_none);
	return ArrKeyDelLights[key]->TypeLight;
}

void Lights::DelDel(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key);
	mem_delete(ArrKeyDelLights[key]);
	ArrKeyDelLights.erase(key);
}

ID Lights::DelGetIDArr(ID key, ID inid, int how)
{
	LIGHTS_PRE_COND_KEY_DEL(key, -1);

	if (ArrKeyDelLights[key]->ShadowSM)
	{
		if (how == 0)
			return ArrKeyDelLights[key]->ShadowSM->GetIDArr(inid);
	}
	else if (ArrKeyDelLights[key]->ShadowCube)
	{
		if (how >= 0 && how < 6)
			return ArrKeyDelLights[key]->ShadowCube->GetIDArr(inid, how);
	}
	else if (ArrKeyDelLights[key]->ShadowPSSM)
	{
		if (how >= 0 && how < 5)
			return ArrKeyDelLights[key]->ShadowPSSM->GetIDArr(inid, how);
	}
}