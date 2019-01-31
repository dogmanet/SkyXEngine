
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "pe_data.h"

namespace pe_data
{
	IGXContext *pDXDevice = 0;
	IGXVertexDeclaration *pVertexDeclarationParticles = 0;

	void Init(IGXContext *pDevice);

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

		namespace kit
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

	GXVERTEXELEMENT oInstanceParticles[] =
	{
		{ 0, 0,  GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION, GXDECLSPEC_PER_VERTEX_DATA},
		{ 0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD, GXDECLSPEC_PER_VERTEX_DATA},
				 					
		{ 1, 0,  GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1, GXDECLSPEC_PER_INSTANCE_DATA},
		{ 1, 12, GXDECLTYPE_FLOAT4, GXDECLUSAGE_TEXCOORD2, GXDECLSPEC_PER_INSTANCE_DATA},
		{ 1, 28, GXDECLTYPE_FLOAT1, GXDECLUSAGE_TEXCOORD3, GXDECLSPEC_PER_INSTANCE_DATA},
		{ 1, 32, GXDECLTYPE_FLOAT1, GXDECLUSAGE_TEXCOORD4, GXDECLSPEC_PER_INSTANCE_DATA},
				 										
		{ 1, 36, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD5, GXDECLSPEC_PER_INSTANCE_DATA},
		{ 1, 44, GXDECLTYPE_FLOAT1, GXDECLUSAGE_TEXCOORD6, GXDECLSPEC_PER_INSTANCE_DATA},
		GXDECL_END()
	};

	pe_data::pVertexDeclarationParticles = pe_data::pDXDevice->createVertexDeclaration(oInstanceParticles);

	pe_data::shader_id::vs::idParticlesTrack = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "particles_track.vs", "particles_track.vs", SHADER_CHECKDOUBLE_PATH);
	pe_data::shader_id::ps::idParticlesTrack = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_track.ps", "particles_track.ps", SHADER_CHECKDOUBLE_PATH);

	pe_data::shader_id::vs::idParticles = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "particles_main.vs", "particles_main.vs", SHADER_CHECKDOUBLE_PATH);
	pe_data::shader_id::ps::idParticles = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_main.ps", SHADER_CHECKDOUBLE_NAME);

	GXMACRO Defines_PART_SOFT[] = { { "PART_SOFT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoft = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT);

	GXMACRO Defines_PART_REFRACTION[] = { { "PART_REFRACTION", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesRefraction = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_refraction.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_REFRACTION);

	GXMACRO Defines_PART_LIGHT[] = { { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_LIGHT);

	GXMACRO Defines_PART_SOFT_REFRACTION[] = { { "PART_SOFT", "" }, { "PART_REFRACTION", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoftRefraction = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft_refraction.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT_REFRACTION);

	GXMACRO Defines_PART_SOFT_LIGHT[] = { { "PART_SOFT", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoftLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT_LIGHT);

	GXMACRO Defines_PART_REFRACTION_LIGHT[] = { { "PART_REFRACTION", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesRefractionLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_refraction_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_REFRACTION_LIGHT);

	GXMACRO Defines_PART_SOFT_REFRACTION_LIGHT[] = { { "PART_SOFT", "" }, { "PART_REFRACTION", "" }, { "PART_LIGHT", "" }, { 0, 0 } };
	pe_data::shader_id::ps::idParticlesSoftRefractionLight = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "particles_main.ps", "particles_soft_refraction_light.ps", SHADER_CHECKDOUBLE_NAME, Defines_PART_SOFT_REFRACTION_LIGHT);


	pe_data::shader_id::kit::idParticles = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticles);
	pe_data::shader_id::kit::idParticlesSoft = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesSoft);
	pe_data::shader_id::kit::idParticlesRefraction = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesRefraction);
	pe_data::shader_id::kit::idParticlesLight = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesLight);
	pe_data::shader_id::kit::idParticlesSoftRefraction = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesSoftRefraction);
	pe_data::shader_id::kit::idParticlesSoftLight = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesSoftLight);
	pe_data::shader_id::kit::idParticlesRefractionLight = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesRefractionLight);
	pe_data::shader_id::kit::idParticlesSoftRefractionLight = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticles, pe_data::shader_id::ps::idParticlesSoftRefractionLight);
	pe_data::shader_id::kit::idParticlesTrack = SGCore_ShaderCreateKit(pe_data::shader_id::vs::idParticlesTrack, pe_data::shader_id::ps::idParticlesTrack);
}
