#include "LogicStringbuilder.h"

/*! \skydocent logic_stringbuilder

*/

BEGIN_PROPTABLE(CLogicStringbuilder)
	//! Выдать результат
	DEFINE_OUTPUT(m_onOutValue, "OutValue", "Out value")

	//! Значение по умолчанию
	DEFINE_FIELD_STRING(m_szDefaultValue, PDFF_NONE, "value", "Default value", EDITOR_TEXTFIELD)

	//! Сброс на значение по умолчанию
	DEFINE_INPUT(reset, "reset", "Reset", PDF_NONE)
	//! Установка значения
	DEFINE_INPUT(setValue, "setValue", "Set value", PDF_STRING)
	//! Добавляет значение в конец
	DEFINE_INPUT(appendValue, "appendValue", "Append value", PDF_STRING)
	//! Добавляет значение в начало
	DEFINE_INPUT(prependValue, "prependValue", "Prepend value", PDF_STRING)

	//! Вызывает выход OutValue и передает значение
	DEFINE_INPUT(outValue, "outValue", "Out value", PDF_NONE)
END_PROPTABLE()

REGISTER_ENTITY(CLogicStringbuilder, logic_stringbuilder);

void CLogicStringbuilder::reset(inputdata_t *pInputdata)
{
	m_shouldLoadDefault = true;
}
void CLogicStringbuilder::setValue(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	m_sValue = pInputdata->parameter.str;
}
void CLogicStringbuilder::appendValue(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	loadDefault();

	m_sValue += pInputdata->parameter.str;
}
void CLogicStringbuilder::prependValue(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	loadDefault();

	m_sValue = String(pInputdata->parameter.str) + m_sValue;
}

void CLogicStringbuilder::outValue(inputdata_t *pInputdata)
{
	loadDefault();

	inputdata_t inputData;
	inputData.type = PDF_STRING;
	inputData.parameter.str = m_sValue.c_str();

	FIRE_OUTPUT_PARAM(m_onOutValue, pInputdata->pInflictor, &inputData);
}

void CLogicStringbuilder::loadDefault()
{
	if(m_shouldLoadDefault)
	{
		m_shouldLoadDefault = false;

		m_sValue = m_szDefaultValue;
	}
}
