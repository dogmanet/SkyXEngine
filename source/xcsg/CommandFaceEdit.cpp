#include "CommandFaceEdit.h"
#include "EditorObject.h"

bool XMETHODCALLTYPE CCommandFaceEdit::exec()
{
	checkSort();

	for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		ModFace &f = m_aFaces[i];

		f.newBrushFace.szMaterial = m_aMaterials[f.uNewMat].c_str();

		f.faceDesc.pObject->setFaceInfo(f.faceDesc.uFace, f.newBrushFace);
	}

	return(true);
}
bool XMETHODCALLTYPE CCommandFaceEdit::undo()
{
	checkSort();

	for(UINT i = 0, l = m_aFaces.size(); i < l; ++i)
	{
		ModFace &f = m_aFaces[i];

		f.newBrushFace.szMaterial = m_aMaterials[f.uOldMat].c_str();

		f.faceDesc.pObject->setFaceInfo(f.faceDesc.uFace, f.oldBrushFace);
	}

	return(true);
}

void CCommandFaceEdit::addFace(const FaceDesc &fd, const BrushFace &bf)
{
	ModFace &f = m_aFaces[m_aFaces.size()];

	f.faceDesc = fd;
	f.newBrushFace = bf;
	f.uNewMat = addMaterial(f.newBrushFace.szMaterial);

	fd.pObject->getFaceInfo(fd.uFace, &f.oldBrushFace);
	f.uOldMat = addMaterial(f.oldBrushFace.szMaterial);

	m_isSorted = false;
}

UINT CCommandFaceEdit::addMaterial(const char *szMat)
{
	int idx = m_aMaterials.indexOf(szMat, [](const String &a, const char *b){
		return(!strcmp(a.c_str(), b));
	});
	if(idx < 0)
	{
		idx = (int)m_aMaterials.size();
		m_aMaterials.push_back(szMat);
	}
	return((UINT)idx);
}

void CCommandFaceEdit::checkSort()
{
	if(!m_isSorted)
	{
		m_isSorted = true;

		m_aFaces.quickSort([](const ModFace &a, const ModFace &b){
			return(a.faceDesc.pObject < b.faceDesc.pObject);
		});
	}
}
