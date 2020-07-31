#ifndef __IXTEXTUREPROXY_H
#define __IXTEXTUREPROXY_H

#include <gdefines.h>
#include "resource/IXResourceTexture.h"

// {BD03E0C0-7589-4162-8130-5CD770581EE3}
#define IXTEXTUREPROXY_GUID DEFINE_XGUID(0xbd03e0c0, 0x7589, 0x4162, 0x81, 0x30, 0x5c, 0xd7, 0x70, 0x58, 0x1e, 0xe3)
#define IXTEXTUREPROXY_VERSION 2

class IXTextureProxy: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getDescription() const = 0;

	/*! Преобразует входное имя в имя файла, относительно корневого пути. 
		При передаче в szOutput NULL, подсчитывает длину строки и записывает ее по адресу puBufSize.
		По адресу pbWantLoad записывается истина, если прокси хочет сам управлять загрузкой текстурного ресурса, в таком случае будет вызван метод loadTexture,
		в противном случае записывается ложь
	*/
	virtual bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool *pbWantLoad = NULL) = 0;

	//! Выполняет загрузку текстуры, вызывается только если метод resolveName записал истинное значение по адресу pbWantLoad
	virtual bool XMETHODCALLTYPE loadTexture(const char *szName, IXResourceTexture **ppOut) = 0;
};

#endif
