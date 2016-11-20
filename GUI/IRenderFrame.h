#ifndef _IRenderFrame_H_
#define _IRenderFrame_H_

#include "GUIbase.h"
#include "CDOM.h"
#include "ICSS.h"
#include "IFont.h"

#include "IRenderBorder.h"

namespace GUI
{
	namespace DOM
	{
		//class IFont;
		namespace Render
		{
			class ITranslationManager
			{
			public:
				static void PushMatrix(const SMMATRIX & m);
				static void PopMatrix();
				static SMMATRIX GetCurrentMatrix();
			protected:
				static SMMATRIX m_result;
				static Stack<SMMATRIX> m_stack;
			};

			class IScrollBar;

			class IRenderFrame
			{
				friend class CDOMnode;
			public:
				IRenderFrame(CDOMnode * pNode, IRenderFrame * pRootNode);
				~IRenderFrame();

				virtual UINT Layout();
				virtual void Render(UINT lvl);

				struct renderElement
				{
					StringW m_szStr;
					IDirect3DVertexBuffer9 * m_pVertexBuffer;
					IDirect3DIndexBuffer9 * m_pIndexBuffer;
					UINT m_iVertexCount;
					UINT m_iIndexBaseCount;
					UINT m_iIndexAddCount;
					UINT m_iTopOffset;
					UINT m_iLeftOffset;
					UINT m_iHeight;
					UINT m_iWIdth;

					int m_iLineIdx;

					UINT m_iFirstSym;
					UINT m_iLastSym;
					Array<RECT> * m_prc;
				};

				/*enum CUT_TYPE
				{
				CUT_NONE,
				CUT_BORDER,
				CUT_CONTENT
				};*/

				IRenderFrame * GetParent();
				IRenderFrame * GetNextSibling();
				IRenderFrame * GetPrevSibling();

				IRenderFrame * m_pRootNode;

				UINT GetChildCount();
				IRenderFrame * GetChild(UINT id);
				CDOMdocument * m_pDoc;

				CSS::ICSSstyle * GetComputedStyle();

				bool m_bIsOutOfFlow;

				UINT GetWidth();
				UINT GetContentWidth();
				UINT GetContentHeight();
				UINT GetHeight();
				int GetXpos();
				void SetXpos(int x);
				int GetYpos();

				UINT GetInnerWidth();
				UINT GetInnerHeight();

				void AddChild(IRenderFrame * pChild, bool bOutFlow = false);
				void SetParent(IRenderFrame * pParent);

				static IRenderFrame * CreateNode(CDOMnode * pNode, IRenderFrame * pRootNode);

				void DebugPrint(UINT lvl = 0);

				void SetWidth(UINT width);
				void SetHeight(UINT width);

				UINT GetTopPos();
				UINT GetLeftPos();
				void SetLeftPos(UINT p);
				void SetTopPos(UINT p);
				UINT GetTopPosMax();
				void SetTopPosMax(UINT p);

				UINT GetTopOffset();
				UINT GetLeftOffset();

				UINT GetClientTop();
				UINT GetClientLeft();

				CDOMnode * GetNode();

				UINT GetContentTop();
				UINT GetContentLeft();

				UINT GetInnerTop();
				UINT GetInnerLeft();

				void TextStart();
				void TextClear();
				void TextBreak();
				void TextAppend(renderElement * rel, int iLineIdx);
				int TextGetLineIdx();
				UINT TextFixUp();

				void UpdateStyles();

				void UpdateBorderColor();

				void ResetLayout(bool first = true);

				RECT GetClientRect();
				RECT GetVisibleRect();

				int GetScrollLeft();
				int GetScrollLeftMax();
				void SetScrollLeft(int x);

				int GetScrollTop();
				int GetScrollTopMax();
				void SetScrollTop(int x, bool _check_bounds = false);

				void SetScrollSpeedX(int x);
				void SetScrollSpeedY(int x);
				void UpdateScroll();

				void FindElementByXY(int x, int y, IDOMnode ** ppNode, bool sendEnterLeave = false);

				bool InRect(const RECT & rc, int x, int y);

				bool HasFixedSize()
				{
					return(m_bHasFixedSize || IsFreezzed());
				}

				bool IsOutOfFlow()
				{
					return(m_bIsOutOfFlow);
				}

				void IsOutOfFlow(bool is)
				{
					m_bIsOutOfFlow = is;
				}

				bool IsChildOf(IRenderFrame * pNode)
				{
					IRenderFrame * pCur = GetParent();
					while(pCur)
					{
						if(pCur == pNode)
						{
							return(true);
						}
						pCur = pCur->GetParent();
					}
					return(false);
				}

				void SetDocument(CDOMdocument * doc)
				{
					m_pDoc = doc;
				}

				bool IsLastChild()
				{
					if(!m_pParent)
					{
						return(true);
					}
					return(m_pParent->GetChild(m_pParent->GetChildCount() - 1) == this);
				}

				void Freeze(bool dof = true)
				{
					m_bFreezed = dof;
				}

				bool IsFreezzed()
				{
					return(m_bFreezed);
				}

				void RemoveChild(IRenderFrame * prf);

			protected:

				struct pointtex
				{
					float3_t Pos;
					float2_t Tex;
					DWORD color;
				};

				bool m_bFreezed;

				pointtex m_pVBackground[6];

				void UpdateBorder();
				void UpdateBackground();

				void RenderBackground(UINT lvl);

				bool m_bHasBackground;
				bool m_bHasBackgroundImage;

				float4_t m_pBackgroundColor;
				UINT m_iBackgroundColor;

				int m_iBackgroundOffsetX;
				int m_iBackgroundOffsetY;

				CPITexture m_pBackgroundImage;

				CDOMnode * m_pNode;
				CSS::CCSSstyle m_ComputedStyle;

				IRenderFrame * m_pParent;
				IRenderFrame * m_pNext;
				IRenderFrame * m_pPrev;
				Array<IRenderFrame*> m_pChilds;
				Array<IRenderFrame*> m_pChildsOutFlow;

				int m_iXpos;
				int m_iYpos;
				UINT m_iWidth;
				UINT m_iHeight;

				UINT m_iTopPos;
				UINT m_iTopPosMax;
				UINT m_iLeftPos;

				bool isWidthSet;

				int m_iScrollTop;
				int m_iScrollLeft;
				int m_iScrollTopMax;
				int m_iScrollLeftMax;
				int m_iScrollSpeedX;
				int m_iScrollSpeedY;

				bool m_bBackgroundRepeatX;
				bool m_bBackgroundRepeatY;

				bool m_bBackgroundScrolling;
				bool m_bBackgroundFixed;

				bool m_bHasFixedSize;
				bool m_bIsRectValid;

				float2_t m_fBackgroundImageSize;

				bool m_bNeedCut;

				IRenderBorder m_border;

				IScrollBar * m_pScrollBarVert;
				IScrollBar * m_pScrollBarHorz;

				DWORD rc;

				UINT m_iTextIdx;
				Array<Array<renderElement*>> m_mTextRELs;
			};

			class IRenderBlock: public IRenderFrame
			{
				DECLARE_CLASS(IRenderBlock, IRenderFrame);
			public:
				IRenderBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);
			};

			class IRenderAnonymousBlock: public IRenderFrame
			{
				DECLARE_CLASS(IRenderAnonymousBlock, IRenderFrame);
			public:
				IRenderAnonymousBlock(IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);
			};


			class IRenderInlineBlock: public IRenderFrame
			{
				DECLARE_CLASS(IRenderInlineBlock, IRenderFrame);
			public:
				IRenderInlineBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);
			};

			class IRenderInline: public IRenderFrame
			{
				DECLARE_CLASS(IRenderInline, IRenderFrame);
			public:
				IRenderInline(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);
			};

			class IRenderText: public IRenderFrame
			{
				DECLARE_CLASS(IRenderText, IRenderFrame);
			public:
				IRenderText(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);

			protected:
				IDirect3DVertexBuffer9 * m_pVertexBuffer;
				IDirect3DIndexBuffer9 * m_pIndexBuffer;
				UINT m_iVertexCount;
				UINT m_iIndexBaseCount;
				UINT m_iIndexAddCount;

				StringW m_szClearText;
				UINT m_iWordCount;

				IFont * pFont;

				void SetText(const StringW & text);
			};

			class IRenderTextNew: public IRenderFrame
			{
				DECLARE_CLASS(IRenderTextNew, IRenderFrame);
			public:
				IRenderTextNew(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);

				UINT GetCaretPos();
				void SetCaretPos(int cp, bool force = false);
				void MoveCaretPos(int shift);
				void MoveCaretLine(int shift);
				UINT GetCaretMaxPos();


				void PlaceCaretByXY(int x, int y);
				void SelectionStart();
				void SelectionEnd();
				StringW GetSelectionText();

				UINT GetSelectionStart();
				UINT GetSelectionEnd();

				void SetSelectionStart(int cp);
				void SetSelectionEnd(int cp);

			protected:

				Array<renderElement> m_pRenderElems;
				StringW m_szClearText;
				UINT m_iWordCount;

				Array<RECT> m_vCharRects;

				float4_t m_pColor;

				CSS::ICSSstyle * m_pStyle;

				IFont * pFont;

				void SetText(const StringW & text);

				UINT m_iCaretPos;
				UINT m_iSelectionStart;
				UINT m_iSelectionEnd;
				void DrawCaret();
				void RenderSelection();
				bool m_bInSelection;

				int m_iCaretX;
				int m_iCaretY;
				int m_iTextSize;
			};

			class IRenderImageBlock: public IRenderBlock
			{
				DECLARE_CLASS(IRenderImageBlock, IRenderBlock);
			public:
				IRenderImageBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);
			};

			class IRenderImageInlineBlock: public IRenderInlineBlock
			{
				DECLARE_CLASS(IRenderImageInlineBlock, IRenderInlineBlock);
			public:
				IRenderImageInlineBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT Layout();
				void Render(UINT lvl);
			};
		};
	};
};

#endif
