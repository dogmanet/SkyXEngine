#ifndef _IDSGvertexDeclarationOgl_H_
#define _IDSGvertexDeclarationOgl_H_

#include "GXContext.h"

class CGXVertexDeclaration: public IGXVertexDeclaration
{
	friend class CGXContext;
	
	CGXVertexDeclaration(IDirect3DDevice9 *pDevice, CGXContext * pRender, const GXVERTEXELEMENT * pDecl);
	~CGXVertexDeclaration();
	CGXContext * m_pRender;
		
	IDirect3DVertexDeclaration9 *m_pDeclaration = NULL;

	byte m_u8StreamCount;
	byte m_u8StreamStride[MAXGXVSTREAM];
	GXDECLSPEC m_u8SpecSpec[MAXGXVSTREAM];

public:
	void Release();
};

#endif
