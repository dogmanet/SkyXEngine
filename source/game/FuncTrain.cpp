#include "FuncTrain.h"
#include "PathCorner.h"


BEGIN_PROPTABLE(FuncTrain)
	DEFINE_FIELD_FLOAT(m_fSpeed, 0, "speed", "Move speed", EDITOR_TEXTFIELD)
	DEFINE_FIELD_ENTITY(m_pStartStop, 0, "start", "Start point", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY(FuncTrain, func_train);

FuncTrain::FuncTrain(EntityManager * pMgr):
	BaseClass(pMgr),
	m_fSpeed(0.0f),
	m_pStartStop(NULL),
	m_bRunning(false),
	m_fCurDist(0.0f)
{
}

void FuncTrain::OnPostLoad()
{
	BaseClass::OnPostLoad();
	m_pCurStop = m_pStartStop;
	if(m_pStartStop)
	{
	//	SetPos(m_pStartStop->GetPos());
	//	SetOrient(m_pStartStop->GetOrient());
	}
}

void FuncTrain::Stop()
{
	if(m_bRunning)
	{
		m_bRunning = false;
		CLEAR_INTERVAL(m_iPostIval);
	}
}

void FuncTrain::Start()
{
	if(!m_bRunning)
	{
		m_bRunning = true;
		m_iPostIval = SET_INTERVAL(MoveFunc, 1.0f/60.0f);
	}
}

void FuncTrain::MoveFunc(float dt)
{
	m_fCurDist += m_fSpeed * dt;
	while(m_pCurStop && (m_fCurDist > m_pCurStop->GetLength()))
	{
		m_fCurDist -= m_pCurStop->GetLength();
		m_pCurStop = m_pCurStop->GetNext();
	}
	if(m_pCurStop)
	{
		SetPos(m_pCurStop->GetPoint(m_fCurDist));
		SetOrient(m_pCurStop->GetRot(m_fCurDist));
	}
	else
	{
		m_pCurStop = m_pStartStop;
		while(m_pCurStop->GetNext())
		{
			m_pCurStop = m_pCurStop->GetNext();
		}
		SetPos(m_pCurStop->GetPoint(m_fCurDist));
		SetOrient(m_pCurStop->GetRot(m_fCurDist));
		Stop();
	}
}
