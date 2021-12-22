#include <xcommon/resource/IXResourceManager.h>
#include "Combiner.h"
#include "Editable.h"
#include "EditorObject.h"


//##########################################################################

CCombiner::CCombiner(CEditable *pEditable):
	m_pEditable(pEditable)
{
}
CCombiner::~CCombiner()
{
	clean();
}

void CCombiner::build()
{
	clean();

	// TODO Build real csg mesh
	// TODO Split meshes into parts

	IXResourceManager *pRM = m_pEditable->getCore()->getResourceManager();
	IXResourceModelStatic *pResource = pRM->newResourceModelStatic();

	CMeshBuilder meshBuilder;
	auto &aObjects = m_pEditable->getObjects();
	bool hasObjects = false;
	fora(i, aObjects)
	{
		if(!aObjects[i]->getModel())
		{
			hasObjects = true;
			aObjects[i]->buildMesh(&meshBuilder);
			aObjects[i]->buildPhysbox(pResource);
		}
	}

	if(hasObjects)
	{
		bool isFirst = true;

		for(UINT i = 0, l = meshBuilder.getSubsetCount(); i < l; ++i)
		{
			if(meshBuilder.getSubset(i).aIndices.size())
			{
				if(!isFirst)
				{
					pResource = pRM->newResourceModelStatic();
				}
				meshBuilder.buildResource(pResource, i);
				m_aResources.push_back(pResource);
				isFirst = false;
			}
		}
		
		if(isFirst)
		{
			mem_release(pResource);
		}
	}
	else
	{
		mem_release(pResource);
	}
}

UINT CCombiner::getResourceCount()
{
	return(m_aResources.size());
}
void CCombiner::getResourceAt(UINT idx, IXResourceModel **ppOut)
{
	assert(idx <= m_aResources.size());
	if(idx <= m_aResources.size())
	{
		*ppOut = m_aResources[idx];
		add_ref(m_aResources[idx]);
	}
	else
	{
		*ppOut = NULL;
	}
}

void CCombiner::clean()
{
	fora(i, m_aResources)
	{
		mem_release(m_aResources[i]);
	}
	m_aResources.clearFast();
}
