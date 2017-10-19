
#include <managed_render/editor/grid.h>

AxesStatic::AxesStatic()
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

AxesStatic::~AxesStatic()
{
	mem_release(VertexBuffer);
	mem_release(VertexDeclaration);
}

void AxesStatic::Create(float len)
{
	GData::DXDevice->CreateVertexBuffer(
		3 * 2 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&VertexBuffer,
		0);

	Vertex* vertices;
	VertexBuffer->Lock(0, 0, (void**)&vertices, 0);

	vertices[0].pos = float3_t(0, 0.001, 0);
	vertices[0].color = D3DCOLOR_ARGB(255, 255, 0, 0);

	vertices[1].pos = float3_t(len, 0.001, 0);
	vertices[1].color = D3DCOLOR_ARGB(255, 255, 0, 0);

	vertices[2].pos = float3_t(0, 0.001, 0);
	vertices[2].color = D3DCOLOR_ARGB(255, 0, 255, 0);

	vertices[3].pos = float3_t(0, len, 0);
	vertices[3].color = D3DCOLOR_ARGB(255, 0, 255, 0);

	vertices[4].pos = float3_t(0, 0.001, 0);
	vertices[4].color = D3DCOLOR_ARGB(255, 0, 0, 255);

	vertices[5].pos = float3_t(0, 0.001, len);
	vertices[5].color = D3DCOLOR_ARGB(255, 0, 0, 255);

	VertexBuffer->Unlock();
}

void AxesStatic::Render()
{
	GData::DXDevice->SetTexture(0, 0);
	//GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_ShaderUnBind();
	GData::DXDevice->SetVertexDeclaration(VertexDeclaration);
	//GData::DXDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	GData::DXDevice->SetStreamSource(0, VertexBuffer, 0, sizeof(Vertex));
	GData::DXDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
}