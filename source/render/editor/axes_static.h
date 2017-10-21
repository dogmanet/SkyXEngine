
#ifndef __AXES_STATIC_H
#define __AXES_STATIC_H

#include <windows.h>
#include <common/sxtypes.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <render/gdata.h>

class CAxesStatic
{
	struct CVertex
	{
		float3_t m_vPos;
		D3DCOLOR m_dwColor;
	};
public:
	CAxesStatic();
	~CAxesStatic();

	void create(float fLen);
	void render();

protected:

	IDirect3DVertexBuffer9 *m_pVertexBuffer;
	IDirect3DVertexDeclaration9 *m_pVertexDeclaration;
};

#endif