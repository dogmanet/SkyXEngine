#ifndef __LOGIC_CONSOLE_H
#define __LOGIC_CONSOLE_H

#include "PointEntity.h"

class CLogicConsole: public CPointEntity
{
	DECLARE_CLASS(CLogicConsole, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	
protected:
	void setAutoNewLine(inputdata_t *pInputdata);
	void sendMessage(inputdata_t *pInputdata);
	void sendWarning(inputdata_t *pInputdata);
	void sendError(inputdata_t *pInputdata);

	bool m_useAutoNewLine = true;
};

#endif
