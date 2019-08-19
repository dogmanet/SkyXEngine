#ifndef __MATERIALSYSTEM_H
#define __MATERIALSYSTEM_H

#include "IXMaterialSystem.h"

class CTexture: public IXTexture
{
public:
	CTexture(ID id);
	void getAPITexture(IGXBaseTexture **ppTexture) override;
	ID getId();
protected:
	ID m_id = -1;
};

class CMaterial: public IXMaterial
{
public:
	CMaterial(ID id);
	void getMainTexture(IXTexture **ppTexture) override;
	ID getId();
	bool isTransparent() override;
	bool isRefractive() override;
	bool isBlurred() override;

	ID getInternalID()
	{
		return(m_id);
	}
protected:
	ID m_id = -1;
};

class CMaterialSystem: public IXMaterialSystem
{
public:
	CMaterialSystem();
	~CMaterialSystem();

	void loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount = 0, XSHADER_VARIANT_DESC *pVariantsDesc = NULL);
	bool getMaterial(const char *szName, IXMaterial **ppMaterial);

	bool getTexture(const char *szName, IXTexture **ppTexture);
	void addTexture(const char *szName, IGXTexture2D *pTexture);

	void bindMaterial(IXMaterial *pMaterial, IXShaderVariant *pShaderVariant = NULL);
	void bindTexture(IXTexture *pTexture, UINT slot = 0);
	void setWorld(const SMMATRIX &mWorld);

	void overrideGeometryShader(ID id);
	void overridePixelShader(ID id);

protected:
	struct CObjectData
	{
		SMMATRIX m_mW;
	//	SMMATRIX m_mWV;
	//	SMMATRIX m_mWVP;
	} m_objectData;
	IGXConstantBuffer *m_pObjectConstantBuffer = NULL;
};

#endif
