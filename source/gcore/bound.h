
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __BOUND_H
#define __BOUND_H

#include <common/SXMath.h>
#include <d3dx9.h>
#include "sxgcore.h"

void CreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, IDirect3DDevice9 * pDevice,UINT iSideCount);

void ComputeBoundingBox(IDirect3DVertexBuffer9* vertex_buffer,ISXBound** bound,DWORD count_vert,DWORD bytepervert);
void ComputeBoundingBox2(IDirect3DVertexBuffer9* vertex_buffer,ISXBound* bound,DWORD count_vert,DWORD bytepervert);

bool InPosition2D(float3* min,float3* max,float3* pos);
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

void CreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh, IDirect3DDevice9* device);

//простой объект с минимальным описанием
//дл€ корректного использовани€ необходимо сначала установить позицию/поворот/масштаб после чего CalculateWorld
struct CSXTransObject : public virtual ISXTransObject
{
	CSXTransObject(){};
	~CSXTransObject(){};

	void Release(){ mem_del(this); };

	SX_ALIGNED_OP_MEM

	float4x4* calcWorld();
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
//дл€ создан€и минимума и максимума необходимо вызвать CalculateBound
//SetMinMax, GetMinMax до вызова CalculateWorldAndTrans возвращают нетрансформирвоанные данные
//конечным этапом построени€ Bound и Object €вл€етс€ CalculateWorldAndTrans
class CSXBound : public CSXTransObject, public virtual ISXBound
{
public:
	CSXBound(){};
	~CSXBound(){};

	void Release(){ mem_del(this); };

	SX_ALIGNED_OP_MEM

	void calcBound(IDirect3DVertexBuffer9* vertex_buffer, DWORD count_vert, DWORD bytepervert);

	//функци€ просчета мировой матрицы и трансформации минимума и максимума
	float4x4* calcWorldAndTrans();

	void getPosBBScreen(SXPosBBScreen *res, float3* campos, float3* sizemapdepth, float4x4* mat);

	void setMinMax(const float3* min, const float3* max);
	void getMinMax(float3* min, float3* max) const;

	void setSphere(const float3* center, float radius);
	void getSphere(float3* center, float* radius) const;

	bool isPointInSphere(const float3* point) const;
	bool isPointInBox(const float3* point) const;

protected:
	float3 m_vMin;
	float3 m_vMax;

	float3 m_vCenter;
	float m_fRadius;
};

#endif