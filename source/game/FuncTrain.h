#ifndef _FuncTrain_H_
#define _FuncTrain_H_

#include "SXpointEntity.h"

class PathCorner;

class FuncTrain: public SXpointEntity
{
	DECLARE_CLASS(FuncTrain, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	FuncTrain(EntityManager * pMgr);

	void Stop();
	void Start();

protected:
	void OnPostLoad();
	void MoveFunc(float dt);

	PathCorner * m_pStartStop;
	PathCorner * m_pCurStop;

	float m_fSpeed;
	float m_fCurDist;

	bool m_bRunning;

	ID m_iPostIval;
};

#endif
