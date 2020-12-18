
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "FuncTrain.h"
#include "PathCorner.h"

/*! \skydocent func_train
Поезд, движется по траектории, построенной из path_corner
*/

BEGIN_PROPTABLE(CFuncTrain)
	//! Скорость движения
	DEFINE_FIELD_FLOAT(m_fSpeed, 0, "speed", "Move speed", EDITOR_TEXTFIELD)

	//! path_corner, с которого начнется движение
	DEFINE_FIELD_ENTITY2(CPathCorner, m_pStartStop, 0, "start", "Start point", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY(CFuncTrain, func_train);

void CFuncTrain::onPostLoad()
{
	BaseClass::onPostLoad();
	m_pCurStop = m_pStartStop;
	if(m_pStartStop)
	{
	//	setPos(m_pStartStop->getPos());
	//	setOrient(m_pStartStop->getOrient());
	}
}

void CFuncTrain::stop()
{
	if(m_bRunning)
	{
		m_bRunning = false;
		CLEAR_INTERVAL(m_iPostIval);
	}
}

void CFuncTrain::start()
{
	if(!m_bRunning)
	{
		m_bRunning = true;
		m_iPostIval = SET_INTERVAL(moveFunc, 1.0f/60.0f);
	}
}

void CFuncTrain::moveFunc(float dt)
{
	m_fCurDist += m_fSpeed * dt;
	while(m_pCurStop && (m_fCurDist > m_pCurStop->getLength()))
	{
		m_fCurDist -= m_pCurStop->getLength();
		m_pCurStop = m_pCurStop->getNext();
	}
	if(m_pCurStop)
	{
		setPos(m_pCurStop->getPoint(m_fCurDist));
		setOrient(m_pCurStop->getRot(m_fCurDist));
	}
	else
	{
		m_pCurStop = m_pStartStop;
		while(m_pCurStop->getNext())
		{
			m_pCurStop = m_pCurStop->getNext();
		}
		setPos(m_pCurStop->getPoint(m_fCurDist));
		setOrient(m_pCurStop->getRot(m_fCurDist));
		stop();
	}
}
