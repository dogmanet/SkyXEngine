#ifndef __XMATERIALPROPERTY_H
#define __XMATERIALPROPERTY_H

#include <gdefines.h>

enum XMATERIAL_PROPERTY_TYPE
{
	XMPT_UNKNOWN = 0,
	XMPT_PARAM_TEXTURE,
	XMPT_PARAM_TEXTURE_OPT,
	XMPT_PARAM_FLAG,
	XMPT_PARAM_GROUP,
	XMPT_PARAM_GROUP_END,
	XMPT_PARAM_RANGE,
	XMPT_PARAM_COLOR,
	XMPT_STATIC_PARAM,
	XMPT_GLOBAL_FLAG,
	XMPT_GLOBAL_PARAM
};

struct XMaterialProperty
{
	XMATERIAL_PROPERTY_TYPE type;
	const char *szTitle;
	const char *szKey;
	const char *szDefine;
	const char *szCondition;

	GXDECLTYPE varType;
	float4_t fValue;
};

#define XMATERIAL_PARAM_TEXTURE(szTitle, szKey)                       {XMPT_PARAM_TEXTURE,     szTitle, szKey,   NULL,     NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_PARAM_TEXTURE_OPT(szTitle, szKey, szDefine)         {XMPT_PARAM_TEXTURE_OPT, szTitle, szKey,   szDefine, NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_PARAM_FLAG(szTitle, szParam, szDefine)              {XMPT_PARAM_FLAG,        szTitle, NULL,    szDefine, NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_PARAM_GROUP(szTitle, szCondition)                   {XMPT_PARAM_GROUP,       szTitle, NULL,    NULL,     szCondition, GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_PARAM_GROUP_END()                                   {XMPT_PARAM_GROUP_END,   NULL,    NULL,    NULL,     NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_PARAM_RANGE(szTitle, szParam, fFrom, fTo, fDefault) {XMPT_PARAM_RANGE,       szTitle, szParam, NULL,     NULL,        GXDECLTYPE_FLOAT1, float4_t(fDefault, fFrom, fTo,  0.0f)}
#define XMATERIAL_PARAM_COLOR(szTitle, szParam, v4Default)            {XMPT_PARAM_COLOR,       szTitle, szParam, NULL,     NULL,        GXDECLTYPE_FLOAT4, float4_t(v4Default                  )}
#define XMATERIAL_STATIC_PARAM(szParam, fValue)                       {XMPT_STATIC_PARAM,      NULL,    szParam, NULL,     NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_GLOBAL_FLAG(szFlag, szDefine)                       {XMPT_GLOBAL_FLAG,       NULL,    szFlag,  szDefine, NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_GLOBAL_PARAM(szParam, type)                         {XMPT_GLOBAL_PARAM,      NULL,    szParam, NULL,     NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}
#define XMATERIAL_PROPERTY_LIST_END()                                 {XMPT_UNKNOWN,           NULL,    NULL,    NULL,     NULL,        GXDECLTYPE_UNUSED, float4_t(0.0f,     0.0f,  0.0f, 0.0f)}

#endif
