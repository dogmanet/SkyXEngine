#ifndef __LOGIC_ENTITY_EXISTS_H
#define __LOGIC_ENTITY_EXISTS_H

#include "PointEntity.h"

class CLogicEntityExists: public CPointEntity
{
	DECLARE_CLASS(CLogicEntityExists, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	
protected:
	void setEntityName(inputdata_t *pInputdata);
	void setEntityNameAndTest(inputdata_t *pInputdata);
	void test(inputdata_t *pInputdata);

	output_t m_onExists;
	output_t m_onNotExists;
	output_t m_onTest;

private:
	String m_sTestName;
};

#endif
