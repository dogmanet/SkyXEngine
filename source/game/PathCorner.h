#ifndef _PathCorner_H_
#define _PathCorner_H_

#include "SXpointEntity.h"

enum PATH_CORNER_TYPE
{
	PCT_CORNER = 0,
	PCT_SPLINE = 1
};

class PathCorner: public SXpointEntity
{
	DECLARE_CLASS(PathCorner, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	PathCorner(EntityManager * pMgr);


	float3 GetPoint(float dist);
	SMQuaternion GetRot(float dist);
	float GetLength();

	PathCorner * GetNext();
	PathCorner * GetPrev();

protected:
	void RecalcPath(float t);
	void OnPostLoad();

	int m_type;
	float m_fNewSpeed;

	float m_fLength;

	float3_t m_fH;

	float3_t m_fCoeffsA;
	float3_t m_fCoeffsB;
	float3_t m_fCoeffsC;
	float3_t m_fCoeffsD;

	float3_t m_fDelta;
	float3_t m_fLambda;

	PathCorner * m_pNextStop;
	PathCorner * m_pPrevStop;
};

#endif
