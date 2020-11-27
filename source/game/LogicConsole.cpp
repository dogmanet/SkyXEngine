#include "LogicConsole.h"

/*! \skydocent logic_console

*/

BEGIN_PROPTABLE(CLogicConsole)
	//! Добавлять автоматически перенос строки
	DEFINE_FIELD_BOOL(m_useAutoNewLine, PDFF_NONE, "auto_new_line", "Auto new line", EDITOR_COMBOBOX)
		COMBO_OPTION("Yes", "1")
		COMBO_OPTION("No", "0")
	EDITOR_COMBO_END()

	//! Переносить автоматически строку или нет
	DEFINE_INPUT(setAutoNewLine, "setAutoNewLine", "Set auto new line", PDF_BOOL)
	//! Печатает сообщение
	DEFINE_INPUT(sendMessage, "sendMessage", "Send message", PDF_STRING)
	//! Печатает предупреждение
	DEFINE_INPUT(sendWarning, "sendWarning", "Send warning", PDF_STRING)
	//! Печатает ошибку
	DEFINE_INPUT(sendError, "sendError", "Send error", PDF_STRING)
END_PROPTABLE()

REGISTER_ENTITY(CLogicConsole, logic_console);

void CLogicConsole::setAutoNewLine(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_BOOL);

	m_useAutoNewLine = pInputdata->parameter.b;
}
void CLogicConsole::sendMessage(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	LibReport(REPORT_MSG_LEVEL_NOTICE, "%s%s", pInputdata->parameter.str, m_useAutoNewLine ? "\n" : "");
}
void CLogicConsole::sendWarning(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	LibReport(REPORT_MSG_LEVEL_WARNING, "%s%s", pInputdata->parameter.str, m_useAutoNewLine ? "\n" : "");
}
void CLogicConsole::sendError(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	LibReport(REPORT_MSG_LEVEL_ERROR, "%s%s", pInputdata->parameter.str, m_useAutoNewLine ? "\n" : "");
}

