
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <core/sxcore.h>
#include <gcore/sxgcore.h>
#include <physics/sxphysics.h>

#include "DecalManager.h"

// Array<float3_t> g_dbgDraw;
// float4 spherePos;

DecalManager::DecalManager():
	m_bNeedUpdate(false),
	m_pVertexBuffer(NULL)
{
	//SkyXEngine::Core::Data::Pathes::Textures
	const char * szDecalsIniPath = "textures/decal/decals.ini";

	Core_0RegisterCVarInt("r_maxdecals", 300, "Max decals");
	Core_0RegisterCVarInt("r_max_overlapped_decals", 4, "Max overlapped decals");

	dev = SGCore_GetDXDevice();

	ISXConfig * config = Core_OpConfig(szDecalsIniPath);

	int sections = config->getSectionCount();
	const char * sect;

	for(int i = 0; i < sections; ++i)
	{
		sect = config->getSectionName(i);

		int id;
		if(config->keyExists(sect, "id") && sscanf(config->getKey(sect, "id"), "%d", &id))
		{
			if(id < 0 || id >= DECAL_TYPE_LAST)
			{
				LibReport(REPORT_MSG_LEVEL_WARNING, "Incorrect decal type id '%s'\n", sect);
			}
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to read decal id '%s'\n", sect);
			continue;
		}
		const char * tex;
		if(config->keyExists(sect, "tex"))
		{
			tex = config->getKey(sect, "tex");
		}
		else
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to read decal tex '%s'\n", sect);
			continue;
		}
		if(config->keyExists(sect, "base_scale"))
		{
			if(!sscanf(config->getKey(sect, "base_scale"), "%f", &m_DecalTypes[id].m_fBaseScale))
			{
				LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to read decal base_scale '%s'\n", sect);
				m_DecalTypes[id].m_fBaseScale = 1.0f;
			}
		}
		else
		{
			m_DecalTypes[id].m_fBaseScale = 1.0f;
		}
		int j = 0;

		DecalTexRange rng;
		char key[64];
		while(sprintf(key, "tex%d", j) && config->keyExists(sect, key))
		{
			if(sscanf(config->getKey(sect, key), "[%d,%d,%d,%d]", &rng.xmin, &rng.ymin, &rng.xmax, &rng.ymax) != 4)
			{
				printf("Unable to read decal tex coords \"%s\" \"%s\"\n", config->getKey(sect, key), sect);
			}
			else
			{
				m_DecalTypes[id].m_texs.push_back(rng);
			}
			j++;
		}
		m_DecalTypes[id].m_MaterialId = SGCore_MtlLoad(tex, MTL_TYPE_GEOM);
	}
	
	mem_release(config);
}

DecalManager::~DecalManager()
{

}

int DecalManager::addDecal(Decal * pDecal)
{
	static const int * MAX_DECALS = GET_PCVAR_INT("r_maxdecals");
	static const int * DECAL_MAX_OVERLAPPED = GET_PCVAR_INT("r_max_overlapped_decals");
	int numdecal = 0;
	//printf("addDecal(); bgn; c = %d\n", m_vDecals.size());
	if(m_vDecals.size() >= (UINT)*MAX_DECALS)
	{
		for(UINT i = 0, l = m_vDecals.size(); i < l; ++i)
		{
			if(!(m_vDecals[i]->m_flags & DECAL_PERMANENT))
			{
				removeDecal(i);
				break;
			}
		}
	}

	numdecal = m_vDecals.size();
	m_vDecals.push_back(pDecal);
	//ArrPerSound[numdecal]->render();
	static UINT iDecalId = 0;
	_DecalMatItem dmi;
	dmi.m_pDecal = pDecal;
	dmi.iDecalId = iDecalId++;

	UINT iOverlappedCount = 0;
	UINT iFirstOverlpd = -1;
	for(UINT i = 0, l = m_vDecals.size(); i < l; ++i)
	{
		if(SMVector3Length2(m_vDecals[i]->m_vPos - pDecal->m_vPos) < (pDecal->m_Size + m_vDecals[i]->m_Size))
		{
			if(iFirstOverlpd == -1 && !(m_vDecals[i]->m_flags & DECAL_PERMANENT))
			{
				iFirstOverlpd = i;
			}
			iOverlappedCount++;
			if(iOverlappedCount > (UINT)*DECAL_MAX_OVERLAPPED)
			{
				//printf("addDecal(); rmv\n");
				removeDecal(iFirstOverlpd);
				break;
			}
		}
	}
	//pDecal->iMatSortId = m_MaterialSort[pDecal->m_material].size();
	m_MaterialSort[pDecal->m_material].push_back(dmi);
	//printf("addDecal(); end; c = %d\n", m_vDecals.size());
	m_bNeedUpdate = true;
	return numdecal;
}

void DecalManager::removeDecal(UINT iDecal)
{
	//printf("removeDecal(); bgn; c = %d\n", m_vDecals.size());
	if(iDecal >= m_vDecals.size())
	{
		return;
	}
	for(UINT i = 0, l = m_MaterialSort[m_vDecals[iDecal]->m_material].size(); i < l; ++i)
	{
		if(m_MaterialSort[m_vDecals[iDecal]->m_material][i].m_pDecal == m_vDecals[iDecal])
		{
			m_MaterialSort[m_vDecals[iDecal]->m_material].erase(i);
			break;
		}
	}
	//m_MaterialSort[m_vDecals[iDecal]->m_material].erase(m_vDecals[iDecal]->iMatSortId);
	mem_delete_a(m_vDecals[iDecal]->m_pVerts);
	m_aDecals.Delete(m_vDecals[iDecal]);
	m_vDecals.erase(iDecal);
	m_bNeedUpdate = true;
	//printf("removeDecal(); end; c = %d\n", m_vDecals.size());
}

bool DecalManager::inside(const float3_t * p, char axis, float coord)
{
	switch(axis)
	{
	case 's':
		return(coord > 0.0f ? p->x < coord : p->x > coord);
	case 't':
		return(coord > 0.0f ? p->y < coord : p->y > coord);
	}
	return(false);
}

void DecalManager::intersect(const float3_t * one, const float3_t * two, float3_t * out, char axis, float coord)
{
	float t;
	switch(axis)
	{
	case 's':
		t = (coord - two->x) / (one->x - two->x);
		break;
	case 't':
		t = (coord - two->y) / (one->y - two->y);
		break;
	}

	*out = (float3)SMVectorLerp(*two, *one, t);
}

void DecalManager::clip(const Array<float3_t> & InVerts, Array<float3_t> & OutVerts, char axis, float coord)
{
	OutVerts.clear();
	if(!InVerts.size())
	{
		return;
	}

	int j, outCount, vertCount;
	const float3_t *s, *p;
	float3_t out;

	outCount = 0;
	vertCount = InVerts.size();

	s = &InVerts[vertCount - 1];
	for(j = 0; j < vertCount; j++)
	{
		p = &InVerts[j];
		if(inside(p, axis, coord))
		{
			if(inside(s, axis, coord))
			{
				OutVerts.push_back(*p);
				outCount++;
			}
			else
			{
				intersect(s, p, &out, axis, coord);
				OutVerts.push_back(out);
				outCount++;

				OutVerts.push_back(*p);
				outCount++;
			}
		}
		else
		{
			if(inside(s, axis, coord))
			{
				intersect(p, s, &out, axis, coord);
				OutVerts.push_back(out);
				outCount++;
			}
		}
		s = p;
	}

	//return outCount;
}

const DecalType * DecalManager::getDecalType(DECAL_TYPE type)
{
	if(type < 0 || type >= DECAL_TYPE_LAST)
	{
		printf("Incorrect decal type %d\n", type);
		return(NULL);
	}
	return(&m_DecalTypes[type]);
}

void DecalManager::shootDecal(DECAL_TYPE type, const float3 & position, ID iMaterial, int iEnt, const float3 * saxis, float scale, int flags, const float3 * _normal)
{
	// g_dbgDraw.clear();
	//m_dbgRender.push_back(position);
	//float3 tmp;
	//SkyXEngine::Core::Data::ObjCamera->GetPosition(&tmp);
	//m_dbgRender.push_back(tmp);

	DecalInfo di;
	di.m_Position = position;

	if(saxis)
	{
		flags |= DECAL_USESAXIS;
		di.m_SAxis = *saxis;
	}

	float3 unormal(0.0f, 1.0f, 0.0f);

	if(!_normal)
	{
		flags &= ~DECAL_ONE_SIDED;
	}
	else
	{
		unormal = SMVector3Normalize(*_normal);
	}

	di.m_iMaterial = iMaterial;

	if(!(flags & DECAL_NOCLIP))
	{
		flags |= DECAL_CLIPTEST;
	}

	di.m_iEnt = iEnt;
	di.m_Flags = flags;
	
	ID pMat = -1;
	IDirect3DTexture9 * pTex = NULL;
	const DecalType * dt = NULL;
	if(type == DECAL_TYPE_CUSTOM)
	{
		pMat = iMaterial;
	}
	else
	{
		dt = getDecalType(type);
		if(!dt)
		{
			return;
		}
		scale *= dt->m_fBaseScale;
		pMat = dt->m_MaterialId;
	}
	if(pMat < 0)
	{
		return;
	}

	pTex = SGCore_LoadTexGetTex(SML_MtlGetTextureID(pMat));
	if(!pTex)
	{
		return;
	}

	D3DSURFACE_DESC _info;
	pTex->GetLevelDesc(0, &_info);

	di.m_Size = (float)(_info.Width >> 1);
	if((int)(_info.Height >> 1) > di.m_Size)
	{
		di.m_Size = (float)(_info.Height >> 1);
	}

	scale *= 0.0008f;

	di.m_scale = 1.0f / scale;
	di.m_Size *= scale;

	DecalTexRange rng = {0, 0, _info.Width, _info.Height};

	//Get random decal from atlas
	if(dt)
	{
		rng = dt->m_texs[rand() % dt->m_texs.size()];
	}

	di.m_decalWidth = (float)(rng.xmax - rng.xmin) / di.m_scale;
	di.m_decalHeight = (float)(rng.ymax - rng.ymin) / di.m_scale;


	float2 sBound(-di.m_decalWidth * 0.5f, di.m_decalWidth * 0.5f);
	float2 tBound(-di.m_decalHeight * 0.5f, di.m_decalHeight * 0.5f);

	//R_DecalNode( pnodes, &decalInfo );

	Array<float3_t> vClippedVerts;
	//Array<UINT> vSplitIDs;
	Array<float3_t> vClippedVerts2;

	Array<DecalVertex> vDecalVerts;
	
	DecalVertex vert;
	DecalVertex vert0;

	Decal * decal = m_aDecals.Alloc();
	decal->m_flags = di.m_Flags;
	decal->m_fScale = di.m_scale;
	decal->m_iEntity = iEnt;
	if(dt)
	{
		decal->m_material = dt->m_MaterialId;
	}
	else
	{
		decal->m_material = iMaterial;
	}
	decal->m_vPos = position;
	if(saxis)
	{
		decal->m_vSAxis = *saxis;
	}
	float r = sqrtf(decal->m_Size = ((sBound.x - sBound.y) * (sBound.x - sBound.y) + (tBound.x - tBound.y) * (tBound.x - tBound.y)));

	{
		struct	AllConvexResultCallback: public btDiscreteDynamicsWorld::ConvexResultCallback
		{
			struct part
			{
				const btCollisionObject * m_hitCollisionObject;
				btDiscreteDynamicsWorld::LocalShapeInfo si;
			};

			AllConvexResultCallback(const btVector3&	rayFromWorld, const btVector3&	rayToWorld)
			:m_rayFromWorld(rayFromWorld),
			m_rayToWorld(rayToWorld)
			{
			}

			Array<part> m_hitParts;

			btVector3	m_rayFromWorld;//used to calculate hitPointWorld from hitFraction
			btVector3	m_rayToWorld;
			
			virtual	btScalar addSingleResult(btDiscreteDynamicsWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
			{
				if(convexResult.m_localShapeInfo)
				{
					m_closestHitFraction = 0;
					m_hitParts.push_back({convexResult.m_hitCollisionObject, *convexResult.m_localShapeInfo});
				}
				return(m_closestHitFraction);
			}
		};

		float3 n;
		float3 tsBasis[3];
		SMMATRIX mBasis;
		SMMATRIX mBasisInv;
		float det;

		// spherePos = position;
		// spherePos.w = r;
		btVector3 vec(F3_BTVEC(position));
		AllConvexResultCallback cb(vec, vec);
		btSphereShape sphere(r);
		btTransform xForm;
		btVector3 offs = F3_BTVEC(unormal) * 0.01f;
		xForm.setOrigin(vec - offs);
		xForm.getBasis().setIdentity();
		btTransform xForm2;
		xForm2.setOrigin(vec + offs);
		xForm2.getBasis().setIdentity();
		SXPhysics_GetDynWorld()->convexSweepTest(&sphere, xForm, xForm2, cb);
		AllConvexResultCallback::part * part;
		if(cb.hasHit())
		{
			for(int i = 0, l = cb.m_hitParts.size(); i < l; ++i)
			{
				part = &cb.m_hitParts[i];
				if(part->m_hitCollisionObject->getCollisionShape()->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
				{
					btTriangleMeshShape * shape = (btTriangleMeshShape*)part->m_hitCollisionObject->getCollisionShape();
					btStridingMeshInterface * iface = shape->getMeshInterface();

					float3 * verices;
					int numverts;
					PHY_ScalarType type = PHY_INTEGER;
					int stride = 0;
					UINT * indexbase = 0;
					int indexstride = 0;
					int numfaces = 0;
					PHY_ScalarType indextype = PHY_INTEGER;

					iface->getLockedReadOnlyVertexIndexBase((const unsigned char **)&verices, numverts, type, stride, (const unsigned char **)&indexbase, indexstride, numfaces, indextype, part->si.m_shapePart);

					UINT idx = part->si.m_triangleIndex * 3;
					
					float3 a = verices[indexbase[idx]];
					float3 b = verices[indexbase[idx + 1]];
					float3 c = verices[indexbase[idx + 2]];

					// g_dbgDraw.push_back(a);
					// g_dbgDraw.push_back(b);
					// g_dbgDraw.push_back(c);

					iface->unLockReadOnlyVertexBase(part->si.m_shapePart);

					{
						n = SMVector3Normalize(SMVector3Cross(c - b, a - b));
						if(flags & DECAL_ONE_SIDED)
						{
							if(SMVector3Dot(n, unormal) < -0.1f)
							{
								continue;
							}
						}

						computeBasis(n, (di.m_Flags & DECAL_USESAXIS) ? &(float3)di.m_SAxis : NULL, tsBasis);

						mBasis.r[0] = float4(float3(tsBasis[0]), 0.0f);
						mBasis.r[1] = float4(float3(tsBasis[1]), 0.0f);
						mBasis.r[2] = float4(float3(tsBasis[2]), 0.0f);
						mBasis.r[3] = float4(float3(position), 1.0f);

						mBasisInv = SMMatrixInverse(&det, mBasis);

						//float3 x0(0, 0, 0);
						//float3 x1(1, 1, 1);

						vClippedVerts.clear();

						//Transform to new basis {S, T, N}
						vClippedVerts.push_back(a * mBasisInv);
						vClippedVerts.push_back(b * mBasisInv);
						vClippedVerts.push_back(c * mBasisInv);

						float nn = vClippedVerts[0].z + 0.002f;

						//Clip decal by triangle in {S, T} plane
						clip(vClippedVerts, vClippedVerts2, 's', sBound.y);
						clip(vClippedVerts2, vClippedVerts, 's', sBound.x);
						clip(vClippedVerts, vClippedVerts2, 't', tBound.y);
						clip(vClippedVerts2, vClippedVerts, 't', tBound.x);

						/*for(int ijk = 0; ijk < 3; ijk++)
						{
						m_dbgRender.push_back(position);
						m_dbgRender.push_back((float3)(position + tsBasis[ijk]));
						}*/
						int len = vClippedVerts.size();
						if(len < 3)
						{
							continue;
						}
						vert0.pos = mBasis * vClippedVerts[0];
						vert0.normal = n;
						vert0.tex = float2((vClippedVerts[0].x - sBound.x) / (sBound.y - sBound.x), (vClippedVerts[0].y - tBound.x) / (tBound.y - tBound.x));
						vert0.tex = (float2)(vert0.tex * float2((float)(rng.xmax - rng.xmin) / (float)_info.Width, (float)(rng.ymax - rng.ymin) / (float)_info.Height) + float2((float)rng.xmin / (float)_info.Width, (float)rng.ymin / (float)_info.Height));
						for(int ii = 1; ii < len - 1; ii++)
						{
							vClippedVerts[ii].z = nn; // fix normal
							vClippedVerts[ii + 1].z = nn; // fix normal
							//Transform to World
							vDecalVerts.push_back(vert0);
							//m_dbgRender.push_back(vert0.pos);

							vert.pos = mBasis * vClippedVerts[ii];
							vert.normal = n;
							vert.tex = float2((vClippedVerts[ii].x - sBound.x) / (sBound.y - sBound.x), (vClippedVerts[ii].y - tBound.x) / (tBound.y - tBound.x));
							vert.tex = (float2)(vert.tex * float2((float)(rng.xmax - rng.xmin) / (float)_info.Width, (float)(rng.ymax - rng.ymin) / (float)_info.Height) + float2((float)rng.xmin / (float)_info.Width, (float)rng.ymin / (float)_info.Height));

							vDecalVerts.push_back(vert);
							//m_dbgRender.push_back(vert.pos);

							vert.pos = mBasis * vClippedVerts[ii + 1];
							vert.tex = float2((vClippedVerts[ii + 1].x - sBound.x) / (sBound.y - sBound.x), (vClippedVerts[ii + 1].y - tBound.x) / (tBound.y - tBound.x));
							vert.tex = (float2)(vert.tex * float2((float)(rng.xmax - rng.xmin) / (float)_info.Width, (float)(rng.ymax - rng.ymin) / (float)_info.Height) + float2((float)rng.xmin / (float)_info.Width, (float)rng.ymin / (float)_info.Height));
							vDecalVerts.push_back(vert);
							//m_dbgRender.push_back(vert.pos);

							//m_dbgRender.push_back(mBasis * vClippedVerts[ii]);
							//m_dbgRender.push_back(mBasis * vClippedVerts[(ii + 1) % len]);

						}
					}
				}
			}
		}
	}

	if(vDecalVerts.size())
	{
		decal->iVertCount = vDecalVerts.size();
		decal->m_pVerts = new DecalVertex[decal->iVertCount];
		memcpy(decal->m_pVerts, &vDecalVerts[0], sizeof(DecalVertex) * vDecalVerts.size());

		int numdecal = addDecal(decal);
	}
}

void DecalManager::render()
{
	/*for(int i = 0, l = m_dbgRender.size(); i < l; i += 3)
	{
		SXPhysics_GetDynWorld()->getDebugDrawer()->drawTriangle(F3_BTVEC(m_dbgRender[i]), F3_BTVEC(m_dbgRender[i + 1]), F3_BTVEC(m_dbgRender[i + 2]), btVector3(1.0f, 1.0f, 1.0f), 1.0f);
	}*/
	//SXPhysics_GetDynWorld()->getDebugDrawer()->drawSphere(F3_BTVEC(spherePos), spherePos.w, btVector3(1, 1, 1));
	
	updateBuffer();

	if(!m_pVertexBuffer)
	{
		return;
	}

	dev->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);


//	SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
//	SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)40);
//	SkyXEngine::Core::Data::Device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);

//	SkyXEngine::Core::Data::Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	SkyXEngine::Core::Data::Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
//	SkyXEngine::Core::Data::Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//	SkyXEngine::Core::Data::Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	/*for(int i = 0; i < m_vDecals.size(); i++)
	{
		SkyXEngine::Core::Data::MaterialsManager->Render(m_vDecals[i]->m_material, &SMMatrixIdentity(), 0);
		SkyXEngine::Core::Data::Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vDecals[i]->iVertCount / 3, m_vDecals[i]->m_pVerts, sizeof(DecalVertex));
	}*/
	dev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(DecalVertex));
	for(UINT i = 0; i < m_iRngs.size(); i++)
	{
		SGCore_MtlSet(m_iRngs[i].iMaterialId, &SMMatrixIdentity());
		dev->DrawPrimitive(D3DPT_TRIANGLELIST, m_iRngs[i].iStartVertex, m_iRngs[i].iVertexCount / 3);
	}

	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void DecalManager::updateBuffer()
{
	if(!m_bNeedUpdate)
	{
		return;
	}

	UINT iVC = 0;
	UINT iCC = 0;

	_DecalTexRange rng;
	m_iRngs.clear();
	for(AssotiativeArray<ID, Array<_DecalMatItem>>::Iterator i = m_MaterialSort.begin(); i; i++)
	{
		for(int j = 0, lj = i.second->size(); j < lj; ++j)
		{
			iCC += i.second[0][j].m_pDecal->iVertCount;
		}
		if(iCC != 0)
		{
			rng.iMaterialId = *(i.first);
			rng.iStartVertex = iVC;
			rng.iVertexCount = iCC;
			m_iRngs.push_back(rng);
			iVC += iCC;
			iCC = 0;
		}
	}

	

	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = NULL;
	}

	if(iVC == 0)
	{
		return;
	}

	dev->CreateVertexBuffer(sizeof(DecalVertex)* iVC, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_MANAGED, &m_pVertexBuffer, 0);
	DecalVertex * pData;
	if(!FAILED(m_pVertexBuffer->Lock(0, sizeof(DecalVertex) * iVC, (void**)&pData, 0)))
	{
		iVC = 0;
		for(AssotiativeArray<ID, Array<_DecalMatItem>>::Iterator i = m_MaterialSort.begin(); i; i++)
		{
			for(UINT j = 0, l = i.second->size(); j < l; ++j)
			{
				//iVC += i.second[0][j].m_pDecal->iVertCount;
				//iCC += i.second[0][j].m_pDecal->iVertCount;

				memcpy(pData + iVC, i.second[0][j].m_pDecal->m_pVerts, sizeof(DecalVertex) * i.second[0][j].m_pDecal->iVertCount);
				iVC += i.second[0][j].m_pDecal->iVertCount;
			}
		}
		m_pVertexBuffer->Unlock();
	}
	//m_pVertexBuffer

	m_bNeedUpdate = false;
}


void DecalManager::update()
{
	//empty
}

void DecalManager::sync()
{
	//empty
}
