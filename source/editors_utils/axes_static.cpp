
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "grid.h"

CAxesStatic::CAxesStatic()
{
	m_pVertexBuffer = 0;
	m_pVertexDeclaration = 0;

	GXVERTEXELEMENT DeclGrid[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT4, GXDECLUSAGE_COLOR},

		GXDECL_END()
	};

	m_pVertexDeclaration = gdata::pDXDevice->createVertexDeclaration(DeclGrid);
}

CAxesStatic::~CAxesStatic()
{
	mem_release(m_pVertexBuffer);
	mem_release(m_pVertexDeclaration);
}

void CAxesStatic::create(float len)
{
	//@FIXME: Consider to make it static
	m_pVertexBuffer = gdata::pDXDevice->createVertexBuffer(3 * 2 * sizeof(CVertex), GX_BUFFER_USAGE_DYNAMIC | GX_BUFFER_WRITEONLY);

	CVertex *pVertices;

	if(m_pVertexBuffer->lock((void**)&pVertices, GXBL_WRITE))
	{

		pVertices[0].m_vPos = float3_t(0, 0.001f, 0);
		pVertices[0].m_vColor = GXCOLOR_COLORVECTOR_ARGB(GXCOLOR_ARGB(255, 255, 0, 0));

		pVertices[1].m_vPos = float3_t(len, 0.001f, 0);
		pVertices[1].m_vColor = GXCOLOR_COLORVECTOR_ARGB(GXCOLOR_ARGB(255, 255, 0, 0));

		pVertices[2].m_vPos = float3_t(0, 0.001f, 0);
		pVertices[2].m_vColor = GXCOLOR_COLORVECTOR_ARGB(GXCOLOR_ARGB(255, 0, 255, 0));

		pVertices[3].m_vPos = float3_t(0, len, 0);
		pVertices[3].m_vColor = GXCOLOR_COLORVECTOR_ARGB(GXCOLOR_ARGB(255, 0, 255, 0));

		pVertices[4].m_vPos = float3_t(0, 0.001f, 0);
		pVertices[4].m_vColor = GXCOLOR_COLORVECTOR_ARGB(GXCOLOR_ARGB(255, 0, 0, 255));

		pVertices[5].m_vPos = float3_t(0, 0.001f, len);
		pVertices[5].m_vColor = GXCOLOR_COLORVECTOR_ARGB(GXCOLOR_ARGB(255, 0, 0, 255));

		m_pVertexBuffer->unlock();
	}

	m_pRenderBuffer = gdata::pDXDevice->createRenderBuffer(1, &m_pVertexBuffer, m_pVertexDeclaration);
}

void CAxesStatic::render()
{
	gdata::pDXDevice->setTexture(NULL);
	//gdata::pDXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_ShaderUnBind();
	gdata::pDXDevice->setRenderBuffer(m_pRenderBuffer);
	gdata::pDXDevice->setPrimitiveTopology(GXPT_LINELIST);
#ifdef _GRAPHIX_API
	gdata::pDXDevice->drawPrimitive(0, 3);
#endif
}
