
#ifndef __axes_static_h
#define __axes_static_h

#include <windows.h>
#include <common/sxtypes.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <render/gdata.h>

class AxesStatic
{
	struct Vertex
	{
		float3_t pos;
		D3DCOLOR color;
	};
public:
	AxesStatic();
	~AxesStatic();

	void Create(float len);
	void Render();

protected:
	long PolyCount;
	IDirect3DVertexBuffer9* VertexBuffer;
	IDirect3DVertexDeclaration9* VertexDeclaration;
};

#endif