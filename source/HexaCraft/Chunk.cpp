#include "Chunk.h"

#include "PerlinNoise.h"

#include "DScoder.h"

#include "ChunkManager.h"


/*
Known bugs:
	1. Invalid collision detection near the Z chunk border // Fixed ?
*/


CChunk::CChunk(int64_t x, int64_t y, CChunkManager *pChunkManager):
	m_iXpos(x),
	m_iYpos(y),
	m_pChunkManager(pChunkManager)
{
	//m_pMatrix = new WORD[HC_CHUNK_SIZE_Y * HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Z];
}

CChunk::~CChunk()
{
	save();

	dematerialize();

	//mem_delete_a(m_pMatrix);
	
	releasePhysics();

	//mem_release(m_pResourceModel);
	//mem_release(m_pModel);
}

void CChunk::save()
{
	if(m_bChanged)
	{
		writeFile();
		m_bChanged = false;
	}
}


void CChunk::releasePhysics()
{
#if 0
	if(m_pRigidBody)
	{
		DSphysics::Lock();
		DSphysics::m_pDynamicsWorld->removeRigidBody(m_pRigidBody);
		DSphysics::Unlock();
	}
	DS_SAFE_DELETE(m_pRigidBody);
	DS_SAFE_DELETE(m_pCollideShape);
	DS_SAFE_DELETE(m_pCollideMesh);
	m_bReady = false;
#endif
}

void CChunk::updatePos()
{
	//if(m_bReady)
	{
		//int64_t x, z;
		//m_pChunkManager->getDelta(&x, &z);

		
		//btMotionState * pOld = m_pRigidBody->getMotionState();
		//btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3((float)(HC_CHUNK_SIZE_X * (m_iXpos - x) / 2) * 3.0f * HC_R, 0.0f, (float)(HC_CHUNK_SIZE_Z * (m_iYpos - z)) * Rcos30x2)));
		//m_pRigidBody = new btRigidBody(rigidBodyCI);
		//m_pRigidBody->setMotionState(motionState);
		//DS_SAFE_DELETE(pOld);
	}
}

#if 0
void CChunk::render()
{
	//return;
	DS_WAIT(m_bUpdating);
	m_bInRender = true;
	static const UINT iTexCount = DS::pDSHCCM->GetTotalTextureCount();
	const float R = HC_R;
	UINT stride = sizeof(DSmaterialSystem::vertex);
	UINT offset = 0;
	if(/*(m_iIndexCount[0] || m_iIndexCount[1]) && */m_iIndexCount && CheckFrustrum(DS::pCamera))
	{
		DS::pDSHCCM->m_iChunkCount++;
		int64_t x, z;
		DS::pDSHCCM->GetDelta(&x, &z);
		DSmaterialSystem::XMSetWord(XMMatrixTranslation((float)(HC_CHUNK_SIZE_X * (m_iXpos - x) / 2) * 3.0f * R, 0.0f, (float)(HC_CHUNK_SIZE_Z * (m_iYpos - z)) * Rcos30x2));
		DS::g_pImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DSmaterialSystem::SetColor(m_fColor);

		for(UINT tc = 0; tc < iTexCount; tc++)
		{
			if(m_iIndexCount[tc])
			{
				DSmaterialSystem::bindMaterial(m_pMaterials[tc]);
				DS::g_pImmediateContext()->IASetIndexBuffer(m_pIndexBuffer[tc], DXGI_FORMAT_R32_UINT, 0);
				DS::g_pImmediateContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[tc], &stride, &offset);
				DSmaterialSystem::PreRender();
				DS::g_pImmediateContext()->DrawIndexed(m_iIndexCount[tc], 0, 0);
			}
		}
	}
	m_bInRender = false;
}
#endif

void CChunk::buildBuffer2() //добавить сглаживание нормаей
{
	if(m_pEntity)
	{
		return;
	}
	/*if(m_bReady && !m_bBorder)
	{
		return;
	}*/

	static const UINT s_uTexCount = m_pChunkManager->getTotalTextureCount();


	int64_t xx, zz;
	m_pChunkManager->getDelta(&xx, &zz);

	
	// (float)(HC_CHUNK_SIZE_X * (m_iXpos - x) / 2) * 3.0f * R
	// (float)(HC_CHUNK_SIZE_Z * (m_iYpos - z)) * Rcos30x2


	IXResourceManager *pRM = m_pChunkManager->getResourceManager();

	if(m_bBorder)
	{
		//return;
	}

	//m_bReady = false;

	m_bBorder = false;
	DWORD t0 = GetTickCount();
	const float R = HC_R;
	const float H = HC_H;

	float fCurX = 0.0f;
	float fCurY = 0.0f; // top
	float fCurZ = 0.0f;
	XResourceModelStaticVertex **ppVertices = (XResourceModelStaticVertex**)alloca(sizeof(XResourceModelStaticVertex*) * s_uTexCount);
	UINT **ppIndices = (UINT**)alloca(sizeof(UINT*) * s_uTexCount);
	memset(ppIndices, 0, sizeof(UINT*) * s_uTexCount);
	memset(ppVertices, 0, sizeof(XResourceModelStaticVertex*) * s_uTexCount);
	
	UINT *puIndexCount = (UINT*)alloca(sizeof(UINT) * s_uTexCount);
	UINT *puVertexCount = (UINT*)alloca(sizeof(UINT) * s_uTexCount);
	memset(puIndexCount, 0, sizeof(UINT) * s_uTexCount);
	memset(puVertexCount, 0, sizeof(UINT) * s_uTexCount);

	IXResourceModelStatic *pResourceModel = NULL;

	bool b[8] = {false};
	int x, y, z;
	WORD pBlocks[21] = {NULL};
	XResourceModelStaticSubset *pSubset = NULL;
	for(UINT isFirst = 0; isFirst <= 1; ++isFirst)
	{
		if(isFirst)
		{
			//mem_release(m_pResourceModel);
			UINT uMatCount = 0;
			for(UINT i = 0; i < s_uTexCount; ++i)
			{
				if(puIndexCount[i])
				{
					++uMatCount;
				}
			}

			if(!uMatCount)
			{
				return;
			}

			// create a new resource
			pResourceModel = pRM->newResourceModelStatic();

			pResourceModel->setMaterialCount(uMatCount, 1);

			UINT *puLodIndices = (UINT*)alloca(sizeof(UINT) * uMatCount);
			UINT *puLodVertices = (UINT*)alloca(sizeof(UINT) * uMatCount);

			UINT uCurMat = 0;
			for(UINT i = 0; i < s_uTexCount; ++i)
			{
				if(puIndexCount[i])
				{
					puLodIndices[uCurMat] = puIndexCount[i];
					puLodVertices[uCurMat] = puVertexCount[i];

					pResourceModel->setMaterial(uCurMat, 0, m_pChunkManager->getTextureByID(i));

					++uCurMat;
				}
			}

			pResourceModel->addLod(uMatCount, puLodVertices, puLodIndices);

			uCurMat = 0;
			for(UINT i = 0; i < s_uTexCount; ++i)
			{
				if(puIndexCount[i])
				{
					pSubset = pResourceModel->getSubset(0, uCurMat);

					pSubset->iMaterialID = uCurMat;

					ppVertices[i] = pSubset->pVertices;
					ppIndices[i] = pSubset->pIndices;

					++uCurMat;
				}
			}

			memset(puIndexCount, 0, sizeof(UINT) * s_uTexCount);
			memset(puVertexCount, 0, sizeof(UINT) * s_uTexCount);

			// m_pResourceModel->getSubset(0, 0);
		}

		for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
		{
			fCurX = 0;
			for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
			{
				fCurZ = j % 2 ? 0.0f : -Rcos30;
				for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
				{
					/*if(!HC_BIT_SET(m_pppMatrix[i][j][k].GetType(), HC_BLOCK_TYPE_FULL))
					{
					continue;
					}*/

					const UINT typeID = HC_GET_ID(m_pMatrix[HCC_INDEX(i, j, k)]);
					if(typeID == HC_BLOCK_AIR) //@@TODO: there should be check for empty
					{
						fCurZ += Rcos30x2;
						continue;
					}
					z = i; // Y
					x = j; // X
					y = k - 1; // Z


					pBlocks[0] = getBlockOffset(j, k, i - 1, 0, 0);
					pBlocks[1] = getBlockOffset(j - 1, k + ((j & 1) ? 1 : 0), i - 1, 0, 0);
					pBlocks[2] = getBlockOffset(j, k + 1, i - 1, 0, 0);
					pBlocks[3] = getBlockOffset(j + 1, k + ((j & 1) ? 1 : 0), i - 1, 0, 0);
					pBlocks[4] = getBlockOffset(j + 1, k + ((j & 1) ? 0 : -1), i - 1, 0, 0);
					pBlocks[5] = getBlockOffset(j, k - 1, i - 1, 0, 0);
					pBlocks[6] = getBlockOffset(j - 1, k + ((j & 1) ? 0 : -1), i - 1, 0, 0);

					pBlocks[7] = getBlockOffset(j, k, i, 0, 0);
					pBlocks[8] = getBlockOffset(j - 1, k + ((j & 1) ? 1 : 0), i, 0, 0);
					pBlocks[9] = getBlockOffset(j, k + 1, i, 0, 0);
					pBlocks[10] = getBlockOffset(j + 1, k + ((j & 1) ? 1 : 0), i, 0, 0);
					pBlocks[11] = getBlockOffset(j + 1, k + ((j & 1) ? 0 : -1), i, 0, 0);
					pBlocks[12] = getBlockOffset(j, k - 1, i, 0, 0);
					pBlocks[13] = getBlockOffset(j - 1, k + ((j & 1) ? 0 : -1), i, 0, 0);

					pBlocks[14] = getBlockOffset(j, k, i + 1, 0, 0);
					pBlocks[15] = getBlockOffset(j - 1, k + ((j & 1) ? 1 : 0), i + 1, 0, 0);
					pBlocks[16] = getBlockOffset(j, k + 1, i + 1, 0, 0);
					pBlocks[17] = getBlockOffset(j + 1, k + ((j & 1) ? 1 : 0), i + 1, 0, 0);
					pBlocks[18] = getBlockOffset(j + 1, k + ((j & 1) ? 0 : -1), i + 1, 0, 0);
					pBlocks[19] = getBlockOffset(j, k - 1, i + 1, 0, 0);
					pBlocks[20] = getBlockOffset(j - 1, k + ((j & 1) ? 0 : -1), i + 1, 0, 0);

					WORD pC = NULL;
					//first
					pC = pBlocks[12];
					//b[0] = !(pC && HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL)) && pC;
					b[0] = !HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL);
					//

					//second
					pC = pBlocks[9];
					//b[3] = !(pC && HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL)) && pC;
					b[3] = !HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL);
					//

					//third
					pC = pBlocks[13];
					b[1] = !HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL);
					//

					//fourth
					pC = pBlocks[11];
					b[5] = !HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL);
					//

					//fifth
					pC = pBlocks[8];
					b[2] = !HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL);
					//

					//sixth
					pC = pBlocks[10];
					b[4] = !HC_BIT_SET(HC_GET_TYPE(pC), HC_BLOCK_TYPE_FULL);
					//

					z = i + 1; // top
					x = j;
					y = k;
					//b[6] = (z < 0 || z >= HC_CHUNK_SIZE_Y || !HC_BIT_SET(m_pppMatrix[z][x][y].GetType(), HC_BLOCK_TYPE_FULL));
					//b[6] = (z < 0 || z >= HC_CHUNK_SIZE_Y || !HC_BIT_SET(m_pppMatrix[z][x][y].GetType(), HC_BLOCK_TYPE_FULL));
					b[6] = (z < 0 || z >= HC_CHUNK_SIZE_Y || !HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(z, x, y)]), HC_BLOCK_TYPE_FULL));

					z = i - 1; // bottom
					//b[7] = (z < 0 || z >= HC_CHUNK_SIZE_Y || !HC_BIT_SET(m_pppMatrix[z][x][y].GetType(), HC_BLOCK_TYPE_FULL)) && z >= 0;
					b[7] = (z < 0 || z >= HC_CHUNK_SIZE_Y || !HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(z, x, y)]), HC_BLOCK_TYPE_FULL)) && z >= 0;

					if((b[0] || b[1] || b[2] || b[3] || b[4] || b[5] || b[6] || b[7]) && i == 0 && j == 15 && k == 15)
					{
						int a = 0;
					}

					for(UINT tc = 0, tcc = CBlockManager::GetMaterialCount(typeID); tc < tcc; tc++)
					{
						UINT uMatId = CBlockManager::GetMaterialById(typeID, tc);

						CBlockManager::CreateMesh(m_pMatrix[HCC_INDEX(i, j, k)], tc, fCurX, fCurY, fCurZ, R, H, ppVertices[uMatId], ppIndices[uMatId], &puVertexCount[uMatId], &puIndexCount[uMatId], b, pBlocks);
					}
					fCurZ += Rcos30x2;
				}
				fCurX += R * 1.5f;
			}
			fCurY += H;
		}
	}



	char tmp[64];
	sprintf(tmp, "c_%lld_%lld", m_iXpos, m_iYpos);
	pRM->addModel(tmp, pResourceModel);

	m_pEntity = SGame_CreateEntity("prop_static");
	m_pEntity->setPos(float3((float)(HC_CHUNK_SIZE_X * (m_iXpos - xx) / 2) * 3.0f * R, 0.0f, (float)(HC_CHUNK_SIZE_Z * (m_iYpos - zz)) * Rcos30x2));
	m_pEntity->setKV("use_trimesh", "2");
	m_pEntity->setKV("model", tmp);

	m_pEntity->setFlags(m_pEntity->getFlags() | EF_LEVEL/* | EF_EXPORT*/);
	
	
#if 0
	/*btTriangleIndexVertexArray * indexVertexArrays = new btTriangleIndexVertexArray
		(
		(int)(indices.size() / 3),
		&indices[0],
		sizeof(int) * 3,
		(int)(vertices.size() / 3),
		&vertices[0],
		sizeof(float) * 3
		);*/

	//DS_SAFE_DELETE(m_pCollideMesh);
	//if(!m_pCollideMesh)
	{
		m_pCollideMesh = new btTriangleMesh();
	}
	//for(int i = 0; i < iCurVertex[0]; i += 3)
	for(UINT tc = 0; tc < iTexCount; tc++)
	{
		for(int i = 0; i < m_iIndexCount[tc]; i += 3)
		{
			m_pCollideMesh->addTriangle(XM3_BTVEC(pVertices[tc][pIndices[tc][i]].Pos), XM3_BTVEC(pVertices[tc][pIndices[tc][i + 1]].Pos), XM3_BTVEC(pVertices[tc][pIndices[tc][i + 2]].Pos));
		}
	}
	//if(!m_pCollideShape)
	{
		//m_pCollideShape = new btBvhTriangleMeshShape(m_pCollideMesh, true);
		//@@TODO: Delete old pointers
	}
	//	else
	//{
	btCollisionShape * pOldShape = m_pCollideShape;
	if(m_pCollideMesh->getNumTriangles() != 0)
	{
		m_pCollideShape = new btBvhTriangleMeshShape(m_pCollideMesh, true);
		//DS_WAIT(DSphysics::m_bInProgress);
		DSphysics::Lock();
		if(m_pRigidBody)
		{
			m_pRigidBody->setCollisionShape(m_pCollideShape);
		}
		//}
		else
		{
			int64_t x, z;
			DS::pDSHCCM->GetDelta(&x, &z);
			btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3((float)(HC_CHUNK_SIZE_X * (m_iXpos - x) / 2) * 3.0f * R, 0.0f, (float)(HC_CHUNK_SIZE_Z * (m_iYpos - z)) * Rcos30x2)));
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
				0,                  // mass
				motionState,        // initial position
				m_pCollideShape,    // collision shape of body
				btVector3(0, 0, 0)  // local inertia
				);
			m_pRigidBody = new btRigidBody(rigidBodyCI);
			//m_pRigidBody->setFriction(10.0f);
			//m_pRigidBody->setFriction(0.0f);
			m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
			m_pRigidBody->setFriction(100.0f);
			//m_pRigidBody->setRestitution(10.0f);
	
			DSphysics::AddShape(m_pRigidBody);
		}
		DSphysics::Unlock();
	}
	
	//m_pCollideMesh
	for(UINT tc = 0; tc < iTexCount; tc++)
	{
		DS_SAFE_DELETE_A(pVertices[tc]);
		DS_SAFE_DELETE_A(pIndices[tc]);
	}
	DS_SAFE_DELETE_A(iCurVertex);
	DS_SAFE_DELETE_A(iCurIndex);
	DS_SAFE_DELETE_A(pVertices);
	DS_SAFE_DELETE_A(pIndices);
	DS_SAFE_DELETE(pOldMesh);
	DS_SAFE_DELETE(pOldShape);
	DWORD t3 = GetTickCount();
#endif
	//m_bReady = true;
	//printf("t1: %u; t2: %u; t3: %u\n", t1 - t0, t2 - t1, t3 - t2);
}

#if 0
void CChunk::buildBuffer()
{
	const UINT xSize = 10;
	const UINT ySize = 10;
	const float fHeight = 7.0f;
	UINT ppMatrix[xSize][ySize] = {
		/*{1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
		{0, 0, 1, 1, 0, 0, 0, 0, 1, 1},
		{0, 0, 0, 1, 1, 0, 0, 0, 1, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 1, 1},
		{0, 0, 1, 1, 1, 0, 0, 0, 1, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
		{0, 0, 1, 1, 0, 0, 0, 0, 1, 1},
		{0, 0, 0, 1, 1, 0, 0, 0, 1, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 1, 1},
		{0, 0, 1, 1, 1, 0, 0, 0, 1, 1},*/
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	const float R = HC_R;


	//XMFLOAT2 ppCentres[5][5];

	

	UINT iHCount = 0;
	UINT iHWount = 0;
	UINT iHWindex = 0;
	int x;
	int y;
	
	for(int i = 0; i < xSize; i++)
	{
		for(int j = 0; j < ySize; j++)
		{
			bool b[6];
			if(ppMatrix[i][j])
			{
				iHCount++;
				x = i;
				y = j - 1;
				//first
				if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
				{
					b[0] = b[5] = true;
					iHWindex += 6;
				}
				//

				y = j + 1;
				//second
				if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
				{
					b[2] = b[3] = true;
					iHWindex += 6;
				}
				//

				x = i - 1;
				y = i % 2 ? j : j - 1;
				//third
				if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
				{
					b[1] = b[0] = true;
					iHWindex += 6;
				}
				//

				x = i + 1;
				//fourth
				if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
				{
					b[4] = b[5] = true;
					iHWindex += 6;
				}
				//

				x = i - 1;
				y += 1;
				//fifth
				if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
				{
					b[2] = b[1] = true;
					iHWindex += 6;
				}
				//

				x = i + 1;
				//sixth
				if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
				{
					b[4] = b[3] = true;
					iHWindex += 6;
				}
				//


				b[0] ? iHWount++ : 0;
				b[1] ? iHWount++ : 0;
				b[2] ? iHWount++ : 0;
				b[3] ? iHWount++ : 0;
				b[4] ? iHWount++ : 0;
				b[5] ? iHWount++ : 0;
			}
		}
	}
	m_iIndexCount[0] = iHCount * 12 + iHWindex;
	XResourceModelStaticVertex * pVertices = new XResourceModelStaticVertex[iHCount * 6 + iHWount];
	UINT * pIndices = new UINT[m_iIndexCount[0]];

	float fCurX = 0.0f;
	float fCurY = 0.0f;
	UINT iCurInd = 0;
	UINT iCurVert = 0;
	
	for(int i = 0; i < xSize; i++)
	{
		fCurY = i % 2 ? 0.0f : -Rcos30;
		for(int j = 0; j < ySize; j++)
		{
			UINT bb[6] = {0};
			//ppCentres[i][j].x = fCurX;
			//ppCentres[i][j].y = fCurY;
			//DSconsole::write("X: %.3f, Y: %.3f\n", ppCentres[i][j].x, ppCentres[i][j].y);
			if(ppMatrix[i][j])
			{

				pIndices[iCurInd] = 0 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 1 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 2 + iCurVert;
				iCurInd++;

				pIndices[iCurInd] = 0 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 3 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 1 + iCurVert;
				iCurInd++;

				pIndices[iCurInd] = 1 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 4 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 2 + iCurVert;
				iCurInd++;

				pIndices[iCurInd] = 2 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 5 + iCurVert;
				iCurInd++;
				pIndices[iCurInd] = 0 + iCurVert;
				iCurInd++;

				UINT icv = iCurVert;


				pVertices[iCurVert].Pos = XMFLOAT3(fCurX - R, fHeight, fCurY);
				pVertices[iCurVert].Tex = XMFLOAT2(0.5f, 0.0f);
				iCurVert++;
				pVertices[iCurVert].Pos = XMFLOAT3(fCurX + Rdiv2, fHeight, fCurY + Rcos30);
				pVertices[iCurVert].Tex = XMFLOAT2(0.5f + cos30div2, 0.75f);
				iCurVert++;
				pVertices[iCurVert].Pos = XMFLOAT3(fCurX + Rdiv2, fHeight, fCurY - Rcos30);
				pVertices[iCurVert].Tex = XMFLOAT2(0.5f - cos30div2, 0.75f);
				iCurVert++;
				pVertices[iCurVert].Pos = XMFLOAT3(fCurX - Rdiv2, fHeight, fCurY + Rcos30);
				pVertices[iCurVert].Tex = XMFLOAT2(0.5f + cos30div2, 0.25f);
				iCurVert++;
				pVertices[iCurVert].Pos = XMFLOAT3(fCurX + R, fHeight, fCurY);
				pVertices[iCurVert].Tex = XMFLOAT2(0.5f, 1.0f);
				iCurVert++;
				pVertices[iCurVert].Pos = XMFLOAT3(fCurX - Rdiv2, fHeight, fCurY - Rcos30);
				pVertices[iCurVert].Tex = XMFLOAT2(0.5f - cos30div2, 0.25f);
				iCurVert++;



				{
					x = i;
					y = j - 1;
					//first
					if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
					{
						//b[0] = b[5] = true;

						if(!bb[0])
						{
							bb[0] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX - Rdiv2, 0.0f, fCurY - Rcos30);
							iCurVert++;
						}
						if(!bb[5])
						{
							bb[5] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX + Rdiv2, 0.0f, fCurY - Rcos30);
							iCurVert++;
						}

						pIndices[iCurInd] = 5 + icv;
						iCurInd++;
						pIndices[iCurInd] = 2 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[0];
						iCurInd++;

						pIndices[iCurInd] = bb[0];
						iCurInd++;
						pIndices[iCurInd] = 2 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[5];
						iCurInd++;
						
					}
					//

					y = j + 1;
					//second
					if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
					{
						if(!bb[2])
						{
							bb[2] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX - Rdiv2, 0.0f, fCurY + Rcos30);
							iCurVert++;
						}
						if(!bb[3])
						{
							bb[3] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX + Rdiv2, 0.0f, fCurY + Rcos30);
							iCurVert++;
						}

						pIndices[iCurInd] = bb[3];
						iCurInd++;
						pIndices[iCurInd] = 1 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[2];
						iCurInd++;

						pIndices[iCurInd] = 1 + icv;
						iCurInd++;
						pIndices[iCurInd] = 3 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[2];
						iCurInd++;
						
					}
					//

					x = i - 1;
					y = i % 2 ? j : j - 1;
					//third
					if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
					{
						if(!bb[1])
						{
							bb[1] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX - R, 0.0f, fCurY);
							iCurVert++;
						}
						if(!bb[0])
						{
							bb[0] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX - Rdiv2, 0.0f, fCurY - Rcos30);
							iCurVert++;
						}

						pIndices[iCurInd] = bb[1];
						iCurInd++;
						pIndices[iCurInd] = 5 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[0];
						iCurInd++;

						pIndices[iCurInd] = 0 + icv;
						iCurInd++;
						pIndices[iCurInd] = 5 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[1];
						iCurInd++;

					}
					//

					x = i + 1;
					//fourth
					if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
					{
						if(!bb[4])
						{
							bb[4] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX + R, 0.0f, fCurY);
							iCurVert++;
						}
						if(!bb[5])
						{
							bb[5] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX + Rdiv2, 0.0f, fCurY - Rcos30);
							iCurVert++;
						}

						pIndices[iCurInd] = bb[5];
						iCurInd++;
						pIndices[iCurInd] = 4 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[4];
						iCurInd++;

						pIndices[iCurInd] = 2 + icv;
						iCurInd++;
						pIndices[iCurInd] = 4 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[5];
						iCurInd++;

					}
					//

					x = i - 1;
					y += 1;
					//fifth
					if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
					{
						if(!bb[2])
						{
							bb[2] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX - Rdiv2, 0.0f, fCurY + Rcos30);
							iCurVert++;
						}
						if(!bb[1])
						{
							bb[1] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX - R, 0.0f, fCurY);
							iCurVert++;
						}

						pIndices[iCurInd] = bb[2];
						iCurInd++;
						pIndices[iCurInd] = 0 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[1];
						iCurInd++;

						pIndices[iCurInd] = 3 + icv;
						iCurInd++;
						pIndices[iCurInd] = 0 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[2];
						iCurInd++;

					}
					//

					x = i + 1;
					//sixth
					if(x < 0 || x >= xSize || y < 0 || y >= ySize || !ppMatrix[x][y])
					{
						if(!bb[4])
						{
							bb[4] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX + R, 0.0f, fCurY);
							iCurVert++;
						}
						if(!bb[3])
						{
							bb[3] = iCurVert;
							pVertices[iCurVert].Pos = XMFLOAT3(fCurX + Rdiv2, 0.0f, fCurY + Rcos30);
							iCurVert++;
						}

						pIndices[iCurInd] = bb[4];
						iCurInd++;
						pIndices[iCurInd] = 1 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[3];
						iCurInd++;

						pIndices[iCurInd] = 4 + icv;
						iCurInd++;
						pIndices[iCurInd] = 1 + icv;
						iCurInd++;
						pIndices[iCurInd] = bb[4];
						iCurInd++;

					}
					//
				}

				

				
			}

			fCurY += Rcos30x2;
		}
		fCurX += R * 1.5f;
	}

	DS::CreateBuffers(pVertices, iHCount * 6 + iHWount, pIndices, m_iIndexCount[0], &m_pVertexBuffer[0], &m_pIndexBuffer[0]);
	if(!m_pIndexBuffer[0] || !m_pVertexBuffer[0])
	{
		m_iIndexCount[0] = 0;
	}
	DS_SAFE_DELETE_A(pIndices);
	DS_SAFE_DELETE_A(pVertices);
}
#endif

#if 0
UINT CChunk::GetLODvertexCount(UINT level)
{
	return((HC_CHUNK_SIZE_X >> level) * (HC_CHUNK_SIZE_Z >> level));
}

void CChunk::BuildLOD(UINT level, UINT * pIndices, DSmaterialSystem::vertex * pVertices, UINT * pCurIndex, UINT * pCurVertex)
{
	const float R = HC_R;
	const float H = HC_H;

	float fCurX = 0.0f;
	float fCurY = 0.0f; // top
	float fCurZ = 0.0f;
	
	int x, y, z;

	for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
	{
		fCurX = 0;
		for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
		{
			fCurZ = j % 2 ? 0.0f : -Rcos30;
			for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
			{
				
				fCurZ += Rcos30x2;
			}
			fCurX += R * 1.5f;
		}
		fCurY += H;
	}
}
#endif


void CChunk::makeRandom()
{
	m_bChanged = true;
	const int seed = 59705657;
	//PerlinNoise pn(1, 0.005, 1, 7, 4);
	CPerlinNoise pn(0.9, 0.005, 1, 7, 4);
	CPerlinNoise pn2(0.9, 0.01, 1, 5, 4);
	float ** ppMatrix;
	//float ** ppMatrixNew;
	ppMatrix = new float *[HC_CHUNK_SIZE_X + 2];
	//ppMatrixNew = new float *[HC_CHUNK_SIZE_X + 2];
	for(int j = 0; j < HC_CHUNK_SIZE_X + 2; j++)
	{
		ppMatrix[j] = new float[HC_CHUNK_SIZE_Z + 2];
		//ppMatrixNew[j] = new float[HC_CHUNK_SIZE_Z + 2];
		for(int k = 0; k < HC_CHUNK_SIZE_Z + 2; k++)
		{
			//float val = sqrtf((MakeRandNum(j, k, seed) + 1.0f) * 0.5f) * (float)HC_CHUNK_SIZE_Y;
			//ppMatrix[j][k] = (MakeRandNum((j - 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1) + HC_CHUNK_SIZE_Z * m_iYpos, seed) + 1.0f) * 0.5f;
			/*ppMatrix[j][k] = (
				(
				MakeRandNum((j - 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1) + HC_CHUNK_SIZE_Z * m_iYpos, seed) +
				MakeRandNum((j - 1 - 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1 - 1) + HC_CHUNK_SIZE_Z * m_iYpos, seed) +
				MakeRandNum((j - 1 - 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1 + 1) + HC_CHUNK_SIZE_Z * m_iYpos, seed) +
				MakeRandNum((j - 1 + 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1 - 1) + HC_CHUNK_SIZE_Z * m_iYpos, seed) +
				MakeRandNum((j - 1 + 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1 + 1) + HC_CHUNK_SIZE_Z * m_iYpos, seed)
				) / 40.0f
				+ 1.0f) * 0.5f;*/
			float v2 = pn2.getHeight((j - 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1) + HC_CHUNK_SIZE_Z * m_iYpos) / 9.8f * 2.5f;
			//float v2 = 0.0f;
			ppMatrix[j][k] = (pn.getHeight((j - 1) + HC_CHUNK_SIZE_X * m_iXpos, (k - 1) + HC_CHUNK_SIZE_Z * m_iYpos) + 2.2f) / 20.0f + 0.5f + (v2 > 0.0f ? v2 : 0.0f);
			/*if(ppMatrix[j][k] < 0)
			{
				_asm
				{
				//	int 3;
				};
			}*/
		}
	}

	/*for(int j = 1; j < HC_CHUNK_SIZE_X + 1; j++)
	{
		for(int k = 1; k < HC_CHUNK_SIZE_Z + 1; k++)
		{
			ppMatrixNew[j][k] = (ppMatrix[j][k] * 0.5f
				+ ppMatrix[j + 1][k] * 0.5f
				+ ppMatrix[j - 1][k] * 0.5f
				+ ppMatrix[j][k + 1] * 0.5f
				+ ppMatrix[j][k - 1] * 0.5f) / 2.5f;
		}
	}

	for(int j = 1; j < HC_CHUNK_SIZE_X + 1; j++)
	{
		for(int k = 1; k < HC_CHUNK_SIZE_Z + 1; k++)
		{
			ppMatrix[j][k] = (ppMatrixNew[j][k] * 0.5f
				+ ppMatrixNew[j + 1][k] * 0.5f
				+ ppMatrixNew[j - 1][k] * 0.5f
				+ ppMatrixNew[j][k + 1] * 0.5f
				+ ppMatrixNew[j][k - 1] * 0.5f) / 2.5f;
		}
	}

	for(int j = 1; j < HC_CHUNK_SIZE_X + 1; j++)
	{
		for(int k = 1; k < HC_CHUNK_SIZE_Z + 1; k++)
		{
			ppMatrixNew[j][k] = (ppMatrix[j][k] * 0.5f
				+ ppMatrix[j + 1][k] * 0.5f
				+ ppMatrix[j - 1][k] * 0.5f
				+ ppMatrix[j][k + 1] * 0.5f
				+ ppMatrix[j][k - 1] * 0.5f) / 2.5f;
		}
	}

	for(int j = 1; j < HC_CHUNK_SIZE_X + 1; j++)
	{
		for(int k = 1; k < HC_CHUNK_SIZE_Z + 1; k++)
		{
			ppMatrix[j][k] = (ppMatrixNew[j][k] * 0.5f
				+ ppMatrixNew[j + 1][k] * 0.5f
				+ ppMatrixNew[j - 1][k] * 0.5f
				+ ppMatrixNew[j][k + 1] * 0.5f
				+ ppMatrixNew[j][k - 1] * 0.5f) / 2.5f;
		}
	}

	for(int j = 1; j < HC_CHUNK_SIZE_X + 1; j++)
	{
		for(int k = 1; k < HC_CHUNK_SIZE_Z + 1; k++)
		{
			ppMatrixNew[j][k] = (ppMatrix[j][k] * 0.5f
				+ ppMatrix[j + 1][k] * 0.5f
				+ ppMatrix[j - 1][k] * 0.5f
				+ ppMatrix[j][k + 1] * 0.5f
				+ ppMatrix[j][k - 1] * 0.5f) / 2.5f;
		}
	}*/

	for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
	{
		for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
		{
			//float n = MakeRandNum(j, k, seed);
			//float val = sqrtf((n + 1.0f) * 0.5f) * (float)HC_CHUNK_SIZE_Y;
			float val = ppMatrix[j + 1][k + 1] * (float)HC_CHUNK_SIZE_Y;
			for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
			{
				//DS_ASSERT((void*)m_pppMatrix[i] != (void*)0xCDCDCDCD);
				//m_pppMatrix[i][j][k].SetType(i <= val ? HC_BLOCK_TYPE_FULL : HC_BLOCK_TYPE_NONE);
				//m_pMatrix[HCC_INDEX(i, j, k)]->SetType(i <= val ? HC_BLOCK_TYPE_FULL : HC_BLOCK_TYPE_NONE);
				//m_pMatrix[HCC_INDEX(i, j, k)] = HC_NEW_BLOCK(i <= val ? HC_MKTYPE(HC_BLOCK_TYPE_FULL, i <= val - 2 ? HC_BLOCK_SAND : HC_BLOCK_GRASS) : HC_MKTYPE(HC_BLOCK_TYPE_NONE, HC_BLOCK_AIR), m_pBlockPool);
				m_pMatrix[HCC_INDEX(i, j, k)] = i <= val ? HC_MKTYPE(HC_BLOCK_TYPE_FULL, i <= val - 2 ? HC_BLOCK_SAND : HC_BLOCK_GRASS) : HC_MKTYPE(HC_BLOCK_TYPE_NONE, HC_BLOCK_AIR);
			}
		}
	}

	for(int j = 0; j < HC_CHUNK_SIZE_X + 2; j++)
	{
		mem_delete_a(ppMatrix[j]);
		//DS_SAFE_DELETE_A(ppMatrixNew[j]);
	}
	mem_delete_a(ppMatrix);
	//DS_SAFE_DELETE_A(ppMatrixNew);
}

bool CChunk::inMyInterval(const float3 &pos)
{
	/*
	 ___     ___     
	/   \___/   \___
	\___/   \___/   \
	/   \___/   \___/
	\___/   \___/   \
	/   \___/   \___/
	\___/   \___/   \
	/   \___/   \___/
	\___/   \___/   \
	    \___/   \___/
	*/
	const float R = HC_R;
	//return(pos.x + R < (float)(HC_CHUNK_SIZE_X * (m_iXpos + 1) / 2) * 3.0f * R && pos.x + R >= (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * R &&
	//	pos.z + Rcos30 * 2.0f < (float)(HC_CHUNK_SIZE_Z * (m_iYpos + 1)) * Rcos30x2 && pos.z + Rcos30 * 2.0f >= (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2);

	const float fXmin = (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * R - 3.0f * 0.25f * R;
	const float fXmax = (float)(HC_CHUNK_SIZE_X * (m_iXpos + 1) / 2) * 3.0f * R - 3.0f * 0.25f * R;

	const float fYmin = (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2 - Rcos30 * 0.5f;
	const float fYmax = (float)(HC_CHUNK_SIZE_Z * (m_iYpos + 1)) * Rcos30x2 - Rcos30 * 0.5f;

	return(pos.x < fXmax && pos.x >= fXmin && pos.z < fYmax && pos.z >= fYmin);
}

#if 0
float CChunk::isOnGround(DSHCplayer * pl)
{
	if(!m_bReady)
	{
		return(0.0f);
	}

	int pX;
	int pY;
	int pZ;

	pl->GetBlockIn(&pX, &pY, &pZ);

	XMFLOAT3 pos = pl->GetPos();
	XMFLOAT3 bp = GetBlockPos(pX, pZ, pY);
	float fCurY = bp.y;
	for(int i = pY; i < HC_CHUNK_SIZE_Y; i++)
	{
		if(fCurY > pos.y)
		{
			if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(i, pX, pZ)]), HC_BLOCK_TYPE_FULL))
			{
				return(fCurY - pos.y);
			}
			else
			{
				return(-1.0f); //?
			}
		}

		fCurY += HC_H;
	}

	return(-1.0f);
}

float CChunk::IsOnGround(XMFLOAT3 pos)
{
	//DS_ASSERT(false);
	if(!m_bReady)
	{
		return(0.0f);
	}


	UINT pX = 0;
	UINT pZ = 0;

	FindUserInChunk(pos.x, pos.z, &pX, &pZ);
	float fCurY = 0.0f;
	for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
	{
		if(fCurY > pos.y)
		{
			if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(i, pX, pZ)]), HC_BLOCK_TYPE_FULL))
			{
				return(fCurY - pos.y);
			}
			else
			{
				return(-1.0f); //?
			}
		}

		fCurY += HC_H;
	}

	return(-1.0f);

	/*const float R = HC_R;
	const float H = HC_H;


	XMFLOAT3 fOrigPos = pos;

	pos.x -= (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * R;
	pos.z -= (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2;

	float fCurX;
	float fCurY = 0;
	float fCurZ;

	bool bFound = false;
	//for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
	//{
	fCurX = 0;
	for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
	{
		if(fCurX + R * 3.0f * 0.25f > pos.x && fCurX - R * 3.0f * 0.25f <= pos.x)
		{
			fCurZ = j % 2 ? 0.0f : -Rcos30;
			for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
			{
				if(fCurZ + Rcos30 * 1.5f + 1.0 > pos.z && fCurZ - Rcos30 * 1.5f - 1.0f <= pos.z)
					//if(fCurZ + Rcos30 > pos.z && fCurZ - Rcos30 < pos.z)
				{
					fCurY = 0.0f;
					for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
					{
						bFound = true;
						if(fCurY > pos.y)
						{
							if(HC_BIT_SET(m_pMatrix[HCC_INDEX(i, j, k)].GetType(), HC_BLOCK_TYPE_FULL))
							{
								return(fCurY - pos.y);
							}
							else
							{
								return(-1.0f); //?
							}
						}

						fCurY += H;
					}
				}
				fCurZ += Rcos30x2;
			}
		}
		fCurX += R * 1.5f;
	}
	//	fCurY += H;
	//}
	return(bFound ? -1.0f : (m_pNeighbors[HC_CHUNK_NEIGHBOR_EAST] ? m_pNeighbors[HC_CHUNK_NEIGHBOR_EAST]->IsOnGround(fOrigPos) : -0.0f));
*/
}
#endif

CChunk* CChunk::getHeighbour(UINT which)
{
	switch(which)
	{
	case HC_CHUNK_NEIGHBOR_SOUTH:
		return(m_pChunkManager->getChunk(m_iXpos + 1, m_iYpos, false));
	case HC_CHUNK_NEIGHBOR_NORTH:
		return(m_pChunkManager->getChunk(m_iXpos - 1, m_iYpos, false));
	case HC_CHUNK_NEIGHBOR_EAST:
		return(m_pChunkManager->getChunk(m_iXpos, m_iYpos + 1, false));
	case HC_CHUNK_NEIGHBOR_WEST:
		return(m_pChunkManager->getChunk(m_iXpos, m_iYpos - 1, false));
	}
}

WORD CChunk::getBlockOffset(int iXpos, int iYpos, int iZ, int iXoffset, int iYoffset)
{
	if(iZ >= HC_CHUNK_SIZE_Y || iZ < 0)
	{
		return(NULL);
	}
	int iX = iXpos + iXoffset;
	int iY = iYpos + iYoffset;


	CChunk * pCurChunk = this;
	while(iX >= HC_CHUNK_SIZE_X && pCurChunk)
	{
		iX -= HC_CHUNK_SIZE_X;
		pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_SOUTH);
	}

	while(iX < 0 && pCurChunk)
	{
		iX += HC_CHUNK_SIZE_X;
		pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_NORTH);
	}

	while(iY >= HC_CHUNK_SIZE_Z && pCurChunk)
	{
		iY -= HC_CHUNK_SIZE_Z;
		pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_EAST);
	}

	while(iY < 0 && pCurChunk)
	{
		iY += HC_CHUNK_SIZE_Z;
		pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_WEST);
	}

	if(pCurChunk)
	{
		//return(&pCurChunk->m_pppMatrix[iZ][iX][iY]);
		return(pCurChunk->m_pMatrix[HCC_INDEX(iZ, iX, iY)]);
	}
	return(NULL);
}

#if 0
void CChunk::fixPlayerPos(DSHCplayer * pl)
{
	if(!m_bReady)
	{
		return;
	}
	XMFLOAT3 fOrigPos = pl->GetPos();
	XMFLOAT3 pos = fOrigPos;
	float fCurY = 0.0f;

	int pX = 0;
	int pZ = 0;
	int pY = 0;
	float fX = 0.0f;
	float fCurZ = 0.0f;
	//float fCurY = 0.0f;
	//FindUserInChunk(pos.x, pos.z, &pX, &pZ, &fX, &fCurZ);
	pl->GetBlockInPos(&fX, &fCurY, &fCurZ);
	fCurY = 0.0f;
	pl->GetBlockIn(&pX, &pY, &pZ);

	pos.x -= fX;
	pos.z -= fCurZ;

	UINT j = pX;
	UINT k = pZ;
	fX = 0;
	fCurZ = 0;
	for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
	{
		//bFound = true;
		//if(fCurY < pos.y && fCurY + HC_H * 1.8 >= pos.y)
		if(fCurY >= pos.y && fCurY - pos.y < 1.8f * HC_H) //@@TODO: Fix that
		{
			WORD pBlock = NULL;

			if((pBlock = GetBlockOffset(j, k, i + 1, 0, 1)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					//float d = pBlock->GetPos();
					float d = Rcos30 - pos.z;
					if(d < HC_PR)
					{
						fOrigPos.z -= HC_PR - d;
						pos.z -= HC_PR - d;
					}

					//DS_ASSERT(abs(HC_PR - d) < 10.0f);
					//return(fOrigPos);
				}
			}
			if((pBlock = GetBlockOffset(j, k, i + 1, 0, -1)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					//float d = pBlock->GetPos();
					float d = pos.z + Rcos30;
					if(d < HC_PR)
					{
						fOrigPos.z += HC_PR - d;
						pos.z += HC_PR - d;
					}
					//DS_ASSERT(abs(HC_PR - d) < 10.0f);
					//return(fOrigPos);
				}
			}
			int z = (j & 1) ? k : k - 1;
			if((pBlock = GetBlockOffset(j, z, i + 1, -1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(-Rdiv2, 0.0f, -Rcos30);
					XMFLOAT3 p1(-HC_R, 0.0f, 0.0f);
					float A = -(p2.z - p1.z);
					float B = p2.x - p1.x;
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
						DS_ASSERT(abs(fix.z) < 10.0f);
					}

					
				}
				//XMFLOAT3 q = p2 - p1;
			}

			if((pBlock = GetBlockOffset(j, z, i + 1, 1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(Rdiv2, 0.0f, -Rcos30);
					XMFLOAT3 p1(HC_R, 0.0f, 0.0f);
					float A = (p2.z - p1.z);
					float B = -(p2.x - p1.x);
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
						DS_ASSERT(abs(fix.z) < 10.0f);
					}
				}
			}

			if((pBlock = GetBlockOffset(j, z + 1, i + 1, -1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(-Rdiv2, 0.0f, Rcos30);
					XMFLOAT3 p1(-HC_R, 0.0f, 0.0f);
					float A = (p2.z - p1.z);
					float B = -(p2.x - p1.x);
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
						DS_ASSERT(abs(fix.z) < 10.0f);
					}
				}
			}

			if((pBlock = GetBlockOffset(j, z + 1, i + 1, 1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(Rdiv2, 0.0f, Rcos30);
					XMFLOAT3 p1(HC_R, 0.0f, 0.0f);
					float A = -(p2.z - p1.z);
					float B = (p2.x - p1.x);
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
						DS_ASSERT(abs(fix.z) < 10.0f);
					}
				}
			}
			if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(i, j, k)]), HC_BLOCK_TYPE_FULL))
			{
				//return(fCurY - pos.y);
				int a = 3;
				a++;
				//if(/ *some checks* /)
				//{

				//}
			}
			//return(fOrigPos);
			{
				XMFLOAT3 cp = pl->GetPos() - fOrigPos;
				if(cp.x > 10.0f || cp.y > 10.0f || cp.z > 10.0f)
				{
					//DS_DBG_BREAK;
				}
			}
			pl->SetPos(fOrigPos);
			//return;
		}

		fCurY += HC_H;
	}
	//pl->SetPos(pos);
	//return(pos);
}

XMFLOAT3 CChunk::FixPlayerPos(XMFLOAT3 pos)
{
	//return(pos);
	//DS_ASSERT(false);
	if(!m_bReady)
	{
		//return(0.0f);
		return(pos);
	}
	XMFLOAT3 fOrigPos = pos;
	float fCurY = 0.0f;

	UINT pX = 0;
	UINT pZ = 0;
	float fX = 0;
	float fCurZ = 0;
	FindUserInChunk(pos.x, pos.z, &pX, &pZ, &fX, &fCurZ);

	pos.x -= fX;
	pos.z -= fCurZ;

	UINT j = pX;
	UINT k = pZ;
	fX = 0;
	fCurZ = 0;
	for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
	{
		//bFound = true;
		if(fCurY >= pos.y && fCurY - pos.y < 1.8f * HC_H)
		{
			WORD pBlock = NULL;
			
			if((pBlock = GetBlockOffset(j, k, i + 1, 0, 1)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					//float d = pBlock->GetPos();
					float d = Rcos30 - pos.z;
					if(d < HC_PR)
					{
						fOrigPos.z -= HC_PR - d;
						pos.z -= HC_PR - d;
					}
					//return(fOrigPos);
				}
			}
			if((pBlock = GetBlockOffset(j, k, i + 1, 0, -1)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					//float d = pBlock->GetPos();
					float d = pos.z + Rcos30;
					if(d < HC_PR)
					{
						fOrigPos.z += HC_PR - d;
						pos.z += HC_PR - d;
					}
					//return(fOrigPos);
				}
			}
			int z = (j & 1) ? k : k - 1;
			if((pBlock = GetBlockOffset(j, z, i + 1, -1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(-Rdiv2, 0.0f, -Rcos30);
					XMFLOAT3 p1(-HC_R, 0.0f, 0.0f);
					float A = -(p2.z - p1.z);
					float B = p2.x - p1.x;
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
					}
				}
				//XMFLOAT3 q = p2 - p1;
			}

			if((pBlock = GetBlockOffset(j, z, i + 1, 1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(Rdiv2, 0.0f, -Rcos30);
					XMFLOAT3 p1(HC_R, 0.0f, 0.0f);
					float A = (p2.z - p1.z);
					float B = -(p2.x - p1.x);
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
					}
				}
			}

			if((pBlock = GetBlockOffset(j, z + 1, i + 1, -1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(-Rdiv2, 0.0f, Rcos30);
					XMFLOAT3 p1(-HC_R, 0.0f, 0.0f);
					float A = (p2.z - p1.z);
					float B = -(p2.x - p1.x);
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
					}
				}
			}

			if((pBlock = GetBlockOffset(j, z + 1, i + 1, 1, 0)))
			{
				if(HC_BIT_SET(HC_GET_TYPE(pBlock), HC_BLOCK_TYPE_FULL))
				{
					XMFLOAT3 p2(Rdiv2, 0.0f, Rcos30);
					XMFLOAT3 p1(HC_R, 0.0f, 0.0f);
					float A = -(p2.z - p1.z);
					float B = (p2.x - p1.x);
					float l = sqrtf(A * A + B * B);
					XMFLOAT3 n;
					float d;
					if(((p2.x - p1.x) * (p2.x - pos.x) + (p2.z - p1.z) * (p2.z - pos.z)) < 0 ||
						((p1.x - p2.x) * (p1.x - pos.x) + (p1.z - p2.z) * (p1.z - pos.z)) < 0)
					{
						float d1 = sqrtf((pos.x - p1.x) * (pos.x - p1.x) + (pos.z - p1.z) * (pos.z - p1.z));
						float d2 = sqrtf((pos.x - p2.x) * (pos.x - p2.x) + (pos.z - p2.z) * (pos.z - p2.z));
						d = min(d1, d2);
						if(d1 < d2)
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						else
						{
							n = XMFLOAT3(pos.x - p1.x, 0.0f, pos.z - p1.z);
						}
						float l = sqrtf(n.x * n.x + n.z * n.z);
						n = n / l;
					}
					else
					{
						d = abs(A * pos.x + B * pos.z + B * p1.z - A * p1.x) / l;
						n = XMFLOAT3(A / l, 0.0f, B / l);
					}
					if(d < HC_PR)
					{
						XMFLOAT3 fix = n * (HC_PR - d);
						fOrigPos = fOrigPos + fix;
						pos = fOrigPos + fix;
					}
				}
			}
			/*if((pBlock = GetBlockOffset(j, k, i + 2, 0, 1)))
			{
			if(HC_BIT_SET(pBlock->GetType(), HC_BLOCK_TYPE_FULL))
			{
			//float d = pBlock->GetPos();
			float d = (fCurZ + Rcos30) - pos.z;
			if(d < Rpl)
			{
			fOrigPos.z -= Rpl - d;
			}
			//return(fOrigPos);
			}
			}*/
			if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(i, j, k)]), HC_BLOCK_TYPE_FULL))
			{
				//return(fCurY - pos.y);
				int a = 3;
				a++;
				//if(/ *some checks* /)
				//{

				//}
			}
			return(fOrigPos);
		}

		fCurY += HC_H;
	}







	return(pos);
	/*
	const float R = HC_R;
	const float H = HC_H;

	const float Rpl = 4.0f;

	pos.x -= (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * R;
	pos.z -= (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2;

	float fCurX;
	float fCurY = 0;
	float fCurZ;

	bool bFound = false;
	//for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
	//{
	fCurX = 0;
	for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
	{
		if(fCurX + R * 3.0f * 0.25f > pos.x && fCurX - R * 3.0f * 0.25f <= pos.x)
		{
			fCurZ = j % 2 ? 0.0f : -Rcos30;
			for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
			{
				if(fCurZ + Rcos30 * 1.5f + 1.0 > pos.z && fCurZ - Rcos30 * 1.5f - 1.0f <= pos.z)
					//if(fCurZ + Rcos30 > pos.z && fCurZ - Rcos30 < pos.z)
				{
					/ *fCurZ += Rcos30x2;
					if(!(fCurZ + Rcos30 * 1.5f + 1.0 > pos.z && fCurZ - Rcos30 * 1.5f - 1.0f <= pos.z))
					{
						fCurZ -= Rcos30x2;
					}* /
					fCurY = 0.0f;
					for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
					{
						bFound = true;
						if(fCurY >= pos.y && fCurY - pos.y < 2.0f * H)
						{
							CSolidBlock * pBlock = NULL;
							//z = j % 2 ? k : k - 1;
							if((pBlock = GetBlockOffset(j, k, i + 1, 0, 1)))
							{
								if(HC_BIT_SET(pBlock->GetType(), HC_BLOCK_TYPE_FULL))
								{
									//float d = pBlock->GetPos();
									float d = (fCurZ + Rcos30) - pos.z;
									if(d < Rpl)
									{
										fOrigPos.z -= Rpl - d;
									}
									//return(fOrigPos);
								}
							}
							if((pBlock = GetBlockOffset(j, k, i + 1, 0, -1)))
							{
								if(HC_BIT_SET(pBlock->GetType(), HC_BLOCK_TYPE_FULL))
								{
									//float d = pBlock->GetPos();
									float d = pos.z - (fCurZ - Rcos30);
									if(d < Rpl)
									{
										fOrigPos.z += Rpl - d;
									}
									//return(fOrigPos);
								}
							}
							/ *if((pBlock = GetBlockOffset(j, k, i + 2, 0, 1)))
							{
								if(HC_BIT_SET(pBlock->GetType(), HC_BLOCK_TYPE_FULL))
								{
									//float d = pBlock->GetPos();
									float d = (fCurZ + Rcos30) - pos.z;
									if(d < Rpl)
									{
										fOrigPos.z -= Rpl - d;
									}
									//return(fOrigPos);
								}
							}* /
							if(HC_BIT_SET(m_pppMatrix[i][j][k].GetType(), HC_BLOCK_TYPE_FULL))
							{
								//return(fCurY - pos.y);
								int a = 3;
								a++;
								//if(/ *some checks* /)
								//{

								//}
							}
							return(fOrigPos);
						}

						fCurY += H;
					}
				}
				//m_pppMatrix[i][j][k].CreateMesh(0, fCurX, fCurY, fCurZ, R, H, pVertices[0], pIndices[0], &iCurVertex[0], &iCurIndex[0], b);
				fCurZ += Rcos30x2;
			}
		}
		fCurX += R * 1.5f;
	}
	//	fCurY += H;
	//}
//	return(bFound ? -1.0f : (m_pNeighbors[HC_CHUNK_NEIGHBOR_EAST] ? m_pNeighbors[HC_CHUNK_NEIGHBOR_EAST]->IsOnGround(pos) : -0.0f));
	return(fOrigPos);*/
}
#endif

/*std::string CChunk::GetFileName(bool bCreate)
{
	std::string name = DScVarSystem::get("game") + "/blocks";
	int64_t ax = m_iXpos;
	int64_t ay = m_iYpos;
	int bx = 0;
	int by = 0;
	std::string path = "";

	for(int i = 0; i < 7; i++)
	{
		bx = ax % 1000;
		by = ay % 1000;
		ay = ay / 1000;
		ax = ax / 1000;
		path = "/" + numToStr(bx) + '_' + numToStr(by) + path;
	}
	name += path;
	if(bCreate)
	{
		std::string n = name;
		std::string dir = "";
		int aa;
		do
		{
			aa = n.find('/');
			dir += n.substr(0, aa + 1);
			n = n.substr(aa + 1);
			CreateDirectoryA(dir.c_str(), NULL);
		}
		while(n.length() && aa >= 0);
	}
	
	name += ".dse";
	return(name);
}*/

String CChunk::getFileName()
{
	String name = String("levels/")+m_pChunkManager->getLevelName()+"/blocks";
	int64_t ax = m_iXpos;
	int64_t ay = m_iYpos;
	short int bx = 0;
	short int by = 0;
	String path;

	for(int i = 0; i < 4; i++)
	{
		bx = ax & 0xFFFF;
		by = ay & 0xFFFF;
		ay >>= 16;
		ax >>= 16;
		path = String("/") + bx + '_' + by + path;
	}
	name += path;
	
	name += ".dse";
	return(name);
}

void CChunk::writeFile()
{
	/*if(!m_pMatrix)
	{
		return;
	}*/
	String name = getFileName();
	IFile *pFile = m_pChunkManager->getFileSystem()->openFile(name.c_str(), FILE_MODE_WRITE);
	if(!pFile)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to write chunk %s\n", name.c_str());
		return;
	}
	CChunkHeader ch;
	
	//fwrite(m_pMatrix, sizeof(UINT), 1, pF);
	//fwrite(m_pMatrix, sizeof(CBaseBlock), HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Y * HC_CHUNK_SIZE_Z, pF);
	UINT len = HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Z * HC_CHUNK_SIZE_Y * sizeof(WORD);
	byte * pData = new byte[len];
	byte * pCData;
	UINT Clen;
	UINT ptr = 0;
	/*for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
	{
		for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
		{
			for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
			{
				//UINT a = HC_MKTYPE(m_pppMatrix[i][j][k].GetType(), m_pppMatrix[i][j][k].GetTypeID());
				WORD a = m_pMatrix[HCC_INDEX(i, j, k)]->GetTypeFull();
				//fwrite(&a, sizeof(WORD), 1, pF);
				memcpy(&pData[ptr], &a, sizeof(WORD));
				ptr += sizeof(WORD);
			}
		}
	}*/
	memcpy(pData, m_pMatrix, len);
	DScoder::Pack(pData, len, &pCData, &Clen);
	ch.iDataLen = Clen;
	pFile->writeBin(&ch, sizeof(ch));
	pFile->writeBin(pCData, Clen);
	mem_delete_a(pData);
	mem_delete_a(pCData);
	mem_release(pFile);
}

bool CChunk::readFile()
{
	String name = getFileName();
	//name = DScVarSystem::get("game") + "/blocks/" + name + ".dse";
	IFile *pFile = m_pChunkManager->getFileSystem()->openFile(name.c_str());
	
	if(!pFile)
	{
		//DS_WARNING("Unable to read chunk");
		return(false);
	}
	CChunkHeader ch;
	UINT c = pFile->readBin(&ch, sizeof(ch));
	if(c < sizeof(ch))
	{
		mem_release(pFile);
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load chunk \"%s\". Seems the file was corrupt. Regenerating...\n", name.c_str());
		return(false);
	}
	
	UINT Clen = ch.iDataLen;
	UINT len;
	byte * pCData = new byte[Clen];
	byte * pData;
	//fread(m_pMatrix, sizeof(CBaseBlock), HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Y * HC_CHUNK_SIZE_Z, pF);
	pFile->readBin(pCData, Clen);
	mem_release(pFile);
	UINT ptr = 0;
	DScoder::UnPack(pCData, Clen, &pData, &len);
	mem_delete_a(pCData);
	if(!pData)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load chunk \"%s\". Seems the file was corrupt. Regenerating...\n", name.c_str());
		return(false);
	}
	memcpy(m_pMatrix, pData, sizeof(WORD) * HC_CHUNK_SIZE_X * HC_CHUNK_SIZE_Z * HC_CHUNK_SIZE_Y);
	/*for(int j = 0; j < HC_CHUNK_SIZE_X; j++)
	{
		for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
		{
			for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
			{
				WORD a;
				memcpy(&a, &pData[ptr], sizeof(WORD));
				ptr += sizeof(WORD);
				//fread(&a, sizeof(UINT), 1, pF);
				//m_pppMatrix[i][j][k].SetType(HC_GET_TYPE(a));
				//m_pMatrix[HCC_INDEX(i, j, k)].SetTypeFull(a);
				m_pMatrix[HCC_INDEX(i, j, k)] = HC_NEW_BLOCK(a, m_pBlockPool);
				//m_pppMatrix[i][j][k].Set(HC_GET_TYPE(a));
			}
		}
	}*/
	mem_delete_a(pData);
	return(true);
}

#if 0
void CChunk::LODinit(DSHCLOD * pLOD)
{
	return;
	//int64_t iX = m_iXpos + (HC_GEN_SIZE / 2);
	//int64_t iZ = m_iYpos + (HC_GEN_SIZE / 2);

	int64_t iX = m_iXpos;
	int64_t iZ = m_iYpos;

	const CChunkManager::DSHCCC64RECT * pRect = DS::pDSHCCM->GetRRect();
	
	//bool b = !m_pNeighbors[HC_CHUNK_NEIGHBOR_NORTH] && !m_pNeighbors[HC_CHUNK_NEIGHBOR_SOUTH] && !m_pNeighbors[HC_CHUNK_NEIGHBOR_EAST] && !m_pNeighbors[HC_CHUNK_NEIGHBOR_WEST];
	
	bool b = iX > pRect->Xmax || iX < pRect->Xmin || iZ > pRect->Zmax || iZ < pRect->Zmin;
	
	for(int i = 0; i < HC_CHUNK_SIZE_X; i++)
	{
		for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
		{
			UINT s = 0;
			if(b)
			{
				for(int j = HC_CHUNK_SIZE_Y - 1; j >= 0; j--)
				{
					//if(HC_BIT_SET(m_pppMatrix[j][i][k].GetType(), HC_BLOCK_TYPE_FULL))
					if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(j, i, k)]), HC_BLOCK_TYPE_FULL))
					{
						s = j;
						break;
					}
				}
			}
			(*pLOD)[iX * HC_CHUNK_SIZE_X + i][iZ * HC_CHUNK_SIZE_Z + k] = s;
		}
	}

	//if(!m_pNeighbors[HC_CHUNK_NEIGHBOR_EAST])
	if(iZ == pRect->Zmax)
	{
		for(int i = 0; i < HC_CHUNK_SIZE_X; i++)
		{
			int k = HC_CHUNK_SIZE_Z - 1;
			UINT s = 0;

			for(int j = HC_CHUNK_SIZE_Y - 1; j >= 0; j--)
			{
				if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(j, i, k)]), HC_BLOCK_TYPE_FULL))
				//if(HC_BIT_SET(m_pppMatrix[j][i][k].GetType(), HC_BLOCK_TYPE_FULL))
				{
					s = j;
					break;
				}
			}

			(*pLOD)[iX * HC_CHUNK_SIZE_X + i][iZ * HC_CHUNK_SIZE_Z + k] = s > 1 && !b ? s - 1 : s;
		}
	}
	//if(!m_pNeighbors[HC_CHUNK_NEIGHBOR_WEST])
	if(iZ == pRect->Zmin)
	{
		for(int i = 0; i < HC_CHUNK_SIZE_X; i++)
		{
			int k = 0;
			UINT s = 0;

			for(int j = HC_CHUNK_SIZE_Y - 1; j >= 0; j--)
			{
				//if(HC_BIT_SET(m_pppMatrix[j][i][k].GetType(), HC_BLOCK_TYPE_FULL))
				if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(j, i, k)]), HC_BLOCK_TYPE_FULL))
				{
					s = j;
					break;
				}
			}

			(*pLOD)[iX * HC_CHUNK_SIZE_X + i][iZ * HC_CHUNK_SIZE_Z + k] = s > 1 && !b ? s - 1 : s;
		}
	}

	//if(!m_pNeighbors[HC_CHUNK_NEIGHBOR_SOUTH])
	if(iX == pRect->Xmax)
	{
		int i = HC_CHUNK_SIZE_X - 1;
		for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
		{
			UINT s = 0;

			for(int j = HC_CHUNK_SIZE_Y - 1; j >= 0; j--)
			{
				if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(j, i, k)]), HC_BLOCK_TYPE_FULL))
				//if(HC_BIT_SET(m_pppMatrix[j][i][k].GetType(), HC_BLOCK_TYPE_FULL))
				{
					s = j;
					break;
				}
			}

			(*pLOD)[iX * HC_CHUNK_SIZE_X + i][iZ * HC_CHUNK_SIZE_Z + k] = s > 1 && !b ? s - 1 : s;
		}
	}
	//if(!m_pNeighbors[HC_CHUNK_NEIGHBOR_NORTH])
	if(iX == pRect->Xmin)
	{
		int i = 0;
		for(int k = 0; k < HC_CHUNK_SIZE_Z; k++)
		{
			UINT s = 0;

			for(int j = HC_CHUNK_SIZE_Y - 1; j >= 0; j--)
			{
				//if(HC_BIT_SET(m_pppMatrix[j][i][k].GetType(), HC_BLOCK_TYPE_FULL))
				if(HC_BIT_SET(HC_GET_TYPE(m_pMatrix[HCC_INDEX(j, i, k)]), HC_BLOCK_TYPE_FULL))
				{
					s = j;
					break;
				}
			}

			(*pLOD)[iX * HC_CHUNK_SIZE_X + i][iZ * HC_CHUNK_SIZE_Z + k] = s > 1 && !b ? s - 1 : s;
		}
	}
}
#endif


void CChunk::findUserInChunk(float fX, float fZ, UINT * pX, UINT * pZ, float * fXblock, float * fZblock)
{
	int64_t px, pz;
	m_pChunkManager->getDelta(&px, &pz);
	float x = fX - (float)(HC_CHUNK_SIZE_X * (m_iXpos - px) / 2) * 3.0f * HC_R;
	float z = fZ - (float)(HC_CHUNK_SIZE_Z * (m_iYpos - pz)) * Rcos30x2;
	//z += Rcos30;

	//float fXcur = (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * HC_R;
	float fXcur = 0.0f;
	//float fZcur = (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2;
	float fZcur = -Rcos30;

	float fZofs0 = Rcos30x2;

	float fXofs = 1.5f * HC_R;
	float fZofs = Rcos30x2;

	int iXstripe = -1;
	int iZstripe = -1;

	for(int i = 0; i < HC_CHUNK_SIZE_X; i++) // through X dimension
	{
		if(x > fXcur - HC_R && x <= fXcur + HC_R)
		{
			if(x > (fXcur + HC_R * 1.5) - HC_R && x <= (fXcur + HC_R * 1.5) + HC_R)
			{
				//border checks
				int indz = (z - fZcur) / fZofs0;
				float cz = z - (fZcur + fZofs0 * indz);
				//float cx = fXcur + fXofs - HC_R;
				float cx = x - (fXcur + HC_R * 0.5f);
				if(cz < Rcos30)
				{
					if(cx < cz / (2 * cos30))
					{
						iXstripe = i;
					}
					else
					{
						iXstripe = i + 1;
						fXcur += HC_R * 1.5;
					}
				}
				else
				{
					if(cx < Rdiv2 - (cz - Rcos30) / (2 * cos30))
					{
						iXstripe = i;
					}
					else
					{
						iXstripe = i + 1;
						fXcur += HC_R * 1.5;
					}
				}
			}
			else
			{
				iXstripe = i;
			}
			break;
		}
		fXcur += HC_R * 1.5;
	}
	if(fXblock)
	{
		*fXblock = fXcur + (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * HC_R;
	}
	if(iXstripe >= 0)
	{
		fZcur = (iXstripe & 1) ? 0.0f : -Rcos30;
		for(int j = 0; j < HC_CHUNK_SIZE_Z; j++)
		{
			if((z > fZcur - Rcos30 || j == 0) && (z <= fZcur + fZofs - Rcos30 || j == HC_CHUNK_SIZE_Z - 1))
			{
				if(z > fZcur + fZofs - Rcos30 && z <= fZcur + fZofs + Rcos30)
				{
					float cz = z - (fZcur + fZofs - Rcos30);
					//int indx = (x - fXcur) / (3.0f * HC_R);
					int indx = (x) / (3.0f * HC_R);
					float cx = x - (3.0f * HC_R) * indx;
					if(cx < Rdiv2 || cx >= 2.5f * HC_R) // ++
					{
						iZstripe = j + 1;
						fZcur += fZofs;
					}
					else if(cx >= HC_R && cx < 2.0f * HC_R) // --
					{
						iZstripe = j;
					}
					else if(cx < HC_R && cx >= Rdiv2) // +- zOne
					{
						if(cx >= Rdiv2 + 0.5f * cz) // --
						{
							iZstripe = j;
						}
						else // ++
						{
							iZstripe = j + 1;
							fZcur += fZofs;
						}
					}
					else // -+ zOne
					{
						if(cx >= 2.5f * HC_R - 0.5f * cz) // ++
						{
							iZstripe = j + 1;
							fZcur += fZofs;
						}
						else // --
						{
							iZstripe = j;
						}
					}
				}
				else
				{
					iZstripe = j;
				}
				break;
			}
			fZcur += fZofs;
		}
		if(fZblock)
		{
			*fZblock = fZcur + (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2;
		}
	}
	if(pX)
	{
		*pX = iXstripe % HC_CHUNK_SIZE_X;
	}
	if(pZ)
	{
		*pZ = iZstripe % HC_CHUNK_SIZE_Z;
	}
}


#if 0
void CChunk::FindUserInChunk(DSHCplayer * pl, UINT * pX, UINT * pZ, UINT * pY, float * fXblock, float * fZblock, float * fYblock)
{
	XMFLOAT3 pos = pl->GetPos();
	float fX = pos.x;
	float fZ = pos.z;
	int64_t px, pz;
	DS::pDSHCCM->GetDelta(&px, &pz);
	float x = fX - (float)(HC_CHUNK_SIZE_X * (m_iXpos - px) / 2) * 3.0f * HC_R;
	float z = fZ - (float)(HC_CHUNK_SIZE_Z * (m_iYpos - pz)) * Rcos30x2;
	//z += Rcos30;

	//float fXcur = (float)(HC_CHUNK_SIZE_X * m_iXpos / 2) * 3.0f * HC_R;
	float fXcur = 0.0f;
	//float fZcur = (float)(HC_CHUNK_SIZE_Z * m_iYpos) * Rcos30x2;
	float fZcur = -Rcos30;

	float fZofs0 = Rcos30x2;

	float fXofs = 1.5f * HC_R;
	float fZofs = Rcos30x2;

	int iXstripe = -1;
	int iZstripe = -1;

	for(int i = 0; i < HC_CHUNK_SIZE_X; i++) // through X dimension
	{
		if(x > fXcur - HC_R && x <= fXcur + HC_R)
		{
			if(x > (fXcur + HC_R * 1.5) - HC_R && x <= (fXcur + HC_R * 1.5) + HC_R)
			{
				//border checks
				int indz = (z - fZcur) / fZofs0;
				float cz = z - (fZcur + fZofs0 * indz);
				//float cx = fXcur + fXofs - HC_R;
				float cx = x - (fXcur + HC_R * 0.5f);
				if(cz < Rcos30)
				{
					if(cx < cz / (2 * cos30))
					{
						iXstripe = i;
					}
					else
					{
						iXstripe = i + 1;
						fXcur += HC_R * 1.5;
					}
				}
				else
				{
					if(cx < Rdiv2 - (cz - Rcos30) / (2 * cos30))
					{
						iXstripe = i;
					}
					else
					{
						iXstripe = i + 1;
						fXcur += HC_R * 1.5;
					}
				}
			}
			else
			{
				iXstripe = i;
			}
			break;
		}
		fXcur += HC_R * 1.5;
	}
	if(fXblock)
	{
		*fXblock = fXcur + (float)(HC_CHUNK_SIZE_X * (m_iXpos - px) / 2) * 3.0f * HC_R;
	}
	if(iXstripe >= 0)
	{
		fZcur = (iXstripe & 1) ? 0.0f : -Rcos30;
		for(int j = 0; j < HC_CHUNK_SIZE_Z; j++)
		{
			if((z > fZcur - Rcos30 || j == 0) && (z <= fZcur + fZofs - Rcos30 || j == HC_CHUNK_SIZE_Z - 1))
			{
				if(z > fZcur + fZofs - Rcos30 && z <= fZcur + fZofs + Rcos30)
				{
					float cz = z - (fZcur + fZofs - Rcos30);
					//int indx = (x - fXcur) / (3.0f * HC_R);
					int indx = (x) / (3.0f * HC_R);
					float cx = x - (3.0f * HC_R) * indx;
					if(cx < Rdiv2 || cx >= 2.5f * HC_R) // ++
					{
						iZstripe = j + 1;
						fZcur += fZofs;
					}
					else if(cx >= HC_R && cx < 2.0f * HC_R) // --
					{
						iZstripe = j;
					}
					else if(cx < HC_R && cx >= Rdiv2) // +- zOne
					{
						if(cx >= Rdiv2 + 0.5f * cz) // --
						{
							iZstripe = j;
						}
						else // ++
						{
							iZstripe = j + 1;
							fZcur += fZofs;
						}
					}
					else // -+ zOne
					{
						if(cx >= 2.5f * HC_R - 0.5f * cz) // ++
						{
							iZstripe = j + 1;
							fZcur += fZofs;
						}
						else // --
						{
							iZstripe = j;
						}
					}
				}
				else
				{
					iZstripe = j;
				}
				break;
			}
			fZcur += fZofs;
		}
		if(fZblock)
		{
			*fZblock = fZcur + (float)(HC_CHUNK_SIZE_Z * (m_iYpos - pz)) * Rcos30x2;
		}
	}
	if(pX)
	{
		*pX = iXstripe % HC_CHUNK_SIZE_X;
	}
	if(pZ)
	{
		*pZ = iZstripe % HC_CHUNK_SIZE_Z;
	}
	float fCurY = 0.0f;
	int i;
	/*for(i = 0; i < HC_CHUNK_SIZE_Y; i++)
	{
		if(fCurY >= pos.y)
		{
			if(HC_BIT_SET(m_pMatrix[HCC_INDEX(i, *pX, *pZ)].GetType(), HC_BLOCK_TYPE_FULL))
			{
				//return(fCurY - pos.y);
				break;
			}
		}

		fCurY += HC_H;
	}*/
	i = pos.y / HC_H;
	fCurY = HC_H * (float)i;
	if(pY)
	{
		*pY = i;
	}
	if(fYblock)
	{
		*fYblock = fCurY;
	}
}
#endif


void CChunk::findAimingBlock(const float3 &pos, const float3 &vec, float len, DSHCaimPos * pAP)
{
	pAP->iAimType = 0;
	/*if(!m_bReady)
	{
		return;
	}*/
	__try
	{
		UINT pX = 0;
		UINT pZ = 0;
		findUserInChunk(pos.x, pos.z, &pX, &pZ);

		float fCurY = 0.0f;
		for(int i = 0; i < HC_CHUNK_SIZE_Y; i++)
		{
			if(fCurY < pos.y && fCurY + HC_H >= pos.y)
				//if(fCurY >= pos.y - HC_H * 2.0f && fCurY - pos.y < 1.8f * HC_H + HC_H * 2.0f)
			{
				WORD pBlock = NULL;
				bool inv = (pX & 1);
				for(int h = 0; h < 4; h++)
				{
					const int offsetCount = 19;
					int offsets[19][2] = {
						{0, 0},

						{1, 0},
						{1, inv ? 1 : -1},
						{-1, 0},
						{-1, inv ? 1 : -1},
						{0, 1},
						{0, -1},

						{0, -2},
						{0, 2},
						{1, inv ? -1 : 1},
						{1, inv ? 2 : -2},
						{-1, inv ? -1 : 1},
						{-1, inv ? 2 : -2},
						{-2, -1},
						{-2, 0},
						{-2, +1},
						{2, -1},
						{2, 0},
						{2, 1}
					};
					for(int o = 0; o < offsetCount; o++)
					{
						if((pBlock = getBlockOffset(pX, pZ, i + h, offsets[o][0], offsets[o][1])))
						{
							if(HC_IS_FULL(HC_GET_TYPE(pBlock)))
							{
								float3 bpos = getBlockPos(pX + offsets[o][0], pZ + offsets[o][1], i + h);
								UINT wh;
								
								if((wh = CBlockManager::CheckVecCollision(pBlock, pos, vec, bpos)))
								{
									pAP->iAimType = wh;
									pAP->iAimPosX = pX + offsets[o][0];
									pAP->iAimPosY = pZ + offsets[o][1];
									pAP->iAimPosZ = i + h;
									pAP->pChunk = this;
									pAP->vBPos = bpos;
									/*if(DS::pKeyBoard->KeyState(DSKEY_LBUTTON))
									{
										RemoveBlock(pX + offsets[o][0], pZ + offsets[o][1], i + h);
									}
									else if(DS::pKeyBoard->KeyState(DSKEY_RBUTTON))
									{*/
										//DSconsole::write("Plane: %d\n", wh);
										switch(wh)
										{
										case 1:
											pAP->iNewPosX = pX + offsets[o][0] - 1;
											pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1);
											pAP->iNewPosZ = i + h;
											//SetBlock(pX + offsets[o][0] - 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 2:
											pAP->iNewPosX = pX + offsets[o][0] - 1;
											pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0);
											pAP->iNewPosZ = i + h;
											//SetBlock(pX + offsets[o][0] - 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 3:
											pAP->iNewPosX = pX + offsets[o][0];
											pAP->iNewPosY = pZ + offsets[o][1] + 1;
											pAP->iNewPosZ = i + h;
											//SetBlock(pX + offsets[o][0], pZ + offsets[o][1] + 1, i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 4:
											pAP->iNewPosX = pX + offsets[o][0] + 1;
											pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0);
											pAP->iNewPosZ = i + h;
											//SetBlock(pX + offsets[o][0] + 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 5:
											pAP->iNewPosX = pX + offsets[o][0] + 1;
											pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1);
											pAP->iNewPosZ = i + h;
											//SetBlock(pX + offsets[o][0] + 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 6:
											pAP->iNewPosX = pX + offsets[o][0];
											pAP->iNewPosY = pZ + offsets[o][1];
											pAP->iNewPosZ = i + h + 1;
											//SetBlock(pX + offsets[o][0], pZ + offsets[o][1], i + h + 1, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 7:
											pAP->iNewPosX = pX + offsets[o][0];
											pAP->iNewPosY = pZ + offsets[o][1];
											pAP->iNewPosZ = i + h - 1;
											//SetBlock(pX + offsets[o][0], pZ + offsets[o][1], i + h - 1, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										case 8:
											pAP->iNewPosX = pX + offsets[o][0];
											pAP->iNewPosY = pZ + offsets[o][1] - 1;
											pAP->iNewPosZ = i + h;
											//SetBlock(pX + offsets[o][0], pZ + offsets[o][1] - 1, i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
											break;
										}
									/*}*/
									//DS::UTDrawLine(bpos, bpos);
									return;
								}
							}
						}
					}
					if(h != 0 && i - h >= 0)
					{
						for(int o = 0; o < offsetCount; o++)
						{
							if((pBlock = getBlockOffset(pX, pZ, i - h, offsets[o][0], offsets[o][1])))
							{
								if(HC_IS_FULL(HC_GET_TYPE(pBlock)))
								{
									float3 bpos = getBlockPos(pX + offsets[o][0], pZ + offsets[o][1], i - h);
									UINT wh;
									if((wh = CBlockManager::CheckVecCollision(pBlock, pos, vec, bpos)))
									{
										pAP->iAimType = wh;
										pAP->iAimPosX = pX + offsets[o][0];
										pAP->iAimPosY = pZ + offsets[o][1];
										pAP->iAimPosZ = i - h;
										pAP->pChunk = this;
										pAP->vBPos = bpos;
										//else if(DS::pKeyBoard->KeyState(DSKEY_RBUTTON))
										{
											//DSconsole::write("Plane: %d\n", wh);
											switch(wh)
											{
											case 1:
												pAP->iNewPosX = pX + offsets[o][0] - 1;
												pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1);
												pAP->iNewPosZ = i - h;
												//SetBlock(pX + offsets[o][0] - 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 2:
												pAP->iNewPosX = pX + offsets[o][0] - 1;
												pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0);
												pAP->iNewPosZ = i - h;
												//SetBlock(pX + offsets[o][0] - 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 3:
												pAP->iNewPosX = pX + offsets[o][0];
												pAP->iNewPosY = pZ + offsets[o][1] + 1;
												pAP->iNewPosZ = i - h;
												//SetBlock(pX + offsets[o][0], pZ + offsets[o][1] + 1, i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 4:
												pAP->iNewPosX = pX + offsets[o][0] + 1;
												pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0);
												pAP->iNewPosZ = i - h;
												//SetBlock(pX + offsets[o][0] + 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 1 : 0), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 5:
												pAP->iNewPosX = pX + offsets[o][0] + 1;
												pAP->iNewPosY = pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1);
												pAP->iNewPosZ = i - h;
												//SetBlock(pX + offsets[o][0] + 1, pZ + offsets[o][1] + ((pX + offsets[o][0]) & 1 ? 0 : -1), i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 6:
												pAP->iNewPosX = pX + offsets[o][0];
												pAP->iNewPosY = pZ + offsets[o][1];
												pAP->iNewPosZ = i - h + 1;
												//SetBlock(pX + offsets[o][0], pZ + offsets[o][1], i + h + 1, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 7:
												pAP->iNewPosX = pX + offsets[o][0];
												pAP->iNewPosY = pZ + offsets[o][1];
												pAP->iNewPosZ = i - h - 1;
												//SetBlock(pX + offsets[o][0], pZ + offsets[o][1], i + h - 1, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											case 8:
												pAP->iNewPosX = pX + offsets[o][0];
												pAP->iNewPosY = pZ + offsets[o][1] - 1;
												pAP->iNewPosZ = i - h;
												//SetBlock(pX + offsets[o][0], pZ + offsets[o][1] - 1, i + h, HC_MKTYPE(HC_BLOCK_TYPE_FULL, HC_BLOCK_SAND));
												break;
											}
										}
										//DS::UTDrawLine(bpos, bpos);
										return;
									}
								}
							}
						}
					}
				}
				/*if((pBlock = GetBlockOffset(pX, pZ, i - 1, 0, 1)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX, pZ + 1, i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX, pZ + 1, i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}

				if((pBlock = GetBlockOffset(pX, pZ, i - 1, 0, -1)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX, pZ - 1, i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX, pZ - 1, i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}


				if((pBlock = GetBlockOffset(pX, pZ, i - 1, 0, 0)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX, pZ, i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX, pZ, i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}


				if((pBlock = GetBlockOffset(pX, pZ, i - 1, 0, 0)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX, pZ, i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX, pZ, i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}

				if((pBlock = GetBlockOffset(pX, pZ, i - 1, 1, 0)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX + 1, pZ, i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX + 1, pZ, i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}

				if((pBlock = GetBlockOffset(pX, pZ, i - 1, -1, 0)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX - 1, pZ, i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX - 1, pZ, i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}

				if((pBlock = GetBlockOffset(pX, pZ, i - 1, -1, (pX & 1) ? 1 : -1)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX - 1, pZ + ((pX & 1) ? 1 : -1), i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX - 1, pZ + ((pX & 1) ? 1 : -1), i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);
				break;
				}
				}
				}

				if((pBlock = GetBlockOffset(pX, pZ, i - 1, 1, (pX & 1) ? 1 : -1)))
				{
				if(HC_IS_FULL(pBlock->GetType()))
				{
				XMFLOAT3 bpos = GetBlockPos(pX + 1, pZ + ((pX & 1) ? 1 : -1), i - 1);
				if(pBlock->CheckVecCollision(pos, vec, bpos))
				{
				if(DS::bNL)
				{
				RemoveBlock(pX + 1, pZ + ((pX & 1) ? 1 : -1), i - 1);
				DS::bNL = false;
				}
				DS::UTDrawLine(bpos, bpos);

				break;
				}
				}
				}*/
			}
			fCurY += HC_H;
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{

	}
}

float3 CChunk::getBlockPos(int x, int z, int y)
{
	int64_t px, pz;
	m_pChunkManager->getDelta(&px, &pz);
	return(float3(HC_R * ((float)(HC_CHUNK_SIZE_X * (m_iXpos - px) / 2) * 3.0f + x * 1.5f), y * HC_H, (float)(HC_CHUNK_SIZE_Z * (m_iYpos - pz)) * Rcos30x2 + Rcos30x2 * z + ((x & 1) ? 0.0f : -Rcos30)));
}

void CChunk::removeBlock(int iX, int iZ, int y)
{
	setBlock(iX, iZ, y, HC_BLOCK_TYPE_NONE);
}

void CChunk::setBlock(int iX, int iZ, int y, UINT type, bool reb)
{
	if(iX >= HC_CHUNK_SIZE_X || iX < 0 || iZ >= HC_CHUNK_SIZE_Z || iZ < 0)
	{
		CChunk * pCurChunk = this;
		while(iX >= HC_CHUNK_SIZE_X && pCurChunk)
		{
			iX -= HC_CHUNK_SIZE_X;
			pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_SOUTH);
		}

		while(iX < 0 && pCurChunk)
		{
			iX += HC_CHUNK_SIZE_X;
			pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_NORTH);
		}

		while(iZ >= HC_CHUNK_SIZE_Z && pCurChunk)
		{
			iZ -= HC_CHUNK_SIZE_Z;
			pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_EAST);
		}

		while(iZ < 0 && pCurChunk)
		{
			iZ += HC_CHUNK_SIZE_Z;
			pCurChunk = pCurChunk->getHeighbour(HC_CHUNK_NEIGHBOR_WEST);
		}

		if(pCurChunk)
		{
			pCurChunk->removeBlock(iX, iZ, y);
			//return(&pCurChunk->m_pppMatrix[iZ][iX][iY]);
			//return(&pCurChunk->m_pMatrix[HCC_INDEX(y, iX, iZ)]);
		}
	}
	else
	{
		m_bChanged = true;
		/*CBaseBlock * b = m_pMatrix[HCC_INDEX(y, iX, iZ)];
		m_pMatrix[HCC_INDEX(y, iX, iZ)] = HC_NEW_BLOCK(type, m_pBlockPool);
		HC_FREE_BLOCK(b, m_pBlockPool);*/
		m_pMatrix[HCC_INDEX(y, iX, iZ)] = type;
		if(reb)
		{
			reBuild();
			if(iX == HC_CHUNK_SIZE_X - 1)
			{
				getHeighbour(HC_CHUNK_NEIGHBOR_SOUTH)->reBuild();
			}
			if(iX == 0)
			{
				getHeighbour(HC_CHUNK_NEIGHBOR_NORTH)->reBuild();
			}

			if(iZ == HC_CHUNK_SIZE_Z - 1)
			{
				getHeighbour(HC_CHUNK_NEIGHBOR_EAST)->reBuild();
			}
			if(iZ == 0)
			{
				getHeighbour(HC_CHUNK_NEIGHBOR_WEST)->reBuild();
			}
		}
	}
}

void CChunk::reBuild()
{
	//m_bReady = false;
	buildBuffer2();
}

void XMETHODCALLTYPE CChunk::FinalRelease()
{
	m_pChunkManager->enqueueChunkDelete(this);
}

void CChunk::dematerialize()
{
	if(m_pEntity)
	{
		SGame_RemoveEntity(m_pEntity);
		m_pEntity = NULL;
	}
}
