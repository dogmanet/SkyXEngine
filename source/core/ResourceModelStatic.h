#ifndef __RESOURCEMODELSTATIC_H
#define __RESOURCEMODELSTATIC_H

#include "ResourceModel.h"

class CResourceModelStatic final: public CResourceModel, public virtual IXResourceModelStatic
{
public:
	XMODELTYPE getType() const override;

	UINT getSubsetCount(UINT uLod) const override;
	UINT addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount) override;

	const XResourceModelStaticSubset *getSubset(UINT uLod, UINT uSubset) const override;
	XResourceModelStaticSubset *getSubset(UINT uLod, UINT uSubset) override;

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
