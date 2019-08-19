
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "crosshair.h"
#include <common/Math.h>
#include "GameData.h"

CCrosshair::CCrosshair():
	m_bDirty(true),
	m_bBuildBuff(false),
	m_fSize(0),
	m_idTexture(-1),
	m_bHidden(true),
	m_fMaxSize(0.7f), // 70% of screen height
	m_style(SPLIT_MOVE),
	m_fFixedRadius(0.0f),
	m_fAngle(0.0f),
	m_iNumSegs(4),
	m_u8ActiveBuffer(0),
	m_pTexture(NULL)
{
	m_pDev = SGCore_GetDXDevice();
	int iNumSegs = CROSSHAIR_MAXSEGS * 2;
	int segs = iNumSegs;
	while(segs < CROSSHAIR_MINSEGS)
	{
		segs += iNumSegs;
	}
	int iVC = (iNumSegs + segs + iNumSegs) * 2 + segs;
	int iIC = (segs - 2) * 3 + segs * 6;
	m_pMemoryBlob = new byte[(sizeof(Vertex) * iVC + sizeof(USHORT) * iIC) * 2];
	m_pVertices[0] = (Vertex*)&m_pMemoryBlob[0];
	m_pIndices[0] = (USHORT*)&m_pMemoryBlob[sizeof(Vertex)* iVC];
	m_pVertices[1] = (Vertex*)&m_pMemoryBlob[(sizeof(Vertex)* iVC + sizeof(USHORT)* iIC)];
	m_pIndices[1] = (USHORT*)&m_pMemoryBlob[sizeof(Vertex)* iVC + (sizeof(Vertex)* iVC + sizeof(USHORT)* iIC)];

	GXVertexElement vel[] = {
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		GX_DECL_END()
	};

	m_pVertexDeclaration = m_pDev->createVertexDeclaration(vel);
	//@TODO: Change to GXBUFFER_USAGE_STREAM (can be lost in DX9)
	m_pVertexBuffer = m_pDev->createVertexBuffer(sizeof(Vertex)* iVC, GXBUFFER_USAGE_DYNAMIC);
	m_pIndexBuffer = m_pDev->createIndexBuffer(sizeof(USHORT)* iIC, GXBUFFER_USAGE_DYNAMIC, GXIT_UINT16);
	m_pRenderBuffer = m_pDev->createRenderBuffer(1, &m_pVertexBuffer, m_pVertexDeclaration);

	m_idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs");
	m_idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps");
	m_idShaderKit = SGCore_ShaderCreateKit(m_idVS, m_idPS);

	GXBlendDesc blendDesc;
	//blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_SRC_ALPHA;
	//blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_SRC_ALPHA;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_ONE;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_ONE;
	blendDesc.renderTarget[0].useBlend = TRUE;
	m_pBlendState = m_pDev->createBlendState(&blendDesc);

	GXDepthStencilDesc dsDesc;
	dsDesc.useDepthTest = FALSE;
	dsDesc.useDepthWrite = FALSE;
	m_pDepthState = m_pDev->createDepthStencilState(&dsDesc);

	GXSamplerDesc sampDesc;
	sampDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	m_pSamplerState = m_pDev->createSamplerState(&sampDesc);
}

CCrosshair::~CCrosshair()
{
	mem_release(m_pSamplerState);
	mem_release(m_pBlendState);
	mem_release(m_pVertexBuffer);
	mem_release(m_pIndexBuffer);
	mem_release(m_pRenderBuffer);
	mem_release(m_pVertexDeclaration);

	mem_delete_a(m_pMemoryBlob);
}

void CCrosshair::setTexInfo(const float2_t & offs, const float2_t & size)
{
	m_bDirty = true;
	m_f2TexOffs = offs;
	m_f2TexSize = size;
}


void CCrosshair::update()
{
	if(m_bDirty && m_idTexture >= 0)
	{
		if(!m_pTexture)
		{
			m_pTexture = SGCore_LoadTexGetTex(m_idTexture);
			if(!m_pTexture)
			{
				return;
			}
		}

		static const int *r_win_width = GET_PCVAR_INT("r_win_width");
		static const int *r_win_height = GET_PCVAR_INT("r_win_height");

		//build new buffer
		float fScreenWidth = (float)*r_win_width;
		float fScreenHeight = (float)*r_win_height;
		float fTexWidth = m_f2TexSize.x;
		float fTexHeight = m_f2TexSize.y;
		float fXradius = fTexWidth / fScreenWidth * 0.5f;
		float fYradius = fTexHeight / fScreenHeight * 0.5f;
		float fYradiusMax = m_fMaxSize * fScreenHeight * 0.5f;
		float fXradiusMax = fYradiusMax * (fTexWidth / fTexHeight);
		float fXoffset = m_fSize * (fXradiusMax - fTexWidth * 0.5f) / fScreenWidth;
		float fYoffset = m_fSize * (fYradiusMax - fTexHeight * 0.5f) / fScreenHeight;

		float fXFixedRadius = m_fFixedRadius / fScreenWidth;
		float fYFixedRadius = m_fFixedRadius / fScreenHeight;

		float fTexLeft = m_f2TexOffs.x;
		float fTexTop = m_f2TexOffs.y;

		float fTexFullWidth = (float)m_pTexture->getWidth();
		float fTexFullHeight = (float)m_pTexture->getHeight();

		float fTexFRX = m_fFixedRadius / fTexWidth * 2.0f;
		float fTexFRY = m_fFixedRadius / fTexHeight * 2.0f;

		float2_t f2TexMin = float2_t(fTexLeft / fTexFullWidth, (fTexFullHeight - fTexTop) / fTexFullHeight);
		float2_t f2TexMax = f2TexMin + float2_t(fTexWidth / fTexFullWidth, -fTexHeight / fTexFullHeight);
		//f2TexMin = float2_t(0, 0);
		//f2TexMax = float2_t(1, 1);

		int segs = m_iNumSegs;
		while(segs < CROSSHAIR_MINSEGS)
		{
			segs += m_iNumSegs;
		}

		Vertex * pVertices = m_pVertices[m_u8ActiveBuffer ? 0 : 1];
		USHORT * pIndices = m_pIndices[m_u8ActiveBuffer ? 0 : 1];
		int iCurVtx = 0,
			iCurIdx = 0,
			iStopIdx = 0;

		float angle = m_fAngle;
		float fOX = 0.0f, fOY = 0.0f;
		int i = 0;
		bool stop;
		if(m_style == SPLIT_MOVE)
		{
			if(m_fFixedRadius == 0.0f)
			{
				//centered
				//iVC = m_iNumSegs + segs + m_iNumSegs;
				//iIC = segs * 3;

				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs; 
					angle < angleEnd; 
					angle += angleStep, ++i)
				{
					stop = i % (segs / m_iNumSegs) == 0;
					if(stop)
					{
						if(i != 0)
						{
							pIndices[iCurIdx++] = iStopIdx;
							pIndices[iCurIdx++] = iCurVtx;
							pIndices[iCurIdx++] = iCurVtx - 1;
							pVertices[iCurVtx++] = {
								float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
								float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
							}; // circle
						}

						float avgAngle = angle + (SM_2PI / (float)m_iNumSegs) * 0.5f;
						fOX = sinf(avgAngle) * fXoffset;
						fOY = -cosf(avgAngle) * fYoffset;

						iStopIdx = iCurVtx;
						pVertices[iCurVtx++] = {
							float3_t(fOX, fOY, 0.0f),
							float2_t(lerpf(f2TexMin.x, f2TexMax.x, 0.5f), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, 0.5f))
						}; // center
					}
					else
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
					//a=0 x=0 y=-1
				}
			}
			else
			{
				//iVC = (m_iNumSegs + segs + m_iNumSegs) * 2 + segs;
				//iIC = (segs - 2) * 3 + segs * 6;

				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					stop = i % (segs / m_iNumSegs) == 0;
					if(stop)
					{
						if(i != 0)
						{
							pIndices[iCurIdx++] = iCurVtx - 2;
							pIndices[iCurIdx++] = iCurVtx;
							pIndices[iCurIdx++] = iCurVtx - 1;

							pIndices[iCurIdx++] = iCurVtx;
							pIndices[iCurIdx++] = iCurVtx + 1;
							pIndices[iCurIdx++] = iCurVtx - 1;

							pVertices[iCurVtx++] = {
								float3_t(fXFixedRadius * sinf(angle) + fOX, fYFixedRadius * -cosf(angle) + fOY, 0.0f),
								float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
							}; // circle inner
							pVertices[iCurVtx++] = {
								float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
								float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
							}; // circle
						}

						float avgAngle = angle + (SM_2PI / (float)m_iNumSegs) * 0.5f;
						fOX = sinf(avgAngle) * fXoffset;
						fOY = -cosf(avgAngle) * fYoffset;

						/*pVertices[iCurVtx++] = {
							float3_t(fOX, fOY, 0.0f),
							float2_t(lerpf(f2TexMin.x, f2TexMax.x, 0.5f), -lerpf(f2TexMax.y, f2TexMin.y, 0.5f))
						}; // center
						*/
					}
					else
					{
						pIndices[iCurIdx++] = iCurVtx - 2;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;

						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx + 1;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle) + fOX, fYFixedRadius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
					pVertices[iCurVtx++] = {
						float3_t(fXradius * sinf(angle) + fOX, fYradius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), 1.0f-lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
					//a=0 x=0 y=-1
				}

				i = 0;
				iStopIdx = iCurVtx;
				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					if(i > 1)
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle), fYFixedRadius * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
				}
			}
		}
		else if(m_style == SCALED)
		{
			if(m_fFixedRadius == 0.0f)
			{
				//centered
				//iVC = segs;
				//iIC = (segs - 2) * 3;

				iStopIdx = iCurVtx;
				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					if(i > 1)
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t((fXradius + fXoffset) * sinf(angle), (fYradius + fYoffset) * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
				}

			}
			else
			{
				//iVC = segs * 2;
				//iIC = (segs - 2) * 3 + segs * 2;

				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					//if(i > 1)
					{
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx + 2;
						pIndices[iCurIdx++] = iCurVtx + 1;

						pIndices[iCurIdx++] = iCurVtx + 1;
						pIndices[iCurIdx++] = iCurVtx + 2;
						pIndices[iCurIdx++] = iCurVtx + 3;
					}


					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle) + fOX, fYFixedRadius * -cosf(angle) + fOY, 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
					pVertices[iCurVtx++] = {
						float3_t((fXradius + fXoffset) * sinf(angle), (fYradius + fYoffset) * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, sinf(angle) * 0.5f + 0.5f), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, -cosf(angle) * 0.5f + 0.5f))
					}; // circle
				}

				i = 0;
				iStopIdx = iCurVtx;
				for(float angleEnd = angle + SM_2PI, angleStep = SM_2PI / (float)segs;
					angle < angleEnd;
					angle += angleStep, ++i)
				{
					if(i > 1)
					{
						pIndices[iCurIdx++] = iStopIdx;
						pIndices[iCurIdx++] = iCurVtx;
						pIndices[iCurIdx++] = iCurVtx - 1;
					}
					pVertices[iCurVtx++] = {
						float3_t(fXFixedRadius * sinf(angle), fYFixedRadius * -cosf(angle), 0.0f),
						float2_t(lerpf(f2TexMin.x, f2TexMax.x, (fTexFRX * sinf(angle) * 0.5f + 0.5f)), 1.0f - lerpf(f2TexMax.y, f2TexMin.y, (fTexFRY * -cosf(angle) * 0.5f + 0.5f)))
					}; // circle inner
				}
			}
		}

		m_iVertexCount[m_u8ActiveBuffer ? 0 : 1] = iCurVtx;
		m_iIndexCount[m_u8ActiveBuffer ? 0 : 1] = iCurIdx;
	}
}
void CCrosshair::render()
{
	if(m_bHidden || !m_pTexture)
	{
		return;
	}
	if(m_bBuildBuff)
	{
		m_bBuildBuff = false;
		//update buffer
		void * pData;
		if(m_pVertexBuffer->lock(&pData, GXBL_WRITE))
		{
			memcpy(pData, m_pVertices[m_u8ActiveBuffer], sizeof(Vertex)* m_iVertexCount[m_u8ActiveBuffer]);
			m_pVertexBuffer->unlock();
		}

		if(m_pIndexBuffer->lock(&pData, GXBL_WRITE))
		{
			memcpy(pData, m_pIndices[m_u8ActiveBuffer], sizeof(USHORT)* m_iIndexCount[m_u8ActiveBuffer]);
			m_pIndexBuffer->unlock();
		}
	}
	SGCore_ShaderBind(m_idShaderKit);
	m_pDev->setBlendState(m_pBlendState);
	m_pDev->setSamplerState(m_pSamplerState, 0);
	m_pDev->setIndexBuffer(m_pIndexBuffer);
	m_pDev->setRenderBuffer(m_pRenderBuffer);
	m_pDev->setTexture(m_pTexture);
	m_pDev->setDepthStencilState(m_pDepthState);
	m_pDev->setPrimitiveTopology(GXPT_TRIANGLELIST);
	m_pDev->drawIndexed(m_iVertexCount[m_u8ActiveBuffer], m_iIndexCount[m_u8ActiveBuffer] / 3);
}
void CCrosshair::onSync()
{
	if(m_bDirty)
	{
		m_bDirty = false;

		//swap buffers
		m_u8ActiveBuffer = m_u8ActiveBuffer ? 0 : 1;

		m_bBuildBuff = true;
	}
}

void CCrosshair::enable(bool en)
{
	m_bHidden = !en;
}

void CCrosshair::setNumSegmens(int num)
{
	if(num < 1)
	{
		num = 1;
	}
	else if(num > CROSSHAIR_MAXSEGS)
	{
		num = CROSSHAIR_MAXSEGS;
	}
	if(m_iNumSegs != num)
	{
		m_bDirty = true;
		m_iNumSegs = num;
	}
}
void CCrosshair::setAngle(float a)
{
	if(m_fAngle != a)
	{
		m_bDirty = true;
		m_fAngle = a;
	}
}
void CCrosshair::setFixedRadius(float r)
{
	if(m_fFixedRadius != r)
	{
		m_bDirty = true;
		m_fFixedRadius = r;
	}
}
void CCrosshair::setStyle(STYLE style)
{
	if(m_style != style)
	{
		m_bDirty = true;
		m_style = style;
	}
}
void CCrosshair::setMaxSize(float size)
{
	m_bDirty = true;
	m_fMaxSize = size;
}
void CCrosshair::setTexture(ID id)
{
	if(m_idTexture != id)
	{
		m_bDirty = true;
		m_idTexture = id;
		if(m_idTexture == -1)
		{
			m_pTexture = NULL;
		}
		else
		{
			m_pTexture = SGCore_LoadTexGetTex(m_idTexture);
		}
	}
}

void CCrosshair::setSize(float size)
{
	if(m_fSize != size)
	{
		m_bDirty = true;
		m_fSize = size;
	}
}
