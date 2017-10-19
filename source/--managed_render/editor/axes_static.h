
#ifndef __axes_static_h
#define __axes_static_h

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