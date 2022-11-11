#ifndef __MODELLOADER_H
#define __MODELLOADER_H

#include <xcommon/IXModelLoader.h>
#include "ModelFile.h"

class CModelLoader: public IXUnknownImplementation<IXModelLoader>
{
public:
	UINT XMETHODCALLTYPE getVersion() override;

	UINT XMETHODCALLTYPE getExtCount() const override;
	const char* XMETHODCALLTYPE getExt(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getExtText(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getAuthor() const override;
	const char* XMETHODCALLTYPE getCopyright() const override;
	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE open(IFile *pFile) override;
	XMODELTYPE XMETHODCALLTYPE getType() const override;
	bool XMETHODCALLTYPE loadAsStatic(IXResourceModelStatic *pResource) override;
	bool XMETHODCALLTYPE loadAsAnimated(IXResourceModelAnimated *pResource) override;
	void XMETHODCALLTYPE getInfo(XModelInfo *pModelInfo) override;
	void XMETHODCALLTYPE close() override;


	bool loadGeneric(IXResourceModel *pResource);

protected:
	IFile *m_pCurrentFile = NULL;

	ModelHeader m_hdr;
	ModelHeader2 m_hdr2;
	ModelHeader3 m_hdr3;
};

#endif
