#ifndef __IXCSS_H
#define __IXCSS_H

#include <gdefines.h>
#include "IXCSSSelector.h"

// class IXCSS;
// class IXCSSStyle; // list of ICSSproperty
// class IXCSSproperty;
// class IXCSSSelector;

// class IXCSSRule; // IXCSSSelector + IXCSSStyle
// class IXCSSRuleSet; // list of IXCSSRule

class IXCSS: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE newSelector(const char *szSelector, IXCSSSelector **ppOut) = 0;
};

#endif
