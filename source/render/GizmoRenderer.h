#ifndef __GIZMORENDERER_H
#define __GIZMORENDERER_H

#include <xcommon/render/IXRenderUtils.h>

class CGizmoRenderer final: public IXUnknownImplementation<IXGizmoRenderer>
{
public:
	CGizmoRenderer(IGXDevice *pDev);
	~CGizmoRenderer();

	void XMETHODCALLTYPE reset() override;
	void XMETHODCALLTYPE render(bool isOrtho) override;

	void XMETHODCALLTYPE setLineWidth(float fWidth) override;

	void XMETHODCALLTYPE setColor(const float4 &vColor) override;
	void XMETHODCALLTYPE setTexture(IGXBaseTexture *pTexture) override;

	void XMETHODCALLTYPE jumpTo(const float3 &vPos) override;
	void XMETHODCALLTYPE lineTo(const float3 &vPos) override;


	
	void XMETHODCALLTYPE setPointSize(float fWidth) override;
	void XMETHODCALLTYPE setPointMode(XGIZMO_POINT_MODE pointMode) override;
	void XMETHODCALLTYPE drawPoint(const float3 &vPos) override;

	void XMETHODCALLTYPE drawAABB(const SMAABB &aabb) override;

	void XMETHODCALLTYPE drawEllipsoid(const float3 &vPos, const float3 vSize) override;

private:
	IGXDevice *m_pDev;

	IGXBlendState *m_pBlendAlpha = NULL;
	IGXDepthStencilState *m_pDSState3D = NULL;
	IGXDepthStencilState *m_pDSState2D = NULL;

	IGXBaseTexture *m_pCurrentTexture = NULL;
	byte m_u8CurrentTexture = 0xFF;
	float4_t m_vCurrentColor = float4_t(0.0f, 0.0f, 0.0f, 1.0f);
	float4_t m_vLastPointColor = float4_t(0.0f, 0.0f, 0.0f, 1.0f);
	float m_fCurrentLineWidth = 1.0f;
	float m_fCurrentPointSize = 1.0f;
	XGIZMO_POINT_MODE m_pointMode = XGPM_SQUARE;

	struct LineVertex
	{
		float4_t vPosWidth;
		float4_t vColor;
		float3_t vDir;
		float2_t vTexUV;
	};

	struct LinePoint
	{
		LineVertex vtx;
		byte u8Texture;
		byte isStart;
	};

	struct LineRange
	{
		byte u8Texture;
		UINT uStartVtx;
		UINT uTriangleCount;
	};

	Array<LineRange> m_aLineRanges;
	Array<IGXBaseTexture*> m_aTextures;
	Array<LinePoint> m_aVertices;
	float3_t m_vNextLineStart;
	bool m_isLineStart = true;

	UINT m_uLineCount = 0;
	UINT m_uLineSegmentCount = 0;

	UINT m_uLinesVBSize = 0;
	IGXVertexDeclaration *m_pLinesVD = NULL;
	IGXRenderBuffer *m_pLinesRB = NULL;
	IGXVertexBuffer *m_pLinesVB = NULL;
	bool m_isLinesDirty = false;

	ID m_idLinesShaderColored = -1;
	ID m_idLinesShaderTextured = -1;
	//IGXIndexBuffer *m_pLinesIB = NULL;

private:
	//void prepareLinesIB();
};

#endif
