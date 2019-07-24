#ifndef __RESOURCEMODELSTATIC_H
#define __RESOURCEMODELSTATIC_H

#include "ResourceModel.h"

#pragma warning(push)
#pragma warning(disable: 4250)

class CResourceModelStatic final: public CResourceModel, public virtual IXResourceModelStatic
{
public:
	CResourceModelStatic(CResourceManager *pMgr):
		CResourceModel(pMgr)
	{
	}
	~CResourceModelStatic();

	void XMETHODCALLTYPE setPrimitiveTopology(XPT_TOPOLOGY topology) override;
	XPT_TOPOLOGY XMETHODCALLTYPE getPrimitiveTopology() const override;

	XMODELTYPE XMETHODCALLTYPE getType() const override;

	UINT XMETHODCALLTYPE getSubsetCount(UINT uLod) const override;
	UINT XMETHODCALLTYPE addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount) override;
	UINT XMETHODCALLTYPE getLodCount() const override;

	const XResourceModelStaticSubset* XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) const override;
	XResourceModelStaticSubset* XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) override;

	bool XMETHODCALLTYPE validate() const;

	void XMETHODCALLTYPE optimize() override;

protected:

	XPT_TOPOLOGY m_topology = XPT_TRIANGLELIST;

	struct _subset_meta
	{
		UINT uSubsetCount;
		XResourceModelStaticSubset *pSubsets;
		XResourceModelStaticVertex *pVertices;
		UINT *pIndices;

		void clean()
		{
			mem_delete_a(pSubsets);
			mem_delete_a(pVertices);
			mem_delete_a(pIndices);
		}
	};
	Array<_subset_meta> m_aLods;
};

#pragma warning(pop)

#endif
