
#include <managed_render/editor/grid.h>

Grid::Grid()
{
	VertexBuffer = 0;

	D3DVERTEXELEMENT9 DeclGrid[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

		D3DDECL_END()
	};

	GData::DXDevice->CreateVertexDeclaration(DeclGrid, &VertexDeclaration);
}

Grid::~Grid()
{
	mem_release(VertexBuffer);
	mem_release(VertexDeclaration);
}

void Grid::Create(int width, int depth, DWORD color)
{
	GData::DXDevice->CreateVertexBuffer(
		width * depth * 2 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&VertexBuffer,
		0);

	Vertex* vertices;
	VertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	long countvert = 0;

	for (int x = -(width/2); x < (width/2) + 1; ++x)
	{
		vertices[countvert].pos = float3_t(x, 0, -(depth/2));
		vertices[countvert].color = color;
		++countvert;
		vertices[countvert].pos = float3_t(x, 0, depth/2);
		vertices[countvert].color = color;
		++countvert;
	}

	for (int y = -(depth / 2); y < (depth/2) + 1; ++y)
	{
		vertices[countvert].pos = float3_t(-(width/2), 0, y);
		vertices[countvert].color = color;
		++countvert;
		vertices[countvert].pos = float3_t(width/2, 0, y);
		vertices[countvert].color = color;
		++countvert;
	}

	VertexBuffer->Unlock();

	PolyCount = countvert / 2;
}

void Grid::Render()
{
	GData::DXDevice->SetTexture(0, 0);
	//GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_ShaderUnBind();
	GData::DXDevice->SetVertexDeclaration(VertexDeclaration);
	//GData::DXDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	GData::DXDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
	GData::DXDevice->DrawPrimitive(D3DPT_LINELIST, 0, PolyCount);
}