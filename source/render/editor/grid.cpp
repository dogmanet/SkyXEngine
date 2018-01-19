
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "grid.h"

CGrid::CGrid()
{
	m_pVertexBuffer = 0;
	m_pVertexDeclaration = 0;

	D3DVERTEXELEMENT9 DeclGrid[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

		D3DDECL_END()
	};

	GData::DXDevice->CreateVertexDeclaration(DeclGrid, &m_pVertexDeclaration);
}

CGrid::~CGrid()
{
	mem_release(m_pVertexBuffer);
	mem_release(m_pVertexDeclaration);
}

void CGrid::create(int width, int depth, DWORD color)
{
	GData::DXDevice->CreateVertexBuffer(
		width * depth * 2 * sizeof(CVertex),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		0);

	CVertex *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	int oCountVert = 0;

	for (int x = -(width/2); x < (width/2) + 1; ++x)
	{
		pVertices[oCountVert].m_vPos = float3_t((float)x, 0.0f, (float)(-(depth / 2)));
		pVertices[oCountVert].m_dwColor = color;
		++oCountVert;
		pVertices[oCountVert].m_vPos = float3_t((float)x, 0.0f, (float)(depth / 2));
		pVertices[oCountVert].m_dwColor = color;
		++oCountVert;
	}

	for (int y = -(depth / 2); y < (depth/2) + 1; ++y)
	{
		pVertices[oCountVert].m_vPos = float3_t((float)(-(width / 2)), 0.0f, (float)y);
		pVertices[oCountVert].m_dwColor = color;
		++oCountVert;
		pVertices[oCountVert].m_vPos = float3_t((float)(width / 2), 0.0f, (float)y);
		pVertices[oCountVert].m_dwColor = color;
		++oCountVert;
	}

	m_pVertexBuffer->Unlock();

	m_iCountPoly = oCountVert / 2;
}

void CGrid::render()
{
	GData::DXDevice->SetTexture(0, 0);
	//GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_ShaderUnBind();
	GData::DXDevice->SetVertexDeclaration(m_pVertexDeclaration);
	//GData::DXDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	GData::DXDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CVertex));
	GData::DXDevice->DrawPrimitive(D3DPT_LINELIST, 0, m_iCountPoly);
}