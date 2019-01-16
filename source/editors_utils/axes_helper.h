
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __AXES_HELPER_H
#define __AXES_HELPER_H

#include <windows.h>
#include <common/sxtypes.h>

//#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include <common/array.h>
#include <gcore/sxgcore.h>

inline bool line_intersect_triangle(const float3 &t1, const float3 &t2, const float3 &t3,
	const float3 &l1, const float3 &l2, float3 &p)
{
	float3 n = SMVector3Normalize(SMVector3Cross((t2 - t1), (t3 - t2)));
	float d1 = SMVector3Dot((l1 - t1), n) / SMVector3Length(n), d2 = SMVector3Dot((l2 - t1), n) / SMVector3Length(n);
	if ((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0))
		return(false);
	p = l1 + (l2 - l1) * (-d1 / (d2 - d1));
	if (SMVector3Dot(SMVector3Cross((t2 - t1), (p - t1)), n) <= 0.0f) return(false);
	if (SMVector3Dot(SMVector3Cross((t3 - t2), (p - t2)), n) <= 0.0f) return(false);
	if (SMVector3Dot(SMVector3Cross((t1 - t3), (p - t3)), n) <= 0.0f) return(false);
	return(true);
}

#define AXES_HELPER_MOVE_LENGTH 1.f		/*!< */
#define AXES_HELPER_MOVE_SPEED 5.f		/*!< */

#define AXES_HELPER_ROTATE_LENGTH_WIDTH 1.f	/*!< */
#define AXES_HELPER_ROTATE_HEIGHT 0.1f		/*!< */

#define AXES_HELPER_SCALE_LENGTH 1.f		/*!< */
#define AXES_HELPER_SCALE_ASIZE 0.05f		/*!< */
#define AXES_HELPER_SCALE_A2SIZE 0.15f		/*!< */
//#define AXES_HELPER_SCALE_SPEED 1.f			/*!< */

class CAxesHelper
{
public:
	CAxesHelper();
	~CAxesHelper();

	SX_ALIGNED_OP_MEM

	enum HANDLER_TYPE
	{
		HANDLER_TYPE_NONE,
		HANDLER_TYPE_MOVE,
		HANDLER_TYPE_ROTATE,
		HANDLER_TYPE_SCALE
	};

	void setType(HANDLER_TYPE type);
	HANDLER_TYPE getType();

	void render();

	void setPosition(const float3 & pos);
	void setRotation(const float3 & rot);
	void setRotation(const SMQuaternion & rot);
	//void setScale(const float3 & scale);

	const float3 & getPosition();
	const float3 & getRotation();
	const float3 & getScale();
	const SMQuaternion & getRotationQ();

	void update();

	bool isDragging()
	{
		return(m_bIsDragging);
	}

private:

	float3 m_vPosition;
	float3 m_vRotation;
	SMQuaternion m_qRotation;
	float3 m_vScale, m_vScaleOld;
	bool m_bIsDragging;
	SMPLANE m_movementPlane;
	float3 m_vMovementLinePos;
	float3 m_vMovementLineDir;
	float3 m_vTransOffset;

	void drawMove();
	void drawCylinder(float3_t lwh, DWORD color = 0xFFFFFFFF);
	void drawRotate();
	void drawScale();

	void intersectMove(const float3 & start, const float3 & dir);
	void intersectRotate(const float3 & start, const float3 & dir);
	void intersectScale(const float3 & start, const float3 & dir);

	struct vert
	{
		float3_t pos;
		DWORD color;
	};

	enum HANDLER_AXE
	{
		HANDLER_AXE_NONE = 0x00,
		HANDLER_AXE_X = 0x01,
		HANDLER_AXE_Y = 0x02,
		HANDLER_AXE_XY = 0x03,
		HANDLER_AXE_Z = 0x04,
		HANDLER_AXE_XZ = 0x05,
		HANDLER_AXE_YZ = 0x06,
		HANDLER_AXE_XYZ = 0x07
	};

	HANDLER_AXE m_currentAxe;
	HANDLER_TYPE m_htype;
	
	//float3 m_fStartDragPos;
	SMMATRIX m_mHelperMatScale2;
};

#endif