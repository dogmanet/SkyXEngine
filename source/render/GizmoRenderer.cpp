#include "GizmoRenderer.h"
#include <gcore/sxgcore.h>

CGizmoRenderer::CGizmoRenderer(IGXDevice *pDev):
	m_pDev(pDev)
{
	GXVertexElement oLayout[] =
	{
		{0, 0, GXDECLTYPE_FLOAT4, GXDECLUSAGE_POSITION},
		{0, 16, GXDECLTYPE_FLOAT4, GXDECLUSAGE_TEXCOORD},
		{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1},
		{0, 44, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD2},
		GX_DECL_END()
	};
	m_pLinesVD = m_pDev->createVertexDeclaration(oLayout);

	m_idLinesShaderColored = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_lines.vs"), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_lines.ps"));
	
	GXMacro aMacro[] = {
		{"USE_TEXTURE", "1"},
		GX_MACRO_END()
	};
	m_idLinesShaderTextured = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_lines.vs"), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_lines.ps", NULL, aMacro));

	GXBlendDesc blendDesc;
	blendDesc.renderTarget[0].useBlend = true;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_SRC_ALPHA;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_SRC_ALPHA;
	m_pBlendAlpha = pDev->createBlendState(&blendDesc);

	GXDepthStencilDesc dsDesc;
	dsDesc.useDepthWrite = false;
	m_pDSState2D = pDev->createDepthStencilState(&dsDesc);

	dsDesc.cmpFuncDepth = GXCMP_GREATER_EQUAL;
	m_pDSState3D = pDev->createDepthStencilState(&dsDesc);
}
CGizmoRenderer::~CGizmoRenderer()
{
	mem_release(m_pBlendAlpha);
	mem_release(m_pDSState2D);
	mem_release(m_pDSState3D);

	mem_release(m_pLinesVD);
	mem_release(m_pLinesRB);
	mem_release(m_pLinesVB);
	//mem_release(m_pLinesIB);

}

void XMETHODCALLTYPE CGizmoRenderer::reset()
{
	for(UINT i = 0, l = m_aTextures.size(); i < l; ++i)
	{
		mem_release(m_aTextures[i]);
	}

	m_aVertices.clearFast();
	m_aTextures.clearFast();
	m_isLinesDirty = true;

	m_uLineCount = 0;
	m_uLineSegmentCount = 0;
}
void XMETHODCALLTYPE CGizmoRenderer::render(bool isOrtho)
{
	if(m_isLinesDirty)
	{
		m_isLinesDirty = false;

		// update buffers
		UINT uVertexCount = m_uLineSegmentCount * 4 + (m_uLineCount - 1) * 2;
		if(m_uLinesVBSize < uVertexCount)
		{
			m_uLinesVBSize = uVertexCount;
			mem_release(m_pLinesVB);
			mem_release(m_pLinesRB);

			m_pLinesVB = m_pDev->createVertexBuffer(sizeof(LineVertex) * uVertexCount, GXBUFFER_USAGE_DYNAMIC);
			m_pLinesRB = m_pDev->createRenderBuffer(1, &m_pLinesVB, m_pLinesVD);
		}

		m_aLineRanges.clearFast();

		UINT uVC = 0;
		LineVertex *pVertices;
		if(m_pLinesVB->lock((void**)&pVertices, GXBL_WRITE))
		{
			byte u8TexIdx;
			m_aLineRanges.reserve(m_aTextures.size() + 1);
			for(int i = -1, l = m_aTextures.size(); i < l; ++i)
			{
				if(i == -1)
				{
					u8TexIdx = 0xFF;
				}
				else
				{
					u8TexIdx = i;
				}

				LineRange &lr = m_aLineRanges[m_aLineRanges.size()];
				lr.u8Texture = u8TexIdx;

				lr.uStartVtx = uVC;
				bool isFirst = true;
				// 0123|34|4567|78|891011
				for(UINT j = 0, jl = m_aVertices.size(); j < jl; ++j)
				{
					LinePoint &pt = m_aVertices[j];
					if(pt.u8Texture == u8TexIdx)
					{
						if(pt.isStart)
						{
							if(!isFirst)
							{
								pVertices[uVC] = pVertices[uVC - 1];
								++uVC;


								pt.vtx.vTexUV = float2_t(0.0f, 0.0f);
								pVertices[uVC++] = pt.vtx;
							}
						}
						else
						{
							LinePoint &ptPrev = m_aVertices[j - 1];
							
							// add 4 vertices
							ptPrev.vtx.vTexUV = float2_t(0.0f, 0.0f);
							pVertices[uVC] = ptPrev.vtx;
							pVertices[uVC].vDir = pt.vtx.vDir;
							++uVC;
							ptPrev.vtx.vTexUV = float2_t(0.0f, 1.0f);
							pVertices[uVC] = ptPrev.vtx;
							pVertices[uVC].vDir = pt.vtx.vDir;
							++uVC;

							pt.vtx.vTexUV = float2_t(1.0f, 0.0f);
							pVertices[uVC++] = pt.vtx;
							pt.vtx.vTexUV = float2_t(1.0f, 1.0f);
							pVertices[uVC++] = pt.vtx;
						}

						isFirst = false;
					}
				}

				if(uVC > lr.uStartVtx + 2)
				{
					lr.uTriangleCount = uVC - lr.uStartVtx - 2;
				}
				else
				{
					m_aLineRanges.erase(m_aLineRanges.size() - 1);
				}
			}

			m_pLinesVB->unlock();
		}

		assert(uVC == uVertexCount);
	}


	IGXContext *pCtx = m_pDev->getThreadContext();
	
	IGXRasterizerState *pRS = pCtx->getRasterizerState();
	pCtx->setRasterizerState(NULL);
	IGXBlendState *pBS = pCtx->getBlendState();
	pCtx->setBlendState(m_pBlendAlpha);
	IGXDepthStencilState *pDS = pCtx->getDepthStencilState();
	pCtx->setDepthStencilState(isOrtho ? m_pDSState2D : m_pDSState3D);

	pCtx->setPrimitiveTopology(GXPT_TRIANGLESTRIP);
	for(UINT i = 0, l = m_aLineRanges.size(); i < l; ++i)
	{
		pCtx->setRenderBuffer(m_pLinesRB);

		LineRange &lr = m_aLineRanges[i];
		SGCore_ShaderBind(lr.u8Texture == 0xFF ? m_idLinesShaderColored : m_idLinesShaderTextured);

		if(lr.u8Texture != 0xFF)
		{
			pCtx->setPSTexture(m_aTextures[lr.u8Texture]);
		}
		pCtx->drawPrimitive(lr.uStartVtx, lr.uTriangleCount);
	}
	SGCore_ShaderUnBind();
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setRasterizerState(pRS);
	mem_release(pRS);
	pCtx->setBlendState(pBS);
	mem_release(pBS);
	pCtx->setDepthStencilState(pDS);
	mem_release(pDS);
}


void XMETHODCALLTYPE CGizmoRenderer::setLineWidth(float fWidth)
{
	m_fCurrentLineWidth = fWidth;
}

void XMETHODCALLTYPE CGizmoRenderer::setColor(const float4 &vColor)
{
	m_vCurrentColor = vColor;
}

void XMETHODCALLTYPE CGizmoRenderer::setTexture(IGXBaseTexture *pTexture)
{
	m_pCurrentTexture = pTexture;

	if(pTexture)
	{
		int idx = m_aTextures.indexOf(pTexture);
		if(idx < 0)
		{
			idx = m_aTextures.size();
			m_aTextures.push_back(pTexture);
			pTexture->AddRef();
		}
		m_u8CurrentTexture = idx;
	}
	else
	{
		m_u8CurrentTexture = 0xFF;
	}
}


void XMETHODCALLTYPE CGizmoRenderer::jumpTo(const float3 &vPos)
{
	m_isLineStart = true;
	m_vNextLineStart = vPos;
	m_vLastPointColor = m_vCurrentColor;
}

void XMETHODCALLTYPE CGizmoRenderer::lineTo(const float3 &vPos)
{
	LineVertex vtx;
	vtx.vDir = (float3)(vPos - m_vNextLineStart);
	if(m_isLineStart)
	{
		vtx.vPosWidth = float4(m_vNextLineStart, m_fCurrentLineWidth);
		vtx.vColor = m_vLastPointColor;
		m_aVertices.push_back({vtx, m_u8CurrentTexture, true});
		m_isLineStart = false;
		++m_uLineCount;
	}

	vtx.vPosWidth = float4(vPos, m_fCurrentLineWidth);
	vtx.vColor = m_vCurrentColor;
	m_aVertices.push_back({vtx, m_u8CurrentTexture, false});

	++m_uLineSegmentCount;

	m_vNextLineStart = vPos;
	m_vLastPointColor = m_vCurrentColor;

	m_isLinesDirty = true;
}


void XMETHODCALLTYPE CGizmoRenderer::setPointSize(float fWidth)
{
	m_fCurrentPointSize = fWidth;
}

void XMETHODCALLTYPE CGizmoRenderer::setPointMode(XGIZMO_POINT_MODE pointMode)
{
	m_pointMode = pointMode;
}

void XMETHODCALLTYPE CGizmoRenderer::drawPoint(const float3 &vPos)
{
}


void XMETHODCALLTYPE CGizmoRenderer::drawAABB(const SMAABB &aabb)
{
	jumpTo(aabb.vMin);
	lineTo(float3(aabb.vMin.x, aabb.vMin.y, aabb.vMax.z));
	lineTo(float3(aabb.vMax.x, aabb.vMin.y, aabb.vMax.z));
	lineTo(aabb.vMax);
	lineTo(float3(aabb.vMax.x, aabb.vMax.y, aabb.vMin.z));
	lineTo(float3(aabb.vMax.x, aabb.vMin.y, aabb.vMin.z));
	lineTo(aabb.vMin);
	lineTo(float3(aabb.vMin.x, aabb.vMax.y, aabb.vMin.z));
	lineTo(float3(aabb.vMin.x, aabb.vMax.y, aabb.vMax.z));
	lineTo(float3(aabb.vMin.x, aabb.vMin.y, aabb.vMax.z));
	jumpTo(float3(aabb.vMin.x, aabb.vMax.y, aabb.vMin.z));
	lineTo(float3(aabb.vMax.x, aabb.vMax.y, aabb.vMin.z));
	jumpTo(float3(aabb.vMin.x, aabb.vMax.y, aabb.vMax.z));
	lineTo(aabb.vMax);
	jumpTo(float3(aabb.vMax.x, aabb.vMin.y, aabb.vMin.z));
	lineTo(float3(aabb.vMax.x, aabb.vMin.y, aabb.vMax.z));
}


void XMETHODCALLTYPE CGizmoRenderer::drawEllipsoid(const float3 &vPos, const float3 vSize)
{
}
