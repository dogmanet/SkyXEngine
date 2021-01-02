#include "LogicEntityExists.h"

/*! \skydocent logic_entity_exists

*/

BEGIN_PROPTABLE(CLogicEntityExists)
	//! Устанавливает имя
	DEFINE_INPUT(setEntityName, "setEntityName", "Set entity name", PDF_STRING)
	//! Устанавливает имя и выполняет проверку
	DEFINE_INPUT(setEntityNameAndTest, "setEntityNameAndTest", "Set name and test", PDF_STRING)
	//! Выполняет проверку
	DEFINE_INPUT(test, "test", "Perform test", PDF_NONE)

	//! Выдается если тест пройден
	DEFINE_OUTPUT(m_onExists, "OnExists", "On exists")
	//! Выдается если тест не пройден
	DEFINE_OUTPUT(m_onNotExists, "OnNotExists", "On not exists")
	//! Выдается после теста, выдает истину или ложь
	DEFINE_OUTPUT(m_onTest, "OnTest", "On test")
END_PROPTABLE()

REGISTER_ENTITY(CLogicEntityExists, logic_entity_exists, REC_ICON("dev_logic_entity_exist"));

void CLogicEntityExists::setEntityName(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	m_sTestName = pInputdata->parameter.str;
}
void CLogicEntityExists::setEntityNameAndTest(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	m_sTestName = pInputdata->parameter.str;

	test(pInputdata);
}
void CLogicEntityExists::test(inputdata_t *pInputdata)
{
	bool isFound = m_pMgr->findEntityByName(m_sTestName.c_str()) != NULL;

	if(isFound)
	{
		FIRE_OUTPUT(m_onExists, pInputdata->pInflictor);
	}
	else
	{
		FIRE_OUTPUT(m_onNotExists, pInputdata->pInflictor);
	}

	inputdata_t data = {0};
	data.type = PDF_BOOL;
	data.parameter.b = isFound;
	FIRE_OUTPUT_PARAM(m_onTest, pInputdata->pInflictor, &data);
}
