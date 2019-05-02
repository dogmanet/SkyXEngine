#ifndef __HUD_CONTROLLER_H
#define __HUD_CONTROLLER_H

#include <gdefines.h>
#include <gui/guimain.h>
#include <xcommon/XEvents.h>

enum HUD_EVENT
{
	HUD_EVENT_PLAYER_MOVE, // lParam = float3_t* position
	HUD_EVENT_PLAYER_ROTATE, // lParam = float3_t* angles
	HUD_EVENT_MESSAGE,
	HUD_EVENT_MAP_LOAD,
	HUD_EVENT_MAP_MARKER_ADD,
	HUD_EVENT_MAP_MARKER_DEL,
	HUD_EVENT_MAP_MARKER_CHG,
	HUD_EVENT_WPN_CHANGE,
	HUD_EVENT_WPN_AMMO,
	HUD_EVENT_WPN_MAXAMMO,
	HUD_EVENT_HEALTH
};

class CHUDcontroller
{
public:

	CHUDcontroller();

	~CHUDcontroller();

	void activate();

	void setPlayerPos(const float3 & vPosition);
	void setPlayerRot(const float3 & vRotation);

	void loadMap(const char *szName);

	void setWeaponCurrentLoad(int iLoad);
	void setWeaponMaxLoad(int iLoad);
	void setWeaponMaxAmmo(int iMax);

	void setAimRange(float fRange);

	void setPlayerHealth(float fHealth, float fHealthMax = 100.0f);
	void fadeScreenDmg();

	void chatMsg(const char *msg);

	void update();

protected:
	IEventChannel<XEventLevelSize> *m_pLevelSizeChannel = NULL;
	gui::IDesktop * m_pDesktop;

	gui::dom::IDOMnode * m_pMinimapSpin;
	gui::dom::IDOMnode * m_pMinimapPan;

	gui::dom::IDOMnode * m_pWeaponSpin;
	gui::dom::IDOMnode * m_pWeaponImg;
	gui::dom::IDOMnode * m_pWeaponName;
	gui::dom::IDOMnode * m_pWeaponCurrent;
	gui::dom::IDOMnode * m_pWeaponMax;
	gui::dom::IDOMnode * m_pAimRange;
	gui::dom::IDOMnode * m_pHealthSpin;
	gui::dom::IDOMnode * m_pDeadOverlay;
	gui::dom::IDOMnode * m_pChatLog;

	float3_t m_vLevelMin, m_vLevelMax;

	float m_fMapScale;

	int m_iWeaponMaxLoad;
	int m_iWeaponCurrentLoad;

	float m_fAimRange;

	int64_t m_i64TimeHideChat = 0;
};

#endif
