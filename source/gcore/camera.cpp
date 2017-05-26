
#pragma once
#include <gcore\camera.h>

Frustum::Frustum()
{
	
}

Frustum::~Frustum()
{

}

void Frustum::Update(const float4x4* view,const float4x4* proj)
{
	float4x4 matComb = SMMatrixMultiply(*view, *proj);
	
	ArrFrustumPlane[0].Normal.x = matComb._14 - matComb._11; 
	ArrFrustumPlane[0].Normal.y = matComb._24 - matComb._21; 
	ArrFrustumPlane[0].Normal.z = matComb._34 - matComb._31; 
	ArrFrustumPlane[0].Distance = matComb._44 - matComb._41;
	
	ArrFrustumPlane[1].Normal.x = matComb._14 + matComb._11; 
	ArrFrustumPlane[1].Normal.y = matComb._24 + matComb._21; 
	ArrFrustumPlane[1].Normal.z = matComb._34 + matComb._31; 
	ArrFrustumPlane[1].Distance = matComb._44 + matComb._41;

	ArrFrustumPlane[2].Normal.x = matComb._14 + matComb._12; 
	ArrFrustumPlane[2].Normal.y = matComb._24 + matComb._22; 
	ArrFrustumPlane[2].Normal.z = matComb._34 + matComb._32; 
	ArrFrustumPlane[2].Distance = matComb._44 + matComb._42;

	ArrFrustumPlane[3].Normal.x = matComb._14 - matComb._12; 
	ArrFrustumPlane[3].Normal.y = matComb._24 - matComb._22; 
	ArrFrustumPlane[3].Normal.z = matComb._34 - matComb._32; 
	ArrFrustumPlane[3].Distance = matComb._44 - matComb._42;

	ArrFrustumPlane[4].Normal.x = matComb._14 - matComb._13; 
	ArrFrustumPlane[4].Normal.y = matComb._24 - matComb._23; 
	ArrFrustumPlane[4].Normal.z = matComb._34 - matComb._33; 
	ArrFrustumPlane[4].Distance = matComb._44 - matComb._43;

	ArrFrustumPlane[5].Normal.x = matComb._14 + matComb._13; 
	ArrFrustumPlane[5].Normal.y = matComb._24 + matComb._23; 
	ArrFrustumPlane[5].Normal.z = matComb._34 + matComb._33; 
	ArrFrustumPlane[5].Distance = matComb._44 + matComb._43;

		//Нормализация плоскостей
		for (int i = 0; i < 6; ++i)
			ArrFrustumPlane[i].Normalize();
}

bool Frustum::PointInFrustum(const float3 *point)
{
		for (int i=0; i<6; i++)
		{
			float tmp = ArrFrustumPlane[i].Normal.x*(point->x) + ArrFrustumPlane[i].Normal.y*(point->y) +  ArrFrustumPlane[i].Normal.z*(point->z) + ArrFrustumPlane[i].Distance;
				if(long(tmp * 1000.0f) <= long(0 * 1000.0f))
				{
					return false;
				}
		}
    return true;
}

bool Frustum::PolyInFrustum(const float3* p1,const float3* p2,const float3* p3)
{
		if(PointInFrustum(p1) || PointInFrustum(p2) || PointInFrustum(p3))
			return true;

	return false;
}

bool Frustum::PolyInFrustumAbs(const float3* p1,const float3* p2,const float3* p3)
{
		if(PointInFrustum(p1) && PointInFrustum(p2) && PointInFrustum(p3))
			return true;

	return false;
}
			
bool Frustum::SphereInFrustum(const float3 *point, float radius) const
{
		for (int i=0; i<6; ++i)
		{
				if (ArrFrustumPlane[i].Normal.x*point->x + ArrFrustumPlane[i].Normal.y*point->y + ArrFrustumPlane[i].Normal.z*point->z + ArrFrustumPlane[i].Distance <= -radius)
					return false;
		}
	return true;
}

bool Frustum::SphereInFrustumAbs(const float3 *point, float radius)
{
		for (int i=0; i<6; i++)
		{
				if (ArrFrustumPlane[i].Normal.x*point->x + ArrFrustumPlane[i].Normal.y*point->y + ArrFrustumPlane[i].Normal.z*point->z + ArrFrustumPlane[i].Distance > -radius)
					return false;
		}
	return true;
}

bool Frustum::BoxInFrustum(float3* min,float3* max)
{
		for ( register int p = 0; p < 6; p++ )
		{
			if( ArrFrustumPlane[p].Normal.x * (min->x) + ArrFrustumPlane[p].Normal.y * (min->y) + ArrFrustumPlane[p].Normal.z * (min->z) + ArrFrustumPlane[p].Distance > 0 ) continue;

			if( ArrFrustumPlane[p].Normal.x * (min->x) + ArrFrustumPlane[p].Normal.y * (min->y) + ArrFrustumPlane[p].Normal.z * (max->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			if( ArrFrustumPlane[p].Normal.x * (min->x) + ArrFrustumPlane[p].Normal.y * (max->y) + ArrFrustumPlane[p].Normal.z * (max->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			if( ArrFrustumPlane[p].Normal.x * (max->x) + ArrFrustumPlane[p].Normal.y * (max->y) + ArrFrustumPlane[p].Normal.z * (max->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			if( ArrFrustumPlane[p].Normal.x * (max->x) + ArrFrustumPlane[p].Normal.y * (min->y) + ArrFrustumPlane[p].Normal.z * (min->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			if( ArrFrustumPlane[p].Normal.x * (max->x) + ArrFrustumPlane[p].Normal.y * (max->y) + ArrFrustumPlane[p].Normal.z * (min->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			if( ArrFrustumPlane[p].Normal.x * (max->x) + ArrFrustumPlane[p].Normal.y * (min->y) + ArrFrustumPlane[p].Normal.z * (max->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			if( ArrFrustumPlane[p].Normal.x * (min->x) + ArrFrustumPlane[p].Normal.y * (max->y) + ArrFrustumPlane[p].Normal.z * (min->z) + ArrFrustumPlane[p].Distance > 0 ) continue;
			return false;
		}

	return true;
}

/////////////////////////

Camera::Camera()
{
	Position = float3(0.0f, 0.0f, 0.0f);
	Right = float3(1.0f, 0.0f, 0.0f);
	Up = float3(0.0f, 1.0f, 0.0f);
	Look = float3(0.0f, 0.0f, 1.0f);

	AngleUpDown=AngleRightLeft=AngleRoll=0;

	ObjFrustum = new Frustum();

}

Camera::~Camera	()
{
	mem_delete(ObjFrustum);
}

inline void Camera::PosLeftRight(float units)
{
	Position += float3(Right.x, 0.0f, Right.z) * units;
	LastVal.x = units;
}

inline void Camera::PosUpDown(float units)
{
	Position.y += Up.y * units;
}

inline void Camera::PosFrontBack(float units)
{
	Position += float3(Look.x, 0.0f, Look.z) * units;
	LastVal.z = units;
}

			
inline void Camera::RotUpDown(float angle)
{
		/*if(AngleUpDown + angle > 1.1f)
			angle = 1.1f - AngleUpDown;

		if(AngleUpDown + angle < -1.2f)
			angle = -1.2f - AngleUpDown;*/

	float4x4 T = SMMatrixRotationAxis(Right, angle);

	Up = SMVector3Transform(Up, T);
	Look = SMVector3Transform(Look, T);

	AngleUpDown += angle;
		if(abs(AngleUpDown) >= SM_2PI)
			AngleUpDown = SM_2PI * modf((AngleUpDown / SM_2PI),&angle);

	/*D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &Right,	angle);

	D3DXVec3TransformCoord(&Up,&Up, &T);
	D3DXVec3TransformCoord(&Look,&Look, &T);*/
}

inline void Camera::RotRightLeft(float angle)
{
	float4x4 T = SMMatrixRotationY(angle);

	Right = SMVector3Transform(Right, T);
	Look = SMVector3Transform(Look, T);

	AngleRightLeft += angle;
		if(abs(AngleRightLeft) >= SM_2PI)
			AngleRightLeft = SM_2PI * modf((AngleRightLeft / SM_2PI),&angle);

	/*D3DXMATRIX T;

	D3DXMatrixRotationY(&T, angle);

	D3DXVec3TransformCoord(&Right,&Right, &T);
	D3DXVec3TransformCoord(&Look,&Look, &T);*/
}

inline void Camera::Roll(float angle)
{
	//AngleRoll = angle;
	float4x4 T = SMMatrixRotationAxis(Look,	angle);

	Right = SMVector3Transform(Right, T);
	Up = SMVector3Transform(Up, T);
	/*if(!Rolling)
	{Rolling = true;
	AngleRoll = angle;
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &Look,	angle);

		D3DXVec3TransformCoord(&Right,&Right, &T);
		D3DXVec3TransformCoord(&Up,&Up, &T);
	}*/
}

inline void Camera::SetOrientation(const SMQuaternion & q)
{
	float3 angles = SMMatrixToEuler(q.GetMatrix());
	AngleUpDown = angles.x;
	AngleRightLeft = angles.y;
	AngleRoll = angles.z;

	Right = q * float3(1.0f, 0.0f, 0.0f);
	Up = q * float3(0.0f, 1.0f, 0.0f);
	Look = q * float3(0.0f, 0.0f, 1.0f);
}


inline void Camera::GetViewMatrix(float4x4* view_matrix)
{
	Look = SMVector3Normalize(Look);

	Up = SMVector3Cross(Look, Right);
	Up = SMVector3Normalize(Up);

	Right = SMVector3Cross(Up, Look);
	Right = SMVector3Normalize(Right);

	float x = -SMVector3Dot(Right, Position);
	float y = -SMVector3Dot(Up, Position);
	float z = -SMVector3Dot(Look, Position);

	(*view_matrix)._11 = Right.x;	(*view_matrix)._12 = Up.x;	(*view_matrix)._13 = Look.x; (*view_matrix)._14 = 0.0f;
	(*view_matrix)._21 = Right.y;	(*view_matrix)._22 = Up.y;	(*view_matrix)._23 = Look.y; (*view_matrix)._24 = 0.0f;
	(*view_matrix)._31 = Right.z;	(*view_matrix)._32 = Up.z;	(*view_matrix)._33 = Look.z; (*view_matrix)._34 = 0.0f;
	(*view_matrix)._41 = x;			(*view_matrix)._42 = y;		(*view_matrix)._43 = z;      (*view_matrix)._44 = 1.0f;

	/*D3DXVec3Normalize(&Look, &Look);

	D3DXVec3Cross(&Up, &Look, &Right);
	D3DXVec3Normalize(&Up, &Up);

	D3DXVec3Cross(&Right, &Up, &Look);
	D3DXVec3Normalize(&Right, &Right);

	float x = -D3DXVec3Dot(&Right, &Position);
	float y = -D3DXVec3Dot(&Up, &Position);
	float z = -D3DXVec3Dot(&Look, &Position);

	(*view_matrix)(0,0) = Right.x; (*view_matrix)(0, 1) = Up.x; (*view_matrix)(0, 2) = Look.x; (*view_matrix)(0, 3) = 0.0f;
	(*view_matrix)(1,0) = Right.y; (*view_matrix)(1, 1) = Up.y; (*view_matrix)(1, 2) = Look.y; (*view_matrix)(1, 3) = 0.0f;
	(*view_matrix)(2,0) = Right.z; (*view_matrix)(2, 1) = Up.z; (*view_matrix)(2, 2) = Look.z; (*view_matrix)(2, 3) = 0.0f;
	(*view_matrix)(3,0) = x;       (*view_matrix)(3, 1) = y;    (*view_matrix)(3, 2) = z;      (*view_matrix)(3, 3) = 1.0f;*/
}

			
inline void Camera::GetPosition(float3* pos)
{
	*pos = Position;
}

inline void Camera::SetPosition(float3* pos)
{
	Position = *pos;
}


inline void Camera::GetDirection(float3* dir)
{

}

inline void Camera::SetDirection(float3* dir)
{

}

			
inline void Camera::GetRight(float3* right)
{
	*right = Right;
}

inline void Camera::GetUp(float3* up)
{
	*up = Up;
}

inline void Camera::GetLook(float3* look)
{
	*look = Look;
}

inline void Camera::GetRotation(float3* rot)
{
	rot->x = AngleUpDown;
	rot->y = AngleRightLeft;
	rot->z = AngleRoll;
}

inline float Camera::GetRotationX()
{
	return AngleUpDown;
}

inline float Camera::GetRotationY()
{
	return AngleRightLeft;
}

inline float Camera::GetRotationZ()
{
	return AngleRoll;
}
