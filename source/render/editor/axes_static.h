
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __AXES_STATIC_H
#define __AXES_STATIC_H

#include <windows.h>
#include <common/sxtypes.h>

//#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <render/gdata.h>

class CAxesStatic
{
	struct CVertex
	{
		float3_t m_vPos;
		GXCOLOR m_dwColor;
	};
public:
	CAxesStatic();
	~CAxesStatic();

	void create(float fLen);
	void render();

protected:

	IGXVertexBuffer *m_pVertexBuffer;
	IGXRenderBuffer *m_pRenderBuffer = NULL;
	IGXVertexDeclaration *m_pVertexDeclaration;
};

#endif