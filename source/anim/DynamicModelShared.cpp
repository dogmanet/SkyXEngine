#include "DynamicModelShared.h"
#include "DynamicModelProvider.h"
#include <mtrl/IXMaterialSystem.h>

CDynamicModelShared::CDynamicModelShared(CDynamicModelProvider *pProvider):
	m_pProvider(pProvider),
	m_pDevice(pProvider->getDevice()),
	m_pMaterialSystem(pProvider->getMaterialSystem())
{
}
CDynamicModelShared::~CDynamicModelShared()
{
	m_pProvider->onSharedModelRelease(this);
	
	for(UINT i = 0; i < m_uSkinCount; ++i)
	{
		for(UINT j = 0; j < m_uMaterialCount; ++j)
		{
			mem_release(m_pppMaterials[i][j]);
		}
	}

	mem_delete_a(m_ppMaterialsBlob);
	m_pppMaterials = NULL;

	mem_delete_a(m_isTransparent);

	if(m_pDevice)
	{
		for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
		{
			mem_release(m_ppRenderBuffer[i]);
			mem_release(m_ppIndexBuffer[i]);
		}
		mem_delete_a(m_ppRenderBuffer);
		mem_delete_a(m_ppIndexBuffer);
	}
	
	mem_release(m_pResource);

	if(m_ppTempIndices)
	{
		for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
		{
			mem_delete_a(m_ppTempIndices[i]);
			mem_delete_a(m_ppTempVertices[i]);
		}
		mem_delete_a(m_ppTempIndices);
		mem_delete_a(m_ppTempVertices);
		mem_delete_a(m_puTempTotalIndices);
		mem_delete_a(m_puTempTotalVertices);
	}
}
void CDynamicModelShared::AddRef()
{
	++m_uRefCount;
}
void CDynamicModelShared::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		delete this;
	}
}

bool CDynamicModelShared::init(IXResourceModelStatic *pResource)
{
	m_pResource = pResource;
	pResource->AddRef();

	m_topology = pResource->getPrimitiveTopology();

	{
		m_uMaterialCount = pResource->getMaterialCount();
		m_uSkinCount = pResource->getSkinCount();

		m_isTransparent = new bool[m_uSkinCount];

		m_ppMaterialsBlob = new void*[m_uMaterialCount * m_uSkinCount + m_uSkinCount];
		m_pppMaterials = (IXMaterial***)m_ppMaterialsBlob;
		XSHADER_DEFAULT_DESC shaderDesc;
		shaderDesc.szFileVS = "mtrlgeom_base.vs";
		shaderDesc.szFilePS = "mtrlgeom_base.ps";

		const char *szMaterial;
		for(UINT i = 0; i < m_uSkinCount; ++i)
		{
			m_pppMaterials[i] = (IXMaterial**)(m_ppMaterialsBlob + m_uSkinCount + m_uMaterialCount * i);
			bool isTransparent = false;

			for(UINT j = 0; j < m_uMaterialCount; ++j)
			{
				szMaterial = m_pResource->getMaterial(j, i);
				if(szMaterial && szMaterial[0])
				{
					m_pMaterialSystem->loadMaterial(szMaterial, &m_pppMaterials[i][j], &shaderDesc);
				}
				else if(i == 0)
				{
					m_pppMaterials[i][j] = NULL;
				}
				else
				{
					m_pppMaterials[i][j] = m_pppMaterials[0][j];
					m_pppMaterials[i][j]->AddRef();
				}

				if(m_pppMaterials[i][j] && m_pppMaterials[i][j]->isTransparent())
				{
					isTransparent = true;
				}
			}

			m_isTransparent[i] = isTransparent;
		}
	}

	{
		UINT uLodCount = m_pResource->getLodCount();
		m_aLods.resizeFast(uLodCount);
		if(m_pDevice)
		{
			m_ppRenderBuffer = new IGXRenderBuffer*[uLodCount];
			m_ppIndexBuffer = new IGXIndexBuffer*[uLodCount];

			if(!m_pProvider->getCore()->isOnMainThread())
			{
				m_ppTempIndices = new UINT*[uLodCount];
				m_ppTempVertices = new XResourceModelStaticVertex*[uLodCount];
				m_puTempTotalIndices = new UINT[uLodCount];
				m_puTempTotalVertices = new UINT[uLodCount];
			}

			bool isMinMaxSet = false;
			float3 vLocalMin, vLocalMax;
			for(UINT i = 0; i < uLodCount; ++i)
			{
				UINT uTotalIndices = 0;
				UINT uTotalVertices = 0;

				for(UINT j = 0, jl = m_pResource->getSubsetCount(i); j < jl; ++j)
				{
					auto pSubset = m_pResource->getSubset(i, j);
					uTotalIndices += pSubset->iIndexCount;
					uTotalVertices += pSubset->iVertexCount;
				}

				XResourceModelStaticVertex *pVertices = new XResourceModelStaticVertex[uTotalVertices];
				UINT *pIndices = new UINT[uTotalIndices];

				subset_t subset;
				for(UINT uMaterial = 0; uMaterial < m_uMaterialCount; ++uMaterial)
				{
					for(UINT j = 0, jl = m_pResource->getSubsetCount(i); j < jl; ++j)
					{
						auto pSubset = m_pResource->getSubset(i, j);
						if(pSubset->iMaterialID == uMaterial)
						{
							subset.uStartIndex += subset.uIndexCount;
							subset.uStartVertex += subset.uVertexCount;

							subset.uIndexCount = pSubset->iIndexCount;
							subset.uVertexCount = pSubset->iVertexCount;

							memcpy(pVertices + subset.uStartVertex, pSubset->pVertices, sizeof(XResourceModelStaticVertex) * pSubset->iVertexCount);
							memcpy(pIndices + subset.uStartIndex, pSubset->pIndices, sizeof(UINT) * pSubset->iIndexCount);

							m_aLods[i][uMaterial] = subset;

							if(i == 0)
							{
								for(UINT k = 0; k < pSubset->iVertexCount; ++k)
								{
									if(isMinMaxSet)
									{
										vLocalMin = SMVectorMin(vLocalMin, pSubset->pVertices[k].vPos);
										vLocalMax = SMVectorMax(vLocalMax, pSubset->pVertices[k].vPos);
									}
									else
									{
										vLocalMin = vLocalMax = pSubset->pVertices[k].vPos;
										isMinMaxSet = true;
									}
								}
							}

							break;
						}
					}
				}

				if(m_pProvider->getCore()->isOnMainThread())
				{
					m_ppIndexBuffer[i] = m_pDevice->createIndexBuffer(sizeof(UINT) * uTotalIndices, GXBUFFER_USAGE_STATIC, GXIT_UINT32, pIndices);
					IGXVertexBuffer *pVertexBuffer = m_pDevice->createVertexBuffer(sizeof(XResourceModelStaticVertex) * uTotalVertices, GXBUFFER_USAGE_STATIC, pVertices);
					m_ppRenderBuffer[i] = m_pDevice->createRenderBuffer(1, &pVertexBuffer, m_pProvider->getVertexDeclaration());
					mem_release(pVertexBuffer);

					mem_delete_a(pIndices);
					mem_delete_a(pVertices);
				}
				else
				{
					m_ppIndexBuffer[i] = NULL;
					m_ppRenderBuffer[i] = NULL;

					m_ppTempIndices[i] = pIndices;
					m_ppTempVertices[i] = pVertices;
					m_puTempTotalIndices[i] = uTotalIndices;
					m_puTempTotalVertices[i] = uTotalVertices;

					m_pProvider->scheduleSharedGPUinit(this);
				}

			}

			m_vLocalMin = vLocalMin;
			m_vLocalMax = vLocalMax;

			//printf("Min: %.2f, %.2f, %.2f; Max: %.2f, %.2f, %.2f\n", m_vLocalMin.x, m_vLocalMin.y, m_vLocalMin.z, m_vLocalMax.x, m_vLocalMax.y, m_vLocalMax.z);
		}

	}

	for(UINT m = 0, ml = pResource->getPhysboxCount(); m < ml; ++m)
	{
		m_apPhysboxes.push_back(pResource->getPhysbox(m));
	}

	buildPSPs();
	
	return(true);
}

void CDynamicModelShared::onMaterialTransparencyChanged(const IXMaterial *pMaterial)
{
	bool isChanged = false;
	for(UINT i = 0; i < m_uSkinCount; ++i)
	{
		bool isTransparent = false;

		for(UINT j = 0; j < m_uMaterialCount; ++j)
		{
			if(m_pppMaterials[i][j])
			{
				if(m_pppMaterials[i][j]->isTransparent())
				{
					isTransparent = true;
				}

				if(m_pppMaterials[i][j] == pMaterial)
				{
					isChanged = true;
				}
			}
		}

		m_isTransparent[i] = isTransparent;
	}

	if(isChanged)
	{
		buildPSPs();
	}
}

void CDynamicModelShared::initGPUresources()
{
	if(!m_ppTempIndices)
	{
		return;
	}
	assert(m_pProvider->getCore()->isOnMainThread());

	for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
	{
		m_ppIndexBuffer[i] = m_pDevice->createIndexBuffer(sizeof(UINT) * m_puTempTotalIndices[i], GXBUFFER_USAGE_STATIC, GXIT_UINT32, m_ppTempIndices[i]);
		IGXVertexBuffer *pVertexBuffer = m_pDevice->createVertexBuffer(sizeof(XResourceModelStaticVertex) * m_puTempTotalVertices[i], GXBUFFER_USAGE_STATIC, m_ppTempVertices[i]);
		m_ppRenderBuffer[i] = m_pDevice->createRenderBuffer(1, &pVertexBuffer, m_pProvider->getVertexDeclaration());
		mem_release(pVertexBuffer);

		mem_delete_a(m_ppTempIndices[i]);
		mem_delete_a(m_ppTempVertices[i]);
	}
	mem_delete_a(m_ppTempIndices);
	mem_delete_a(m_ppTempVertices);
	mem_delete_a(m_puTempTotalIndices);
	mem_delete_a(m_puTempTotalVertices);
}

IXResourceModelStatic *CDynamicModelShared::getResource()
{
	return(m_pResource);
}


UINT CDynamicModelShared::getPhysboxCount() const
{
	return(m_apPhysboxes.size());
}
const IModelPhysbox *CDynamicModelShared::getPhysBox(UINT id) const
{
	assert(id < m_apPhysboxes.size());

	return(m_apPhysboxes[id]);
}

float3 CDynamicModelShared::getLocalBoundMin() const
{
	return(m_vLocalMin);
}

float3 CDynamicModelShared::getLocalBoundMax() const
{
	return(m_vLocalMax);
}

void CDynamicModelShared::render(UINT uSkin, UINT uLod, const float4_t &vColor, bool isTransparent)
{
	if(!m_pDevice)
	{
		return;
	}

	if(uSkin >= m_uSkinCount)
	{
		uSkin = 0;
	}

	if(isTransparent && !hasTransparentSubsets(uSkin, uLod))
	{
		return;
	}

	if(uLod >= m_aLods.size())
	{
		return;
	}

	if(!m_ppIndexBuffer[uLod] || !m_ppRenderBuffer[uLod])
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setIndexBuffer(m_ppIndexBuffer[uLod]);
	pCtx->setRenderBuffer(m_ppRenderBuffer[uLod]);

	switch(m_topology)
	{
	case XPT_TRIANGLELIST:
		pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);
		break;
	case XPT_TRIANGLESTRIP:
		pCtx->setPrimitiveTopology(GXPT_TRIANGLESTRIP);
		break;
	}


	UINT uPrimCount = 0;
	subset_t *pSubset;
	for(UINT i = 0; i < m_uMaterialCount; ++i)
	{
		pSubset = &m_aLods[uLod][i];
		
		if(pSubset->uIndexCount != 0 && ((!m_pppMaterials[uSkin][i] && !isTransparent) || m_pppMaterials[uSkin][i]->isTransparent() == isTransparent))
		{
			m_pMaterialSystem->bindMaterial(m_pppMaterials[uSkin][i]);

			switch(m_topology)
			{
			case XPT_TRIANGLELIST:
				uPrimCount = pSubset->uIndexCount / 3;
				break;
			case XPT_TRIANGLESTRIP:
				uPrimCount = pSubset->uIndexCount - 2;
				break;
			}

			pCtx->drawIndexed(pSubset->uVertexCount, uPrimCount, pSubset->uStartIndex, pSubset->uStartVertex);
		}
	}
}

bool CDynamicModelShared::hasTransparentSubsets(UINT uSkin, UINT uLod)
{
	assert(uSkin < m_uSkinCount);
	if(uSkin >= m_uSkinCount)
	{
		return(false);
	}
	//! @todo add uLod support

	return(m_isTransparent[uSkin]);
}

IXMaterial* CDynamicModelShared::getTransparentMaterial(UINT uSkin, UINT uLod)
{
	assert(uSkin < m_uSkinCount);
	if(uSkin >= m_uSkinCount)
	{
		return(NULL);
	}

	assert(uLod < m_aLods.size());
	if(uLod >= m_aLods.size())
	{
		return(NULL);
	}

	for(UINT j = 0, jl = m_pResource->getSubsetCount(uLod); j < jl; ++j)
	{
		auto pSubset = m_pResource->getSubset(uLod, j);
		IXMaterial *pMaterial = m_pppMaterials[uSkin][pSubset->iMaterialID];

		if(pMaterial && pMaterial->isTransparent())
		{
			pMaterial->AddRef();
			return(pMaterial);
		}
	}
	return(NULL);
}

float3 CDynamicModelShared::getTransparentBoundMin(UINT uSkin, UINT uLod) const
{
	//! @todo Implement me!
	return(m_vLocalMin);
}
float3 CDynamicModelShared::getTransparentBoundMax(UINT uSkin, UINT uLod) const
{
	//! @todo Implement me!
	return(m_vLocalMax);
}

void CDynamicModelShared::buildPSPs()
{
	assert(m_topology == XPT_TRIANGLELIST);
	if(m_topology != XPT_TRIANGLELIST)
	{
		return;
	}

	const float c_fEpsilon = 0.01f;

	m_aPSPs.reserve(m_aLods.size());
	for(UINT i = 0, uLodCount = m_aLods.size(); i < uLodCount; ++i)
	{
		m_aPSPs[i].reserve(m_uSkinCount);
		for(UINT j = 0, jl = m_pResource->getSubsetCount(i); j < jl; ++j)
		{
			auto pSubset = m_pResource->getSubset(i, j);

			for(UINT k = 0; k < m_uSkinCount; ++k)
			{
				if(j == 0)
				{
					m_aPSPs[i][k].clearFast();
				}
				IXMaterial *pMaterial = m_pppMaterials[k][pSubset->iMaterialID];
				if(pMaterial && pMaterial->isTransparent())
				{
					// Если сабсет достаточно плоский, он может породить PSP
					UINT uTotalFaces = pSubset->iIndexCount / 3;
					float3 vFirstNormal, vNormal;
					bool isGood = true;
					float3 a, b, c;
					for(UINT uFace = 0; uFace < uTotalFaces; ++uFace)
					{
						a = pSubset->pVertices[pSubset->pIndices[uFace * 3]].vPos;
						b = pSubset->pVertices[pSubset->pIndices[uFace * 3 + 1]].vPos;
						c = pSubset->pVertices[pSubset->pIndices[uFace * 3 + 2]].vPos;
						vNormal = SMVector3Normalize(SMVector3Cross(b - a, c - a));

						if(uFace == 0)
						{
							vFirstNormal = vNormal;
						}
						else
						{
							if(1.0f - fabsf(SMVector2Dot(vNormal, vFirstNormal)) > c_fEpsilon)
							{
								isGood = false;
								break;
							}
						}
					}

					//! @fixme Здесь не учитывается расстояние между полигонами

					if(isGood)
					{
						m_aPSPs[i][k].push_back(SMPLANE(vNormal, a));
					}
				}
			}
		}
	}
}

const Array<float4_t>& CDynamicModelShared::getPSPs(UINT uSkin, UINT uLod) const
{
	assert(uSkin < m_uSkinCount);
	assert(uLod < m_aLods.size());

	return(m_aPSPs[uLod][uSkin]);
}
