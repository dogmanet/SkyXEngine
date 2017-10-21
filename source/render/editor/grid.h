
#ifndef __GRID_H
#define __GRID_H

#include <windows.h>
#include <common/sxtypes.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <render/gdata.h>

class CGrid
{
	struct CVertex
	{
		float3_t m_vPos;
		D3DCOLOR m_dwColor;
	};
public:

	CGrid();
	~CGrid();

	void create(int iWidth, int iDepth, DWORD dwColor);
	void render();

protected:

	int m_iCountPoly;
	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	IDirect3DVertexDeclaration9 *m_pVertexDeclaration;
};

#endif