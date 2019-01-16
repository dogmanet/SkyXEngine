
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __GRID_H
#define __GRID_H

#include <windows.h>
#include <common/sxtypes.h>

//#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <render/gdata.h>

class CGrid
{
	struct CVertex
	{
		float3_t m_vPos;
		GXCOLOR m_dwColor;
	};
public:

	CGrid();
	~CGrid();

	void create(int iWidth, int iDepth, DWORD dwColor);
	void render();

protected:

	int m_iCountPoly = 0;
	IGXVertexBuffer *m_pVertexBuffer;
	IGXRenderBuffer *m_pRenderBuffer = NULL;
	IGXVertexDeclaration *m_pVertexDeclaration;
};

#endif