
#include "LightPoint.h"

#include <mtllight/sxmtllight.h>

BEGIN_PROPTABLE(CLightPoint)
	DEFINE_FIELD_VECTOR(m_vColor, 0, "color", "Color", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_fDist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	DEFINE_FIELD_FLOAT(m_fShadowDist, 0, "light_far", "Light far", EDITOR_TEXTFIELD)

	DEFINE_FIELD_INT(m_iShadowType, 0, "type_shadow", "Type shadow", EDITOR_COMBOBOX)
		COMBO_OPTION("None", "-1")
		COMBO_OPTION("Static", "0")
		COMBO_OPTION("Dynamic", "1")
	EDITOR_COMBO_END()

END_PROPTABLE()

REGISTER_ENTITY(CLightPoint, light_point);

CLightPoint::CLightPoint(EntityManager * pMgr) :
BaseClass(pMgr)
{
	m_vColor = float3(1, 1, 1);
	m_vPosition = float3_t(0, 0, 0);
	m_fDist = 10;
	m_fShadowDist = m_fDist;
	m_iShadowType = 1;
	m_idLight = SML_LigthsCreatePoint(&float3(0, 0, 0), m_fDist, &m_vColor, false, true);
}

CLightPoint::~CLightPoint()
{
	SML_LigthsDeleteLight(m_idLight);
}

void CLightPoint::toggleEnable()
{
	m_isEnable = !m_isEnable;
	SML_LigthsSetEnable(m_idLight, m_isEnable);
}

void CLightPoint::OnSync()
{
	BaseClass::OnSync();

	if (SML_LigthsGetEnable(m_idLight) != m_isEnable)
		SML_LigthsSetEnable(m_idLight, m_isEnable);

	static float3 vec;
	SML_LigthsGetPos(m_idLight, &vec, false);

	if (vec.x != m_vPosition.x || vec.y != m_vPosition.y || vec.z != m_vPosition.z)
		SML_LigthsSetPos(m_idLight, &(float3)m_vPosition, false);

	SML_LigthsSetColor(m_idLight, &m_vColor);
	
	if (SML_LigthsGetDist(m_idLight) != m_fDist)
	{
		SML_LigthsSetDist(m_idLight, m_fDist, true);
		m_fShadowDist = m_fDist;
	}

	if (SML_LigthsGetShadowLocalFar(m_idLight) != m_fShadowDist)
		SML_LigthsSetShadowLocalFar(m_idLight, m_fShadowDist);

	if (SML_LigthsGetTypeShadowed(m_idLight) != m_iShadowType)
		SML_LigthsSetTypeShadowed(m_idLight, (LightsTypeShadow)m_iShadowType);
}
