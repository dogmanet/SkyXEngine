#ifndef _IRenderBorder_H_
#define _IRenderBorder_H_

#include "GUIbase.h"

namespace GUI
{
	namespace DOM
	{
		namespace Render
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

				void SetWidth(SIDE s, UINT w);
				void SetRadius(CORNER c, UINT r);
				void SetCornerMethod(CORNER c, CORNER_METHOD m);

				void SetContentSize(UINT w, UINT h);

				void SetColor(SIDE s, float4_t clr);

				UINT GetWidth(SIDE s);
				UINT GetRadius(CORNER c);
				CORNER_METHOD GetCornerMethod(CORNER c);
				float4_t GetColor(SIDE s);

				void GetContentSize(UINT * w, UINT * h);

				void Build();

				void Render();

				void RenderInnerFill();

			protected:
				UINT m_iWidth[4];
				UINT m_iRadius[4];
				CORNER_METHOD m_method[4];

				IDirect3DIndexBuffer9 * m_pIndexBuffer;
				IDirect3DVertexBuffer9 * m_pVertexBuffer;

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

				static void BuildRadius(IRB_PARAM);
				static void BuildCut(IRB_PARAM);

				void ConcatBuffers(buff_t * pOut, const buff_t * b1, UINT idx1, const buff_t * b2, UINT idx2, float3_t * br);

				void CreateAPIbuffers(buff_t * t, buff_t * r, buff_t * b, buff_t * l, UINT * pIndices, UINT IndexCount);

				void ReleaseBuffer(buff_t * pIn);

				void CreateFill(buff_t * t, buff_t * r, buff_t * b, buff_t * l, UINT ** ppIndices, UINT * pIndexCount, UINT ** ppFillIDx[4], UINT * pFillIdxCount[4]);
			};
		};
	};
};

#endif
