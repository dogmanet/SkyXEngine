#include "ResourceModelStatic.h"

CResourceModelStatic::~CResourceModelStatic()
{
	for(UINT i = 0, l = m_aLods.size(); i < l; ++i)
	{
		m_aLods[i].clean();
	}
}

XMODELTYPE CResourceModelStatic::getType() const
{
	return(XMT_STATIC);
}

UINT CResourceModelStatic::getSubsetCount(UINT uLod) const
{
	assert(uLod < m_aLods.size());

	return(m_aLods[uLod].uSubsetCount);
}
UINT CResourceModelStatic::addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount)
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
		subset.pSubsets[i].iIndexCount = puVertexCount[i];
		subset.pSubsets[i].iVertexCount = puIndexCount[i];
		uVertexCount += puVertexCount[i];
		uIndexCount += puIndexCount[i];
	}

	m_aLods.push_back(subset);
	return(m_aLods.size() - 1);
}

const XResourceModelStaticSubset *CResourceModelStatic::getSubset(UINT uLod, UINT uSubset) const
{
	assert(uLod < m_aLods.size());
	assert(uSubset < m_aLods[uLod].uSubsetCount);

	return(&m_aLods[uLod].pSubsets[uSubset]);
}
XResourceModelStaticSubset *CResourceModelStatic::getSubset(UINT uLod, UINT uSubset)
{
	assert(uLod < m_aLods.size());
	assert(uSubset < m_aLods[uLod].uSubsetCount);

	return(&m_aLods[uLod].pSubsets[uSubset]);
}

bool CResourceModelStatic::validate() const
{
	//@TODO: Implement me!
	return(true);
}
