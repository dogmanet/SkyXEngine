#include "ResourceModelStatic.h"
#include "MeshOptimizer.h"

CResourceModelStatic::~CResourceModelStatic()
{
	for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
	{
		m_aLods[i].clean();
	}
}

void XMETHODCALLTYPE CResourceModelStatic::setPrimitiveTopology(XPT_TOPOLOGY topology)
{
	m_topology = topology;
}
XPT_TOPOLOGY XMETHODCALLTYPE CResourceModelStatic::getPrimitiveTopology() const
{
	return(m_topology);
}


XMODELTYPE XMETHODCALLTYPE CResourceModelStatic::getType() const
{
	return(XMT_STATIC);
}

UINT XMETHODCALLTYPE CResourceModelStatic::getSubsetCount(UINT uLod) const
{
	assert(uLod < m_aLods.size());

	return(m_aLods[uLod].uSubsetCount);
}
UINT XMETHODCALLTYPE CResourceModelStatic::addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount)
{
	_subset_meta subset;

	subset.uSubsetCount = uSubsetCount;
	subset.pSubsets = new XResourceModelStaticSubset[uSubsetCount];

	UINT uIndexCount = 0;
	UINT uVertexCount = 0;
	for(UINT i = 0; i < uSubsetCount; ++i)
	{
		uVertexCount += puVertexCount[i];
		uIndexCount += puIndexCount[i];
	}
	subset.pVertices = new XResourceModelStaticVertex[uVertexCount];
	subset.pIndices = new UINT[uIndexCount];
	memset(subset.pIndices, 0, sizeof(UINT)* uIndexCount);

	uIndexCount = 0;
	uVertexCount = 0;
	for(UINT i = 0; i < uSubsetCount; ++i)
	{
		subset.pSubsets[i].pVertices = subset.pVertices + uVertexCount;
		subset.pSubsets[i].pIndices = subset.pIndices + uIndexCount;
		subset.pSubsets[i].iIndexCount = puIndexCount[i];
		subset.pSubsets[i].iVertexCount = puVertexCount[i];
		uVertexCount += puVertexCount[i];
		uIndexCount += puIndexCount[i];
	}

	m_aLods.push_back(subset);
	return(m_aLods.size() - 1);
}
UINT XMETHODCALLTYPE CResourceModelStatic::getLodCount() const
{
	return(m_aLods.size());
}

const XResourceModelStaticSubset* XMETHODCALLTYPE CResourceModelStatic::getSubset(UINT uLod, UINT uSubset) const
{
	assert(uLod < m_aLods.size());
	assert(uSubset < m_aLods[uLod].uSubsetCount);

	return(&m_aLods[uLod].pSubsets[uSubset]);
}
XResourceModelStaticSubset* XMETHODCALLTYPE CResourceModelStatic::getSubset(UINT uLod, UINT uSubset)
{
	assert(uLod < m_aLods.size());
	assert(uSubset < m_aLods[uLod].uSubsetCount);

	return(&m_aLods[uLod].pSubsets[uSubset]);
}

bool XMETHODCALLTYPE CResourceModelStatic::validate() const
{
	//@TODO: Implement me!
	return(true);
}

void XMETHODCALLTYPE CResourceModelStatic::optimize()
{
	for(UINT uLod = 0, uLodCount = m_aLods.size(); uLod < uLodCount; ++uLod)
	{
		auto *pLod = &m_aLods[uLod];
		for(UINT uSubset = 0, uSubsetCount = pLod->uSubsetCount; uSubset < uSubsetCount; ++uSubset)
		{
			auto *pSubset = &pLod->pSubsets[uSubset];

			CMeshOptimizer<XResourceModelStaticVertex, UINT>::Optimize(pSubset->pVertices, pSubset->pIndices, pSubset->iVertexCount, pSubset->iIndexCount);
		}
	}
}
