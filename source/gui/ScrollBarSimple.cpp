#include "ScrollBarSimple.h"
#include "IRenderFrame.h"

namespace gui
{
	namespace dom
	{
		namespace render
		{
			CScrollBarSimple::CScrollBarSimple(IRenderFrame * _parent, SCROLLBAR_DIR _dir):
				IScrollBar(_parent, _dir)
			{
				m_pVertices = GetGUI()->getDevice()->createVertexBuffer(sizeof(point) * 4, GXBUFFER_USAGE_STREAM);
				m_pRenderBuffer = GetGUI()->getDevice()->createRenderBuffer(1, &m_pVertices, GetGUI()->getVertexDeclarations()->m_pXYZTex);
			}

			CScrollBarSimple::~CScrollBarSimple()
			{
				mem_release(m_pVertices);
				mem_release(m_pRenderBuffer);
			}
			
			int CScrollBarSimple::getWidth()
			{
				return(16);
			}

			void CScrollBarSimple::render()
			{
				static CPITexture texWhite = CTextureManager::getTexture(TEX_WHITE);
			//	static CSHADER shText = CTextureManager::loadShader(L"text");
			//	GetGUI()->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

				auto shader = GetGUI()->getShaders()->m_baseTexturedColored;
				SGCore_ShaderBind(shader.m_idShaderKit);

			//	CTextureManager::bindShader(shText);
				CTextureManager::bindTexture(texWhite);
			//	float4_t asd(1, 1, 1, 0.5);

				static IGXConstantBuffer *s_pColorConstant = NULL;
				if(!s_pColorConstant)
				{
					s_pColorConstant = GetGUI()->getDevice()->createConstantBuffer(sizeof(float4));
					s_pColorConstant->update(&float4_t(1, 1, 1, 0.5));
				}
				GetGUI()->getDevice()->setPixelShaderConstant(s_pColorConstant);
				//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, shader.m_idPS, "g_vColor", (float*)&float4_t(1, 1, 1, 0.5), 1);
			//	DX_CALL(GetGUI()->getDevice()->SetPixelShaderConstantF(0, (float*)&asd, 1));

				updateData();
				if(m_iScrollMax == 0)
				{
					return;
				}
				int P = 4; // Padding 4px
				int W = getWidth();
				int S = m_iLength * (m_iLength - P) / (m_iScrollMax + m_iLength);
				int T = m_iScrollCur * (m_iLength - 2 * P - S) / m_iScrollMax;

				point *pData;
				if(m_pVertices->lock((void**)&pData, GXBL_WRITE))
				{
					pData[0] = {P, P + T, 0, 0, 0};
					pData[1] = {W - P, P + T, 0, 0, 1};
					pData[2] = {W - P, P + T + S, 0, 1, 1};
					pData[3] = {P, P + T + S, 0, 1, 0};

					if(m_eDir == SCROLLBAR_DIR_HORIZONTAL)
					{
						float _x;
						for(int i = 0; i < 4; i++)
						{
							_x = pData[i].x;
							pData[i].x = pData[i].y;
							pData[i].y = _x;
						}
					}

					m_pVertices->unlock();
				}

				
				GetGUI()->getDevice()->setRenderBuffer(m_pRenderBuffer);
				GetGUI()->getDevice()->setIndexBuffer(GetGUI()->getQuadIndexBuffer());
				GetGUI()->getDevice()->drawIndexed(4, 2, 0, 0);
			//	DX_CALL(GetGUI()->getDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point)));
			}

			void CScrollBarSimple::dispatchEvent(IEvent & ev)
			{
				if(m_eDir == SCROLLBAR_DIR_VERTICAL && CKeyMap::keyState(KEY_LBUTTON))
				{
					if(ev.type == GUI_EVENT_TYPE_MOUSEMOVE)
					{
						if(!m_bDragging)
						{
							if(ev.offsetX > (int)m_pParent->getContentWidth() - getWidth())
							{
								m_bDragging = true;
								m_iDragPos = ev.clientY;
								m_iScrollStart = m_iScrollCur;
							}
						}
						else
						{
							int P = 4; // Padding 4px
							int S = m_iLength * (m_iLength - P) / m_iScrollMax;

							int delta = ev.clientY - m_iDragPos;
							m_pParent->setScrollTop(m_iScrollStart + delta * m_iScrollMax / (m_iLength - 2 * P - S));
						}
					}
				}
				else
				{
					m_bDragging = false;
				}
				//do nothing now
			}

		};
	};
};