#include "ShaderVariant.h"
#include <gcore/sxgcore.h>

CShaderVariant::CShaderVariant(const char *szShaderVS, const char *szShaderPS, GXMacro *pMacros)
{
	ID idShaderVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, szShaderVS, NULL, pMacros);
	ID idShaderPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, szShaderPS, NULL, pMacros);
	m_idShaderKit = SGCore_ShaderCreateKit(idShaderVS, idShaderPS);
}

ID CShaderVariant::getShaderKit() const
{
	return(m_idShaderKit);
}
