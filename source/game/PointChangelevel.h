#ifndef __POINT_CHANGELEVEL_H
#define __POINT_CHANGELEVEL_H

#include "PointEntity.h"

//! Смена уровня
class CPointChangelevel: public CPointEntity
{
	DECLARE_CLASS(CPointChangelevel, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

private:
	void inChangeLevel(inputdata_t *pInputdata);

private:
	const char *m_szNextLevelName;
};

#endif
