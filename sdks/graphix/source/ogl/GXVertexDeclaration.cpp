#include "GXVertexDeclaration.h"

void CGXVertexDeclaration::Release()
{
	m_pRender->destroyVertexDeclaration(this);
}

CGXVertexDeclaration::CGXVertexDeclaration(CGXContext * pRender, const GXVERTEXELEMENT * pDecl):
	m_pRender(pRender),
	m_uDeclCount(0),
	m_pDeclItems(NULL)
{
	while(pDecl[m_uDeclCount++].Type != GXDECLTYPE_UNUSED);

	m_uDeclCount--;

	m_pDeclItems = new _declItem[m_uDeclCount];

	UINT stride = 0;

	for(UINT i = 0; i < m_uDeclCount; ++i)
	{
#if defined(_DEBUG) && 0
		if(pDecl[i].UsageIndex > MAXDSGDECLUSAGEINDEX)
		{
			DS_FATAL((DStringW(L"Bad usage index ") + pDecl[i].UsageIndex).c_str());
		}
#endif
		m_pDeclItems[i].stream = pDecl[i].Stream;
		m_pDeclItems[i].pointer​ = (void*)pDecl[i].Offset;
		m_pDeclItems[i].index = pDecl[i].Usage;
		switch(pDecl[i].Type)
		{
		case GXDECLTYPE_FLOAT1:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 1;
			m_pDeclItems[i].type​ = GL_FLOAT;
			stride += 4;
			break;
		case GXDECLTYPE_FLOAT2:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 2;
			m_pDeclItems[i].type​ = GL_FLOAT;
			stride += 8;
			break;
		case GXDECLTYPE_FLOAT3:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 3;
			m_pDeclItems[i].type​ = GL_FLOAT;
			stride += 12;
			break;
		case GXDECLTYPE_FLOAT4:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_FLOAT;
			stride += 16;
			break;


		case GXDECLTYPE_UBYTE4:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_UNSIGNED_BYTE;
			stride += 4;
			break;

		case GXDECLTYPE_SHORT2:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 2;
			m_pDeclItems[i].type​ = GL_SHORT;
			stride += 4;
			break;
		case GXDECLTYPE_SHORT4:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_SHORT;
			stride += 8;
			break;

		case GXDECLTYPE_UBYTE4N:
			m_pDeclItems[i].normalized = TRUE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_UNSIGNED_BYTE;
			stride += 4;
			break;

		case GXDECLTYPE_SHORT2N:
			m_pDeclItems[i].normalized = TRUE;
			m_pDeclItems[i].size = 2;
			m_pDeclItems[i].type​ = GL_SHORT;
			stride += 4;
			break;
		case GXDECLTYPE_SHORT4N:
			m_pDeclItems[i].normalized = TRUE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_SHORT;
			stride += 8;
			break;

		case GXDECLTYPE_USHORT2N:
			m_pDeclItems[i].normalized = TRUE;
			m_pDeclItems[i].size = 2;
			m_pDeclItems[i].type​ = GL_UNSIGNED_SHORT;
			stride += 4;
			break;
		case GXDECLTYPE_USHORT4N:
			m_pDeclItems[i].normalized = TRUE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_UNSIGNED_SHORT;
			stride += 8;
			break;

		case GXDECLTYPE_UDEC3:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 3;
			m_pDeclItems[i].type​ = GL_INT_2_10_10_10_REV;
			stride += 32;
			break;

		case GXDECLTYPE_DEC3N:
			m_pDeclItems[i].normalized = TRUE;
			m_pDeclItems[i].size = 3;
			m_pDeclItems[i].type​ = GL_UNSIGNED_INT_2_10_10_10_REV;
			stride += 32;
			break;

		case GXDECLTYPE_FLOAT16_2:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 2;
			m_pDeclItems[i].type​ = GL_HALF_FLOAT;
			stride += 4;
			break;

		case GXDECLTYPE_FLOAT16_4:
			m_pDeclItems[i].normalized = FALSE;
			m_pDeclItems[i].size = 4;
			m_pDeclItems[i].type​ = GL_HALF_FLOAT;
			stride += 8;
			break;

		case GXDECLTYPE_GXCOLOR:
			assert(!"Not implemented");
			break;
		}
		//glVertexAttribPointer
	}

	for(UINT i = 0; i < m_uDeclCount; ++i)
	{
		m_pDeclItems[i].stride​ = stride;
	}
};

CGXVertexDeclaration::~CGXVertexDeclaration()
{
	mem_delete(m_pDeclItems);
}
