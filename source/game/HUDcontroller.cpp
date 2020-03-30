
#include "HUDcontroller.h"
#include "GameData.h"
#include <common/file_utils.h>

CHUDcontroller::CHUDcontroller():
	m_fMapScale(2.0f),
	m_iWeaponMaxLoad(0),
	m_iWeaponCurrentLoad(0),
	m_fAimRange(-1.0f)
{
	m_pDesktop = GameData::m_pGUIStack->createDesktopA("game_hud", "ingame.html");

	m_pMinimapSpin = m_pDesktop->getDocument()->getElementById(L"minimap_spin");
	m_pMinimapPan = m_pDesktop->getDocument()->getElementById(L"minimap_pan");

	m_pWeaponSpin = m_pDesktop->getDocument()->getElementById(L"wpn_spin");
	m_pWeaponImg = m_pDesktop->getDocument()->getElementById(L"wpn_img");
	m_pWeaponName = m_pDesktop->getDocument()->getElementById(L"wpn_name");
	m_pWeaponCurrent = m_pDesktop->getDocument()->getElementById(L"wpn_current");
	m_pWeaponMax = m_pDesktop->getDocument()->getElementById(L"wpn_max");

	m_pAimRange = m_pDesktop->getDocument()->getElementById(L"aim_range");

	m_pHealthSpin = m_pDesktop->getDocument()->getElementById(L"health_spin");

	m_pDeadOverlay = m_pDesktop->getDocument()->getElementById(L"dmg_overlay");

	m_pChatLog = m_pDesktop->getDocument()->getElementById(L"chatlog");
	if(m_pChatLog)
	{
		m_pChatLog->addPseudoclass(0x00004);
	}
	
	Core_0RegisterCVarBool("hud_draw", true, "Включает отображение HUD"); 

	m_pLevelSizeChannel = Core_GetIXCore()->getEventChannel<XEventLevelSize>(EVENT_LEVEL_GET_SIZE_GUID);
}

CHUDcontroller::~CHUDcontroller()
{
	mem_release(m_pDesktop);
}

void CHUDcontroller::activate()
{
	GameData::m_pGUIStack->setActiveDesktop(m_pDesktop);
}

void CHUDcontroller::setPlayerPos(const float3 & vPosition)
{
	if(m_pMinimapPan)
	{
		m_pMinimapPan->getStyleSelf()->top->set((vPosition.z - m_vLevelMax.z) * m_fMapScale);
		m_pMinimapPan->getStyleSelf()->left->set(-(vPosition.x - m_vLevelMin.x) * m_fMapScale);
		m_pMinimapPan->updateStyles();
	}
}

void CHUDcontroller::setPlayerRot(const float3 & vRotation)
{
	if(m_pMinimapSpin)
	{
		m_pMinimapSpin->getStyleSelf()->transform->set(SMMatrixRotationZ(vRotation.y));
		m_pMinimapSpin->updateStyles();
	}
}

void CHUDcontroller::loadMap(const char *szName)
{
	// /hud/levels/stalker_atp.png
	XEventLevelSize levelSize;
	m_pLevelSizeChannel->broadcastEvent(&levelSize);

	m_vLevelMin = levelSize.vMin;
	m_vLevelMax = levelSize.vMax;

	if(m_pMinimapPan)
	{
		char szMapFile[128];
		
		sprintf(szMapFile, "./gui/textures/hud/levels/%s.png", szName);
		if(FileExistsFile(szMapFile))
		{
			sprintf(szMapFile, "/hud/levels/%s.png", szName);
			m_pMinimapPan->getStyleSelf()->background_size_x->set(100.0f);
			m_pMinimapPan->getStyleSelf()->background_size_y->set(100.0f);
			m_pMinimapPan->getStyleSelf()->background_size_x->setDim(gui::css::ICSSproperty::DIM_PC);
			m_pMinimapPan->getStyleSelf()->background_size_y->setDim(gui::css::ICSSproperty::DIM_PC);
		}
		else
		{
			sprintf(szMapFile, "/hud/map_placeholder.png");
			m_pMinimapPan->getStyleSelf()->background_size_x->unset();
			m_pMinimapPan->getStyleSelf()->background_size_y->unset();
		}
		
		m_pMinimapPan->getStyleSelf()->width->set((m_vLevelMax.x - m_vLevelMin.x) * m_fMapScale);
		m_pMinimapPan->getStyleSelf()->height->set((m_vLevelMax.z - m_vLevelMin.z) * m_fMapScale);
		m_pMinimapPan->getStyleSelf()->background_image->set(StringW(String(szMapFile)));
		m_pMinimapPan->updateStyles();
	}
}

void CHUDcontroller::setWeaponCurrentLoad(int iLoad)
{
	m_iWeaponCurrentLoad = iLoad;

	if(m_pWeaponCurrent)
	{
		m_pWeaponCurrent->setText(StringW(iLoad) + L" / " + StringW(m_iWeaponMaxLoad), TRUE);
	}
	if(m_pWeaponSpin)
	{
		const float fStep = 100.0f / 40.0f;
		float fPercent = (float)iLoad / (float)m_iWeaponMaxLoad * 100.0f;

		m_pWeaponSpin->getStyleSelf()->_gui_spin_percent->set(floorf(fPercent / fStep) * fStep);
		m_pWeaponSpin->updateStyles();
	}
}

void CHUDcontroller::setWeaponMaxLoad(int iLoad)
{
	m_iWeaponMaxLoad = iLoad;
	setWeaponCurrentLoad(m_iWeaponCurrentLoad);
}

void CHUDcontroller::setWeaponMaxAmmo(int iMax)
{
	if(m_pWeaponMax)
	{
		wchar_t str[64];
		wsprintfW(str, L"%04d", iMax);
		m_pWeaponMax->setText(str, TRUE);
	}
}

void CHUDcontroller::setAimRange(float fRange)
{
	if(!m_pAimRange)
	{
		return;
	}
	if(fRange >= 0.0f)
	{
		if(fRange != m_fAimRange)
		{
			wchar_t txt[32];
			swprintf(txt, L"%.2f", fRange);
			m_pAimRange->setText(txt, TRUE);
		}
	}
	else if(m_fAimRange >= 0.0f)
	{
		m_pAimRange->setText(L"", TRUE);
	}
	m_fAimRange = fRange;
}

void CHUDcontroller::setPlayerHealth(float fHealth, float fHealthMax)
{
	float fRangeMin = 0.08333f;
	float fRangeMax = 0.41667f;

	float fSpin = (fHealth / fHealthMax) * (fRangeMax - fRangeMin) + fRangeMin;

	m_pHealthSpin->getStyleSelf()->_gui_spin_percent->set(fSpin * 100.0f);
	m_pHealthSpin->updateStyles();

	if(fHealth <= 0.0f)
	{
		m_pDeadOverlay->getStyleSelf()->visibility->set(gui::css::ICSSproperty::VISIBILITY_VISIBLE);
	}
	else
	{
		m_pDeadOverlay->getStyleSelf()->visibility->set(gui::css::ICSSproperty::VISIBILITY_HIDDEN);
	}
	m_pDeadOverlay->updateStyles();
}

void CHUDcontroller::fadeScreenDmg()
{
}

void CHUDcontroller::chatMsg(const char *msg)
{
	static const float *s_fChatTime = GET_PCVAR_FLOAT("hud_chat_time");
	static const int *s_iChatLines = GET_PCVAR_INT("hud_chat_lines");
	
	if(m_pChatLog)
	{
		m_pChatLog->appendHTML(StringW(L"<div>")+StringW(String(msg))+L"</div>");
		while((int)m_pChatLog->getChilds()->size() > *s_iChatLines)
		{
			m_pChatLog->removeChild(m_pChatLog->getChilds()[0][0]);
		}
		m_pChatLog->removePseudoclass(0x00004);
		m_i64TimeHideChat = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) + (int64_t)(*s_fChatTime * 1000000.0f);
	}
}

void CHUDcontroller::update()
{
	if(m_i64TimeHideChat != 0 && TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) > m_i64TimeHideChat)
	{
		m_i64TimeHideChat = 0;
		m_pChatLog->addPseudoclass(0x00004);
	}
}
