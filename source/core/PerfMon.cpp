
#include "PerfMon.h"

CPerfMon::CPerfMon()
{
	memset(m_aiRecStackPtr, 0, sizeof(m_aiRecStackPtr));
}

void CPerfMon::startSection(ID idSection)
{
	assert(idSection >= 0 && "IDs below 0 are reserved!");

	ID idThread = Core_MGetThreadID();

	CPerfRecord rec;
	rec.m_iDepth = m_aiRecStackPtr[idThread]++;
	rec.m_idSection = idSection;
	rec.m_isEntry = true;
	rec.m_time = std::chrono::high_resolution_clock::now();

	m_aaRecords[m_iWriteIdx][idThread].push_back(rec);
}

void CPerfMon::endSection(ID idSection)
{
	assert(idSection >= 0 && "IDs below 0 are reserved!");

	ID idThread = Core_MGetThreadID();

	CPerfRecord rec;
	rec.m_iDepth = --m_aiRecStackPtr[idThread];
	rec.m_idSection = idSection;
	rec.m_isEntry = false;
	rec.m_time = std::chrono::high_resolution_clock::now();

	m_aaRecords[m_iWriteIdx][idThread].push_back(rec);
}

void CPerfMon::endFrame()
{
	ID idThread = Core_MGetThreadID();
	assert(idThread == 0);

	CPerfRecord rec;
	rec.m_iDepth = 0;
	rec.m_idSection = -2;
	rec.m_isEntry = false;
	rec.m_time = std::chrono::high_resolution_clock::now();

	m_aaRecords[m_iWriteIdx][idThread].push_back(rec);
	

	std::swap(m_iReadIdx, m_iWriteIdx);

	for(int i = 0; i < SX_MAX_THREAD_COUNT; ++i)
	{
		m_aaRecords[m_iWriteIdx][i].clearFast();
		assert(m_aiRecStackPtr[i] == 0);
	}


	rec.m_isEntry = true;
	m_aaRecords[m_iWriteIdx][idThread].push_back(rec);
}

void CPerfMon::syncBegin()
{
	ID idThread = Core_MGetThreadID();
	assert(idThread == 0);

	CPerfRecord rec;
	rec.m_iDepth = 0;
	rec.m_idSection = -1;
	rec.m_isEntry = true;
	rec.m_time = std::chrono::high_resolution_clock::now();

	m_aaRecords[m_iWriteIdx][idThread].push_back(rec);
}

const CPerfRecord *CPerfMon::getRecords(ID idThread, int *piRecordCount) const
{
	assert(piRecordCount);

	auto &arr = m_aaRecords[m_iReadIdx][idThread];
	*piRecordCount = arr.size();
	if(!*piRecordCount)
	{
		return(NULL);
	}

	return(&arr[0]);
}
