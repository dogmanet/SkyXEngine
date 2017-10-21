#include "PhyWorld.h"
#include <core/sxcore.h>
#include <geom/sxgeom.h>
#include <gcore/sxgcore.h>
#include <mtllight/sxmtllight.h>


#include <../Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.h>

PhyWorld::PhyWorld():
	m_pGeomStaticCollideMesh(NULL),
	m_pGeomStaticCollideShape(NULL),
	m_pGeomStaticRigidBody(NULL),
	m_pGeomMtlTypes(0),
	m_iGeomFacesCount(0),
	m_iGeomModelCount(0),
	m_ppGreenStaticCollideShape(NULL),
	m_pppGreenStaticRigidBody(NULL),
	m_iGreenShapes(0),
	m_piGreenTotal(NULL)
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
		
	m_pDebugDrawer = new DebugDrawer();
	m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	//m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_FastWireframe);
	m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);

	Core_0RegisterCVarBool("r_physdebug", false, "Debug drawing physics shapes");
	m_bDebugDraw = GET_PCVAR_BOOL("r_physdebug");
	printf("Done!\n");
}

PhyWorld::~PhyWorld()
{
	UnloadGeom();

	mem_delete(m_pDynamicsWorld);
	mem_delete(m_pGHostPairCallback);
	mem_delete(m_pSolver);
	mem_delete(m_pDispatcher);
	mem_delete(m_pCollisionConfiguration);
	mem_delete(m_pBroadphase);
}

void PhyWorld::Render()
{
	if(*m_bDebugDraw)
	{
		m_pDynamicsWorld->debugDrawWorld();
		((DebugDrawer*)(m_pDynamicsWorld->getDebugDrawer()))->Render();
	}
}

void PhyWorld::SetThreadNum(int tnum)
{
}
void PhyWorld::Update(int thread)
{
	static UINT time0 = GetTickCount();
	UINT time1 = GetTickCount();

	if(time1 - time0 > 5000)
	{
		time0 = time1;
	}
	m_pDynamicsWorld->stepSimulation((float)(time1 - time0) / 1000.0f, 0, 1.0f / 60.0f);

	time0 = time1;
}
void PhyWorld::Sync()
{
}

void PhyWorld::AddShape(btRigidBody * pBody)
{
	m_pDynamicsWorld->addRigidBody(pBody);
}

void PhyWorld::RemoveShape(btRigidBody * pBody)
{
	if(pBody)
	{
		m_pDynamicsWorld->removeRigidBody(pBody);
	}
}

void PhyWorld::LoadGeom(const char * file)
{
	if(file && ImportGeom(file))
	{
		return;
	}
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

		m_pGeomMtlTypes = new MTLTYPE_PHYSIC[m_iGeomFacesCount];
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
				m_pGeomMtlTypes[iFace++] = SML_MtlGetPhysicMaterial(ppMtls[tc][i]);
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

			AddShape(m_pGeomStaticRigidBody);
		}
	}
	SGeom_ModelsClearArrBuffsGeom(ppVertices, pVertexCount, ppIndices, ppMtls, pIndexCount, iModelCount);





	float3_t** green_arr_vertex;
	int32_t* green_arr_count_vertex;
	uint32_t** green_arr_index;
	ID** green_arr_mtl;
	int32_t* green_arr_count_index;
	GreenDataVertex** green_arr_transform;
	int32_t* green_arr_count_transform;
	int32_t green_arr_count_green;

	SGeom_GreenGetNavMeshAndTransform(&green_arr_vertex, &green_arr_count_vertex, &green_arr_index, &green_arr_mtl, &green_arr_count_index, &green_arr_transform, &green_arr_count_transform, &green_arr_count_green);

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
				MTLTYPE_PHYSIC type = SML_MtlGetPhysicMaterial(green_arr_mtl[num_green][i]);
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
					type = SML_MtlGetPhysicMaterial(green_arr_mtl[num_green][idx]);
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
				type = SML_MtlGetPhysicMaterial(green_arr_mtl[num_green][i]);
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

					AddShape(body);
				}
			}
		}

	}
	SGeom_GreenClearNavMeshAndTransform(green_arr_vertex, green_arr_count_vertex, green_arr_index, green_arr_mtl, green_arr_count_index, green_arr_transform, green_arr_count_transform, green_arr_count_green);

	if (file)
		ExportGeom(file);
}

void PhyWorld::UnloadGeom()
{
	for(int i = 0; i < m_iGreenShapes; ++i)
	{
		for(int j = 0; j < m_piGreenTotal[i]; ++j)
		{
			RemoveShape(m_pppGreenStaticRigidBody[i][j]);
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
	mem_delete_a(m_piGreenTotal);
	mem_delete_a(m_pppGreenStaticRigidBody);
	mem_delete_a(m_ppGreenStaticCollideShape);



	RemoveShape(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticCollideShape);
	mem_delete(m_pGeomStaticCollideMesh);

	m_iGeomModelCount = 0;
	m_iGeomFacesCount = 0;
	mem_delete_a(m_pGeomMtlTypes);
}

bool PhyWorld::ImportGeom(const char * file)
{
	UnloadGeom();
	
	int len = strlen(file) + 1;
	char * name = (char*)alloca(sizeof(char)*(len + 1));
	memcpy(name, file, sizeof(char)* len);
	name[len - 1] = 'm';
	name[len] = 0;

	if(!Core_0FileExists(file) || !Core_0FileExists(name))
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
					m_pGeomMtlTypes = new MTLTYPE_PHYSIC[m_iGeomFacesCount];
					fread(m_pGeomMtlTypes, sizeof(MTLTYPE_PHYSIC), m_iGeomFacesCount, pF);
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
		UnloadGeom();
	}
	return(ret);
}

const char * _allocStr(const char * _str)
{
	char * str = new char[strlen(_str) + 1];
	strcpy(str, _str);
	return(str);
}

bool PhyWorld::ExportGeom(const char * _file)
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
		pmf.uiGeomFaceCount = m_iGeomFacesCount;
		fwrite(&pmf, sizeof(pmf), 1, file);
		fwrite(m_pGeomMtlTypes, sizeof(MTLTYPE_PHYSIC), m_iGeomFacesCount, file);
		fclose(file);
	}
	return(ret);
}

MTLTYPE_PHYSIC PhyWorld::GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	if(pBody == m_pGeomStaticRigidBody && m_iGeomFacesCount > pShapeInfo->m_triangleIndex)
	{
		return(m_pGeomMtlTypes[pShapeInfo->m_triangleIndex]);
	}
	return(MTLTYPE_PHYSIC_DEFAULT);
}

//##############################################################

void PhyWorld::DebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
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

void PhyWorld::DebugDrawer::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}

void PhyWorld::DebugDrawer::reportErrorWarning(const char * warningString)
{
	reportf(REPORT_MSG_LEVEL_WARNING, "%s\n", warningString);
}

void PhyWorld::DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}

void PhyWorld::DebugDrawer::setDebugMode(int debugMode)
{
	m_iDebugMode = debugMode;
}

int PhyWorld::DebugDrawer::getDebugMode() const
{
	return(m_iDebugMode);
}

void PhyWorld::DebugDrawer::Render()
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
	SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFF);

	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	SGCore_GetDXDevice()->SetTexture(0, 0);
	
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, m_vDrawData.size() / 2, &(m_vDrawData[0]), sizeof(render_point));

	m_vDrawData.clear();
}
