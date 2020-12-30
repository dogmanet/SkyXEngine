#include "FuncRotating.h"

BEGIN_PROPTABLE(CFuncRotating)
	//! Включить
	DEFINE_INPUT(turnOn, "turnOn", "Turn On", PDF_NONE)
	//! Выключить
	DEFINE_INPUT(turnOff, "turnOff", "Turn Off", PDF_NONE)
	//! Переключить
	DEFINE_INPUT(toggle, "toggle", "Toggle", PDF_NONE)

	//! Скорость
	DEFINE_FIELD_FLOAT(m_fSpeed, PDFF_NONE, "speed", "Rotation speed", EDITOR_TEXTFIELD)

	//! Изначально включен
	DEFINE_FLAG(FUNC_ROTATING_START_ENABLED, "Start on")
	//! Если отмечено, то объект будет вращаться в противоположную сторону
	DEFINE_FLAG(FUNC_ROTATING_REVERSE_DIRECTION, "Reverse direction")
END_PROPTABLE()

REGISTER_ENTITY(CFuncRotating, func_rotating);

CFuncRotating::~CFuncRotating()
{
	if(ID_VALID(m_idThinkInterval))
	{
		CLEAR_INTERVAL(m_idThinkInterval);
	}
}

void CFuncRotating::turnOn(inputdata_t *pInputdata)
{
	if(!ID_VALID(m_idThinkInterval))
	{
		m_idThinkInterval = SET_INTERVAL(think, 0);
	}
}

void CFuncRotating::turnOff(inputdata_t *pInputdata)
{
	if(ID_VALID(m_idThinkInterval))
	{
		CLEAR_INTERVAL(m_idThinkInterval);
	}
}

void CFuncRotating::toggle(inputdata_t *pInputdata)
{
	if(ID_VALID(m_idThinkInterval))
	{
		turnOff(pInputdata);
	}
	else
	{
		turnOn(pInputdata);
	}
}

void CFuncRotating::think(float fDt)
{
	float3 fAxis = getOrient() * float3(0.0f, m_isReversed ? -1.0f : 1.0f, 0.0f);
	setOrient((getOrient() * SMQuaternion(fAxis, m_fSpeed * fDt)).Normalize());
}

void CFuncRotating::updateFlags()
{
	BaseClass::updateFlags();

	m_isReversed = !!(getFlags() & FUNC_ROTATING_REVERSE_DIRECTION);

	if(getFlags() & FUNC_ROTATING_START_ENABLED)
	{
		if(!ID_VALID(m_idThinkInterval))
		{
			m_idThinkInterval = SET_INTERVAL(think, 0);
		}
	}
	else
	{
		if(ID_VALID(m_idThinkInterval))
		{
			CLEAR_INTERVAL(m_idThinkInterval);
		}
	}
}
