
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __PE_DATA_H
#define __PE_DATA_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <gcore/sxgcore.h>

namespace pe_data
{
	extern IGXContext *pDXDevice;
	extern IGXVertexDeclaration *pVertexDeclarationParticles;

	void Init();

	namespace rt_id
	{
		extern ID idDepthScene;
	};

	namespace shader_id
	{
		namespace vs
		{
			extern ID idParticles;
			extern ID idParticlesTrack;
		};

		namespace ps
		{
			extern ID idParticles;
			extern ID idParticlesSoft;
			extern ID idParticlesRefraction;
			extern ID idParticlesLight;
			extern ID idParticlesSoftRefraction;
			extern ID idParticlesSoftLight;
			extern ID idParticlesRefractionLight;
			extern ID idParticlesSoftRefractionLight;
			extern ID idParticlesTrack;
		};
	};
};

#endif