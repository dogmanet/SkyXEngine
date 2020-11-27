#ifndef __IXDOMDOCUMENT_H
#define __IXDOMDOCUMENT_H

#include <gdefines.h>
#include "IXDOMNode.h"
#include "IXCSSSelector.h"

class IXDOMDocument: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE newElement(const char *szTagName, IXDOMElement **ppOut) = 0;
	virtual bool XMETHODCALLTYPE newTextNode(const char *szText, IXDOMText **ppOut) = 0;
	virtual bool XMETHODCALLTYPE newDocumentFragment(const char *szHTML, IXDOMDocumentFragment **ppOut) = 0;

	virtual void XMETHODCALLTYPE getDocumentElement(IXDOMElement **ppOut) = 0;

	virtual bool XMETHODCALLTYPE getElementById(const char *szId, IXDOMElement **ppOut) = 0;
	virtual bool XMETHODCALLTYPE getElementsByClass(const char *szClassName, IXDOMElementList **ppOut) = 0;
	virtual bool XMETHODCALLTYPE getElementsByTag(const char *szTag, IXDOMElementList **ppOut) = 0;

	virtual bool XMETHODCALLTYPE querySelector(const char *szSelector, IXDOMElement **ppOut) = 0;
	virtual bool XMETHODCALLTYPE querySelector(IXCSSSelector *pSelector, IXDOMElement **ppOut) = 0;
	virtual bool XMETHODCALLTYPE querySelectorAll(const char *szSelector, IXDOMElementList **ppOut) = 0;
	virtual bool XMETHODCALLTYPE querySelectorAll(IXCSSSelector *pSelector, IXDOMElementList **ppOut) = 0;

	virtual void XMETHODCALLTYPE requestFocus(IXDOMElement *pElement) = 0;
	virtual void XMETHODCALLTYPE getFocus(IXDOMElement **ppOut) = 0;
};

#endif
