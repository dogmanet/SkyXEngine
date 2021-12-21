#ifndef __BRUSH_CREATOR_FREE_H
#define __BRUSH_CREATOR_FREE_H

#include <xcommon/editor/IXEditable.h>
#include <mtrl/IXMaterialSystem.h>
#include <xcommon/IXCore.h>
#include "Outline.h"
#include "BrushMesh.h"
#include "IBrushCreator.h"

class CEditable;
//class CEditorObject;

class CBrushCreatorFree final: public IXUnknownImplementation<IBrushCreator>
{
public:
	CBrushCreatorFree(CEditable *pEditable, IXEditor *pEditor, IXGizmoRenderer *pRenderer);
	~CBrushCreatorFree();

	bool isInitiated() override;
	void init(const float3 &vNormal) override;

	void activate() override;
	void deactivate() override;

	bool onMouseDown(bool isPrimary) override;
	bool onMouseMove() override;
	void onMouseUp(bool isPrimary) override;

	bool onKeyDown(UINT uKey) override;
	void onKeyUp(UINT uKey) override;

	void render(bool is3D);

	UINT getClassCount() override;
	const char* getClassAt(UINT idx) override;
	bool useClass(const char *szClassName) override;

private:
	CEditable *m_pEditable;
	IXEditor *m_pEditor;

	IXGizmoRenderer *m_pRenderer = NULL;
	COutline *m_pNewOutline = NULL;

	bool m_isMouseDown = false;
};

#endif
