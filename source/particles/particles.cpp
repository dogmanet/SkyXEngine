
#include <particles/particles.h>

#pragma once

Particles::Particles()
{
	OldTime = 0;
	TimeNextSpawnParticle = 0;
	VertexBuff = 0;
	IndexBuff = 0;
	IDTex = -1;
}

Particles::~Particles()
{
	mem_delete_a(Arr);
	mem_release_del(TransVertBuf);
	mem_release_del(VertexBuff);
	mem_release_del(IndexBuff);
}

void Particles::OnLostDevice()
{
	mem_release_del(TransVertBuf);
}

void Particles::OnResetDevice()
{
	PESet::DXDevice->CreateVertexBuffer(
		Count * sizeof(CommonParticleDecl2),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&TransVertBuf,
		0);
}

void Particles::Init(ParticlesData* data)
{
	if (data)
		memcpy(&Data, data, sizeof(ParticlesData));

	if (IDTex >= 0)
		AnimTexDataInit();

	VertexCreate();
}

ParticlesData* Particles::GetData()
{
	return &Data;
}

void Particles::NameSet(const char* name)
{
	if (name)
		strcpy(Name,name);
}

void Particles::NameGet(char* name)
{
	if (name)
		strcpy(name, Name);
}

void Particles::SetTexture(ID tex)
{
	IDTex = tex;
	AnimTexDataInit();
}

void Particles::AnimTexDataInit()
{
	if (Data.AnimTexCountCadrsX != 0 && Data.AnimTexCountCadrsY != 0)
	{
		D3DSURFACE_DESC desc;
		SGCore_LoadTexGetTex(IDTex)->GetLevelDesc(0, &desc);

		AnimTexSize.x = desc.Width;
		AnimTexSize.y = desc.Height;

		AnimSizeCadr.x = (AnimTexSize.x / float(Data.AnimTexCountCadrsX));
		AnimSizeCadr.y = (AnimTexSize.y / float(Data.AnimTexCountCadrsY));
		AnimSizeCadr.z = AnimSizeCadr.x / AnimTexSize.x;
		AnimSizeCadr.w = AnimSizeCadr.y / AnimTexSize.y;
	}
}

bool Particles::GetAlife()
{
	return IsAlife;
}

////////////////////

void Particles::ComputeLighting()
{
	if (CountLifeParticle > 0 && Data.Lighting)
	{
		float3 tmpPosition;
		float3 tmpColor;
		for (int i = 0; i < Count; i++)
		{
			if (Arr[i].IsAlife)
			{
				Arr[i].Ambient.x = Arr[i].Ambient.y = Arr[i].Ambient.z = Arr[i].Ambient.w = 0;

				for (int k = 0; k<SML_LigthsGetCount(); ++k)
				{
					//поулчаем идентификатор света по ключу
					ID tmpid = SML_LigthsGetIDOfKey(i);

					//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
					if (SML_LigthsGetVisibleForFrustum(tmpid) && SML_LigthsIsEnable(tmpid))
					{
						SML_LigthsGetColor(tmpid, &tmpColor);

						float intens = 1;

						if (SML_LigthsGetType(tmpid) != LightsTypeLight::ltl_global)
						{
							SML_LigthsGetPos(tmpid, &tmpPosition, false, true);

							float dist = SMVector3Dot(Arr[i].Pos - float3(tmpPosition.x, tmpPosition.y, tmpPosition.z));
							float invdist = 1.f - (dist / (SML_LigthsGetDist(tmpid)));
							if (invdist > 1.f)
								invdist = 1.f;
							else if (invdist < 0.f)
								invdist = 0.f;

							intens = invdist;
						}

						Arr[i].LightingIntens += intens;
					}
				}
			}
		}
	}
}

void Particles::ReCreate(int count)
{
	if (count <= 0)
	{
		Count = count;
		mem_delete_a(Arr);
		mem_release_del(TransVertBuf);
		Arr = new CommonParticle[Count];

		PESet::DXDevice->CreateVertexBuffer(
			Count * sizeof(CommonParticleDecl2),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_DEFAULT,
			&TransVertBuf,
			0);
	}

	CreateParticles();
}

void Particles::Create(int count)
{
	Count = count;
	Arr = new CommonParticle[Count];

	CreateParticles();

	PESet::DXDevice->CreateVertexBuffer(
		Count * sizeof(CommonParticleDecl2),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&TransVertBuf,
		0);

	VertexCreate();
}

void Particles::VertexCreate()
{
	mem_release_del(VertexBuff);
	mem_release_del(IndexBuff);

	/*int count_quads = 10;
	bool isrand = true;
	bool tapx = false;
	bool tapy = true;
	bool tapz = true;*/

	PESet::DXDevice->CreateVertexBuffer(
		4 * Data.FigureCountQuads * sizeof(CommonParticleDecl),
		0,
		0,
		D3DPOOL_MANAGED,
		&VertexBuff,
		0);

	PESet::DXDevice->CreateIndexBuffer(
		6 * Data.FigureCountQuads * sizeof(WORD),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IndexBuff,
		0);


	CommonParticleDecl* vertices;
	VertexBuff->Lock(0, 0, (void**)&vertices, 0);

	float prev_angle_x = 0;
	float prev_angle_y = 0;
	float prev_angle_z = 0;

	int countvert = 0;

	for (int i = 0; i < Data.FigureCountQuads; ++i)
	{
		float3 v0(-0.5f, -0.5f, 0.f);
		float3 v1(-0.5f, 0.5f, 0.f);
		float3 v2(0.5f, 0.5f, 0.f);
		float3 v3(0.5f, -0.5f, 0.f);

		float4x4 mat;

		if (Data.FigureTapX)
		{
			if (Data.FigureRotRand)
				prev_angle_x = randf(0, SM_PI);
			else
				prev_angle_x = SM_PI / float(Data.FigureCountQuads) * i;

			mat *= SMMatrixRotationX(prev_angle_x);
		}

		if (Data.FigureTapY)
		{
			if (Data.FigureRotRand)
				prev_angle_y = randf(0, SM_PI);
			else
				prev_angle_y = SM_PI / float(Data.FigureCountQuads) * i;

			mat *= SMMatrixRotationY(prev_angle_y);
		}

		if (Data.FigureTapZ)
		{
			if (Data.FigureRotRand)
				prev_angle_z = randf(0, SM_PI);
			else
				prev_angle_z = SM_PI / float(Data.FigureCountQuads) * i;

			mat *= SMMatrixRotationZ(prev_angle_z);
		}

		v0 = SMVector3Transform(v0, mat);
		v1 = SMVector3Transform(v1, mat);
		v2 = SMVector3Transform(v2, mat);
		v3 = SMVector3Transform(v3, mat);

		vertices[countvert + 0] = CommonParticleDecl(v0.x, v0.y, v0.z, 0.0f, 1.0f);
		vertices[countvert + 1] = CommonParticleDecl(v1.x, v1.y, v1.z, 0.0f, 0.0f);
		vertices[countvert + 2] = CommonParticleDecl(v2.x, v2.y, v2.z, 1.0f, 0.0f);
		vertices[countvert + 3] = CommonParticleDecl(v3.x, v3.y, v3.z, 1.0f, 1.0f);

		countvert += 4;
	}

	/*vertices[0] = CommonParticleDecl(-0.5f, -0.5f, 0.f, 0.0f, 1.0f);
	vertices[1] = CommonParticleDecl(-0.5f,  0.5f, 0.f, 0.0f, 0.0f);
	vertices[2] = CommonParticleDecl( 0.5f,  0.5f, 0.f, 1.0f, 0.0f);
	vertices[3] = CommonParticleDecl( 0.5f, -0.5f, 0.f, 1.0f, 1.0f);*/

	VertexBuff->Unlock();


	WORD* indices = 0;
	IndexBuff->Lock(0, 0, (void**)&indices, 0);

	/*indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;*/

	int countind = 0;
	countvert = 0;

	for (int i = 0; i < Data.FigureCountQuads; ++i)
	{
		indices[countind + 0] = countvert + 0; indices[countind + 1] = countvert + 1; indices[countind + 2] = countvert + 2;
		indices[countind + 3] = countvert + 0; indices[countind + 4] = countvert + 2; indices[countind + 5] = countvert + 3;

		countind += 6;
		countvert += 4;
	}

	IndexBuff->Unlock();
}

void Particles::CreateParticles()
{
	CountReCreate2 = 0;
	CountLifeParticle = 0;
	IsAlife = true;

	for (int i = 0; i<Count; i++)
	{
		if (abs(Data.ReCreateCount) > CountReCreate2)
		{
			ReCreateParticles(i);
			CountReCreate2++;
			CountLifeParticle++;
		}
		else
		{
			CountReCreate2 = 0;
			i = Count;
			if (Data.SpawnNextTime)
				TimeNextSpawnParticle = timeGetTime() + Data.SpawnNextTime;//(Data.SpawnNextTime + (Data.SpawnNextTimeDisp > 0 ? rand()%Data.SpawnNextTimeDisp : 0));
		}
	}

}

inline void Particles::ReCreateParticles(WORD id)
{
	//если разброс недопустим то спавним только в точке
	if (Data.SpawnPosType == ParticlesSpawnPosType::pspt_strictly)
	{
		Arr[id].Pos = Data.SpawnOrigin;
	}
	//разрешен рандомный спавн
	else if (Data.SpawnPosType == ParticlesSpawnPosType::pspt_bound)
	{
		if (Data.BoundType == ParticlesBoundType::pbt_cone)
		{
			Arr[id].Pos.y = randf(Data.BoundConeBeginPos.y, Data.BoundConeEndPos.y);
			float tmplerp = (Data.BoundConeEndPos.y - Arr[id].Pos.y) / (Data.BoundConeEndPos.y - Data.BoundConeBeginPos.y);

			float tmpradius = lerp(Data.BoundConeEndRadius, Data.BoundConeBeginRadius, tmplerp);
			float3 tmpcoord = float3(lerp(Data.BoundConeEndPos.x, Data.BoundConeBeginPos.x, tmplerp), 0, lerp(Data.BoundConeEndPos.z, Data.BoundConeBeginPos.z, tmplerp));

			Arr[id].Pos.x = randf(tmpcoord.x - (tmpradius*0.5), tmpcoord.x + (tmpradius*0.5));
			Arr[id].Pos.z = randf(tmpcoord.z - (tmpradius*0.5), tmpcoord.z + (tmpradius*0.5));
		}
		else if (Data.BoundType == ParticlesBoundType::pbt_box)
		{
			if (Data.SpawnBoundBindCreateXNeg && Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = randf(Data.BoundBoxMin.x, Data.BoundBoxMax.x);
			else if (Data.SpawnBoundBindCreateXNeg)
				Arr[id].Pos.x = Data.BoundBoxMin.x;
			else if (Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = Data.BoundBoxMax.x;
			else
				Arr[id].Pos.x = Data.SpawnOrigin.x;

			if (Data.SpawnBoundBindCreateYNeg && Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = randf(Data.BoundBoxMin.y, Data.BoundBoxMax.y);
			else if (Data.SpawnBoundBindCreateYNeg)
				Arr[id].Pos.y = Data.BoundBoxMin.y;
			else if (Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = Data.BoundBoxMax.y;
			else
				Arr[id].Pos.y = Data.SpawnOrigin.y;

			if (Data.SpawnBoundBindCreateZNeg && Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = randf(Data.BoundBoxMin.z, Data.BoundBoxMax.z);
			else if (Data.SpawnBoundBindCreateZNeg)
				Arr[id].Pos.z = Data.BoundBoxMin.z;
			else if (Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = Data.BoundBoxMax.z;
			else
				Arr[id].Pos.z = Data.SpawnOrigin.z;
		}
		else if (Data.BoundType == ParticlesBoundType::pbt_sphere)
		{
			Arr[id].Pos.x = randf(Data.BoundSphereCenter.x - (Data.BoundSphereRadius*0.5), Data.BoundSphereCenter.x + (Data.BoundSphereRadius*0.5));
			Arr[id].Pos.y = randf(Data.BoundSphereCenter.y - (Data.BoundSphereRadius*0.5), Data.BoundSphereCenter.y + (Data.BoundSphereRadius*0.5));
			Arr[id].Pos.z = randf(Data.BoundSphereCenter.z - (Data.BoundSphereRadius*0.5), Data.BoundSphereCenter.z + (Data.BoundSphereRadius*0.5));
		}
		else if (Data.BoundType == ParticlesBoundType::pbt_none)
		{
			if (Data.SpawnOriginDisp != 0.0f)
			{
				Arr[id].Pos.x = randf(Data.SpawnOrigin.x - (Data.SpawnOriginDisp*0.5), Data.SpawnOrigin.x + (Data.SpawnOriginDisp*0.5));
				Arr[id].Pos.z = randf(Data.SpawnOrigin.z - (Data.SpawnOriginDisp*0.5), Data.SpawnOrigin.z + (Data.SpawnOriginDisp*0.5));
				Arr[id].Pos.y = randf(Data.SpawnOrigin.y - (Data.SpawnOriginDisp*0.5), Data.SpawnOrigin.y + (Data.SpawnOriginDisp*0.5));
			}
		}
	}

	Arr[id].PosCreate = Arr[id].Pos;
	Arr[id].IsAlife = true;
	Arr[id].AlphaAgeDependCoef = 1.f;

	Arr[id].Age = 0;
	Arr[id].TimeLife = Data.TimeLife + (Data.TimeLifeDisp != 0 ? ((rand() % (Data.TimeLifeDisp / 2)) - (Data.TimeLifeDisp / 2)) : 0);//GetRandomFloat(5000,30000);

	if (Data.AnimTexCountCadrsX == 0 || Data.AnimTexCountCadrsY == 0)
	{
		Arr[id].AnimTexSizeCadrAndBias.x = Arr[id].AnimTexSizeCadrAndBias.y = 1.f;
		Arr[id].AnimTexSizeCadrAndBias.z = Arr[id].AnimTexSizeCadrAndBias.w = 0.f;
		Arr[id].AnimTexRateMls = 0;
	}
	else
	{
		Arr[id].AnimTexCurrentCadr = Data.AnimTexStartCadr + (Data.AnimTexStartCadrDisp>0 ? ((rand() % (Data.AnimTexStartCadrDisp / 2)) - (Data.AnimTexStartCadrDisp / 2)) : 0);
		Arr[id].AnimTexCurrentMls = 0;
		Arr[id].AnimTexRateMls = Data.AnimTexRate + (Data.AnimTexRateDisp>0 ? ((rand() % (Data.AnimTexRateDisp / 2)) - (Data.AnimTexRateDisp / 2)) : 0);

		if (Arr[id].AnimTexCurrentCadr < Data.AnimTexCountCadrsX*Data.AnimTexCountCadrsY)
		{
			Arr[id].AnimTexCurrentCadr++;

			WORD tmpy = Arr[id].AnimTexCurrentCadr / Data.AnimTexCountCadrsX;
			WORD tmpx = Arr[id].AnimTexCurrentCadr - (Data.AnimTexCountCadrsX * tmpy);

			if ((int(Arr[id].AnimTexCurrentCadr) - int(Data.AnimTexCountCadrsX * tmpy)) == 0)
				tmpy -= 1;

			if (tmpx == 0)
				tmpx = Data.AnimTexCountCadrsX;

			tmpx -= 1;

			Arr[id].AnimTexSizeCadrAndBias.x = AnimSizeCadr.z;
			Arr[id].AnimTexSizeCadrAndBias.y = AnimSizeCadr.w;
			Arr[id].AnimTexSizeCadrAndBias.z = (float(tmpx) * AnimSizeCadr.x) / AnimTexSize.x;
			Arr[id].AnimTexSizeCadrAndBias.w = (float(tmpy) * AnimSizeCadr.y) / AnimTexSize.y;
		}
	}

	Arr[id].CharacterRotateAngleZ = 0;

	if (Data.SizeDisp > 0.f)
	{
		float tmprand = randf(0.0, Data.SizeDisp);
		Arr[id].Size.x = Data.SizeParticle.x + tmprand;
		Arr[id].Size.y = Data.SizeParticle.y + tmprand;

		if (Data.SizeDependAge == ParticlesDependType::padt_direct)
		{
			Arr[id].Size.y = Arr[id].Size.x;
			Arr[id].Size.x = Data.SizeDisp;
		}
	}
	else
	{
		Arr[id].Size.y = Data.SizeParticle.x;
		Arr[id].Size.x = (Data.SizeDependAge == ParticlesDependType::padt_direct ? 0 : Data.SizeParticle.x);
	}


	if (Data.VelocityDisp == 0.0f)
	{
		Arr[id].Velocity.x = Data.Velocity.x;
		Arr[id].Velocity.y = Data.Velocity.y;
		Arr[id].Velocity.z = Data.Velocity.z;
	}
	else if (Data.VelocityDisp != 0.0f)
	{
		if (Data.VelocityDispXNeg)
			Arr[id].Velocity.x = Data.Velocity.x + (randf(-Data.VelocityDisp*0.5, Data.VelocityDisp*0.5));
		else
			Arr[id].Velocity.x = Data.Velocity.x + (randf(0, Data.VelocityDisp));

		if (Data.VelocityDispYNeg)
			Arr[id].Velocity.y = Data.Velocity.y + (randf(-Data.VelocityDisp*0.5, Data.VelocityDisp*0.5));
		else
			Arr[id].Velocity.y = Data.Velocity.y + (randf(0, Data.VelocityDisp));

		if (Data.VelocityDispZNeg)
			Arr[id].Velocity.z = Data.Velocity.z + (randf(-Data.VelocityDisp*0.5, Data.VelocityDisp*0.5));
		else
			Arr[id].Velocity.z = Data.Velocity.z + (randf(0, Data.VelocityDisp));
	}


	if (Data.AccelerationDisp == 0.0f)
	{
		Arr[id].Acceleration.x = Data.Acceleration.x;
		Arr[id].Acceleration.y = Data.Acceleration.y;
		Arr[id].Acceleration.z = Data.Acceleration.z;
	}
	else if (Data.AccelerationDisp != 0.0f)
	{
		if (Data.AccelerationDispXNeg)
			Arr[id].Acceleration.x = Data.Acceleration.x + (randf(-Data.AccelerationDisp*0.5, Data.AccelerationDisp*0.5));
		else
			Arr[id].Acceleration.x = Data.Acceleration.x + (randf(0, Data.AccelerationDisp));

		if (Data.AccelerationDispYNeg)
			Arr[id].Acceleration.y = Data.Acceleration.x + (randf(-Data.AccelerationDisp*0.5, Data.AccelerationDisp*0.5));
		else
			Arr[id].Acceleration.y = Data.Acceleration.x + (randf(0, Data.AccelerationDisp));

		if (Data.AccelerationDispZNeg)
			Arr[id].Acceleration.z = Data.Acceleration.z + (randf(-Data.AccelerationDisp*0.5, Data.AccelerationDisp*0.5));
		else
			Arr[id].Acceleration.z = Data.Acceleration.z + (randf(0, Data.AccelerationDisp));
	}
}

void Particles::UpdateAnimTex(WORD idparticle, DWORD tmptime)
{
	//если подошло время обновления анимации текстуры
	if (Arr[idparticle].AnimTexRateMls <= Arr[idparticle].AnimTexCurrentMls)
	{
		Arr[idparticle].AnimTexCurrentMls = 0;

		//если номер текущего кадра меньше чем общее количество кадров, то обрабатываем
		if (Arr[idparticle].AnimTexCurrentCadr < Data.AnimTexCountCadrsX*Data.AnimTexCountCadrsY)
		{
			Arr[idparticle].AnimTexCurrentCadr++;

			WORD tmpy = Arr[idparticle].AnimTexCurrentCadr / Data.AnimTexCountCadrsX;
			WORD tmpx = Arr[idparticle].AnimTexCurrentCadr - (Data.AnimTexCountCadrsX * tmpy);

			if ((int(Arr[idparticle].AnimTexCurrentCadr) - int(Data.AnimTexCountCadrsX * tmpy)) == 0)
				tmpy -= 1;

			if (tmpx == 0)
				tmpx = Data.AnimTexCountCadrsX;

			tmpx -= 1;

			Arr[idparticle].AnimTexSizeCadrAndBias.x = AnimSizeCadr.z;
			Arr[idparticle].AnimTexSizeCadrAndBias.y = AnimSizeCadr.w;
			Arr[idparticle].AnimTexSizeCadrAndBias.z = (float(tmpx) * AnimSizeCadr.x) / AnimTexSize.x;
			Arr[idparticle].AnimTexSizeCadrAndBias.w = (float(tmpy) * AnimSizeCadr.y) / AnimTexSize.y;
		}
		else
			Arr[idparticle].AnimTexCurrentCadr = 0;
	}
	else
	{
		Arr[idparticle].AnimTexCurrentMls += tmptime - OldTime;
	}
}

inline bool Particles::IsPointInCone(float3* point)
{
	if (point->y >= Data.BoundConeBeginPos.y && point->y <= Data.BoundConeEndPos.y)
	{
		float tmplerp = (Data.BoundConeEndPos.y - point->y) / (Data.BoundConeEndPos.y - Data.BoundConeBeginPos.y);
		float tmpradius = lerp(Data.BoundConeEndRadius, Data.BoundConeBeginRadius, tmplerp) * 0.5f;
		float3 tmpcoord = float3(lerp(Data.BoundConeEndPos.x, Data.BoundConeBeginPos.x, tmplerp), 0, lerp(Data.BoundConeEndPos.z, Data.BoundConeBeginPos.z, tmplerp));
		if (
			tmpcoord.x + tmpradius >= point->x && tmpcoord.x - tmpradius <= point->x &&
			tmpcoord.z + tmpradius >= point->z && tmpcoord.z - tmpradius <= point->z
			)
			return true;
	}

	return false;
}

inline bool Particles::IsPointInSphere(float3* point)
{
	float distsqr = SMVector3Dot(Data.BoundSphereCenter - *point);
	if (distsqr <= Data.BoundSphereRadius*Data.BoundSphereRadius)
		return true;
	else
		return false;
}

inline bool Particles::IsPointInBox(float3* point)
{
	if (point->x >= Data.BoundBoxMin.x && point->y >= Data.BoundBoxMin.y && point->z >= Data.BoundBoxMin.z && point->x <= Data.BoundBoxMax.x && point->y <= Data.BoundBoxMax.y && point->z <= Data.BoundBoxMax.z)
		return true;
	else
		return false;
}

void Particles::Compute()
{
	CountReCreate2 = 0;
	if (OldTime > 0 && Data.ReCreateCount > 0 && Data.ReCreateCount <= Count - CountLifeParticle)
	{
		//DWORD tt = timeGetTime();
		if (timeGetTime() > TimeNextSpawnParticle)
		{
			for (int i = 0; i<Count; i++)
			{
				if (!(Arr[i].IsAlife) && Data.ReCreateCount > CountReCreate2)
				{
					ReCreateParticles(i);
					CountReCreate2++;
				}
				else if (Data.ReCreateCount <= CountReCreate2)
				{
					CountReCreate2 = 0;
					i = Count;
					if (Data.SpawnNextTime)
						TimeNextSpawnParticle = timeGetTime() + (Data.SpawnNextTime + (Data.SpawnNextTimeDisp > 0 ? rand() % Data.SpawnNextTimeDisp : 0));
				}
			}
		}
		IsAlife = true;
	}

	DWORD tmptime = timeGetTime();
	CountLifeParticle = 0;

	for (int i = 0; i<Count && OldTime != 0 && IsAlife; i++)
	{
		//если время жизни частицы больше либо равно назначенному то значит она уже умерла
		if (Arr[i].IsAlife && Arr[i].TimeLife > 0 && Arr[i].Age >= Arr[i].TimeLife)
			Arr[i].IsAlife = false;

		//иначе инкремент общего количества живых частиц
		else if (Arr[i].IsAlife)
		{
			//если установлен ограничивающий объем то проверяем не выходит ли частица за его пределы, если выходит то значит она умерла

			if (Data.BoundType == ParticlesBoundType::pbt_sphere && IsPointInSphere(&Arr[i].Pos))
				CountLifeParticle++;
			else if (Data.BoundType == ParticlesBoundType::pbt_box && IsPointInBox(&Arr[i].Pos))
				CountLifeParticle++;
			else if (Data.BoundType == ParticlesBoundType::pbt_cone && IsPointInCone(&(Arr[i].Pos)))
				CountLifeParticle++;
			else if (Data.BoundType != ParticlesBoundType::pbt_none)
				Arr[i].IsAlife = false;
			else if (Data.BoundType == ParticlesBoundType::pbt_none)
				CountLifeParticle++;
		}

		//если частица жива то обрабатываем поведение
		if (Arr[i].IsAlife)
		{
			//обрабокта возраста
			Arr[i].Age += tmptime - OldTime;

			//обработка зависимости прозрачности от возраста
			if (Data.AlphaAgeDepend == ParticlesDependType::padt_direct)
				Arr[i].AlphaAgeDependCoef = 1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife));
			else if (Data.AlphaAgeDepend == ParticlesDependType::padt_inverse)
				Arr[i].AlphaAgeDependCoef = (float(Arr[i].Age) / float(Arr[i].TimeLife));
			else
				Arr[i].AlphaAgeDependCoef = 1;

			if (Arr[i].AlphaAgeDependCoef > 1.f)
				Arr[i].AlphaAgeDependCoef = 1.f;
			else if (Arr[i].AlphaAgeDependCoef < 0.f)
				Arr[i].AlphaAgeDependCoef = 0.f;

			//обработка зависимости размера от возвраста
			if (Data.SizeDependAge == ParticlesDependType::padt_direct)
			{
				Arr[i].Size.x = Arr[i].Size.y * (float(Arr[i].Age) / float(Arr[i].TimeLife));
			}
			else if (Data.SizeDependAge == ParticlesDependType::padt_inverse)
			{
				Arr[i].Size.x = Arr[i].Size.y * (1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife)));
			}

			float tmpcountsec = float(tmptime - OldTime) * 0.001f;
			//обработка скорости и ускорения
			Arr[i].Pos += Arr[i].Velocity * tmpcountsec + Arr[i].Acceleration*(float(Arr[i].Age) * tmpcountsec);

			//если назначена анимация текстуры то обрабатываем
			if (Arr[i].AnimTexRateMls > 0)
				UpdateAnimTex(i, tmptime);

			//CHARACTER

			//движение по кругу
			if (Data.CharacterCircle)
			{
				float tmpangle = 0;

				//если возможна дисперсия
				if (Data.CharacterCircleAngleDisp != 0.0f)
				{
					//если возможно брать отрицательные значения
					if (Data.CharacterCircleAngleDispNeg)
						tmpangle = randf(-Data.CharacterCircleAngleDisp*0.5, Data.CharacterCircleAngleDisp*0.5);
					else
						tmpangle = randf(0.f, Data.CharacterCircleAngleDisp);
				}

				tmpangle += Data.CharacterCircleAngle;

				static float4x4 matrot;

				if (Data.CharacterCircleAxis == ParticlesAxis::pa_x)
					matrot = SMMatrixRotationX(tmpangle);
				else if (Data.CharacterCircleAxis == ParticlesAxis::pa_y)
					matrot = SMMatrixRotationY(tmpangle);
				else if (Data.CharacterCircleAxis == ParticlesAxis::pa_z)
					matrot = SMMatrixRotationZ(tmpangle);

				Arr[i].Pos = SMVector3Transform(Arr[i].Pos, matrot);
			}

			//если есть хараткер поворота
			if (Data.CharacterRotate)
			{
				float tmpangle = 0;

				//если возможна дисперсия
				if (Data.CharacterRotateAngleDisp != 0.0f)
				{
					//если возможно брать отрицательные значения
					if (Data.CharacterRotateAngleDispNeg)
						tmpangle = randf(-Data.CharacterRotateAngleDisp*0.5, Data.CharacterRotateAngleDisp*0.5);
					else
						tmpangle = randf(0.f, Data.CharacterRotateAngleDisp);
				}

				Arr[i].CharacterRotateAngleZ += Data.CharacterRotateAngle + tmpangle;

				if (Arr[i].CharacterRotateAngleZ >= SM_2PI)
					Arr[i].CharacterRotateAngleZ = Arr[i].CharacterRotateAngleZ - SM_2PI;

				Arr[i].rot_1.x = sinf(Arr[i].CharacterRotateAngleZ);
				Arr[i].rot_1.y = cosf(Arr[i].CharacterRotateAngleZ);
			}

			//если определен характер движения
			if (Data.CharacterDeviation)
			{
				//если определен волновой тип движения
				if (Data.CharacterDeviationType == ParticlesDeviationType::pdt_wave)
				{
					float tmpdist = 0;

					//определяем ось для волны
					if (Data.CharacterDeviationAxis == ParticlesAxis::pa_x)
						tmpdist = Arr[i].Pos.x;
					else if (Data.CharacterDeviationAxis == ParticlesAxis::pa_y)
						tmpdist = Arr[i].Pos.y;
					else if (Data.CharacterDeviationAxis == ParticlesAxis::pa_z)
						tmpdist = Arr[i].Pos.z;

					//если разрешена дисперсия
					if (Data.CharacterDeviationDisp != 0.0f)
					{
						if (Data.CharacterDeviationDispNeg)
						{
							tmpdist += randf(-Data.CharacterDeviationDisp*0.5, Data.CharacterDeviationDisp*0.5);
						}
						else
						{
							//если амплитуда отрицательная, значит и рандомное значение тоже
							if (Data.CharacterDeviationAmplitude < 0)
								tmpdist += randf(Data.CharacterDeviationDisp, 0);
							else
								tmpdist += randf(0, Data.CharacterDeviationDisp);
						}
						
					}

					//домножаем на коэфициент
					tmpdist *= Data.CharacterDeviationCoefAngle;

					if (Data.CharacterDeviationTapX)
						Arr[i].DeviationVector.x = Data.CharacterDeviationAmplitude * sinf(tmpdist);

					if (Data.CharacterDeviationTapY)
						Arr[i].DeviationVector.y = Data.CharacterDeviationAmplitude * sinf(tmpdist);

					if (Data.CharacterDeviationTapZ)
						Arr[i].DeviationVector.z = Data.CharacterDeviationAmplitude * sinf(tmpdist);

					Arr[i].Pos += Arr[i].DeviationVector;
				}


				//если пришло время обновления
				else if (Arr[i].CharacterDeviationCountDelayMls2 >= Data.CharacterDeviationCountDelayMls)
				{
					//обнуляем вектор отклонения
					Arr[i].DeviationVector.x = Arr[i].DeviationVector.y = Arr[i].DeviationVector.z = 0;

					//если простое смещение
					if (Data.CharacterDeviationType == ParticlesDeviationType::pdt_rnd)
					{
						//если разрешена дисперсия то генерируем
						if (Data.CharacterDeviationDisp != 0.0f)
						{
							if (Data.CharacterDeviationTapX)
								Arr[i].DeviationVector.x = randf(0, Data.CharacterDeviationAmplitude) - (Data.CharacterDeviationDispNeg ? Data.CharacterDeviationAmplitude*0.5f : 0);

							if (Data.CharacterDeviationTapY)
								Arr[i].DeviationVector.y = randf(0, Data.CharacterDeviationAmplitude) - (Data.CharacterDeviationDispNeg ? Data.CharacterDeviationAmplitude*0.5f : 0);

							if (Data.CharacterDeviationTapZ)
								Arr[i].DeviationVector.z = randf(0, Data.CharacterDeviationAmplitude) - (Data.CharacterDeviationDispNeg ? Data.CharacterDeviationAmplitude*0.5f : 0);
						}
						else
						{
							if (Data.CharacterDeviationTapX)
								Arr[i].DeviationVector.x = Data.CharacterDeviationAmplitude;

							if (Data.CharacterDeviationTapY)
								Arr[i].DeviationVector.y = Data.CharacterDeviationAmplitude;

							if (Data.CharacterDeviationTapZ)
								Arr[i].DeviationVector.z = Data.CharacterDeviationAmplitude;
						}
					}
					//иначе если равномерное смещение вдоль
					else if (Data.CharacterDeviationType == ParticlesDeviationType::pdt_along)
					{
						Data.CharacterDeviationAmplitude = -Data.CharacterDeviationAmplitude;

						if (Data.CharacterDeviationTapX)
							Arr[i].DeviationVector.x = Data.CharacterDeviationAmplitude;

						if (Data.CharacterDeviationTapY)
							Arr[i].DeviationVector.y = Data.CharacterDeviationAmplitude;

						if (Data.CharacterDeviationTapZ)
							Arr[i].DeviationVector.z = Data.CharacterDeviationAmplitude;
					}

					Arr[i].CharacterDeviationCountDelayMls2 = 0;
				}
				else
				{
					Arr[i].Pos += Arr[i].DeviationVector * (float(Arr[i].CharacterDeviationCountDelayMls2) / float(Data.CharacterDeviationCountDelayMls));
				}
				Arr[i].CharacterDeviationCountDelayMls2 += tmptime - OldTime;
			}

			//считаем ограничивающий объем
			float tmpmaxedge = Arr[i].Size.x;
			if (Arr[i].Size.x < Arr[i].Size.y)
				tmpmaxedge = Arr[i].Size.y;

			if (Arr[i].Pos.x + tmpmaxedge > CurrMax.x)
				CurrMax.x = Arr[i].Pos.x;
			else if (Arr[i].Pos.x - tmpmaxedge > CurrMin.x)
				CurrMin.x = Arr[i].Pos.x;

			if (Arr[i].Pos.y + tmpmaxedge > CurrMax.y)
				CurrMax.y = Arr[i].Pos.y;
			else if (Arr[i].Pos.y - tmpmaxedge > CurrMin.y)
				CurrMin.y = Arr[i].Pos.y;

			if (Arr[i].Pos.z + tmpmaxedge > CurrMax.z)
				CurrMax.z = Arr[i].Pos.z;
			else if (Arr[i].Pos.z - tmpmaxedge > CurrMin.z)
				CurrMin.z = Arr[i].Pos.z;
		}
		else
		{

		}
	}

	//все частицы отыграли свое
	if (OldTime != 0 && CountLifeParticle == 0)
		IsAlife = false;

	OldTime = tmptime;
}



void Particles::Render(DWORD timeDelta, float4x4* matrot, float4x4* matpos)
{
	if (CountLifeParticle > 0)
	{
		CommonParticleDecl2* RTGPUArrVerteces;
		TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		DWORD tmpcount = 0;
		for (DWORD i = 0; i<Count; i++)
		{
			if (Arr[i].IsAlife)
			{
				RTGPUArrVerteces[tmpcount].pos = Arr[i].Pos;
				RTGPUArrVerteces[tmpcount].tex = Arr[i].AnimTexSizeCadrAndBias;
				RTGPUArrVerteces[tmpcount].alpha = Arr[i].AlphaAgeDependCoef * Data.TransparencyCoef;
				RTGPUArrVerteces[tmpcount].size = Arr[i].Size.x;
				RTGPUArrVerteces[tmpcount].lighting = Arr[i].LightingIntens;

				RTGPUArrVerteces[tmpcount].rot_1 = Arr[i].rot_1;
				tmpcount++;
			}
		}

		TransVertBuf->Unlock();

		PESet::DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpcount));

		PESet::DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		PESet::DXDevice->SetStreamSource(1, TransVertBuf, 0, sizeof(CommonParticleDecl2));

		PESet::DXDevice->SetVertexDeclaration(PESet::VertexDeclarationParticles);

		PESet::DXDevice->SetStreamSource(0, VertexBuff, 0, sizeof(CommonParticleDecl));
		PESet::DXDevice->SetIndices(IndexBuff);

		static float4x4 MCamView;
		static float4x4 MCamProj;
		static float2_t NearFar;
		static float3 ConstCamPos;
		Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &ConstCamPos);
		NearFar.x = Core_RFloatGet(G_RI_FLOAT_OBSERVER_NEAR);
		NearFar.y = Core_RFloatGet(G_RI_FLOAT_OBSERVER_FAR);

		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &MCamView);
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &MCamProj);

		float4x4 world = SMMatrixIdentity();
		float determ = 0;
		float4x4 invview = SMMatrixInverse(&determ, MCamView);
		if (Data.FigureType == ParticlesFigureType::pft_billboard)
		{
			world = invview;
			world._41 = world._42 = world._43 = 0;
		}

		static float4x4 tmpmatrot = SMMatrixIdentity();
		tmpmatrot = (matrot ? (*matrot) : SMMatrixIdentity());

		/*static float tmpangle = 0.0;
		tmpangle += 0.01;
		tmpmatrot = SMMatrixRotationY(tmpangle);*/

		static float4x4 tmpmatpos = SMMatrixIdentity();
		tmpmatpos = (matpos ? (*matpos) : SMMatrixIdentity());

		float4x4 worldmat = tmpmatrot * tmpmatpos;

		float4x4 vp = MCamView * MCamProj;
		PESet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(IDTex));

		SGCore_ShaderBind(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles);

		if (Data.Soft && !Data.Refraction && !Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoft);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoft, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoft, "NearFar", &NearFar);
		}
		else if (Data.Soft && Data.Refraction && !Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefraction);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefraction, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefraction, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefraction, "RefractCoef", &Data.RefractionCoef);
		}
		else if (Data.Soft && Data.Refraction && Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefractionLight);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefractionLight, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefractionLight, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftRefractionLight, "RefractCoef", &Data.RefractionCoef);
		}
		else if (Data.Soft && !Data.Refraction && Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftLight);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftLight, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftLight, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesSoftLight, "RefractCoef", &Data.RefractionCoef);
		}
		else if (!Data.Soft && Data.Refraction && Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesRefractionLight);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesRefractionLight, "RefractCoef", &Data.RefractionCoef);
		}
		else if (!Data.Soft && !Data.Refraction && Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesLight);
		}
		else if (!Data.Soft && Data.Refraction && !Data.Lighting)
		{
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesRefraction);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, PESet::IDsShaders::PS::ParticlesRefraction, "RefractCoef", &Data.RefractionCoef);
		}
		else
			SGCore_ShaderBind(ShaderType::st_pixel, PESet::IDsShaders::PS::Particles);

		SGCore_ShaderSetVRF(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles, "ViewProjection", &SMMatrixTranspose(vp));
		SGCore_ShaderSetVRF(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles, "WorldMat", &SMMatrixTranspose(worldmat));
		SGCore_ShaderSetVRF(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles, "World", &SMMatrixTranspose(world));
		SGCore_ShaderSetVRF(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles, "MatRot", &SMMatrixTranspose(tmpmatrot));
		SGCore_ShaderSetVRF(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles, "MatPos", &SMMatrixTranspose(tmpmatpos));
		SGCore_ShaderSetVRF(ShaderType::st_vertex, PESet::IDsShaders::VS::Particles, "PosCam", &ConstCamPos);

		if (Data.FigureType == ParticlesFigureType::pft_quad_composite)
			PESet::DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4 * Data.FigureCountQuads, 0, 2 * Data.FigureCountQuads);
		else
			PESet::DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		SGCore_ShaderUnBind();

		PESet::DXDevice->SetStreamSourceFreq(0, 1);
		PESet::DXDevice->SetStreamSourceFreq(1, 1);
	}
}