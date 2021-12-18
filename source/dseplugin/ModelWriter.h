#ifndef __MODELWRITER_H
#define __MODELWRITER_H

#include <xcommon/IXModelWriter.h>
#include "ModelFile.h"

class CModelWriter: public IXUnknownImplementation<IXModelWriter>
{
public:
	XIMPLEMENT_VERSION(IXMODELWRITER_VERSION);

	UINT XMETHODCALLTYPE getExtCount() const override;
	const char* XMETHODCALLTYPE getExt(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getExtText(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getAuthor() const override;
	const char* XMETHODCALLTYPE getCopyright() const override;
	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE writeModel(IXResourceModel *pResource, IFile *pFile) override;

protected:
	ModelHeader m_hdr;
	ModelHeader2 m_hdr2;
	bool m_bWantHdr2 = false;
};

#endif
