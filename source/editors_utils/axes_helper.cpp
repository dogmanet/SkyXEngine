
#include "axes_helper.h"

AxesHelper::AxesHelper()
{
	m_bIsDragging = m_bIsDraggingStart = m_bIsDraggingStop = false;
	Scale = float3(1, 1, 1);
	m_htype = HT_NONE;
}

AxesHelper::~AxesHelper()
{

}

void AxesHelper::SetType(AxesHelper::HANDLER_TYPE type)
{
	m_htype = type;
}

AxesHelper::HANDLER_TYPE AxesHelper::GetType()
{
	return m_htype;
}

void AxesHelper::SetPosition(const float3 & pos)
{
	Position = pos;
	m_mHelperMat = SMMatrixScaling(Scale) * QRotation.GetMatrix() * SMMatrixTranslation(Position);
}

void AxesHelper::SetRotation(const SMQuaternion & rot)
{
	QRotation = rot;
	Rotation = SMMatrixToEuler(rot.GetMatrix());
	m_mHelperMat = SMMatrixScaling(Scale) * QRotation.GetMatrix() * SMMatrixTranslation(Position);
}

void AxesHelper::SetRotation(const float3 & rot)
{
	Rotation = rot;
	QRotation = SMQuaternion(rot.x, 'x') * SMQuaternion(rot.y, 'y') * SMQuaternion(rot.z, 'z');
	m_mHelperMat = SMMatrixScaling(Scale) * QRotation.GetMatrix() * SMMatrixTranslation(Position);
}

void AxesHelper::SetScale(const float3 & scale)
{
	Scale = scale;
	m_mHelperMat = SMMatrixScaling(Scale) * QRotation.GetMatrix() * SMMatrixTranslation(Position);
}


const float3 & AxesHelper::GetPosition()
{
	return Position;
}

const float3 & AxesHelper::GetRotation()
{
	return Rotation;
}

const float3 & AxesHelper::getScale()
{
	return Scale;
}

const SMQuaternion & AxesHelper::GetRotationQ()
{
	return(QRotation);
}

void AxesHelper::Render()
{
	if (m_htype == HT_NONE)
		return;

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);
	SGCore_GetDXDevice()->SetTexture(0, 0);
	SGCore_ShaderUnBind();

	if(!m_bIsDragging)
	{
		dist = SMVector3Distance(Position, vCamPos) * 0.20f;
		dist = (dist > 0.25f ? dist : 0.25f);
		m_mHelperMatScale2 = SMMatrixScaling(dist, dist, dist);
	}

	if (m_htype == HT_MOVE)
		DrawMove();
	else if (m_htype == HT_ROTATE)
		DrawRotate();
	else if (m_htype == HT_SCALE)
		DrawScale();
}

void AxesHelper::DrawMove()
{
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMatScale2 * SMMatrixTranslation(Position)));
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
		{ float3_t(0, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },

		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFFFF0000 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0x3FFF0000 },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFFFF0000 },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0x3FFF0000 },

		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF00FF00 },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0x3F00FF00 },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFF00FF00 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0x3F00FF00 },

		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF0000FF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0x3F0000FF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFF0000FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0x3F0000FF },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));

	float asize = 0.1f;
	float a2size = 0.3f;


	vert l2[] = {
		//arrow X
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },

		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },

		//arrow Y
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },

		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },

		//arrow Z
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },

		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },


		//plane XY
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len * 0.5f, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFFFF00 },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },

		//plane XZ
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(len * 0.5f, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len * 0.5f, 0, len * 0.5f), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF00FF },

		//plane YZ
		{ float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, 0, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
		{ float3_t(0, len * 0.5f, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len * 0.5f, len * 0.5f), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FFFF },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, sizeof(l2) / sizeof(vert) / 3, l2, sizeof(vert));

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
}

void AxesHelper::DrawCylinder(float3_t lwh, DWORD color)
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

void AxesHelper::DrawRotate()
{
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMatScale2 * m_mHelperMat));
	DWORD color_act = 0xFFFFFF00;

	SMMATRIX mOld;
	SGCore_GetDXDevice()->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);

	DrawCylinder(float3_t(AXES_HELPER_ROTATE_LENGTH_WIDTH, AXES_HELPER_ROTATE_HEIGHT, AXES_HELPER_ROTATE_LENGTH_WIDTH), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00);
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixRotationZ(SM_PIDIV2) * mOld));
	DrawCylinder(float3_t(AXES_HELPER_ROTATE_LENGTH_WIDTH, AXES_HELPER_ROTATE_HEIGHT, AXES_HELPER_ROTATE_LENGTH_WIDTH), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000);
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(SMMatrixRotationX(SM_PIDIV2) * mOld));
	DrawCylinder(float3_t(AXES_HELPER_ROTATE_LENGTH_WIDTH, AXES_HELPER_ROTATE_HEIGHT, AXES_HELPER_ROTATE_LENGTH_WIDTH), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF);


	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&mOld);
}

void AxesHelper::DrawScale()
{
	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&(m_mHelperMatScale2 * m_mHelperMat));
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
		{ float3_t(0, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, 0, 0), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },

		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFFFF0000 },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFF00FF00 },
		{ float3_t(len075, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFFFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act : 0xFF00FF00 },

		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFF0000FF },
		{ float3_t(len075, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFFFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act : 0xFF0000FF },

		{ float3_t(0, 0, len05), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF0000FF },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF0000FF },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act : 0xFF00FF00 },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, sizeof(l) / sizeof(vert) / 2, l, sizeof(vert));

	float asize = AXES_HELPER_SCALE_ASIZE;
	float a2size = AXES_HELPER_SCALE_A2SIZE;


	vert l2[] = {
		//arrow X
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },

		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * 0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * -0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len, asize * 0.5f, asize * -0.5f), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },
		{ float3_t(len + asize * 2.0f, 0, 0), (m_currentAxe & HA_X) ? color_act : 0xFFFF0000 },

		//arrow Y
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },

		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * 0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * 0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(asize * -0.5f, len, asize * -0.5f), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },
		{ float3_t(0, len + asize * 2.0f, 0), (m_currentAxe & HA_Y) ? color_act : 0xFF00FF00 },

		//arrow Z
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },

		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * 0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * 0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(asize * -0.5f, asize * -0.5f, len), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },
		{ float3_t(0, 0, len + asize * 2.0f), (m_currentAxe & HA_Z) ? color_act : 0xFF0000FF },


		//plane XY
		{ float3_t(len075, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(len075, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_XY) == HA_XY ? color_act2 : 0x1F00FF00 },

		//plane XZ
		{ float3_t(len075, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len075, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_XZ) == HA_XZ ? color_act2 : 0x1F0000FF },

		//plane YZ
		{ float3_t(0, len075, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len075, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len075), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_YZ) == HA_YZ ? color_act2 : 0x1F0000FF },

		//plane XYZ
		{ float3_t(0, len05, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F0000FF },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, len05, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F00FF00 },
		{ float3_t(len05, 0, 0), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1FFF0000 },
		{ float3_t(0, 0, len05), (m_currentAxe & HA_XYZ) == HA_XYZ ? color_act2 : 0x1F0000FF },
	};
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, sizeof(l2) / sizeof(vert) / 3, l2, sizeof(vert));

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
}

void AxesHelper::OnMouseMove(int x, int y)
{
	if (m_htype == HT_NONE)
		return;

	static float4x4 mCamView, mCamProj;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mCamView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mCamProj);

	float det = 0;
	SMMATRIX mat = SMMatrixInverse(&det, m_mHelperMatScale2 * (m_bIsDragging && !m_bIsDraggingStart ? m_mOldDragMat : m_mHelperMat) * mCamView * mCamProj);
	SMMATRIX mat2 = SMMatrixInverse(&det, m_mHelperMatScale2 * (m_bIsDragging && !m_bIsDraggingStart ? m_mOldDragMatPos : SMMatrixTranslation(Position)) * mCamView * mCamProj);
	D3DVIEWPORT9 vp;
	SGCore_GetDXDevice()->GetViewport(&vp);

	float px = (((2.0f*x) / vp.Width) - 1.0f);
	float py = (((-2.0f*y) / vp.Height) + 1.0f);

	float3 pos(px, py, -1);
	float3 dir(px, py, 1);
	pos = SMVector3Transform(pos, mat);
	dir = SMVector3Transform(dir, mat);
	pos /= pos.w;
	dir /= dir.w;

	float3 pos2(px, py, -1);
	float3 dir2(px, py, 1);
	pos2 = SMVector3Transform(pos2, mat2);
	dir2 = SMVector3Transform(dir2, mat2);
	pos2 /= pos2.w;
	dir2 /= dir2.w;

	if (m_htype == HT_MOVE)
		IntersectMove(pos2, dir2);
	else if (m_htype == HT_ROTATE)
		IntersectRotate(pos, dir);
	else if (m_htype == HT_SCALE)
		IntersectScale(pos, dir);

	if (m_bIsDraggingStart)
	{
		m_bIsDraggingStart = false;
		m_fStartDragPos = pos;
		m_mOldDragMat = m_mHelperMat;
		m_mOldDragMatPos = SMMatrixTranslation(Position);
		m_mOldHelperMat = m_mHelperMat;
		ScaleOld = Scale;
	}

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	if (m_bIsDragging)
	{
		float3 dv = (pos - m_fStartDragPos) * (1000.f + SMVector3Length(vCamPos - float3(m_mHelperMat._41, m_mHelperMat._42, m_mHelperMat._43)) * AXES_HELPER_MOVE_SPEED);
		float4x4 m_res;

		if (m_htype == HT_MOVE)
		{
			m_res = SMMatrixTranslation((m_currentAxe & HA_X) ? dv.x : 0, (m_currentAxe & HA_Y) ? dv.y : 0, (m_currentAxe & HA_Z) ? dv.z : 0);
			m_mHelperMat = m_mOldHelperMat * m_res;
			Position = m_mHelperMat.r[3];
		}
		else if (m_htype == HT_ROTATE)
		{
			m_res = SMMatrixRotationX((m_currentAxe & HA_X) ? dv.x : 0) * SMMatrixRotationY((m_currentAxe & HA_Y) ? dv.y : 0) * SMMatrixRotationZ((m_currentAxe & HA_Z) ? dv.z : 0);
			m_mHelperMat = m_res * m_mOldHelperMat;
			QRotation = SMQuaternion(m_mHelperMat);
			Rotation = SMMatrixToEuler(QRotation.GetMatrix());
		}
		else if (m_htype == HT_SCALE)
		{
			//dv = (pos - m_fStartDragPos) * (1000.0f + SMVector3Length(GData::ConstCurrCamPos - float3(m_mHelperMat._41, m_mHelperMat._42, m_mHelperMat._43)) * AXES_HELPER_SCALE_SPEED);
			//dv /= 10.0f;
			dv += float3(1.0f, 1.0f, 1.0f);
			if ((m_currentAxe & HA_XY) == HA_XY)
				dv.x = dv.z = dv.y;
			else if ((m_currentAxe & HA_YZ) == HA_YZ)
				dv.y = dv.z;
			else if ((m_currentAxe & HA_XZ) == HA_XZ)
				dv.x = dv.z;

			m_mHelperMat = SMMatrixScaling((m_currentAxe & HA_X) ? dv.x : 1, (m_currentAxe & HA_Y) ? dv.y : 1, (m_currentAxe & HA_Z) ? dv.z : 1) * QRotation.GetMatrix() * SMMatrixTranslation(Position);
			Scale = (SMVECTOR)ScaleOld * float3(SMVector3Length(float3(m_mHelperMat._11, m_mHelperMat._12, m_mHelperMat._13)),
				SMVector3Length(float3(m_mHelperMat._21, m_mHelperMat._22, m_mHelperMat._23)),
				SMVector3Length(float3(m_mHelperMat._31, m_mHelperMat._32, m_mHelperMat._33)));
		}
	}
	if (m_bIsDraggingStop)
	{
		m_bIsDraggingStop = false;
		if (m_htype == HT_SCALE)
		{
			Scale = float3(1, 1, 1);
			m_mHelperMat = SMMatrixScaling(Scale) * QRotation.GetMatrix() * SMMatrixTranslation(Position);
		}
	}
}

void AxesHelper::IntersectMove(const float3 & start, const float3 & dir)
{
	if (m_bIsDragging)
		return;

	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	float3 end = dir;
	float len = AXES_HELPER_ROTATE_LENGTH_WIDTH;
	float asize = 0.1f;
	float a2size = 0.3f;

	m_currentAxe = HA_NONE;

	float3 p;

	float mind = FLOAT_INF;

	if (line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, 0), float3_t(len * 0.5f, len * 0.5f, 0), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, len * 0.5f, 0), float3_t(0, len * 0.5f, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_XY;
		}
	}

	if (line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, 0), float3_t(len * 0.5f, 0, len * 0.5f), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(len * 0.5f, 0, len * 0.5f), float3_t(0, 0, len * 0.5f), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_XZ;
		}
	}

	if (line_intersect_triangle(float3(0, 0, 0), float3(0, 0, len * 0.5f), float3_t(0, len * 0.5f, len * 0.5f), start, end, p)
		|| line_intersect_triangle(float3(0, 0, 0), float3(0, len * 0.5f, len * 0.5f), float3_t(0, len * 0.5f, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_YZ;
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
				m_currentAxe = HA_X;
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
				m_currentAxe = HA_Y;
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
				m_currentAxe = HA_Z;
				break;
			}
		}
	}
}

void AxesHelper::IntersectRotate(const float3 & start, const float3 & dir)
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


	m_currentAxe = HA_NONE;

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
				m_currentAxe = HA_Y;
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
				m_currentAxe = HA_X;
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
				m_currentAxe = HA_Z;
				break;
			}
		}
	}
}

void AxesHelper::IntersectScale(const float3 & start, const float3 & dir)
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

	m_currentAxe = HA_NONE;

	float3 p;

	float mind = FLOAT_INF;

	if (line_intersect_triangle(float3(len075, 0, 0), float3(0, len075, 0), float3_t(len05, 0, 0), start, end, p)
		|| line_intersect_triangle(float3(len05, 0, 0), float3(0, len075, 0), float3_t(0, len05, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_XY;
		}
	}

	if (line_intersect_triangle(float3(len075, 0, 0), float3(0, 0, len075), float3_t(len05, 0, 0), start, end, p)
		|| line_intersect_triangle(float3(len05, 0, 0), float3(0, 0, len075), float3_t(0, 0, len05), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_XZ;
		}
	}

	if (line_intersect_triangle(float3(0, len075, 0), float3(0, 0, len075), float3_t(0, len05, 0), start, end, p)
		|| line_intersect_triangle(float3(0, len05, 0), float3(0, 0, len075), float3_t(0, 0, len05), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_YZ;
		}
	}

	if (line_intersect_triangle(float3(0, len05, 0), float3(0, 0, len05), float3_t(len05, 0, 0), start, end, p))
	{
		float d = SMVector3Length(p - vCamPos);
		if (d < mind)
		{
			mind = d;
			m_currentAxe = HA_XYZ;
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
				m_currentAxe = HA_X;
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
				m_currentAxe = HA_Y;
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
				m_currentAxe = HA_Z;
				break;
			}
		}
	}
}