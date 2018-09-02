
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "grid.h"

CAxesStatic::CAxesStatic()
{
	m_pVertexBuffer = 0;
	m_pVertexDeclaration = 0;

	D3DVERTEXELEMENT9 DeclGrid[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

		D3DDECL_END()
	};

	gdata::pDXDevice->CreateVertexDeclaration(DeclGrid, &m_pVertexDeclaration);
}

CAxesStatic::~CAxesStatic()
{
	mem_release(m_pVertexBuffer);
	mem_release(m_pVertexDeclaration);
}

void CAxesStatic::create(float len)
{
	gdata::pDXDevice->CreateVertexBuffer(
		3 * 2 * sizeof(CVertex),
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		0);

	CVertex *pVertices;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].m_vPos = float3_t(0, 0.001f, 0);
	pVertices[0].m_dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertices[1].m_vPos = float3_t(len, 0.001f, 0);
	pVertices[1].m_dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	pVertices[2].m_vPos = float3_t(0, 0.001f, 0);
	pVertices[2].m_dwColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertices[3].m_vPos = float3_t(0, len, 0);
	pVertices[3].m_dwColor = D3DCOLOR_ARGB(255, 0, 255, 0);

	pVertices[4].m_vPos = float3_t(0, 0.001f, 0);
	pVertices[4].m_dwColor = D3DCOLOR_ARGB(255, 0, 0, 255);

	pVertices[5].m_vPos = float3_t(0, 0.001f, len);
	pVertices[5].m_dwColor = D3DCOLOR_ARGB(255, 0, 0, 255);

	m_pVertexBuffer->Unlock();
}

void CAxesStatic::render()
{
	gdata::pDXDevice->SetTexture(0, 0);
	//gdata::pDXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_ShaderUnBind();
	gdata::pDXDevice->SetVertexDeclaration(m_pVertexDeclaration);
	//gdata::pDXDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	gdata::pDXDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CVertex));
	gdata::pDXDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
}