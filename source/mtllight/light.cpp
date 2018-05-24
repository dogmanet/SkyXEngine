
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "light.h"

Lights::Lights()
{
	HowShadow = 0;

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	ShadowMap = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap", 1);
	ShadowMap2 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap2", 1);

	GlobalLight = -1;
	m_isCastGlobalShadow = false;
}

Lights::~Lights()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		mem_delete(ArrLights[i]);
	}

	ArrLights.clear();

	SGCore_RTDelete(ShadowMap);
	SGCore_RTDelete(ShadowMap2);
}

ID Lights::CreateCopy(ID id)
{
	LIGHTS_PRE_COND_ID(id,-1);
	
		Light* tmplight = ArrLights[id];
		Light* tmplight2 = new Light();

		tmplight2->Angle = tmplight->Angle;
		tmplight2->BoundVolume = SGCore_CrBound();
		IDirect3DVertexBuffer9* vertexbuf;
		tmplight->Mesh->GetVertexBuffer(&vertexbuf);
		tmplight->BoundVolume->calcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
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
	TypeLight = LTYPE_LIGHT_NONE;
	Name[0] = 0;

	Id = -1;
	
	TypeShadowed = LTYPE_SHADOW_STATIC;
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
	for (int i = 0; i < ArrLights.size(); ++i)
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
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->OnResetDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->OnResetDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->OnResetDevice();

		LightCountUpdateNull(i);
	}
}

ID Lights::AddLight(Light* obj)
{
	ID idadd = -1;

	if (ArrFreeIDs.size() > 0)
	{
		ArrLights[ArrFreeIDs[0]] = obj;
		obj->Id = ArrFreeIDs[0];
		ArrFreeIDs.erase(0);
	}
	else
	{
		ArrLights.push_back(obj);
		obj->Id = ArrLights.size() - 1;
	}

	return obj->Id;
}

int Lights::GetCountLights() const 
{
	return ArrLights.size();
}

void Lights::Clear()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		mem_delete(ArrLights[i]);
	}

	ArrLights.clear();
	GlobalLight = -1;
}

void Lights::ClearIDArr()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (!ArrLights[i])
			continue;

		if (ArrLights[i]->ShadowSM)
		{
			for (int k = 0; k < ArrLights[i]->ShadowSM->GetCountIDArrs(); ++k)
			{
				ArrLights[i]->ShadowSM->SetIDArr(k, -1);
			}
		}
		else if (ArrLights[i]->ShadowCube)
		{
			for (int k = 0; k < ArrLights[i]->ShadowCube->GetCountIDArrs(); ++k)
			{
				ArrLights[i]->ShadowCube->SetIDArr(k, 0, -1);
				ArrLights[i]->ShadowCube->SetIDArr(k, 1, -1);
				ArrLights[i]->ShadowCube->SetIDArr(k, 2, -1);
				ArrLights[i]->ShadowCube->SetIDArr(k, 3, -1);
				ArrLights[i]->ShadowCube->SetIDArr(k, 4, -1);
				ArrLights[i]->ShadowCube->SetIDArr(k, 5, -1);
			}
		}
		else if (ArrLights[i]->ShadowPSSM)
		{
			for (int k = 0; k < ArrLights[i]->ShadowPSSM->GetCountIDArrs(); ++k)
			{
				ArrLights[i]->ShadowPSSM->SetIDArr(k, 0, -1);
				ArrLights[i]->ShadowPSSM->SetIDArr(k, 1, -1);
				ArrLights[i]->ShadowPSSM->SetIDArr(k, 2, -1);
				ArrLights[i]->ShadowPSSM->SetIDArr(k, 3, -1);
				ArrLights[i]->ShadowPSSM->SetIDArr(k, 4, -1);
			}
		}
	}
}

bool Lights::getExists(ID id) const
{
	return (ArrLights.size() > id && ArrLights[id]);
}

void Lights::DeleteLight(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->IsGlobal)
		GlobalLight = -1;


	ArrLights[id]->IsEnable = false;
	ArrDelLights.push_back(ArrLights[id]);
	ArrLights[id] = 0;
	ArrFreeIDs.push_back(id);
}

char* Lights::GetLightName(ID id)
{
	LIGHTS_PRE_COND_ID(id, 0);
	return ArrLights[id]->Name;
}

void Lights::SetLightName(ID id, const char* name)
{
	LIGHTS_PRE_COND_ID(id, _VOID);
	sprintf(ArrLights[id]->Name, "%s", name);
}

ID Lights::CreatePoint(ID id, const float3* center, float dist, const float3* color, bool isglobal, bool is_shadow/*, const char* bound_volume*/)
{
	if (GlobalLight != -1 && isglobal)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - light: global light exists, you can not create 2 global light sources", GEN_MSG_LOCATION);
		return -1;
	}

	Light* tmplight = 0;// new Light();

	if (id < 0)
		tmplight = new Light();
	else
	{
		tmplight = new Light();
		tmplight->Id = ArrLights[id]->Id;
		mem_delete(ArrLights[id]);
		ArrLights[id] = tmplight;
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

	tmplight->TypeLight = (isglobal ? LTYPE_LIGHT_GLOBAL : LTYPE_LIGHT_POINT);
		/*if(StrValidate(bound_volume))
		{
			
		}
		else
		{*/
			D3DXCreateSphere(MLSet::DXDevice, tmplight->Dist, 20, 20, &tmplight->Mesh, 0);
		//}

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
			tmplight->TypeShadowed = LTYPE_SHADOW_DYNAMIC;
		else
			tmplight->TypeShadowed = LTYPE_SHADOW_NONE;
		
	ID tmpid = id;

	if (id == -1)
		tmpid = AddLight(tmplight);

	SetLightPos(tmpid, &float3(center->x, center->y, center->z));

	tmplight->Color = *color;
	tmplight->IsEnable = true;
	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->calcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	if (tmplight->IsGlobal)
		GlobalLight = tmpid;

	return tmpid;
}

ID Lights::CreateDirection(ID id, const float3* pos, float dist, const float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	Light* tmplight = 0;

	if (id < 0)
		tmplight = new Light();
	else
	{
		tmplight = new Light();
		tmplight->Id = ArrLights[id]->Id;
		mem_delete(ArrLights[id]);
		ArrLights[id] = tmplight;
	}

	tmplight->IsGlobal = false;
	tmplight->TypeLight = LTYPE_LIGHT_DIR;
	tmplight->Angle = angle;
	float bottom_radius = top_radius + dist * tanf(angle / 2.f);
	tmplight->TopBottomRadius.x = top_radius;
	tmplight->TopBottomRadius.y = bottom_radius;

	/*if (StrValidate(bound_volume))
	{
		
	}
	else*/
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
		tmplight->TypeShadowed = LTYPE_SHADOW_DYNAMIC;
	else
		tmplight->TypeShadowed = LTYPE_SHADOW_NONE;

	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->calcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	ID tmpid = id;

	if (id == -1)
		tmpid = AddLight(tmplight);
	
	return tmpid;
}

void Lights::Render(ID id, DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	float4x4 tmpwmat = ArrLights[id]->WorldMat;
	MLSet::DXDevice->SetTransform(D3DTS_WORLD, &(ArrLights[id]->WorldMat.operator D3DXMATRIX()));
	ArrLights[id]->Mesh->DrawSubset(0);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + (ArrLights[id]->Mesh->GetNumFaces() / 3));
}

ID Lights::GetLightGlobal() const
{
	return GlobalLight;
}

bool Lights::GetCastGlobalShadow() const
{
	return m_isCastGlobalShadow;
}

void Lights::SetCastGlobalShadow(bool isShadowed)
{
	m_isCastGlobalShadow = isShadowed;
}

void Lights::GetLightColor(ID id, float3* vec) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	(*vec) = ArrLights[id]->Color;
}

void Lights::SetLightColor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->Color = *vec;
}

void Lights::GetLightPos(ID id, float3* vec, bool greal) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->IsGlobal && !greal)
	{
		vec->x = ArrLights[id]->GAngleX;
		vec->y = ArrLights[id]->GAngleY;
		vec->z = 0;
	}
	else
	{
		vec->x = ArrLights[id]->Position.x;
		vec->y = ArrLights[id]->Position.y;
		vec->z = ArrLights[id]->Position.z;	
	}
}

float Lights::GetLightPower(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	float power = ArrLights[id]->Color.x;

	if (power < ArrLights[id]->Color.y)
		power = ArrLights[id]->Color.y;

	if (power < ArrLights[id]->Color.z)
		power = ArrLights[id]->Color.z;

	if (ArrLights[id]->IsGlobal)
		return power;
	else
	{
		return power * (ArrLights[id]->Dist / LIGHTS_LOCAL_MAX_DIST);
	}
}

float Lights::GetLightDist(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->Dist;
}

void Lights::SetLightDist(ID id, float radius_height, bool is_create)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->Dist = radius_height;
	if (ArrLights[id]->TypeLight == LTYPE_LIGHT_DIR && is_create)
	{
		ArrLights[id]->TopBottomRadius.y = ArrLights[id]->TopBottomRadius.x + radius_height * tanf(ArrLights[id]->Angle / 2.f);
		mem_release_del(ArrLights[id]->Mesh);
		SGCore_FCreateCone(ArrLights[id]->TopBottomRadius.x, ArrLights[id]->TopBottomRadius.y, radius_height, &ArrLights[id]->Mesh, 32);
	}

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Dist));

	if (ArrLights[id]->TypeLight == LTYPE_LIGHT_POINT && is_create)
	{
		mem_release_del(ArrLights[id]->Mesh);
		D3DXCreateSphere(MLSet::DXDevice, radius_height, 20, 20, &ArrLights[id]->Mesh, 0);
	}

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->SetNearFar(&float2(0.1, ArrLights[id]->Dist));

	if (ArrLights[id]->Mesh)
	{
		IDirect3DVertexBuffer9* vertexbuf;
		ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
		ArrLights[id]->BoundVolume->calcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
		mem_release_del(vertexbuf);
	}

	LightCountUpdateNull(id);
}

void Lights::SetLightPos(ID id, const float3* vec, bool greal)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->IsGlobal)
	{
		Light* tmplight = ArrLights[id];
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
				ArrLights[id]->Position.x = (*vec).x;
				ArrLights[id]->Position.y = (*vec).y;
				ArrLights[id]->Position.z = (*vec).z;

				float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
				ArrLights[id]->WorldMat = ArrLights[id]->Quaternion.GetMatrix() * mpos;

			if (ArrLights[id]->ShadowSM)
			{
				ArrLights[id]->ShadowSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}

			if (ArrLights[id]->ShadowCube)
			{
				ArrLights[id]->WorldMat = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
				ArrLights[id]->ShadowCube->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}
	}

	LightCountUpdateNull(id);
}

void Lights::GetLightOrient(ID id, SMQuaternion* q) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	*q = ArrLights[id]->Quaternion;
}

void Lights::SetLightOrient(ID id, const SMQuaternion* q)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->Quaternion = *q;

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->SetDirection(&(ArrLights[id]->Quaternion * LIGHTS_DIR_BASE));

	LightCountUpdateNull(id);
}

void Lights::SetShadowBias(ID id, float val)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetBias(val);
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetBias(val);

		LightCountUpdateNull(id);
}

float Lights::GetShadowBias(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->GetBias();
	else if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->GetBias();
	return(0);
}

float Lights::GetLightTopRadius(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->TopBottomRadius.x;
	return(0);
}

float Lights::GetLightAngle(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->Angle;
	return(0);
}


bool Lights::ComVisibleForFrustum(ID id, const ISXFrustum* frustum)
{
	LIGHTS_PRE_COND_ID(id, false);

	float3 tmpcenter;
	float tmpradius;
	ArrLights[id]->BoundVolume->getSphere(&tmpcenter, &tmpradius);
	tmpcenter = SMVector3Transform(tmpcenter, ArrLights[id]->WorldMat);

	return frustum->sphereInFrustum(&tmpcenter, tmpradius);
}

bool Lights::GetVisibleForFrustum(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsVisibleFor;
}

float Lights::ComDistFor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return SMVector3Distance((float3)ArrLights[id]->Position, *vec);
}

void Lights::ComVisibleFrustumDistFor(const ISXFrustum* frustum, const float3* vec)
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i])
		{
			ArrLights[i]->DistFor = SMVector3Distance((float3)ArrLights[i]->Position, *vec);
			ArrLights[i]->IsVisibleFor = ComVisibleForFrustum(ArrLights[i]->Id, frustum);
		}
	}
}

float Lights::GetDistFor(ID id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->DistFor;
}

IDirect3DTexture9* Lights::GetShadow2()
{
	return SGCore_RTGetTexture((HowShadow == 1 ? ShadowMap2 : ShadowMap));
}

bool Lights::GetLightEnable(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsEnable;
}

void Lights::SetLightEnable(ID id, bool val)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->IsEnable = val;
	LightCountUpdateNull(id);
}

bool Lights::GetLightShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return (ArrLights[id]->TypeShadowed != LTYPE_SHADOW_NONE);
}

LTYPE_LIGHT Lights::GetLightType(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LTYPE_LIGHT_NONE);
	return ArrLights[id]->TypeLight;
}

void Lights::ShadowRenderBegin(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->Begin();
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->Begin();
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->Begin();
}

void Lights::ShadowRenderEnd(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->End();
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->End();
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->End();
}

void Lights::ShadowRenderPre(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->Pre(cube);
	else if (ArrLights[id]->ShadowPSSM && cube >= 0 && cube < 5)
		ArrLights[id]->ShadowPSSM->PreRender(cube);
}

void Lights::ShadowRenderPost(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->Post(cube);
}

void Lights::InitShaderOfTypeMaterial(ID id, int typemat, float4x4* wmat)
{
	if (!wmat)
		wmat = &SMMatrixIdentity();
	
	float4x4 tmpmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &tmpmat);
	tmpmat = SMMatrixTranspose((*wmat) * tmpmat);
	float4x4 tmpwmat = SMMatrixTranspose(*wmat);
	if (ArrLights[id]->ShadowSM || ArrLights[id]->ShadowPSSM)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreePSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreePSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect, "WorldViewProjection", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthSkinPSSMDirect);
		}
	}
	else if (ArrLights[id]->ShadowCube)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "WorldViewProjection", &tmpmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "LightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomCube);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube, "WorldViewProjection", &tmpmat);
			//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube, "LightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube, "WorldViewProjection", &tmpmat);
			//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube, "LightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube, "WorldViewProjection", &tmpmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube, "World", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube, "LightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthSkinCube);
		}
	}
}

ISXFrustum* Lights::GetLightFrustum(ID id, int how) const
{
	LIGHTS_PRE_COND_ID(id, 0);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrLights[id]->ShadowSM->Frustum;
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrLights[id]->ShadowCube->Frustums[how];
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrLights[id]->ShadowPSSM->Frustums[how];
		}
	return(NULL);
}

ISXFrustum* Lights::GetLightFrustumG(ID id, int split) const
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (ArrLights[id]->ShadowPSSM && split >= 0 && split < 4 && ArrLights[id]->ShadowPSSM->Frustums[split])
		return ArrLights[id]->ShadowPSSM->Frustums[split];
	return(NULL);
}

void Lights::UpdateLightGFrustums(ID id, int split, const float3* pos, const float3* dir)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->ShadowPSSM->UpdateFrustums(split, pos, dir);
}

void Lights::ShadowGen2(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->GenShadow2(SGCore_RTGetTexture(ShadowMap));
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->GenShadow2(SGCore_RTGetTexture(ShadowMap));
	else if (ArrLights[id]->ShadowPSSM)
	{
		if (!m_isCastGlobalShadow)
			ArrLights[id]->ShadowPSSM->GenShadow2(SGCore_RTGetTexture(ShadowMap));
		else
			ArrLights[id]->ShadowPSSM->GenShadowAll(SGCore_RTGetTexture(ShadowMap));
	}
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
	LIGHTS_PRE_COND_ID(id, _VOID);

		ArrLights[id]->Angle = angle;
		if (ArrLights[id]->TypeLight == LTYPE_LIGHT_DIR && is_create)
		{
			mem_release_del(ArrLights[id]->Mesh);
			ArrLights[id]->TopBottomRadius.y = ArrLights[id]->TopBottomRadius.x + ArrLights[id]->Dist * tanf(angle / 2.f);
			ArrLights[id]->Angle = angle;
			SGCore_FCreateCone(ArrLights[id]->TopBottomRadius.x, ArrLights[id]->TopBottomRadius.y, ArrLights[id]->Dist, &ArrLights[id]->Mesh, 32);
		}

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->calcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release(vertexbuf);
		}

		if (ArrLights[id]->TypeLight == LTYPE_LIGHT_DIR && ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(angle, 0.1, ArrLights[id]->Dist));

		LightCountUpdateNull(id);
}

void Lights::SetLightTopRadius(ID id, float top_radius)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->TypeLight == 1)
		{
			mem_release_del(ArrLights[id]->Mesh);
			ArrLights[id]->TopBottomRadius.x = top_radius;
			SGCore_FCreateCone(top_radius, ArrLights[id]->TopBottomRadius.y, ArrLights[id]->Dist, &ArrLights[id]->Mesh, 32);
		}

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->calcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		LightCountUpdateNull(id);
}

void Lights::SetShadowBlurPixel(ID id, float blur_pixel)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->SetBlurPixel(blur_pixel);

		LightCountUpdateNull(id);
}

float Lights::GetShadowBlurPixel(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->GetBlurPixel();
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetBlurPixel();
		else if (ArrLights[id]->ShadowPSSM)
			return ArrLights[id]->ShadowPSSM->GetBlurPixel();
	return(0);
}

void Lights::SetShadowLocalNear(ID id, float slnear)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->SetNear(slnear);
	else if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->SetNear(slnear);

	LightCountUpdateNull(id);
}

float Lights::GetShadowLocalNear(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->GetNear();
	else if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->GetNear();
	return(0);
}

void Lights::SetShadowLocalFar(ID id, float slfar)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
	{
		float2 tmpnf;
		ArrLights[id]->ShadowCube->GetNearFar(&tmpnf);
		tmpnf.y = slfar;
		ArrLights[id]->ShadowCube->SetNearFar(&tmpnf);
	}
	else if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->SetFar(slfar);

	LightCountUpdateNull(id);
}

float Lights::GetShadowLocalFar(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowCube)
	{
		float2 tmpnf;
		ArrLights[id]->ShadowCube->GetNearFar(&tmpnf);
		return tmpnf.y;
	}
	else if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->GetFar();
	else
		return ArrLights[id]->Dist;
}

void Lights::SetLightCubeEdgeEnable(ID id, int edge, bool enable)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->SetEnableCubeEdge(edge, enable);

	LightCountUpdateNull(id);
}

bool Lights::GetLightCubeEdgeEnable(ID id, int edge) const
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->GetEnableCubeEdge(edge);
	return(false);
}

ID Lights::GetLightIDArr(ID id, ID inid, int how)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrLights[id]->ShadowSM->GetIDArr(inid);
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrLights[id]->ShadowCube->GetIDArr(inid, how);
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrLights[id]->ShadowPSSM->GetIDArr(inid, how);
		}
	return(-1);
}

void Lights::SetLightIDArr(ID id, ID inid, int how, ID id_arr)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				ArrLights[id]->ShadowSM->SetIDArr(inid, id_arr);
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				ArrLights[id]->ShadowCube->SetIDArr(inid, how, id_arr);
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				ArrLights[id]->ShadowPSSM->SetIDArr(inid, how, id_arr);
		}
}

void Lights::SetLightTypeShadowed(ID id, LTYPE_SHADOW type)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->TypeShadowed = type;

	if (ArrLights[id]->TypeLight == LTYPE_LIGHT_DIR)
	{
		if (!ArrLights[id]->ShadowSM)
		{
			ArrLights[id]->ShadowSM = new ShadowMapTech();
			ArrLights[id]->ShadowSM->Init();
			ArrLights[id]->ShadowSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			ArrLights[id]->ShadowSM->SetDirection(&(ArrLights[id]->Quaternion * LIGHTS_DIR_BASE));
			ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Dist));
		}
	}
	else if (ArrLights[id]->TypeLight == LTYPE_LIGHT_GLOBAL)
	{
		if (ArrLights[id]->IsGlobal)
		{
			if (!ArrLights[id]->ShadowPSSM)
			{
				ArrLights[id]->ShadowPSSM = new PSSM();
				ArrLights[id]->ShadowPSSM->Init();
				ArrLights[id]->ShadowPSSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}
		}
	}
	else if (ArrLights[id]->TypeLight == LTYPE_LIGHT_POINT)
	{
		if (!ArrLights[id]->ShadowCube)
		{
			ArrLights[id]->ShadowCube = new ShadowMapCubeTech();
			ArrLights[id]->ShadowCube->Init();
			ArrLights[id]->ShadowCube->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			ArrLights[id]->ShadowCube->SetNearFar(&float2(0.1f, ArrLights[id]->Dist));
		}
	}

	LightCountUpdateNull(id);
}

LTYPE_SHADOW Lights::GetLightTypeShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LTYPE_SHADOW_NONE);

	return ArrLights[id]->TypeShadowed;
}


bool Lights::LightCountUpdateUpdate(ID id, const float3* viewpos, int ghow)
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			Light* tmpl = ArrLights[id];
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

		else if (ArrLights[id]->TypeShadowed == LTYPE_SHADOW_STATIC)
		{
			if (ArrLights[id]->CountUpdate < LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
			{
				++(ArrLights[id]->CountUpdate);
				return true;
			}
			else
				return false;
		}
		else
		{
			Light* tmpl = ArrLights[id];

			float3 tmpcenter;
			float tmpradius;

			tmpl->BoundVolume->getSphere(&tmpcenter, &tmpradius);
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
	return(false);
}

bool Lights::LightCountUpdateAllowed(ID id, int ghow) const
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			if (ArrLights[id]->ShadowPSSM)
			{
				return (ArrLights[id]->ShadowPSSM->IsUpdate[ghow] == 0);
			}
		}
		else if (ArrLights[id]->TypeShadowed == LTYPE_SHADOW_STATIC)
		{
			if (ArrLights[id]->CountUpdate >= LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
				return false;
		}
	return(ArrLights[id]->CountUpdate == 0);
}

void Lights::LightCountUpdateNull(ID id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		LIGHTS_PRE_COND_ID(id, _VOID);
		ArrLights[id]->CountUpdate = 0;
	}
	else if (id < 0)
	{
		for (int i = 0; i < ArrLights.size(); ++i)
		{
			if (ArrLights[i])
				ArrLights[i]->CountUpdate = 0;
		}
	}
}

void Lights::ShadowSoft(bool randomsam, float size, bool isfirst)
{
	if (isfirst)
		HowShadow = 0;

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

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
	
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);

		if(randomsam)
		{
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
			MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise);
		}
		else
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "PixelSize", &float2_t(size / float(*r_win_width), size / float(*r_win_height)));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "NearFar", &float2_t(*r_near, *r_far));
		}
		else
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBased, "PixelSize", &float2_t(size / float(*r_win_width), size / float(*r_win_height)));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBased, "NearFar", &float2_t(*r_near, *r_far));
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
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	MLSet::GetArrDownScale4x4(*r_win_width, *r_win_height, MLSet::HDRSampleOffsets);

	LPDIRECT3DSURFACE9 SurfSceneScale, BackBuf;

	SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthComScaled)->GetSurfaceLevel(0, &SurfSceneScale);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, SurfSceneScale);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumIterative);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumIterative, "ArrOffsets", &(MLSet::HDRSampleOffsets));

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

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumInit);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumInit, "ArrOffsets", &(MLSet::HDRSampleOffsets));

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

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumIterative);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumIterative, "ArrOffsets", &(MLSet::HDRSampleOffsets));

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

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::CalcAdaptedLum);

	float ElapsedTime = float(timeDelta) * 0.001f * (factor_adapted * 1000.f);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::CalcAdaptedLum, "ElapsedTime", &(ElapsedTime));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(SurfAdaptedLum);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	MLSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	
}

void Lights::Set4Or3Splits(ID id, bool is4)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->Set4Or3Splits(is4);
}

bool Lights::Get4Or3Splits(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrLights[id]->ShadowPSSM)
		return ArrLights[id]->ShadowPSSM->Get4Or3Splits();

	return false;
}

//##########################################################################

int Lights::DelGetCount()
{
	return ArrDelLights.size();
}

LTYPE_LIGHT Lights::DelGetType(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, LTYPE_LIGHT_NONE);
	return ArrDelLights[key]->TypeLight;
}

void Lights::DelDel(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, _VOID);
	mem_delete(ArrDelLights[key]);
	ArrDelLights.erase(key);
}

ID Lights::DelGetIDArr(ID key, ID inid, int how)
{
	LIGHTS_PRE_COND_KEY_DEL(key, -1);

	if (ArrDelLights[key]->ShadowSM)
	{
		if (how == 0)
			return ArrDelLights[key]->ShadowSM->GetIDArr(inid);
	}
	else if (ArrDelLights[key]->ShadowCube)
	{
		if (how >= 0 && how < 6)
			return ArrDelLights[key]->ShadowCube->GetIDArr(inid, how);
	}
	else if (ArrDelLights[key]->ShadowPSSM)
	{
		if (how >= 0 && how < 5)
			return ArrDelLights[key]->ShadowPSSM->GetIDArr(inid, how);
	}
	return(-1);
}