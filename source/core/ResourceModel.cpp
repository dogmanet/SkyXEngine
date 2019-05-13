#include "ResourceModel.h"
#include "ResourceModelStatic.h"
#include "ResourceModelAnimated.h"

CResourceModel::~CResourceModel()
{
	for(UINT i = 0, l = m_aPhysBoxes.size(); i < l; ++i)
	{
		mem_release(m_aPhysBoxes[i]);
	}

	for(UINT i = 0, l = m_asGibs.size(); i < l; ++i)
	{
		mem_release(m_asGibs[i]);
	}
}

void CResourceModel::setPrimitiveTopology(XPT_TOPOLOGY topology)
{
	m_topology = topology;
}
XPT_TOPOLOGY CResourceModel::getPrimitiveTopology() const
{
	return(m_topology);
}


UINT CResourceModel::getPhysboxCount() const
{
	return(m_aPhysBoxes.size());
}
const IModelPhysbox *CResourceModel::getPhysbox(UINT uPart) const
{
	assert(uPart < m_aPhysBoxes.size());

	return(m_aPhysBoxes[uPart]);
}
void CResourceModel::addPhysbox(IModelPhysbox *pPhysbox)
{
	assert(pPhysbox);

	pPhysbox->AddRef();

	m_aPhysBoxes.push_back(pPhysbox);
}


UINT CResourceModel::getMaterialCount() const
{
	return(m_uMaterialCount);
}
UINT CResourceModel::getSkinCount() const
{
	return(m_uSkinCount);
}
const char *CResourceModel::getMaterial(UINT uMaterial, UINT uSkin) const
{
	assert(uMaterial < m_uMaterialCount);
	assert(uSkin < m_uSkinCount);

	return(m_aasMaterials[uSkin][uMaterial].c_str());
}
void CResourceModel::setMaterialCount(UINT uMaterialCount, UINT uSkinCount)
{
	m_uMaterialCount = uMaterialCount;
	m_uSkinCount = uSkinCount;

	m_aasMaterials.resize(m_uSkinCount);

	for(UINT i = 0, l = m_uSkinCount; i < l; ++i)
	{
		m_aasMaterials[i].resize(uMaterialCount);
	}
}
void CResourceModel::setMaterial(UINT uMaterial, UINT uSkin, const char *szName)
{
	assert(uMaterial < m_uMaterialCount);
	assert(uSkin < m_uSkinCount);

	m_aasMaterials[uSkin][uMaterial] = szName;
}


float CResourceModel::getMass() const
{
	return(m_fMass);
}
void CResourceModel::setMass(float fMass)
{
	assert(fMass >= 0.0f);

	m_fMass = fMass;
}


UINT CResourceModel::getGibsCount() const
{
	return(m_asGibNames.size());
}
const char *CResourceModel::getGibName(UINT uIndex) const
{
	assert(uIndex < m_asGibNames.size());

	return(m_asGibNames[uIndex].c_str());
}
UINT CResourceModel::addGibName(const char *szFileName)
{
	m_asGibNames.push_back(szFileName);
	m_asGibs.push_back(NULL);

	return(m_asGibNames.size() - 1);
}

const IXResourceModel *CResourceModel::getGib(UINT uIndex) const
{
	assert(uIndex < m_asGibs.size());

	return(m_asGibs[uIndex]);
}
void CResourceModel::setGib(UINT uIndex, IXResourceModel *pResource)
{
	assert(uIndex < m_asGibs.size());
	assert(pResource);

	pResource->AddRef();

	mem_release(m_asGibs[uIndex]);

	m_asGibs[uIndex] = pResource;
}

const IXResourceModelStatic *CResourceModel::asStatic() const
{
	if(getType() == XMT_STATIC)
	{
		return((const CResourceModelStatic*)this);
	}
	return(NULL);
}
const IXResourceModelAnimated * CResourceModel::asAnimated() const
{
	if(getType() == XMT_ANIMATED)
	{
		return((const CResourceModelAnimated*)this);
	}
	return(NULL);
}
