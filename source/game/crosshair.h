#ifndef _crosshair_H_
#define _crosshair_H_

//#include <gdefines.h>
#include "sxgame.h"
//#include <common/SXmath.h>

#define CROSSHAIR_MINSEGS 12
#define CROSSHAIR_MAXSEGS 12

static_assert(CROSSHAIR_MINSEGS <= CROSSHAIR_MAXSEGS, "Maxsegs cannot be lesser than minsegs!");

class Crosshair
{
public:
	enum STYLE
	{
		SPLIT_MOVE,
		SCALED
	};

	Crosshair();
	~Crosshair();

	void Update();
	void Render();
	void OnSync();

	void Enable(bool en = true);

	void SetNumSegmens(int num);
	void SetAngle(float a);
	void SetFixedRadius(float r);
	void SetStyle(STYLE style);
	void SetMaxSize(float size);
	void SetTexture(ID id);
	
	void SetSize(float size);

	void SetTexInfo(const float2_t & offs, const float2_t & size);

protected:

	typedef struct
	{
		float3_t pos;
		float2_t tex;
	} Vertex;

	void _BuildBuffer();

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
	UINT * m_pIndices[2];
	int m_iVertexCount[2];
	int m_iIndexCount[2];

	IDirect3DVertexBuffer9 * m_pVertexBuffer;
	IDirect3DIndexBuffer9 * m_pIndexBuffer;

	IDirect3DDevice9 * m_pDev;
	IDirect3DTexture9 * m_pTexture;

	float2_t m_f2TexOffs;
	float2_t m_f2TexSize;
};

#endif
