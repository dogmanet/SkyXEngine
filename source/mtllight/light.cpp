
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "light.h"

CLights::CLights()
{
	HowShadow = 0;

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	ShadowMap = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap", 1);
	ShadowMap2 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap2", 1);

	GlobalLight = -1;
	m_isCastGlobalShadow = false;
}

CLights::~CLights()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		mem_delete(ArrLights[i]);
	}

	ArrLights.clear();

	SGCore_RTDelete(ShadowMap);
	SGCore_RTDelete(ShadowMap2);
}

ID CLights::createCopy(ID id)
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
			tmplight2->ShadowPSSM->init();
			tmplight2->GAngleX = tmplight->GAngleX;
			tmplight2->GAngleY = tmplight->GAngleY;
			tmplight2->ShadowPSSM->setPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			memcpy((tmplight2->ShadowPSSM->m_aNearFar), (tmplight->ShadowPSSM->m_aNearFar), sizeof(float2)* 5);
			memcpy((tmplight2->ShadowPSSM->m_aIsUpdate), (tmplight->ShadowPSSM->m_aIsUpdate), sizeof(int)* 5);
		}
		else
			tmplight2->ShadowPSSM = 0;

		if (tmplight->ShadowSM)
		{
			tmplight2->ShadowSM = new ShadowMapTech();
			tmplight2->ShadowSM->init();
			tmplight2->ShadowSM->setPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowSM->setDirection(&(tmplight->Quaternion * LIGHTS_DIR_BASE));
			
			tmplight2->ShadowSM->setBias(tmplight->ShadowSM->getBias());
			tmplight2->ShadowSM->setBlurPixel(tmplight->ShadowSM->getBlurPixel());
			float3 anf;
			tmplight2->ShadowSM->getAngleNearFar(&anf);
			tmplight2->ShadowSM->setAngleNearFar(&anf);
		}
		else
			tmplight2->ShadowSM = 0;

		if (tmplight->ShadowCube)
		{
			tmplight2->ShadowCube = new ShadowMapCubeTech();
			tmplight2->ShadowCube->init();
			tmplight2->ShadowCube->setPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowCube->setNearFar(&float2(0.1f, tmplight->Dist));
			tmplight2->ShadowCube->setNear(tmplight2->ShadowCube->getNear());
			tmplight2->ShadowCube->setFar(tmplight2->ShadowCube->getFar());
			tmplight2->ShadowCube->setBias(tmplight->ShadowCube->getBias());
			tmplight2->ShadowCube->setBlurPixel(tmplight->ShadowCube->getBlurPixel());

			for (int i = 0; i < 6; ++i)
			{
				tmplight2->ShadowCube->setEnableCubeEdge(i, tmplight2->ShadowCube->getEnableCubeEdge(i));
			}
		}
		else
			tmplight2->ShadowCube = 0;

		tmplight->Mesh->CloneMeshFVF(tmplight->Mesh->GetOptions(), tmplight->Mesh->GetFVF(), MLSet::DXDevice, &(tmplight2->Mesh));

	return addLight(tmplight);
}

CLights::Light::Light()
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

CLights::Light::~Light()
{
	mem_release_del(Mesh);
	mem_release_del(BoundVolume);
	
	mem_delete(ShadowPSSM);
	mem_delete(ShadowSM);
	mem_delete(ShadowCube);
}

void CLights::onLostDevice()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->onLostDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->onLostDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->onLostDevice();
	}
}

void CLights::onResetDevice()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->onResetDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->onResetDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->onResetDevice();

		lightCountUpdateNull(i);
	}
}

ID CLights::addLight(Light* obj)
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

int CLights::getCountLights() const 
{
	return ArrLights.size();
}

void CLights::clear()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		mem_delete(ArrLights[i]);
	}

	ArrLights.clear();
	GlobalLight = -1;
}

void CLights::clearIDArr()
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (!ArrLights[i])
			continue;

		if (ArrLights[i]->ShadowSM)
		{
			for (int k = 0; k < ArrLights[i]->ShadowSM->getCountIDArrs(); ++k)
			{
				ArrLights[i]->ShadowSM->setIDArr(k, -1);
			}
		}
		else if (ArrLights[i]->ShadowCube)
		{
			for (int k = 0; k < ArrLights[i]->ShadowCube->getCountIDArrs(); ++k)
			{
				ArrLights[i]->ShadowCube->setIDArr(k, 0, -1);
				ArrLights[i]->ShadowCube->setIDArr(k, 1, -1);
				ArrLights[i]->ShadowCube->setIDArr(k, 2, -1);
				ArrLights[i]->ShadowCube->setIDArr(k, 3, -1);
				ArrLights[i]->ShadowCube->setIDArr(k, 4, -1);
				ArrLights[i]->ShadowCube->setIDArr(k, 5, -1);
			}
		}
		else if (ArrLights[i]->ShadowPSSM)
		{
			for (int k = 0; k < ArrLights[i]->ShadowPSSM->getCountIDArrs(); ++k)
			{
				ArrLights[i]->ShadowPSSM->setIDArr(k, 0, -1);
				ArrLights[i]->ShadowPSSM->setIDArr(k, 1, -1);
				ArrLights[i]->ShadowPSSM->setIDArr(k, 2, -1);
				ArrLights[i]->ShadowPSSM->setIDArr(k, 3, -1);
				ArrLights[i]->ShadowPSSM->setIDArr(k, 4, -1);
			}
		}
	}
}

bool CLights::getExists(ID id) const
{
	return (ArrLights.size() > id && ArrLights[id]);
}

void CLights::deleteLight(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->IsGlobal)
		GlobalLight = -1;


	ArrLights[id]->IsEnable = false;
	ArrDelLights.push_back(ArrLights[id]);
	ArrLights[id] = 0;
	ArrFreeIDs.push_back(id);
}

char* CLights::getLightName(ID id)
{
	LIGHTS_PRE_COND_ID(id, 0);
	return ArrLights[id]->Name;
}

void CLights::setLightName(ID id, const char* name)
{
	LIGHTS_PRE_COND_ID(id, _VOID);
	sprintf(ArrLights[id]->Name, "%s", name);
}

ID CLights::createPoint(ID id, const float3* center, float dist, const float3* color, bool isglobal, bool is_shadow/*, const char* bound_volume*/)
{
	if (GlobalLight != -1 && isglobal)
	{
		setLightPos(GlobalLight, center);
		setLightColor(GlobalLight, color);
		setLightTypeShadowed(GlobalLight, (is_shadow ? LTYPE_SHADOW_DYNAMIC : LTYPE_SHADOW_NONE));
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - light: global light exists, you can not create 2 global light sources\n", GEN_MSG_LOCATION);
		return GlobalLight;
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
			tmplight->ShadowPSSM->init();
			tmplight->ShadowPSSM->setPosition(&float3(center->x, center->y, center->z));
		}
		else if (is_shadow)
		{
			tmplight->ShadowCube = new ShadowMapCubeTech();
			tmplight->ShadowCube->init();
			tmplight->ShadowCube->setPosition(&float3(center->x, center->y, center->z));
			tmplight->ShadowCube->setNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, tmplight->Dist));
			
		}

		if (is_shadow)
			tmplight->TypeShadowed = LTYPE_SHADOW_DYNAMIC;
		else
			tmplight->TypeShadowed = LTYPE_SHADOW_NONE;
		
	ID tmpid = id;

	if (id == -1)
		tmpid = addLight(tmplight);

	setLightPos(tmpid, &float3(center->x, center->y, center->z));

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

ID CLights::createDirection(ID id, const float3* pos, float dist, const float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
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
		tmplight->ShadowSM->init();
		tmplight->ShadowSM->setPosition(&float3(pos->x, pos->y, pos->z));
		tmplight->ShadowSM->setDirection(&(tmplight->Quaternion * LIGHTS_DIR_BASE));
		tmplight->ShadowSM->setAngleNearFar(&float3(angle, LIGHTS_LOCAL_STD_NEAR, dist));
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
		tmpid = addLight(tmplight);
	
	return tmpid;
}

void CLights::render(ID id, DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	float4x4 tmpwmat = ArrLights[id]->WorldMat;
	MLSet::DXDevice->SetTransform(D3DTS_WORLD, &(ArrLights[id]->WorldMat.operator D3DXMATRIX()));
	ArrLights[id]->Mesh->DrawSubset(0);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + (ArrLights[id]->Mesh->GetNumFaces() / 3));
}

ID CLights::getLightGlobal() const
{
	return GlobalLight;
}

bool CLights::getCastGlobalShadow() const
{
	return m_isCastGlobalShadow;
}

void CLights::setCastGlobalShadow(bool isShadowed)
{
	m_isCastGlobalShadow = isShadowed;
}

void CLights::getLightColor(ID id, float3* vec) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	(*vec) = ArrLights[id]->Color;
}

void CLights::setLightColor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->Color = *vec;
}

void CLights::getLightPos(ID id, float3* vec, bool greal) const
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

float CLights::getLightPower(ID id) const
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

float CLights::getLightDist(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->Dist;
}

void CLights::setLightDist(ID id, float radius_height, bool is_create)
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
		ArrLights[id]->ShadowSM->setAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Dist));

	if (ArrLights[id]->TypeLight == LTYPE_LIGHT_POINT && is_create)
	{
		mem_release_del(ArrLights[id]->Mesh);
		D3DXCreateSphere(MLSet::DXDevice, radius_height, 20, 20, &ArrLights[id]->Mesh, 0);
	}

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->setNearFar(&float2(0.1, ArrLights[id]->Dist));

	if (ArrLights[id]->Mesh)
	{
		IDirect3DVertexBuffer9* vertexbuf;
		ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
		ArrLights[id]->BoundVolume->calcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
		mem_release_del(vertexbuf);
	}

	lightCountUpdateNull(id);
}

void CLights::setLightPos(ID id, const float3* vec, bool greal)
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
			tmplight->ShadowPSSM->setPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
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
				ArrLights[id]->ShadowSM->setPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}

			if (ArrLights[id]->ShadowCube)
			{
				ArrLights[id]->WorldMat = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
				ArrLights[id]->ShadowCube->setPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}
	}

	lightCountUpdateNull(id);
}

void CLights::getLightOrient(ID id, SMQuaternion* q) const
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	*q = ArrLights[id]->Quaternion;
}

void CLights::setLightOrient(ID id, const SMQuaternion* q)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->Quaternion = *q;

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->setDirection(&(ArrLights[id]->Quaternion * LIGHTS_DIR_BASE));

	lightCountUpdateNull(id);
}

void CLights::setShadowBias(ID id, float val)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->setBias(val);
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->setBias(val);

		lightCountUpdateNull(id);
}

float CLights::getShadowBias(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->getBias();
	else if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->getBias();
	return(0);
}

float CLights::getLightTopRadius(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->TopBottomRadius.x;
	return(0);
}

float CLights::getLightAngle(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->Angle;
	return(0);
}


bool CLights::comVisibleForFrustum(ID id, const IFrustum* frustum)
{
	LIGHTS_PRE_COND_ID(id, false);

	float3 tmpcenter;
	float tmpradius;
	ArrLights[id]->BoundVolume->getSphere(&tmpcenter, &tmpradius);
	tmpcenter = SMVector3Transform(tmpcenter, ArrLights[id]->WorldMat);

	return frustum->sphereInFrustum(&tmpcenter, tmpradius);
}

bool CLights::getVisibleForFrustum(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsVisibleFor;
}

float CLights::comDistFor(ID id, const float3* vec)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return SMVector3Distance((float3)ArrLights[id]->Position, *vec);
}

void CLights::comVisibleFrustumDistFor(const IFrustum* frustum, const float3* vec)
{
	for (int i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i])
		{
			ArrLights[i]->DistFor = SMVector3Distance((float3)ArrLights[i]->Position, *vec);
			ArrLights[i]->IsVisibleFor = comVisibleForFrustum(ArrLights[i]->Id, frustum);
		}
	}
}

float CLights::getDistFor(ID id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->DistFor;
}

IDirect3DTexture9* CLights::getShadow2()
{
	return SGCore_RTGetTexture((HowShadow == 1 ? ShadowMap2 : ShadowMap));
}

bool CLights::getLightEnable(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsEnable;
}

void CLights::setLightEnable(ID id, bool val)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->IsEnable = val;
	lightCountUpdateNull(id);
}

bool CLights::getLightShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, false);

	return (ArrLights[id]->TypeShadowed != LTYPE_SHADOW_NONE);
}

LTYPE_LIGHT CLights::getLightType(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LTYPE_LIGHT_NONE);
	return ArrLights[id]->TypeLight;
}

void CLights::shadowRenderBegin(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->begin();
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->begin();
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->begin();
}

void CLights::shadowRenderEnd(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->end();
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->end();
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->end();
}

void CLights::shadowRenderPre(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->pre(cube);
	else if (ArrLights[id]->ShadowPSSM && cube >= 0 && cube < 5)
		ArrLights[id]->ShadowPSSM->preRender(cube);
}

void CLights::shadowRenderPost(ID id, int cube)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->post(cube);
}

void CLights::initShaderOfTypeMaterial(ID id, int typemat, const float4x4* wmat)
{
	float4x4 mWorld = (wmat ? (*wmat) : SMMatrixIdentity());
	
	float4x4 tmpmat;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &tmpmat);
	tmpmat = SMMatrixTranspose(mWorld * tmpmat);
	float4x4 tmpwmat = SMMatrixTranspose(mWorld);
	if (ArrLights[id]->ShadowSM || ArrLights[id]->ShadowPSSM)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreePSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreePSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect, "g_mWVP", &tmpmat);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthSkinPSSMDirect);
		}
	}
	else if (ArrLights[id]->ShadowCube)
	{
		if (typemat == MTL_TYPE_GEOM)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "g_mWVP", &tmpmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "g_vLightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomCube);
		}
		else if (typemat == MTL_TYPE_GRASS)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube, "g_mWVP", &tmpmat);
			//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube, "g_vLightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGrassCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_TREE)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube, "g_mWVP", &tmpmat);
			//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube, "g_vLightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthTreeCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGreenCube);
		}
		else if (typemat == MTL_TYPE_SKIN)
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube, "g_mWVP", &tmpmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube, "g_mW", &tmpwmat);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube, "g_vLightPos", &ArrLights[id]->Position);
			SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthSkinCube);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthSkinCube);
		}
	}
}

IFrustum* CLights::getLightFrustum(ID id, int how) const
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
				return ArrLights[id]->ShadowCube->m_aFrustums[how];
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrLights[id]->ShadowPSSM->m_aFrustums[how];
		}
	return(NULL);
}

IFrustum* CLights::getLightFrustumG(ID id, int split) const
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (ArrLights[id]->ShadowPSSM && split >= 0 && split < 4 && ArrLights[id]->ShadowPSSM->m_aFrustums[split])
		return ArrLights[id]->ShadowPSSM->m_aFrustums[split];
	return(NULL);
}

void CLights::updateLightGFrustums(ID id, int split, const float3* pos, const float3* dir)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	ArrLights[id]->ShadowPSSM->updateFrustums(split, pos, dir);
}

void CLights::shadowGen2(ID id)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->genShadow(SGCore_RTGetTexture(ShadowMap));
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->genShadow(SGCore_RTGetTexture(ShadowMap));
	else if (ArrLights[id]->ShadowPSSM)
	{
		if (!m_isCastGlobalShadow)
			ArrLights[id]->ShadowPSSM->genShadow(SGCore_RTGetTexture(ShadowMap));
		else
			ArrLights[id]->ShadowPSSM->genShadowAll(SGCore_RTGetTexture(ShadowMap));
	}
}

void CLights::shadowNull()
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

void CLights::setLightAngle(ID id, float angle, bool is_create)
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
			ArrLights[id]->ShadowSM->setAngleNearFar(&float3(angle, 0.1, ArrLights[id]->Dist));

		lightCountUpdateNull(id);
}

void CLights::setLightTopRadius(ID id, float top_radius)
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

		lightCountUpdateNull(id);
}

void CLights::setShadowBlurPixel(ID id, float blur_pixel)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->setBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->setBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->setBlurPixel(blur_pixel);

		lightCountUpdateNull(id);
}

float CLights::getShadowBlurPixel(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->getBlurPixel();
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->getBlurPixel();
		else if (ArrLights[id]->ShadowPSSM)
			return ArrLights[id]->ShadowPSSM->getBlurPixel();
	return(0);
}

void CLights::setShadowLocalNear(ID id, float slnear)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->setNear(slnear);
	else if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->setNear(slnear);

	lightCountUpdateNull(id);
}

float CLights::getShadowLocalNear(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->getNear();
	else if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->getNear();
	return(0);
}

void CLights::setShadowLocalFar(ID id, float slfar)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
	{
		float2 tmpnf;
		ArrLights[id]->ShadowCube->getNearFar(&tmpnf);
		tmpnf.y = slfar;
		ArrLights[id]->ShadowCube->setNearFar(&tmpnf);
	}
	else if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->setFar(slfar);

	lightCountUpdateNull(id);
}

float CLights::getShadowLocalFar(ID id) const
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowCube)
	{
		float2 tmpnf;
		ArrLights[id]->ShadowCube->getNearFar(&tmpnf);
		return tmpnf.y;
	}
	else if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->getFar();
	else
		return ArrLights[id]->Dist;
}

void CLights::setLightCubeEdgeEnable(ID id, int edge, bool enable)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->setEnableCubeEdge(edge, enable);

	lightCountUpdateNull(id);
}

bool CLights::getLightCubeEdgeEnable(ID id, int edge) const
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->getEnableCubeEdge(edge);
	return(false);
}

ID CLights::getLightIDArr(ID id, ID inid, int how)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrLights[id]->ShadowSM->getIDArr(inid);
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrLights[id]->ShadowCube->getIDArr(inid, how);
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrLights[id]->ShadowPSSM->getIDArr(inid, how);
		}
	return(-1);
}

void CLights::setLightIDArr(ID id, ID inid, int how, ID id_arr)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				ArrLights[id]->ShadowSM->setIDArr(inid, id_arr);
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				ArrLights[id]->ShadowCube->setIDArr(inid, how, id_arr);
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				ArrLights[id]->ShadowPSSM->setIDArr(inid, how, id_arr);
		}
}

void CLights::setLightTypeShadowed(ID id, LTYPE_SHADOW type)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->TypeShadowed == type)
		return;

	ArrLights[id]->TypeShadowed = type;

	if (ArrLights[id]->TypeLight == LTYPE_LIGHT_DIR)
	{
		if (!ArrLights[id]->ShadowSM)
		{
			ArrLights[id]->ShadowSM = new ShadowMapTech();
			ArrLights[id]->ShadowSM->init();
			ArrLights[id]->ShadowSM->setPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			ArrLights[id]->ShadowSM->setDirection(&(ArrLights[id]->Quaternion * LIGHTS_DIR_BASE));
			ArrLights[id]->ShadowSM->setAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Dist));
		}
	}
	else if (ArrLights[id]->TypeLight == LTYPE_LIGHT_GLOBAL)
	{
		if (ArrLights[id]->IsGlobal)
		{
			if (!ArrLights[id]->ShadowPSSM)
			{
				ArrLights[id]->ShadowPSSM = new PSSM();
				ArrLights[id]->ShadowPSSM->init();
				ArrLights[id]->ShadowPSSM->setPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}
		}
	}
	else if (ArrLights[id]->TypeLight == LTYPE_LIGHT_POINT)
	{
		if (!ArrLights[id]->ShadowCube)
		{
			ArrLights[id]->ShadowCube = new ShadowMapCubeTech();
			ArrLights[id]->ShadowCube->init();
			ArrLights[id]->ShadowCube->setPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			ArrLights[id]->ShadowCube->setNearFar(&float2(0.1f, ArrLights[id]->Dist));
		}
	}

	lightCountUpdateNull(id);
}

LTYPE_SHADOW CLights::getLightTypeShadowed(ID id) const
{
	LIGHTS_PRE_COND_ID(id, LTYPE_SHADOW_NONE);

	return ArrLights[id]->TypeShadowed;
}


bool CLights::lightCountUpdateUpdate(ID id, const float3* viewpos, int ghow)
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			Light* tmpl = ArrLights[id];
			if (tmpl->ShadowPSSM)
			{
				if (ghow >= 0 && ghow < 4)
				{
					if (tmpl->ShadowPSSM->m_aIsUpdate[ghow] >= ghow*LIGHTS_UPDATE_PSSM_SPLIT)
						tmpl->ShadowPSSM->m_aIsUpdate[ghow] = -1;
				}
				else
				{
					if (tmpl->ShadowPSSM->m_aIsUpdate[ghow] >= LIGHTS_UPDATE_PSSM_SPLIT_5)
						tmpl->ShadowPSSM->m_aIsUpdate[ghow] = -1;
					
				}

				++(tmpl->ShadowPSSM->m_aIsUpdate[ghow]);
				
				return (tmpl->ShadowPSSM->m_aIsUpdate[ghow] == 0);
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

bool CLights::lightCountUpdateAllowed(ID id, int ghow) const
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			if (ArrLights[id]->ShadowPSSM)
			{
				return (ArrLights[id]->ShadowPSSM->m_aIsUpdate[ghow] == 0);
			}
		}
		else if (ArrLights[id]->TypeShadowed == LTYPE_SHADOW_STATIC)
		{
			if (ArrLights[id]->CountUpdate >= LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
				return false;
		}
	return(ArrLights[id]->CountUpdate == 0);
}

void CLights::lightCountUpdateNull(ID id)
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

void CLights::shadowSoft(bool randomsam, float size, bool isfirst)
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
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "g_vPixelSize", &float2_t(size / float(*r_win_width), size / float(*r_win_height)));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "g_vNearFar", &float2_t(*r_near, *r_far));
		}
		else
		{
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBased, "g_vPixelSize", &float2_t(size / float(*r_win_width), size / float(*r_win_height)));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PPBlurDepthBased, "g_vNearFar", &float2_t(*r_near, *r_far));
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

void CLights::toneMappingCom(DWORD timeDelta, float factor_adapted)
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
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumIterative, "g_aOffsets", &(MLSet::HDRSampleOffsets));

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
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumInit, "g_aOffsets", &(MLSet::HDRSampleOffsets));

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
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SampleLumIterative, "g_aOffsets", &(MLSet::HDRSampleOffsets));

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
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::CalcAdaptedLum, "g_fElapsedTime", &(ElapsedTime));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();
	mem_release(SurfAdaptedLum);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	MLSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	
}

void CLights::set4Or3Splits(ID id, bool is4)
{
	LIGHTS_PRE_COND_ID(id, _VOID);

	if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->set4Or3Splits(is4);
}

bool CLights::get4Or3Splits(ID id)
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrLights[id]->ShadowPSSM)
		return ArrLights[id]->ShadowPSSM->get4Or3Splits();

	return false;
}

//##########################################################################

int CLights::delGetCount()
{
	return ArrDelLights.size();
}

LTYPE_LIGHT CLights::delGetType(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, LTYPE_LIGHT_NONE);
	return ArrDelLights[key]->TypeLight;
}

void CLights::delDel(ID key)
{
	LIGHTS_PRE_COND_KEY_DEL(key, _VOID);
	mem_delete(ArrDelLights[key]);
	ArrDelLights.erase(key);
}

ID CLights::delGetIDArr(ID key, ID inid, int how)
{
	LIGHTS_PRE_COND_KEY_DEL(key, -1);

	if (ArrDelLights[key]->ShadowSM)
	{
		if (how == 0)
			return ArrDelLights[key]->ShadowSM->getIDArr(inid);
	}
	else if (ArrDelLights[key]->ShadowCube)
	{
		if (how >= 0 && how < 6)
			return ArrDelLights[key]->ShadowCube->getIDArr(inid, how);
	}
	else if (ArrDelLights[key]->ShadowPSSM)
	{
		if (how >= 0 && how < 5)
			return ArrDelLights[key]->ShadowPSSM->getIDArr(inid, how);
	}
	return(-1);
}