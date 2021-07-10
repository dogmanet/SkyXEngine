#if 0

#include "DSbase.h"

#include "DSHCLOD.h"


DSHCLOD::DSHCLOD():pVertices(NULL), pIndices(NULL), m_bInRender(false), m_bInConstruct(false), m_pVertexNormalBuffer(NULL), m_iNormalCount(0), m_pHeights(NULL), m_iLevel(0), m_bBuilded(false), m_pIndexBuffer(NULL), m_pVertexBuffer(NULL), m_iIndexCount(0)
{
	Init();
}

DSHCLOD::~DSHCLOD()
{
	Release();
}

void DSHCLOD::Init()
{
	m_iStripeCount = HC_LOD_SIZE * (HC_CHUNK_SIZE_X >> m_iLevel);
	m_iStripeLength = HC_LOD_SIZE * (HC_CHUNK_SIZE_Z >> m_iLevel);
	m_pHeights = new DSHCLODstripe[m_iStripeCount];
	for(int i = 0; i < m_iStripeCount; i++)
	{
		m_pHeights[i].Init(m_iStripeLength);
	}
}

UINT DSHCLOD::GetLevel() const
{
	return(m_iLevel);
}

void DSHCLOD::Release()
{
	DS_SAFE_DELETE_A(m_pHeights);
	DS_SAFE_RELEASE(m_pIndexBuffer);
	DS_SAFE_RELEASE(m_pVertexBuffer);
	DS_SAFE_RELEASE(m_pVertexNormalBuffer);

	DS_SAFE_DELETE_A(pIndices);
	DS_SAFE_DELETE_A(pVertices);
}

void DSHCLOD::Render()
{
	return;
	if(!m_bBuilded)
	{
		return;
	}
	m_bInRender = true;

	DS_WAIT(m_bInConstruct);

	const float R = HC_R;
	//const CChunkManager::DSHCCC64RECT * RR = DS::pDSHCCM->GetRRect();
	UINT stride = sizeof(DSmaterialSystem::vertex);
	UINT offset = 0;
	if(m_iIndexCount)
	{
		//DSmaterialSystem::XMSetWord(XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		//DSmaterialSystem::XMSetWord(XMMatrixTranslation((float)(HC_CHUNK_SIZE_X * -(HC_GEN_SIZE / 2) / 2) * 3.0f * R, HC_H, (float)(HC_CHUNK_SIZE_Z * -(HC_GEN_SIZE / 2)) * Rcos30x2 - Rcos30));
		DSmaterialSystem::XMSetWord(m_mPos);
		DS::g_pImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		DSmaterialSystem::bindMaterial("/hc/block.grass.top");
		DS::g_pImmediateContext()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DS::g_pImmediateContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		DS::g_pImmediateContext()->DrawIndexed(m_iIndexCount, 0, 0);
	}

	if(m_iNormalCount && false)
	{
		DS::g_pImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		DSmaterialSystem::bindMaterial("/dev/white");
		DS::g_pImmediateContext()->IASetVertexBuffers(0, 1, &m_pVertexNormalBuffer, &stride, &offset);
		DS::g_pImmediateContext()->Draw(m_iNormalCount * 2, 0);
	}
	m_bInRender = false;
}

void DSHCLOD::Build()
{
	return;
	UINT iVertexCount = m_iStripeCount * m_iStripeLength;
	UINT iIndexCount = m_iStripeLength * 6 * m_iStripeCount;

	UINT iCurVertex = 0;
	UINT iCurIndex = 0;

	if(!pIndices)
	{
		pIndices = new UINT[iIndexCount];
	}
	if(!pVertices)
	{
		pVertices = new DSmaterialSystem::vertex[iVertexCount];
	}
	//DSmaterialSystem::vertex * pNormalVertices = new DSmaterialSystem::vertex[iVertexCount * 2];
	UINT iNormalVertexCount = 0;

	for(int i = 0; i < m_iStripeCount; i++)
	{
		for(int j = 0; j < m_iStripeLength; j++)
		{
			if(!m_pHeights[i].at(j))
			{
				iCurVertex++;
				continue;
			}
			pVertices[iCurVertex].Pos = XMFLOAT3(i * HC_R * 1.5f, m_pHeights[i].at(j) * HC_H, j * Rcos30x2 + (i % 2 ? +Rcos30 : 0.0f));

			pVertices[iCurVertex].Tex = XMFLOAT2((float)(i % HC_CHUNK_SIZE_X) / (float)HC_CHUNK_SIZE_X, (float)(j % HC_CHUNK_SIZE_Z) / (float)HC_CHUNK_SIZE_Z);

			//count normal
			{
				XMFLOAT3 norm(0.0f, 0.0f, 0.0f);
				XMFLOAT3 a, b, c, rn;
				XMVECTOR n;
				c = pVertices[i * m_iStripeLength + j].Pos;
				UINT cc = 0;
				if(i > 0)
				{
					if(j > 0)
					{
						if(i % 2 != 0)
						{
							a = pVertices[(i - 1) * m_iStripeLength + j].Pos - c;
							b = pVertices[i * m_iStripeLength + j - 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
						else
						{
							a = pVertices[(i - 1) * m_iStripeLength + j - 1].Pos - c;
							b = pVertices[i * m_iStripeLength + j - 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;

							//a = pVertices[(i - 1) * m_iStripeLength + j - 1].Pos - c;
							b = pVertices[(i - 1) * m_iStripeLength + j].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
					}
					if(j < m_iStripeLength - 1)
					{
						if(i % 2 != 0)
						{
							a = pVertices[(i - 1) * m_iStripeLength + j].Pos - c;
							b = pVertices[(i - 1) * m_iStripeLength + j + 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;

							a = pVertices[i * m_iStripeLength + j + 1].Pos - c;
							//b = pVertices[(i - 1) * m_iStripeLength + j + 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
						else
						{
							a = pVertices[(i - 1) * m_iStripeLength + j].Pos - c;
							b = pVertices[i * m_iStripeLength + j + 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
					}
				}
				if(i < m_iStripeCount - 1)
				{
					if(j > 0)
					{
						if(i % 2 != 0)
						{
							a = pVertices[(i + 1) * m_iStripeLength + j].Pos - c;
							b = pVertices[i * m_iStripeLength + j - 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
						else
						{
							a = pVertices[(i + 1) * m_iStripeLength + j - 1].Pos - c;
							b = pVertices[i * m_iStripeLength + j - 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;

							//a = pVertices[(i + 1) * m_iStripeLength + j - 1].Pos - c;
							b = pVertices[(i + 1) * m_iStripeLength + j].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
					}
					if(j < m_iStripeLength - 1)
					{
						if(i % 2 != 0)
						{
							a = pVertices[(i + 1) * m_iStripeLength + j].Pos - c;
							b = pVertices[(i + 1) * m_iStripeLength + j + 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;

							a = pVertices[i * m_iStripeLength + j + 1].Pos - c;
							//b = pVertices[(i + 1) * m_iStripeLength + j + 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
						else
						{
							a = pVertices[(i + 1) * m_iStripeLength + j].Pos - c;
							b = pVertices[i * m_iStripeLength + j + 1].Pos - c;
							n = XMVector3Normalize(XMVector3Cross(XMVectorSet(a.x, a.y, a.z, 1.0f), XMVectorSet(b.x, b.y, b.z, 1.0f)));
							rn.x = XMVectorGetX(n);
							rn.y = XMVectorGetY(n);
							rn.z = XMVectorGetZ(n);
							if(rn.y < 0.0f)
							{
								rn = rn * -1.0f;
							}
							norm = norm + rn;
							cc++;
						}
					}
				}
				if(cc)
				{
					norm = norm / (float)cc;
				}
				pVertices[iCurVertex].Norm = norm;

			}
			//pNormalVertices[iNormalVertexCount].Pos = pVertices[iCurVertex].Pos;
			//iNormalVertexCount++;
			//pNormalVertices[iNormalVertexCount].Pos = pVertices[iCurVertex].Pos + pVertices[iCurVertex].Norm * 5.0f;
			//iNormalVertexCount++;
			
			iCurVertex++;

			if(i < m_iStripeCount - 1 && j < m_iStripeLength - 1)
			{
				if(i % 2 != 0)
				{
					if(m_pHeights[i + 1].at(j) && m_pHeights[i + 1].at(j + 1))
					{
						pIndices[iCurIndex] = i * m_iStripeLength + j;
						iCurIndex++;
						pIndices[iCurIndex] = (i + 1) * m_iStripeLength + j + 1;
						iCurIndex++;
						pIndices[iCurIndex] = (i + 1) * m_iStripeLength + j;
						iCurIndex++;
					}
					if(m_pHeights[i + 1].at(j + 1) && m_pHeights[i].at(j + 1))
					{
						pIndices[iCurIndex] = i * m_iStripeLength + j;
						iCurIndex++;
						pIndices[iCurIndex] = i * m_iStripeLength + j + 1;
						iCurIndex++;
						pIndices[iCurIndex] = (i + 1) * m_iStripeLength + j + 1;
						iCurIndex++;
					}
				}
				else
				{
					if(m_pHeights[i + 1].at(j) && m_pHeights[i].at(j + 1))
					{
						pIndices[iCurIndex] = i * m_iStripeLength + j;
						iCurIndex++;
						pIndices[iCurIndex] = i * m_iStripeLength + j + 1;
						iCurIndex++;
						pIndices[iCurIndex] = (i + 1) * m_iStripeLength + j;
						iCurIndex++;
					}
					if(m_pHeights[i + 1].at(j) && m_pHeights[i + 1].at(j + 1))
					{
						pIndices[iCurIndex] = i * m_iStripeLength + j + 1;
						iCurIndex++;
						pIndices[iCurIndex] = (i + 1) * m_iStripeLength + j + 1;
						iCurIndex++;
						pIndices[iCurIndex] = (i + 1) * m_iStripeLength + j;
						iCurIndex++;
					}
				}
			}
		}
	}
	m_bInConstruct = true;

	DS_WAIT(m_bInRender);

	DS_SAFE_RELEASE(m_pVertexBuffer);
	DS_SAFE_RELEASE(m_pIndexBuffer);
	DS::CreateBuffers(pVertices, iCurVertex, pIndices, iCurIndex, &m_pVertexBuffer, &m_pIndexBuffer);
	//DS::CreateBuffers(pNormalVertices, iNormalVertexCount, NULL, 0, &m_pVertexNormalBuffer, NULL);
	m_iNormalCount = iNormalVertexCount / 2;
	m_iIndexCount = iCurIndex;
	const CChunkManager::DSHCCC64RECT * RR = DS::pDSHCCM->GetRRect();
	m_mPos = XMMatrixTranslation((float)(HC_CHUNK_SIZE_X * (RR->Xmin - (HC_GEN_SIZE - (RR->Xmax - RR->Xmin)) / 2) / 2) * 3.0f * HC_R, HC_H, (float)(HC_CHUNK_SIZE_Z * (RR->Zmin - (HC_GEN_SIZE - (RR->Zmax - RR->Zmin)) / 2)) * Rcos30x2 - Rcos30);
	
	m_bInConstruct = false;

	
	//DS_SAFE_DELETE_A(pNormalVertices);
	m_bBuilded = true;
}

DSHCLOD::DSHCLODstripe & DSHCLOD::operator[](int64_t index)
{
	const CChunkManager::DSHCCC64RECT * RR = DS::pDSHCCM->GetRRect();
	int64_t i = index - (RR->Xmin - ((HC_LOD_SIZE - (RR->Xmax - RR->Xmin)) >> 1)) * HC_CHUNK_SIZE_X;
	DS_ASSERT(i >= 0 && i < m_iStripeCount);
	return(m_pHeights[i]);
}

const DSHCLOD::DSHCLODstripe & DSHCLOD::operator[](int64_t index) const
{
	const CChunkManager::DSHCCC64RECT * RR = DS::pDSHCCM->GetRRect();
	int64_t i = index - (RR->Xmin - ((HC_LOD_SIZE - (RR->Xmax - RR->Xmin)) >> 1)) * HC_CHUNK_SIZE_X;
	DS_ASSERT(i >= 0 && i < m_iStripeCount);
	return(m_pHeights[i]);
}


DSHCLOD::DSHCLODstripe::DSHCLODstripe():m_iLength(0), m_pHeigts(NULL)
{

}

DSHCLOD::DSHCLODstripe::DSHCLODstripe(UINT size):m_iLength(0), m_pHeigts(NULL)
{
	Init(size);
}

DSHCLOD::DSHCLODstripe::~DSHCLODstripe()
{
	DS_SAFE_DELETE_A(m_pHeigts);
}

void DSHCLOD::DSHCLODstripe::Init(UINT size)
{
	m_iLength = size;
	m_pHeigts = new UINT[size];
	memset(m_pHeigts, 0, sizeof(UINT) * size);
}

UINT DSHCLOD::DSHCLODstripe::length() const
{
	return(m_iLength);
}

const UINT & DSHCLOD::DSHCLODstripe::operator[](int64_t index) const
{
	const CChunkManager::DSHCCC64RECT * RR = DS::pDSHCCM->GetRRect();
	int64_t i = index - (RR->Zmin - ((HC_LOD_SIZE - (RR->Zmax - RR->Zmin)) >> 1)) * HC_CHUNK_SIZE_Z;
	DS_ASSERT(i >= 0 && i < m_iLength);
	return(m_pHeigts[i]);
}

UINT & DSHCLOD::DSHCLODstripe::operator[](int64_t index)
{
	const CChunkManager::DSHCCC64RECT * RR = DS::pDSHCCM->GetRRect();
	int64_t i = index - (RR->Zmin - ((HC_LOD_SIZE - (RR->Zmax - RR->Zmin)) >> 1)) * HC_CHUNK_SIZE_Z;
	DS_ASSERT(i >= 0 && i < m_iLength);
	return(m_pHeigts[i]);
}

UINT & DSHCLOD::DSHCLODstripe::at(int64_t index)
{
	return(m_pHeigts[index]);
}

#endif
