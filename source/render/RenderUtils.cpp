#include "RenderUtils.h"
#include "GizmoRenderer.h"
#include "LineRenderer.h"
#include <gcore/sxgcore.h>

CRenderUtils::~CRenderUtils()
{
	mem_release(m_pQuadIB);
}

void XMETHODCALLTYPE CRenderUtils::newGizmoRenderer(IXGizmoRenderer **ppOut)
{
	*ppOut = new CGizmoRenderer(this, SGCore_GetDXDevice());
}

void XMETHODCALLTYPE CRenderUtils::newLineRenderer(IXLineRenderer **ppOut)
{
	*ppOut = new CLineRenderer(SGCore_GetDXDevice());
}

void XMETHODCALLTYPE CRenderUtils::getQuadIndexBuffer(UINT uQuads, IGXIndexBuffer **ppIndexBuffer)
{
	assert(uQuads < USHRT_MAX / 4);

	if(uQuads > m_uQuadIBSize)
	{
		UINT m_uFontIBChars = 1;
		while(m_uFontIBChars < uQuads)
		{
			m_uFontIBChars *= 2;
		}

		USHORT *pIB = new USHORT[m_uFontIBChars * 6];

		UINT CI = 0;
		for(UINT i = 0; i < m_uFontIBChars; ++i)
		{
			pIB[CI++] = i * 4 + 0;
			pIB[CI++] = i * 4 + 1;
			pIB[CI++] = i * 4 + 2;
			pIB[CI++] = i * 4 + 2;
			pIB[CI++] = i * 4 + 1;
			pIB[CI++] = i * 4 + 3;
		}

		mem_release(m_pQuadIB);
		m_pQuadIB = SGCore_GetDXDevice()->createIndexBuffer(sizeof(USHORT)* m_uFontIBChars * 6, GXBUFFER_USAGE_STATIC, GXIT_UINT16, pIB);

		mem_delete_a(pIB);

		m_uQuadIBSize = m_uFontIBChars;
	}

	m_pQuadIB->AddRef();
	*ppIndexBuffer = m_pQuadIB;
}
