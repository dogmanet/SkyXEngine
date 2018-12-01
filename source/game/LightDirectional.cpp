
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightDirectional.h"

#include <light/sxlight.h>

/*! \skydocent light_directional
Направленный источник света
*/

BEGIN_PROPTABLE(CLightDirectional)
	//! Цвет
	DEFINE_FIELD_VECTOR(m_vColor, 0, "color", "Color", EDITOR_TEXTFIELD)
	//! Дальность
	DEFINE_FIELD_FLOAT(m_fDist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	//! Дальность дальняя
	DEFINE_FIELD_FLOAT(m_fShadowDist, 0, "shadow_dist", "Shadow distance", EDITOR_TEXTFIELD)
	//! Интенсивность теней
	DEFINE_FIELD_FLOAT(m_fShadowIntensity, 0, "shadow_intensity", "Shadow intensity", EDITOR_TEXTFIELD)

	//! Тип тени
	DEFINE_FIELD_INT(m_iShadowType, 0, "shadow_type", "Shadow type", EDITOR_COMBOBOX)
		COMBO_OPTION("None", "-1")   //!< Нет
		COMBO_OPTION("Static", "0")  //!< Статическая тень
		COMBO_OPTION("Dynamic", "1") //!< Динамическая тень
	EDITOR_COMBO_END()

	//! Угол
	DEFINE_FIELD_FLOAT(m_fAngle, 0, "angle", "Angle", EDITOR_TEXTFIELD)
	//! Верхний радиус
	DEFINE_FIELD_FLOAT(m_fRadiusTop, 0, "radius_top", "Radius top", EDITOR_TEXTFIELD)

END_PROPTABLE()

REGISTER_ENTITY(CLightDirectional, light_directional);

CLightDirectional::CLightDirectional(CEntityManager *pMgr) :
BaseClass(pMgr)
{
	m_vColor = float3(1, 1, 1);
	m_vPosition = float3_t(0, 0, 0);
	m_fDist = 10;
	m_fShadowDist = m_fDist;
	m_iShadowType = 1;
	m_fShadowIntensity = 0;
	m_fAngle = SM_PI * 0.4f;
	m_fRadiusTop = 0.01f;
	m_idLight = SLight_CreateDirection(&float3(0, 0, 0), m_fDist, &(float3)m_vColor, &SMQuaternion(-SM_PI, 'z'), m_fRadiusTop, m_fAngle, true);
	m_isEnable = true;

	float3 f = LIGHTS_DIR_BASE;
	float3 a = SMVector3Cross(f, f);
	float ang = acosf(SMVector3Dot(f, f));
	m_vOrientation = SMQuaternion(a, ang);
}

CLightDirectional::~CLightDirectional()
{
	SLight_DeleteLight(m_idLight);
}

void CLightDirectional::toggleEnable()
{
	m_isEnable = !m_isEnable;
	SLight_SetEnable(m_idLight, m_isEnable);
}

void CLightDirectional::onSync()
{
	BaseClass::onSync();

	if (SLight_GetEnable(m_idLight) != m_isEnable)
		SLight_SetEnable(m_idLight, m_isEnable);
	
	static float3 vec;
	SLight_GetPos(m_idLight, &vec, false);

	if (vec.x != m_vPosition.x || vec.y != m_vPosition.y || vec.z != m_vPosition.z)
		SLight_SetPos(m_idLight, &(float3)m_vPosition, false);

	SLight_SetColor(m_idLight, &(float3)m_vColor);
	
	if (SLight_GetDist(m_idLight) != m_fDist)
	{
		SLight_SetDist(m_idLight, m_fDist, true);
		m_fShadowDist = m_fDist;
	}

	if (SLight_GetShadowLocalFar(m_idLight) != m_fShadowDist)
		SLight_SetShadowLocalFar(m_idLight, m_fShadowDist);

	if (SLight_GetTypeShadowed(m_idLight) != m_iShadowType)
		SLight_SetTypeShadowed(m_idLight, (LTYPE_SHADOW)m_iShadowType);

	SLight_SetShadowCoef(m_idLight, m_fShadowIntensity);

	static SMQuaternion curr_rot;
	SLight_GetOrient(m_idLight, &curr_rot);

	if (curr_rot.x != m_vOrientation.x || curr_rot.y != m_vOrientation.y || curr_rot.z != m_vOrientation.z || curr_rot.w != m_vOrientation.w)
		SLight_SetOrient(m_idLight, &m_vOrientation);

	if (SLight_GetAngle(m_idLight) != m_fAngle)
		SLight_SetAngle(m_idLight, m_fAngle);

	if (SLight_GetTopRadius(m_idLight) != m_fRadiusTop)
		SLight_SetTopRadius(m_idLight, m_fRadiusTop);
}
