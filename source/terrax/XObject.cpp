#include "XObject.h"

float3_t CXObject::getPos()
{
	return(m_vPos);
}
void CXObject::setPos(const float3_t &pos)
{
	m_vPos = pos;
}

bool CXObject::isSelected()
{
	return(m_isSelected);
}

void CXObject::setSelected(bool set)
{
	m_isSelected = set;
}
