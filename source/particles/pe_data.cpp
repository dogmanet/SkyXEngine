
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "pe_data.h"

namespace pe_data
{
	IDirect3DDevice9 *pDXDevice = 0;
	IDirect3DVertexDeclaration9 *pVertexDeclarationParticles = 0;

	void Init(IDirect3DDevice9 *pDevice);

	namespace rt_id
	{
		ID idDepthScene = -1;
	};

	namespace shader_id
	{
		namespace vs
		{
			ID idParticles;
			ID idParticlesTrack;
		};

		namespace ps
		{
			ID idParticles;
			ID idParticlesSoft;
			ID idParticlesRefraction;
			ID idParticlesLight;
			ID idParticlesSoftRefraction;
			ID idParticlesSoftLight;
			ID idParticlesRefractionLight;
			ID idParticlesSoftRefractionLight;
			ID idParticlesTrack;
		};
	};
};

void pe_data::Init()
{
	pe_data::pDXDevice = SGCore_GetDXDevice();

	D3DVERTEXELEMENT9 oInstanceParticles[] =
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

	pe_data::pDXDevice->CreateVertexDeclaration(oInstanceParticles, &pe_data::pVertexDeclarationParticles);

	pe_data::shader_id::vs::idParticlesTrack = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "particles_track.vs", "particles_track.vs", SHADER_CHECKDOUBLE_PATH);
	pe_data::shader_id::ps::idParticlesTrack = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_track.ps", "particles_track.ps", SHADER_CHECKDOUBLE_PATH);

	pe_data::shader_id::vs::idParticles = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "particles_main.vs", "particles_main.vs", SHADER_CHECKDOUBLE_PATH);
	pe_data::shader_id::ps::idParticles = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_main.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_PART_SOFT[] = { { "PART_SOFT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoft = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT);

	D3DXMACRO Defines_PART_REFRACTION[] = { { "PART_REFRACTION", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesRefraction = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_refraction.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_REFRACTION);

	D3DXMACRO Defines_PART_LIGHT[] = { { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_LIGHT);

	D3DXMACRO Defines_PART_SOFT_REFRACTION[] = { { "PART_SOFT", "" }, { "PART_REFRACTION", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoftRefraction = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft_refraction.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT_REFRACTION);

	D3DXMACRO Defines_PART_SOFT_LIGHT[] = { { "PART_SOFT", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoftLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT_LIGHT);

	D3DXMACRO Defines_PART_REFRACTION_LIGHT[] = { { "PART_REFRACTION", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesRefractionLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_refraction_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_REFRACTION_LIGHT);

	D3DXMACRO Defines_PART_SOFT_REFRACTION_LIGHT[] = { { "PART_SOFT", "" }, { "PART_REFRACTION", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoftRefractionLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft_refraction_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT_REFRACTION_LIGHT);
}
