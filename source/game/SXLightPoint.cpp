
#include "SXLightPoint.h"

#include <mtllight/sxmtllight.h>

BEGIN_PROPTABLE(SXLightPoint)
	DEFINE_FIELD_VECTOR(m_color, 0, "color", "Color", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_power, 0, "power", "Power", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_dist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_light_far, 0, "light_far", "Light far", EDITOR_TEXTFIELD)

	DEFINE_FIELD_INT(m_typeshadow, 0, "type_shadow", "Type shadow", EDITOR_COMBOBOX)
		COMBO_OPTION("None", "-1")
		COMBO_OPTION("Static", "0")
		COMBO_OPTION("Dynamic", "1")
	EDITOR_COMBO_END()

END_PROPTABLE()

REGISTER_ENTITY(SXLightPoint, light_point);

SXLightPoint::SXLightPoint(EntityManager * pMgr) :
BaseClass(pMgr)
{
	m_color = float3(1, 1, 1);
	m_vPosition = float3_t(0, 0, 0);
	m_power = 10;
	m_dist = 10;
	m_light_far = m_dist;
	m_typeshadow = 1;
	IDLight = SML_LigthsCreatePoint(&float3(0, 0, 0), m_power, m_dist, &m_color, false, true);
}

SXLightPoint::~SXLightPoint()
{
	SML_LigthsDeleteLight(IDLight);
}

void SXLightPoint::OnSync()
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
}
