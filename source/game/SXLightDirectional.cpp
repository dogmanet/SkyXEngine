
#include "SXLightDirectional.h"

#include <mtllight/sxmtllight.h>

BEGIN_PROPTABLE(SXLightDirectional)
	DEFINE_FIELD_VECTOR(m_color, 0, "color", "Color", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_power, 0, "power", "Power", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_dist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_light_far, 0, "light_far", "Light far", EDITOR_TEXTFIELD)

	DEFINE_FIELD_INT(m_typeshadow, 0, "type_shadow", "Type shadow", EDITOR_COMBOBOX)
		COMBO_OPTION("None", "-1")
		COMBO_OPTION("Static", "0")
		COMBO_OPTION("Dynamic", "1")
	EDITOR_COMBO_END()

	DEFINE_FIELD_FLOAT(m_angle, 0, "angle", "Angle", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_topradius, 0, "topradius", "Top radius", EDITOR_TEXTFIELD)

END_PROPTABLE()

REGISTER_ENTITY(SXLightDirectional, light_directional);

SXLightDirectional::SXLightDirectional(EntityManager * pMgr) :
BaseClass(pMgr)
{
	m_color = float3(1, 1, 1);
	m_vPosition = float3_t(0, 0, 0);
	m_power = 10;
	m_dist = 10;
	m_light_far = m_dist;
	m_typeshadow = 1;
	m_angle = SM_PI * 0.4f;
	m_topradius = 0.01f;
	IDLight = SML_LigthsCreateDirection(&float3(0, 0, 0), m_power, m_dist, &m_color, &float3(0, -1, 0), m_topradius, m_angle, true);

	float3 f = LIGHTS_DIR_BASE;
	float3 a = SMVector3Cross(f, f);
	float ang = acosf(SMVector3Dot(f, f));
	m_vOrientation = SMQuaternion(a, ang);
}

SXLightDirectional::~SXLightDirectional()
{
	SML_LigthsDeleteLight(IDLight);
}

void SXLightDirectional::OnSync()
{
	BaseClass::OnSync();
	
	static float3 vec;
	SML_LigthsGetPos(IDLight, &vec, false);

	if (vec.x != m_vPosition.x || vec.y != m_vPosition.y || vec.z != m_vPosition.z)
		SML_LigthsSetPos(IDLight, &(float3)m_vPosition, false);

	SML_LigthsSetColor(IDLight, &m_color);
	SML_LigthsSetPower(IDLight, m_power);
	
	if (SML_LigthsGetDist(IDLight) != m_dist)
	{
		SML_LigthsSetDist(IDLight, m_dist, true);
		m_light_far = m_dist;
	}

	if (SML_LigthsGetShadowLocalFar(IDLight) != m_light_far)
		SML_LigthsSetShadowLocalFar(IDLight, m_light_far);

	if (SML_LigthsGetTypeShadowed(IDLight) != m_typeshadow)
		SML_LigthsSetTypeShadowed(IDLight, (LightsTypeShadow)m_typeshadow);


	static float3 curr_rot;
	curr_rot = SMMatrixToEuler(m_vOrientation.GetMatrix());
	SML_LigthsGetRot(IDLight, &vec);

	if (vec.x != curr_rot.x || vec.y != curr_rot.y || vec.z != curr_rot.z)
		SML_LigthsSetRot(IDLight, &curr_rot);

	if (SML_LigthsGetAngle(IDLight) != m_angle)
		SML_LigthsSetAngle(IDLight, m_angle);

	if (SML_LigthsGetTopRadius(IDLight) != m_topradius)
		SML_LigthsSetTopRadius(IDLight, m_topradius);
}
