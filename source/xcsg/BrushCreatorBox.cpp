#include "BrushCreatorBox.h"
#include "EditorExtension.h"
#include "EditorObject.h"
#include "Editable.h"
#include "CommandCreate.h"

//##########################################################################

CBrushCreatorBox::CBrushCreatorBox(CEditable *pEditable, IXEditor *pEditor, IXGizmoRenderer *pRenderer):
	m_pEditable(pEditable),
	m_pEditor(pEditor),
	m_pRenderer(pRenderer)
{
	add_ref(pRenderer);
}
CBrushCreatorBox::~CBrushCreatorBox()
{
	mem_release(m_pRenderer);
}

bool CBrushCreatorBox::isInitiated()
{
	return(m_isInitiated);
}
void CBrushCreatorBox::init(const float3 &vNormal)
{
	assert(!m_isInitiated);

	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return;
	}

	m_fHeight = m_pEditor->getGridStep();
	if(m_fHeight <= 0.0f)
	{
		m_fHeight = 0.2f;
	}

	m_vUp = SMVector3Normalize(vNormal);
	m_isInitiated = true;
	m_isBoxBuilt = false;
}

void CBrushCreatorBox::activate()
{
}
void CBrushCreatorBox::deactivate()
{
}

bool CBrushCreatorBox::onMouseDown(bool isPrimary)
{
	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return(false);
	}
	
	if(m_isBoxBuilt)
	{
		return(false);
	}

	if(isPrimary)
	{
		m_isMouseDown = true;
		m_vPos1 = m_vPos0 = m_pEditor->getState()->vResolvedWorldMousePos;
		return(true);
	}

	return(false);
}
bool CBrushCreatorBox::onMouseMove()
{
	if(m_pEditor->getViewForWindow(m_pEditor->getState()->activeWindow) == X2D_NONE)
	{
		return(false);
	}

	if(m_isMouseDown)
	{
		m_vPos1 = m_pEditor->getState()->vResolvedWorldMousePos;

		return(true);
	}
	return(false);
}
void CBrushCreatorBox::onMouseUp(bool isPrimary)
{
	if(m_isMouseDown)
	{
		m_isMouseDown = false;

		m_vPos1 = m_pEditor->getState()->vResolvedWorldMousePos;

		if(!isValid())
		{
			m_isInitiated = false;
		}
		else
		{
			m_isBoxBuilt = true;
		}
	}
}

bool CBrushCreatorBox::onKeyDown(UINT uKey)
{
	if(uKey == SIK_ESCAPE)
	{
		if(m_isInitiated)
		{
			m_isInitiated = false;
			return(true);
		}
	}
	else if(uKey == SIK_ENTER || uKey == SIK_NUMPADENTER)
	{
		if(m_isInitiated)
		{
			m_isInitiated = false;
			const char *szMat = m_pEditor->getMaterialBrowser()->getCurrentMaterial();
			// create brush with default height

			COutline outline(m_pEditor, m_pRenderer);
			outline.setPlane(m_vUp);


			float3 vT = CBrushMesh::GetTangent(m_vUp);
			float3 vS = SMVector3Cross(vT, m_vUp);
			float fDT = SMVector3Dot(m_vPos1, vT) - SMVector3Dot(m_vPos0, vT);
			float fDS = SMVector3Dot(m_vPos1, vS) - SMVector3Dot(m_vPos0, vS);

			if(m_isBoxMode)
			{
				outline.setMouse(m_vPos0);
				outline.addPoint();
				outline.setMouse((float3)(m_vPos0 + vT * fDT));
				outline.addPoint();
				outline.setMouse(m_vPos1);
				outline.addPoint();
				outline.setMouse((float3)(m_vPos0 + vS * fDS));
				outline.addPoint();
			}
			else
			{
				float3 vCenter = (m_vPos1 + m_vPos0) * 0.5f;

				fDT *= 0.5f;
				fDS *= 0.5f;

				UINT uSideCount = 16;

				for(UINT i = 0; i < uSideCount; ++i)
				{
					float A = (float)i / (float)uSideCount * SM_2PI;
					outline.setMouse((float3)(vCenter + vS * fDS * cosf(A) - vT * fDT * sinf(A)));
					outline.addPoint();
				}
			}
			outline.closePath();
			outline.buildContours();

			CEditorObject *pObject = new CEditorObject(m_pEditable);
			for(UINT i = 0, l = outline.getContourCount(); i < l; ++i)
			{
				pObject->addBrush(new CBrushMesh(m_pEditable->getCore(), &outline, i, szMat, m_fHeight));
			}
			
			pObject->fixPos();

			CCommandCreate *pCmd = new CCommandCreate(m_pEditor, pObject);
			m_pEditor->execCommand(pCmd);
			
			return(true);
		}
	}

	return(false);
}
void CBrushCreatorBox::onKeyUp(UINT uKey)
{
}

void CBrushCreatorBox::render(bool is3D)
{
	if(true)
	{
		m_pRenderer->reset();
		if(m_isInitiated)
		{
			m_pRenderer->setColor(isValid() ? float4_t(0.6f, 0.8f, 1.0f, 1.0f) : float4_t(1.0f, 0.2f, 0.2f, 1.0f));
			m_pRenderer->drawAABB(SMAABB(m_vPos0, m_vPos1 + m_vUp * m_fHeight));
		}
	}

	m_pRenderer->render(!is3D);
}

UINT CBrushCreatorBox::getClassCount()
{
	return(2);
}

const char* CBrushCreatorBox::getClassAt(UINT idx)
{
	switch(idx)
	{
	case 0:
		return("Box");
	case 1:
		return("Cylinder");
	}
	assert(!"Invalid class idx");
	return(NULL);
}

bool CBrushCreatorBox::useClass(const char *szClassName)
{
	m_isBoxMode = !strcmp("Box", szClassName);
	return(true);
}

bool CBrushCreatorBox::isValid()
{
	return(!SMIsZero(SMAABBVolume(SMAABB(m_vPos0, m_vPos1 + m_vUp * m_fHeight))));
}
