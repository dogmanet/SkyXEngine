#include "stdafx.h"
#include "GeomDetector.h"

#if 0
bool CGeomDetector::IsBox(IGameMesh *pMesh, float3_t *pCenter, float3_t *pLWH)
{
	int iVertNumber = pMesh->GetNumberOfVerts();
	Point3 pt;
	Point3 ptMin, ptMax;
	for(int i = 0; i < iVertNumber; ++i)
	{
		pt = pMesh->GetVertex(i, true);

		ptMin.x = min(ptMin.x, pt.x);
		ptMax.x = max(ptMax.x, pt.x);
		ptMin.y = min(ptMin.y, pt.y);
		ptMax.y = max(ptMax.y, pt.y);
		ptMin.z = min(ptMin.z, pt.z);
		ptMax.z = max(ptMax.z, pt.z);
	}

	for(int i = 0; i < iVertNumber; ++i)
	{
		pt = pMesh->GetVertex(i, true);

		if(!((pt.x == ptMin.x || pt.x == ptMax.x)
			&& (pt.y == ptMin.y || pt.y == ptMax.y)
			&& (pt.z == ptMin.z || pt.z == ptMax.z)))
		{
			return(false);
		}
	}

	pCenter->x = (ptMin.x + ptMax.x) * 0.5f;
	pCenter->y = (ptMin.y + ptMax.y) * 0.5f;
	pCenter->z = (ptMin.z + ptMax.z) * 0.5f;

	pLWH->x = (ptMax.x - ptMin.x) * 0.5f;
	pLWH->y = (ptMax.y - ptMin.y) * 0.5f;
	pLWH->z = (ptMax.z - ptMin.z) * 0.5f;

	return(true);
}

bool CGeomDetector::IsSphere(IGameMesh *pMesh, float3_t *pCenter, float3_t *pLWH)
{
	int iVertNumber = pMesh->GetNumberOfVerts();
	Point3 pt;
	Point3 ptCenter;
	float fRadius = 0.0f;

	Point3 ptMin, ptMax;
	for(int i = 0; i < iVertNumber; ++i)
	{
		pt = pMesh->GetVertex(i, true);

		ptMin.x = min(ptMin.x, pt.x);
		ptMax.x = max(ptMax.x, pt.x);
		ptMin.y = min(ptMin.y, pt.y);
		ptMax.y = max(ptMax.y, pt.y);
		ptMin.z = min(ptMin.z, pt.z);
		ptMax.z = max(ptMax.z, pt.z);
	}

	float fx = ptMax.x - ptMin.x;
	float fy = ptMax.y - ptMin.y;
	float fz = ptMax.z - ptMin.z;

	if(fx != fy || fy != fz)
	{
		return(false);
	}

	fRadius = fx * 0.5f;
	ptCenter.x = (ptMin.x + ptMax.x) * 0.5f;
	ptCenter.y = (ptMin.y + ptMax.y) * 0.5f;
	ptCenter.z = (ptMin.z + ptMax.z) * 0.5f;

	float r21 = fRadius * fRadius, r22;
	for(int i = 0; i < iVertNumber; ++i)
	{
		pt = pMesh->GetVertex(i, true) - ptCenter;

		r22 = pt.x * pt.x + pt.y * pt.y + pt.z * pt.z;

		if(r21 - r22 > 0.0256f)
		{
			return(false);
		}
	}

	pCenter->x = ptCenter.x;
	pCenter->y = ptCenter.y;
	pCenter->z = ptCenter.z;

	pLWH->x = pLWH->y = pLWH->z = fRadius;

	return(true);
}

bool CGeomDetector::IsCylinder(IGameMesh *pMesh, float3_t *pCenter, float *pHeight, float *pRadius)
{
	int iVertNumber = pMesh->GetNumberOfVerts();
	Point3 pt;
	Point3 ptCenter;
	float fRadius = 0.0f;

	Point3 ptMin, ptMax;
	for(int i = 0; i < iVertNumber; ++i)
	{
		pt = pMesh->GetVertex(i, true);

		ptMin.x = min(ptMin.x, pt.x);
		ptMax.x = max(ptMax.x, pt.x);
		ptMin.y = min(ptMin.y, pt.y);
		ptMax.y = max(ptMax.y, pt.y);
		ptMin.z = min(ptMin.z, pt.z);
		ptMax.z = max(ptMax.z, pt.z);
	}

	float fx = ptMax.x - ptMin.x;
	float fy = ptMax.y - ptMin.y;
	float fz = ptMax.z - ptMin.z;

	bool isX = fy == fz && fx > fz;
	bool isY = fx == fz && fz > fx;
	bool isZ = fy == fz && fx != fz;

	if((!isX && !isY && !isZ) || (isX && isY) || (isX && isZ) || (isY && isZ))
	{
		return(false);
	}

	return(false);
}

#endif
