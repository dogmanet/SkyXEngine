#include "GizmoRenderer.h"
#include <gcore/sxgcore.h>
#include "gdata.h"
#include "RenderUtils.h"

std::atomic_uint CGizmoRenderer::s_uResRefCount{0};
IGXVertexDeclaration *CGizmoRenderer::s_pPointsVD = NULL;
IGXBlendState *CGizmoRenderer::s_pBlendAlpha = NULL;
IGXDepthStencilState *CGizmoRenderer::s_pDSState3D = NULL;
IGXDepthStencilState *CGizmoRenderer::s_pDSState2D = NULL;
IGXDepthStencilState *CGizmoRenderer::s_pDSStateNoZ = NULL;
bool CGizmoRenderer::s_isShadersLoaded = false;
ID CGizmoRenderer::s_idShaders[2][2][2]; // [isTextured][is3D][isFixed]

CGizmoRenderer::CGizmoRenderer(CRenderUtils *pRenderUtils, IGXDevice *pDev):
	m_pRenderUtils(pRenderUtils),
	m_pDev(pDev),
	m_lineRenderer(pDev)
{
	add_ref(m_pRenderUtils);

	if(!s_uResRefCount)
	{
		GXVertexElement oLayout[] =
		{
			{0, 0, GXDECLTYPE_FLOAT4, GXDECLUSAGE_POSITION},
			{0, 16, GXDECLTYPE_FLOAT4, GXDECLUSAGE_TEXCOORD},
			{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD2},
			GX_DECL_END()
		};
		s_pPointsVD = m_pDev->createVertexDeclaration(oLayout);

		GXBlendDesc blendDesc;
		blendDesc.renderTarget[0].useBlend = true;
		blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_SRC_ALPHA;
		blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_SRC_ALPHA;
		s_pBlendAlpha = pDev->createBlendState(&blendDesc);

		GXDepthStencilDesc dsDesc;
		dsDesc.useDepthWrite = false;
		s_pDSState2D = pDev->createDepthStencilState(&dsDesc);

		dsDesc.cmpFuncDepth = GXCMP_GREATER_EQUAL;
		s_pDSState3D = pDev->createDepthStencilState(&dsDesc);

		dsDesc.useDepthTest = false;
		s_pDSStateNoZ = pDev->createDepthStencilState(&dsDesc);
	}

	++s_uResRefCount;

	if(!s_isShadersLoaded)
	{
		s_isShadersLoaded = true;

		GXMacro aMacro1[] = {
			{"IS_ORTHO", "1"},
			GX_MACRO_END()
		};
		GXMacro aMacro2[] = {
			{"IS_FIXED", "1"},
			GX_MACRO_END()
		};
		GXMacro aMacro3[] = {
			{"IS_ORTHO", "1"},
			{"IS_FIXED", "1"},
			GX_MACRO_END()
		};
		s_idShaders[0][1][0] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs"), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps"));
		s_idShaders[0][1][1] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs", 0, aMacro2), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps"));
		s_idShaders[0][0][0] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs", 0, aMacro1), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps"));
		s_idShaders[0][0][1] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs", 0, aMacro3), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps"));

		GXMacro aMacro[] = {
			{"USE_TEXTURE", "1"},
			GX_MACRO_END()
		};
		s_idShaders[1][1][0] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs"), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps", NULL, aMacro));
		s_idShaders[1][1][1] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs", 0, aMacro2), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps", NULL, aMacro));
		s_idShaders[1][0][0] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs", 0, aMacro1), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps", NULL, aMacro));
		s_idShaders[1][0][1] = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "dev_points.vs", 0, aMacro3), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "dev_points.ps", NULL, aMacro));
	}


	m_pRightVecCB = pDev->createConstantBuffer(sizeof(float3));
}
CGizmoRenderer::~CGizmoRenderer()
{
	mem_release(m_pRenderUtils);
	mem_release(m_pRightVecCB);

	if(--s_uResRefCount)
	{
		mem_release(s_pBlendAlpha);
		mem_release(s_pDSState2D);
		mem_release(s_pDSState3D);
		mem_release(s_pDSStateNoZ);
		mem_release(s_pPointsVD);
	}

	mem_release(m_pPointsRB);
	mem_release(m_pPointsVB);
	mem_release(m_pPointsIB);
}

void XMETHODCALLTYPE CGizmoRenderer::reset()
{
	m_lineRenderer.reset();
	m_isDirty = true;
	m_aPoints.clearFast();

	mem_release(m_pCurrentTexture);
	m_isCurrentTextureDirty = true;
	for(UINT i = 0, l = m_aTextures.size(); i < l; ++i)
	{
		mem_release(m_aTextures[i]);
	}
	m_aTextures.clearFast();
}
void XMETHODCALLTYPE CGizmoRenderer::render(bool isOrtho, bool useConstantSize, bool useDepthTest)
{
	m_lineRenderer.render(isOrtho, useConstantSize, useDepthTest);

	if(m_isDirty)
	{
		m_isDirty = false;

		UINT uVertexCount = m_aPoints.size() * 4;
		if(m_uPointsVBSize < uVertexCount)
		{
			m_uPointsVBSize = uVertexCount;
			mem_release(m_pPointsVB);
			mem_release(m_pPointsRB);

			m_pPointsVB = m_pDev->createVertexBuffer(sizeof(PointVertex) * uVertexCount, GXBUFFER_USAGE_DYNAMIC);
			m_pPointsRB = m_pDev->createRenderBuffer(1, &m_pPointsVB, s_pPointsVD);
		}


		m_aPointRanges.clearFast();

		UINT uMaxQuadCount = 0;
		UINT uVC = 0;
		PointVertex *pVertices;
		if(m_pPointsVB->lock((void**)&pVertices, GXBL_WRITE))
		{
			byte u8TexIdx;
			m_aPointRanges.reserve(m_aTextures.size() + 1);
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

				PointRange &lr = m_aPointRanges[m_aPointRanges.size()];
				lr.u8Texture = u8TexIdx;

				lr.uStartVtx = uVC;
				for(UINT j = 0, jl = m_aPoints.size(); j < jl; ++j)
				{
					Point &pt = m_aPoints[j];
					if(pt.u8Texture == u8TexIdx)
					{
						float fPM = pt.mode == XGPM_SQUARE ? 0.0f : 1.0f;
						pt.vtx.vTexUVMode = float3_t(0.0f, 1.0f, fPM);
						pVertices[uVC++] = pt.vtx;
						pt.vtx.vTexUVMode = float3_t(0.0f, 0.0f, fPM);
						pVertices[uVC++] = pt.vtx;
						pt.vtx.vTexUVMode = float3_t(1.0f, 1.0f, fPM);
						pVertices[uVC++] = pt.vtx;
						pt.vtx.vTexUVMode = float3_t(1.0f, 0.0f, fPM);
						pVertices[uVC++] = pt.vtx;
					}
				}

				if(uVC > lr.uStartVtx)
				{
					lr.uQuadCount = (uVC - lr.uStartVtx) / 4;
					if(lr.uQuadCount > uMaxQuadCount)
					{
						uMaxQuadCount = lr.uQuadCount;
					}
				}
				else
				{
					m_aPointRanges.erase(m_aPointRanges.size() - 1);
				}
			}

			m_pPointsVB->unlock();
		}

		assert(uVC == uVertexCount);

		mem_release(m_pPointsIB);
		m_pRenderUtils->getQuadIndexBuffer(uMaxQuadCount, &m_pPointsIB);
	}

	IGXContext *pCtx = m_pDev->getThreadContext();
	
	IGXRasterizerState *pRS = pCtx->getRasterizerState();
	pCtx->setRasterizerState(NULL);
	IGXBlendState *pBS = pCtx->getBlendState();
	pCtx->setBlendState(s_pBlendAlpha);
	IGXDepthStencilState *pDS = pCtx->getDepthStencilState();
	pCtx->setDepthStencilState(isOrtho ? s_pDSState2D : s_pDSState3D);
	
	float3 vRight = gdata::pCamera->getRight();
	m_pRightVecCB->update(&vRight);
	pCtx->setVSConstant(m_pRightVecCB, 6);

	pCtx->setRenderBuffer(m_pPointsRB);
	pCtx->setIndexBuffer(m_pPointsIB);

	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);
	for(UINT i = 0, l = m_aPointRanges.size(); i < l; ++i)
	{
		PointRange &lr = m_aPointRanges[i];
		SGCore_ShaderBind(s_idShaders[lr.u8Texture != 0xFF ? 1 : 0][isOrtho ? 0 : 1][useConstantSize ? 1 : 0]);

		if(lr.u8Texture != 0xFF)
		{
			pCtx->setPSTexture(m_aTextures[lr.u8Texture]);
		}
		pCtx->drawIndexed(lr.uQuadCount * 4, lr.uQuadCount * 2, 0, lr.uStartVtx);
	}
	SGCore_ShaderUnBind();


	pCtx->setRasterizerState(pRS);
	mem_release(pRS);
	pCtx->setBlendState(pBS);
	mem_release(pBS);
	pCtx->setDepthStencilState(pDS);
	mem_release(pDS);
}


void XMETHODCALLTYPE CGizmoRenderer::setLineWidth(float fWidth)
{
	m_lineRenderer.setWidth(fWidth);
}

void XMETHODCALLTYPE CGizmoRenderer::setColor(const float4 &vColor)
{
	m_vCurrentColor = vColor;
	m_lineRenderer.setColor(vColor);
}

void XMETHODCALLTYPE CGizmoRenderer::setTexture(IGXBaseTexture *pTexture)
{
	m_lineRenderer.setTexture(pTexture);

	if(m_pCurrentTexture == pTexture)
	{
		return;
	}
	add_ref(pTexture);
	mem_release(m_pCurrentTexture);
	m_pCurrentTexture = pTexture;
	m_isCurrentTextureDirty = true;
}


void XMETHODCALLTYPE CGizmoRenderer::jumpTo(const float3 &vPos)
{
	m_lineRenderer.jumpTo(vPos);
}

void XMETHODCALLTYPE CGizmoRenderer::lineTo(const float3 &vPos)
{
	m_lineRenderer.lineTo(vPos);
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
	Point &pt = m_aPoints[m_aPoints.size()];
	PointVertex &vtx = pt.vtx;
	vtx.vPosWidth = float4(vPos, m_fCurrentPointSize);
	pt.mode = m_pointMode;
	pt.u8Texture = getTextureId();
	vtx.vColor = m_vCurrentColor;

	m_isDirty = true;
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

void XMETHODCALLTYPE CGizmoRenderer::drawEllipsoid(const float3 &vPos, const float3 &vSize)
{
	const int nSides = 16;
	const int nSlices = 8;

	float fUpStep = SM_PIDIV2 / (float)nSlices;
	float fRoundStep = SM_2PI / (float)nSides;

	for(int i = 0; i < nSlices; ++i)
	{
		float3 vec = SMQuaternion(fUpStep * (float)i, 'z') * float3(1.0f, 0.0f, 0.0f);

		jumpTo(vec * vSize + vPos);
		for(int j = 0; j <= nSides; ++j)
		{
			lineTo(SMQuaternion(fRoundStep * (float)j, 'y') * vec * vSize + vPos);
		}

		if(i)
		{
			vec.y *= -1.0f;
			jumpTo(vec * vSize + vPos);
			for(int j = 0; j <= nSides; ++j)
			{
				lineTo(SMQuaternion(fRoundStep * (float)j, 'y') * vec * vSize + vPos);
			}
		}
	}

	for(int j = 0; j < nSides; ++j)
	{
		
		for(int i = -nSlices; i <= nSlices; ++i)
		{
			float3 vec = SMQuaternion(fUpStep * (float)i, 'z') * SMQuaternion(fRoundStep * (float)j, 'y') * float3(1.0f, 0.0f, 0.0f) * vSize + vPos;
			if(i == -nSlices)
			{
				jumpTo(vec);
			}
			else
			{
				lineTo(vec);
			}
		}
	}
}

byte CGizmoRenderer::getTextureId()
{
	if(m_isCurrentTextureDirty)
	{
		m_isCurrentTextureDirty = false;

		if(m_pCurrentTexture)
		{
			int idx = m_aTextures.indexOf(m_pCurrentTexture);
			if(idx < 0)
			{
				idx = m_aTextures.size();
				m_aTextures.push_back(m_pCurrentTexture);
				add_ref(m_pCurrentTexture);
			}
			m_u8CurrentTexture = idx;
		}
		else
		{
			m_u8CurrentTexture = 0xFF;
		}
	}

	return(m_u8CurrentTexture);
}
