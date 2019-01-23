#include "GXVertexDeclaration.h"

void CGXVertexDeclaration::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyVertexDeclaration(this);
	}
}

CGXVertexDeclaration::CGXVertexDeclaration(IDirect3DDevice9 *pDevice, CGXContext * pRender, const GXVERTEXELEMENT * pDecl):
	m_pRender(pRender)
{
	UINT uDeclCount = 0;
	while(pDecl[uDeclCount++].Type != GXDECLTYPE_UNUSED);
	--uDeclCount;

	for(UINT i = 0; i < MAXGXVSTREAM; ++i)
	{
		m_u8SpecSpec[i] = (GXDECLSPEC)-1;
	}

	for(UINT i = 0; i < uDeclCount; ++i)
	{
		if(pDecl[i].Stream >= MAXGXVSTREAM)
		{
			CGXContext::debugMessage(GX_LOG_ERROR, "Unable to create vertex declaration: Stream >= MAXGXVSTREAM!");
			return;
		}
		if(m_u8SpecSpec[pDecl[i].Stream] != (GXDECLSPEC)-1 && m_u8SpecSpec[pDecl[i].Stream] != pDecl[i].spec)
		{
			CGXContext::debugMessage(GX_LOG_ERROR, "Unable to create vertex declaration: vertex instance specs in a stream must be the same!");
			return;
		}
	}

	D3DVERTEXELEMENT9 *pEls = (D3DVERTEXELEMENT9*)alloca(sizeof(D3DVERTEXELEMENT9) * (uDeclCount + 1));

	memset(m_u8StreamStride, 0, sizeof(m_u8StreamStride));

	pEls[uDeclCount].Stream = 0xFF;
	pEls[uDeclCount].Offset = 0;
	pEls[uDeclCount].Type = D3DDECLTYPE_UNUSED;
	pEls[uDeclCount].Method = 0;
	pEls[uDeclCount].Usage = 0;
	pEls[uDeclCount].UsageIndex = 0;

	UINT stride = 0;
	m_u8StreamCount = 0;
	for(UINT i = 0; i < uDeclCount; ++i)
	{
		pEls[i].Stream = pDecl[i].Stream;
		pEls[i].Offset = pDecl[i].Offset;
		//pEls[i].Type = //?pDecl[i].Type;
		pEls[i].Method = D3DDECLMETHOD_DEFAULT;
		//pEls[i].Usage = //?pDecl[i].Usage;
		pEls[i].UsageIndex = 0;

		if(pDecl[i].Stream + 1 > m_u8StreamCount)
		{
			m_u8StreamCount = pDecl[i].Stream + 1;
		}

		
		UINT uSize = 0;
		switch(pDecl[i].Type)
		{
		case GXDECLTYPE_FLOAT1:
			pEls[i].Type = D3DDECLTYPE_FLOAT1; uSize = 4;  break;
		case GXDECLTYPE_FLOAT2:
			pEls[i].Type = D3DDECLTYPE_FLOAT2; uSize = 8; break;
		case GXDECLTYPE_FLOAT3:
			pEls[i].Type = D3DDECLTYPE_FLOAT3; uSize = 12; break;
		case GXDECLTYPE_FLOAT4:
			pEls[i].Type = D3DDECLTYPE_FLOAT4; uSize = 16; break;

		case GXDECLTYPE_GXCOLOR:
			pEls[i].Type = D3DDECLTYPE_D3DCOLOR; uSize = 4; break;


		case GXDECLTYPE_UBYTE4:
			pEls[i].Type = D3DDECLTYPE_UBYTE4; uSize = 4; break;

		case GXDECLTYPE_SHORT2:
			pEls[i].Type = D3DDECLTYPE_SHORT2; uSize = 4; break;
		case GXDECLTYPE_SHORT4:
			pEls[i].Type = D3DDECLTYPE_SHORT4; uSize = 8; break;

		case GXDECLTYPE_UBYTE4N:
			pEls[i].Type = D3DDECLTYPE_UBYTE4N; uSize = 4; break;

		case GXDECLTYPE_SHORT2N:
			pEls[i].Type = D3DDECLTYPE_SHORT2N; uSize = 4; break;
		case GXDECLTYPE_SHORT4N:
			pEls[i].Type = D3DDECLTYPE_SHORT4N; uSize = 8; break;

		case GXDECLTYPE_USHORT2N:
			pEls[i].Type = D3DDECLTYPE_USHORT2N; uSize = 4; break;
		case GXDECLTYPE_USHORT4N:
			pEls[i].Type = D3DDECLTYPE_USHORT4N; uSize = 8; break;

		case GXDECLTYPE_UDEC3:
			pEls[i].Type = D3DDECLTYPE_UDEC3; uSize = 32; break;

		case GXDECLTYPE_DEC3N:
			pEls[i].Type = D3DDECLTYPE_DEC3N; uSize = 32; break;

		case GXDECLTYPE_FLOAT16_2:
			pEls[i].Type = D3DDECLTYPE_FLOAT16_2; uSize = 4; break;

		case GXDECLTYPE_FLOAT16_4:
			pEls[i].Type = D3DDECLTYPE_FLOAT16_4; uSize = 8; break;
		}

		if(pEls[i].Offset + uSize > m_u8StreamStride[pDecl[i].Stream])
		{
			m_u8StreamStride[pDecl[i].Stream] = pEls[i].Offset + uSize;
		}

		switch(pDecl[i].Usage)
		{
		case GXDECLUSAGE_POSITION:
			pEls[i].Usage = D3DDECLUSAGE_POSITION; break;
		case GXDECLUSAGE_TEXCOORD:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD; break;
		case GXDECLUSAGE_NORMAL:
			pEls[i].Usage = D3DDECLUSAGE_NORMAL; break;
		case GXDECLUSAGE_BLENDWEIGHT:
			pEls[i].Usage = D3DDECLUSAGE_BLENDWEIGHT; break;
		case GXDECLUSAGE_BLENDINDICES:
			pEls[i].Usage = D3DDECLUSAGE_BLENDINDICES; break;
		case GXDECLUSAGE_COLOR:
			pEls[i].Usage = D3DDECLUSAGE_COLOR; break;
		case GXDECLUSAGE_TANGENT:
			pEls[i].Usage = D3DDECLUSAGE_TANGENT; break;
		case GXDECLUSAGE_BINORMAL:
			pEls[i].Usage = D3DDECLUSAGE_BINORMAL; break;
		case GXDECLUSAGE_TESSFACTOR:
			pEls[i].Usage = D3DDECLUSAGE_TESSFACTOR; break;
		case GXDECLUSAGE_TEXCOORD1:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD; 
			pEls[i].UsageIndex = 1;
			break;
		case GXDECLUSAGE_TEXCOORD2:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pEls[i].UsageIndex = 2;
			break;
		case GXDECLUSAGE_TEXCOORD3:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pEls[i].UsageIndex = 3;
			break;
		case GXDECLUSAGE_TEXCOORD4:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pEls[i].UsageIndex = 4;
			break;
		case GXDECLUSAGE_TEXCOORD5:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pEls[i].UsageIndex = 5;
			break;
		case GXDECLUSAGE_TEXCOORD6:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pEls[i].UsageIndex = 6;
			break;
		case GXDECLUSAGE_TEXCOORD7:
			pEls[i].Usage = D3DDECLUSAGE_TEXCOORD;
			pEls[i].UsageIndex = 7;
			break;
		}
	}

	DX_CALL(pDevice->CreateVertexDeclaration(pEls, &m_pDeclaration));
};

CGXVertexDeclaration::~CGXVertexDeclaration()
{
	mem_release(m_pDeclaration);
}
