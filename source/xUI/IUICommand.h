#ifndef __IUICOMMAND_H 
#define __IUICOMMAND_H 

#include <gdefines.h>

class IUICommand : public IXUnknown
{
public:
	virtual void execute() = 0;
};

#endif
