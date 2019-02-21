#include "XObject.h"

float3_t CXObject::getPos()
{
	return(m_vPos);
}
void CXObject::setPos(const float3_t &pos)
{
	m_vPos = pos;
}

SMQuaternion CXObject::getOrient()
{
	return(SMQuaternion());
}
void CXObject::setOrient(const SMQuaternion &orient)
{
}

float3_t CXObject::getScale()
{
	return(float3_t(1.0f, 1.0f, 1.0f));
}
void CXObject::setScale(const float3_t &pos)
{
}

bool CXObject::isSelected()
{
	return(m_isSelected);
}

void CXObject::setSelected(bool set)
{
	m_isSelected = set;
}
