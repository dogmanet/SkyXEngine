#ifndef __BRUSHMESH_H
#define __BRUSHMESH_H


#include <gdefines.h>
#include <xcommon/resource/IXModel.h>
#include <xcommon/IXCore.h>
#include "Outline.h"



class CBrushMesh
{
public:
	CBrushMesh(IXCore *pCore, COutline *pOutline, UINT uContour);
	~CBrushMesh();

	SX_ALIGNED_OP_MEM();

	void enable(bool yesNo);
	const SMAABB& getAABB();

	void move(const float3 &vOffset);
	void rotate(const float3 &vOrigin, const SMQuaternion &qOffset);
	
	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut = NULL, float3 *pvNormal = NULL, bool isRayInWorldSpace = true, bool bReturnNearestPoint = false);

	void renderSelection(bool is3D, IXGizmoRenderer *pGizmoRenderer);

private:
	void buildModel();
	void setupFromOutline(COutline *pOutline, UINT uContour);


private:
	//IXEditorGizmoHandle *m_pHandle[2];
	bool m_isFinalized = false;
	IXStaticModel *m_pModel = NULL;
	IXCore *m_pCore = NULL;

	struct Edge
	{
		UINT uVertex[2];
	};

	struct TextureInfo
	{
		float3_t vS;
		float fSShift;    // Texture shift in s direction
		float3_t vT;
		float fTShift;    // Texture shift in t direction
		UINT uMatId;
		//uint32_t iMiptex; // Index into textures array
		//uint32_t nFlags;  // Texture flags, seem to always be 0
	};

	struct Face
	{
		TextureInfo texInfo;
		Array<UINT> aEdges;
		float3_t vNormal;
	};

	Array<float3_t> m_aVertices;
	Array<Edge> m_aEdges;
	Array<Face> m_aFaces;

	Array<const char*> m_aMaterials;

	SMAABB m_aabb;
	bool m_isBoundDirty = true;
};

#endif
