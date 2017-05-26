
#include <particles/effect.h>

#pragma once

Effects::Effects()
{
	
}

Effects::~Effects()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			mem_delete(ArrID[i]);
		}
	}

	ArrID.clear();
}

Effects::Effect::Effect()
{
	Id = Key = -1;
	DistForCamera = 0.0f;
	IsRenderForCamera = false;

	IDLight = -1;
	IsAlife = false;
	IsEnable = true;
}

Effects::Effect::~Effect()
{
	for (int i = 0; i < Arr.size(); ++i)
	{
		mem_delete(Arr[i]);
	}

	Arr.clear();
}

void Effects::OnLostDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->Arr.size(); ++k)
			{
				if (ArrID[i]->Arr[k])
					ArrID[i]->Arr[k]->OnLostDevice();
			}
		}
	}
}

void Effects::OnResetDevice()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i])
		{
			for (int k = 0; k < ArrID[i]->Arr.size(); ++k)
			{
				if (ArrID[i]->Arr[k])
					ArrID[i]->Arr[k]->OnResetDevice();
			}
		}
	}
}

void Effects::ParticlesNameSet(ID id, ID id_part, const char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->NameSet(name);
}

void Effects::ParticlesNameGet(ID id, ID id_part, char* name)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->NameGet(name);
}

ID Effects::ParticlesAdd(ID id, ParticlesData* data)
{
	EFFECTS_EFFECT_PRECOND(id, -1);

	Particles* tmppart = new Particles();
	tmppart->Init(data);
	ArrID[id]->Arr.push_back(tmppart);
	return ArrID[id]->Arr.size() - 1;
}

void Effects::ParticlesReInit(ID id, ID id_part, ParticlesData* data)
{
	EFFECTS_PARTICLES_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->Init(data);
}

int Effects::ParticlesGetCount(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, -1);
	return ArrID[id]->Arr.size();
}

void Effects::ParticlesDelete(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, _VOID);
}

ParticlesData* Effects::ParticlesGetData(ID id, ID id_part)
{
	EFFECTS_PRECOND(id, id_part, 0);

	return ArrID[id]->Arr[id_part]->GetData();
}

void Effects::ParticlesCreate(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->Create(count);
}

void Effects::ParticlesReCreate(ID id, ID id_part, int count)
{
	EFFECTS_PRECOND(id, id_part,  _VOID);

	ArrID[id]->Arr[id_part]->ReCreate(count);
}

void Effects::ParticlesTextureSet(ID id, ID id_part, ID tex)
{
	EFFECTS_PRECOND(id, id_part, _VOID);

	ArrID[id]->Arr[id_part]->SetTexture(tex);
}



ID Effects::EffectAdd(const char* name)
{
	Effect* tmpeffect = new Effect();
	if (name)
		strcpy(tmpeffect->Name, name);

	return AddEffect(tmpeffect);
}

ID Effects::AddEffect(Effect* obj)
{
	ID idadd = -1;
	for (long i = 0; i < ArrID.size(); ++i)
	{
		if (ArrID[i] == 0)
		{
			ArrID[i] = obj;
			idadd = i;
			break;
		}
	}

	if (idadd == -1)
	{
		ArrID.push_back(obj);
		idadd = ArrID.size() - 1;
	}

	obj->Id = idadd;
	ArrKey.push_back(obj);
	obj->Key = ArrKey.size() - 1;
	return idadd;
}

int Effects::EffectCountGet()
{
	return ArrKey.size();
}

ID Effects::EffectIdOfKey(ID key)
{
	EFFECTS_EFFECT_PRECOND_KEY(key, -1);

	return ArrKey[key]->Id;
}

void Effects::EffectDelete(ID id)
{
	for (long i = ArrID[id]->Key + 1; i < ArrKey.size(); ++i)
	{
		--(ArrKey[i]->Key);
	}

	ArrID[id]->IsEnable = false;
	ArrKey.erase(ArrID[id]->Key);
	mem_delete(ArrID[id]);
}

void Effects::EffectNameSet(ID id, const char* name)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (name)
		strcpy(ArrID[id]->Name, name);
}

void Effects::EffectNameGet(ID id, char* name)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (name)
		strcpy(name, ArrID[id]->Name);
}

void Effects::EffectCompute(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->IsEnable)
		return;

	int countdead = 0;	//счетик живых партиклов
	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		if (ArrID[id]->Arr[i])
			ArrID[id]->Arr[i]->Compute();

		//если партиклы метрвы то инкрементируем счетчик
		if (!ArrID[id]->Arr[i]->GetAlife())
			++countdead;
		else //иначе партиклы живы, считаем объем эффекта
		{
			if (ArrID[id]->Arr[i]->CurrMax.x > ArrID[id]->CurrMax.x)
				ArrID[id]->CurrMax.x = ArrID[id]->Arr[i]->CurrMax.x;
			
			if (ArrID[id]->Arr[i]->CurrMax.y > ArrID[id]->CurrMax.y)
				ArrID[id]->CurrMax.y = ArrID[id]->Arr[i]->CurrMax.y;

			if (ArrID[id]->Arr[i]->CurrMax.z > ArrID[id]->CurrMax.z)
				ArrID[id]->CurrMax.z = ArrID[id]->Arr[i]->CurrMax.z;

			if (ArrID[id]->Arr[i]->CurrMin.x < ArrID[id]->CurrMin.x)
				ArrID[id]->CurrMin.x = ArrID[id]->Arr[i]->CurrMin.x;

			if (ArrID[id]->Arr[i]->CurrMin.y < ArrID[id]->CurrMin.y)
				ArrID[id]->CurrMin.y = ArrID[id]->Arr[i]->CurrMin.y;

			if (ArrID[id]->Arr[i]->CurrMin.z < ArrID[id]->CurrMin.z)
				ArrID[id]->CurrMin.z = ArrID[id]->Arr[i]->CurrMin.z;
		}
	}

	//если количество мертвых объектов партиклов равно общему количесву объектов партиклов
	if (countdead == ArrID[id]->Arr.size())
		ArrID[id]->IsAlife = false;	//эффект мертв
	else
		ArrID[id]->IsAlife = true;

	EffectLightEnable(id, ArrID[id]->IsAlife);
}

void Effects::EffectComputeLighting(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->IsRenderForCamera)
		return;

	if (!ArrID[id]->IsEnable)
		return;

	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		if (ArrID[id]->Arr[i] && ArrID[id]->Arr[i]->GetAlife())
		{
			ArrID[id]->Arr[i]->ComputeLighting();
		}
	}
}

void Effects::EffectRender(ID id, DWORD timeDelta)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (!ArrID[id]->IsRenderForCamera)
		return;

	if (!ArrID[id]->IsEnable)
		return;

	for (int i = 0; i < ArrID[id]->Arr.size(); ++i)
	{
		if (ArrID[id]->Arr[i] && ArrID[id]->Arr[i]->GetAlife())
		{
			ArrID[id]->Arr[i]->Render(timeDelta, &ArrID[id]->MatRotate, &ArrID[id]->MatTranslation);
		}
	}
}

void Effects::EffectComputeAll()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectCompute(ArrID[i]->Id);
	}
}

void Effects::EffectComputeLightingAll()
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectComputeLighting(ArrID[i]->Id);
	}
}

void Effects::EffectRenderAll(DWORD timeDelta)
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectRender(ArrID[i]->Id, timeDelta);
	}
}

bool Effects::EffectVisibleCom(ID id, ISXFrustum* frustum, float3* view)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	float3 scenter;
	float sradius;

	scenter = (ArrID[id]->CurrMin + ArrID[id]->CurrMax) * 0.5f;
	sradius = SMVector3Length(scenter - ArrID[id]->CurrMax);

	ArrID[id]->IsRenderForCamera = frustum->SphereInFrustum(&scenter, sradius);

	ArrID[id]->DistForCamera = SMVector3Length((scenter - (*view))) - sradius;

	return ArrID[id]->IsRenderForCamera;
}

void Effects::EffectVisibleComAll(ISXFrustum* frustum, float3* view)
{
	for (int i = 0; i < ArrID.size(); ++i)
	{
		EffectVisibleCom(i, frustum, view);
	}
}

bool Effects::EffectVisibleGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->IsRenderForCamera;
}

float Effects::EffectDistToViewGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, 0);

	return ArrID[id]->DistForCamera;
}


bool Effects::EffectAlifeGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->IsAlife;
}

bool Effects::EffectEnableGet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, false);

	return ArrID[id]->IsEnable;
}

void Effects::EffectEnableSet(ID id, bool isenable)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	//если предыдущее состояние эффекта было "выключен", а теперь включаем
	if (!ArrID[id]->IsEnable && isenable)
	{
		//перерождаем все частицы
		for (int i = 0; i < ArrID.size(); ++i)
		{
			ArrID[id]->Arr[i]->ReCreate();
		}

		EffectLightEnable(id, true);
	}

	ArrID[id]->IsEnable = isenable;

	if (!ArrID[id]->IsEnable)
	{
		ArrID[id]->IsAlife = false;
		EffectLightEnable(id, false);
	}
}

void Effects::EffectPosSet(ID id, float3* pos)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->Position = *pos;
	ArrID[id]->MatTranslation = SMMatrixTranslation(ArrID[id]->Position);
}

void Effects::EffectDirSet(ID id, float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);
	float3 tmpdir = *dir;
	tmpdir.y = 0;
	tmpdir = SMVector3Normalize(tmpdir);
	ArrID[id]->Direction = tmpdir;

	static float3 f = SXPARTICLES_BASIS_DIR;
	//float3 f(0, 1, 0);
	float3 a = SMVector3Cross(f, tmpdir);
	float ang = acosf(SMVector3Dot(f, tmpdir));
	ArrID[id]->MatRotate = /*SMMatrixRotationX(SM_PIDIV2) * */SMMatrixRotationAxis(a, ang);

	ArrID[id]->Rotation = SMMatrixToEuler(ArrID[id]->MatRotate);
}

void Effects::EffectRotSet(ID id, float3* rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	ArrID[id]->Rotation = *rot;

	ArrID[id]->Direction = SMEulerToVec(ArrID[id]->Rotation);

	static float3 f = SXPARTICLES_BASIS_DIR;
	//float3 f(0, -1, 0);
	float3 a = SMVector3Cross(f, ArrID[id]->Direction);
	float ang = acosf(SMVector3Dot(f, ArrID[id]->Direction));
	ArrID[id]->MatRotate = SMMatrixRotationAxis(a, ang);
}

void Effects::EffectPosGet(ID id, float3* pos)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*pos = ArrID[id]->Position;
}

void Effects::EffectDirGet(ID id, float3* dir)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*dir = ArrID[id]->Direction;
}

void Effects::EffectRotGet(ID id, float3* rot)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	*rot = ArrID[id]->Rotation;
}





void Effects::EffectLightSet(ID id, float3* color, float dist, bool is_shadow)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
	{
		SML_LigthsSetColor(ArrID[id]->IDLight, color);
		SML_LigthsSetDist(ArrID[id]->IDLight, dist, true);
		SML_LigthsSetPower(ArrID[id]->IDLight, dist / LIGHTS_LOCAL_MAX_POWER);
		SML_LigthsSetShadow(ArrID[id]->IDLight, is_shadow);
	}
	else
		ArrID[id]->IDLight = SML_LigthsCreatePoint(&ArrID[id]->Position, dist / LIGHTS_LOCAL_MAX_POWER, dist, color, false, is_shadow);
}

void Effects::EffectLightDelete(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
	{
		SML_LigthsDeleteLight(ArrID[id]->IDLight);
		ArrID[id]->IDLight = -1;
	}
}

void Effects::EffectLightEnable(ID id, bool is_enable)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
		SML_LigthsSetEnable(ArrID[id]->IDLight, is_enable);
}

void Effects::EffectLightRotationSet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
		SML_LigthsSetRot(ArrID[id]->IDLight, &ArrID[id]->Rotation, false);
}

void Effects::EffectLightPositionSet(ID id)
{
	EFFECTS_EFFECT_PRECOND(id, _VOID);

	if (ArrID[id]->IDLight >= 0)
		SML_LigthsSetPos(ArrID[id]->IDLight, &ArrID[id]->Position);
}