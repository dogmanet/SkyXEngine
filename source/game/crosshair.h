
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CROSSHAIR_H
#define __CROSSHAIR_H

//#include <gdefines.h>
#include "sxgame.h"
//#include <common/SXmath.h>

#define CROSSHAIR_MINSEGS 12
#define CROSSHAIR_MAXSEGS 12

static_assert(CROSSHAIR_MINSEGS <= CROSSHAIR_MAXSEGS, "Maxsegs cannot be lesser than minsegs!");

class CCrosshair
{
public:
	enum STYLE
	{
		SPLIT_MOVE,
		SCALED
	};

	CCrosshair();
	~CCrosshair();

	void update();
	void render();
	void onSync();

	void enable(bool en = true);

	void setNumSegmens(int num);
	void setAngle(float a);
	void setFixedRadius(float r);
	void setStyle(STYLE style);
	void setMaxSize(float size);
	void setTexture(ID id);
	
	void setSize(float size);

	void setTexInfo(const float2_t & offs, const float2_t & size);

protected:

	typedef struct
	{
		float3_t pos;
		float2_t tex;
	} Vertex;

	bool m_bDirty;
	bool m_bBuildBuff;
	float m_fSize;
	ID m_idTexture;
	bool m_bHidden;
	float m_fMaxSize;
	STYLE m_style;
	float m_fFixedRadius;
	float m_fAngle;
	int m_iNumSegs;

	byte * m_pMemoryBlob;

	byte m_u8ActiveBuffer;
	Vertex * m_pVertices[2];
	USHORT * m_pIndices[2];
	int m_iVertexCount[2];
	int m_iIndexCount[2];

	IGXVertexBuffer *m_pVertexBuffer;
	IGXIndexBuffer *m_pIndexBuffer;
	IGXVertexDeclaration *m_pVertexDeclaration;
	IGXRenderBuffer *m_pRenderBuffer;

	IGXContext * m_pDev;
	IGXTexture2D * m_pTexture;

	float2_t m_f2TexOffs;
	float2_t m_f2TexSize;

	ID m_idVS = -1;
	ID m_idPS = -1;
	ID m_idShaderKit = -1;

	IGXBlendState *m_pBlendState = NULL;
	IGXDepthStencilState *m_pDepthState = NULL;
};

#endif
