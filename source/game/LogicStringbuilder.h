#ifndef __LOGIC_STRINGBUILDER_H
#define __LOGIC_STRINGBUILDER_H

#include "PointEntity.h"

class CLogicStringbuilder: public CPointEntity
{
	DECLARE_CLASS(CLogicStringbuilder, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	
protected:
	void reset(inputdata_t *pInputdata);
	void setValue(inputdata_t *pInputdata);
	void appendValue(inputdata_t *pInputdata);
	void prependValue(inputdata_t *pInputdata);

	void outValue(inputdata_t *pInputdata);

	output_t m_onOutValue;

	const char *m_szDefaultValue;

private:
	bool m_shouldLoadDefault = true;

	void loadDefault();

	String m_sValue;
};

#endif
