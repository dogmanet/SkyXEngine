
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Emitter.h"

void Emitter::NullingInit()
{
	OldTime = 0;
	TimeNextSpawnParticle = 0;
	VertexBuff = 0;
	IndexBuff = 0;
	VertexBuffQuad = 0;
	IndexBuffQuad = 0;
	isTexInit = false;
	IDTex = -1;
	Arr = 0;
	TransVertBuf = 0;
	Name[0] = 0;
	Count = 0;
	Enable = false;
	Alife = false;
	GTransparency = 1;
	TimerDeath = 0;
	SizeAdd = 0;

	//isTexTrackInit = false;
	IDTexTrack = -1;
	OldSize.x = 0;
	OldSize.y = 0;
}

Emitter::Emitter()
{
	NullingInit();
}

Emitter::Emitter(Emitter& part)
{
	NullingInit();
	IDTex = part.IDTex;
	IDTexTrack = part.IDTexTrack;
	Data = part.Data;
	CountSet(part.Count);
}

Emitter::~Emitter()
{
	mem_delete_a(Arr);
	mem_release_del(TransVertBuf);
	mem_release_del(VertexBuff);
	mem_release_del(IndexBuff);
	mem_release_del(VertexBuffQuad);
	mem_release_del(IndexBuffQuad);
}

void Emitter::OnLostDevice()
{
	mem_release_del(TransVertBuf);
}

void Emitter::OnResetDevice()
{
	PESet::DXDevice->CreateVertexBuffer(
		Count * sizeof(CommonParticleDecl2),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&TransVertBuf,
		0);
}

void Emitter::Init(ParticlesData* data)
{
	if (data)
		memcpy(&Data, data, sizeof(ParticlesData));

	if (IDTex >= 0)
		AnimTexDataInit();

	GeomDataCreate();

	if (Enable)
	{
		EnableSet(false);
		EnableSet(true);
	}
}

ParticlesData* Emitter::GetData()
{
	return &Data;
}

void Emitter::NameSet(const char* name)
{
	if (name)
		strcpy(Name,name);
}

void Emitter::NameGet(char* name)
{
	if (name)
		strcpy(name, Name);
}

void Emitter::TextureSetID(ID tex)
{
	IDTex = tex;

	isTexInit = false;
	if (SGCore_LoadTexGetTex(IDTex))
		isTexInit = true;

	AnimTexDataInit();
}

void Emitter::TextureSet(const char* tex)
{
	IDTex = SGCore_LoadTexAddName(tex, LOAD_TEXTURE_TYPE_LOAD);
	//SGCore_LoadTexLoadTextures();
	isTexInit = false;
	if (SGCore_LoadTexGetTex(IDTex))
		isTexInit = true;

	AnimTexDataInit();
}

ID Emitter::TextureGetID()
{
	return IDTex;
}

void Emitter::TextureGet(char* tex)
{
	if (IDTex >= 0)
	{
		SGCore_LoadTexGetName(IDTex, tex);
	}
}


void Emitter::TextureTrackSetID(ID tex)
{
	IDTexTrack = tex;
}

void Emitter::TextureTrackSet(const char* tex)
{
	IDTexTrack = SGCore_LoadTexAddName(tex, LOAD_TEXTURE_TYPE_LOAD);
	//SGCore_LoadTexLoadTextures();
}

ID Emitter::TextureTrackGetID()
{
	return IDTexTrack;
}

void Emitter::TextureTrackGet(char* tex)
{
	if (IDTexTrack >= 0)
	{
		SGCore_LoadTexGetName(IDTexTrack, tex);
	}
}


void Emitter::AnimTexDataInit()
{
	if (isTexInit && Data.AnimTexCountCadrsX != 0 && Data.AnimTexCountCadrsY != 0)
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

void Emitter::AlifeSet(bool alife)
{
	if (Alife != alife)
	{
		GTransparency = 1.f;
		TimerDeath = 0;
	}

	Alife = alife;
	if (!Enable && Alife)
		Enable = Alife;
}

bool Emitter::AlifeGet()
{
	return Alife;
}

////////////////////

void Emitter::ComputeLighting()
{
	if (!Enable)
		return;

	if (CountLifeParticle > 0 && Data.Lighting)
	{
		float3 tmpPosition;
		float3 tmpColor;
		for (int i = 0; i < Count; ++i)
		{
			if (Arr[i].IsAlife)
			{
				Arr[i].Ambient.x = Arr[i].Ambient.y = Arr[i].Ambient.z = Arr[i].Ambient.w = 0;

				for (int k = 0; k<SML_LigthsGetCount(); ++k)
				{
					//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
					if (SML_LigthsGetVisibleForFrustum(k) && SML_LigthsGetEnable(k))
					{
						SML_LigthsGetColor(k, &tmpColor);

						float intens = 1;

						if (SML_LigthsGetType(k) != LTYPE_LIGHT_GLOBAL)
						{
							SML_LigthsGetPos(k, &tmpPosition, true);

							float dist = SMVector3Dot(Arr[i].Pos - float3(tmpPosition.x, tmpPosition.y, tmpPosition.z));
							float invdist = 1.f - (dist / (SML_LigthsGetDist(k)));
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

void Emitter::CountSet(int count)
{
	Count = count;

	if (Count <= 0)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - buffer null size", GEN_MSG_LOCATION);
		return;
	}

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

	GeomDataCreate();
}

int Emitter::CountGet()
{
	return Count;
}

int Emitter::CountLifeGet()
{
	return CountLifeParticle;
}

void Emitter::EnableSet(bool enable)
{
	if (!Enable && enable)
	{
		CreateParticles();

		for (int i = 0; i < Count; ++i)
		{
			if (Arr[i].IsAlife)
			{
				CurrMax = Arr[i].Pos;
				CurrMin = Arr[i].Pos;
				break;
			}
		}
	}

	Alife = enable;
	OldTime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	Enable = enable;

	if (!Enable)
	{
		for (int i = 0; i < Count; ++i)
		{
			Arr[i].IsAlife = false;
		}
	}
}

bool Emitter::EnableGet()
{
	return Enable;
}

void Emitter::VertexBuffModify()
{
	if (!VertexBuff)
		return;

	CommonParticleDecl* vertices;
	VertexBuff->Lock(0, 0, (void**)&vertices, 0);

	float prev_angle_x = 0;
	float prev_angle_y = 0;
	float prev_angle_z = 0;

	int countvert = 0;

	static float4x4 mat;
	mat = SMMatrixIdentity();

	float x = Data.Size.x * 0.5f;
	float y = Data.Size.y * 0.5f;

	for (int i = 0; i < Data.FigureCountQuads; ++i)
	{
		float3 v0(-x, -y, 0.f);
		float3 v1(-x, y, 0.f);
		float3 v2(x, y, 0.f);
		float3 v3(x, -y, 0.f);

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

	VertexBuff->Unlock();
}

void Emitter::GeomDataCreate()
{
	mem_release_del(VertexBuff);
	mem_release_del(IndexBuff);

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

	VertexBuffModify();

	WORD* indices = 0;
	IndexBuff->Lock(0, 0, (void**)&indices, 0);

	int countind = 0;
	int countvert = 0;

	for (int i = 0; i < Data.FigureCountQuads; ++i)
	{
		indices[countind + 0] = countvert + 0; indices[countind + 1] = countvert + 1; indices[countind + 2] = countvert + 2;
		indices[countind + 3] = countvert + 0; indices[countind + 4] = countvert + 2; indices[countind + 5] = countvert + 3;

		countind += 6;
		countvert += 4;
	}

	IndexBuff->Unlock();



	PESet::DXDevice->CreateVertexBuffer(
		4 * sizeof(CommonParticleDecl),
		0,
		0,
		D3DPOOL_MANAGED,
		&VertexBuffQuad,
		0);

	PESet::DXDevice->CreateIndexBuffer(
		6  * sizeof(WORD),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IndexBuffQuad,
		0);

	CommonParticleDecl* vertices;
	VertexBuffQuad->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = CommonParticleDecl(-0.5, 0, -0.5, 0.0f, 1.0f);
	vertices[1] = CommonParticleDecl(-0.5, 0,  0.5, 0.0f, 0.0f);
	vertices[2] = CommonParticleDecl( 0.5, 0,  0.5, 1.0f, 0.0f);
	vertices[3] = CommonParticleDecl( 0.5, 0,  -0.5, 1.0f, 1.0f);

	VertexBuffQuad->Unlock();


	indices = 0;
	IndexBuffQuad->Lock(0, 0, (void**)&indices, 0);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	IndexBuffQuad->Unlock();
}

void Emitter::CreateParticles()
{
	CountReCreate2 = 0;
	CountLifeParticle = 0;
	
	for (int i = 0; i<Count; i++)
	{
		if (abs(Data.ReCreateCount) > CountReCreate2 || Data.ReCreateCount == 0)
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
				TimeNextSpawnParticle = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + Data.SpawnNextTime;//(Data.SpawnNextTime + (Data.SpawnNextTimeDisp > 0 ? rand()%Data.SpawnNextTimeDisp : 0));
		}
	}

}

void Emitter::ReCreateParticles(WORD id)
{
	//если разброс недопустим то спавним только в точке
	if (Data.SpawnPosType == PARTICLESTYPE_SPAWNPOS_STRICTLY)
	{
		Arr[id].Pos = Data.SpawnOrigin;
	}
	//разрешен рандомный спавн
	else if (Data.SpawnPosType == PARTICLESTYPE_SPAWNPOS_BOUND)
	{
		if (Data.BoundType == PARTICLESTYPE_BOUND_CONE)
		{
			if (Data.SpawnBoundBindCreateYNeg && Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = randf(Data.BoundVec1.y, Data.BoundVec2.y);
			else if (Data.SpawnBoundBindCreateYNeg)
				Arr[id].Pos.y = Data.BoundVec1.y;
			else if (Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = Data.BoundVec2.y;
			else
				Arr[id].Pos.y = Data.SpawnOrigin.y;

			float tmplerp = (Data.BoundVec2.y - Arr[id].Pos.y) / (Data.BoundVec2.y - Data.BoundVec1.y);

			float tmpradius = vlerp(Data.BoundVec2.w, Data.BoundVec1.w, tmplerp);
			float3 tmpcoord = float3(Data.BoundVec1.x, 0,Data.BoundVec1.z);

			if (Data.SpawnBoundBindCreateXNeg && Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = randf(tmpcoord.x - tmpradius, tmpcoord.x + tmpradius);
			else if (Data.SpawnBoundBindCreateXNeg)
				Arr[id].Pos.x = tmpcoord.x - tmpradius;
			else if (Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = tmpcoord.x + tmpradius;
			else
				Arr[id].Pos.x = Data.SpawnOrigin.x;

			if (Data.SpawnBoundBindCreateZNeg && Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = randf(tmpcoord.z - tmpradius, tmpcoord.z + tmpradius);
			else if (Data.SpawnBoundBindCreateZNeg)
				Arr[id].Pos.z = tmpcoord.z - tmpradius;
			else if (Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = tmpcoord.z + tmpradius;
			else
				Arr[id].Pos.z = Data.SpawnOrigin.z;
		}
		else if (Data.BoundType == PARTICLESTYPE_BOUND_BOX)
		{
			if (Data.SpawnBoundBindCreateXNeg && Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = randf(Data.BoundVec1.x, Data.BoundVec2.x);
			else if (Data.SpawnBoundBindCreateXNeg)
				Arr[id].Pos.x = Data.BoundVec1.x;
			else if (Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = Data.BoundVec2.x;
			else
				Arr[id].Pos.x = Data.SpawnOrigin.x;

			if (Data.SpawnBoundBindCreateYNeg && Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = randf(Data.BoundVec1.y, Data.BoundVec2.y);
			else if (Data.SpawnBoundBindCreateYNeg)
				Arr[id].Pos.y = Data.BoundVec1.y;
			else if (Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = Data.BoundVec2.y;
			else
				Arr[id].Pos.y = Data.SpawnOrigin.y;

			if (Data.SpawnBoundBindCreateZNeg && Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = randf(Data.BoundVec1.z, Data.BoundVec2.z);
			else if (Data.SpawnBoundBindCreateZNeg)
				Arr[id].Pos.z = Data.BoundVec1.z;
			else if (Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = Data.BoundVec2.z;
			else
				Arr[id].Pos.z = Data.SpawnOrigin.z;
		}
		else if (Data.BoundType == PARTICLESTYPE_BOUND_SPHERE)
		{
			if (Data.SpawnBoundBindCreateXNeg && Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = randf(Data.BoundVec1.x - Data.BoundVec1.w, Data.BoundVec1.x + Data.BoundVec1.w);
			else if (Data.SpawnBoundBindCreateXNeg)
				Arr[id].Pos.x = Data.BoundVec1.x - Data.BoundVec1.w;
			else if (Data.SpawnBoundBindCreateXPos)
				Arr[id].Pos.x = Data.BoundVec1.x + Data.BoundVec1.w;
			else
				Arr[id].Pos.x = Data.SpawnOrigin.x;

			if (Data.SpawnBoundBindCreateYNeg && Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = randf(Data.BoundVec1.y - Data.BoundVec1.w, Data.BoundVec1.y + Data.BoundVec1.w);
			else if (Data.SpawnBoundBindCreateYNeg)
				Arr[id].Pos.y = Data.BoundVec1.y - Data.BoundVec1.w;
			else if (Data.SpawnBoundBindCreateYPos)
				Arr[id].Pos.y = Data.BoundVec1.y + Data.BoundVec1.w;
			else
				Arr[id].Pos.y = Data.SpawnOrigin.y;

			if (Data.SpawnBoundBindCreateZNeg && Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = randf(Data.BoundVec1.z - Data.BoundVec1.w, Data.BoundVec1.z + Data.BoundVec1.w);
			else if (Data.SpawnBoundBindCreateZNeg)
				Arr[id].Pos.z = Data.BoundVec1.z - Data.BoundVec1.w;
			else if (Data.SpawnBoundBindCreateZPos)
				Arr[id].Pos.z = Data.BoundVec1.z + Data.BoundVec1.w;
			else
				Arr[id].Pos.z = Data.SpawnOrigin.z;
		}
		else if (Data.BoundType == PARTICLESTYPE_BOUND_NONE)
		{
			if (Data.SpawnOriginDisp != 0.0f)
			{
				if (Data.SpawnBoundBindCreateXNeg && Data.SpawnBoundBindCreateXPos)
					Arr[id].Pos.x = randf(Data.SpawnOrigin.x - Data.SpawnOriginDisp, Data.SpawnOrigin.x + Data.SpawnOriginDisp);
				else if (Data.SpawnBoundBindCreateXNeg)
					Arr[id].Pos.x = randf(Data.SpawnOrigin.x - Data.SpawnOriginDisp, Data.SpawnOrigin.x);
				else if (Data.SpawnBoundBindCreateXPos)
					Arr[id].Pos.x = randf(Data.SpawnOrigin.x, Data.SpawnOrigin.x + Data.SpawnOriginDisp);
				else
					Arr[id].Pos.x = Data.SpawnOrigin.x;

				if (Data.SpawnBoundBindCreateYNeg && Data.SpawnBoundBindCreateYPos)
					Arr[id].Pos.y = randf(Data.SpawnOrigin.y - Data.SpawnOriginDisp, Data.SpawnOrigin.y + Data.SpawnOriginDisp);
				else if (Data.SpawnBoundBindCreateYNeg)
					Arr[id].Pos.y = randf(Data.SpawnOrigin.y - Data.SpawnOriginDisp, Data.SpawnOrigin.y);
				else if (Data.SpawnBoundBindCreateYPos)
					Arr[id].Pos.y = randf(Data.SpawnOrigin.y, Data.SpawnOrigin.y + Data.SpawnOriginDisp);
				else
					Arr[id].Pos.y = Data.SpawnOrigin.y;

				if (Data.SpawnBoundBindCreateZNeg && Data.SpawnBoundBindCreateZPos)
					Arr[id].Pos.z = randf(Data.SpawnOrigin.z - Data.SpawnOriginDisp, Data.SpawnOrigin.z + Data.SpawnOriginDisp);
				else if (Data.SpawnBoundBindCreateZNeg)
					Arr[id].Pos.z = randf(Data.SpawnOrigin.z - Data.SpawnOriginDisp, Data.SpawnOrigin.z);
				else if (Data.SpawnBoundBindCreateZPos)
					Arr[id].Pos.z = randf(Data.SpawnOrigin.z, Data.SpawnOrigin.z + Data.SpawnOriginDisp);
				else
					Arr[id].Pos.z = Data.SpawnOrigin.z;
			}
		}
	}

	Arr[id].PosCreate = Arr[id].Pos;
	Arr[id].IsAlife = true;
	Arr[id].AlphaAgeDependCoef = 1.f;
	Arr[id].AlphaDeath = 1.f;

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
		Arr[id].Size.x = Data.Size.x + tmprand;
		Arr[id].Size.y = Data.Size.y + tmprand;

		if (Data.SizeDependAge == PARTICLESTYPE_DEPEND_DIRECT)
		{
			Arr[id].Size.y = Arr[id].Size.x;
			Arr[id].Size.x = Data.SizeDisp;
		}
	}
	else
	{
		Arr[id].Size.y = Data.Size.x;
		Arr[id].Size.x = (Data.SizeDependAge == PARTICLESTYPE_DEPEND_DIRECT ? 0 : Data.Size.x);
	}


	Arr[id].Velocity.x = Data.Velocity.x;
	Arr[id].Velocity.y = Data.Velocity.y;
	Arr[id].Velocity.z = Data.Velocity.z;

	if (Data.VelocityDisp != 0.0f)
	{
		if (Arr[id].Velocity.x != 0.f)
		{
			if (Data.VelocityDispXNeg)
				Arr[id].Velocity.x = Data.Velocity.x + (randf(-Data.VelocityDisp*0.5, Data.VelocityDisp*0.5));
			else
				Arr[id].Velocity.x = Data.Velocity.x + (randf(0, Data.VelocityDisp));
		}

		if (Arr[id].Velocity.y != 0.f)
		{
			if (Data.VelocityDispYNeg)
				Arr[id].Velocity.y = Data.Velocity.y + (randf(-Data.VelocityDisp*0.5, Data.VelocityDisp*0.5));
			else
				Arr[id].Velocity.y = Data.Velocity.y + (randf(0, Data.VelocityDisp));
		}

		if (Arr[id].Velocity.z != 0.f)
		{
			if (Data.VelocityDispZNeg)
				Arr[id].Velocity.z = Data.Velocity.z + (randf(-Data.VelocityDisp*0.5, Data.VelocityDisp*0.5));
			else
				Arr[id].Velocity.z = Data.Velocity.z + (randf(0, Data.VelocityDisp));
		}
	}


	if (Data.AccelerationDisp == 0.0f)
	{
		Arr[id].Acceleration.x = Data.Acceleration.x;
		Arr[id].Acceleration.y = Data.Acceleration.y;
		Arr[id].Acceleration.z = Data.Acceleration.z;
	}
	else if (Data.AccelerationDisp != 0.0f)
	{
		if (Arr[id].Acceleration.x != 0.f)
		{
			if (Data.AccelerationDispXNeg)
				Arr[id].Acceleration.x = Data.Acceleration.x + (randf(-Data.AccelerationDisp*0.5, Data.AccelerationDisp*0.5));
			else
				Arr[id].Acceleration.x = Data.Acceleration.x + (randf(0, Data.AccelerationDisp));
		}

		if (Arr[id].Acceleration.y != 0.f)
		{
			if (Data.AccelerationDispYNeg)
				Arr[id].Acceleration.y = Data.Acceleration.x + (randf(-Data.AccelerationDisp*0.5, Data.AccelerationDisp*0.5));
			else
				Arr[id].Acceleration.y = Data.Acceleration.x + (randf(0, Data.AccelerationDisp));
		}

		if (Arr[id].Acceleration.z != 0.f)
		{
			if (Data.AccelerationDispZNeg)
				Arr[id].Acceleration.z = Data.Acceleration.z + (randf(-Data.AccelerationDisp*0.5, Data.AccelerationDisp*0.5));
			else
				Arr[id].Acceleration.z = Data.Acceleration.z + (randf(0, Data.AccelerationDisp));
		}
	}
}

void Emitter::UpdateAnimTex(WORD idparticle, DWORD tmptime)
{
	if (!isTexInit)
		return;

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

bool Emitter::IsPointInCone(float3* point)
{
	if (point->y >= Data.BoundVec1.y && point->y <= Data.BoundVec2.y)
	{
		float tmplerp = (Data.BoundVec2.y - point->y) / (Data.BoundVec2.y - Data.BoundVec1.y);
		float tmpradius = vlerp(Data.BoundVec2.w, Data.BoundVec1.w, tmplerp);
		float3 tmpcoord = float3(Data.BoundVec1.x, 0, Data.BoundVec1.z);
		if (
			tmpcoord.x + tmpradius >= point->x && tmpcoord.x - tmpradius <= point->x &&
			tmpcoord.z + tmpradius >= point->z && tmpcoord.z - tmpradius <= point->z
			)
			return true;
	}

	return false;
}

bool Emitter::IsPointInSphere(float3* point)
{
	float distsqr = SMVector3Dot(Data.BoundVec1 - *point);
	if (distsqr <= Data.BoundVec1.w*Data.BoundVec1.w)
		return true;
	else
		return false;
}

bool Emitter::IsPointInBox(float3* point)
{
	if (point->x >= Data.BoundVec1.x && point->y >= Data.BoundVec1.y && point->z >= Data.BoundVec1.z && point->x <= Data.BoundVec2.x && point->y <= Data.BoundVec2.y && point->z <= Data.BoundVec2.z)
		return true;
	else
		return false;
}

void Emitter::Compute(const float4x4 * mat)
{
	if (!Enable)
		return;

	SizeAdd = (Data.Size.x > Data.Size.y ? Data.Size.x : Data.Size.y) + Data.SizeDisp;

	CountReCreate2 = 0;
	if (Alife && OldTime > 0 && Data.ReCreateCount > 0 && Data.ReCreateCount <= Count - CountLifeParticle)
	{
		if (TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) > TimeNextSpawnParticle)
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
						TimeNextSpawnParticle = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + (Data.SpawnNextTime + (Data.SpawnNextTimeDisp > 0 ? rand() % Data.SpawnNextTimeDisp : 0));
				}
			}
		}
	}

	if (!Alife)
	{
		TimeNextSpawnParticle = 0;
		if (TimerDeath == 0)
			TimerDeath = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
		else
		{
			GTransparency = 1.f - float(TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - TimerDeath) / float(SXPARTICLES_DEADTH_TIME);
			if (TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - TimerDeath > SXPARTICLES_DEADTH_TIME)
			{
				Enable = false;
				GTransparency = 1.f;
				TimerDeath = 0;
				for (int i = 0; i < Count; ++i)
				{
					Arr[i].IsAlife = false;
				}
				return;
			}
		}
	}


	DWORD tmptime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	CountLifeParticle = 0;

	float3 tmpoldpos,tmpnewpos;

	for (int i = 0; i<Count && OldTime != 0; i++)
	{
		//если время жизни частицы больше либо равно назначенному то значит она уже умерла
		if (Arr[i].IsAlife && Arr[i].TimeLife > 0 && Arr[i].Age >= Arr[i].TimeLife)
			Arr[i].IsAlife = false;

		//иначе инкремент общего количества живых частиц
		else if (Arr[i].IsAlife)
		{
			//если установлен ограничивающий объем то проверяем не выходит ли частица за его пределы, если выходит то значит она умерла

			if (Data.BoundType == PARTICLESTYPE_BOUND_SPHERE && IsPointInSphere(&Arr[i].Pos))
				CountLifeParticle++;
			else if (Data.BoundType == PARTICLESTYPE_BOUND_BOX && IsPointInBox(&Arr[i].Pos))
				CountLifeParticle++;
			else if (Data.BoundType == PARTICLESTYPE_BOUND_CONE && IsPointInCone(&(Arr[i].Pos)))
				CountLifeParticle++;
			else if (Data.BoundType != PARTICLESTYPE_BOUND_NONE)
				Arr[i].IsAlife = false;
			else if (Data.BoundType == PARTICLESTYPE_BOUND_NONE)
				CountLifeParticle++;
		}

		
		//если частица жива то обрабатываем поведение
		if (Arr[i].IsAlife)
		{
			tmpoldpos = Arr[i].Pos;
			//обрабокта возраста
			Arr[i].Age += tmptime - OldTime;

			//обработка зависимости прозрачности от возраста
			if (Data.AlphaDependAge == PARTICLESTYPE_DEPEND_DIRECT)
				Arr[i].AlphaAgeDependCoef = 1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife));
			else if (Data.AlphaDependAge == PARTICLESTYPE_DEPEND_INVERSE)
				Arr[i].AlphaAgeDependCoef = (float(Arr[i].Age) / float(Arr[i].TimeLife));
			else
				Arr[i].AlphaAgeDependCoef = 1;

			if (Arr[i].AlphaAgeDependCoef > 1.f)
				Arr[i].AlphaAgeDependCoef = 1.f;
			else if (Arr[i].AlphaAgeDependCoef < 0.f)
				Arr[i].AlphaAgeDependCoef = 0.f;

			if (!Alife)
				Arr[i].AlphaDeath = GTransparency;

			//обработка зависимости размера от возвраста
			if (Data.SizeDependAge == PARTICLESTYPE_DEPEND_DIRECT)
			{
				Arr[i].Size.x = Arr[i].Size.y * (float(Arr[i].Age) / float(Arr[i].TimeLife));
			}
			else if (Data.SizeDependAge == PARTICLESTYPE_DEPEND_INVERSE)
			{
				Arr[i].Size.x = Arr[i].Size.y * (1.f - (float(Arr[i].Age) / float(Arr[i].TimeLife)));
			}

			float tmpcountsec = float(tmptime - OldTime) * 0.001f;
			//обработка скорости и ускорения
			Arr[i].Pos += Arr[i].Velocity * tmpcountsec + Arr[i].Acceleration*(float(Arr[i].Age) * tmpcountsec);

			//если назначена анимация текстуры то обрабатываем
			if (Arr[i].AnimTexRateMls > 0)
				UpdateAnimTex(i, tmptime);
			/*else if (Data.AnimTexRate > 0 && Data.AnimTexCountCadrsX > 0 && Data.AnimTexCountCadrsY > 0)
			{
				Arr[i].AnimTexRateMls = Data.AnimTexRate + (Data.AnimTexRateDisp>0 ? ((rand() % (Data.AnimTexRateDisp / 2)) - (Data.AnimTexRateDisp / 2)) : 0);
			}*/

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

				if (Data.CharacterCircleAxis == PARTICLES_AXIS_X)
					matrot = SMMatrixRotationX(tmpangle);
				else if (Data.CharacterCircleAxis == PARTICLES_AXIS_Y)
					matrot = SMMatrixRotationY(tmpangle);
				else if (Data.CharacterCircleAxis == PARTICLES_AXIS_Z)
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
				if (Data.CharacterDeviationType == PARTICLESTYPE_DEVIATION_WAVE)
				{
					float tmpdist = 0;

					//определяем ось для волны
					if (Data.CharacterDeviationAxis == PARTICLES_AXIS_X)
						tmpdist = Arr[i].Pos.x;
					else if (Data.CharacterDeviationAxis == PARTICLES_AXIS_Y)
						tmpdist = Arr[i].Pos.y;
					else if (Data.CharacterDeviationAxis == PARTICLES_AXIS_Z)
						tmpdist = Arr[i].Pos.z;

					//если разрешена дисперсия
					if (Data.CharacterDeviationCoefAngleDisp != 0.0f)
					{
						if (Data.CharacterDeviationCoefAngleDispNeg)
						{
							tmpdist += randf(-Data.CharacterDeviationCoefAngleDisp*0.5, Data.CharacterDeviationCoefAngleDisp*0.5);
						}
						else
						{
							//если амплитуда отрицательная, значит и рандомное значение тоже
							if (Data.CharacterDeviationAmplitude < 0)
								tmpdist += randf(Data.CharacterDeviationCoefAngleDisp, 0);
							else
								tmpdist += randf(0, Data.CharacterDeviationCoefAngleDisp);
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
					if (Data.CharacterDeviationType == PARTICLESTYPE_DEVIATION_RAND)
					{
						//если разрешена дисперсия то генерируем
						if (Data.CharacterDeviationCoefAngleDisp != 0.0f)
						{
							if (Data.CharacterDeviationTapX)
								Arr[i].DeviationVector.x = randf(0, Data.CharacterDeviationAmplitude) - (Data.CharacterDeviationCoefAngleDispNeg ? Data.CharacterDeviationAmplitude*0.5f : 0);

							if (Data.CharacterDeviationTapY)
								Arr[i].DeviationVector.y = randf(0, Data.CharacterDeviationAmplitude) - (Data.CharacterDeviationCoefAngleDispNeg ? Data.CharacterDeviationAmplitude*0.5f : 0);

							if (Data.CharacterDeviationTapZ)
								Arr[i].DeviationVector.z = randf(0, Data.CharacterDeviationAmplitude) - (Data.CharacterDeviationCoefAngleDispNeg ? Data.CharacterDeviationAmplitude*0.5f : 0);
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
					else if (Data.CharacterDeviationType == PARTICLESTYPE_DEVIATION_ALONG)
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

			if (Data.CollisionDelete)
			{
				tmpoldpos = SMVector3Transform(tmpoldpos, *mat);
				tmpnewpos = SMVector3Transform(Arr[i].Pos, *mat);

				if (GParticlesPhyCollision(&tmpoldpos, &tmpnewpos, &Arr[i].TrackPos, &Arr[i].TrackNormal /*(!Arr[i].Track ? &Arr[i].TrackPos : 0), (!Arr[i].Track ? &Arr[i].TrackNormal : 0)*/))
				{
					Arr[i].IsAlife = false;

					if (Data.Track)
					{
						Arr[i].TrackPos += Arr[i].TrackNormal * SXPARTICLES_TRACK_ADD;
						float3 f = SXPARTICLES_TRACK_BASE_DIR;
						float3 a = SMVector3Cross(f, Arr[i].TrackNormal);
						float ang = acosf(SMVector3Dot(f, Arr[i].TrackNormal));

						SMQuaternion q(a, ang);
						Arr[i].TrackNormal.x = q.x;
						Arr[i].TrackNormal.y = q.y;
						Arr[i].TrackNormal.z = q.z;
						Arr[i].TrackNormal.w = q.w;
						Arr[i].Track = true;
						Arr[i].TrackTime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
					}
				}
			}

			if (!(Arr[i].IsAlife))
				continue;

			//считаем ограничивающий объем
			float tmpmaxedge = Arr[i].Size.x;
			if (Arr[i].Size.x < Arr[i].Size.y)
				tmpmaxedge = Arr[i].Size.y;

			if (CountLifeParticle == 1)
			{
				CurrMax.x = Arr[i].Pos.x + tmpmaxedge;
				CurrMax.y = Arr[i].Pos.y + tmpmaxedge;
				CurrMax.z = Arr[i].Pos.z + tmpmaxedge;

				CurrMin.x = Arr[i].Pos.x - tmpmaxedge;
				CurrMin.y = Arr[i].Pos.y - tmpmaxedge;
				CurrMin.z = Arr[i].Pos.z - tmpmaxedge;
			}
			else
			{
			if (Arr[i].Pos.x + tmpmaxedge > CurrMax.x)
					CurrMax.x = Arr[i].Pos.x + tmpmaxedge;

				if (Arr[i].Pos.x - tmpmaxedge < CurrMin.x)
					CurrMin.x = Arr[i].Pos.x - tmpmaxedge;

			if (Arr[i].Pos.y + tmpmaxedge > CurrMax.y)
					CurrMax.y = Arr[i].Pos.y + tmpmaxedge;

				if (Arr[i].Pos.y - tmpmaxedge < CurrMin.y)
					CurrMin.y = Arr[i].Pos.y - tmpmaxedge;

			if (Arr[i].Pos.z + tmpmaxedge > CurrMax.z)
					CurrMax.z = Arr[i].Pos.z + tmpmaxedge;

				if (Arr[i].Pos.z - tmpmaxedge < CurrMin.z)
					CurrMin.z = Arr[i].Pos.z - tmpmaxedge;
			}
		}
		else
		{

		}
	}

	//все частицы отыграли свое и будущего спавна нет
	if (OldTime != 0 && CountLifeParticle == 0 && TimeNextSpawnParticle == 0)
		Enable = false;

	if (!Enable)
		Alife = Enable;

	OldTime = tmptime;

	/*CurrMin.x -= SizeAdd;
	CurrMin.y -= SizeAdd;
	CurrMin.z -= SizeAdd;

	CurrMax.x += SizeAdd;
	CurrMax.y += SizeAdd;
	CurrMax.z += SizeAdd;*/
}



void Emitter::Render(DWORD timeDelta, float4x4* matrot, float4x4* matpos)
{
	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	if (!Enable)
		return;

	if (!isTexInit)
	{
		if (SGCore_LoadTexGetTex(IDTex))
		{
			isTexInit = true;
			AnimTexDataInit();
		}
	}

	if (CountLifeParticle > 0)
	{
		//если размер частиц изменился то меняем данные в вершинном буфере на новые
		if (Data.Size.x != OldSize.x || Data.Size.y != OldSize.y)
		{
			OldSize = Data.Size;
			VertexBuffModify();
		}

		CommonParticleDecl2* RTGPUArrVerteces;
		TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		DWORD tmpcount = 0;
		for (DWORD i = 0; i<Count; i++)
		{
			if (Arr[i].IsAlife)
			{
				RTGPUArrVerteces[tmpcount].pos = Arr[i].Pos;
				RTGPUArrVerteces[tmpcount].tex = Arr[i].AnimTexSizeCadrAndBias;
				RTGPUArrVerteces[tmpcount].alpha = Arr[i].AlphaAgeDependCoef * Arr[i].AlphaDeath * Data.TransparencyCoef;
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
		NearFar.x = *r_near;
		NearFar.y = *r_far;

		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &MCamView);
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &MCamProj);

		float4x4 cammat = SMMatrixIdentity();

		static float4x4 tmpmatrot = SMMatrixIdentity();
		tmpmatrot = (matrot ? (*matrot) : SMMatrixIdentity());

		if (Data.FigureType == PARTICLESTYPE_FIGURE_BILLBOARD)
		{
			float determ = 0;
			cammat = SMMatrixInverse(&determ, tmpmatrot * MCamView);
			cammat._41 = cammat._42 = cammat._43 = 0;
		}

		/*static float tmpangle = 0.0;
		tmpangle += 0.01;
		tmpmatrot = SMMatrixRotationY(tmpangle);*/

		static float4x4 tmpmatpos = SMMatrixIdentity();
		tmpmatpos = (matpos ? (*matpos) : SMMatrixIdentity());
		
		float4x4 worldmat = tmpmatrot * tmpmatpos;

		float4x4 vp = MCamView * MCamProj;
		PESet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(IDTex));
		if (Data.Soft)
		{
			if (PESet::IDsRenderTargets::DepthScene >= 0)
				PESet::DXDevice->SetTexture(1, SGCore_RTGetTexture(PESet::IDsRenderTargets::DepthScene));
			else
				LibReport(REPORT_MSG_LEVEL_WARNING, "sxparticles - not init depth map\n");
		}

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles);

		static ID psid = -1;

		if (Data.Soft && !Data.Refraction && !Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesSoft;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoft);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoft, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoft, "NearFar", &NearFar);
		}
		else if (Data.Soft && Data.Refraction && !Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesSoftRefraction;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefraction);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefraction, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefraction, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefraction, "RefractCoef", &Data.RefractionCoef);
		}
		else if (Data.Soft && Data.Refraction && Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesSoftRefractionLight;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefractionLight);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefractionLight, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefractionLight, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftRefractionLight, "RefractCoef", &Data.RefractionCoef);
		}
		else if (Data.Soft && !Data.Refraction && Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesSoftLight;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftLight);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftLight, "SoftCoef", &Data.SoftCoef);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftLight, "NearFar", &NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesSoftLight, "RefractCoef", &Data.RefractionCoef);
		}
		else if (!Data.Soft && Data.Refraction && Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesRefractionLight;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesRefractionLight);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesRefractionLight, "RefractCoef", &Data.RefractionCoef);
		}
		else if (!Data.Soft && !Data.Refraction && Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesLight;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesLight);
		}
		else if (!Data.Soft && Data.Refraction && !Data.Lighting)
		{
			psid = PESet::IDsShaders::PS::ParticlesRefraction;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesRefraction);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesRefraction, "RefractCoef", &Data.RefractionCoef);
		}
		else
		{
			psid = PESet::IDsShaders::PS::Particles;
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::Particles);
		}

		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "ViewProjection", &SMMatrixTranspose(vp));
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "WorldViewProjection", &SMMatrixTranspose(worldmat * vp));
		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "CamRot", &SMMatrixTranspose(cammat));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "World", &SMMatrixTranspose(world));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "MatRot", &SMMatrixTranspose(tmpmatrot));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "MatPos", &SMMatrixTranspose(tmpmatpos));
		//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::Particles, "PosCam", &ConstCamPos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, psid, "ColorCoef", &Data.ColorCoef);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, psid, "Color", &Data.Color);

		PESet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		if (Data.AlphaBlendType == PARTICLESTYPE_ALPHABLEND_ALPHA)
		{
			PESet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			PESet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			PESet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			PESet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
		else if (Data.AlphaBlendType == PARTICLESTYPE_ALPHABLEND_ADD)
		{
			PESet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			PESet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			PESet::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		}

		if (Data.FigureType == PARTICLESTYPE_FIGURE_QUAD_COMPOSITE)
			PESet::DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4 * Data.FigureCountQuads, 0, 2 * Data.FigureCountQuads);
		else
			PESet::DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		PESet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		SGCore_ShaderUnBind();

		PESet::DXDevice->SetStreamSourceFreq(0, 1);
		PESet::DXDevice->SetStreamSourceFreq(1, 1);
	}

	if (!Data.Track)
		return;

	bool exists_track = false;

	for (int i = 0; i < Count; ++i)
	{
		if (Arr[i].Track)
		{
			exists_track = true;
			break;
		}
	}

	if (exists_track)
	{
		CommonParticleDecl2* RTGPUArrVerteces;
		TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
		int tmpcount = 0;
		for (int i = 0; i<Count; ++i)
		{
			if (TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - Arr[i].TrackTime > Data.TrackTime)
				Arr[i].Track = false;

			if (Arr[i].Track)
			{
				RTGPUArrVerteces[tmpcount].pos = Arr[i].TrackPos;
				RTGPUArrVerteces[tmpcount].tex = (float4_t)Arr[i].TrackNormal;
				RTGPUArrVerteces[tmpcount].alpha = 1.f - (float(TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - Arr[i].TrackTime) / float(Data.TrackTime));
				RTGPUArrVerteces[tmpcount].size = Data.TrackSize;
				++tmpcount;
			}
		}

		TransVertBuf->Unlock();


		if (tmpcount <= 0)
			return;

		PESet::DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | tmpcount));

		PESet::DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		PESet::DXDevice->SetStreamSource(1, TransVertBuf, 0, sizeof(CommonParticleDecl2));

		PESet::DXDevice->SetVertexDeclaration(PESet::VertexDeclarationParticles);

		PESet::DXDevice->SetStreamSource(0, VertexBuffQuad, 0, sizeof(CommonParticleDecl));
		PESet::DXDevice->SetIndices(IndexBuffQuad);

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::ParticlesTrack);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesTrack);

		PESet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		PESet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		PESet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		PESet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		PESet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		static float4x4 MCamView;
		static float4x4 MCamProj;
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &MCamView);
		Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &MCamProj);

		float4x4 vp = MCamView * MCamProj;
		PESet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(IDTexTrack));

		SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PESet::IDsShaders::VS::ParticlesTrack, "WorldViewProjection", &SMMatrixTranspose(vp));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesTrack, "Color", &Data.Color);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PESet::IDsShaders::PS::ParticlesTrack, "ColorCoef", &Data.ColorCoef);

		PESet::DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		PESet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		SGCore_ShaderUnBind();

		PESet::DXDevice->SetStreamSourceFreq(0, 1);
		PESet::DXDevice->SetStreamSourceFreq(1, 1);
	}
}

int Emitter::TrackCountGet()
{
	if (!Enable || !Data.Track || !Data.CollisionDelete)
		return 0;

	int count_track = 0;

	for (int i = 0; i < Count; ++i)
	{
		if (Arr[i].Track)
			++count_track;
	}

	return count_track;
}

int Emitter::TrackPosGet(float3** arr, int count)
{
	if (!arr || !Enable || !Data.Track || !Data.CollisionDelete)
		return 0;

	int count_track = 0;

	for (int i = 0; i < Count; ++i)
	{
		if (Arr[i].Track)
		{
			if (count_track >= count)
				break;

			(*arr)[count_track] = Arr[i].TrackPos;
			++count_track;
		}
	}

	return count_track;
}