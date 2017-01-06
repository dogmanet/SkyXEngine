
#ifndef __static_geom
#define __static_geom

#define STATIC_GEOM_MAX_SIZE_POLY_IN_BUFF 256000

class StaticGeom
{
public:
	StaticGeom();

	long AddModel(const char* path);

	struct Group
	{
		String name;
		DWORD idtex;
		long CountVertex;
		long CountIndex;
		Array<IDirect3DVertexBuffer9*,4> VertexBuffer;
		Array<IDirect3DIndexBuffer9*,4> IndexBuffer;
	};

	void Render();

	Array<Group> AllGroups;
	IDirect3DVertexDeclaration9* VertexDeclarationStatic;
};

#endif