#include "PlayerSpawn.h"

/*! \skydocent info_player_spawn
Камера
*/

BEGIN_PROPTABLE(CPlayerSpawn)
	/*! Команда игроков, для которой работает спавн. 
	\note Только для многопользовательского режима
	*/
	DEFINE_FIELD_INT(m_iTeam, 0, "team", "Team", EDITOR_COMBOBOX)
		COMBO_OPTION("Team 1", "1")
		COMBO_OPTION("Team 2", "2")
		COMBO_OPTION("Team 3", "3")
		COMBO_OPTION("Team 4", "4")
	EDITOR_COMBO_END()

END_PROPTABLE()

REGISTER_ENTITY(CPlayerSpawn, info_player_spawn);
