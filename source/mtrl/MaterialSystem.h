#ifndef __MATERIALSYSTEM_H
#define __MATERIALSYSTEM_H

#include "IXMaterialSystem.h"

class CTexture: public IXTexture
{
public:
	CTexture(ID id);
	void getAPITexture(IGXBaseTexture **ppTexture);
	ID getId();
protected:
	ID m_id = -1;
};

class CMaterial: public IXMaterial
{
public:
	CMaterial(ID id);
	void getMainTexture(IXTexture **ppTexture);
	ID getId();
	bool isTransparent();
protected:
	ID m_id = -1;
};

class CMaterialSystem: public IXMaterialSystem
{
public:
	void loadMaterial(const char *szName, IXMaterial **ppMaterial, XSHADER_DEFAULT_DESC *pDefaultShaders, UINT uVariantCount = 0, XSHADER_VARIANT_DESC *pVariantsDesc = NULL);
	bool getMaterial(const char *szName, IXMaterial **ppMaterial);

	bool getTexture(const char *szName, IXTexture **ppTexture);
	void addTexture(const char *szName, IGXTexture2D *pTexture);

	void bindMaterial(IXMaterial *pMaterial, const SMMATRIX *pmWorld = NULL, IXShaderVariant *pShaderVariant = NULL);
	void bindTexture(IXTexture *pTexture, UINT slot = 0);
};

#endif
