
#include "camera.h"

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

	ObjFrustum = new Frustum();

	m_vPitchYawRoll = float3_t(0, 0, 0);
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
	m_vPitchYawRoll.x -= angle;
	if(m_vPitchYawRoll.x > SM_PIDIV2)
	{
		m_vPitchYawRoll.x = SM_PIDIV2;
	}
	else if(m_vPitchYawRoll.x < -SM_PIDIV2)
	{
		m_vPitchYawRoll.x = -SM_PIDIV2;
	}

	UpdateView();
}

inline void Camera::RotRightLeft(float angle)
{
	m_vPitchYawRoll.y -= angle;
	while(m_vPitchYawRoll.y < 0.0f)
	{
		m_vPitchYawRoll.y += SM_2PI;
	}
	while(m_vPitchYawRoll.y > SM_2PI)
	{
		m_vPitchYawRoll.y -= SM_2PI;
	}

	UpdateView();
}

inline void Camera::Roll(float angle)
{
	m_vPitchYawRoll.z -= angle;
	UpdateView();
}

inline void Camera::UpdateView()
{
	SMQuaternion q = SMQuaternion(m_vPitchYawRoll.x, 'x')
		* SMQuaternion(m_vPitchYawRoll.y, 'y')
		* SMQuaternion(m_vPitchYawRoll.z, 'z');

	Right = q * float3(1.0f, 0.0f, 0.0f);
	Up = q * float3(0.0f, 1.0f, 0.0f);
	Look = q * float3(0.0f, 0.0f, 1.0f);
}

inline void Camera::SetOrientation(const SMQuaternion & q)
{
	m_vPitchYawRoll = SMMatrixToEuler(q.GetMatrix());

	Right = q * float3(1.0f, 0.0f, 0.0f);
	Up = q * float3(0.0f, 1.0f, 0.0f);
	Look = q * float3(0.0f, 0.0f, 1.0f);
}


inline void Camera::GetViewMatrix(float4x4* view_matrix)
{
	*view_matrix = SMMatrixLookToLH(Position, Look, Up);
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
	*rot = m_vPitchYawRoll;
}

inline float Camera::GetRotationX()
{
	return m_vPitchYawRoll.x;
}

inline float Camera::GetRotationY()
{
	return m_vPitchYawRoll.y;
}

inline float Camera::GetRotationZ()
{
	return m_vPitchYawRoll.z;
}

inline void Camera::SetFOV(float fov)
{
	m_fFOV = fov;
}

inline float Camera::GetFOV()
{
	return(m_fFOV);
}
