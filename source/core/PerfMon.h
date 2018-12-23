#ifndef __PERFMON_H
#define __PERFMON_H

#include <gdefines.h>
#include <common/array.h>
#include <chrono>
#include "sxcore.h"

class CPerfMon
{
	typedef std::chrono::system_clock::time_point time_point;

public:
	CPerfMon();


	void startSection(ID idSection);
	void endSection(ID idSection);

	void syncBegin();

	void endFrame();

	const CPerfRecord *getRecords(ID idThread, int *piRecordCount) const;
protected:


	int m_iReadIdx = 0;
	int m_iWriteIdx = 1;

	Array<CPerfRecord> m_aaRecords[2][SX_MAX_THREAD_COUNT];
	int m_aiRecStackPtr[SX_MAX_THREAD_COUNT];
};

#endif
