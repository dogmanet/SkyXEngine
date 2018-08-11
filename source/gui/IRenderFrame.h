#ifndef _IRenderFrame_H_
#define _IRenderFrame_H_

#include "GUIbase.h"
#include "DOM.h"
#include "ICSS.h"
#include "Font.h"

#include "IRenderBorder.h"

namespace gui
{
	namespace dom
	{
		//class IFont;
		namespace render
		{
			class CTranslationManager
			{
			public:
				static void pushMatrix(const SMMATRIX & m);
				static void popMatrix();
				static SMMATRIX getCurrentMatrix();
			protected:
				static SMMATRIX m_result;
				static Stack<SMMATRIX, 256, 16> m_stack;
			};

			class IScrollBar;

			class IRenderFrame
			{
				friend class CDOMnode;
			public:
				IRenderFrame(CDOMnode * pNode, IRenderFrame * pRootNode);
				~IRenderFrame();

				virtual UINT layout(bool changed=true);
				virtual void render(UINT lvl);

				virtual void onCreated();

				struct CRenderElement
				{
					CRenderElement():m_pNextREl(NULL)
					{
					}
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

					CRenderElement *m_pNextREl;
				};

				/*enum CUT_TYPE
				{
				CUT_NONE,
				CUT_BORDER,
				CUT_CONTENT
				};*/

				IRenderFrame * getParent();
				IRenderFrame * getNextSibling();
				IRenderFrame * getPrevSibling();

				IRenderFrame * m_pRootNode;

				UINT getChildCount();
				IRenderFrame * getChild(UINT id);
				CDOMdocument * m_pDoc;

				css::ICSSstyle * getComputedStyle();

				bool m_bIsOutOfFlow;

				UINT getWidth();
				UINT getContentWidth();
				UINT getContentHeight();
				UINT getHeight();
				int getXpos();
				void setXpos(int x);
				int getYpos();

				UINT getInnerWidth();
				UINT getInnerHeight();

				void addChild(IRenderFrame * pChild, bool bOutFlow = false);
				void setParent(IRenderFrame * pParent);

				static IRenderFrame * createNode(CDOMnode * pNode, IRenderFrame * pRootNode);

				void debugPrint(UINT lvl = 0);

				void setWidth(UINT width);
				void setHeight(UINT width);

				UINT getTopPos();
				UINT getLeftPos();
				void setLeftPos(UINT p);
				void setTopPos(UINT p);
				UINT getTopPosMax();
				void setTopPosMax(UINT p);

				UINT getTopOffset();
				UINT getLeftOffset();

				UINT getClientTop();
				UINT getClientLeft();

				CDOMnode * getNode();

				UINT getContentTop();
				UINT getContentLeft();

				UINT getInnerTop();
				UINT getInnerLeft();

				void textStart();
				void textClear();
				void textBreak();
				void textAppend(CRenderElement * rel, int iLineIdx);
				int textGetLineIdx();
				UINT textFixUp();

				void updateStyles();

				void updateBorderColor();

				void resetLayout(bool first = true, bool bForce = false);

				RECT getClientRect();
				RECT getVisibleRect();

				int getScrollLeft();
				int getScrollLeftMax();
				void setScrollLeft(int x);

				int getScrollTop();
				int getScrollTopMax();
				void setScrollTop(int x, bool _check_bounds = false);

				void setScrollSpeedX(int x);
				void setScrollSpeedY(int x);
				void updateScroll();

				void findElementByXY(int x, int y, IDOMnode ** ppNode, bool sendEnterLeave = false);

				bool inRect(const RECT & rc, int x, int y);

				bool hasFixedSize()
				{
					return(m_bHasFixedSize || isFreezzed());
				}

				bool isOutOfFlow()
				{
					return(m_bIsOutOfFlow);
				}

				void isOutOfFlow(bool is)
				{
					m_bIsOutOfFlow = is;
				}

				bool isChildOf(IRenderFrame * pNode)
				{
					IRenderFrame * pCur = getParent();
					while(pCur)
					{
						if(pCur == pNode)
						{
							return(true);
						}
						pCur = pCur->getParent();
					}
					return(false);
				}

				void setDocument(CDOMdocument * doc)
				{
					m_pDoc = doc;
				}

				bool isLastChild()
				{
					if(!m_pParent)
					{
						return(true);
					}
					return(m_pParent->getChild(m_pParent->getChildCount() - 1) == this);
				}

				void freeze(bool dof = true)
				{
					m_bFreezed = dof;
				}

				bool isFreezzed()
				{
					return(m_bFreezed);
				}

				void removeChild(IRenderFrame * prf);

			protected:

				struct pointtex
				{
					float3_t Pos;
					float2_t Tex;
					DWORD color;
				};

				bool m_bFreezed;

				pointtex m_pVBackground[15];
				int m_iTCBackground;

				void updateBorder();
				void updateBackground();

				void renderBackground(UINT lvl);

				bool m_bHasBackground;
				bool m_bHasBackgroundImage;

				float4_t m_pBackgroundColor;
				UINT m_iBackgroundColor;

				int m_iBackgroundOffsetX;
				int m_iBackgroundOffsetY;

				CPITexture m_pBackgroundImage;

				CDOMnode * m_pNode;
				css::CCSSstyle m_ComputedStyle;

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
				Array<Array<CRenderElement*>> m_mTextRELs;
			};

			class IRenderBlock: public IRenderFrame
			{
				DECLARE_CLASS(IRenderBlock, IRenderFrame);
			public:
				IRenderBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				void render(UINT lvl);
			};

			class IRenderAnonymousBlock: public IRenderFrame
			{
				DECLARE_CLASS(IRenderAnonymousBlock, IRenderFrame);
			public:
				IRenderAnonymousBlock(IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				void render(UINT lvl);
			};


			class IRenderInlineBlock: public IRenderBlock
			{
				DECLARE_CLASS(IRenderInlineBlock, IRenderBlock);
			public:
				IRenderInlineBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				//void render(UINT lvl);
			};

			class IRenderInline: public IRenderFrame
			{
				DECLARE_CLASS(IRenderInline, IRenderFrame);
			public:
				IRenderInline(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				void render(UINT lvl);
			};

#if 0

			class IRenderText: public IRenderFrame
			{
				DECLARE_CLASS(IRenderText, IRenderFrame);
			public:
				IRenderText(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				void render(UINT lvl);

			protected:
				IDirect3DVertexBuffer9 * m_pVertexBuffer;
				IDirect3DIndexBuffer9 * m_pIndexBuffer;
				UINT m_iVertexCount;
				UINT m_iIndexBaseCount;
				UINT m_iIndexAddCount;

				StringW m_szClearText;
				UINT m_iWordCount;

				CFont * pFont;

				void setText(const StringW & text);
			};

#endif

			class IRenderTextNew: public IRenderFrame
			{
				DECLARE_CLASS(IRenderTextNew, IRenderFrame);
			public:
				IRenderTextNew(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				void render(UINT lvl);

				UINT getCaretPos();
				void setCaretPos(int cp, bool force = false);
				void moveCaretPos(int shift);
				void moveCaretLine(int shift);
				UINT getCaretMaxPos();


				void placeCaretByXY(int x, int y);
				void selectionStart();
				void selectionEnd();
				StringW getSelectionText();

				UINT getSelectionStart();
				UINT getSelectionEnd();

				void setSelectionStart(int cp);
				void setSelectionEnd(int cp);

			protected:

				Array<CRenderElement> m_pRenderElems;
				//Array<CRenderElement> m_pRenderElemsShadow;
				StringW m_szClearText;
				UINT m_iWordCount;

				Array<RECT> m_vCharRects;

				float4_t m_pColor;

				css::ICSSstyle * m_pStyle;

				CFont * pFont;
				CFont * pShadowFont;

				void setText(const StringW & text);

				UINT m_iCaretPos;
				UINT m_iSelectionStart;
				UINT m_iSelectionEnd;
				void drawCaret();
				void renderSelection();
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
				UINT layout(bool changed = true);
				void render(UINT lvl);
			};

			class IRenderImageInlineBlock: public IRenderInlineBlock
			{
				DECLARE_CLASS(IRenderImageInlineBlock, IRenderInlineBlock);
			public:
				IRenderImageInlineBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);
				void render(UINT lvl);
			};

			class IRenderSelectBlock: public IRenderBlock
			{
				DECLARE_CLASS(IRenderSelectBlock, IRenderBlock);
			public:
				IRenderSelectBlock(CDOMnode * pNode, IRenderFrame * pRootNode);
				UINT layout(bool changed = true);

				void onCreated();

			protected:
				IRenderFrame *m_pOptionsFrame;
			};

			class IRenderSelectOptionsBlock: public IRenderBlock
			{
				DECLARE_CLASS(IRenderSelectOptionsBlock, IRenderBlock);
			public:
				IRenderSelectOptionsBlock(IRenderFrame * pSelectFrame, IRenderFrame * pRootNode);

				UINT layout(bool changed = true);
				void render(UINT lvl);

			protected:
				IRenderFrame *m_pSelectFrame;
			};
		};
	};
};

#endif
