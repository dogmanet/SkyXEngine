#include "FuncLadder.h"


BEGIN_PROPTABLE(CInfoLadderDismount)
	// empty
END_PROPTABLE()

REGISTER_ENTITY(CInfoLadderDismount, info_ladder_dismount);

//##########################################################################

BEGIN_PROPTABLE(CFuncLadder)
	DEFINE_FIELD_VECTORFN(m_vUpPoint, PDFF_USE_GIZMO, "up_point", "Up point", setUpPoint, EDITOR_POINTCOORD)

	//! Плеер залез на лестницу
	DEFINE_OUTPUT(m_onPlayerGetOn, "OnPlayerGetOn", "On player get on")
	//! Плеер слез с лестницы
	DEFINE_OUTPUT(m_onPlayerGetOff, "OnPlayerGetOff", "On player get off")

	//! Включает лестницу
	DEFINE_INPUT(turnOn, "turnOn", "Turn on", PDF_NONE)
	//! Выключает лестницу
	DEFINE_INPUT(turnOff, "turnOff", "Turn off", PDF_NONE)
	//! Переключает состояние лестницы
	DEFINE_INPUT(toggle, "toggle", "Toggle", PDF_NONE)

	//! Изначально выключена
	DEFINE_FLAG(LADDER_INITIALLY_DISABLED, "Start disabled")
END_PROPTABLE()

REGISTER_ENTITY(CFuncLadder, func_ladder);

void CFuncLadder::setUpPoint(const float3 &vUp)
{
	m_isUpSet = true;
	m_vUpPoint = vUp;
}

void CFuncLadder::setPos(const float3 &pos)
{
	BaseClass::setPos(pos);
	if(!m_isUpSet)
	{
		m_vUpPoint = (float3)(pos + float3(0.0f, 2.0f, 0.0f));
	}
}

void CFuncLadder::updateFlags()
{
	BaseClass::updateFlags();

	m_isEnabled = !(getFlags() & LADDER_INITIALLY_DISABLED);
}

void CFuncLadder::turnOn(inputdata_t *pInputdata)
{
	m_isEnabled = true;
}
void CFuncLadder::turnOff(inputdata_t *pInputdata)
{
	m_isEnabled = false;
}
void CFuncLadder::toggle(inputdata_t *pInputdata)
{
	m_isEnabled = !m_isEnabled;
}

void CFuncLadder::renderEditor(bool is3D, bool bRenderSelection, IXGizmoRenderer *pRenderer)
{
	if(pRenderer)
	{
		const float4 c_vLineColor = bRenderSelection ? float4(1.0f, 0.0f, 0.0f, 1.0f) : float4(1.0f, 0.0f, 1.0f, 1.0f);
		pRenderer->setColor(c_vLineColor);
		pRenderer->setLineWidth(is3D ? 0.02f : 1.0f);

		SMAABB aabb;
		getMinMax(&aabb.vMin, &aabb.vMax);
		pRenderer->drawAABB(aabb + getPos());
		pRenderer->drawAABB(aabb + m_vUpPoint);
		pRenderer->jumpTo(getPos());
		pRenderer->lineTo(m_vUpPoint);
	}
}

void CFuncLadder::getMinMax(float3 *min, float3 *max)
{
	if(min)
	{
		min->x = -0.25f;
		min->y = 0.0f;
		min->z = -0.25f;
	}

	if(max)
	{
		max->x = 0.25f;
		max->y = 1.8f;
		max->z = 0.25f;
	}
}

bool CFuncLadder::rayTest(const float3 &vStart, const float3 &vEnd, float3 *pvOut, float3 *pvNormal, bool isRayInWorldSpace, bool bReturnNearestPoint)
{
	// TODO Implement me!
	
	return(false);
}
