#include "GizmoHandle.h"
#include "Editor.h"
#include "terrax.h"

CGizmoHandle::CGizmoHandle(CEditor *pEditor):
	m_pEditor(pEditor)
{
	m_pEditor->setDirty();
}
CGizmoHandle::~CGizmoHandle()
{
	m_pEditor->onGizmoRemoved(this);

	m_pEditor->setDirty();
}

void XMETHODCALLTYPE CGizmoHandle::setPos(const float3_t &vPos)
{
	m_vPos = vPos;

	m_pEditor->setDirty();
}
const float3_t& XMETHODCALLTYPE CGizmoHandle::getPos()
{
	return(m_vPos);
}

void XMETHODCALLTYPE CGizmoHandle::lockInPlane(const float3_t &vPlaneNormal)
{
}
void XMETHODCALLTYPE CGizmoHandle::lockInDir(const float3_t &vDir)
{
}
void XMETHODCALLTYPE CGizmoHandle::unLock(const float3_t &vDir)
{
}

void CGizmoHandle::draw(IXGizmoRenderer *pGRBoth, IXGizmoRenderer *pGR2D, IXGizmoRenderer *pGR3D)
{
	pGRBoth->setPointMode(XGPM_SQUARE);
	pGRBoth->setColor(float4(1.0f));
	pGRBoth->setPointSize(getOnscreenSize());
	pGRBoth->drawPoint(m_vPos);

	drawGrid(pGR3D);
}

void CGizmoHandle::setWorldRay(const float3 &vRayOrigin, const float3 &vRayDir)
{
	float3 vDir = vRayDir * 1000.0f;
	float3 vPoint;
	if(m_planeBase.intersectLine(&vPoint, vRayOrigin - vDir, vRayOrigin + vDir))
	{
		setPos(vPoint);
	}
}
void CGizmoHandle::setTracking(bool isTracking)
{
	m_isTracking = isTracking;

	if(isTracking)
	{
		m_planeBase = SMPLANE(m_vBestPlaneNormal, m_vPos);


	}

	m_pEditor->setDirty();
}

void CGizmoHandle::drawGrid(IXGizmoRenderer *pGR3D)
{
	if(!m_isTracking)
	{
		return;
	}

	float3 vT, vB, vN = m_vBestPlaneNormal;
	if(fabsf(SMVector3Dot(vN, float3(0.0f, 1.0f, 0.0f))) > 0.9f)
	{
		vT = float3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		vT = float3(0.0f, 1.0f, 0.0f);
	}
	vB = SMVector3Cross(vT, vN);
	vT = SMVector3Cross(vN, vB);

	float3 vOrigin;
	if(m_planeBase.intersectLine(&vOrigin, vN * -1000.0f, vN * 1000.0f))
	{
		float fGridStep = XGetGridStep();
		if(fGridStep > 0.0f)
		{
			SMMATRIX m(SMMatrixTranspose(SMMATRIX(
				float4(vT, 0.0f),
				float4(vB, 0.0f),
				float4(vN, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				)));
			SMMATRIX mInv = SMMatrixInverse(NULL, m);

			float3 vGridCenter = m_vPos * m;
			vGridCenter.x -= fmodf(vGridCenter.x, fGridStep);
			vGridCenter.y -= fmodf(vGridCenter.y, fGridStep);

			//vGridCenter = vGridCenter * mInv;

			pGR3D->setLineWidth(1.0f);

			const int iSize = 10;

			float fInvMax = 1.0f / SMVector3Length2(float3((float)iSize, (float)iSize, 0.0f) * fGridStep);

			float3 vPt;
			for(int i = -iSize; i <= iSize; ++i)
			{
				vPt = float3(vGridCenter + float3((float)i, (float)-iSize, 0.0f) * fGridStep) * mInv;
				pGR3D->setColor(float4(1.0f, 1.0f, 1.0f, 1.0f - SMVector3Length2(vPt - m_vPos) * fInvMax));
				pGR3D->jumpTo(vPt);

				vPt = float3(vGridCenter + float3((float)i, 0.0f, 0.0f) * fGridStep) * mInv;
				pGR3D->setColor(float4(1.0f, 1.0f, 1.0f, 1.0f - SMVector3Length2(vPt - m_vPos) * fInvMax));
				pGR3D->lineTo(vPt);

				vPt = float3(vGridCenter + float3((float)i, (float)iSize, 0.0f) * fGridStep) * mInv;
				pGR3D->setColor(float4(1.0f, 1.0f, 1.0f, 1.0f - SMVector3Length2(vPt - m_vPos) * fInvMax));
				pGR3D->lineTo(vPt);
			}
			for(int j = -iSize; j <= iSize; ++j)
			{
				vPt = float3(vGridCenter + float3((float)-iSize, (float)j, 0.0f) * fGridStep) * mInv;
				pGR3D->setColor(float4(1.0f, 1.0f, 1.0f, 1.0f - SMVector3Length2(vPt - m_vPos) * fInvMax));
				pGR3D->jumpTo(vPt);

				vPt = float3(vGridCenter + float3(0.0f, (float)j, 0.0f) * fGridStep) * mInv;
				pGR3D->setColor(float4(1.0f, 1.0f, 1.0f, 1.0f - SMVector3Length2(vPt - m_vPos) * fInvMax));
				pGR3D->lineTo(vPt);

				vPt = float3(vGridCenter + float3((float)iSize, (float)j, 0.0f) * fGridStep) * mInv;
				pGR3D->setColor(float4(1.0f, 1.0f, 1.0f, 1.0f - SMVector3Length2(vPt - m_vPos) * fInvMax));
				pGR3D->lineTo(vPt);
			}
		}
	}
}
