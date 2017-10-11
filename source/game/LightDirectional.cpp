
#include "LightDirectional.h"

#include <mtllight/sxmtllight.h>

BEGIN_PROPTABLE(CLightDirectional)
	DEFINE_FIELD_VECTOR(m_vColor, 0, "color", "Color", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_fDist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_fShadowDist, 0, "shadow_dist", "Shadow distance", EDITOR_TEXTFIELD)

	DEFINE_FIELD_INT(m_iShadowType, 0, "shadow_type", "Shadow type", EDITOR_COMBOBOX)
		COMBO_OPTION("None", "-1")
		COMBO_OPTION("Static", "0")
		COMBO_OPTION("Dynamic", "1")
	EDITOR_COMBO_END()

	DEFINE_FIELD_FLOAT(m_fAngle, 0, "angle", "Angle", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_fRadiusTop, 0, "radius_top", "Radius top", EDITOR_TEXTFIELD)

END_PROPTABLE()

REGISTER_ENTITY(CLightDirectional, light_directional);

CLightDirectional::CLightDirectional(EntityManager *pMgr) :
BaseClass(pMgr)
{
	m_vColor = float3(1, 1, 1);
	m_vPosition = float3_t(0, 0, 0);
	m_fDist = 10;
	m_fShadowDist = m_fDist;
	m_iShadowType = 1;
	m_fAngle = SM_PI * 0.4f;
	m_fRadiusTop = 0.01f;
	m_idLight = SML_LigthsCreateDirection(&float3(0, 0, 0), m_fDist, &(float3)m_vColor, &SMQuaternion(-SM_PI, 'z'), m_fRadiusTop, m_fAngle, true);
	m_isEnable = true;

	float3 f = LIGHTS_DIR_BASE;
	float3 a = SMVector3Cross(f, f);
	float ang = acosf(SMVector3Dot(f, f));
	m_vOrientation = SMQuaternion(a, ang);
}

CLightDirectional::~CLightDirectional()
{
	SML_LigthsDeleteLight(m_idLight);
}

void CLightDirectional::toggleEnable()
{
	m_isEnable = !m_isEnable;
	SML_LigthsSetEnable(m_idLight, m_isEnable);
}

void CLightDirectional::OnSync()
{
	BaseClass::OnSync();

	if (SML_LigthsGetEnable(m_idLight) != m_isEnable)
		SML_LigthsSetEnable(m_idLight, m_isEnable);
	
	static float3 vec;
	SML_LigthsGetPos(m_idLight, &vec, false);

	if (vec.x != m_vPosition.x || vec.y != m_vPosition.y || vec.z != m_vPosition.z)
		SML_LigthsSetPos(m_idLight, &(float3)m_vPosition, false);

	SML_LigthsSetColor(m_idLight, &(float3)m_vColor);
	
	if (SML_LigthsGetDist(m_idLight) != m_fDist)
	{
		SML_LigthsSetDist(m_idLight, m_fDist, true);
		m_fShadowDist = m_fDist;
	}

	if (SML_LigthsGetShadowLocalFar(m_idLight) != m_fShadowDist)
		SML_LigthsSetShadowLocalFar(m_idLight, m_fShadowDist);

	if (SML_LigthsGetTypeShadowed(m_idLight) != m_iShadowType)
		SML_LigthsSetTypeShadowed(m_idLight, (LTYPE_SHADOW)m_iShadowType);


	static SMQuaternion curr_rot;
	SML_LigthsGetOrient(m_idLight, &curr_rot);

	if (curr_rot.x != m_vOrientation.x || curr_rot.y != m_vOrientation.y || curr_rot.z != m_vOrientation.z || curr_rot.w != m_vOrientation.w)
		SML_LigthsSetOrient(m_idLight, &m_vOrientation);

	if (SML_LigthsGetAngle(m_idLight) != m_fAngle)
		SML_LigthsSetAngle(m_idLight, m_fAngle);

	if (SML_LigthsGetTopRadius(m_idLight) != m_fRadiusTop)
		SML_LigthsSetTopRadius(m_idLight, m_fRadiusTop);
}
