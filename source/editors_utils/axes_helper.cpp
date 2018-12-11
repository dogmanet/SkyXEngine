
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include <input/sxinput.h>
#include <physics/sxphysics.h>
#include "axes_helper.h"

CAxesHelper::CAxesHelper()
{
	m_bIsDragging = /*m_bIsDraggingStart = m_bIsDraggingStop = */false;
	m_vScale = float3(1, 1, 1);
	m_htype = HANDLER_TYPE_NONE;
}

CAxesHelper::~CAxesHelper()
{

}

void CAxesHelper::setType(CAxesHelper::HANDLER_TYPE type)
{
	m_htype = type;
}

CAxesHelper::HANDLER_TYPE CAxesHelper::getType()
{
	return m_htype;
}

void CAxesHelper::setPosition(const float3 & pos)
{
	m_vPosition = pos;
	//m_mHelperMat = SMMatrixScaling(m_vScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition);
}

void CAxesHelper::setRotation(const SMQuaternion & rot)
{
	m_qRotation = rot;
	//m_vRotation = QuatToEuler(rot);
	m_vRotation = SMMatrixToEuler(rot.GetMatrix());
	//m_mHelperMat = SMMatrixScaling(m_vScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition);
}

void CAxesHelper::setRotation(const float3 & rot)
{
	m_vRotation = rot;
	m_qRotation = SMQuaternion(-rot.x, 'x') * SMQuaternion(-rot.y, 'y') * SMQuaternion(-rot.z, 'z');
	//m_qRotation.w *= -1.0f;
	//m_qRotation = SMQuaternion(rot.z, 'z') * SMQuaternion(rot.y, 'y') * SMQuaternion(rot.x, 'x');
	//m_mHelperMat = SMMatrixScaling(m_vScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition);
}

//void CAxesHelper::setScale(const float3 & scale)
//{
//	m_vScale = scale;
//	//m_mHelperMat = SMMatrixScaling(m_vScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition);
//}


const float3 & CAxesHelper::getPosition()
{
	return m_vPosition;
}

const float3 & CAxesHelper::getRotation()
{
	return m_vRotation;
}

const float3 & CAxesHelper::getScale()
{
	return m_vScale;
}

const SMQuaternion & CAxesHelper::getRotationQ()
{
	return(m_qRotation);
}

void CAxesHelper::render()
{
	if (m_htype == HANDLER_TYPE_NONE)
		return;

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);
	SGCore_GetDXDevice()->SetTexture(0, 0);
	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	SGCore_ShaderUnBind();

	if(!m_bIsDragging)
	{
		float fDist = SMVector3Distance(m_vPosition, vCamPos) * 0.20f;
		fDist = (fDist > 0.25f ? fDist : 0.25f);
		m_mHelperMatScale2 = SMMatrixScaling(fDist, fDist, fDist);
	}

	if (m_htype == HANDLER_TYPE_MOVE)
		drawMove();
	else if (m_htype == HANDLER_TYPE_ROTATE)
		drawRotate();
	else if (m_htype == HANDLER_TYPE_SCALE)
		drawScale();
}

void CAxesHelper::drawMove()
{
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMatScale2 * SMMatrixTranslation(m_vPosition)));
	SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, 0);
	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	SGCore_GetDXDevice()->SetTexture(0, NULL);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

	SGCore_GetDXDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DWORD color_act = 0xFFFFFF00;
	DWORD color_act2 = 0x1FFFFF00;

	float len = AXES_HELPER_MOVE_LENGTH;
	vert l[] = {
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },

		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0xFFFF0000 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0x3FFF0000 },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0xFFFF0000 },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0x3FFF0000 },

		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0xFF00FF00 },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0x3F00FF00 },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0xFF00FF00 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0x3F00FF00 },

		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0xFF0000FF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0x3F0000FF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0xFF0000FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0x3F0000FF },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));

	float asize = 0.1f;
	float a2size = 0.3f;


	vert l2[] = {
		//arrow X
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },

		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },

		//arrow Y
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },

		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },

		//arrow Z
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },

		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },


		//plane XY
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },

		//plane XZ
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF00FF },

		//plane YZ
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FFFF },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, sizeof(l2) / sizeof(vert) / 3, l2, sizeof(vert));

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
}

void CAxesHelper::drawCylinder(float3_t lwh, DWORD color)
{
	const int segs = 16;

	lwh = (float3)(lwh * 0.5f);
	Array<vert> l;
	l.reserve(segs * 6);
	float deg1, deg2;
	for (int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;

		l.push_back({ lwh * float3_t(sinf(deg1), 1.0f, cosf(deg1)), color });
		l.push_back({ lwh * float3_t(sinf(deg2), 1.0f, cosf(deg2)), color });

		l.push_back({ lwh * float3_t(sinf(deg1), 1.0f, cosf(deg1)), color });
		l.push_back({ lwh * float3_t(sinf(deg1), -1.0f, cosf(deg1)), color });

		l.push_back({ lwh * float3_t(sinf(deg1), -1.0f, cosf(deg1)), color });
		l.push_back({ lwh * float3_t(sinf(deg2), -1.0f, cosf(deg2)), color });
	}
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, l.size() / 2, &l[0], sizeof(vert));
}

void CAxesHelper::drawRotate()
{
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMatScale2 * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition)));
	DWORD color_act = 0xFFFFFF00;

	SMMATRIX mOld;
	SGCore_GetDXDevice()->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);

	drawCylinder(float3_t(AXES_HELPER_ROTATE_LENGTH_WIDTH, AXES_HELPER_ROTATE_HEIGHT, AXES_HELPER_ROTATE_LENGTH_WIDTH), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00);
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixRotationZ(SM_PIDIV2) * mOld));
	drawCylinder(float3_t(AXES_HELPER_ROTATE_LENGTH_WIDTH, AXES_HELPER_ROTATE_HEIGHT, AXES_HELPER_ROTATE_LENGTH_WIDTH), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000);
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixRotationX(SM_PIDIV2) * mOld));
	drawCylinder(float3_t(AXES_HELPER_ROTATE_LENGTH_WIDTH, AXES_HELPER_ROTATE_HEIGHT, AXES_HELPER_ROTATE_LENGTH_WIDTH), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF);


	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);
}

void CAxesHelper::drawScale()
{
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMatScale2 * SMMatrixScaling(m_vScale) * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition)));
	SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, 0);
	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	SGCore_GetDXDevice()->SetTexture(0, NULL);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);

	SGCore_GetDXDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DWORD color_act = 0xFFFFFF00;
	DWORD color_act2 = 0x1FFFFF00;

	float len = AXES_HELPER_SCALE_LENGTH;
	float len05 = len * 0.5f;
	float len075 = len * 0.75f;
	vert l[] = {
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },

		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0xFFFF0000 },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0xFF00FF00 },
		{ float3_t(len075, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0xFFFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act : 0xFF00FF00 },

		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0xFF0000FF },
		{ float3_t(len075, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act : 0xFF0000FF },

		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0xFF0000FF },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0xFF00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0xFF0000FF },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act : 0xFF00FF00 },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));

	float asize = AXES_HELPER_SCALE_ASIZE;
	float a2size = AXES_HELPER_SCALE_A2SIZE;


	vert l2[] = {
		//arrow X
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },

		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HANDLER_AXE_X) ? color_act : 0xFFFF0000 },

		//arrow Y
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },

		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HANDLER_AXE_Y) ? color_act : 0xFF00FF00 },

		//arrow Z
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },

		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HANDLER_AXE_Z) ? color_act : 0xFF0000FF },


		//plane XY
		{ float3_t(len075, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(len075, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY ? color_act2 : 0x1F00FF00 },

		//plane XZ
		{ float3_t(len075, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len075, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ ? color_act2 : 0x1F0000FF },

		//plane YZ
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len075, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ ? color_act2 : 0x1F0000FF },

		//plane XYZ
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len05, 0), (m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len05), (m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ ? color_act2 : 0x1F0000FF },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, sizeof(l2) / sizeof(vert) / 3, l2, sizeof(vert));

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
}

void CAxesHelper::update()
{
	if(m_htype == HANDLER_TYPE_NONE)
		return;

	bool isDraggingStart = false;
	bool isMouseDown = SSInput_GetKeyState(SIM_LBUTTON);
	if(!m_bIsDragging && isMouseDown)
	{ // start dragging
		m_bIsDragging = true;
		if(m_currentAxe == HANDLER_AXE_NONE)
		{
			return;
		}
		m_vScaleOld = m_vScale;
		// build plane
		float3 vNormal;
		if((m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY || m_currentAxe == HANDLER_AXE_Z)
		{
			vNormal = float3(0.0f, 0.0f, 1.0f);
		}
		else if((m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ || m_currentAxe == HANDLER_AXE_X)
		{
			vNormal = float3(1.0f, 0.0f, 0.0f);
		}
		else if((m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ || m_currentAxe == HANDLER_AXE_Y)
		{
			vNormal = float3(0.0f, 1.0f, 0.0f);
		}

		m_movementPlane = SMPLANE(vNormal, m_vPosition);
		if(m_htype == HANDLER_TYPE_MOVE)
		{
			m_vMovementLineDir = vNormal;
			m_vMovementLinePos = m_vPosition;
		}
		else if(m_htype == HANDLER_TYPE_SCALE)
		{
			m_vMovementLinePos = m_vPosition;

			if((m_currentAxe & HANDLER_AXE_XY) == HANDLER_AXE_XY)
			{
				vNormal = m_qRotation * float3(1.0f, 1.0f, 0.0f);
			}
			else if((m_currentAxe & HANDLER_AXE_YZ) == HANDLER_AXE_YZ)
			{
				vNormal = m_qRotation * float3(0.0f, 1.0f, 1.0f);
			}
			else if((m_currentAxe & HANDLER_AXE_XZ) == HANDLER_AXE_XZ)
			{
				vNormal = m_qRotation * float3(1.0f, 0.0f, 1.0f);
			}
			else if((m_currentAxe & HANDLER_AXE_XYZ) == HANDLER_AXE_XYZ)
			{
				vNormal = float3(0.0f, 1.0f, 0.0f);
			}
			m_vMovementLineDir = vNormal;
		}
		isDraggingStart = true;
		//return;
	}
	else if(m_bIsDragging && !isMouseDown)
	{ // mouse released
		m_bIsDragging = false;
		if(m_htype == HANDLER_TYPE_SCALE)
		{
			m_vScale = float3(1, 1, 1);
		}
		return;
	}


	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(SGCore_GetHWND(), &pt);
	int x = pt.x;
	int y = pt.y;

	static float4x4 mCamView, mCamProj;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mCamView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mCamProj);

	float det = 0;
	SMMATRIX mat = SMMatrixInverse(&det, m_mHelperMatScale2 * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition) * mCamView * mCamProj);
	SMMATRIX mat2 = SMMatrixInverse(&det, m_mHelperMatScale2 * SMMatrixTranslation(m_vPosition) * mCamView * mCamProj);
	SMMATRIX matw = SMMatrixInverse(&det, mCamView * mCamProj);
	D3DVIEWPORT9 vp;
	SGCore_GetDXDevice()->GetViewport(&vp);

	float px = (((2.0f*x) / vp.Width) - 1.0f);
	float py = (((-2.0f*y) / vp.Height) + 1.0f);

	// В пространстве геометрии хелпера
	float3 pos(px, py, -1);
	float3 dir(px, py, 1);
	pos = SMVector3Transform(pos, mat);
	dir = SMVector3Transform(dir, mat);
	pos /= pos.w;
	dir /= dir.w;

	// В пространстве геометрии хелпера без учета масштабирования и поворота (для хелпера перемещения)
	float3 pos2(px, py, -1);
	float3 dir2(px, py, 1);
	pos2 = SMVector3Transform(pos2, mat2);
	dir2 = SMVector3Transform(dir2, mat2);
	pos2 /= pos2.w;
	dir2 /= dir2.w;

	// В пространстве мира
	float3 posw(px, py, -1);
	float3 dirw(px, py, 1);
	posw = SMVector3Transform(posw, matw);
	dirw = SMVector3Transform(dirw, matw);
	posw /= posw.w;
	dirw /= dirw.w;

	switch(m_htype)
	{
	case HANDLER_TYPE_MOVE:
		intersectMove(pos2, dir2);
		break;
	case HANDLER_TYPE_ROTATE:
		intersectRotate(pos, dir);
		break;
	case HANDLER_TYPE_SCALE:
		intersectScale(pos, dir);
		break;
	}

	if(m_currentAxe == HANDLER_AXE_NONE)
	{
		return;
	}

	if(isDraggingStart)
	{
		isDraggingStart = false;
		if(m_currentAxe == HANDLER_AXE_X || m_currentAxe == HANDLER_AXE_Y || m_currentAxe == HANDLER_AXE_Z || (m_htype == HANDLER_TYPE_SCALE))
		{
			float3 vLinePos;
			SMCrossLines(m_vMovementLinePos, m_vMovementLineDir, posw, dirw, &vLinePos);
			if(m_htype == HANDLER_TYPE_MOVE)
			{
				m_vTransOffset = m_vPosition - vLinePos;
			}
			else
			{
				m_vTransOffset = vLinePos;
			}
		}
		else
		{
			float3 vPlanePos;
			if(!m_movementPlane.intersectLine(&vPlanePos, posw, posw + dirw * 10000.0f))
			{
				m_bIsDragging = false;
				return;
			}
			m_vTransOffset = m_vPosition - vPlanePos;
			//printf("%.2f %.2f %.2f\n", m_vTransOffset.x, m_vTransOffset.y, m_vTransOffset.z);
		}
		return;
	}

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	//SPhysics_GetDynWorld()->getDebugDrawer()->drawLine(F3_BTVEC(posw), F3_BTVEC(posw + dirw * 10000.0f), btVector3(1.0f, 1.0f, 1.0f));

	if(m_bIsDragging)
	{
		//float3 dv = (pos - m_fStartDragPos) * (1000.f + SMVector3Length(vCamPos - float3(m_mHelperMat._41, m_mHelperMat._42, m_mHelperMat._43)) * AXES_HELPER_MOVE_SPEED);
		//float4x4 m_res;
		

		if(m_htype == HANDLER_TYPE_MOVE)
		{
			if(m_currentAxe == HANDLER_AXE_X || m_currentAxe == HANDLER_AXE_Y || m_currentAxe == HANDLER_AXE_Z)
			{
				float3 vLinePos;
				SMCrossLines(m_vMovementLinePos, m_vMovementLineDir, posw, dirw, &vLinePos);
				m_vPosition = m_vTransOffset + vLinePos;
			}
			else
			{
				float3 vPlanePos;
				if(!m_movementPlane.intersectLine(&vPlanePos, vCamPos, vCamPos + dirw * 10000.0f))
				{
					return;
				}
				//SPhysics_GetDynWorld()->getDebugDrawer()->drawSphere(F3_BTVEC(vPlanePos), 0.05f, btVector3(1.0f, 1.0f, 1.0f));
				
				m_vPosition = m_vTransOffset + vPlanePos;
				//SPhysics_GetDynWorld()->getDebugDrawer()->drawLine(F3_BTVEC(vPlanePos), F3_BTVEC(m_vPosition), btVector3(1.0f, 1.0f, 1.0f));
			}
		}
		else if(m_htype == HANDLER_TYPE_ROTATE)
		{
			float3 vLinePos;
			SMCrossLines(m_vMovementLinePos, m_vMovementLineDir, posw, dirw, &vLinePos);
			float3 vDelta = m_vTransOffset - vLinePos;
			float fDelta = SMVector3Length(vDelta) * -sign(SMVector3Dot(vDelta, m_vMovementLineDir));
			m_vTransOffset = vLinePos;
			float3 vAxis;
			switch(m_currentAxe)
			{
			case HANDLER_AXE_X:
				vAxis = float3(1.0f, 0.0f, 0.0f);
				break;
			case HANDLER_AXE_Y:
				vAxis = float3(0.0f, 1.0f, 0.0f);
				break;
			case HANDLER_AXE_Z:
				vAxis = float3(0.0f, 0.0f, 1.0f);
				break;
			}

			m_qRotation = m_qRotation * SMQuaternion(m_qRotation * vAxis, fDelta);
			//m_vRotation = QuatToEuler(m_qRotation);
			m_vRotation = SMMatrixToEuler(m_qRotation.GetMatrix());
			m_vRotation.x *= -1.0f;
			m_vRotation.z *= -1.0f;
		}
		else if(m_htype == HANDLER_TYPE_SCALE)
		{
			float3 vLinePos;
			SMCrossLines(m_vMovementLinePos, m_vMovementLineDir, posw, dirw, &vLinePos);

			float3 vDelta = m_vTransOffset - vLinePos;
			float fDelta = SMVector3Length(vDelta) + 1.0f;
			if(sign(SMVector3Dot(vDelta, m_vMovementLineDir)) > 0)
			{
				fDelta = 1.0f / fDelta;
			}
				
			switch(m_currentAxe)
			{
			case HANDLER_AXE_X:
				m_vScale.x = fDelta;
				break;
			case HANDLER_AXE_Y:
				m_vScale.y = fDelta;
				break;
			case HANDLER_AXE_Z:
				m_vScale.z = fDelta;
				break;
			case HANDLER_AXE_XY:
				m_vScale.x = fDelta;
				m_vScale.y = fDelta;
				break;
			case HANDLER_AXE_XZ:
				m_vScale.x = fDelta;
				m_vScale.z = fDelta;
				break; 
			case HANDLER_AXE_YZ:
				m_vScale.y = fDelta;
				m_vScale.z = fDelta;
				break; 
			case HANDLER_AXE_XYZ:
				m_vScale.x = fDelta;
				m_vScale.y = fDelta;
				m_vScale.z = fDelta;
				break;
			}
		}
	}
}

void CAxesHelper::intersectMove(const float3 & start, const float3 & dir)
{
	if (m_bIsDragging)
		return;

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	float3 end = dir;
	float len = AXES_HELPER_ROTATE_LENGTH_WIDTH;
	float asize = 0.1f;
	float a2size = 0.3f;

	m_currentAxe = HANDLER_AXE_NONE;

	float3 p;

	float mind = FLOAT_INF;

	if (line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, 0), float3_t(len * 0.5f, len * 0.5f, 0), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, len * 0.5f, 0), float3_t(0, len * 0.5f, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_XY;
		}
	}

	if (line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, 0), float3_t(len * 0.5f, 0, len * 0.5f), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, len * 0.5f), float3_t(0, 0, len * 0.5f), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_XZ;
		}
	}

	if (line_intersect_triangle(float3(0, 0, 0), float3(0, 0, len * 0.5f), float3_t(0, len * 0.5f, len * 0.5f), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(0, len * 0.5f, len * 0.5f), float3_t(0, len * 0.5f, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_YZ;
		}
	}


	float3 asX[] = {
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
	};

	float3 asY[] = {
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
	};

	float3 asZ[] = {
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
	};

	for (int i = 0, l = sizeof(asX) / sizeof(asX[0]); i < l; i += 3)
	{
		if (line_intersect_triangle(asX[i], asX[i + 1], asX[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				m_currentAxe = HANDLER_AXE_X;
				break;
			}
		}
	}
	for (int i = 0, l = sizeof(asY) / sizeof(asY[0]); i < l; i += 3)
	{
		if (line_intersect_triangle(asY[i], asY[i + 1], asY[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				m_currentAxe = HANDLER_AXE_Y;
				break;
			}
		}
	}
	for (int i = 0, l = sizeof(asZ) / sizeof(asZ[0]); i < l; i += 3)
	{
		if (line_intersect_triangle(asZ[i], asZ[i + 1], asZ[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				m_currentAxe = HANDLER_AXE_Z;
				break;
			}
		}
	}
}

void CAxesHelper::intersectRotate(const float3 & start, const float3 & dir)
{
	if (m_bIsDragging)
	{
		return;
	}

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	float3 end = dir;
	float rad = AXES_HELPER_ROTATE_LENGTH_WIDTH;
	float h = AXES_HELPER_ROTATE_HEIGHT;


	m_currentAxe = HANDLER_AXE_NONE;

	float3 p;

	float mind = FLOAT_INF;

	const int segs = 16;

	float3_t lwh(rad, h, rad);

	lwh = (float3)(lwh * 0.5f);
	float deg1, deg2;

	/*
	1--2
	\
	\
	3--4
	*/

	float3 vIntersectPoint;

	//Y
	for (int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;


		float3_t p1(lwh * float3_t(sinf(deg1), 1.0f, cosf(deg1)));
		float3_t p2(lwh * float3_t(sinf(deg2), 1.0f, cosf(deg2)));

		float3_t p3(lwh * float3_t(sinf(deg1), -1.0f, cosf(deg1)));
		float3_t p4(lwh * float3_t(sinf(deg2), -1.0f, cosf(deg2)));
		if (line_intersect_triangle(p1, p2, p4, start, end, p)
			|| line_intersect_triangle(p1, p4, p3, start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				vIntersectPoint = p;
				m_currentAxe = HANDLER_AXE_Y;
				break;
			}
		}
	}
	lwh = float3_t(lwh.y, lwh.x, lwh.z);
	//X
	for (int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;


		float3_t p1(lwh * float3_t(1.0f, sinf(deg1), cosf(deg1)));
		float3_t p2(lwh * float3_t(1.0f, sinf(deg2), cosf(deg2)));

		float3_t p3(lwh * float3_t(-1.0f, sinf(deg1), cosf(deg1)));
		float3_t p4(lwh * float3_t(-1.0f, sinf(deg2), cosf(deg2)));
		if (line_intersect_triangle(p1, p2, p4, start, end, p)
			|| line_intersect_triangle(p1, p4, p3, start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				vIntersectPoint = p;
				m_currentAxe = HANDLER_AXE_X;
				break;
			}
		}
	}
	lwh = float3_t(lwh.y, lwh.z, lwh.x);
	//Z
	for (int i = 0; i < segs; ++i)
	{
		deg1 = ((float)i / (float)segs) * SM_2PI;
		deg2 = ((float)((i + 1) % segs) / (float)segs) * SM_2PI;


		float3_t p1(lwh * float3_t(sinf(deg1), cosf(deg1), 1.0f));
		float3_t p2(lwh * float3_t(sinf(deg2), cosf(deg2), 1.0f));

		float3_t p3(lwh * float3_t(sinf(deg1), cosf(deg1), -1.0f));
		float3_t p4(lwh * float3_t(sinf(deg2), cosf(deg2), -1.0f));
		if (line_intersect_triangle(p1, p2, p4, start, end, p)
			|| line_intersect_triangle(p1, p4, p3, start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				vIntersectPoint = p;
				m_currentAxe = HANDLER_AXE_Z;
				break;
			}
		}
	}

	float3 vAxis;
	switch(m_currentAxe)
	{
	case HANDLER_AXE_X:
		vAxis = float3(1.0f, 0.0f, 0.0f);
		break;
	case HANDLER_AXE_Y:
		vAxis = float3(0.0f, 1.0f, 0.0f);
		break;
	case HANDLER_AXE_Z:
		vAxis = float3(0.0f, 0.0f, 1.0f);
		break;
	default:
		return;
	}

	m_vMovementLinePos = m_vPosition;
	m_vMovementLineDir = SMVector3Cross(vAxis, m_vPosition - SMVector3Transform(p, m_mHelperMatScale2 * m_qRotation.GetMatrix() * SMMatrixTranslation(m_vPosition)));
}

void CAxesHelper::intersectScale(const float3 & start, const float3 & dir)
{
	if (m_bIsDragging)
		return;

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	float3 end = dir;
	float len = AXES_HELPER_SCALE_LENGTH;
	float len05 = len * 0.5f;
	float len075 = len * 0.75f;
	float asize = AXES_HELPER_SCALE_ASIZE;
	float a2size = AXES_HELPER_SCALE_A2SIZE;

	m_currentAxe = HANDLER_AXE_NONE;

	float3 p;

	float mind = FLOAT_INF;

	if (line_intersect_triangle(float3(len075, 0, 0), float3(0, len075, 0), float3_t(len05, 0, 0), start, end, p)
		|| line_intersect_triangle(float3(len05, 0, 0), float3(0, len075, 0), float3_t(0, len05, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_XY;
		}
	}

	if (line_intersect_triangle(float3(len075, 0, 0), float3(0, 0, len075), float3_t(len05, 0, 0), start, end, p)
		|| line_intersect_triangle(float3(len05, 0, 0), float3(0, 0, len075), float3_t(0, 0, len05), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_XZ;
		}
	}

	if (line_intersect_triangle(float3(0, len075, 0), float3(0, 0, len075), float3_t(0, len05, 0), start, end, p)
		|| line_intersect_triangle(float3(0, len05, 0), float3(0, 0, len075), float3_t(0, 0, len05), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_YZ;
		}
	}

	if (line_intersect_triangle(float3(0, len05, 0), float3(0, 0, len05), float3_t(len05, 0, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HANDLER_AXE_XYZ;
		}
	}


	float3 asX[] = {
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, a2size * 0.5f),
		float3(len + asize * 2.0f, a2size * 0.5f, -a2size * 0.5f),

		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, -a2size * 0.5f),
		float3(len + asize * 2.0f, -a2size * 0.5f, a2size * 0.5f),
	};

	float3 asY[] = {
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
		float3(a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),

		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, -a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, a2size * 0.5f),
		float3(-a2size * 0.5f, len + asize * 2.0f, -a2size * 0.5f),
	};

	float3 asZ[] = {
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),

		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),

		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, a2size * 0.5f),
		float3(-a2size * 0.5f, -a2size * 0.5f, len + asize * 2.0f),
		float3(-a2size * 0.5f, a2size * 0.5f, len + asize * 2.0f),
	};

	for (int i = 0, l = sizeof(asX) / sizeof(asX[0]); i < l; i += 3)
	{
		if (line_intersect_triangle(asX[i], asX[i + 1], asX[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				m_currentAxe = HANDLER_AXE_X;
				break;
			}
		}
	}
	for (int i = 0, l = sizeof(asY) / sizeof(asY[0]); i < l; i += 3)
	{
		if (line_intersect_triangle(asY[i], asY[i + 1], asY[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				m_currentAxe = HANDLER_AXE_Y;
				break;
			}
		}
	}
	for (int i = 0, l = sizeof(asZ) / sizeof(asZ[0]); i < l; i += 3)
	{
		if (line_intersect_triangle(asZ[i], asZ[i + 1], asZ[i + 2], start, end, p))
		{
			float d = SMVector3Length(p - vCamPos);
			if (d < mind)
			{
				mind = d;
				m_currentAxe = HANDLER_AXE_Z;
				break;
			}
		}
	}
}