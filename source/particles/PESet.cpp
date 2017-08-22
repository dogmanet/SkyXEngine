
#include "PESet.h"

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
