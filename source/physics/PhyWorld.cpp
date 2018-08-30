
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PhyWorld.h"
#include <core/sxcore.h>
#include <geom/sxgeom.h>
#include <green/sxgreen.h>
#include <gcore/sxgcore.h>
#include <light/sxlight.h>


#include <../Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.h>

CPhyWorld::CPhyWorld():
	m_pGeomStaticCollideMesh(NULL),
	m_pGeomStaticCollideShape(NULL),
	m_pGeomStaticRigidBody(NULL),
	m_pGeomMtlIDs(0),
	m_iGeomFacesCount(0),
	m_iGeomModelCount(0),
	m_ppGreenStaticCollideShape(NULL),
	m_pppGreenStaticRigidBody(NULL),
	m_iGreenShapes(0),
	m_piGreenTotal(NULL),
	m_isRunning(false)
{
	printf("Initializing physics engine...   ");
	m_pBroadphase = new btDbvtBroadphase();
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	m_pSolver = new btSequentialImpulseConstraintSolver;
	m_pGHostPairCallback = new btGhostPairCallback();
	m_pBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(m_pGHostPairCallback);
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
	
	Core_0RegisterCVarString("phy_world_gravity", "0 -10 0", "World gravity (x y z)");
	m_pDynamicsWorld->setGravity(btVector3(0, -10, 0));
			
	m_pDebugDrawer = new CDebugDrawer();
	m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	//m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_FastWireframe);
	m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);

	Core_0RegisterCVarBool("r_physdebug", false, "Debug drawing physics shapes");
	m_bDebugDraw = GET_PCVAR_BOOL("r_physdebug");
	printf("Done!\n");
}

CPhyWorld::~CPhyWorld()
{
	unloadGeom();

	mem_delete(m_pDynamicsWorld);
	mem_delete(m_pGHostPairCallback);
	mem_delete(m_pSolver);
	mem_delete(m_pDispatcher);
	mem_delete(m_pCollisionConfiguration);
	mem_delete(m_pBroadphase);
}

void CPhyWorld::render()
{
	if(*m_bDebugDraw)
	{
		m_pDynamicsWorld->debugDrawWorld();
		((CDebugDrawer*)(m_pDynamicsWorld->getDebugDrawer()))->render();
	}
}

void CPhyWorld::setThreadNum(int tnum)
{
}
void CPhyWorld::update(int thread)
{
	if(!m_isRunning)
	{
		return;
	}

	static UINT time0 = GetTickCount();
	UINT time1 = GetTickCount();

	if(time1 - time0 > 5000)
	{
		time0 = time1;
	}
	m_pDynamicsWorld->stepSimulation((float)(time1 - time0) / 1000.0f, 0, 1.0f / 60.0f);

	time0 = time1;
}
void CPhyWorld::sync()
{
}

void CPhyWorld::addShape(btRigidBody * pBody)
{
	m_pDynamicsWorld->addRigidBody(pBody);
}

void CPhyWorld::addShape(btRigidBody * pBody, int group, int mask)
{
	m_pDynamicsWorld->addRigidBody(pBody, group, mask);
}

void CPhyWorld::removeShape(btRigidBody * pBody)
{
	if(pBody)
	{
		m_pDynamicsWorld->removeRigidBody(pBody);
	}
}

void CPhyWorld::loadGeom(const char * file)
{
	if(file && importGeom(file))
	{
		return;
	}

	unloadGeom();

	float3_t ** ppVertices;
	int32_t * pVertexCount;
	uint32_t ** ppIndices;
	ID ** ppMtls;
	int32_t * pIndexCount;

	int32_t iModelCount;

	SGeom_ModelsGetArrBuffsGeom(&ppVertices, &pVertexCount, &ppIndices, &ppMtls, &pIndexCount, &iModelCount);
	if(iModelCount > 0)
	{
		m_iGeomFacesCount = 0;
		for(int32_t tc = 0; tc < iModelCount; ++tc)
		{
			m_iGeomFacesCount += pIndexCount[tc] / 3;
		}

		m_pGeomMtlIDs = new ID[m_iGeomFacesCount];
		m_iGeomModelCount = iModelCount;
		m_pGeomStaticCollideMesh = new btTriangleMesh(true, false);

		uint32_t IC = 0, VC = 0;
		for(int32_t tc = 0; tc < iModelCount; ++tc)
		{
			IC += pIndexCount[tc];
			VC += pVertexCount[tc];
		}

		m_pGeomStaticCollideMesh->preallocateIndices(IC);
		m_pGeomStaticCollideMesh->preallocateVertices(VC);

		

		IC = 0;
		VC = 0;
		int32_t iFace = 0;
		for(int32_t tc = 0; tc < iModelCount; ++tc)
		{
			for(int i = 0; i < pVertexCount[tc]; ++i)
			{
				m_pGeomStaticCollideMesh->findOrAddVertex(F3_BTVEC(ppVertices[tc][i]), false);
			}
			for(int i = 0; i < pIndexCount[tc]; i += 3)
			{
				//m_pGeomMtlTypes[iFace++] = SMtrl_MtlGetPhysicMaterial(ppMtls[tc][i]);
				m_pGeomMtlIDs[iFace++] = ppMtls[tc][i];
				m_pGeomStaticCollideMesh->addTriangleIndices(ppIndices[tc][i] + VC, ppIndices[tc][i + 1] + VC, ppIndices[tc][i + 2] + VC);
			}
			IC += pIndexCount[tc];
			VC += pVertexCount[tc];
		}

		if(m_pGeomStaticCollideMesh->getNumTriangles() != 0)
		{
			m_pGeomStaticCollideShape = new btBvhTriangleMeshShape(m_pGeomStaticCollideMesh, true);
			btDefaultMotionState motionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0f, 0.0f, 0.0f)));
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
				0,                  // mass
				&motionState,        // initial position
				m_pGeomStaticCollideShape,    // collision shape of body
				btVector3(0, 0, 0)  // local inertia
				);
			m_pGeomStaticRigidBody = new btRigidBody(rigidBodyCI);

			m_pGeomStaticRigidBody->setCollisionFlags(m_pGeomStaticRigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
			m_pGeomStaticRigidBody->setFriction(100.0f);

			addShape(m_pGeomStaticRigidBody);
		}
	}
	SGeom_ModelsClearArrBuffsGeom(ppVertices, pVertexCount, ppIndices, ppMtls, pIndexCount, iModelCount);





	float3_t** green_arr_vertex = 0;
	int32_t* green_arr_count_vertex = 0;
	uint32_t** green_arr_index = 0;
	ID** green_arr_mtl = 0;
	int32_t* green_arr_count_index = 0;
	CGreenDataVertex** green_arr_transform = 0;
	int32_t* green_arr_count_transform = 0;
	int32_t green_arr_count_green = 0;

	SGreen_GetNavMeshAndTransform(&green_arr_vertex, &green_arr_count_vertex, &green_arr_index, &green_arr_mtl, &green_arr_count_index, &green_arr_transform, &green_arr_count_transform, &green_arr_count_green);

	if(green_arr_count_green > 0)
	{
		m_ppGreenStaticCollideShape = new btCollisionShape*[green_arr_count_green];
		m_pppGreenStaticRigidBody = new btRigidBody**[green_arr_count_green];
		memset(m_pppGreenStaticRigidBody, 0, sizeof(btRigidBody**)* green_arr_count_green);
		m_piGreenTotal = new int[green_arr_count_green];
		memset(m_piGreenTotal, 0, sizeof(int)* green_arr_count_green);
		m_iGreenShapes = green_arr_count_green;
		for(int32_t num_green = 0; num_green < green_arr_count_green; ++num_green)
		{
			int iIC = 0;
			for(int i = 0; i < green_arr_count_index[num_green]; i += 3)
			{
				MTLTYPE_PHYSIC type = SMtrl_MtlGetPhysicMaterial(green_arr_mtl[num_green][i]);
				if (type != MTLTYPE_PHYSIC_LEAF_GRASS)
				{
					iIC += 3;
				}
			}

			MTLTYPE_PHYSIC type;

			{
				Array<UINT> idxs;
				idxs.reserve(iIC);
				UINT idx;
				bool found;
				for(int i = 0; i < green_arr_count_index[num_green]; ++i)
				{
					idx = green_arr_index[num_green][i];
					type = SMtrl_MtlGetPhysicMaterial(green_arr_mtl[num_green][idx]);
					if (type != MTLTYPE_PHYSIC_LEAF_GRASS)
					{
						found = false;
						for(int j = 0, lj = idxs.size(); j < lj; ++j)
						{
							if(idxs[j] == idx)
							{
								found = true;
								break;
							}
						}
						if(!found)
						{
							idxs.push_back(idx);
						}
					}
				}
			}

			Array<UINT> queue;
			float3 a, b, c, x, y, z;
			for(int i = 0; i < green_arr_count_index[num_green]; i += 3)
			{
				type = SMtrl_MtlGetPhysicMaterial(green_arr_mtl[num_green][i]);
				if (type == MTLTYPE_PHYSIC_LEAF_GRASS)
				{
					continue;
				}
				queue.push_back(i);
			}

			Array<bool> tripending;
			tripending.reserve(iIC / 3);
			for(int i = 0, l = iIC / 3; i < l; ++i)
			{
				tripending[i] = true;
			}

			btCompoundShape * cshape = new btCompoundShape(false);
			m_ppGreenStaticCollideShape[num_green] = cshape;
			float3 na, nb, nc;
			bool end = false;
			while(!end)
			{
				Array<bool> tmptri = tripending;
				end = true;
				btConvexHullShape * shape = new btConvexHullShape();
				//btTriangleMesh * trishape = new btTriangleMesh(true, false);
				for(int i = 0, l = tmptri.size(); i < l; ++i)
				{
					if(!tmptri[i])
					{
						continue;
					}
					end = false;

					a = green_arr_vertex[num_green][green_arr_index[num_green][queue[i]]];
					b = green_arr_vertex[num_green][green_arr_index[num_green][queue[i] + 1]];
					c = green_arr_vertex[num_green][green_arr_index[num_green][queue[i] + 2]];

					float3 normal = -SMVector3Normalize(SMVector3Cross(c - a, b - a));
					for(int j = 0; j < l; ++j)
					{
						if(!tmptri[j] || i == j)
						{
							continue;
						}
						x = green_arr_vertex[num_green][green_arr_index[num_green][queue[j]]];
						y = green_arr_vertex[num_green][green_arr_index[num_green][queue[j] + 1]];
						z = green_arr_vertex[num_green][green_arr_index[num_green][queue[j] + 2]];

						float3 na = SMVector3Normalize(x - a);
						float3 nb = SMVector3Normalize(y - a);
						float3 nc = SMVector3Normalize(z - a);
						float da, db, dc;
						if((da = SMVector3Dot(normal, na)) > 0.1f || (db = SMVector3Dot(normal, nb)) > 0.1f || (dc = SMVector3Dot(normal, nc)) > 0.1f)
						{
							tmptri[j] = false;
						}
					}


					//break;
				}
				for(int i = 0, l = tmptri.size(); i < l; ++i)
				{
					if(!tmptri[i])
					{
						continue;
					}

					a = green_arr_vertex[num_green][green_arr_index[num_green][queue[i]]];
					b = green_arr_vertex[num_green][green_arr_index[num_green][queue[i] + 1]];
					c = green_arr_vertex[num_green][green_arr_index[num_green][queue[i] + 2]];

					//++ccc;
					shape->addPoint(F3_BTVEC(a), false);
					shape->addPoint(F3_BTVEC(b), false);
					shape->addPoint(F3_BTVEC(c), false);

					tripending[i] = false;

					//trishape->addTriangle(F3_BTVEC(a), F3_BTVEC(b), F3_BTVEC(c));
					//trishape->addTriangle(F3_BTVEC(a), F3_BTVEC(a), F3_BTVEC(a + normal));
				}
				if(shape->getNumPoints())
				{
					static int co = 0;
					shape->recalcLocalAabb();
					btTransform tr;
					tr.setIdentity();
					//if(co++ == 0)
					{
						cshape->addChildShape(tr, shape);
						//cshape->addChildShape(tr, new btBvhTriangleMeshShape(trishape, true));
					}
				}
				//break;
			}


			if(cshape->getNumChildShapes())
			{
				m_piGreenTotal[num_green] = green_arr_count_transform[num_green];
				m_pppGreenStaticRigidBody[num_green] = new btRigidBody*[green_arr_count_transform[num_green]];
				for(int i = 0; i < green_arr_count_transform[num_green]; ++i)
				{
					btTransform tr;
					tr.setOrigin(F3_BTVEC(green_arr_transform[num_green][i].m_vPosition));
					tr.setRotation(Q4_BTQUAT(SMQuaternion(-green_arr_transform[num_green][i].m_vTexCoord.y, 'y')));
					btDefaultMotionState motionState(tr);
					btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
						0,                  // mass
						&motionState,        // initial position
						cshape,    // collision shape of body
						btVector3(0, 0, 0)  // local inertia
						);
					btRigidBody * body = m_pppGreenStaticRigidBody[num_green][i] = new btRigidBody(rigidBodyCI);

					//if(i > 10)
					body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
					body->setFriction(100.0f);

					addShape(body);
				}
			}
		}

	}
	SGreen_ClearNavMeshAndTransform(green_arr_vertex, green_arr_count_vertex, green_arr_index, green_arr_mtl, green_arr_count_index, green_arr_transform, green_arr_count_transform, green_arr_count_green);

	if(file)
	{
		exportGeom(file);
	}
}

void CPhyWorld::unloadGeom()
{
	for(int i = 0; i < m_iGreenShapes; ++i)
	{
		for(int j = 0; j < m_piGreenTotal[i]; ++j)
		{
			removeShape(m_pppGreenStaticRigidBody[i][j]);
			mem_delete(m_pppGreenStaticRigidBody[i][j]);
		}
		mem_delete_a(m_pppGreenStaticRigidBody[i]);

		btCompoundShape * cshape = (btCompoundShape*)m_ppGreenStaticCollideShape[i];

		while(cshape->getNumChildShapes())
		{
			btConvexHullShape * shape = (btConvexHullShape*)cshape->getChildShape(0);
			cshape->removeChildShapeByIndex(0);
			mem_delete(shape);
		}
		mem_delete(cshape);
	}
	m_iGreenShapes = 0;
	mem_delete_a(m_piGreenTotal);
	mem_delete_a(m_pppGreenStaticRigidBody);
	mem_delete_a(m_ppGreenStaticCollideShape);



	removeShape(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticCollideShape);
	mem_delete(m_pGeomStaticCollideMesh);

	m_iGeomModelCount = 0;
	m_iGeomFacesCount = 0;
	mem_delete_a(m_pGeomMtlIDs);
}

bool CPhyWorld::importGeom(const char * file)
{
	unloadGeom();
	
	int len = strlen(file) + 1;
	char * name = (char*)alloca(sizeof(char)*(len + 1));
	memcpy(name, file, sizeof(char)* len);
	name[len - 1] = 'm';
	name[len] = 0;

	if (!FileExistsFile(file) || !FileExistsFile(name))
	{
		return(false);
	}

	btBulletWorldImporter * importer = new btBulletWorldImporter(m_pDynamicsWorld);

	bool ret = importer->loadFile(file);
	if(ret)
	{
		m_pGeomStaticCollideShape = importer->getCollisionShapeByName("m_pGeomStaticCollideShape");
		m_pGeomStaticRigidBody = importer->getRigidBodyByName("m_pGeomStaticRigidBody");
		m_pGeomStaticRigidBody->setCollisionFlags(m_pGeomStaticRigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

		m_iGreenShapes = -1;
		char str[64];
		do
		{
			sprintf(str, "m_ppGreenStaticCollideShape[%d]", ++m_iGreenShapes);
		}
		while(importer->getCollisionShapeByName(str));
		if(m_iGreenShapes)
		{
			m_ppGreenStaticCollideShape = new btCollisionShape*[m_iGreenShapes];
			m_piGreenTotal = new int[m_iGreenShapes];
			m_pppGreenStaticRigidBody = new btRigidBody**[m_iGreenShapes];
			for(int i = 0; i < m_iGreenShapes; ++i)
			{
				sprintf(str, "m_ppGreenStaticCollideShape[%d]", i);
				m_ppGreenStaticCollideShape[i] = importer->getCollisionShapeByName(str);
				m_piGreenTotal[i] = -1;
				do
				{
					sprintf(str, "m_pppGreenStaticRigidBody[%d][%d]", i, ++m_piGreenTotal[i]);
				}
				while(importer->getRigidBodyByName(str));

				m_pppGreenStaticRigidBody[i] = new btRigidBody*[m_piGreenTotal[i]];

				for(int j = 0; j < m_piGreenTotal[i]; ++j)
				{
					sprintf(str, "m_pppGreenStaticRigidBody[%d][%d]", i, j);
					m_pppGreenStaticRigidBody[i][j] = importer->getRigidBodyByName(str);
					m_pppGreenStaticRigidBody[i][j]->setCollisionFlags(m_pppGreenStaticRigidBody[i][j]->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
				}
			}
		}

		FILE *pF = fopen(name, "rb");
		if(pF)
		{
			PhyMatFile pmf;
			if(fread(&pmf, sizeof(pmf), 1, pF))
			{
				if(pmf.i64Magick == PHY_MAT_FILE_MAGICK)
				{
					m_iGeomFacesCount = pmf.uiGeomFaceCount;

					char szTmp[1024];

					AssotiativeArray<String, ID> mMatMap;
					const AssotiativeArray<String, ID>::Node *pMatMapNode;
					int iMatCount = SMtrl_MtlGetCount();
					for(int i = 0; i < iMatCount; ++i)
					{
						szTmp[0] = 0;
						SMtrl_MtlGetTexture(i, szTmp);
						mMatMap[szTmp] = i;
					}

					ID *aidMatMap = (ID*)alloca(sizeof(ID) * pmf.uiMatCount);
					memset(aidMatMap, 0, sizeof(ID)* pmf.uiMatCount);

					uint16_t ui16NameLen;
					for(int i = 0; i < pmf.uiMatCount; ++i)
					{
						fread(&ui16NameLen, sizeof(uint16_t), 1, pF);
						if(ui16NameLen)
						{
							fread(szTmp, sizeof(char), ui16NameLen, pF);
							szTmp[ui16NameLen] = 0;

							if(mMatMap.KeyExists(szTmp, &pMatMapNode))
							{
								aidMatMap[i] = *pMatMapNode->Val;
							}
							else
							{
								ret = false;
							}
						}
					}

					m_pGeomMtlIDs = new ID[m_iGeomFacesCount];
					fread(m_pGeomMtlIDs, sizeof(ID), m_iGeomFacesCount, pF);
					for(int i = 0; i < m_iGeomFacesCount; ++i)
					{
						m_pGeomMtlIDs[i] = aidMatMap[m_pGeomMtlIDs[i]];
					}
				}
				else
				{
					ret = false;
				}
			}
			else
			{
				ret = false;
			}
			fclose(pF);
		}
		else
		{
			ret = false;
		}
	}
	mem_delete(importer);
	if(!ret)
	{
		unloadGeom();
	}
	return(ret);
}

const char * _allocStr(const char * _str)
{
	char * str = new char[strlen(_str) + 1];
	strcpy(str, _str);
	return(str);
}

bool CPhyWorld::exportGeom(const char * _file)
{
	btDefaultSerializer * serializer = new btDefaultSerializer();
	serializer->startSerialization();

	serializer->registerNameForPointer(m_pGeomStaticCollideShape, _allocStr("m_pGeomStaticCollideShape"));
	serializer->registerNameForPointer(m_pGeomStaticRigidBody, _allocStr("m_pGeomStaticRigidBody"));

	m_pGeomStaticCollideShape->serializeSingleShape(serializer);
	m_pGeomStaticRigidBody->serializeSingleObject(serializer);
	char str[64];
	for(int i = 0; i < m_iGreenShapes; ++i)
	{
		sprintf(str, "m_ppGreenStaticCollideShape[%d]", i);
		serializer->registerNameForPointer(m_ppGreenStaticCollideShape[i], _allocStr(str));
		m_ppGreenStaticCollideShape[i]->serializeSingleShape(serializer);
		for(int j = 0; j < m_piGreenTotal[i]; ++j)
		{
			sprintf(str, "m_pppGreenStaticRigidBody[%d][%d]", i, j);
			serializer->registerNameForPointer(m_pppGreenStaticRigidBody[i][j], _allocStr(str));
			m_pppGreenStaticRigidBody[i][j]->serializeSingleObject(serializer);
		}
	}
	serializer->finishSerialization();

	FILE * file = fopen(_file, "wb");
	bool ret = file
		&& fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file)
		&& !fclose(file);
	mem_delete(serializer);

	int len = strlen(_file)+1;
	char * name = (char*)alloca(sizeof(char)*(len + 1));
	memcpy(name, _file, sizeof(char)* len);
	name[len - 1] = 'm';
	name[len] = 0;

	file = fopen(name, "wb");
	if(!file)
	{
		ret = false;
	}
	else
	{
		PhyMatFile pmf;
		char szTmp[1024];
		pmf.uiGeomFaceCount = m_iGeomFacesCount;
		ID idMatMax = 0;
		for(int i = 0; i < m_iGeomFacesCount; ++i)
		{
			if(m_pGeomMtlIDs[i] > idMatMax)
			{
				idMatMax = m_pGeomMtlIDs[i];
			}
		}
		++idMatMax;

		pmf.uiMatCount = idMatMax;

		const char ** aszMatNames = (const char**)alloca(sizeof(char*) * idMatMax);
		memset(aszMatNames, 0, sizeof(char*)* idMatMax);

		for(int i = 0; i < m_iGeomFacesCount; ++i)
		{
			if(!aszMatNames[m_pGeomMtlIDs[i]])
			{
				szTmp[0] = 0;
				SMtrl_MtlGetTexture(m_pGeomMtlIDs[i], szTmp);
				aszMatNames[m_pGeomMtlIDs[i]] = _allocStr(szTmp);
			}
		}

		fwrite(&pmf, sizeof(pmf), 1, file);

		uint16_t u16NameLen;
		for(int i = 0; i < idMatMax; ++i)
		{
			if(aszMatNames[i])
			{
				u16NameLen = strlen(aszMatNames[i]);
			}
			else
			{
				u16NameLen = 0;
			}
			fwrite(&u16NameLen, sizeof(u16NameLen), 1, file);

			if(aszMatNames[i])
			{
				fwrite(aszMatNames[i], sizeof(char), u16NameLen, file);
			}
		}

		fwrite(m_pGeomMtlIDs, sizeof(ID), m_iGeomFacesCount, file);
		fclose(file);

		for(int i = 0; i < idMatMax; ++i)
		{
			mem_delete_a(aszMatNames[i]);
		}
	}
	return(ret);
}

MTLTYPE_PHYSIC CPhyWorld::getMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	ID idMtl = getMtlID(pBody, pShapeInfo);
	if(ID_VALID(idMtl))
	{
		return(SMtrl_MtlGetPhysicMaterial(idMtl));
	}
	return(MTLTYPE_PHYSIC_DEFAULT);
}

ID CPhyWorld::getMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	if(pBody == m_pGeomStaticRigidBody && m_iGeomFacesCount > pShapeInfo->m_triangleIndex)
	{
		return(m_pGeomMtlIDs[pShapeInfo->m_triangleIndex]);
	}
	return(-1);
}

void CPhyWorld::disableSimulation()
{
	m_isRunning = false;
}
void CPhyWorld::enableSimulation()
{
	m_isRunning = true;
}

//##############################################################

void CPhyWorld::CDebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	int clr = 0;
	clr += 255;
	clr <<= 8;
	clr += (int)(color.getX() * 255.0f);
	clr <<= 8;
	clr += (int)(color.getY() * 255.0f);
	clr <<= 8;
	clr += (int)(color.getZ() * 255.0f);
	//clr <<= 8;

	render_point pt;
	pt.clr = clr;

	pt.pos = float3_t(from.x(), from.y(), from.z());
	m_vDrawData.push_back(pt);

	pt.pos = float3_t(to.x(), to.y(), to.z());
	m_vDrawData.push_back(pt);
}

void CPhyWorld::CDebugDrawer::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}

void CPhyWorld::CDebugDrawer::reportErrorWarning(const char * warningString)
{
	if (m_bExpectObject)
	{
		m_bExpectObject = false;
		btCollisionObject *pObj = (btCollisionObject*)warningString;

		btVector3 vOrigin = pObj->getWorldTransform().getOrigin();
		LibReport(REPORT_MSG_LEVEL_WARNING, "Object world position: %.2f, %.2f, %.2f\n", vOrigin.x(), vOrigin.y(), vOrigin.z());

		MessageBoxA(NULL, "found", "bug", MB_OK | MB_ICONSTOP);

		btVector3 minAabb, maxAabb;
		pObj->getCollisionShape()->getAabb(pObj->getWorldTransform(), minAabb, maxAabb);
		LibReport(REPORT_MSG_LEVEL_WARNING, "AABBmin: %.2f, %.2f, %.2f\n", minAabb.x(), minAabb.y(), minAabb.z());
		LibReport(REPORT_MSG_LEVEL_WARNING, "AABBmax: %.2f, %.2f, %.2f\n", maxAabb.x(), maxAabb.y(), maxAabb.z());
		if (pObj->getCollisionShape()->getShapeType() == CAPSULE_SHAPE_PROXYTYPE)
		{
			btCapsuleShape *pCaps = (btCapsuleShape*)pObj->getCollisionShape();
			LibReport(REPORT_MSG_LEVEL_WARNING, "Radius: %.2f; HalfHeight: %.2f\n", pCaps->getRadius(), pCaps->getHalfHeight());
			SMMATRIX mat;
			pObj->getWorldTransform().getOpenGLMatrix((float*)&mat);
			LibReport(REPORT_MSG_LEVEL_WARNING, "Transform:\n");
			LibReport(REPORT_MSG_LEVEL_WARNING, " %5.4f %5.4f %5.4f %5.4f\n", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]);
			LibReport(REPORT_MSG_LEVEL_WARNING, " %5.4f %5.4f %5.4f %5.4f\n", mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]);
			LibReport(REPORT_MSG_LEVEL_WARNING, " %5.4f %5.4f %5.4f %5.4f\n", mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);
			LibReport(REPORT_MSG_LEVEL_WARNING, " %5.4f %5.4f %5.4f %5.4f\n", mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);
		}

		btTransform &trans = pObj->getWorldTransform();

		LibReport(REPORT_MSG_LEVEL_WARNING, " 0x%08xf\n", &trans);
		byte * pByte = (byte*)&trans;
		for (int i = 0, l = sizeof(trans); i < l; ++i)
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, " %02x", pByte[i]);
		}

		if (!pObj->getUserPointer())
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "getUserPointer() is NULL.\n");
			return;
		}
		Core_0ConsoleExecCmd("ent_dump_info %x", pObj->getUserPointer());
		return;
	}
	if (!strcmp(warningString, "@@@obj"))
	{
		m_bExpectObject = true;
		return;
	}

	LibReport(REPORT_MSG_LEVEL_WARNING, "%s", warningString);
}

void CPhyWorld::CDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}

void CPhyWorld::CDebugDrawer::setDebugMode(int debugMode)
{
	m_iDebugMode = debugMode;
}

int CPhyWorld::CDebugDrawer::getDebugMode() const
{
	return(m_iDebugMode);
}

void CPhyWorld::CDebugDrawer::render()
{
	if(!m_vDrawData.size())
	{
		return;
	}
	SGCore_ShaderUnBind();

	SMMATRIX mView, mProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mProj);

	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&SMMatrixIdentity());
	SGCore_GetDXDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
	SGCore_GetDXDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj);

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	//SGCore_GetDXDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFF);

	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	SGCore_GetDXDevice()->SetTexture(0, 0);
	
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, m_vDrawData.size() / 2, &(m_vDrawData[0]), sizeof(render_point));

	m_vDrawData.clear();
}
