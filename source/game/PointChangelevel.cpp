#include "PointChangelevel.h"
//#include "GameData.h"

BEGIN_PROPTABLE(CPointChangelevel)
	//! Запустить смену уровня, опциональный параметр имя нового уровня
	DEFINE_INPUT(inChangeLevel, "changeLevel", "Change level", PDF_STRING)
	//! Имя следующего уровня
	DEFINE_FIELD_STRING(m_szNextLevelName, 0, "next_level", "Next level name", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY(CPointChangelevel, point_changelevel);

void CPointChangelevel::inChangeLevel(inputdata_t *pInputdata)
{
	const char *szNextLevel = m_szNextLevelName;
	if(pInputdata->type == PDF_STRING && pInputdata->parameter.str && pInputdata->parameter.str[0])
	{
		szNextLevel = pInputdata->parameter.str;
	}

	static const bool *dev_reset_world_on_run = GET_PCVAR_BOOL("dev_reset_world_on_run");
	if(*dev_reset_world_on_run)
	{
		// We're likely in editor, don't actually changelevel
		LibReport(REPORT_MSG_LEVEL_NOTICE, COLOR_LMAGENTA "Changelevel to %s initiated" COLOR_RESET "\n", szNextLevel);
	}
	else
	{
		Core_GetIXCore()->getConsole()->execCommand2("map %s", szNextLevel);
	}
}
