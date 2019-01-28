#ifndef _IRenderBorder_H_
#define _IRenderBorder_H_

#include "GUIbase.h"

namespace gui
{
	namespace dom
	{
		namespace render
		{

#define IRB_PARAM float t, float l, float r, float3_t *** pppVertices, UINT *** pppIndexes, UINT * iVertexCount, UINT * iIndexCount, float3_t * pBrd, int * c, UINT *** ppFillIDx, UINT ** pFillIdxCount
			class IRenderBorder
			{
			public:
				IRenderBorder();
				~IRenderBorder();

				enum CORNER_METHOD
				{
					CORNER_METHOD_RADIUS,
					CORNER_METHOD_CUT
				};

				enum SIDE
				{
					TOP,
					RIGHT,
					BOTTOM,
					LEFT
				};

				enum CORNER
				{
					TOP_LEFT,
					TOP_RIGHT,
					BOTTOM_LEFT,
					BOTTOM_RIGHT
				};

				void setWidth(SIDE s, UINT w);
				void setRadius(CORNER c, UINT r);
				void setCornerMethod(CORNER c, CORNER_METHOD m);

				void setContentSize(UINT w, UINT h);

				void setColor(SIDE s, float4_t clr);

				UINT getWidth(SIDE s);
				UINT getRadius(CORNER c);
				CORNER_METHOD getCornerMethod(CORNER c);
				float4_t getColor(SIDE s);

				void getContentSize(UINT * w, UINT * h);

				void build();

				void render();

				void renderInnerFill();

			protected:
				UINT m_iWidth[4];
				UINT m_iRadius[4];
				CORNER_METHOD m_method[4];

				IGXIndexBuffer * m_pIndexBuffer;
				IGXVertexBuffer * m_pVertexBuffer;
				IGXRenderBuffer *m_pRenderBuffer = NULL;

				UINT m_iVertexCount[4];
				UINT m_iVertexStart[4];
				UINT m_iIndexStart[4];
				UINT m_iIndexCount[4];

				int m_iSideCount[4];

				int m_iFillIndexCount;

				// 0 =
				// 1 = 

				float4_t m_pColor[4];

				UINT m_iInnerWidth;
				UINT m_iInnerHeight;

				struct buff_t
				{
					float3_t ** vb;
					UINT ** ib;
					float3_t br[4];
					UINT iIC[2];
					UINT iVC[2];
				};

				static void buildRadius(IRB_PARAM);
				static void buildCut(IRB_PARAM);

				void concatBuffers(buff_t * pOut, const buff_t * b1, UINT idx1, const buff_t * b2, UINT idx2, float3_t * br);

				void createAPIbuffers(buff_t * t, buff_t * r, buff_t * b, buff_t * l, UINT * pIndices, UINT IndexCount);

				void releaseBuffer(buff_t * pIn);

				void createFill(buff_t * t, buff_t * r, buff_t * b, buff_t * l, UINT ** ppIndices, UINT * pIndexCount, UINT ** ppFillIDx[4], UINT * pFillIdxCount[4]);
			};
		};
	};
};

#endif
