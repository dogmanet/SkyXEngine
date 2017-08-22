
#ifndef __PESET_H
#define __PESET_H

#include <gdefines.h>
#include <d3d9.h>
#include <gcore/sxgcore.h>

namespace PESet
{
	extern IDirect3DDevice9* DXDevice;
	extern IDirect3DVertexDeclaration9* VertexDeclarationParticles;

	void Init(IDirect3DDevice9* device);

	namespace IDsRenderTargets
	{
		extern ID DepthScene;
	};

	namespace IDsShaders
	{
		namespace VS
		{
			extern ID Particles;
			extern ID ParticlesTrack;
		};

		namespace PS
		{
			extern ID Particles;
			extern ID ParticlesSoft;
			extern ID ParticlesRefraction;
			extern ID ParticlesLight;
			extern ID ParticlesSoftRefraction;
			extern ID ParticlesSoftLight;
			extern ID ParticlesRefractionLight;
			extern ID ParticlesSoftRefractionLight;
			extern ID ParticlesTrack;
		};
	};
};

#endif