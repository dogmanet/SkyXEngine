
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PathCorner.h"

/*! \skydocent path_corner
Точка для формирования пути для #func_train
*/

BEGIN_PROPTABLE(CPathCorner)
	//! Тип сглаживания пути
	//DEFINE_FIELD_INT(m_type, 0, "type", "Type", EDITOR_COMBOBOX)
	//	COMBO_OPTION("Corner", "0") //!< Нет сглаживания
	//	COMBO_OPTION("Spline", "1") //!< Сплайном
	//EDITOR_COMBO_END()

	//! Новая скорость для поезда
	DEFINE_FIELD_FLOAT(m_fNewSpeed, 0, "speed", "New speed", EDITOR_TEXTFIELD)

	//! Следующая точка пути
	DEFINE_FIELD_ENTITY2FN(CPathCorner, m_pNextStop, 0, "next", "Next stop", setNextPoint, EDITOR_TEXTFIELD)

END_PROPTABLE()

REGISTER_ENTITY(CPathCorner, path_corner);

CPathCorner::CPathCorner(CEntityManager * pMgr):
	BaseClass(pMgr),
	m_type(PCT_SPLINE),
	m_fNewSpeed(0.0f),
	m_pNextStop(NULL),
	m_pPrevStop(NULL),
	m_fLength(0.0f)
{
}

CPathCorner::~CPathCorner()
{
	setNextPoint(NULL);
	if(m_pPrevStop)
	{
		m_pPrevStop->setNextPoint(NULL);
	}
}

void CPathCorner::setNextPoint(CPathCorner *pEnt)
{
	CPathCorner *pCur = pEnt;
	while(pCur && pCur != this)
	{
		pCur = pCur->m_pNextStop;
	}
	if(pCur == this)
	{
		printf(COLOR_LRED "path_corner cycle detected! ent: '%s'" COLOR_RESET "\n", getName());
		return;
	}

	if(m_pNextStop)
	{
		m_pNextStop->m_pPrevStop = NULL;
		m_pNextStop->recalcPath(0);
	}
	m_pNextStop = (CPathCorner*)pEnt;
	if(m_pNextStop)
	{
		m_pNextStop->m_pPrevStop = this;
	}
	recalcPath(0);
}

void CPathCorner::onPostLoad()
{
	BaseClass::onPostLoad();

	if(m_pNextStop)
	{
		m_pNextStop->m_pPrevStop = this;
	}

	if(!m_pPrevStop)
	{
		SET_TIMEOUT(recalcPath, 0);
	}
}

void CPathCorner::recalcPath(float t)
{
	CPathCorner * pNext;
	CPathCorner * pCur = this;
	if(m_pPrevStop)
	{
		while(pCur->m_pPrevStop)
		{
			pCur = pCur->m_pPrevStop;
		}
		pCur->recalcPath(t);
		return;
	}

	while(pCur->m_pNextStop)
	{
		pNext = pCur->m_pNextStop;

		float3 vec = pNext->getPos() - pCur->getPos();
		pCur->m_fLength = SMVector3Length(vec);
		//pCur->m_fLength = 1.0f;
		pCur->m_fH = (float3)(vec / pCur->m_fLength);
		pCur->m_fCoeffsA = pCur->m_pNextStop->m_vPosition;

		pCur = pNext;
	}

	pCur = m_pNextStop; // 1

	if (pCur)
	{
		pCur->m_fDelta = (float3)(-pCur->m_fLength / (2.0f * (m_fLength + pCur->m_fLength)));
		pCur->m_fLambda = (float3)(1.5f * float3(pCur->m_fH - m_fH) / (m_fLength + pCur->m_fLength));
		pCur = pCur->m_pNextStop; // 2

		if (pCur)
		{
			while (pCur->m_pNextStop)
			{
				pNext = pCur->m_pNextStop;

				pCur->m_fDelta = (float3)(-float3(pCur->m_fLength) / (float3(2.0f * pCur->m_pPrevStop->m_fLength + 2.0f * pCur->m_fLength) + pCur->m_pPrevStop->m_fLength * pCur->m_pPrevStop->m_fDelta));
				pCur->m_fLambda = (float3)((3.0f * pCur->m_fH - 3.0f * pCur->m_pPrevStop->m_fH - pCur->m_pPrevStop->m_fLength * pCur->m_pPrevStop->m_fLambda) /
					(float3(2.0f * pCur->m_pPrevStop->m_fLength + 2.0f * pCur->m_fLength) + pCur->m_pPrevStop->m_fLength * pCur->m_pPrevStop->m_fDelta));

				pCur = pNext;
			}

			while (pCur->m_pPrevStop)
			{
				pCur->m_pPrevStop->m_fCoeffsC = (float3)(pCur->m_fDelta * pCur->m_fCoeffsC + pCur->m_fLambda);
				pCur = pCur->m_pPrevStop;
			}
		}
	}
	
	/*pCur = pCur->m_pPrevStop;
	while(pCur->m_pPrevStop)
	{
		pCur->m_fCoeffsC = (float3)(pCur->m_fDelta * pCur->m_pNextStop->m_fCoeffsC + pCur->m_fLambda);
		pCur = pCur->m_pPrevStop;
	}
	*/
	
	pCur = m_pNextStop; // 1
	while(pCur)
	{
		pNext = pCur->m_pNextStop;

		pCur->m_pPrevStop->m_fCoeffsD = (float3)((pCur->m_pPrevStop->m_fCoeffsC - (pCur->m_pPrevStop->m_pPrevStop ? pCur->m_pPrevStop->m_pPrevStop->m_fCoeffsC : float3(0.0f))) / (3.0f * pCur->m_pPrevStop->m_fLength));
		pCur->m_pPrevStop->m_fCoeffsB = (float3)(float3(pCur->m_pPrevStop->m_fH) + (2.0f * pCur->m_pPrevStop->m_fCoeffsC*pCur->m_pPrevStop->m_fLength + pCur->m_pPrevStop->m_fLength * (pCur->m_pPrevStop->m_pPrevStop ? pCur->m_pPrevStop->m_pPrevStop->m_fCoeffsC : float3(0.0f))) / 3.0f);

		pCur = pNext;
	}
	

	/*pCur = m_pNextStop; // 1
	while(pCur)
	{
		pNext = pCur->m_pNextStop;

		pCur->m_fCoeffsD = (float3)((pCur->m_fCoeffsC - pCur->m_pPrevStop->m_fCoeffsC) / (3.0f * pCur->m_pPrevStop->m_fLength));
		pCur->m_fCoeffsB = (float3)(float3(pCur->m_pPrevStop->m_fH) + (2.0f * pCur->m_fCoeffsC*pCur->m_pPrevStop->m_fLength + pCur->m_pPrevStop->m_fLength * pCur->m_pPrevStop->m_fCoeffsC) / 3.0f);

		pCur = pNext;
	}*/

	int a = 0;
}

float CPathCorner::getLength()
{
	return(m_fLength);
}

float3 CPathCorner::getPoint(float dist)
{
	if(dist < 0)
	{
		if(m_pPrevStop)
		{
			return(m_pPrevStop->getPoint(dist + m_pPrevStop->m_fLength));
		}
		else
		{
			return(m_vPosition);
		}
	}
	else if(dist > m_fLength)
	{
		if(m_pNextStop)
		{
			return(m_pNextStop->getPoint(dist - m_fLength));
		}
		else
		{
			return(m_vPosition);
		}
	}
	dist -= m_fLength;
	return(m_fCoeffsA + m_fCoeffsB * dist + m_fCoeffsC * dist * dist + m_fCoeffsD * dist * dist * dist);
}

SMQuaternion CPathCorner::getRot(float dist)
{
	if(dist < 0)
	{
		if(m_pPrevStop)
		{
			return(m_pPrevStop->getRot(dist + m_pPrevStop->m_fLength));
		}
		else
		{
			return(m_vOrientation);
		}
	}
	else if(dist > m_fLength)
	{
		if(m_pNextStop)
		{
			return(m_pNextStop->getRot(dist - m_fLength));
		}
		else
		{
			return(m_vOrientation);
		}
	}
	if(m_pNextStop)
	{
		return(SMquaternionSlerp(m_vOrientation, m_pNextStop->m_vOrientation, (dist / m_fLength)*(dist / m_fLength) * (3.0f - 2.0f * (dist / m_fLength))));
	}
	else
	{
		return(m_vOrientation);
	}
}

CPathCorner * CPathCorner::getNext()
{
	return(m_pNextStop);
}

CPathCorner * CPathCorner::getPrev()
{
	return(m_pPrevStop);
}

void CPathCorner::setPos(const float3 & pos)
{
	CBaseEntity::setPos(pos);

	recalcPath(0);
}

float CPathCorner::getNewSpeed()
{
	return(m_fNewSpeed);
}
