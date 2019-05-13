#ifndef __RESOURCEMODEL_H
#define __RESOURCEMODEL_H

#include <xcommon/resource/IXResourceModel.h>
#include <common/string.h>

class CResourceModel: public IXResourceModel
{
public:
	~CResourceModel();

	void setPrimitiveTopology(XPT_TOPOLOGY topology) override;
	XPT_TOPOLOGY getPrimitiveTopology() const override;


	UINT getPhysboxCount() const override;
	const IModelPhysbox *getPhysbox(UINT uPart) const override;
	void addPhysbox(IModelPhysbox *pPhysbox) override;


	UINT getMaterialCount() const override;
	UINT getSkinCount() const override;
	const char *getMaterial(UINT uMaterial, UINT uSkin = 0) const override;
	void setMaterialCount(UINT uMaterialCount, UINT uSkinCount) override;
	void setMaterial(UINT uMaterial, UINT uSkin, const char *szName) override;


	float getMass() const override;
	void setMass(float fMass) override;


	UINT getGibsCount() const override;
	const char *getGibName(UINT uIndex) const override;
	UINT addGibName(const char *szFileName) override;

	const IXResourceModel * getGib(UINT uIndex) const override;
	void setGib(UINT uIndex, IXResourceModel *pResource) override;

	const IXResourceModelStatic * asStatic() const override;
	const IXResourceModelAnimated * asAnimated() const override;

protected:
	XPT_TOPOLOGY m_topology = XPT_TRIANGLELIST;
	Array<IModelPhysbox*> m_aPhysBoxes;
	UINT m_uMaterialCount = 0;
	UINT m_uSkinCount = 0;
	Array<Array<String>> m_aasMaterials;
	float m_fMass = 0.0f;
	Array<String> m_asGibNames;
	Array<IXResourceModel*> m_asGibs;
};

#endif
