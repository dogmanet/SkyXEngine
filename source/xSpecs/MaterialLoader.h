#ifndef __MATERIALLOADER_H
#define __MATERIALLOADER_H

#include <xcommon/IXMaterialLoader.h>
#include <xcommon/IXCore.h>

class CMaterialLoader: public IXMaterialLoader
{
public:
	CMaterialLoader(IXCore *pCore);

	UINT XMETHODCALLTYPE getExtCount() const override;
	bool XMETHODCALLTYPE canSave(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getExt(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getExtText(UINT uIndex) const override;
	const char* XMETHODCALLTYPE getAuthor() const override;
	const char* XMETHODCALLTYPE getCopyright() const override;
	const char* XMETHODCALLTYPE getDescription() const override;

	bool XMETHODCALLTYPE open(const char *szFileName, const char *szArg, bool forSave = false) override;
	bool XMETHODCALLTYPE load(IXMaterial *pMaterial) override;
	bool XMETHODCALLTYPE save(IXMaterial *pMaterial) override;
	void XMETHODCALLTYPE close() override;

protected:
	IXCore *m_pCore = NULL;

	IXConfig *m_pConfig = NULL;
};

#endif
