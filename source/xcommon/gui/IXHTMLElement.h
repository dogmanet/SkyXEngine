#ifndef __IXHTMLELEMENT_H
#define __IXHTMLELEMENT_H

#include <gdefines.h>

XENUM(XHTML_ELEMENT_TYPE,
	XHTML_TYPE_UNKNOWN,
	XHTML_TYPE_GENERIC,
	XNODE_TYPE_INPUT
	);

class IXHTMLElement: public IXUnknown
{
public:
	virtual XHTML_ELEMENT_TYPE XMETHODCALLTYPE getElementType() const = 0;
};

class IXHTMLInputElement: public IXHTMLElement
{
public:
	virtual void XMETHODCALLTYPE setValue(const char *szClassName) = 0;
	virtual const char* XMETHODCALLTYPE getValue() = 0;
};

#endif
