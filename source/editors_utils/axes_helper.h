
#ifndef axes_helper_h
#define axes_helper_h

#include <windows.h>
#include <common/sxtypes.h>

#define SM_D3D_CONVERSIONS
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
#define AXES_HELPER_SCALE_SPEED 1.f			/*!< */

class AxesHelper
{
public:
	AxesHelper();
	~AxesHelper();

	SX_ALIGNED_OP_MEM

	enum HANDLER_TYPE
	{
		HT_NONE,
		HT_MOVE,
		HT_ROTATE,
		HT_SCALE
	};

	void SetType(HANDLER_TYPE type);
	HANDLER_TYPE GetType();

	void Render();

	void SetPosition(const float3 & pos);
	void SetRotation(const float3 & rot);
	void SetRotation(const SMQuaternion & rot);
	void SetScale(const float3 & scale);

	const float3 & GetPosition();
	const float3 & GetRotation();
	const float3 & getScale();
	const SMQuaternion & GetRotationQ();

	void OnMouseMove(int x, int y);

	bool m_bIsDragging;
	bool m_bIsDraggingStart;
	bool m_bIsDraggingStop;

private:

	float3 Position;
	float3 Rotation;
	SMQuaternion QRotation;
	float3 Scale, ScaleOld;

	void DrawMove();
	void DrawCylinder(float3_t lwh, DWORD color = 0xFFFFFFFF);
	void DrawRotate();
	void DrawScale();

	void IntersectMove(const float3 & start, const float3 & dir);
	void IntersectRotate(const float3 & start, const float3 & dir);
	void IntersectScale(const float3 & start, const float3 & dir);

	struct vert
	{
		float3_t pos;
		DWORD color;
	};

	enum HANDLER_AXE
	{
		HA_NONE = 0x00,
		HA_X = 0x01,
		HA_Y = 0x02,
		HA_XY = 0x03,
		HA_Z = 0x04,
		HA_XZ = 0x05,
		HA_YZ = 0x06,
		HA_XYZ = 0x07
	};

	HANDLER_AXE m_currentAxe;
	HANDLER_TYPE m_htype;
	
	float3 m_fStartDragPos;
	SMMATRIX m_mHelperMat;
	SMMATRIX m_mOldHelperMat;
	SMMATRIX m_mHelperMatScale2;
	float dist;
	SMMATRIX m_mOldDragMat;
	SMMATRIX m_mOldDragMatPos;
};

#endif