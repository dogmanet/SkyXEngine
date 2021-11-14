#include "BrushMesh.h"
#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/resource/IXModelProvider.h>
#include <xcommon/IPluginManager.h>


CBrushMesh::CBrushMesh(IXCore *pCore, COutline *pOutline, UINT uContour, const char *szMat, float fHeight):
	m_pCore(pCore)
{
	//m_pHandle[0] = NULL;
	//m_pHandle[1] = NULL;
	//m_aMaterials.push_back("LAB1_FLOOR4");
	//m_aMaterials.push_back("C1A3WALL04");
	// m_aMaterials.push_back("dev_floor");
	// m_aMaterials.push_back("dev_wall");


	m_pPhysics = (IXPhysics*)m_pCore->getPluginManager()->getInterface(IXPHYSICS_GUID);

	m_aMaterials.push_back(szMat);

	//m_aMaterials.push_back("block_grass_top");
	//m_aMaterials.push_back("block_grass_side");
	setupFromOutline(pOutline, uContour, fHeight);

}

CBrushMesh::CBrushMesh(IXCore *pCore):
	m_pCore(pCore)
{
	m_pPhysics = (IXPhysics*)m_pCore->getPluginManager()->getInterface(IXPHYSICS_GUID);
}

CBrushMesh::~CBrushMesh()
{
	if(m_pRigidBody && m_isPhysicsLoaded)
	{
		m_pPhysics->removeCollisionObject(m_pRigidBody);
	}
	mem_release(m_pRigidBody);
	mem_release(m_pModel);
}

void CBrushMesh::enable(bool yesNo)
{
	SAFE_CALL(m_pModel, enable, yesNo);
	if(m_pRigidBody)
	{
		if(yesNo)
		{
			if(!m_isPhysicsLoaded)
			{
				m_pPhysics->addCollisionObject(m_pRigidBody, CG_STATIC, CG_STATIC_MASK);
				m_isPhysicsLoaded = true;
			}
		}
		else if(m_isPhysicsLoaded)
		{
			m_pPhysics->removeCollisionObject(m_pRigidBody);
			m_isPhysicsLoaded = false;
		}
	}
}

void CBrushMesh::buildModel()
{
	struct Subset
	{
		Array<XResourceModelStaticVertex> aVertices;
		Array<UINT> aIndices;
	};
	Array<Subset> aSubsets(m_aMaterials.size());

	for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		Face &face = m_aFaces[i];
		TextureInfo &texInfo = face.texInfo;
		Subset &subset = aSubsets[texInfo.uMatId];

		bool isFirst = true;
		UINT uFirstIdx;
		UINT uNextIdx;
		UINT uLastVtx = UINT_MAX;

		Edge &e0 = m_aEdges[face.aEdges[0]];
		Edge &e1 = m_aEdges[face.aEdges[1]];
		if(e0.uVertex[0] == e1.uVertex[0] || e0.uVertex[0] == e1.uVertex[1])
		{
			std::swap(e0.uVertex[0], e0.uVertex[1]);
		}

		for(UINT j = 0, jl = face.aEdges.size() - 1; j < jl; ++j)
		{
			Edge edge = m_aEdges[face.aEdges[j]];
			if(edge.uVertex[0] != uLastVtx && uLastVtx != UINT_MAX)
			{
				std::swap(edge.uVertex[0], edge.uVertex[1]);
			}
			uLastVtx = edge.uVertex[1];

			XResourceModelStaticVertex vtx = {};
			vtx.vNorm = face.vNormal;
			vtx.vTangent = texInfo.vS;
			vtx.vBinorm = SMVector3Normalize(SMVector3Cross(vtx.vTangent, vtx.vNorm));
			vtx.vTangent = SMVector3Normalize(SMVector3Cross(vtx.vNorm, vtx.vBinorm));

			if(isFirst)
			{
				isFirst = false;

				vtx.vPos = m_aVertices[edge.uVertex[0]];
				vtx.vTex.x = (SMVector3Dot(vtx.vPos, texInfo.vS) + texInfo.fSShift);
				vtx.vTex.y = (SMVector3Dot(vtx.vPos, texInfo.vT) + texInfo.fTShift);

				uFirstIdx = subset.aVertices.size();
				subset.aVertices.push_back(vtx);

				vtx.vPos = m_aVertices[edge.uVertex[1]];
				vtx.vTex.x = (SMVector3Dot(vtx.vPos, texInfo.vS) + texInfo.fSShift);
				vtx.vTex.y = (SMVector3Dot(vtx.vPos, texInfo.vT) + texInfo.fTShift);

				uNextIdx = subset.aVertices.size();
				subset.aVertices.push_back(vtx);

				// place first vertex, save idx
				// place second vertex, save idx
			}
			else
			{
				vtx.vPos = m_aVertices[edge.uVertex[1]];
				vtx.vTex.x = (SMVector3Dot(vtx.vPos, texInfo.vS) + texInfo.fSShift);
				vtx.vTex.y = (SMVector3Dot(vtx.vPos, texInfo.vT) + texInfo.fTShift);

				subset.aIndices.push_back(uFirstIdx);
				subset.aIndices.push_back(uNextIdx);

				uNextIdx = subset.aVertices.size();
				subset.aVertices.push_back(vtx);
				subset.aIndices.push_back(uNextIdx);

				// place new vertex, place 3 indices, save idx
			}
		}
	}


	// create model
	IXResourceManager *pRM = m_pCore->getResourceManager();
	IXResourceModelStatic *pResource = pRM->newResourceModelStatic();

	UINT uSubsets = 0;
	pResource->setMaterialCount(m_aMaterials.size(), 1);
	for(UINT i = 0, l = m_aMaterials.size(); i < l; ++i)
	{
		if(aSubsets[i].aIndices.size())
		{
			pResource->setMaterial(i, 0, m_aMaterials[i].c_str());
			++uSubsets;
		}
	}

	UINT *puVtxCount = (UINT*)alloca(sizeof(UINT) * uSubsets);
	UINT *puIdxCount = (UINT*)alloca(sizeof(UINT) * uSubsets);

	uSubsets = 0;
	for(UINT i = 0, l = m_aMaterials.size(); i < l; ++i)
	{
		Subset &subset = aSubsets[i];
		if(subset.aIndices.size())
		{
			puVtxCount[uSubsets] = subset.aVertices.size();
			puIdxCount[uSubsets] = subset.aIndices.size();

			++uSubsets;
		}
	}

	pResource->addLod(uSubsets, puVtxCount, puIdxCount);
	uSubsets = 0;
	for(UINT i = 0, l = m_aMaterials.size(); i < l; ++i)
	{
		Subset &subset = aSubsets[i];
		if(subset.aIndices.size())
		{
			XResourceModelStaticSubset *pSubset = pResource->getSubset(0, uSubsets);

			pSubset->iMaterialID = i;
			pSubset->iIndexCount = subset.aIndices.size();
			pSubset->iVertexCount = subset.aVertices.size();

			memcpy(pSubset->pIndices, subset.aIndices, sizeof(UINT) * pSubset->iIndexCount);
			memcpy(pSubset->pVertices, subset.aVertices, sizeof(XResourceModelStaticVertex) * pSubset->iVertexCount);

			++uSubsets;
		}
	}


	mem_release(m_pModel);
	IXModelProvider *pProvider = (IXModelProvider*)m_pCore->getPluginManager()->getInterface(IXMODELPROVIDER_GUID);
	pProvider->createStaticModel(pResource, &m_pModel);
	mem_release(pResource);

	if(m_pRigidBody && m_isPhysicsLoaded)
	{
		m_pPhysics->removeCollisionObject(m_pRigidBody);
	}
	mem_release(m_pRigidBody);

	IConvexHullShape *pShape = m_pPhysics->newConvexHullShape(m_aVertices.size(), m_aVertices, sizeof(float3_t), false);

	XRIDIGBODY_DESC desc = {};
	desc.fMass = 0.0f;
	desc.pCollisionShape = pShape;
	m_pRigidBody = m_pPhysics->newRigidBody(&desc);
	mem_release(pShape);

	m_pPhysics->addCollisionObject(m_pRigidBody, CG_STATIC, CG_STATIC_MASK);
	m_isPhysicsLoaded = true;
}

static float3 GetTangent(const float3 &vNormal)
{
	if(fabsf(SMVector3Dot(vNormal, float3(0.0f, 1.0f, 0.0f))) > cosf(SM_PIDIV4))
	{
		return(float3(0.0f, 0.0f, 1.0f));
	}
	return(float3(0.0f, -1.0f, 0.0f));
}

void CBrushMesh::setupFromOutline(COutline *pOutline, UINT uContour, float fHeight)
{
	const CContour &c = pOutline->getContour(uContour);
	UINT uOutlinePts = pOutline->getPointCount();
	UINT uPts = c.getPointCount();

	//const float c_fDefaultWidth = 1.2f;

	m_aVertices.resize(uPts * 2);
	m_aEdges.resize(uPts * 3);
	m_aFaces.resize(uPts + 2);

	float3 vNormal = pOutline->getNormal();
	float3 vOffset = vNormal * fHeight;


	Face &f1 = m_aFaces[uPts + 0];
	Face &f2 = m_aFaces[uPts + 1];

	f1.texInfo = {};
	f2.texInfo = {};
	f1.vNormal = (float3)-vNormal;
	f2.vNormal = vNormal;
	f1.texInfo.vT = GetTangent(f1.vNormal);
	f1.texInfo.vS = SMVector3Cross(f1.texInfo.vT, f1.vNormal);
	
	f2.texInfo.vT = GetTangent(f2.vNormal);
	f2.texInfo.vS = SMVector3Cross(f2.texInfo.vT, f2.vNormal);

	f1.aEdges.reserve(uPts);
	f2.aEdges.reserve(uPts);

	for(UINT i = 0; i < uPts; ++i)
	{
		m_aVertices[i] = pOutline->getPoint(c.getPoint(i));
		m_aVertices[i + uPts] = (float3)(m_aVertices[i] + vOffset);
	}

	for(UINT i = 0; i < uPts; ++i)
	{
		bool isInternal = (c.getPoint(i) + 1) % uOutlinePts != c.getPoint(i + 1);

		Edge &e = m_aEdges[i];
		e.uVertex[0] = i;
		e.uVertex[1] = (i + 1) % uPts;
		e.isInternal = isInternal;

		Edge &e2 = m_aEdges[i + uPts];
		e2.uVertex[0] = i + uPts;
		e2.uVertex[1] = (i + 1) % uPts + uPts;
		e2.isInternal = isInternal;

		Edge &e3 = m_aEdges[i + uPts * 2];
		e3.uVertex[0] = i;
		e3.uVertex[1] = i + uPts;
		e3.isInternal = false;

		// m_aVertices[i] = pOutline->getPoint(c.getPoint(i));
		// m_aVertices[i + uPts] = (float3)(m_aVertices[i] + vOffset);

		Face &f = m_aFaces[i];
		f.aEdges.reserve(4);
		f.aEdges.push_back(i + uPts * 2);
		f.aEdges.push_back(i + uPts);
		f.aEdges.push_back((i + 1) % uPts + uPts * 2);
		f.aEdges.push_back(i);

		float3 v1 = (e3.uVertex[0] == e2.uVertex[0] || e3.uVertex[0] == e2.uVertex[1])
			? m_aVertices[e3.uVertex[1]] - m_aVertices[e3.uVertex[0]]
			: m_aVertices[e3.uVertex[0]] - m_aVertices[e3.uVertex[1]];

		float3 v0 = (e2.uVertex[0] == e3.uVertex[0] || e2.uVertex[0] == e3.uVertex[1])
			? m_aVertices[e2.uVertex[1]] - m_aVertices[e2.uVertex[0]]
			: m_aVertices[e2.uVertex[0]] - m_aVertices[e2.uVertex[1]];

		f.texInfo = {};
		f.vNormal = SMVector3Normalize(SMVector3Cross(v0, v1));
		f.texInfo.vT = GetTangent(f.vNormal);
		f.texInfo.vS = SMVector3Cross(f.texInfo.vT, f.vNormal);
		f.texInfo.uMatId = 0; //1;

		f1.aEdges.push_back(i);
		f2.aEdges.push_back((uPts - i - 1) + uPts);
	}

	buildModel();

	m_isBoundDirty = true;
}

const SMAABB& CBrushMesh::getAABB()
{
	if(m_isBoundDirty)
	{
		m_isBoundDirty = false;
		float3 vMin = m_aVertices[0];
		float3 vMax = m_aVertices[0];
		for(UINT i = 1, l = m_aVertices.size(); i < l; ++i)
		{
			vMin = SMVectorMin(vMin, m_aVertices[i]);
			vMax = SMVectorMax(vMax, m_aVertices[i]);
		}
		m_aabb = SMAABB(vMin, vMax);
	}

	return(m_aabb);
}

void CBrushMesh::move(const float3 &vOffset)
{
	if(m_pModel)
	{
		m_pModel->setPosition(m_pModel->getPosition() + vOffset);
	}
	
	for(UINT i = 0, l = m_aVertices.size(); i < l; ++i)
	{
		m_aVertices[i] = (float3)(m_aVertices[i] + vOffset);
	}

	if(m_pRigidBody)
	{
		m_pRigidBody->setPosition(m_pRigidBody->getPosition() + vOffset);
	}

	for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		Face &f = m_aFaces[i];
		f.texInfo.fSShift -= SMVector3Dot(f.texInfo.vS, vOffset);
		f.texInfo.fTShift -= SMVector3Dot(f.texInfo.vT, vOffset);
	}

	m_aabb = m_aabb + vOffset;
}

void CBrushMesh::rotate(const float3 &vOrigin, const SMQuaternion &qOffset)
{
	if(m_pModel)
	{
		m_pModel->setOrientation(m_pModel->getOrientation() * qOffset);
		m_pModel->setPosition(qOffset * (m_pModel->getPosition() - vOrigin) + vOrigin);
	}

	if(m_pRigidBody)
	{
		m_pRigidBody->setRotation(m_pRigidBody->getRotation() * qOffset);
		m_pRigidBody->setPosition(qOffset * (m_pRigidBody->getPosition() - vOrigin) + vOrigin);
	}

	for(UINT i = 0, l = m_aVertices.size(); i < l; ++i)
	{
		m_aVertices[i] = (float3)(qOffset * (m_aVertices[i] - vOrigin) + vOrigin);
	}

	for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		Face &f = m_aFaces[i];

		f.texInfo.fSShift += SMVector3Dot(f.texInfo.vS, vOrigin);
		f.texInfo.fTShift += SMVector3Dot(f.texInfo.vT, vOrigin);

		f.vNormal = qOffset * f.vNormal;
		f.texInfo.vS = qOffset * f.texInfo.vS;
		f.texInfo.vT = qOffset * f.texInfo.vT;

		f.texInfo.fSShift -= SMVector3Dot(f.texInfo.vS, vOrigin);
		f.texInfo.fTShift -= SMVector3Dot(f.texInfo.vT, vOrigin);
	}


	m_isBoundDirty = true;


}

void CBrushMesh::resize(const float3 &vOrigin, const float3 &vRelativeSize)
{
	for(UINT i = 0, l = m_aVertices.size(); i < l; ++i)
	{
		m_aVertices[i] = (float3)(vRelativeSize * (m_aVertices[i] - vOrigin) + vOrigin);
	}

	//! TODO: add support for texture scale lock

	/*for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		Face &f = m_aFaces[i];
		f.texInfo.vS = (float3)(f.texInfo.vS / vRelativeSize);
		f.texInfo.vT = (float3)(f.texInfo.vT / vRelativeSize);
	}*/

	buildModel();

	m_isBoundDirty = true;
}

bool CBrushMesh::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, float3 *pvNormal, bool isRayInWorldSpace, bool bReturnNearestPoint)
{
	if(m_pModel)
	{
		return(m_pModel->rayTest(vStart, vEnd, pvOut, pvNormal, isRayInWorldSpace, bReturnNearestPoint));
	}
	assert(!"Check me!");
	return(false);
}

void CBrushMesh::renderSelection(bool is3D, IXGizmoRenderer *pGizmoRenderer)
{
	pGizmoRenderer->setColor(float4(1.0f, 1.0f, 0.0f, 1.0f));
	pGizmoRenderer->setLineWidth(is3D ? 0.02f : 1.0f);
	for(UINT i = 0, l = m_aEdges.size(); i < l; ++i)
	{
		Edge &e = m_aEdges[i];
		if(!e.isInternal)
		{
			pGizmoRenderer->jumpTo(m_aVertices[e.uVertex[0]]);
			pGizmoRenderer->lineTo(m_aVertices[e.uVertex[1]]);
		}
	}

	if(is3D)
	{
		pGizmoRenderer->setColor(float4(1.0f, 0.0f, 0.0f, 0.15f));
		// generate polygons
		for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
		{
			Face &face = m_aFaces[i];

			bool isFirst = true;
			UINT uLastVtx = UINT_MAX;

			float3 vA, vB, vC, vS;

			vS = face.vNormal * 0.005f;

			Edge &e0 = m_aEdges[face.aEdges[0]];
			Edge &e1 = m_aEdges[face.aEdges[1]];
			if(e0.uVertex[0] == e1.uVertex[0] || e0.uVertex[0] == e1.uVertex[1])
			{
				std::swap(e0.uVertex[0], e0.uVertex[1]);
			}

			for(UINT j = 0, jl = face.aEdges.size() - 1; j < jl; ++j)
			{
				Edge edge = m_aEdges[face.aEdges[j]];
				if(edge.uVertex[0] != uLastVtx && uLastVtx != UINT_MAX)
				{
					std::swap(edge.uVertex[0], edge.uVertex[1]);
				}
				uLastVtx = edge.uVertex[1];

				if(isFirst)
				{
					isFirst = false;

					vA = m_aVertices[edge.uVertex[0]] + vS;
					vB = m_aVertices[edge.uVertex[1]] + vS;
				}
				else
				{
					vC = m_aVertices[edge.uVertex[1]] + vS;
					pGizmoRenderer->drawPoly(vA, vB, vC);
					vB = vC;
				}
			}
		}
	}
}

void CBrushMesh::serialize(Array<char> *paData)
{
	char tmp[256];
	paData->append("{\"v\":[");
	for(UINT i = 0, l = m_aVertices.size(); i < l; ++i)
	{
		const float3_t &v = m_aVertices[i];
		sprintf(tmp, "%s[%g,%g,%g]", i == 0 ? "" : ",", v.x, v.y, v.z);
		paData->append(tmp);
	}
	paData->append("],\"e\":[");
	for(UINT i = 0, l = m_aEdges.size(); i < l; ++i)
	{
		const Edge &e = m_aEdges[i];
		sprintf(tmp, "%s[%u,%u,%u]", i == 0 ? "" : ",", e.uVertex[0], e.uVertex[1], e.isInternal ? 1 : 0);
		paData->append(tmp);
	}
	paData->append("],\"f\":[");
	for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		const Face &f = m_aFaces[i];
		const TextureInfo &t = f.texInfo;
		if(i != 0)
		{
			paData->push_back(',');
		}
		paData->append("{\"t\":{");
		sprintf(tmp, "\"s\":[%g,%g,%g,%g],\"t\":[%g,%g,%g,%g],\"m\":%u", 
			t.vS.x, t.vS.y, t.vS.z, t.fSShift,
			t.vT.x, t.vT.y, t.vT.z, t.fTShift,
			t.uMatId
			);
		paData->append(tmp);
		paData->append("},\"e\":[");
		for(UINT j = 0, jl = f.aEdges.size(); j < jl; ++j)
		{
			sprintf(tmp, "%s%u", j == 0 ? "" : ",", f.aEdges[j]);
			paData->append(tmp);
		}
		paData->append("],\"n\":[");
		sprintf(tmp, "%g,%g,%g", f.vNormal.x, f.vNormal.y, f.vNormal.z);
		paData->append(tmp);
		paData->append("]}");

		// sprintf(tmp, "%s[%u,%u,%u]", i == 0 ? "" : ",", e.uVertex[0], e.uVertex[1], e.isInternal ? 1 : 0);
		// paData->append(tmp);
	}
	paData->append("],\"m\":[");
	for(UINT i = 0, l = m_aMaterials.size(); i < l; ++i)
	{
		if(i != 0)
		{
			paData->push_back(',');
		}
		paData->push_back('"');
		paData->append(m_aMaterials[i].c_str());
		paData->push_back('"');
	}
	paData->append("]");
	paData->append("}");
	// m_aVertices
}

bool CBrushMesh::deserialize(IXJSONObject *pData)
{
	assert(pData);
	if(!pData)
	{
		return(false);
	}

	IXJSONArray *pArr, *pVec;
	IXJSONObject *pFace, *pTI;
	int64_t a, b, c;

	for(UINT i = 0, l = pData->size(); i < l; ++i)
	{
		const char *pKey = pData->getKey(i);
		switch(pKey[0])
		{
		case 'v':
			pArr = pData->at(i)->asArray();
			if(pArr)
			{
				m_aVertices.resize(pArr->size());
				for(UINT j = 0, jl = pArr->size(); j < jl; ++j)
				{
					pVec = pArr->at(j)->asArray();
					if(!pVec || pVec->size() != 3)
					{
						return(false);
					}

					float3_t &vec = m_aVertices[j];

					if(!(pVec->at(0)->getFloat(&vec.x) &&
						pVec->at(1)->getFloat(&vec.y) &&
						pVec->at(2)->getFloat(&vec.z)))
					{
						return(false);
					}
				}
			}
			break;
		case 'e':
			pArr = pData->at(i)->asArray();
			if(pArr)
			{
				m_aEdges.resize(pArr->size());
				for(UINT j = 0, jl = pArr->size(); j < jl; ++j)
				{
					pVec = pArr->at(j)->asArray();
					if(!pVec || pVec->size() != 3)
					{
						return(false);
					}

					Edge &e = m_aEdges[j];
					if(!(pVec->at(0)->getInt64(&a) &&
						pVec->at(1)->getInt64(&b) &&
						pVec->at(2)->getInt64(&c)))
					{
						return(false);
					}

					e.uVertex[0] = a;
					e.uVertex[1] = b;
					e.isInternal = c;
				}
			}
			break;
		case 'f':
			pArr = pData->at(i)->asArray();
			if(pArr)
			{
				m_aFaces.resize(pArr->size());
				for(UINT j = 0, jl = pArr->size(); j < jl; ++j)
				{
					pFace = pArr->at(j)->asObject();
					if(!pFace)
					{
						return(false);
					}
					Face &f = m_aFaces[j];

					for(UINT k = 0, kl = pFace->size(); k < kl; ++k)
					{
						pKey = pFace->getKey(k);
						switch(pKey[0])
						{
						case 't':
							// s t m
							pTI = pFace->at(k)->asObject();
							if(!pTI)
							{
								return(false);
							}

							for(UINT s = 0, sl = pTI->size(); s < sl; ++s)
							{
								pKey = pTI->getKey(s);
								switch(pKey[0])
								{
								case 's':
								case 't':
									pVec = pTI->at(s)->asArray();
									if(!pVec || pVec->size() != 4)
									{
										return(false);
									}

									{
										float3_t &vec = pKey[0] == 's' ? f.texInfo.vS : f.texInfo.vT;
										float &fShift = pKey[0] == 's' ? f.texInfo.fSShift : f.texInfo.fTShift;

										if(!(pVec->at(0)->getFloat(&vec.x) &&
											pVec->at(1)->getFloat(&vec.y) &&
											pVec->at(2)->getFloat(&vec.z) &&
											pVec->at(3)->getFloat(&fShift)))
										{
											return(false);
										}
									}

									break;
								case 'm':
									if(!pTI->at(s)->getInt64(&a))
									{
										return(false);
									}
									f.texInfo.uMatId = (UINT)a;
									break;
								}
							}

							break;

						case 'e':
							pVec = pFace->at(k)->asArray();
							if(!pVec || pVec->size() < 3)
							{
								return(false);
							}
							f.aEdges.resize(pVec->size());
							for(UINT s = 0, sl = pVec->size(); s < sl; ++s)
							{
								if(!pVec->at(s)->getInt64(&a))
								{
									return(false);
								}
								f.aEdges[s] = (UINT)a;
							}

							break;

						case 'n':
							pVec = pFace->at(k)->asArray();
							if(!pVec || pVec->size() != 3)
							{
								return(false);
							}

							float3_t &vec = f.vNormal;

							if(!(pVec->at(0)->getFloat(&vec.x) &&
								pVec->at(1)->getFloat(&vec.y) &&
								pVec->at(2)->getFloat(&vec.z)))
							{
								return(false);
							}
							break;
						}
					}
				}
			}
			break;
		case 'm':
			pArr = pData->at(i)->asArray();
			if(pArr)
			{
				m_aMaterials.resize(pArr->size());
				for(UINT j = 0, jl = pArr->size(); j < jl; ++j)
				{
					m_aMaterials[j] = pArr->at(j)->getString();
				}
			}
			break;
		}
	}

	buildModel();

	m_isBoundDirty = true;

	return(true);
}
