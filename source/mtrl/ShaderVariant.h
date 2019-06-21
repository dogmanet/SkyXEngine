#ifndef __SHADERVARIANT_H
#define __SHADERVARIANT_H

#include "IXMaterialSystem.h"

class CShaderVariant: public IXShaderVariant
{
public:
	CShaderVariant(const char *szShaderVS, const char *szShaderPS, GXMACRO *pMacros);
	ID getShaderKit() const;

protected:
	//! набор шейдеров
	ID m_idShaderKit = -1;
};

#endif
