
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __BOUND_H
#define __BOUND_H

#include <common/Math.h>
#include "sxgcore.h"

void CreateCone(float fTopRadius, float fBottomRadius, float fHeight, IMesh ** ppMesh, IGXContext * pDevice, UINT iSideCount);
void CreateSphere(float fRadius, UINT iSideCount, UINT iStackCount, IMesh ** ppMesh, IGXContext * pDevice);

bool InPosition2D(const float3* min, const float3* max, const float3* pos);
bool InPositionAbs2D(float3* min,float3* max,float3* pos);

int CountPositionPoints2D(float3* min,float3* max,float3* p1,float3* p2,float3* p3);
int CountPositionPointsAbs2D(float3* min,float3* max,float3* p1,float3* p2,float3* p3);
bool InPositionPoints2D(float3* min,float3* max,float3* p1,float3* p2,float3* p3);

bool InPosition3D(float3* min,float3* max,float3* pos);
bool InPositionAbs3D(float3* min,float3* max,float3* pos);

int CountPositionPoints3D(float3* min,float3* max,float3* p1,float3* p2,float3* p3);
int CountPositionPointsAbs3D(float3* min,float3* max,float3* p1,float3* p2,float3* p3);
bool InPositionPoints3D(float3* min,float3* max,float3* p1,float3* p2,float3* p3);

void ComputeBoundingBoxArr8(ISXBound* bound,ISXBound** bound_arr);
void ComputeBoundingBoxArr4(ISXBound* bound,ISXBound** bound_arr);

void CreateBoundingBoxMesh(const float3* min, const float3* max, IMesh** bbmesh, IGXContext* device);

//простой объект с минимальным описанием
//для корректного использования необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
class CTransObject : public virtual ITransObject
{
public:
	CTransObject();
	~CTransObject(){};

	void Release(){ mem_del(this); };

	SX_ALIGNED_OP_MEM

	const float4x4* calcWorld();

	void setPosition(const float3 *pPos);
	void setRotation(const float3 *pRot);
	void setScale(const float3 *pScale);

	const float3* getPosition(float3 *pPos = 0);
	const float3* getRotation(float3 *pRot = 0);
	const float3* getScale(float3 *pScale = 0);

protected:

	float3 m_vPosition;	//!< позиция
	float3 m_vRotation;	//!< повороты
	float3 m_vScale;	//!< масштабирование
	float4x4 m_mWorld;	//!< мировая матрица на основе поворотов масштабирования и позиции
};

#define TRANSFORM_COORD_SCREEN2(point,sizemapdepth)\
	point.x /= abs(point.w); \
	point.y /= abs(point.w); \
	point.z /= sizemapdepth->z; \
	point.x = point.x * 0.5 + 0.5; \
	point.y = point.y * (-0.5) + 0.5; \
	point.x *= sizemapdepth->x; \
	point.y *= sizemapdepth->y;

//класс ограничивающего объема
//для созданяи минимума и максимума необходимо вызвать CalculateBound
//SetMinMax, GetMinMax до вызова CalculateWorldAndTrans возвращают нетрансформирвоанные данные
//конечным этапом построения Bound и Object является CalculateWorldAndTrans
class CBound : public CTransObject, public virtual ISXBound
{
public:
	CBound() :CTransObject(){};
	~CBound(){};

	void Release(){ mem_del(this); };

	SX_ALIGNED_OP_MEM;

	void calcBound(float3_t *pVertexBuffer, int iCountVertex, int iBytePerVertex) override;
	void calcBoundIndex(float3_t *pVertexBuffer, uint32_t **ppArrIndex, uint32_t *pCountIndex, int iCountSubset, int iBytePerVertex) override;

	//функция просчета мировой матрицы и трансформации минимума и максимума
	//float4x4* calcWorldAndTrans();

	void resetTransform();

	//void getPosBBScreen(SXPosBBScreen *res, float3* campos, float3* sizemapdepth, float4x4* mat);

	void setMinMax(const float3* min, const float3* max);
	void getMinMax(float3* min, float3* max) const;

	void setSphere(const float3* center, float radius);
	void getSphere(float3* center, float* radius) const;

	bool isPointInSphere(const float3* point) const;
	bool isPointInBox(const float3* point) const;

	void cloneFrom(ISXBound *pFrom);

protected:
	float3 m_vMinTransform;
	float3 m_vMaxTransform;

	float3 m_vCenterTransform;
	float m_fRadiusTransform;

	float3 m_vMinOrigin;
	float3 m_vMaxOrigin;

	float3 m_vCenterOrigin;
	float m_fRadiusOrigin;
};

#endif