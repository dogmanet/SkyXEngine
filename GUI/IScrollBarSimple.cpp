#include "IScrollBarSimple.h"

namespace GUI
{
	namespace DOM
	{
		namespace Render
		{
			IScrollBarSimple::IScrollBarSimple(IRenderFrame * _parent, SCROLLBAR_DIR _dir):
				IScrollBar(_parent, _dir)
			{
			}

			int IScrollBarSimple::GetWidth()
			{
				return(16);
			}

			void IScrollBarSimple::Render()
			{
				static CPITexture texWhite = ITextureManager::GetTexture(TEX_WHITE);
				static CSHADER shText = ITextureManager::LoadShader(L"text");
				GetGUI()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

				ITextureManager::BindShader(shText);
				ITextureManager::BindTexture(texWhite);
				float4_t asd(0, 0, 0, 0.5);
				GetGUI()->GetDevice()->SetPixelShaderConstantF(0, (float*)&asd, 1);

				UpdateData();
				if(m_iScrollMax == 0)
				{
					return;
				}
				int P = 4; // Padding 4px
				int W = GetWidth();
				int S = m_iLength * (m_iLength - P) / m_iScrollMax;
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

				GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
			}

			void IScrollBarSimple::DispatchEvent(IEvent & ev)
			{
				//do nothing now
			}

		};
	};
};