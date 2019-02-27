
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
		{ 0, 12, GXDECLTYPE_FLOAT4,  GXDECLUSAGE_COLOR},

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
	//@FIXME: Consider to make it static
	m_pVertexBuffer = gdata::pDXDevice->createVertexBuffer(width * depth * 2 * sizeof(CVertex), GX_BUFFER_USAGE_DYNAMIC | GX_BUFFER_WRITEONLY);

	CVertex *pVertices;
	if(m_pVertexBuffer->lock((void**)&pVertices, GXBL_WRITE))
	{

		int oCountVert = 0;

		for(int x = -(width / 2); x < (width / 2) + 1; ++x)
		{
			pVertices[oCountVert].m_vPos = float3_t((float)x, 0.0f, (float)(-(depth / 2)));
			pVertices[oCountVert].m_vColor = GXCOLOR_COLORVECTOR_ARGB(color);
			++oCountVert;
			pVertices[oCountVert].m_vPos = float3_t((float)x, 0.0f, (float)(depth / 2));
			pVertices[oCountVert].m_vColor = GXCOLOR_COLORVECTOR_ARGB(color);
			++oCountVert;
		}

		for(int y = -(depth / 2); y < (depth / 2) + 1; ++y)
		{
			pVertices[oCountVert].m_vPos = float3_t((float)(-(width / 2)), 0.0f, (float)y);
			pVertices[oCountVert].m_vColor = GXCOLOR_COLORVECTOR_ARGB(color);
			++oCountVert;
			pVertices[oCountVert].m_vPos = float3_t((float)(width / 2), 0.0f, (float)y);
			pVertices[oCountVert].m_vColor = GXCOLOR_COLORVECTOR_ARGB(color);
			++oCountVert;
		}

		m_pVertexBuffer->unlock();
		m_iCountPoly = oCountVert / 2;
	}

	m_pRenderBuffer = gdata::pDXDevice->createRenderBuffer(1, &m_pVertexBuffer, m_pVertexDeclaration);

}

void CGrid::render()
{
	gdata::pDXDevice->setTexture(NULL);

	SGCore_ShaderUnBind();
	gdata::pDXDevice->setRenderBuffer(m_pRenderBuffer);

	gdata::pDXDevice->setPrimitiveTopology(GXPT_LINELIST);
#ifdef _GRAPHIX_API
	gdata::pDXDevice->drawPrimitive(0, m_iCountPoly);
#endif
}
