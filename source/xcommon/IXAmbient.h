#ifndef __IXAMBIENT_H
#define __IXAMBIENT_H

#include <gdefines.h>

// {14C00551-A29B-4FB1-B1E0-FDC63EED7336}
#define IXAMBIENT_GUID DEFINE_XGUID(0x14c00551, 0xa29b, 0x4fb1, 0xb1, 0xe0, 0xfd, 0xc6, 0x3e, 0xed, 0x73, 0x36)

class IXTexture;
class IXAmbient: public IXUnknown
{
public:
	//! Установить новый скайбокс, NULL для установки текстуры по умолчанию
	virtual void XMETHODCALLTYPE setSkybox(const char *szTexture) = 0;

	//! Включает/отключает отрисовку скайбокса
	virtual void XMETHODCALLTYPE enableSkybox(bool bEnable) = 0;

	//! Получить текстуру скайбокса
	virtual void XMETHODCALLTYPE getSkyboxTexture(IXTexture **ppTexture) = 0;
};

#endif
