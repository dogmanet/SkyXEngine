#ifndef __BRUSHMESH_H
#define __BRUSHMESH_H


#include <gdefines.h>
#include <xcommon/resource/IXModel.h>
#include <xcommon/IXCore.h>
#include <xcommon/IXJSON.h>
#include "Outline.h"
#include <physics/IXPhysics.h>


class CBrushMesh
{
public:
	CBrushMesh(IXCore *pCore, COutline *pOutline, UINT uContour, const char *szMat, float fHeight);
	CBrushMesh(IXCore *pCore);
	~CBrushMesh();

	SX_ALIGNED_OP_MEM();

	void enable(bool yesNo);
	const SMAABB& getAABB();

	void move(const float3 &vOffset);
	void rotate(const float3 &vOrigin, const SMQuaternion &qOffset);
	void resize(const float3 &vOrigin, const float3 &vRelativeSize);

	bool rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut = NULL, float3 *pvNormal = NULL, bool isRayInWorldSpace = true, bool bReturnNearestPoint = false);

	void renderSelection(bool is3D, IXGizmoRenderer *pGizmoRenderer);

	void serialize(Array<char> *paData);

	bool deserialize(IXJSONObject *pData);

private:
	void buildModel();
	void setupFromOutline(COutline *pOutline, UINT uContour, float fHeight);


private:
	//IXEditorGizmoHandle *m_pHandle[2];
	IXStaticModel *m_pModel = NULL;
	IXCore *m_pCore = NULL;
	IXPhysics *m_pPhysics = NULL;
	IRigidBody *m_pRigidBody = NULL;
	bool m_isFinalized = false;

	struct Edge
	{
		UINT uVertex[2];
		bool isInternal;
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

	Array<String> m_aMaterials;

	SMAABB m_aabb;
	bool m_isBoundDirty = true;
	bool m_isPhysicsLoaded = false;
};

#endif
