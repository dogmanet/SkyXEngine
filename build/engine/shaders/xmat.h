#ifndef __XMAT_H
#define __XMAT_H

XMAT_MS_TEXTURES();
XMAT_MS_SAMPLERS();

struct XMaterial
{
	XMATERIAL_OUTPUT_STRUCT();
#ifdef WANT_WRITE_DEPTH
	float fDepth;
#endif
};

struct PSI_XMaterial
{
	XMAT_PS_STRUCT();
};

XMATERIAL_DEFAULT_LOADER();

//##########################################################################

#include XMATERIAL_SHADER




#endif
