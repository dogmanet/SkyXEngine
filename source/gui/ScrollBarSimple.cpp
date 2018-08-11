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
			}

			int CScrollBarSimple::getWidth()
			{
				return(16);
			}

			void CScrollBarSimple::render()
			{
				static CPITexture texWhite = CTextureManager::getTexture(TEX_WHITE);
				static CSHADER shText = CTextureManager::loadShader(L"text");
				GetGUI()->getDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

				CTextureManager::bindShader(shText);
				CTextureManager::bindTexture(texWhite);
				float4_t asd(1, 1, 1, 0.5);
				GetGUI()->getDevice()->SetPixelShaderConstantF(0, (float*)&asd, 1);

				updateData();
				if(m_iScrollMax == 0)
				{
					return;
				}
				int P = 4; // Padding 4px
				int W = getWidth();
				int S = m_iLength * (m_iLength - P) / (m_iScrollMax + m_iLength);
				int T = m_iScrollCur * (m_iLength - 2 * P - S) / m_iScrollMax;

				struct point
				{
					float x;
					float y;
					float z;
					float tx;
					float ty;
				};


				point a[6] = {
					{P, P + T, 0, 0, 0},
					{W - P, P + T, 0, 0, 1},
					{P, P + T + S, 0, 1, 0},
					{P, P + T + S, 0, 1, 0},
					{W - P, P + T, 0, 0, 1},
					{W - P, P + T + S, 0, 1, 1}
				};

				if(m_eDir == SCROLLBAR_DIR_HORIZONTAL)
				{
					float _x;
					for(int i = 0; i < 6; i++)
					{
						_x = a[i].x;
						a[i].x = a[i].y;
						a[i].y = _x;
					}
				}

				GetGUI()->getDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
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