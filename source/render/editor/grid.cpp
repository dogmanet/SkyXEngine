
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "grid.h"

CGrid::CGrid()
{
	m_pVertexBuffer = 0;
	m_pVertexDeclaration = 0;

	GXVERTEXELEMENT DeclGrid[] =
	{
		{ 0, 0,  GXDECLTYPE_FLOAT3,   GXDECLUSAGE_POSITION},
		{ 0, 12, GXDECLTYPE_GXCOLOR,  GXDECLUSAGE_COLOR},

		GXDECL_END()
	};

	m_pVertexDeclaration = gdata::pDXDevice->createVertexDeclaration(DeclGrid);
}

CGrid::~CGrid()
{
	mem_release(m_pVertexBuffer);
	mem_release(m_pVertexDeclaration);
}

void CGrid::create(int width, int depth, DWORD color)
{
	m_pVertexBuffer = gdata::pDXDevice->createVertexBuffer(width * depth * 2 * sizeof(CVertex), GX_BUFFER_USAGE_STATIC | GX_BUFFER_WRITEONLY);

	CVertex *pVertices;
	if(m_pVertexBuffer->lock((void**)&pVertices, GXBL_WRITE))
	{

		int oCountVert = 0;

		for(int x = -(width / 2); x < (width / 2) + 1; ++x)
		{
			pVertices[oCountVert].m_vPos = float3_t((float)x, 0.0f, (float)(-(depth / 2)));
			pVertices[oCountVert].m_dwColor = color;
			++oCountVert;
			pVertices[oCountVert].m_vPos = float3_t((float)x, 0.0f, (float)(depth / 2));
			pVertices[oCountVert].m_dwColor = color;
			++oCountVert;
		}

		for(int y = -(depth / 2); y < (depth / 2) + 1; ++y)
		{
			pVertices[oCountVert].m_vPos = float3_t((float)(-(width / 2)), 0.0f, (float)y);
			pVertices[oCountVert].m_dwColor = color;
			++oCountVert;
			pVertices[oCountVert].m_vPos = float3_t((float)(width / 2), 0.0f, (float)y);
			pVertices[oCountVert].m_dwColor = color;
			++oCountVert;
		}

		m_pVertexBuffer->unlock();
		m_iCountPoly = oCountVert / 2;
	}

	m_pRenderBuffer = gdata::pDXDevice->createRenderBuffer(1, &m_pVertexBuffer, m_pVertexDeclaration);

}

void CGrid::render()
{
	gdata::pDXDevice->SetTexture(0, 0);

	SGCore_ShaderUnBind();
	gdata::pDXDevice->setRenderBuffer(m_pRenderBuffer);

	gdata::pDXDevice->setPrimitiveTopology(GXPT_LINELIST);

	gdata::pDXDevice->drawPrimitive(0, m_iCountPoly);
}
