#ifndef __COMMAND_FACE_EDIT_H
#define __COMMAND_FACE_EDIT_H

#include <xcommon/editor/IXEditorExtension.h>

//#include <common/assotiativearray.h>
//#include <common/string.h>
//#include <xcommon/editor/IXEditable.h>
#include "FaceEdit.h"

class CCommandFaceEdit final: public IXUnknownImplementation<IXEditorCommand>
{
public:
	bool XMETHODCALLTYPE exec() override;
	bool XMETHODCALLTYPE undo() override;

	const char* XMETHODCALLTYPE getText() override
	{
		return("edit face");
	}

	bool XMETHODCALLTYPE isEmpty() override
	{
		return(m_aFaces.size() == 0);
	}

	void addFace(const FaceDesc &fd, const BrushFace &bf);

private:
	UINT addMaterial(const char *szMat);
	void checkSort();

private:
	struct ModFace
	{
		FaceDesc faceDesc;
		BrushFace oldBrushFace;
		BrushFace newBrushFace;
		UINT uOldMat;
		UINT uNewMat;
	};

	Array<ModFace> m_aFaces;
	Array<String> m_aMaterials;
	bool m_isSorted = false;
};

#endif
