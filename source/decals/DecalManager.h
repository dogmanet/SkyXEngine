
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __DECAL_MANAGER_H
#define __DECAL_MANAGER_H

#include <gdefines.h>
#include <common/SXMath.h>
#include <common/Array.h>
#include <common/AssotiativeArray.h>
#include <graphix/graphix.h>
#include "sxdecals.h"


enum DECAL
{
	DECAL_PERMANENT = 0x01,
	DECAL_HFLIP = 0x02,
	DECAL_VFLIP = 0x04,
	DECAL_CLIPTEST = 0x08,
	DECAL_NOCLIP = 0x10,
	DECAL_USESAXIS = 0x20,
	//DECAL_IS_RENDER = 0x40,
	DECAL_ONE_SIDED = 0x80
};

struct DecalInfo
{
	float3_t m_Position;
	float3_t m_SAxis;
	UINT m_iMaterial;
	float m_Size;
	int m_Flags;
	int m_iEnt;
	float m_scale;
	float m_decalWidth;
	float m_decalHeight;
	float3_t m_Basis[3];
};

struct DecalVertex
{
	float3_t pos;
	float3_t normal;
	float2_t tex;
};

class Decal
{
public:
	UINT m_material;

	float3_t m_vPos;
	float3_t m_vSAxis;
	float m_dx;
	float m_dy;
	float m_fScale;
	short m_flags;
	short m_iEntity;
	float m_Size;
	UINT iMatSortId;

	//UINT * m_pSplitIDs;

	//int iSplitCount;

	DecalVertex * m_pVerts;
	int iVertCount;
};

#define SIN_45_DEGREES 0.70710678118654752440084436210485f

struct DecalTexRange
{
	int xmin;
	int ymin;
	int xmax;
	int ymax;
};

struct DecalType
{
	Array<DecalTexRange> m_texs;
	ID m_MaterialId;
	float m_fBaseScale;
};

struct _DecalMatItem
{
	Decal * m_pDecal;
	UINT iDecalId;
};

struct _DecalTexRange
{
	UINT iStartVertex;
	UINT iVertexCount;
	UINT iMaterialId;
};

class DecalManager
{
	//static Decal * m_pDecals[MAX_DECALS];
	Array<Decal*> m_vDecals;
	Array<float3_t> m_dbgRender;
	AssotiativeArray<ID, Array<_DecalMatItem>> m_MaterialSort;

	DecalType m_DecalTypes[DECAL_TYPE_LAST];
	bool m_bNeedUpdate;

	Array<_DecalTexRange> m_iRngs;

	//static IDirect3DIndexBuffer9 * m_pIndexBuffer;
	IGXVertexBuffer *m_pVertexBuffer;
	IGXRenderBuffer *m_pRenderBuffer;

	IGXVertexDeclaration *m_pVertexDeclaration;

	MemAlloc<Decal, 64> m_aDecals;

	IGXContext *dev;
public:

	DecalManager();
	~DecalManager();

	int addDecal(Decal * pDecal);

	void removeDecal(UINT iDecal);

	const DecalType * getDecalType(DECAL_TYPE type);

	bool inside(const float3_t * p, char axis, float coord);

	void render();

	void clear();

	void update();
	void sync();

	void intersect(const float3_t * one, const float3_t * two, float3_t * out, char axis, float coord);


	void clip(const Array<float3_t> & InVerts, Array<float3_t> & OutVerts, char axis, float coord);

	void computeBasis(float3 const & surfaceNormal, float3 const * pSAxis, float3 * textureSpaceBasis)
	{
		// s, t, textureSpaceNormal (T cross S = textureSpaceNormal(N))
		//   N     
		//   \   
		//    \     
		//     \  
		//      +---->S
		//      | 
		//		|  
		//      |T    
		// S = textureSpaceBasis[0]
		// T = textureSpaceBasis[1]
		// N = textureSpaceBasis[2]

		// Get the surface normal.
		textureSpaceBasis[2] = surfaceNormal;

		if(pSAxis)
		{
			// T = S cross N
			textureSpaceBasis[1] = SMVector3Cross(*pSAxis, textureSpaceBasis[2]);

			// Name sure they aren't parallel or antiparallel
			// In that case, fall back to the normal algorithm.
			if(SMVector3Dot(textureSpaceBasis[1], textureSpaceBasis[1]) > 1e-6)
			{
				// S = N cross T
				textureSpaceBasis[0] = SMVector3Cross(textureSpaceBasis[2], textureSpaceBasis[1]);

				textureSpaceBasis[0] = SMVector3Normalize(textureSpaceBasis[0]);
				textureSpaceBasis[1] = SMVector3Normalize(textureSpaceBasis[1]);
				return;
			}

			// Fall through to the standard algorithm for parallel or antiparallel
		}

		// floor/ceiling?
		if(fabs(surfaceNormal.y) > SIN_45_DEGREES)
		{
			textureSpaceBasis[0].x = 1.0f;
			textureSpaceBasis[0].y = 0.0f;
			textureSpaceBasis[0].z = 0.0f;

			// T = S cross N
			textureSpaceBasis[1] = SMVector3Cross(textureSpaceBasis[0], textureSpaceBasis[2]);

			// S = N cross T
			textureSpaceBasis[0] = SMVector3Cross(textureSpaceBasis[2], textureSpaceBasis[1]);
		}
		// wall
		else
		{
			textureSpaceBasis[1].x = 0.0f;
			textureSpaceBasis[1].y = -1.0f;
			textureSpaceBasis[1].z = 0.0f;

			// S = N cross T
			textureSpaceBasis[0] = SMVector3Cross(textureSpaceBasis[2], textureSpaceBasis[1]);
			// T = S cross N
			textureSpaceBasis[1] = SMVector3Cross(textureSpaceBasis[0], textureSpaceBasis[2]);
		}

		textureSpaceBasis[0] = SMVector3Normalize(textureSpaceBasis[0]);
		textureSpaceBasis[1] = SMVector3Normalize(textureSpaceBasis[1]);
	}

	void updateBuffer();

	void shootDecal(DECAL_TYPE type, const float3 & position, ID iMaterial = -1, int iEnt = 0, const float3 * saxis = NULL, float scale = 1.0f, int flags = 0, const float3 * normal = NULL);
};

#endif
