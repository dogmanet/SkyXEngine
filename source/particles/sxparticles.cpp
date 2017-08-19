
#include <particles/sxparticles.h>
#define SXPARTICLES_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

bool ParticlesPhyCollision(const float3 * lastpos, const float3* nextpos, float3* coll_pos, float3* coll_nrm)
{
	return false;
}

g_particles_phy_collision GParticlesPhyCollision = ParticlesPhyCollision;

void SPE_SetFunc_ParticlesPhyCollision(g_particles_phy_collision func)
{
	GParticlesPhyCollision = func;
}

namespace PESet
{
	IDirect3DDevice9* DXDevice = 0;
	IDirect3DVertexDeclaration9* VertexDeclarationParticles = 0;

	void Init(IDirect3DDevice9* device);

	namespace IDsRenderTargets
	{
		ID DepthScene = -1;
	};

	namespace IDsShaders
	{
		namespace VS
		{
			ID Particles;
			ID ParticlesTrack;
		};

		namespace PS
		{
			ID Particles;
			ID ParticlesSoft;
			ID ParticlesRefraction;
			ID ParticlesLight;
			ID ParticlesSoftRefraction;
			ID ParticlesSoftLight;
			ID ParticlesRefractionLight;
			ID ParticlesSoftRefractionLight;
			ID ParticlesTrack;
		};
	};
};

void PESet::Init(IDirect3DDevice9* device)
{
	PESet::DXDevice = device;

	D3DVERTEXELEMENT9 InstanceParticles[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },

		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 1, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },

		{ 1, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
		{ 1, 44, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6 },
		D3DDECL_END()
	};

	PESet::DXDevice->CreateVertexDeclaration(InstanceParticles, &PESet::VertexDeclarationParticles);

	PESet::IDsShaders::VS::ParticlesTrack = SGCore_ShaderLoad(ShaderType::st_vertex, "particles_track.vs", "particles_track", ShaderCheckDouble::scd_path);
	PESet::IDsShaders::PS::ParticlesTrack = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_track.ps", "particles_track", ShaderCheckDouble::scd_path);

	PESet::IDsShaders::VS::Particles = SGCore_ShaderLoad(ShaderType::st_vertex, "particles_main.vs", "particles", ShaderCheckDouble::scd_path);
	PESet::IDsShaders::PS::Particles = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles", ShaderCheckDouble::scd_name);

	D3DXMACRO Defines_PART_SOFT[] = { { "PART_SOFT", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesSoft = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_soft", ShaderCheckDouble::scd_name, Defines_PART_SOFT);

	D3DXMACRO Defines_PART_REFRACTION[] = { { "PART_REFRACTION", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesRefraction = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_refraction", ShaderCheckDouble::scd_name, Defines_PART_REFRACTION);

	D3DXMACRO Defines_PART_LIGHT[] = { { "PART_LIGHT", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesLight = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_light", ShaderCheckDouble::scd_name, Defines_PART_LIGHT);

	D3DXMACRO Defines_PART_SOFT_REFRACTION[] = { { "PART_SOFT", "" }, { "PART_REFRACTION", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesSoftRefraction = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_soft_refraction", ShaderCheckDouble::scd_name, Defines_PART_SOFT_REFRACTION);

	D3DXMACRO Defines_PART_SOFT_LIGHT[] = { { "PART_SOFT", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesSoftLight = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_soft_light", ShaderCheckDouble::scd_name, Defines_PART_SOFT_LIGHT);

	D3DXMACRO Defines_PART_REFRACTION_LIGHT[] = { { "PART_REFRACTION", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesRefractionLight = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_refraction_light", ShaderCheckDouble::scd_name, Defines_PART_REFRACTION_LIGHT);

	D3DXMACRO Defines_PART_SOFT_REFRACTION_LIGHT[] = { { "PART_SOFT", "" }, { "PART_REFRACTION", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	PESet::IDsShaders::PS::ParticlesSoftRefractionLight = SGCore_ShaderLoad(ShaderType::st_pixel, "particles_main.ps", "particles_soft_refraction_light", ShaderCheckDouble::scd_name, Defines_PART_SOFT_REFRACTION_LIGHT);
}

#include <particles/emitter.cpp>
#include <particles/effect.cpp>

Effects* ArrEffects = 0;

#define PE_PRECOND(retval) if(!ArrEffects){reportf(REPORT_MSG_LEVEL_ERROR, "%s - sxparticles is not init", gen_msg_location); return retval;}

long SPE_0GetVersion()
{
	return SXPARTICLES_VERSION;
}

void SPE_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SPE_0Create(const char* name, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxparticles", gen_msg_location);
			}
			else
			{
				PESet::Init(SGCore_GetDXDevice());
				ArrEffects = new Effects();
			}
		}
		else
		{
			PESet::Init(SGCore_GetDXDevice());
			ArrEffects = new Effects();
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxparticles", gen_msg_location);
}

void SPE_RTDepthSet(ID id)
{
	PESet::IDsRenderTargets::DepthScene = id;
}

void SPE_AKill()
{
	PE_PRECOND(_VOID);

	mem_delete(ArrEffects);
	mem_release(PESet::VertexDeclarationParticles);
}

void SPE_OnLostDevice()
{
	PE_PRECOND(_VOID);

	ArrEffects->OnLostDevice();
}

void SPE_OnResetDevice()
{
	PE_PRECOND(_VOID);

	ArrEffects->OnResetDevice();
}



void SPE_EffectLoad(const char* path)
{
	PE_PRECOND(_VOID);

	ArrEffects->Load(path);
}

void SPE_EffectSave(const char* path)
{
	PE_PRECOND(_VOID);

	ArrEffects->Save(path);
}

void SPE_EffectsClear()
{
	PE_PRECOND(_VOID);

	ArrEffects->Clear();
}

ID SPE_EffectInstanceByName(const char* name)
{
	PE_PRECOND(-1);

	return ArrEffects->EffectInstanceByName(name);
}

ID SPE_EffectInstanceByID(ID id)
{
	PE_PRECOND(-1);

	return ArrEffects->EffectInstanceByID(id);
}

ID SPE_EffectGetByName(const char* name)
{
	PE_PRECOND(-1);

	return ArrEffects->EffectGetByName(name);
}

ID SPE_EffectAdd(const char* name)
{
	PE_PRECOND(-1);

	return ArrEffects->EffectAdd(name);
}

int SPE_EffectCountGet()
{
	PE_PRECOND(0);

	return ArrEffects->EffectCountGet();
}

ID SPE_EffectIdOfKey(ID key)
{
	PE_PRECOND(-1);

	return ArrEffects->EffectIdOfKey(key);
}

void SPE_EffectDelete(ID id)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectDelete(id);
}

void SPE_EffectNameSet(ID id, const char* name)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectNameSet(id, name);
}

void SPE_EffectNameGet(ID id, char* name)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectNameGet(id, name);
}


void SPE_EffectCompute(ID id)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectCompute(id);
}

void SPE_EffectComputeLighting(ID id)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectComputeLighting(id);
}

void SPE_EffectRender(ID id, DWORD timeDelta)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectRender(id, timeDelta);
}


void SPE_EffectComputeAll()
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectComputeAll();
}

void SPE_EffectComputeLightingAll()
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectComputeLightingAll();
}

void SPE_EffectRenderAll(DWORD timeDelta)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectRenderAll(timeDelta);
}


bool SPE_EffectAlifeGet(ID id)
{
	PE_PRECOND(false);

	return ArrEffects->EffectAlifeGet(id);
}

void SPE_EffectAlifeSet(ID id, bool alife)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectAlifeSet(id,alife);
}


bool SPE_EffectEnableGet(ID id)
{
	PE_PRECOND(false);

	return ArrEffects->EffectEnableGet(id);
}

void SPE_EffectEnableSet(ID id, bool isenable)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectEnableSet(id, isenable);
}

void SPE_EffectPlayByID(ID id, float3* pos, float3* dir)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectPlayByID(id, pos, dir);
}

void SPE_EffectPlayByName(const char* name, float3* pos, float3* dir)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectPlayByName(name, pos, dir);
}


void SPE_EffectPosSet(ID id, float3* pos)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectPosSet(id, pos);
}

void SPE_EffectDirSet(ID id, float3* dir)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectDirSet(id, dir);
}

void SPE_EffectRotSet(ID id, float3* rot)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectRotSet(id, rot);
}

void SPE_EffectRotSetQ(ID id, const SMQuaternion & rot)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectRotSet(id, rot);
}


void SPE_EffectPosGet(ID id, float3* pos)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectPosGet(id, pos);
}

void SPE_EffectDirGet(ID id, float3* dir)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectDirGet(id, dir);
}

void SPE_EffectRotGet(ID id, float3* rot)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectRotGet(id, rot);
}


bool SPE_EffectVisibleCom(ID id, ISXFrustum* frustum, float3* view)
{
	PE_PRECOND(false);

	return ArrEffects->EffectVisibleCom(id, frustum, view);
}

void SPE_EffectVisibleComAll(ISXFrustum* frustum, float3* view)
{
	PE_PRECOND(_VOID);

	ArrEffects->EffectVisibleComAll(frustum, view);
}

bool SPE_EffectVisibleGet(ID id)
{
	PE_PRECOND(false);

	return ArrEffects->EffectVisibleGet(id);
}

float SPE_EffectDistToViewGet(ID id)
{
	PE_PRECOND(0);

	return ArrEffects->EffectDistToViewGet(id);
}




ID SPE_EmitterAdd(ID id, ParticlesData* data)
{
	PE_PRECOND(-1);

	return ArrEffects->EmitterAdd(id, data);
}

int SPE_EmitterSCountGet(ID id)
{
	PE_PRECOND(0);

	return ArrEffects->EmitterGetCount(id);
}

void SPE_EmitterDelete(ID id, ID id_part)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterDelete(id, id_part);
}

ParticlesData* SPE_EmitterGetData(ID id, ID id_part)
{
	PE_PRECOND(0);

	return ArrEffects->EmitterGetData(id, id_part);
}

void SPE_EmitterReInit(ID id, ID id_part, ParticlesData* data)
{
	PE_PRECOND(_VOID);

	return ArrEffects->EmitterReInit(id, id_part, data);
}

void SPE_EmitterCountSet(ID id, ID id_part, int count)
{
	PE_PRECOND(_VOID);

	return ArrEffects->EmitterCountSet(id, id_part, count);
}

int SPE_EmitterCountGet(ID id, ID id_part)
{
	PE_PRECOND(0);

	return ArrEffects->EmitterCountGet(id, id_part);
}

int SPE_EmitterCountLifeGet(ID id, ID id_part)
{
	PE_PRECOND(0);

	return ArrEffects->EmitterCountLifeGet(id, id_part);
}

void SPE_EmitterEnableSet(ID id, ID id_part, bool enable)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterEnableSet(id, id_part, enable);
}

bool SPE_EmitterEnableGet(ID id, ID id_part)
{
	PE_PRECOND(false);

	return ArrEffects->EmitterEnableGet(id, id_part);
}


void SPE_EmitterTextureSet(ID id, ID id_part, const char* tex)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterTextureSet(id, id_part, tex);
}

void SPE_EmitterTextureSetID(ID id, ID id_part, ID tex)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterTextureSetID(id, id_part, tex);
}

ID SPE_EmitterTextureGetID(ID id, ID id_part)
{
	PE_PRECOND(-1);

	return ArrEffects->EmitterTextureGetID(id, id_part);
}

void SPE_EmitterTextureGet(ID id, ID id_part, char* tex)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterTextureGet(id, id_part, tex);
}


void SPE_EmitterTextureTrackSet(ID id, ID id_part, const char* tex)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterTextureTrackSet(id, id_part, tex);
}

void SPE_EmitterTextureTrackSetID(ID id, ID id_part, ID tex)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterTextureTrackSetID(id, id_part, tex);
}

ID SPE_EmitterTextureTrackGetID(ID id, ID id_part)
{
	PE_PRECOND(-1);

	return ArrEffects->EmitterTextureTrackGetID(id, id_part);
}

void SPE_EmitterTextureTrackGet(ID id, ID id_part, char* tex)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterTextureTrackGet(id, id_part, tex);
}



void SPE_EmitterNameSet(ID id, ID id_part, const char* name)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterNameSet(id, id_part, name);
}

void SPE_EmitterNameGet(ID id, ID id_part, char* name)
{
	PE_PRECOND(_VOID);

	ArrEffects->EmitterNameGet(id, id_part, name);
}


int SPE_EmitterTrackCountGet(ID id, ID id_part)
{
	PE_PRECOND(0);

	return ArrEffects->EmitterTrackCountGet(id, id_part);
}

int SPE_EmitterTrackPosGet(ID id, ID id_part, float3** arr, int count)
{
	PE_PRECOND(0);

	return ArrEffects->EmitterTrackPosGet(id, id_part, arr, count);
}