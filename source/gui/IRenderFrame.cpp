#include "GUIbase.h"
#include "IRenderFrame.h"
#include "ITEXT.h"
#include "IFont.h"
#include "IVIDEO.h"
#include "CDOMdocument.h"
#include "IScrollBar.h"
#include "IScrollBarSimple.h"


namespace GUI
{
	namespace DOM
	{
		namespace Render
		{
			SMMATRIX ITranslationManager::m_result;
			Stack<SMMATRIX> ITranslationManager::m_stack;

			void ITranslationManager::PushMatrix(const SMMATRIX & m)
			{
				m_stack.push(m);
				m_result *= m;
				GetGUI()->GetDevice()->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&m_result));
			}

			void ITranslationManager::PopMatrix()
			{
				SMMATRIX m = m_stack.pop();
				float det;
				m_result = m_result * SMMatrixInverse(&det, m);
				GetGUI()->GetDevice()->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&m_result));
			}

			SMMATRIX ITranslationManager::GetCurrentMatrix()
			{
				return(m_result);
			}

			IRenderFrame::IRenderFrame(CDOMnode * pNode, IRenderFrame * pRootNode):
				m_bHasFixedSize(false),
				isWidthSet(true),
				m_iScrollLeft(0),
				m_iScrollLeftMax(0),
				m_iBackgroundOffsetX(0),
				m_iBackgroundOffsetY(0),
				m_pBackgroundImage(NULL),
				m_bHasBackground(false),
				m_iScrollTop(0),
				m_iScrollTopMax(0),
				m_bNeedCut(false),
				m_iTextIdx(0),
				m_iWidth(0),
				m_iHeight(0),
				m_iXpos(0),
				m_iYpos(0),
				m_pParent(NULL),
				m_iTopPos(0),
				m_iLeftPos(0),
				m_iTopPosMax(0),
				m_bIsOutOfFlow(false), 
				m_pDoc(NULL), 
				m_bFreezed(false),
				m_iScrollSpeedX(0),
				m_iScrollSpeedY(0),
				m_pScrollBarVert(NULL),
				m_pScrollBarHorz(NULL)
			{
				m_pNode = pNode;
				if(!pRootNode)
				{
					pRootNode = this;
				}
				m_pRootNode = pRootNode;
				m_pDoc = m_pRootNode->GetNode()->GetDocument();
				if(!pNode)
				{
					return;
				}
						
				m_pNode->SetRenderFrame(this);
				const IDOMnodeCollection * pChilds = pNode->GetChilds();
				bool bNeedBoxWrap = false;

				if(pChilds->size() == 0)
				{
					if(pNode->GetNodeId() == CDOMnode::GetNodeIdByName(L"textarea")
						|| pNode->GetNodeId() == CDOMnode::GetNodeIdByName(L"input")
						//|| pNode->GetNodeId() == IDOMnode::GetNodeIdByName(L"password")
						)
					{
						CDOMnode * newNode = (CDOMnode*)CDOMnode::CreateNode(L"text");
						newNode->SetDocument(pNode->GetDocument());
						pNode->AppendChild(newNode, false);
					}
				}

				for(UINT i = 0; i < pChilds->size(); i++)
				{
					CSS::ICSSstyle * css = (*pChilds)[i]->GetStyle();
					if(css->display->GetInt() == CSS::ICSSproperty::DISPLAY_BLOCK)
					{
						bNeedBoxWrap = true;
						break;
					}
				}
				IRenderFrame * pWrapper = NULL;
				for(UINT i = 0; i < pChilds->size(); i++)
				{
					CSS::ICSSstyle * css = (*pChilds)[i]->GetStyle();

					if(bNeedBoxWrap)
					{
						if(css->display->GetInt() == CSS::ICSSproperty::DISPLAY_BLOCK)
						{
							pWrapper = NULL;
						}
						else if(!pWrapper && (!css->display->IsSet() || css->display->GetInt() != CSS::ICSSproperty::DISPLAY_NONE))
						{
							pWrapper = IRenderFrame::CreateNode(NULL, pRootNode);
							AddChild(pWrapper);
						}
					}

					IRenderFrame * node = IRenderFrame::CreateNode((CDOMnode*)(*pChilds)[i], pRootNode);
					if(node)
					{
						if(css->position->GetInt() == CSS::ICSSproperty::POSITION_FIXED)
						{
							pRootNode->AddChild(node, true);
						}
						else if(css->position->GetInt() == CSS::ICSSproperty::POSITION_ABSOLUTE)
						{
							IRenderFrame * pCur = this;
							while(pCur->m_pParent && css->position->GetInt() == CSS::ICSSproperty::POSITION_STATIC)
							{
								pCur = pCur->m_pParent;
							}
							pCur->AddChild(node, true);
						}
						else
						{
							if(pWrapper)
							{
								pWrapper->AddChild(node);
							}
							else
							{
								AddChild(node);
							}
						}
					}

				}

				UpdateBorderColor();

				rc = 0x11000000 | ((rand() & 0xFF) << 16) | ((rand() & 0xFF) << 8) | (rand() & 0xFF);
				rc = 0xFFCCCCCC;
			}

			void IRenderFrame::ResetLayout(bool first)
			{
				if(m_pParent && m_pParent->HasFixedSize() && IsLastChild())
				{
					if(m_pParent->m_iTopPos < m_iHeight)
					{
						m_pParent->m_iTopPos = 0;
						m_pParent->m_iScrollTopMax = 0;
					}
					else
					{
						m_pParent->m_iTopPos -= m_iHeight;
						m_pParent->m_iScrollTopMax -= m_iHeight;
					}
				}
				if(!HasFixedSize())
				{
					m_iWidth = 0;
					m_iHeight = 0;
					m_iXpos = 0;
					m_iYpos = 0;
					m_iBackgroundOffsetX = 0;
					m_iBackgroundOffsetY = 0;
				}
				m_iScrollLeftMax = 0;
				m_iScrollTopMax = 0;
				m_iTopPos = 0;
				m_iLeftPos = 0;
				m_iTopPosMax = 0;

				if(!IsFreezzed())
				{
					for(UINT i = 0; i < m_pChilds.size(); i++)
					{
						m_pChilds[i]->ResetLayout(false);
					}
					for(UINT i = 0; i < m_pChildsOutFlow.size(); i++)
					{
						m_pChildsOutFlow[i]->ResetLayout(false);
					}
				}
			}

			void IRenderFrame::UpdateBorderColor()
			{
				if(m_pNode)
				{
					CSS::ICSSstyle * style = m_pNode->GetStyle();
					if(style->border_top_color->IsSet())
					{
						m_border.SetColor(IRenderBorder::TOP, style->border_top_color->GetColor());
					}
					if(style->border_right_color->IsSet())
					{
						m_border.SetColor(IRenderBorder::RIGHT, style->border_right_color->GetColor());
					}
					if(style->border_bottom_color->IsSet())
					{
						m_border.SetColor(IRenderBorder::BOTTOM, style->border_bottom_color->GetColor());
					}
					if(style->border_left_color->IsSet())
					{
						m_border.SetColor(IRenderBorder::LEFT, style->border_left_color->GetColor());
					}
				}
			}

			IRenderFrame::~IRenderFrame()
			{
				for(UINT i = 0, l = m_pChilds.size(); i < l; i++)
				{
					SX_SAFE_DELETE(m_pChilds[i]);
					//m_pChilds.erase(0);
				}
				for(UINT i = 0, l = m_pChildsOutFlow.size(); i < l; i++)
				{
					SX_SAFE_DELETE(m_pChildsOutFlow[i]);
					//m_pChildsOutFlow.erase(0);
				}
				SX_SAFE_DELETE(m_pScrollBarVert);
				SX_SAFE_DELETE(m_pScrollBarHorz);
			}

			UINT IRenderFrame::GetTopPosMax()
			{
				return(m_iTopPosMax);
			}
			void IRenderFrame::SetTopPosMax(UINT p)
			{
				m_iTopPosMax = p;
			}

			IRenderFrame * IRenderFrame::GetParent()
			{
				return(m_pParent);
			}

			IRenderFrame * IRenderFrame::GetNextSibling()
			{
				return(m_pNext);
			}
			IRenderFrame * IRenderFrame::GetPrevSibling()
			{
				return(m_pPrev);
			}

			CDOMnode * IRenderFrame::GetNode()
			{
				return(m_pNode);
			}

			UINT IRenderFrame::GetChildCount()
			{
				return(m_pChilds.size());
			}
			IRenderFrame * IRenderFrame::GetChild(UINT id)
			{
				if(id < 0 || id >= m_pChilds.size())
				{
					return(NULL);
				}
				return(m_pChilds[id]);
			}

			CSS::ICSSstyle * IRenderFrame::GetComputedStyle()
			{
				return(&m_ComputedStyle);
			}

			UINT IRenderFrame::GetWidth()
			{
				return(m_iWidth);
			}
			UINT IRenderFrame::GetHeight()
			{
				return(m_iHeight);
			}
			void IRenderFrame::SetXpos(int x)
			{
				m_iXpos = x;
			}
			int IRenderFrame::GetXpos()
			{
				return(m_iXpos);
			}
			int IRenderFrame::GetYpos()
			{
				return(m_iYpos);
			}

			void IRenderFrame::AddChild(IRenderFrame * pChild, bool bOutFlow)
			{
				Array<IRenderFrame*> * pArr = bOutFlow ? &m_pChildsOutFlow : &m_pChilds;
				pChild->IsOutOfFlow(bOutFlow);
				if(pArr->size() > 0)
				{
					IRenderFrame * last = (*pArr)[pArr->size() - 1];
					last->m_pNext = pChild;
					pChild->m_pPrev = last;
				}
				pArr->push_back(pChild);
				pChild->m_pParent = this;
			}

			void IRenderFrame::RemoveChild(IRenderFrame * pEl)
			{
				for(UINT i = 0, l = m_pChilds.size(); i < l; ++i)
				{
					if(m_pChilds[i] == pEl)
					{
						if(i > 0)
						{
							m_pChilds[i - 1]->m_pNext = m_pChilds[i]->m_pNext;
						}
						if(i + 1 < m_pChilds.size())
						{
							m_pChilds[i + 1]->m_pPrev = m_pChilds[i]->m_pPrev;
						}
						SX_SAFE_DELETE(m_pChilds[i]);
						m_pChilds.erase(i);
						return;
					}
				}

				for(UINT i = 0, l = m_pChildsOutFlow.size(); i < l; ++i)
				{
					if(m_pChildsOutFlow[i] == pEl)
					{
						if(i > 0)
						{
							m_pChildsOutFlow[i - 1]->m_pNext = m_pChildsOutFlow[i]->m_pNext;
						}
						if(i + 1 < m_pChildsOutFlow.size())
						{
							m_pChildsOutFlow[i + 1]->m_pPrev = m_pChildsOutFlow[i]->m_pPrev;
						}
						SX_SAFE_DELETE(m_pChildsOutFlow[i]);
						m_pChildsOutFlow.erase(i);
						return;
					}
				}
			}

			void IRenderFrame::SetParent(IRenderFrame * pParent)
			{
				m_pParent = pParent;
			}

			int IRenderFrame::GetScrollLeft()
			{
				return(m_iScrollLeft);
			}
			void IRenderFrame::SetScrollLeft(int x)
			{
				m_iScrollLeft = x;
			}

			int IRenderFrame::GetScrollTop()
			{
				return(m_iScrollTop);
			}
			int IRenderFrame::GetScrollTopMax()
			{
				return(m_iScrollTopMax);
			}
			int IRenderFrame::GetScrollLeftMax()
			{
				return(m_iScrollLeftMax);
			}
			void IRenderFrame::SetScrollTop(int x, bool _check_bounds)
			{
				if(_check_bounds)
				{
					if(x < 0)
					{
						x = 0;
					}
					else if(x > m_iScrollTopMax)
					{
						x = m_iScrollTopMax;
					}
				}
				m_iScrollTop = x;
			}

			//m_iScrollSpeedX

			void IRenderFrame::SetScrollSpeedX(int x)
			{
				m_iScrollSpeedX = x;
			}
			void IRenderFrame::SetScrollSpeedY(int x)
			{
				m_iScrollSpeedY = x;
			}
			void IRenderFrame::UpdateScroll()
			{
				int accell = 5;
				if(m_iScrollSpeedX != 0)
				{
					SetScrollLeft(m_iScrollLeft + m_iScrollSpeedX);
					int sign = m_iScrollSpeedX > 0 ? 1 : -1;

					m_iScrollSpeedX -= accell * sign;
					if(m_iScrollSpeedX != 0 && (m_iScrollSpeedX > 0 ? 1 : -1) != sign)
					{
						m_iScrollSpeedX = 0;
					}
				}
				if(m_iScrollSpeedY != 0)
				{
					SetScrollTop(m_iScrollTop + m_iScrollSpeedY, true);
					int sign = m_iScrollSpeedY > 0 ? 1 : -1;

					m_iScrollSpeedY -= accell * sign;
					if(m_iScrollSpeedY != 0 && (m_iScrollSpeedY > 0 ? 1 : -1) != sign)
					{
						m_iScrollSpeedY = 0;
					}
				}
			}


			IRenderFrame * IRenderFrame::CreateNode(CDOMnode * pNode, IRenderFrame * pRootNode)
			{
				if(!pNode)
				{
					return(new IRenderAnonymousBlock(pRootNode));
				}
				static UINT nIMG = CDOMnode::GetNodeIdByName(L"img");
				static UINT nVIDEO = CDOMnode::GetNodeIdByName(L"video");
				UINT nid = pNode->GetNodeId();
				if(nid == 0) // Text node
				{
					return(new IRenderTextNew(pNode, pRootNode));
				}
				
				
				switch(pNode->GetStyle()->display->GetInt())
				{
				case CSS::ICSSproperty::DISPLAY_NONE:
					pNode->SetRenderFrame(NULL);
					return(NULL);
				case CSS::ICSSproperty::DISPLAY_BLOCK:
					if(nid == nIMG || nid == nVIDEO)
					{
						return(new IRenderImageBlock(pNode, pRootNode));
					}
					else
					{
						return(new IRenderBlock(pNode, pRootNode));
					}
				case CSS::ICSSproperty::DISPLAY_INLINE_BLOCK:
					if(nid == nIMG || nid == nVIDEO)
					{
						return(new IRenderImageInlineBlock(pNode, pRootNode));
					}
					else
					{
						return(new IRenderInlineBlock(pNode, pRootNode));
					}
				case CSS::ICSSproperty::DISPLAY_INLINE:
					return(new IRenderInline(pNode, pRootNode));
				}
				return(NULL);
			}

			void IRenderFrame::DebugPrint(UINT lvl)
			{
				for(UINT i = 0; i < lvl; i++)
				{
					wprintf(L" ");
				}
				wprintf(L"%s: %d W: %d H:%d T: %d L:%d\n", m_pNode ? CDOMnode::GetNodeNameById(m_pNode->GetNodeId()).c_str() : L"Anonymous", m_bHasFixedSize ? 1: 0 , m_iWidth, m_iHeight, m_iYpos, m_iXpos);
				for(UINT i = 0; i < m_pChilds.size(); i++)
				{
					m_pChilds[i]->DebugPrint(lvl + 1);
				}
				for(UINT i = 0; i < m_pChildsOutFlow.size(); i++)
				{
					m_pChildsOutFlow[i]->DebugPrint(lvl + 1);
				}
			}

			void IRenderFrame::SetWidth(UINT w)
			{
				m_iWidth = w;
			}

			void IRenderFrame::SetHeight(UINT h)
			{
				m_iHeight = h;
			}

			UINT IRenderFrame::Layout()
			{
				TextClear();
				UINT h = 0;
				for(UINT i = 0; i < m_pChilds.size(); i++)
				{
					UINT p = m_pChilds[i]->Layout();
					h += p;
					m_iTopPos += p;
				}
				if(GetNode() && (GetNode()->GetStyle()->text_align->GetInt() == CSS::ICSSproperty::TEXT_ALIGN_CENTER || GetNode()->GetStyle()->text_align->GetInt() == CSS::ICSSproperty::TEXT_ALIGN_RIGHT))
				{
					int curY = 0;
					int curXpos = GetContentLeft();
					BOOL ctr = GetNode()->GetStyle()->text_align->GetInt() == CSS::ICSSproperty::TEXT_ALIGN_CENTER;
					for(UINT i = 0; i < m_pChilds.size(); i++)
					{
						if(m_pChilds[i]->GetYpos() == curY && m_pChilds[i]->GetNode() && m_pChilds[i]->GetNode()->GetStyle()->display->GetInt() == CSS::ICSSproperty::DISPLAY_INLINE_BLOCK)
						{
							curXpos += m_pChilds[i]->GetWidth();
						}
						else
						{
							int xDelta = ctr ? (GetContentWidth() - curXpos) / 2 : GetContentWidth() - curXpos;
							for(UINT ii = 0; ii < m_pChilds.size(); ii++)
							{
								if(m_pChilds[ii]->GetYpos() == curY&&m_pChilds[ii]->GetNode()->GetStyle()->display->GetInt() == CSS::ICSSproperty::DISPLAY_INLINE_BLOCK)
								{
									m_pChilds[ii]->SetXpos(xDelta + m_pChilds[ii]->GetXpos());
								}
							}

							curY = m_pChilds[i]->GetYpos();
							curXpos = GetContentLeft() + m_pChilds[i]->GetWidth();
						}
					}
					int xDelta = ctr ? (GetContentWidth() - curXpos) / 2 : GetContentWidth() - curXpos;
					for(UINT ii = 0; ii < m_pChilds.size(); ii++)
					{
						if(m_pChilds[ii]->GetYpos() == curY && m_pChilds[ii]->GetNode()->GetStyle()->display->GetInt() == CSS::ICSSproperty::DISPLAY_INLINE_BLOCK)
						{
							m_pChilds[ii]->SetXpos(xDelta + m_pChilds[ii]->GetXpos());
						}
					}
				}
				/*{
					if(GetNode() && GetNode()->GetStyle()->text_align->GetInt() == CSS::ICSSproperty::TEXT_ALIGN_CENTER)
					{
						UINT xDelta = (GetContentWidth() - m_iXpos) / 2;
						UINT curY = 0;
						for(UINT ii = 0, ll = GetChildCount(); ii < ll; ++ii)
						{
							IRenderFrame * rfcur = GetChild(ii);
							if(rfcur->GetYpos() == m_iYpos)
							{
								rfcur->SetXpos(rfcur->GetXpos() + xDelta);
							}
						}
					}
				}*/
				for(UINT i = 0; i < m_pChildsOutFlow.size(); i++)
				{
					UINT p = m_pChildsOutFlow[i]->Layout();
					//h += p;
					//m_iTopPos += p;
				}
				h += TextFixUp();
				if(m_pParent && m_pParent->HasFixedSize() && IsLastChild())
				{
					m_pParent->m_iTopPos += h;
					m_pParent->m_iScrollTopMax += h;
				}
				return(h);
			}

			UINT IRenderFrame::GetContentLeft()
			{
				if(!m_pNode)
				{
					return(0);
				}
				CSS::ICSSstyle * pStyle = m_pNode->GetStyle();
				return(pStyle->margin_left->GetPX(m_iWidth)
					+ pStyle->padding_left->GetPX(m_iWidth)
					+ pStyle->border_left_width->GetPX(m_iWidth));
			}

			UINT IRenderFrame::GetContentTop()
			{
				if(!m_pNode)
				{
					return(GetTopPos());
				}
				CSS::ICSSstyle * pStyle = m_pNode->GetStyle();
				return(GetTopPos()
					+ pStyle->margin_top->GetPX(m_iHeight)
					+ pStyle->padding_top->GetPX(m_iHeight)
					+ pStyle->border_top_width->GetPX(m_iHeight));
			}

			void IRenderFrame::Render(UINT lvl)
			{
				//UpdateScroll();
				for(UINT i = 0; i < m_pChilds.size(); i++)
				{
					if(m_pChilds[i]->GetNode() && (m_pChilds[i]->GetNode()->IsTextNode() || m_pChilds[i]->GetNode()->GetStyle()->display->GetInt() == CSS::ICSSproperty::DISPLAY_INLINE))
					{
						m_pChilds[i]->Render(lvl);
					}
					else
					{
						m_pChilds[i]->UpdateScroll();
						RECT rc = m_pChilds[i]->GetVisibleRect();
						if(rc.top < rc.bottom)
						{
							m_pChilds[i]->Render(lvl);
						}
					}
				}
				for(UINT i = 0; i < m_pChildsOutFlow.size(); i++)
				{
					m_pChildsOutFlow[i]->UpdateScroll();
					m_pChildsOutFlow[i]->Render(lvl);
				}
			}

			UINT IRenderFrame::GetTopPos()
			{
				return(m_iTopPos);
			}

			UINT IRenderFrame::GetContentWidth()
			{
				CSS::ICSSstyle * style = m_pNode ? m_pNode->GetStyle() : NULL;
				return(m_iWidth
					- (m_pNode ? style->margin_left->GetPX(m_iWidth)
					+ style->margin_right->GetPX(m_iWidth)
					+ style->padding_right->GetPX(m_iWidth)
					+ style->padding_left->GetPX(m_iWidth)
					+ style->border_left_width->GetPX(m_iWidth)
					+ style->border_right_width->GetPX(m_iWidth) : 0)); // FIXME: Compute the actual content area width
			}

			UINT IRenderFrame::GetInnerWidth()
			{
				CSS::ICSSstyle * style = m_pNode ? m_pNode->GetStyle() : NULL;
				return(m_iWidth
					- (m_pNode ? style->margin_left->GetPX(m_iWidth)
					+ style->margin_right->GetPX(m_iWidth)
					+ style->border_left_width->GetPX(m_iWidth)
					+ style->border_right_width->GetPX(m_iWidth) : 0)); // FIXME: Compute the actual content area width
			}

			UINT IRenderFrame::GetInnerHeight()
			{
				CSS::ICSSstyle * style = m_pNode ? m_pNode->GetStyle() : NULL;
				return(m_iHeight
					- (m_pNode ? style->margin_top->GetPX(m_iHeight)
					+ style->margin_bottom->GetPX(m_iHeight)
					+ style->border_top_width->GetPX(m_iHeight)
					+ style->border_bottom_width->GetPX(m_iHeight) : 0)); // FIXME: Compute the actual content area width
			}

			UINT IRenderFrame::GetContentHeight()
			{
				CSS::ICSSstyle * style = m_pNode ? m_pNode->GetStyle() : NULL;
				return(m_iHeight
					- (m_pNode ? style->margin_top->GetPX(m_iHeight)
					+ style->margin_bottom->GetPX(m_iHeight)
					+ style->padding_top->GetPX(m_iHeight)
					+ style->padding_bottom->GetPX(m_iHeight)
					+ style->border_top_width->GetPX(m_iHeight)
					+ style->border_bottom_width->GetPX(m_iHeight) : 0)); // FIXME: Compute the actual content area width
			}

			UINT IRenderFrame::GetInnerTop()
			{
				CSS::ICSSstyle * style = m_pNode ? m_pNode->GetStyle() : NULL;
				return(m_pNode ? style->margin_top->GetPX(m_iHeight)
					+ style->border_top_width->GetPX(m_iHeight)
						: 0);
			}
			UINT IRenderFrame::GetInnerLeft()
			{
				CSS::ICSSstyle * style = m_pNode ? m_pNode->GetStyle() : NULL;
				return(m_pNode ? style->margin_left->GetPX(m_iWidth)
					+ style->border_left_width->GetPX(m_iWidth) : 0);
			}

			UINT IRenderFrame::GetLeftPos()
			{
				return(m_iLeftPos);
			}

			void IRenderFrame::SetLeftPos(UINT p)
			{
				m_iLeftPos = p;
			}

			void IRenderFrame::SetTopPos(UINT p)
			{
				m_iTopPos = p;
			}

			UINT IRenderFrame::GetTopOffset()
			{
				UINT r = m_iYpos;
				if(m_pParent)
				{
					r += m_pParent->GetTopOffset();
				}
				return(r);
			}
			UINT IRenderFrame::GetLeftOffset()
			{
				UINT r = m_iXpos;
				if(m_pParent)
				{
					r += m_pParent->GetLeftOffset();
				}
				return(r);
			}

			UINT IRenderFrame::GetClientTop()
			{
				UINT r = m_iYpos - m_iScrollTop;
				if(m_pParent)
				{
					r += m_pParent->GetClientTop();
				}
				return(r);
			}
			UINT IRenderFrame::GetClientLeft()
			{
				UINT r = m_iXpos - m_iScrollLeft;
				if(m_pParent)
				{
					r += m_pParent->GetClientLeft();
				}
				return(r);
			}

			void IRenderFrame::TextStart()
			{
			}
			void IRenderFrame::TextBreak()
			{
				m_iTextIdx++;
			}
			void IRenderFrame::TextAppend(renderElement * rel, int iLineIdx)
			{
				m_mTextRELs[iLineIdx].push_back(rel);
			}
			int IRenderFrame::TextGetLineIdx()
			{
				return(m_iTextIdx);
			}
			void IRenderFrame::TextClear()
			{
				for(UINT i = 0; i < m_mTextRELs.size(); i++)
				{
					for(UINT j = 0; j < m_mTextRELs[i].size(); j++)
					{
						SX_SAFE_RELEASE(m_mTextRELs[i][j]->m_pIndexBuffer);
						SX_SAFE_RELEASE(m_mTextRELs[i][j]->m_pVertexBuffer);
					}
				}
				m_mTextRELs.clear();
				m_iTextIdx = 0;
			}
			UINT IRenderFrame::TextFixUp()
			{
				UINT h = 0;
				UINT iMaxOffs = 0;
				UINT iMaxSize = 0;
				
				if(m_pNode)
				{
					UINT iMaxWidth = 0;

					for(UINT i = 0; i < m_mTextRELs.size(); i++)
					{
						UINT _iMaxWidth = 0;
						for(UINT j = 0; j < m_mTextRELs[i].size(); j++)
						{
							_iMaxWidth += m_mTextRELs[i][j]->m_iWIdth;
						}
						iMaxWidth = max(_iMaxWidth, iMaxWidth);
					}
					//msvcprtd.lib;LIBCMTD.lib;
					CSS::ICSSstyle * style = m_pNode->GetStyle();
					UINT width = m_pParent ? m_pParent->GetWidth() : GetWidth();
					UINT iNewWidth = iMaxWidth
					//	+ style->margin_left->GetPX(width)
					//	+ style->margin_right->GetPX(width)
						+ style->padding_left->GetPX(width)
						+ style->padding_right->GetPX(width)
						+ style->border_left_width->GetPX(width)
						+ style->border_right_width->GetPX(width);
					if(!isWidthSet)
					{
						m_iWidth = iNewWidth;
					}
					else
					{
						if(m_iWidth < iNewWidth)
						{
							m_bNeedCut = true;
							m_iScrollLeftMax = iNewWidth - m_iWidth;
							m_iScrollLeft = m_iScrollLeftMax;
						}
					}
				}
				UINT iAreaWidth = GetContentWidth();
				CSS::ICSSstyle * pStyle = m_pNode ? m_pNode->GetStyle() : m_pParent->GetNode()->GetStyle();
				CSS::ICSSproperty::TYPE_TEXT_ALIGN_e _textAlign = (CSS::ICSSproperty::TYPE_TEXT_ALIGN_e)pStyle->text_align->GetInt();
				bool bAlignCenter = false;
				bool bAlignRight = false;
				if(_textAlign == CSS::ICSSproperty::TEXT_ALIGN_CENTER)
				{
					bAlignCenter = true;
				}
				if(_textAlign == CSS::ICSSproperty::TEXT_ALIGN_RIGHT)
				{
					bAlignRight = true;
				}

				for(UINT i = 0; i < m_mTextRELs.size(); i++)
				{
					iMaxOffs = 0;
					iMaxSize = 0;
					if(m_mTextRELs[i].size() > 1)
					{
						
						for(UINT j = 0; j < m_mTextRELs[i].size(); j++)
						{
							iMaxSize = max(iMaxSize, m_mTextRELs[i][j]->m_iHeight);
						}
						for(UINT j = 0; j < m_mTextRELs[i].size(); j++)
						{
							h = max(h, iMaxSize - m_mTextRELs[i][j]->m_iHeight);
							m_mTextRELs[i][j]->m_iTopOffset += iMaxSize - m_mTextRELs[i][j]->m_iHeight;
						}
					}
					else if(m_mTextRELs[i].size() == 1)
					{
						iMaxSize = max(iMaxSize, m_mTextRELs[i][0]->m_iHeight);
					}
					h += iMaxOffs;

					if(bAlignCenter || bAlignRight)
					{
						UINT _w = 0;
						for(UINT j = 0; j < m_mTextRELs[i].size(); j++)
						{
							_w += m_mTextRELs[i][j]->m_iWIdth;
						}
						int _offs = (int)iAreaWidth - (int)_w;;
						if(bAlignCenter)
						{
							_offs /= 2;
						}
						if(_offs > 0)
						{
							for(UINT j = 0; j < m_mTextRELs[i].size(); j++)
							{
								m_mTextRELs[i][j]->m_iLeftOffset += _offs;
								for(UINT o = m_mTextRELs[i][j]->m_iFirstSym; o <= m_mTextRELs[i][j]->m_iLastSym; o++)
								{
									if((*m_mTextRELs[i][j]->m_prc)[o].left > 65555)
									{
										_asm
										{
											int 3;
										};
									}
									(*m_mTextRELs[i][j]->m_prc)[o].left += _offs;
									(*m_mTextRELs[i][j]->m_prc)[o].right += _offs;
								}
							}
						}
					}
				}
			//	if(m_mTextRELs.size() == 1)
				{
					h += iMaxSize;
				}
				return(h);
			}


			void IRenderFrame::UpdateBorder()
			{
				if(m_pNode)
				{
					CSS::ICSSstyle * style = m_pNode->GetStyle();

					m_border.SetRadius(IRenderBorder::TOP_LEFT, 0);
					m_border.SetRadius(IRenderBorder::TOP_RIGHT, 0);
					m_border.SetRadius(IRenderBorder::BOTTOM_LEFT, 0);
					m_border.SetRadius(IRenderBorder::BOTTOM_RIGHT, 0);

					m_border.SetCornerMethod(IRenderBorder::TOP_LEFT, IRenderBorder::CORNER_METHOD_RADIUS);
					m_border.SetCornerMethod(IRenderBorder::TOP_RIGHT, IRenderBorder::CORNER_METHOD_RADIUS);
					m_border.SetCornerMethod(IRenderBorder::BOTTOM_LEFT, IRenderBorder::CORNER_METHOD_RADIUS);
					m_border.SetCornerMethod(IRenderBorder::BOTTOM_RIGHT, IRenderBorder::CORNER_METHOD_RADIUS);

					m_border.SetWidth(IRenderBorder::TOP, 0);
					m_border.SetWidth(IRenderBorder::RIGHT, 0);
					m_border.SetWidth(IRenderBorder::BOTTOM, 0);
					m_border.SetWidth(IRenderBorder::LEFT, 0);

					if(style->border_top_left_radius->IsSet())
					{
						m_border.SetRadius(IRenderBorder::TOP_LEFT, style->border_top_left_radius->GetPX(0));
					}
					if(style->border_top_right_radius->IsSet())
					{
						m_border.SetRadius(IRenderBorder::TOP_RIGHT, style->border_top_right_radius->GetPX(0));
					}
					if(style->border_bottom_left_radius->IsSet())
					{
						m_border.SetRadius(IRenderBorder::BOTTOM_LEFT, style->border_bottom_left_radius->GetPX(0));
					}
					if(style->border_bottom_right_radius->IsSet())
					{
						m_border.SetRadius(IRenderBorder::BOTTOM_RIGHT, style->border_bottom_right_radius->GetPX(0));
					}



					if(style->border_top_left_method->IsSet())
					{
						m_border.SetCornerMethod(IRenderBorder::TOP_LEFT, style->border_top_left_method->GetInt() == CSS::ICSSproperty::BORDER_METHOD_RADIUS ? IRenderBorder::CORNER_METHOD_RADIUS : IRenderBorder::CORNER_METHOD_CUT);
					}
					if(style->border_top_right_method->IsSet())
					{
						m_border.SetCornerMethod(IRenderBorder::TOP_RIGHT, style->border_top_right_method->GetInt() == CSS::ICSSproperty::BORDER_METHOD_RADIUS ? IRenderBorder::CORNER_METHOD_RADIUS : IRenderBorder::CORNER_METHOD_CUT);
					}
					if(style->border_bottom_right_method->IsSet())
					{
						m_border.SetCornerMethod(IRenderBorder::BOTTOM_RIGHT, style->border_bottom_right_method->GetInt() == CSS::ICSSproperty::BORDER_METHOD_RADIUS ? IRenderBorder::CORNER_METHOD_RADIUS : IRenderBorder::CORNER_METHOD_CUT);
					}
					if(style->border_bottom_left_method->IsSet())
					{
						m_border.SetCornerMethod(IRenderBorder::BOTTOM_LEFT, style->border_bottom_left_method->GetInt() == CSS::ICSSproperty::BORDER_METHOD_RADIUS ? IRenderBorder::CORNER_METHOD_RADIUS : IRenderBorder::CORNER_METHOD_CUT);
					}

					if(style->border_top_width->IsSet())
					{
						m_border.SetWidth(IRenderBorder::TOP, style->border_top_width->GetPX(m_iHeight));
					}
					if(style->border_right_width->IsSet())
					{
						m_border.SetWidth(IRenderBorder::RIGHT, style->border_right_width->GetPX(m_iWidth));
					}
					if(style->border_bottom_width->IsSet())
					{
						m_border.SetWidth(IRenderBorder::BOTTOM, style->border_bottom_width->GetPX(m_iWidth));
					}
					if(style->border_left_width->IsSet())
					{
						m_border.SetWidth(IRenderBorder::LEFT, style->border_left_width->GetPX(m_iHeight));
					}
					{
						UINT t = m_border.GetWidth(IRenderBorder::TOP);
						UINT l = m_border.GetWidth(IRenderBorder::LEFT);
						UINT b = m_border.GetWidth(IRenderBorder::BOTTOM);
						UINT r = m_border.GetWidth(IRenderBorder::RIGHT);
						UINT tl = m_border.GetRadius(IRenderBorder::TOP_LEFT);
						UINT tr = m_border.GetRadius(IRenderBorder::TOP_RIGHT);
						UINT bl = m_border.GetRadius(IRenderBorder::BOTTOM_LEFT);
						UINT br = m_border.GetRadius(IRenderBorder::BOTTOM_RIGHT);

						if((tl > t || tl > l)
							|| (tr > t || tr > r)
							|| (bl > b || bl > l)
							|| (br > b || br > r)
							)
						{
							m_bNeedCut = true;
						}
					}
					m_border.SetContentSize(m_iWidth
						- style->margin_left->GetPX(m_iWidth)
						- style->margin_right->GetPX(m_iWidth)
						- style->border_left_width->GetPX(m_iWidth)
						- style->border_right_width->GetPX(m_iWidth),
						m_iHeight
						- style->margin_top->GetPX(m_iHeight)
						- style->margin_bottom->GetPX(m_iHeight)
						- style->border_top_width->GetPX(m_iHeight)
						- style->border_bottom_width->GetPX(m_iHeight)
						);
					m_border.Build();
				}
			}

			IRenderBlock::IRenderBlock(CDOMnode * pNode, IRenderFrame * pRootNode):BaseClass(pNode, pRootNode)
			{

			}

			UINT IRenderBlock::Layout()
			{
				bool bChanged = !HasFixedSize();
				m_bHasFixedSize = false;
				CSS::ICSSstyle * style = m_pNode->GetStyle();
				UINT _parentWidth = 0;
				UINT _parentHeight = 0;
				if(bChanged)
				{
					if(m_pParent)
					{
						_parentWidth = m_pParent->GetContentWidth();
						_parentHeight = m_pParent->GetContentHeight();
						if(!style->width->IsSet())
						{
							m_iWidth = _parentWidth;
						}
						else
						{
							m_iWidth = style->width->GetPX(_parentWidth);
						}
						m_iYpos = m_pParent->GetContentTop();
					}
					else
					{
						_parentWidth = m_iWidth;
						_parentHeight = m_iHeight;
					}
				}
				bool hSet = m_iHeight != 0;
				if(style->height->IsSet() && !hSet)
				{
					m_iHeight = style->height->GetPX(_parentHeight);
					hSet = true;
				}
				bool bReturnHeight = true;
				if(bChanged)
				{
					if(m_pParent)
					{
						m_iXpos = m_pParent->GetContentLeft();
					}
					
					if(style->position->IsSet())
					{
						switch(style->position->GetInt())
						{
						case CSS::ICSSproperty::POSITION_RELATIVE:
							if(style->left->IsSet())
							{
								m_iXpos += style->left->GetPX(_parentWidth);
							}
							else if(style->right->IsSet())
							{
								m_iXpos -= style->right->GetPX(_parentWidth);
							}
							if(style->top->IsSet())
							{
								m_iYpos += style->top->GetPX(_parentHeight);
							}
							else if(style->bottom->IsSet())
							{
								m_iYpos -= style->bottom->GetPX(_parentHeight);
							}
							break;

						case CSS::ICSSproperty::POSITION_FIXED:
						case CSS::ICSSproperty::POSITION_ABSOLUTE:
							if(style->left->IsSet())
							{
								m_iXpos = style->left->GetPX(_parentWidth);
								if(/*!style->p_width->IsSet() && */style->right->IsSet())
								{
									m_iWidth = _parentWidth - m_iXpos - style->right->GetPX(_parentWidth);
								}
							}
							else if(style->right->IsSet())
							{
								m_iXpos = _parentWidth - m_iWidth - style->right->GetPX(_parentWidth);
							}

							if(style->top->IsSet())
							{
								m_iYpos = style->top->GetPX(_parentHeight);
								if(!style->height->IsSet() && style->bottom->IsSet())
								{
									m_iHeight = _parentHeight - m_iYpos - style->bottom->GetPX(_parentHeight);
									hSet = true;
								}
							}
							else if(style->bottom->IsSet())
							{
								m_iYpos = _parentHeight - m_iHeight - style->bottom->GetPX(_parentHeight);
							}
							bReturnHeight = false;
							break;
						}
					}
				}
				if(!IsFreezzed())
				{
					UINT height = BaseClass::Layout();
					UINT hNew = height
						+ style->margin_top->GetPX(height)
						+ style->margin_bottom->GetPX(height)
						+ style->padding_top->GetPX(height)
						+ style->padding_bottom->GetPX(height)
						+ style->border_top_width->GetPX(height)
						+ style->border_bottom_width->GetPX(height);
					if(!hSet/* && !m_iHeight*/)
					{
						m_iHeight = hNew;
						hSet = true;
					}
					else
					{
						m_bHasFixedSize = true;
						if(hNew > m_iHeight)
						{
							m_iScrollTopMax = hNew - m_iHeight;
							m_bNeedCut = true;
						}
					}
					UpdateBorder();
					UpdateBackground();
				}
				return(bReturnHeight ? m_iHeight : 0);
			}

			void IRenderBlock::Render(UINT lvl)
			{
						
				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				if(m_iScrollTop != 0 || m_iScrollTopMax != 0)
				{
					//m_iScrollTop = Config::TestScrollPos;
					if(m_iScrollTop > m_iScrollTopMax)
					{
						m_iScrollTop = /*Config::TestScrollPos = */m_iScrollTopMax;
					}
					if(m_iScrollTop < 0)
					{
						m_iScrollTop/* = Config::TestScrollPos*/ = 0;
					}
				}
				if(m_iScrollLeftMax != 0)
				{
					//m_iScrollTop = Config::TestScrollPos;
					if(m_iScrollLeft > m_iScrollLeftMax)
					{
						m_iScrollLeft = /*Config::TestScrollPos = */m_iScrollLeftMax;
					}
					if(m_iScrollLeft < 0)
					{
						m_iScrollLeft/* = Config::TestScrollPos*/ = 0;
					}
				}
				if(m_iScrollTopMax != 0 && !m_pScrollBarVert)
				{
					m_pScrollBarVert = new IScrollBarSimple(this, SCROLLBAR_DIR_VERTICAL);
				}
				if(m_iScrollLeftMax != 0 && !m_pScrollBarHorz)
				{
					m_pScrollBarHorz = new IScrollBarSimple(this, SCROLLBAR_DIR_HORIZONTAL);
				}
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));

				if(m_pNode->GetStyle()->visibility->GetInt() != CSS::ICSSproperty::VISIBILITY_HIDDEN)
				{

					struct point
					{
						float x;
						float y;
						float z;
						DWORD rc;
					};
					
					point a[6] = {
						{0, 0, 0, rc},
						{m_iWidth, 0, 0, rc},
						{0, (float)(m_iHeight), 0, rc},
						{0, (float)(m_iHeight), 0, rc},
						{m_iWidth, 0, 0, rc},
						{m_iWidth, (float)(m_iHeight), 0, rc}
					};

					

				//	GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));

					if(m_bNeedCut)
					{
						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
						m_border.RenderInnerFill();

						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
					//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl + 1);
					//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
					}

					m_border.Render();
							
					if(m_bNeedCut)
					{
					//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);
					//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
					}
					RenderBackground(lvl + (m_bNeedCut ? 1 : 0));

					if(m_pScrollBarVert || m_pScrollBarHorz)
					{
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl + (m_bNeedCut ? 1 : 0));
					}
					if(m_pScrollBarVert)
					{
						int sbOffset = m_iWidth - m_pScrollBarVert->GetWidth();
						ITranslationManager::PushMatrix(SMMatrixTranslation(sbOffset, 0.0f, 0.0f));
						m_pScrollBarVert->SetLength(m_iHeight);
						m_pScrollBarVert->Render();
						ITranslationManager::PopMatrix();
					}
					if(m_pScrollBarHorz)
					{
						int sbOffset = m_iHeight - m_pScrollBarHorz->GetWidth();
						ITranslationManager::PushMatrix(SMMatrixTranslation(0.0f, sbOffset, 0.0f));
						m_pScrollBarHorz->SetLength(m_iWidth);
						m_pScrollBarHorz->Render();
						ITranslationManager::PopMatrix();
					}


					ITranslationManager::PushMatrix(SMMatrixTranslation(-m_iScrollLeft, -m_iScrollTop, 0.0f));
					BaseClass::Render(lvl + (m_bNeedCut ? 1 : 0));
					ITranslationManager::PopMatrix();
					if(m_bNeedCut)
					{
						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
						GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
						if(lvl == 0)
						{
							GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
						}
					}
				}

				ITranslationManager::PopMatrix();
			}

			void IRenderFrame::UpdateBackground()
			{
				if(!m_pNode)
				{
					return;
				}

				CSS::ICSSstyle * pStyle = m_pNode->GetStyle();

				m_bHasBackgroundImage = false;
				m_bHasBackground = false;

				if((pStyle->background_color->GetInt() & 0x000000FF) != 0)
				{
					m_pBackgroundColor = pStyle->background_color->GetColor();
					m_iBackgroundColor = ((pStyle->background_color->GetInt() & 0xFFFFFF00) >> 8) | ((pStyle->background_color->GetInt() & 0xFF) << 24);
					m_bHasBackground = true;
				}
				if(pStyle->background_image->IsSet())
				{
					m_bHasBackgroundImage = true;
					if(!m_bHasBackground)
					{
						m_iBackgroundColor = 0x00FFFFFF;
						m_pBackgroundColor = float4_t(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
				
				if(!m_bHasBackgroundImage && !m_bHasBackground)
				{
					return;
				}

				float w = GetInnerWidth();
				float h = GetInnerHeight();

				m_pVBackground[0].Pos = float3_t(0.0f, 0.0f, 0.0f);
				m_pVBackground[1].Pos = float3_t(w, 0.0f, 0.0f);
				m_pVBackground[2].Pos = float3_t(0.0f, h, 0.0f);
				m_pVBackground[3].Pos = float3_t(0.0f, h, 0.0f);
				m_pVBackground[4].Pos = float3_t(w, 0.0f, 0.0f);
				m_pVBackground[5].Pos = float3_t(w, h, 0.0f);

				if(m_bHasBackgroundImage)
				{
					m_pBackgroundImage = ITextureManager::GetTexture(pStyle->background_image->GetString());
					UINT tw = m_pBackgroundImage->GetWidth();
					UINT th = m_pBackgroundImage->GetHeight();

					

					IRenderFrame * pCur = this;
					m_bBackgroundFixed = false;
					switch(pStyle->background_attachment->GetInt())
					{
					case CSS::ICSSproperty::BACKGROUND_ATTACHMENT_FIXED:
						m_bBackgroundScrolling = false;
						while(pCur->m_pParent)
						{
							pCur = pCur->m_pParent;
						}
						w = pCur->m_iWidth;
						h = pCur->m_iHeight;
						m_bBackgroundFixed = true;
						break;

					case CSS::ICSSproperty::BACKGROUND_ATTACHMENT_LOCAL:
						m_bBackgroundScrolling = true;
						break;

					case CSS::ICSSproperty::BACKGROUND_ATTACHMENT_SCROLL:
						m_bBackgroundScrolling = false;
						break;
					}

					float fAspect = (float)tw / (float)th;
					bool s[2] = {false};
					if(pStyle->background_size_x->IsSet())
					{
						tw = pStyle->background_size_x->GetPX(w);
						s[0] = true;
					}
					if(pStyle->background_size_y->IsSet())
					{
						th = pStyle->background_size_y->GetPX(h);
						s[1] = true;
					}

					if(s[0] && !s[1])
					{
						th = (float)tw / fAspect;
					}
					if(!s[0] && s[1])
					{
						tw = (float)th * fAspect;
					}

					float tu = w / (float)tw;
					float tv = h / (float)th;

					m_fBackgroundImageSize = float2_t(tw, th);

					if(pStyle->background_position_x->IsSet())
					{
						m_iBackgroundOffsetX = -pStyle->background_position_x->GetPX(w - tw);
					}
					else
					{
						m_iBackgroundOffsetX = 0;
					}

					if(pStyle->background_position_y->IsSet())
					{
						m_iBackgroundOffsetY = -pStyle->background_position_y->GetPX(h - th);
					}
					else
					{
						m_iBackgroundOffsetY = 0;
					}

					if(pStyle->background_repeat->IsSet())
					{
						int val = pStyle->background_repeat->GetInt();
						m_bBackgroundRepeatX = 0 != (val & CSS::ICSSproperty::BACKGROUND_REPEAT_REPEAT_X);
						m_bBackgroundRepeatY = 0 != (val & CSS::ICSSproperty::BACKGROUND_REPEAT_REPEAT_Y);
					}

					m_pVBackground[0].Tex = float2_t(0.0f, 0.0f);
					m_pVBackground[1].Tex = float2_t(tu, 0.0f);
					m_pVBackground[2].Tex = float2_t(0.0f, tv);
					m_pVBackground[3].Tex = float2_t(0.0f, tv);
					m_pVBackground[4].Tex = float2_t(tu, 0.0f);
					m_pVBackground[5].Tex = float2_t(tu, tv);
				}
				

				//background-image
			}

			void IRenderFrame::RenderBackground(UINT lvl)
			{
				if(!m_bHasBackgroundImage && !m_bHasBackground)
				{
					return;
				}
				static CPITexture texWhite = ITextureManager::GetTexture(TEX_WHITE);
				static CSHADER shText = ITextureManager::LoadShader(L"text");

				if(m_bHasBackgroundImage)
				{
				//	ITextureManager::BindTexture(m_pBackgroundImage);

					//SMMATRIX m = SMMatrixTranslation(0.5f, -m_iScrollTop, 0.0f);
					SMMATRIX m;

					
					int pt = 0;
					int pl = 0;
					if(m_bBackgroundFixed)
					{
						SMMATRIX mt = ITranslationManager::GetCurrentMatrix();
						pl = mt._41;
						pt = mt._42;
					}

					m._31 = (float)((m_bBackgroundScrolling ? m_iScrollLeft : 0) + m_iBackgroundOffsetX + pl) / m_fBackgroundImageSize.x; // X-shift
					m._32 = (float)((m_bBackgroundScrolling ? m_iScrollTop : 0) + m_iBackgroundOffsetY + pt) / m_fBackgroundImageSize.y;
					
					
					GetGUI()->GetDevice()->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX*)&m);
					GetGUI()->GetDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
					GetGUI()->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, m_bBackgroundRepeatX ? D3DTADDRESS_WRAP : D3DTADDRESS_BORDER);
					GetGUI()->GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, m_bBackgroundRepeatY ? D3DTADDRESS_WRAP : D3DTADDRESS_BORDER);
					GetGUI()->GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, m_iBackgroundColor);
				}
				else
				{
					
				}
				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				GetGUI()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
				GetGUI()->GetDevice()->SetPixelShaderConstantF(0, (float*)&m_pBackgroundColor, 1);

				GetGUI()->GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);

				ITranslationManager::PushMatrix(SMMatrixTranslation(GetInnerLeft(), GetInnerTop(), 0.0f));
				if(m_bHasBackground)
				{
					ITextureManager::BindShader(shText);
					ITextureManager::BindTexture(texWhite);
					GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_pVBackground, sizeof(pointtex));
				}
				if(m_bHasBackgroundImage)
				{
					ITextureManager::UnbindShader();
					ITextureManager::BindTexture(m_pBackgroundImage);
					GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_pVBackground, sizeof(pointtex));
				}
				ITranslationManager::PopMatrix();
				

				
				
				

				

				if(m_bHasBackgroundImage)
				{
					GetGUI()->GetDevice()->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0xFFFFFFFF);
					GetGUI()->GetDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
				}
			}

			void IRenderFrame::UpdateStyles()
			{
				if(m_pNode)
				{
					UINT flags = ((CSS::CCSSstyle*)m_pNode->GetStyle())->GetChangesFlags();
					if(flags)
					{
						if(flags & CSS::ICSSproperty::FLAG_UPDATE_STRUCTURE)
						{
							//TODO: Make structure changes
							//m_pNode->GetDocument()->
							return;
						}
						if(flags & CSS::ICSSproperty::FLAG_UPDATE_LAYOUT)
						{
						//	if(m_pNode->GetNodeId() != IDOMnode::GetNodeIdByName(L"video"))
							{
								//GetNode()->GetDocument()->
								m_bHasFixedSize = false; 
								m_pDoc->AddReflowItem(this);
							}
							//	Layout();
							//	return;
						}
						else
						{
							if(flags & (CSS::ICSSproperty::FLAG_UPDATE_BACKGROUNDLAYOUT | CSS::ICSSproperty::FLAG_UPDATE_BACKGROUNDVIEW))
							{
								UpdateBackground();
							}
							if(flags & CSS::ICSSproperty::FLAG_UPDATE_BORDERLAYOUT)
							{
								UpdateBorder();
							}
						}
						if(flags & CSS::ICSSproperty::FLAG_UPDATE_BORDERVIEW)
						{
							UpdateBorderColor();
						}
					}
				}
				for(UINT i = 0; i < m_pChilds.size(); i++)
				{
					m_pChilds[i]->UpdateStyles();
				}
				for(UINT i = 0; i < m_pChildsOutFlow.size(); i++)
				{
					m_pChildsOutFlow[i]->UpdateStyles();
				}
			}

			IRenderAnonymousBlock::IRenderAnonymousBlock(IRenderFrame * pRootNode):BaseClass(NULL, pRootNode)
			{
			}

			UINT IRenderAnonymousBlock::Layout()
			{
				m_iXpos = m_pParent->GetContentLeft();
				m_iYpos = m_pParent->GetContentTop();

				m_iWidth = m_pParent->GetContentWidth();
				m_iHeight = BaseClass::Layout();
				return(m_iHeight);
			}

			void IRenderAnonymousBlock::Render(UINT lvl)
			{
				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));
				BaseClass::Render(lvl);
				ITranslationManager::PopMatrix();
			}

			IRenderInlineBlock::IRenderInlineBlock(CDOMnode * pNode, IRenderFrame * pRootNode):BaseClass(pNode, pRootNode)
			{
			}

			UINT IRenderInlineBlock::Layout()
			{
				bool bChanged = !HasFixedSize();
				m_bHasFixedSize = true;
				CSS::ICSSstyle * style = m_pNode->GetStyle();
				if(bChanged)
				{
					m_iXpos = m_pParent->GetLeftPos();
					m_iYpos = m_pParent->GetTopPos();
					if(!m_iXpos)
					{
						m_pParent->SetLeftPos(m_iXpos = m_pParent->GetContentLeft());
					}
					if(!m_iYpos)
					{
						m_pParent->SetTopPos(m_iYpos = m_pParent->GetContentTop());
					}
					isWidthSet = false;
					
					if(!style->width->IsSet())
					{
						m_iWidth = m_pParent->GetInnerWidth() - m_iXpos;
						m_bHasFixedSize = false;
					}
					else
					{
						m_iWidth = style->width->GetPX(m_pParent->GetContentWidth());
						isWidthSet = true;
					}
				}
				bool hSet = false;
				if(bChanged)
				{
					UINT _PH = m_pParent->GetContentHeight();
					if(style->height->IsSet())
					{
						m_iHeight = style->height->GetPX(_PH);
						hSet = true;
					}
					else if(style->_gui_text_appearance->IsSet() && style->_gui_text_appearance->GetInt() == CSS::ICSSproperty::_GUI_TEXT_APPEARANCE_ONE_LINE)
					{
						m_iHeight = style->font_size->GetPX(_PH)
							+ style->padding_top->GetPX(_PH)
							+ style->padding_bottom->GetPX(_PH)
							+ style->border_top_width->GetPX(_PH)
							+ style->border_bottom_width->GetPX(_PH);
						hSet = true;
					}
				}
				else
				{
					hSet = true;
				}
				UINT height = BaseClass::Layout();
				
				UINT hNew = height
					+ style->margin_top->GetPX(height)
					+ style->margin_bottom->GetPX(height)
					+ style->padding_top->GetPX(height)
					+ style->padding_bottom->GetPX(height)
					+ style->border_top_width->GetPX(height)
					+ style->border_bottom_width->GetPX(height);
				if(!hSet)
				{
					m_bHasFixedSize = false;
					m_iHeight = hNew;
					hSet = true;
				}
				else
				{
					if(hNew > m_iHeight)
					{
						m_iScrollTopMax = hNew - m_iHeight;
						m_bNeedCut = true;
					}
				}
				if(bChanged)
				{
					if(m_iWidth + m_iXpos > m_pParent->GetContentWidth())
					{
						/*if(m_pParent->GetNode() && m_pParent->GetNode()->GetStyle()->text_align->GetInt() == CSS::ICSSproperty::TEXT_ALIGN_CENTER)
						{
							UINT xDelta = (m_pParent->GetContentWidth() - m_iXpos) / 2;
							for(UINT ii = 0, ll = m_pParent->GetChildCount(); ii < ll; ++ii)
							{
								IRenderFrame * rfcur = m_pParent->GetChild(ii);
								if(rfcur->GetYpos() == m_iYpos)
								{
									rfcur->SetXpos(rfcur->GetXpos() + xDelta);
								}
							}
						}*/
						m_iXpos = m_pParent->GetContentLeft();
						m_pParent->SetTopPos(m_pParent->GetTopPosMax());
						m_iYpos = m_pParent->GetTopPos();
						//request line break;
					}
					else
					{
						m_pParent->SetTopPosMax(max(m_pParent->GetTopPosMax(), m_iYpos + m_iHeight));
						//UINT t = max(m_iYpos + m_iHeight
					}
				}
				m_pParent->SetLeftPos(m_iXpos + m_iWidth);
				m_pParent->SetTopPos(m_iYpos - (m_iXpos == m_pParent->GetContentLeft() ? m_iHeight : 0));
				if(bChanged)
				{
					if(m_pParent->GetHeight() == 0)
					{
						m_pParent->SetHeight(m_pParent->GetTopPosMax());
					}
					UpdateBorder();
					UpdateBackground();
				}
				return(m_iXpos == m_pParent->GetContentLeft() ? m_iHeight : 0);
			}

			RECT IRenderFrame::GetClientRect()
			{
				RECT rc;

				rc.top = (m_pParent ? (m_pParent->GetClientTop()/* + m_pParent->GetInnerTop()*/) : 0) + m_iYpos;
				rc.left = (m_pParent ? (m_pParent->GetClientLeft()/* + m_pParent->GetInnerLeft()*/) : 0) + m_iXpos;
				rc.right = rc.left + m_iWidth;
				rc.bottom = rc.top + m_iHeight;

				return(rc);
			}

			RECT IRenderFrame::GetVisibleRect()
			{
				RECT rc;
				if(m_pNode && m_pNode->GetStyle()->visibility->GetInt() == CSS::ICSSproperty::VISIBILITY_HIDDEN)
				{
					memset(&rc, 0, sizeof(rc));
					return(rc);
				}

				if(!m_pParent)
				{
					return(GetClientRect());
				}

				RECT prc = m_pParent->GetVisibleRect();
				rc = GetClientRect();
				rc.top = max(rc.top, prc.top);
				rc.left = max(rc.left, prc.left);
				rc.bottom = min(rc.bottom, prc.bottom);
				rc.right = min(rc.right, prc.right);

				return(rc);
			}

			void IRenderFrame::FindElementByXY(int x, int y, IDOMnode ** ppNode, bool sendEnterLeave)
			{
				RECT rc = GetVisibleRect();
				bool bHovered = sendEnterLeave && m_pNode && m_pNode->PseudoclassExists(CSS::ICSSrule::PSEUDOCLASS_HOVER);
				bool InMe = false;
				if(InRect(rc, x, y) && m_pNode)
				{
					(*ppNode) = m_pNode;
					if(sendEnterLeave && !bHovered)
					{
						IEvent ev;
						ev.type = GUI_EVENT_TYPE_MOUSEENTER;
						ev.clientX = x;
						ev.clientY = y;
						ev.offsetX = x - rc.left;
						ev.offsetY = y - rc.top;
						ev.target = m_pNode;
						m_pNode->DispatchEvent(ev);
					}
					InMe = true;
				}
				else
				{
					if(sendEnterLeave && bHovered)
					{
						IEvent ev;
						ev.type = GUI_EVENT_TYPE_MOUSELEAVE;
						ev.clientX = x;
						ev.clientY = y;
						ev.offsetX = x - rc.left;
						ev.offsetY = y - rc.top;
						ev.target = m_pNode;
						m_pNode->DispatchEvent(ev);
					}
				}
			//	if(InMe)
				{
					for(UINT i = 0; i < m_pChilds.size(); i++)
					{
						m_pChilds[i]->FindElementByXY(x, y, ppNode, sendEnterLeave);
					}
				}
				for(UINT i = 0; i < m_pChildsOutFlow.size(); i++)
				{
					m_pChildsOutFlow[i]->FindElementByXY(x, y, ppNode, sendEnterLeave);
				}
			}

			bool IRenderFrame::InRect(const RECT & rc, int x, int y)
			{
				return(rc.top <= y && rc.left <= x && rc.bottom > y && rc.right > x);
			}

			void IRenderInlineBlock::Render(UINT lvl)
			{
				/*GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));
				struct point
				{
					float x;
					float y;
					float z;
					DWORD rc;
				};
				//UINT y = GetTopOffset();
				//UINT x = GetLeftOffset();
				point a[6] = {
					{0, 0, 0, rc},
					{m_iWidth, 0, 0, rc},
					{0, (float)(m_iHeight), 0, rc},
					{0, (float)(m_iHeight), 0, rc},
					{m_iWidth, 0, 0, rc},
					{m_iWidth, (float)(m_iHeight), 0, rc}
				};

				GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));

				BaseClass::Render(lvl + 1);
				ITranslationManager::PopMatrix();*/

				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				if(m_iScrollTopMax != 0)
				{
					//m_iScrollTop = Config::TestScrollPos;
					if(m_iScrollTop > m_iScrollTopMax)
					{
						m_iScrollTop = /*Config::TestScrollPos = */m_iScrollTopMax;
					}
					if(m_iScrollTop < 0)
					{
						m_iScrollTop/* = Config::TestScrollPos*/ = 0;
					}
				}
				if(m_iScrollLeftMax != 0)
				{
					//m_iScrollTop = Config::TestScrollPos;
					if(m_iScrollLeft > m_iScrollLeftMax)
					{
						m_iScrollLeft = /*Config::TestScrollPos = */m_iScrollLeftMax;
					}
					if(m_iScrollLeft < 0)
					{
						m_iScrollLeft/* = Config::TestScrollPos*/ = 0;
					}
				}
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));

				if(m_pNode->GetStyle()->visibility->GetInt() != CSS::ICSSproperty::VISIBILITY_HIDDEN)
				{

					struct point
					{
						float x;
						float y;
						float z;
						DWORD rc;
					};

					point a[6] = {
						{0, 0, 0, rc},
						{m_iWidth, 0, 0, rc},
						{0, (float)(m_iHeight), 0, rc},
						{0, (float)(m_iHeight), 0, rc},
						{m_iWidth, 0, 0, rc},
						{m_iWidth, (float)(m_iHeight), 0, rc}
					};



					//	GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));

					if(m_bNeedCut)
					{
						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
						m_border.RenderInnerFill();

						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
						//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl + 1);
						//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
					}

					m_border.Render();
					if(m_bNeedCut)
					{
						//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, TRUE);
						//	GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
					}
					RenderBackground(lvl + (m_bNeedCut ? 1 : 0));
					ITranslationManager::PushMatrix(SMMatrixTranslation(-m_iScrollLeft, -m_iScrollTop, 0.0f));
					BaseClass::Render(lvl + (m_bNeedCut ? 1 : 0));
					ITranslationManager::PopMatrix();
					if(m_bNeedCut)
					{
						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
						GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
						GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
						GetGUI()->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F);
						if(lvl == 0)
						{
							GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
						}
					}
				}

				ITranslationManager::PopMatrix();
			}



			IRenderInline::IRenderInline(CDOMnode * pNode, IRenderFrame * pRootNode):BaseClass(pNode, pRootNode)
			{
			}

			UINT IRenderInline::Layout()
			{
				m_iXpos = m_pParent->GetContentLeft();
				m_iYpos = m_pParent->GetContentTop();

				m_iHeight = BaseClass::Layout();
				UpdateBorder();
				UpdateBackground();
				return(m_iHeight);
			}

			void IRenderInline::Render(UINT lvl)
			{
				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));
				m_border.Render();
				RenderBackground(lvl);
				BaseClass::Render(lvl);
				ITranslationManager::PopMatrix();
			}

			IRenderText::IRenderText(CDOMnode * pNode, IRenderFrame * pRootNode):BaseClass(pNode, pRootNode)
			{
				SetText(((IDOMnodeText*)pNode)->GetText());
			}

			UINT IRenderText::Layout()
			{
				IRenderFrame * pBlock = m_pParent;
				while(pBlock->GetNode() && pBlock->GetNode()->GetStyle()->display->GetInt() == CSS::ICSSproperty::DISPLAY_INLINE)
				{
					pBlock = pBlock->GetParent();
				}
				CSS::ICSSstyle * pStyle = m_pParent->GetNode() ? m_pParent->GetNode()->GetStyle() : m_pParent->GetParent()->GetNode()->GetStyle();
				UINT iTextSize = pStyle->font_size->GetPX(pBlock->GetContentHeight());
				const WCHAR * szFontName = pStyle->font_name->GetString();

				CSS::ICSSproperty::TYPE_FONT_STYLE_e fontStyle = (CSS::ICSSproperty::TYPE_FONT_STYLE_e)pStyle->font_style->GetInt();
				CSS::ICSSproperty::TYPE_FONT_WEIGHT_e fontWeight = (CSS::ICSSproperty::TYPE_FONT_WEIGHT_e)pStyle->font_weight->GetInt();
				CSS::ICSSproperty::TYPE_TEXT_ALIGN_e _textAlign = (CSS::ICSSproperty::TYPE_TEXT_ALIGN_e)pStyle->text_align->GetInt();
				CSS::ICSSproperty::TYPE_TEXT_DECORATION_LINE_e _textDecoration = (CSS::ICSSproperty::TYPE_TEXT_DECORATION_LINE_e)pStyle->text_decoration_line->GetInt();


				m_iXpos = m_pParent->GetContentLeft();
				m_iYpos = m_pParent->GetContentTop();

				IFont::TEXT_ALIGN textAlign;
				switch(_textAlign)
				{
				case CSS::ICSSproperty::TEXT_ALIGN_CENTER:
					textAlign = IFont::TEXT_ALIGN_CENTER;
					break;
				case CSS::ICSSproperty::TEXT_ALIGN_RIGHT:
					textAlign = IFont::TEXT_ALIGN_RIGHT;
					break;
				default:
					textAlign = IFont::TEXT_ALIGN_LEFT;
				}

				UINT textDecoration = IFont::DECORATION_NONE;
				switch(_textDecoration)
				{
				case CSS::ICSSproperty::TEXT_DECORATION_LINE_LINE_THROUGH:
					textDecoration = IFont::DECORATION_LINE_THROUGH;
					break;
				case CSS::ICSSproperty::TEXT_DECORATION_LINE_OVERLINE:
					textDecoration = IFont::DECORATION_OVERLINE;
					break;
				case CSS::ICSSproperty::TEXT_DECORATION_LINE_UNDERLINE:
					textDecoration = IFont::DECORATION_UNDERLINE;
					break;
				}

				UINT m_iAreaWidth = pBlock->GetContentWidth();

				IFont::STYLE style = (IFont::STYLE)((fontStyle == CSS::ICSSproperty::FONT_STYLE_ITALIC ? IFont::STYLE_ITALIC : 0) | (fontWeight == CSS::ICSSproperty::FONT_WEIGHT_BOLD ? IFont::STYLE_BOLD : 0));

				pFont = IFontManager::GetFont(szFontName, iTextSize, style);

				struct word
				{
					UINT start_pos;
					UINT length;
				};

				word * pWords = new word[m_iWordCount];
				UINT _len = 0;
				UINT _pos = 0;
				UINT _idx = 0;
				for(UINT i = 0, l = m_szClearText.length(); i < l; i++)
				{
					if(iswspace(m_szClearText[i]))
					{
						pWords[_idx].start_pos = _pos;
						pWords[_idx].length = _len;
						//pWords[_idx] = {_pos, _len};
						_idx++;
						_len = 1;
						_pos = i;
					}
					else
					{
						_len++;
					}
				}
				pWords[_idx].start_pos = _pos;
				pWords[_idx].length = _len;
				//pWords[_idx] = {_pos, _len};

				StringW tmp;
				UINT _w
					,_h;
				UINT _cw = pBlock->GetLeftPos();
				UINT fo = _cw;
				UINT breaks = 0;
				for(UINT i = 0; i < m_iWordCount; i++)
				{
					tmp = m_szClearText.substr(pWords[i].start_pos, pWords[i].length);
					pFont->GetStringMetrics(tmp, &_w, &_h, NULL, NULL, NULL);
					if(_cw + _w >= m_iAreaWidth && i > 0)
					{
						m_szClearText[pWords[i].start_pos] = L'\n';
						_cw = 0;
						breaks++;
					}
					_cw += _w;
				}
				
				pBlock->SetLeftPos(_cw);
				pFont->BuildString(m_szClearText, textDecoration, textAlign, &m_pVertexBuffer, &m_pIndexBuffer, &m_iVertexCount, &m_iIndexBaseCount, &m_iIndexAddCount, m_iAreaWidth, fo);
				//m_pParent->SetTopPos(m_iYpos - iTextSize);
				SX_SAFE_DELETE_A(pWords);
				return(iTextSize * (breaks));
			}

			void IRenderText::Render(UINT lvl)
			{
				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				static CPITexture texWhite = NULL;
				if(!texWhite)
				{
					texWhite = ITextureManager::GetTexture(TEX_WHITE);
				}
				GetGUI()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));
				//SMMATRIX m = SMMatrixTranslation(m_pParent->GetLeftOffset(), m_pParent->GetTopOffset(), 0.0f);
				//GetGUI()->GetDevice()->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&m));
				ITextureManager::BindTexture(pFont->GetTexture(0));
				GetGUI()->GetDevice()->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(IFont::vertex));
				GetGUI()->GetDevice()->SetIndices(m_pIndexBuffer);
				GetGUI()->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iIndexBaseCount / 3);

				ITextureManager::BindTexture(texWhite);

				if(m_iIndexAddCount)
				{
					GetGUI()->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, m_iIndexBaseCount, m_iIndexAddCount / 3);
				}
				ITranslationManager::PopMatrix();
				GetGUI()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
			}

			void IRenderText::SetText(const StringW & text)
			{
				m_szClearText = L"";
				m_iWordCount = 1;
				bool issp = true;
				for(UINT i = 0; i < text.length(); i++)
				{
					if(iswspace(text[i]))
					{
						if(!issp)
						{
							m_szClearText += L' ';
							m_iWordCount++;
						}
						issp = true;
					}
					else
					{
						issp = false;
						m_szClearText += text[i];
					}
				}
			}

			IRenderTextNew::IRenderTextNew(CDOMnode * pNode, IRenderFrame * pRootNode):BaseClass(pNode, pRootNode), m_iCaretPos(0), m_bInSelection(false), m_iSelectionStart(0), m_iSelectionEnd(0)
			{
				//SetText(((IDOMnodeText*)pNode)->GetText());
			}

			void IRenderTextNew::SetText(const StringW & text)
			{
				if(m_pParent && m_pParent->GetNode() && m_pParent->GetNode()->GetStyle()->_gui_text_format->GetInt() == CSS::ICSSproperty::_GUI_TEXT_FORMAT_PREFORMATTED)
				{
					m_szClearText = L"";
					m_iWordCount = 1;
					bool issp = true;
					for(UINT i = 0; i < text.length(); i++)
					{
						if(iswspace(text[i]))
						{
							if(!issp)
							{
								m_iWordCount++;
							}
							issp = true;
						}
						else
						{
							issp = false;
						}
					}
					m_szClearText = text;
				}
				else
				{
					m_szClearText = L"";
					m_iWordCount = 1;
					bool issp = true;
					for(UINT i = 0; i < text.length(); i++)
					{
						if(iswspace(text[i]))
						{
							if(!issp)
							{
								m_szClearText += L' ';
								m_iWordCount++;
							}
							issp = true;
						}
						else
						{
							issp = false;
							m_szClearText += text[i];
						}
					}
				}
			}
			void IRenderTextNew::Render(UINT lvl)
			{
				GetGUI()->GetDevice()->SetRenderState(D3DRS_STENCILREF, lvl);
				static CPITexture texWhite = NULL;
				static CSHADER shText = NULL;
				static CSHADER shTextBlur = NULL;
				if(!texWhite)
				{
					texWhite = ITextureManager::GetTexture(TEX_WHITE);
				}
				if(!shText)
				{
					shText = ITextureManager::LoadShader(L"text");
				}
				if(!shTextBlur)
				{
					shTextBlur = ITextureManager::LoadShader(L"gauss");
				}
				GetGUI()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
				ITranslationManager::PushMatrix(SMMatrixTranslation(m_iXpos, m_iYpos, 0.0f));
				//SMMATRIX m = SMMatrixTranslation(m_pParent->GetLeftOffset(), m_pParent->GetTopOffset(), 0.0f);
				//GetGUI()->GetDevice()->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&m));
				
				ITextureManager::BindShader(shText);
				ITextureManager::BindTexture(texWhite);
				RenderSelection();
				GetGUI()->GetDevice()->SetPixelShaderConstantF(0, (float*)&m_pStyle->color->GetColor(), 1);
				for(UINT i = 0; i < m_pRenderElems.size(); i++)
				{
					renderElement * el = &m_pRenderElems[i];
					ITranslationManager::PushMatrix(SMMatrixTranslation(0.0f, el->m_iTopOffset, 0.0f));
					ITranslationManager::PushMatrix(SMMatrixTranslation(el->m_iLeftOffset, 0.0f, 0.0f));
					ITextureManager::BindTexture(pFont->GetTexture(0));
					GetGUI()->GetDevice()->SetStreamSource(0, el->m_pVertexBuffer, 0, sizeof(IFont::vertex));
					GetGUI()->GetDevice()->SetIndices(el->m_pIndexBuffer);
					GetGUI()->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, el->m_iVertexCount, 0, el->m_iIndexBaseCount / 3);
					if(el->m_iIndexAddCount)
					{
						ITextureManager::BindTexture(texWhite);
						GetGUI()->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, el->m_iVertexCount, el->m_iIndexBaseCount, el->m_iIndexAddCount / 3);
					}
					ITranslationManager::PopMatrix();

				}
				for(UINT i = 0; i < m_pRenderElems.size(); i++)
				{
					ITranslationManager::PopMatrix();
				}
				
				ITextureManager::BindTexture(texWhite);
				DrawCaret();
				ITextureManager::UnbindShader();
				ITranslationManager::PopMatrix();
				GetGUI()->GetDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
			}

			void IRenderTextNew::SelectionStart()
			{
				m_bInSelection = true;
			}
			void IRenderTextNew::SelectionEnd()
			{
				m_bInSelection = false;
			}

			UINT IRenderTextNew::Layout()
			{
				SetText(((IDOMnodeText*)m_pNode)->GetText());
				((IDOMnodeText*)m_pNode)->SetText(m_szClearText);
				IRenderFrame * pBlock = m_pParent;
				while(pBlock->GetNode() && pBlock->GetNode()->GetStyle()->display->GetInt() == CSS::ICSSproperty::DISPLAY_INLINE)
				{
					pBlock = pBlock->GetParent();
				}
				CSS::ICSSstyle * pStyle = m_pStyle = m_pParent->GetNode() ? m_pParent->GetNode()->GetStyle() : m_pParent->GetParent()->GetNode()->GetStyle();
				UINT iTextSize = pStyle->font_size->GetPX(pBlock->GetContentHeight());
				const WCHAR * szFontName = pStyle->font_name->GetString();

				bool isOneLine = (pStyle->_gui_text_appearance->IsSet() && pStyle->_gui_text_appearance->GetInt() == CSS::ICSSproperty::_GUI_TEXT_APPEARANCE_ONE_LINE);

				CSS::ICSSproperty::TYPE_FONT_STYLE_e fontStyle = (CSS::ICSSproperty::TYPE_FONT_STYLE_e)pStyle->font_style->GetInt();
				CSS::ICSSproperty::TYPE_FONT_WEIGHT_e fontWeight = (CSS::ICSSproperty::TYPE_FONT_WEIGHT_e)pStyle->font_weight->GetInt();
				CSS::ICSSproperty::TYPE_TEXT_ALIGN_e _textAlign = (CSS::ICSSproperty::TYPE_TEXT_ALIGN_e)pStyle->text_align->GetInt();
				CSS::ICSSproperty::TYPE_TEXT_DECORATION_LINE_e _textDecoration = (CSS::ICSSproperty::TYPE_TEXT_DECORATION_LINE_e)pStyle->text_decoration_line->GetInt();

				m_iXpos = m_pParent->GetContentLeft();
				m_iYpos = m_pParent->GetContentTop();

				//m_pColor = pStyle->p_color.GetColor();

				IFont::TEXT_ALIGN textAlign;
				switch(_textAlign)
				{
				case CSS::ICSSproperty::TEXT_ALIGN_CENTER:
					textAlign = IFont::TEXT_ALIGN_CENTER;
					break;
				case CSS::ICSSproperty::TEXT_ALIGN_RIGHT:
					textAlign = IFont::TEXT_ALIGN_RIGHT;
					break;
				default:
					textAlign = IFont::TEXT_ALIGN_LEFT;
				}

				UINT textDecoration = IFont::DECORATION_NONE;
				switch(_textDecoration)
				{
				case CSS::ICSSproperty::TEXT_DECORATION_LINE_LINE_THROUGH:
					textDecoration = IFont::DECORATION_LINE_THROUGH;
					break;
				case CSS::ICSSproperty::TEXT_DECORATION_LINE_OVERLINE:
					textDecoration = IFont::DECORATION_OVERLINE;
					break;
				case CSS::ICSSproperty::TEXT_DECORATION_LINE_UNDERLINE:
					textDecoration = IFont::DECORATION_UNDERLINE;
					break;
				}

				UINT m_iAreaWidth = pBlock->GetContentWidth();

				IFont::STYLE style = (IFont::STYLE)((fontStyle == CSS::ICSSproperty::FONT_STYLE_ITALIC ? IFont::STYLE_ITALIC : 0) | (fontWeight == CSS::ICSSproperty::FONT_WEIGHT_BOLD ? IFont::STYLE_BOLD : 0));

				pFont = IFontManager::GetFont(szFontName, iTextSize, style);

				struct word
				{
					UINT start_pos;
					UINT length;
				};

				word * pWords = new word[m_iWordCount];
				UINT _len = 0;
				UINT _pos = 0;
				UINT _idx = 0;
				// FIXME: move that into SetText method
				for(UINT i = 0, l = m_szClearText.length(); i < l; i++)
				{
					if(iswspace(m_szClearText[i]))
					{
						pWords[_idx].start_pos = _pos;
						pWords[_idx].length = _len;
						//pWords[_idx] = {_pos, _len};
						_idx++;
						_len = 1;
						_pos = i;
					}
					else
					{
						_len++;
					}
				}
				pWords[_idx].start_pos = _pos;
				pWords[_idx].length = _len;
				//pWords[_idx] = {_pos, _len};

				StringW tmp;
				UINT _w, _h;
				UINT _cw = pBlock->GetLeftPos();
				UINT fo = _cw;
				UINT breaks = 0;
				renderElement rel;
				UINT sh = 0;
				pBlock->TextStart();
				m_pRenderElems.clear();
				IFont::char_rects cr;
				m_vCharRects.clear();
				cr.pResArray = &m_vCharRects;
				UINT icsh = 0;
				bool isFirstWord = false;
				UINT crc;
				UINT fsym = 0;
				for(UINT i = 0; i < m_iWordCount; i++)
				{
					tmp = m_szClearText.substr(pWords[i].start_pos, pWords[i].length);
					cr.iXoffs = _cw - icsh;
			//		icsh = 0;
					isFirstWord = false;
					cr.iYoffs = sh;
					crc = m_vCharRects.size();
					pFont->GetStringMetrics(tmp, &_w, &_h, NULL, NULL, NULL, &cr);
					
					if(_cw + _w >= m_iAreaWidth && (i > 0 || _cw != 0) && !isOneLine)
					{
						//m_szClearText[pWords[i].start_pos] = L'\n'
						UINT isw = 0;
						if(rel.m_szStr.length())
						{
							pFont->BuildString(rel.m_szStr, textDecoration, IFont::TEXT_ALIGN_LEFT, &rel.m_pVertexBuffer, &rel.m_pIndexBuffer, &rel.m_iVertexCount, &rel.m_iIndexBaseCount, &rel.m_iIndexAddCount, m_iAreaWidth, 0, &isw);

							rel.m_iLeftOffset = breaks == 0 ? fo : 0;
							rel.m_iTopOffset = breaks > 0 ? iTextSize : 0;
							rel.m_iHeight = iTextSize;
							rel.m_iWIdth = isw;
							rel.m_prc = &m_vCharRects;
							rel.m_iFirstSym = fsym;
							rel.m_iLastSym = crc;
							rel.m_iLineIdx = pBlock->TextGetLineIdx();

							m_pRenderElems.push_back(rel);
						}
						sh += _h;

						bool applycsh = icsh == 0;

						icsh = m_vCharRects[crc].right - m_vCharRects[crc].left;
						
						for(UINT o = crc; o < m_vCharRects.size(); o++)
						{
							m_vCharRects[o].top += _h;
							m_vCharRects[o].bottom += _h;
							m_vCharRects[o].right -= _cw + (applycsh ? icsh : 0);
							m_vCharRects[o].left -= _cw + (applycsh ? icsh : 0);
						}
						m_vCharRects[crc].bottom -= m_vCharRects[crc].top;
						m_vCharRects[crc].top = m_vCharRects[crc - 1].top;
						m_vCharRects[crc].bottom += m_vCharRects[crc].top;

						m_vCharRects[crc].right -= m_vCharRects[crc].left;
						m_vCharRects[crc].left = m_vCharRects[crc - 1].right;
						m_vCharRects[crc].right += m_vCharRects[crc].left;

						pBlock->TextBreak();
						_cw = 0;
						rel.m_szStr = tmp.substr(i > 0 ? 1 : 0);
						isFirstWord = true;
						fsym = crc + 1;
						breaks++;
					}
					else
					{
						rel.m_szStr += tmp;
					}
					_cw += _w;
				}
				UINT isw = 0;
				if(rel.m_szStr.length())
				{
					pFont->BuildString(rel.m_szStr, textDecoration, IFont::TEXT_ALIGN_LEFT, &rel.m_pVertexBuffer, &rel.m_pIndexBuffer, &rel.m_iVertexCount, &rel.m_iIndexBaseCount, &rel.m_iIndexAddCount, m_iAreaWidth, 0, &isw);
					rel.m_iLeftOffset = breaks == 0 ? fo : 0;
					rel.m_iTopOffset = breaks > 0 ? iTextSize : 0;
					rel.m_iHeight = iTextSize;
					rel.m_iWIdth = isw;
					rel.m_prc = &m_vCharRects;
					rel.m_iFirstSym = fsym;
					rel.m_iLastSym = m_vCharRects.size() - 1;
					rel.m_iLineIdx = pBlock->TextGetLineIdx();
					m_pRenderElems.push_back(rel);
							
				}

				for(UINT i = 0; i < m_pRenderElems.size(); i++)
				{
					pBlock->TextAppend(&m_pRenderElems[i], m_pRenderElems[i].m_iLineIdx);
				}

				pBlock->SetLeftPos(_cw);

	//@@		/	m_pParent->SetHeight(sh);
			//	pFont->BuildString(m_szClearText, textDecoration, textAlign, &m_pVertexBuffer, &m_pIndexBuffer, &m_iVertexCount, &m_iIndexBaseCount, &m_iIndexAddCount, m_iAreaWidth, fo);
				//m_pParent->SetTopPos(m_iYpos - iTextSize);
				SX_SAFE_DELETE_A(pWords);
				return(iTextSize * (breaks));
			}

			UINT IRenderTextNew::GetCaretPos()
			{
				return(m_iCaretPos);
			}

			void IRenderTextNew::SetSelectionStart(int cp)
			{
				int maxPos = m_vCharRects.size();
				if(cp > maxPos)
				{
					cp = maxPos;
				}
				if(cp < 0)
				{
					cp = 0;
				}
				m_iSelectionStart = cp;
			}
			void IRenderTextNew::SetSelectionEnd(int cp)
			{
				int maxPos = m_vCharRects.size();
				if(cp > maxPos)
				{
					cp = maxPos;
				}
				if(cp < 0)
				{
					cp = 0;
				}
				m_iSelectionEnd = cp;
			}

			void IRenderTextNew::SetCaretPos(int cp, bool force)
			{
				int maxPos = m_vCharRects.size();
				if(cp > maxPos && !force)
				{
					cp = maxPos;
				}
				if(cp < 0)
				{
					cp = 0;
				}
				if(m_bInSelection)
				{
					m_iSelectionEnd = cp;
				}
				else
				{
					m_iSelectionStart = m_iSelectionEnd = cp;
				}
				m_iCaretPos = cp;
			}

			UINT IRenderTextNew::GetCaretMaxPos()
			{
				return(m_vCharRects.size());
			}

			void IRenderTextNew::MoveCaretPos(int shift)
			{
				SetCaretPos((int)m_iCaretPos + shift);
			}

			void IRenderTextNew::DrawCaret()
			{
				if(!m_pNode || !m_pNode->ParentNode())
				{
					return;
				}
				if(m_pNode->ParentNode()->GetStyle()->_gui_text_cursor->GetInt() == CSS::ICSSproperty::_GUI_TEXT_CURSOR_SHOW)
				{
					int iTextSize = m_pNode->ParentNode()->GetStyle()->font_size->GetPX(m_pParent->GetInnerHeight());
					m_iTextSize = iTextSize;
					float4_t color = m_pNode->ParentNode()->GetStyle()->color->GetColor();
					struct point
					{
						float x;
						float y;
						float z;
						DWORD rc;
					};

					DWORD rc = 0xFF00FF00;

					point a[6] = {
						{0, 0, 0, rc},
						{1.0f, 0, 0, rc},
						{0, (float)(iTextSize), 0, rc},
						{0, (float)(iTextSize), 0, rc},
						{1.0f, 0, 0, rc},
						{1.0f, (float)(iTextSize), 0, rc}
					};

					/*m_iCaretPos = 40;*/
					bool bLastPos = m_iCaretPos >= m_vCharRects.size();
					UINT iUseIndex = m_iCaretPos;
					if(bLastPos)
					{
						iUseIndex = m_vCharRects.size() - 1;
					}

					int _x = 0;
					int _y = 0;
					if(m_vCharRects.size() == 0)
					{
						iUseIndex = 0;
					}
					else
					{
						if(bLastPos)
						{
							_x = m_vCharRects[iUseIndex].right;
						}
						else
						{
							_x = m_vCharRects[iUseIndex].left;
						}
						_y = m_vCharRects[iUseIndex].top;
					}

					//float op = sinf((float)GetTickCount() * 0.007f);
					float op = sinf((float)GetTickCount() * 0.003f);
					op *= op;
					color.w = op;
					GetGUI()->GetDevice()->SetPixelShaderConstantF(0, (float*)&color, 1);
					ITranslationManager::PushMatrix(SMMatrixTranslation(_x - 1.0f, _y, 0.0f));
					GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
					ITranslationManager::PopMatrix();

					int _w = m_pParent->GetInnerWidth();
					int _scrollX = m_pParent->GetScrollLeft();
					if(_x < _scrollX)
					{
						_scrollX = _x - 5;
					}
					if(_x > _w + _scrollX)
					{
						_scrollX = _x - _w + 10;
					}
					m_pParent->SetScrollLeft(_scrollX);

					int _h = m_pParent->GetInnerHeight();
					int _scrollY = m_pParent->GetScrollTop();
					if(_y < _scrollY)
					{
						_scrollY = _y - 2;
					}
					if(_y + iTextSize > _h + _scrollY)
					{
						_scrollY = _y + iTextSize - _h + 7;
					}
					m_pParent->SetScrollTop(_scrollY);

					m_iCaretX = _x;
					m_iCaretY = _y;
					

					/*for(UINT i = 0; i < m_vCharRects.size(); i++)
					{
						int _x = m_vCharRects[i].left;
						int _y = m_vCharRects[i].top;
						ITranslationManager::PushMatrix(SMMatrixTranslation(_x - 1.0f, _y, 0.0f));
						GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
						ITranslationManager::PopMatrix();
					}
					_x = m_vCharRects[m_vCharRects.size()-1].right;
					_y = m_vCharRects[m_vCharRects.size() - 1].top;
					ITranslationManager::PushMatrix(SMMatrixTranslation(_x - 1.0f, _y, 0.0f));
					GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
					ITranslationManager::PopMatrix();*/
				}
			}

			void IRenderTextNew::PlaceCaretByXY(int x, int y)
			{
				if(y < 0)
				{
					SetCaretPos(0);
					return;
				}
				for(UINT i = 0; i < m_vCharRects.size(); i++)
				{
					if(m_vCharRects[i].bottom > y && m_vCharRects[i].top <= y)
					{
						if(x < m_vCharRects[i].left)
						{
							SetCaretPos(i);
							return;
						}
						if(x > m_vCharRects[i].right && i < m_vCharRects.size() - 1 && m_vCharRects[i + 1].top != m_vCharRects[i].top)
						{
							SetCaretPos(i);
							return;
						}
						if(x >= m_vCharRects[i].left && x <= m_vCharRects[i].right)
						{
							int _w = m_vCharRects[i].right - m_vCharRects[i].left;
							int dw = x - m_vCharRects[i].left;
							if(dw < _w / 2)
							{
								SetCaretPos(i);
							}
							else
							{
								SetCaretPos(i + 1);
							}
							return;
						}
					}
				}
				SetCaretPos(m_vCharRects.size());
			}

			UINT IRenderTextNew::GetSelectionStart()
			{
				return(min(m_iSelectionEnd, m_iSelectionStart));
			}
			UINT IRenderTextNew::GetSelectionEnd()
			{
				return(max(m_iSelectionEnd, m_iSelectionStart));
			}

			StringW IRenderTextNew::GetSelectionText()
			{
				return(m_szClearText.substr(GetSelectionStart(), GetSelectionEnd() - GetSelectionStart()));
			}

			void IRenderTextNew::MoveCaretLine(int shift)
			{
				PlaceCaretByXY(m_iCaretX, m_iCaretY + m_iTextSize * shift);
			}

			void IRenderTextNew::RenderSelection()
			{
			//	m_iSelectionStart = 0;
			//	m_iSelectionEnd = 2;
				if(m_iSelectionEnd == m_iSelectionStart)
				{
					return;
				}
				if(!m_pNode || !m_pNode->ParentNode())
				{
					return;
				}
				if(m_pNode->ParentNode()->GetStyle()->_gui_text_cursor->GetInt() == CSS::ICSSproperty::_GUI_TEXT_CURSOR_SHOW)
				{
					UINT iTextSize = m_pNode->ParentNode()->GetStyle()->font_size->GetPX(m_pParent->GetInnerHeight());
					m_iTextSize = iTextSize;
					float4_t color = float4_t(0.0f, 0.0f, 1.0f, 0.5f);
					struct point
					{
						float x;
						float y;
						float z;
						DWORD rc;
					};

					DWORD rc = 0xFF00FF00;

					point a[6] = {
						{0, 0, 0, rc},
						{1.0f, 0, 0, rc},
						{0, (float)(iTextSize), 0, rc},
						{0, (float)(iTextSize), 0, rc},
						{1.0f, 0, 0, rc},
						{1.0f, (float)(iTextSize), 0, rc}
					};

					UINT selStart = min(m_iSelectionEnd, m_iSelectionStart);
					UINT selEnd = max(m_iSelectionEnd, m_iSelectionStart);

					if(selEnd > m_vCharRects.size())
					{
						selEnd = m_vCharRects.size();
					}

					GetGUI()->GetDevice()->SetPixelShaderConstantF(0, (float*)&color, 1);

					for(UINT i = selStart; i < selEnd; i++)
					{
						a[0].x = m_vCharRects[i].left;
						a[1].x = m_vCharRects[i].right;
						a[2].x = a[0].x;
						a[3].x = a[0].x;
						a[4].x = a[1].x;
						a[5].x = a[1].x;

						a[0].y = m_vCharRects[i].top;
						a[1].y = a[0].y;
						a[2].y = m_vCharRects[i].bottom;
						a[3].y = a[2].y;
						a[4].y = a[0].y;
						a[5].y = a[2].y;

						GetGUI()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &a, sizeof(point));
					}
				}
			}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

			IRenderImageBlock::IRenderImageBlock(CDOMnode * pNode, IRenderFrame * pRootFrame):BaseClass(pNode, pRootFrame)
			{
			}

			IRenderImageInlineBlock::IRenderImageInlineBlock(CDOMnode * pNode, IRenderFrame * pRootFrame) : BaseClass(pNode, pRootFrame)
			{
				
			}

			UINT IRenderImageBlock::Layout()
			{
				static UINT nIMG = CDOMnode::GetNodeIdByName(L"img");
				static UINT nVIDEO = CDOMnode::GetNodeIdByName(L"video");

				CSS::ICSSstyle * style = m_pNode->GetStyle();

				if(m_pNode->GetNodeId() == nVIDEO)
				{
					StringW src = ((IVIDEO*)m_pNode)->InitRenderer();
					style->background_image->Set(src);
				}
				else
				{
					style->background_image->Set(m_pNode->GetAttribute(L"src"));
				}

				
				
				style->background_attachment->Set(CSS::ICSSproperty::BACKGROUND_ATTACHMENT_LOCAL);
				style->background_position_x->Set(0);
				style->background_position_y->Set(0);
				CPITexture tex = ITextureManager::GetTexture(style->background_image->GetString());

				int _w = tex->GetWidth();
				int _h = tex->GetHeight();

				StringW a = m_pNode->GetAttribute(L"width");
				if(a.length())
				{
					_w = a.ToInt();
				}
				a = m_pNode->GetAttribute(L"height");
				if(a.length())
				{
					_h = a.ToInt();
				}

				if(style->width->IsSet())
				{
					_w = style->width->GetPX(m_pParent->GetWidth());
				}


				if(style->height->IsSet())
				{
					_h = style->height->GetPX(m_pParent->GetHeight());
				}

				bool _ch = !((CSS::CCSSproperty*)style->height)->IsChanged();
				bool _cw = !((CSS::CCSSproperty*)style->width)->IsChanged();

				style->width->Set((float)(m_iWidth = _w));
				style->height->Set((float)(m_iHeight = _h));
				style->width->SetDim(CSS::ICSSproperty::DIM_PX);
				style->height->SetDim(CSS::ICSSproperty::DIM_PX);

	//			if(_ch)
				{
					((CSS::CCSSproperty*)style->height)->UnChange();
				}
	//			if(_cw)
				{
					((CSS::CCSSproperty*)style->width)->UnChange();
				}

				style->background_size_x->Set(100.0f);
				style->background_size_x->SetDim(CSS::ICSSproperty::DIM_PC);
				style->background_size_y->Set(100.0f);
				style->background_size_y->SetDim(CSS::ICSSproperty::DIM_PC);
				return(BaseClass::Layout());
			}

			void IRenderImageBlock::Render(UINT lvl)
			{
				BaseClass::Render(lvl);
			}



			UINT IRenderImageInlineBlock::Layout()
			{
				static UINT nIMG = CDOMnode::GetNodeIdByName(L"img");
				static UINT nVIDEO = CDOMnode::GetNodeIdByName(L"video");

				CSS::ICSSstyle * style = m_pNode->GetStyle();

				if(m_pNode->GetNodeId() == nVIDEO)
				{
					StringW src = ((IVIDEO*)m_pNode)->InitRenderer();
					style->background_image->Set(src);
				}
				else
				{
					style->background_image->Set(m_pNode->GetAttribute(L"src"));
				}

				style->background_attachment->Set(CSS::ICSSproperty::BACKGROUND_ATTACHMENT_LOCAL);
				style->background_position_x->Set(0);
				style->background_position_y->Set(0);
				CPITexture tex = ITextureManager::GetTexture(style->background_image->GetString());

				int _w = tex->GetWidth();
				int _h = tex->GetHeight();

				StringW a = m_pNode->GetAttribute(L"width");
				if(a.length())
				{
					_w = a.ToInt();
				}
				a = m_pNode->GetAttribute(L"height");
				if(a.length())
				{
					_h = a.ToInt();
				}

				if(style->width->IsSet())
				{
					_w = style->width->GetPX(m_pParent->GetWidth());
				}


				if(style->height->IsSet())
				{
					_h = style->height->GetPX(m_pParent->GetHeight());
				}

				bool _ch = !((CSS::CCSSproperty*)style->height)->IsChanged();
				bool _cw = !((CSS::CCSSproperty*)style->width)->IsChanged();

				style->width->Set((float)(m_iWidth = _w));
				style->height->Set((float)(m_iHeight = _h));
				style->width->SetDim(CSS::ICSSproperty::DIM_PX);
				style->height->SetDim(CSS::ICSSproperty::DIM_PX);

		//		if(_ch)
				{
					((CSS::CCSSproperty*)style->height)->UnChange();
				}
		//		if(_cw)
				{
					((CSS::CCSSproperty*)style->width)->UnChange();
				}


				style->background_size_x->Set(100.0f);
				style->background_size_x->SetDim(CSS::ICSSproperty::DIM_PC);
				style->background_size_y->Set(100.0f);
				style->background_size_y->SetDim(CSS::ICSSproperty::DIM_PC);
				return(BaseClass::Layout());
			}

			void IRenderImageInlineBlock::Render(UINT lvl)
			{
				BaseClass::Render(lvl);
			}
		};
	};
};