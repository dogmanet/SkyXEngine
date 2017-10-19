
#ifndef __grid_h
#define __grid_h

#include <windows.h>
#include <common/sxtypes.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <render/gdata.h>

class Grid
{
	struct Vertex
	{
		float3_t pos;
		D3DCOLOR color;
	};
public:

	Grid();
	~Grid();

	void Create(int width, int depth, DWORD color);
	void Render();

protected:
	long PolyCount;
	IDirect3DVertexBuffer9* VertexBuffer;
	IDirect3DVertexDeclaration9* VertexDeclaration;
};

#endif