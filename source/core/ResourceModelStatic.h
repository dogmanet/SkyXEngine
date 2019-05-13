#ifndef __RESOURCEMODELSTATIC_H
#define __RESOURCEMODELSTATIC_H

#include "ResourceModel.h"

class CResourceModelStatic final: public CResourceModel, public virtual IXResourceModelStatic
{
public:
	XMODELTYPE XMETHODCALLTYPE getType() const override;

	UINT XMETHODCALLTYPE getSubsetCount(UINT uLod) const override;
	UINT XMETHODCALLTYPE addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount) override;

	const XResourceModelStaticSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) const override;
	XResourceModelStaticSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) override;

	bool XMETHODCALLTYPE validate() const;

protected:

	struct _subset_meta
	{
		UINT uSubsetCount;
		XResourceModelStaticSubset *pSubsets;
		XResourceModelStaticVertex *pVertices;
		UINT *pIndices;

		~_subset_meta()
		{
			mem_delete_a(pSubsets);
			mem_delete_a(pVertices);
			mem_delete_a(pIndices);
		}
	};
	Array<_subset_meta> m_aLods;
};

#endif
