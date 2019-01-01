
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "GameData.h"
#include "CrosshairManager.h"

#include "Ragdoll.h"

#include <score/sxscore.h>
#include <level/sxlevel.h>
#include <input/sxinput.h>


#include "Tracer.h"

#include "GameStates.h"

#include "SettingsWriter.h"
#include "BaseWeapon.h"
#include "BaseAmmo.h"
#include "BaseMag.h"
#include "FuncTrain.h"

CPlayer * GameData::m_pPlayer;
CPointCamera * GameData::m_pActiveCamera;
gui::IGUI * GameData::m_pGUI = NULL;
CEntityManager * GameData::m_pMgr;
CHUDcontroller * GameData::m_pHUDcontroller;
CGameStateManager * GameData::m_pGameStateManager;
gui::dom::IDOMnode *GameData::m_pCell;
//gui::IDesktop *GameData::m_pStatsUI;

CRagdoll * g_pRagdoll;
IAnimPlayer * pl;

CTracer *g_pTracer;
CTracer *g_pTracer2;



//##########################################################################

static void UpdateSettingsDesktop()
{
	gui::IDesktop * pSettingsDesktop = GameData::m_pGUI->getActiveDesktop();
	gui::dom::IDOMdocument * doc = pSettingsDesktop->getDocument();
	const gui::dom::IDOMnodeCollection &items = *doc->getElementsByClass(L"set_item");

	char szCvarValue[256];
	for(int i = 0, l = items.size(); i < l; ++i)
	{
		gui::dom::IDOMnode * pNode = items[i];

		const StringW &sAttr = pNode->getAttribute(L"cvar");
		if(sAttr.length())
		{
			String s(sAttr);

			int iCvars = parse_str((char*)s.c_str(), NULL, 0, '|');
			char ** parts = (char**)alloca(sizeof(char*)*iCvars);
			iCvars = parse_str((char*)s.c_str(), parts, iCvars, '|');

			StringW sValue;

			for(int j = 0; j < iCvars; ++j)
			{
				Core_0GetCVarAsString(parts[j], szCvarValue, sizeof(szCvarValue));
				if(j > 0)
				{
					sValue += L"|";
				}
				sValue += String(szCvarValue);
			}

			printf("Set: '%s' = '%s'\n", s.c_str(), String(sValue).c_str());
			pNode->setAttribute(L"value", sValue);
		}
	}
}


//##########################################################################

GameData::GameData(HWND hWnd, bool isGame):
	m_hWnd(hWnd)
{
	if(hWnd)
	{
		loadFoostepsSounds();
	}
	else
	{
		memset(m_pidFootstepSound, 0, sizeof(m_pidFootstepSound[0]) * MPT_COUNT);
		memset(m_iFootstepSoundCount, 0, sizeof(m_iFootstepSoundCount[0]) * MPT_COUNT);
	}

	m_isGame = isGame;
	
	if(hWnd)
	{
		HMODULE hDLL = LoadLibrary("sxgui"
#ifdef _DEBUG
			"_d"
#endif
			".dll");
		if(!hDLL)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load sxgui"
#ifdef _DEBUG
				"_d"
#endif
				".dll");
		}

		gui::PFNINITINSTANCE pfnGUIInit;
		pfnGUIInit = (gui::PFNINITINSTANCE)GetProcAddress(hDLL, "InitInstance");

		if(!pfnGUIInit)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "The procedure entry point InitInstance could not be located in the dynamic link library sxgui.dll");
		}

		m_pGUI = pfnGUIInit(SGCore_GetDXDevice(), "./gui/", hWnd);
	
		m_pHUDcontroller = new CHUDcontroller();
	}
	m_pMgr = new CEntityManager(hWnd == NULL);

	Core_0RegisterConcmd("+forward", ccmd_forward_on);
	Core_0RegisterConcmd("-forward", ccmd_forward_off);
	Core_0RegisterConcmd("+backward", ccmd_backward_on);
	Core_0RegisterConcmd("-backward", ccmd_backward_off);
	Core_0RegisterConcmd("+left", ccmd_left_on);
	Core_0RegisterConcmd("-left", ccmd_left_off);
	Core_0RegisterConcmd("+right", ccmd_right_on);
	Core_0RegisterConcmd("-right", ccmd_right_off);
	Core_0RegisterConcmd("+boost", ccmd_boost_on);
	Core_0RegisterConcmd("-boost", ccmd_boost_off);
	Core_0RegisterConcmd("+crouch", ccmd_crouch_on);
	Core_0RegisterConcmd("-crouch", ccmd_crouch_off);
	Core_0RegisterConcmd("+jump", ccmd_jump_on);
	Core_0RegisterConcmd("-jump", ccmd_jump_off);
	Core_0RegisterConcmd("+crawl", ccmd_crawl_on);
	Core_0RegisterConcmd("-crawl", ccmd_crawl_off);
	Core_0RegisterConcmd("+attack", ccmd_attack_on);
	Core_0RegisterConcmd("-attack", ccmd_attack_off);
	Core_0RegisterConcmd("+attack2", ccmd_attack2_on);
	Core_0RegisterConcmd("-attack2", ccmd_attack2_off);
	Core_0RegisterConcmd("reload", ccmd_reload);
	Core_0RegisterConcmd("switch_firemode", ccmd_switch_firemode);
	Core_0RegisterConcmd("flashlight", ccmd_toggleflashlight);
	Core_0RegisterConcmd("+use", ccmd_use_on);
	Core_0RegisterConcmd("-use", ccmd_use_off);
	Core_0RegisterConcmdArg("send_camera", [](int argc, const char ** argv){
		if (argc < 2)
		{
			printf("cmd send_camera requires one argument");
			return;
		}

		CBaseEntity *pEnt = m_pMgr->findEntityByName(argv[1]);
		if (pEnt)
		{
			CFuncTrain *pTrain = (CFuncTrain*)pEnt->getParent();
			if (pTrain)
			{
				m_pActiveCamera = (CPointCamera*)pEnt;
				pTrain->start();
			}
			else
				printf("cmd send_camera of found parent for camera '%s'", argv[1]);
		}
		else
			printf("cmd send_camera not found '%s' camera", argv[1]);
	});

	Core_0RegisterConcmdArg("gui_load", [](int argc, const char ** argv){
		if(argc != 3)
		{
			printf("Usage: gui_load <desktop_name> <file>");
			return;
		}
		if(!GameData::m_pGUI->getDesktopA(argv[1]))
		{
			GameData::m_pGUI->createDesktopA(argv[1], argv[2]);
		}
	});
	Core_0RegisterConcmdArg("gui_push", [](int argc, const char ** argv){
		if(argc != 2)
		{
			printf("Usage: gui_push <desktop_name>");
			return;
		}
		gui::IDesktop * dp = GameData::m_pGUI->getDesktopA(argv[1]);
		if(!dp)
		{
			printf("Desktop '%s' not found", argv[1]);
			return;
		}
		GameData::m_pGUI->pushDesktop(dp);
	}); 
	Core_0RegisterConcmd("gui_pop", [](){
		GameData::m_pGUI->popDesktop();
	});
	Core_0RegisterConcmd("gui_settings_init", []()
	{
		UpdateSettingsDesktop();
	});

	Core_0RegisterConcmdArg("ent_load_level", [](int argc, const char ** argv){
		if(argc != 3)
		{
			printf("Usage: ent_load_file <entfile> <levelname>");
			return;
		}
		LibReport(REPORT_MSG_LEVEL_NOTICE, "load entity\n");
		SGame_LoadEnts(argv[1]);
		SGame_OnLevelLoad(argv[2]);
	});
	Core_0RegisterConcmd("ent_unload_level", [](){
		SGame_UnloadObjLevel();
	});
	Core_0RegisterConcmdArg("ent_save_level", [](int argc, const char ** argv){
		if(argc != 2)
		{
			printf("Usage: ent_save_level <entfile>");
			return;
		}
		SGame_SaveEnts(argv[1]);
	});

	Core_0RegisterConcmdArg("ent_dump_info", [](int argc, const char ** argv)
	{
		if(argc != 2)
		{
			printf("Usage: ent_dump_info <hexaddress>");
			return;
		}

		CBaseEntity *pEnt;
		if(sscanf(argv[1], "%x", &pEnt))
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "Ent: id:%d; cls:'%s'; name:'%s'\n", pEnt->getId(), pEnt->getClassName(), pEnt->getName());
		}
	});
	
	Core_0RegisterConcmdArg("map", [](int argc, const char ** argv)
	{
		if(argc != 2)
		{
			printf("Usage: map <levelname>");
			return;
		}
		
		SLevel_Load(argv[1], true);

		//GameData::m_pGameStateManager->activate("ingame");

		if(m_pMgr->isServerMode())
		{
			Core_0ConsoleExecCmd("gmode server");
		}
		else
		{
			Core_0ConsoleExecCmd("gmode ingame");
			Core_0ConsoleExecCmd("spawn");

			for(int i = 0; i < 0; ++i)
			{
				CBaseEntity* bEnt = SGame_CreateEntity("npc_zombie");
				bEnt->setFlags(bEnt->getFlags() | EF_EXPORT | EF_LEVEL);
				bEnt->setKV("origin", "0 1 0");
			}
		}
	});

	Core_0RegisterConcmdArg("gmode", [](int argc, const char ** argv)
	{
		if(argc != 2)
		{
			printf("Usage: gmode <mode>");
			return;
		}

		GameData::m_pGameStateManager->activate(argv[1]);
	});

	Core_0RegisterConcmd("game_menu", ccmd_game_menu);

	Core_0RegisterConcmd("spawn", ccmd_spawn);
	Core_0RegisterConcmd("observe", ccmd_observe);

	//Core_0RegisterCVarFloat("r_default_fov", 45.0f, "Default FOV value");
	Core_0RegisterCVarBool("cl_mode_editor", false, "Editor control mode");
	Core_0RegisterCVarBool("cl_grab_cursor", true, "Grab cursor on move");
	

	Core_0RegisterCVarBool("cl_bob", true, "View bobbing");
	Core_0RegisterCVarFloat("cl_bob_walk_y", 0.1f, "View bobbing walk y amplitude");
	Core_0RegisterCVarFloat("cl_bob_walk_x", 0.1f, "View bobbing walk strafe amplitude");
	Core_0RegisterCVarFloat("cl_bob_run_y", 0.2f, "View bobbing run y amplitude");
	Core_0RegisterCVarFloat("cl_bob_run_x", 0.15f, "View bobbing run strafe amplitude");
	Core_0RegisterCVarFloat("cl_bob_walk", 0.5f, "View bobbing walk period");
	Core_0RegisterCVarFloat("cl_bob_run", 1.0f, "View bobbing run period");

	Core_0RegisterCVarFloat("lvl_debris_remove_time_min", 120.0f, "Debris collector time min");
	Core_0RegisterCVarFloat("lvl_debris_remove_time_max", 200.0f, "Debris collector time max");

	Core_0RegisterCVarFloat("hud_chat_time", 5.0f, "Chat hide time");
	Core_0RegisterCVarInt("hud_chat_lines", 5, "Chat lines limit");

	Core_0RegisterCVarBool("hud_crosshair", true, "Отображать метку прицела");
	Core_0RegisterCVarBool("hud_rangefinder", true, "Отображать дальномер");

	if(hWnd)
	{
		CCrosshairManager::loadConfig("config/game/crosshairs.cfg");

		m_pPlayer = (CPlayer*)CREATE_ENTITY("player", m_pMgr);
		m_pActiveCamera = m_pPlayer->getCamera();
		m_pCrosshair = m_pPlayer->getCrosshair();

		ID idCrosshairDefault = CCrosshairManager::getCrosshairID("default");
		if(ID_VALID(idCrosshairDefault))
		{
			CCrosshairManager::loadCrosshair(idCrosshairDefault, m_pCrosshair);
		}
	}

	Core_0RegisterConcmdClsArg("+debug_slot_move", m_pPlayer, (SXCONCMDCLSARG)&CPlayer::_ccmd_slot_on);
	Core_0RegisterConcmdCls("-debug_slot_move", m_pPlayer, (SXCONCMDCLS)&CPlayer::_ccmd_slot_off);

	g_pTracer = new CTracer(5000);
	g_pTracer2 = new CTracer(5000);

	m_pGameStateManager = new CGameStateManager();
	if(hWnd)
	{
		m_pGameStateManager->addState(new CMainMenuGameState(), "main_menu");
		m_pGameStateManager->addState(new CIngameMenuGameState(), "ingame_menu");
	}
	else
	{
		m_pGameStateManager->addState(new CServerState(), "server");
	}
	m_pGameStateManager->addState(new CIngameGameState(), "ingame");
	m_pGameStateManager->addState(new CEditorState(), "editor");

	if(m_isGame)
	{
		m_pGameStateManager->activate("main_menu");
	}
	else if(hWnd)
	{
		m_pGameStateManager->activate("editor");
	}
	else
	{
		m_pGameStateManager->activate("server");
	}

	if(m_pGUI)
	{
		m_pGUI->registerCallback("on_exit", [](gui::IEvent * ev){
			PostQuitMessage(0);
		});
		m_pGUI->registerCallback("on_cancel", [](gui::IEvent * ev){
			GameData::m_pGUI->popDesktop();
		});
		m_pGUI->registerCallback("exit_prompt", [](gui::IEvent * ev){
			if(ev->key == KEY_ESCAPE || ev->key == KEY_LBUTTON)
			{
				GameData::m_pGUI->messageBox(StringW(String("Вы действительно хотите выйти?")).c_str(), L"", StringW(String("Да")).c_str(), L"on_exit", StringW(String("Нет")).c_str(), L"on_cancel", NULL);
			}
		});
		m_pGUI->registerCallback("dial_loadlevel", [](gui::IEvent * ev){
			static gui::IDesktop * pLoadLevelDesktop = GameData::m_pGUI->createDesktopA("menu_loadlevel", "menu/loadlevel.html");
			GameData::m_pGUI->pushDesktop(pLoadLevelDesktop);

			gui::dom::IDOMdocument * doc = pLoadLevelDesktop->getDocument();

			doc->getElementsByTag(L"body")[0][0]->removePseudoclass(0x00010);

			gui::dom::IDOMnode * pNode = doc->getElementById(L"map_list");
			while(pNode->getChilds()->size())
			{
				pNode->removeChild((*(pNode->getChilds()))[0]);
			}



			CLevelInfo levelInfo;
			memset(&levelInfo, 0, sizeof(CLevelInfo));
			while(SLevel_EnumLevels(&levelInfo))
			{
				LibReport(REPORT_MSG_LEVEL_NOTICE, "Level: %s, dir: %s\n", levelInfo.m_szLocalName, levelInfo.m_szName);

				StringW text = StringW(L"<radio name=\"map\" class=\"menu-item\" onclick=\"loadlevel_select\" has_preview=\"") + (levelInfo.m_bHasPreview ? L"1" : L"0") + L"\" level_name=\"" + StringW(String(levelInfo.m_szName)) + L"\">" + StringW(String(levelInfo.m_szLocalName)) + L"</radio>";

				gui::dom::IDOMnodeCollection newItems = pLoadLevelDesktop->createFromText(text);
				for(UINT i = 0, l = newItems.size(); i < l; i++)
				{
					pNode->appendChild(newItems[i]);
				}

			}
		});
		m_pGUI->registerCallback("loadlevel_select", [](gui::IEvent * ev){
			gui::dom::IDOMdocument * doc = GameData::m_pGUI->getActiveDesktop()->getDocument();
			doc->getElementsByTag(L"body")[0][0]->addPseudoclass(0x00010);

			//LibReport(REPORT_MSG_LEVEL_NOTICE, "Sel: %s\n", sLevelName.c_str());

			gui::dom::IDOMnode * pNode = doc->getElementById(L"map_preview");
			gui::dom::IDOMnode * pButton = doc->getElementById(L"loadlevel_go");

			if(pNode)
			{
				StringW sLevelName = ev->target->getAttribute(L"level_name");
				if(ev->target->getAttribute(L"has_preview")[0] == L'1')
				{
					StringW sPreview = StringW(L"../../levels/") + sLevelName + L"/preview.bmp";
					pNode->getStyleSelf()->background_image->set(sPreview);
				}
				else
				{
					pNode->getStyleSelf()->background_image->set(L"/default_level_preview.png");
				}
				pNode->updateStyles();

				if(pButton)
				{
					pButton->setAttribute(L"level_name", sLevelName);
				}
			}

		});
		m_pGUI->registerCallback("loadlevel_go", [](gui::IEvent * ev){
			StringW sLevelName = ev->target->getAttribute(L"level_name");

			Core_0ConsoleExecCmd("map %s", String(sLevelName).c_str());
		});
		m_pGUI->registerCallback("return_ingame", [](gui::IEvent * ev){
			if(ev->key == KEY_ESCAPE || ev->key == KEY_LBUTTON)
			{
				GameData::m_pGameStateManager->activate("ingame");
			}
		});
		m_pGUI->registerCallback("mainmenu_prompt", [](gui::IEvent * ev){
			if(ev->key == KEY_ESCAPE || ev->key == KEY_LBUTTON)
			{
				GameData::m_pGUI->messageBox(StringW(String("Вы действительно хотите выйти в главное меню?")).c_str(), StringW(String("Весь несохраненный прогресс будет утерян.")).c_str(), StringW(String("Да")).c_str(), L"to_mainmenu", StringW(String("Нет")).c_str(), L"on_cancel", NULL);
			}
		});
		m_pGUI->registerCallback("to_mainmenu", [](gui::IEvent * ev){
			GameData::m_pGameStateManager->activate("main_menu");

			//Core_0ConsoleExecCmd("observe");
			GameData::m_pPlayer->observe();

			SLevel_Clear();
		});
		m_pGUI->registerCallback("dial_settings", [](gui::IEvent * ev){
			static gui::IDesktop * pSettingsDesktop = GameData::m_pGUI->createDesktopA("menu_settings", "menu/settings.html");
			GameData::m_pGUI->pushDesktop(pSettingsDesktop);
		});
		m_pGUI->registerCallback("engine_command", [](gui::IEvent * ev){
			Core_0ConsoleExecCmd("%s", String(ev->target->getAttribute(L"args")).c_str());
		});
		m_pGUI->registerCallback("dial_settings_video", [](gui::IEvent * ev){
			static gui::IDesktop * pLoadLevelDesktop = GameData::m_pGUI->createDesktopA("menu_settings_video", "menu/settings_video.html");
			GameData::m_pGUI->pushDesktop(pLoadLevelDesktop);

			gui::dom::IDOMdocument * doc = pLoadLevelDesktop->getDocument();

			gui::dom::IDOMnode * pNode = doc->getElementById(L"modes_list");
			while(pNode->getChilds()->size() > 1)
			{
				pNode->removeChild((*(pNode->getChilds()))[1]);
			}
		
			int iModesCount = 0;
			const DEVMODE * pModes = SGCore_GetModes(&iModesCount);

			for(int i = 0; i < iModesCount; ++i)
			{
				wchar_t str[64];
				wsprintfW(str, L"<option value=\"%d|%d\">%dx%d</option>", pModes[i].dmPelsWidth, pModes[i].dmPelsHeight, pModes[i].dmPelsWidth, pModes[i].dmPelsHeight);

				gui::dom::IDOMnodeCollection newItems = pLoadLevelDesktop->createFromText(str);
				for(UINT i = 0, l = newItems.size(); i < l; i++)
				{
					pNode->appendChild(newItems[i]);
				}
			}

			UpdateSettingsDesktop();
		});

		m_pGUI->registerCallback("settings_commit", [](gui::IEvent * ev){

			CSettingsWriter settingsWriter;
			settingsWriter.loadFile("../config_game_user_auto.cfg");

			gui::IDesktop * pSettingsDesktop = GameData::m_pGUI->getActiveDesktop();
			gui::dom::IDOMdocument * doc = pSettingsDesktop->getDocument();
			const gui::dom::IDOMnodeCollection &items = *doc->getElementsByClass(L"set_item");

			for(int i = 0, l = items.size(); i < l; ++i)
			{
				gui::dom::IDOMnode * pNode = items[i];

				const StringW &sAttr = pNode->getAttribute(L"cvar");
				const StringW &sAttrVal = pNode->getAttribute(L"value");
				if(sAttr.length())
				{
					String s(sAttr);
					String sval(sAttrVal);

					int iCvars = parse_str((char*)s.c_str(), NULL, 0, '|');
					char ** parts = (char**)alloca(sizeof(char*)*iCvars);
					char ** partsVal = (char**)alloca(sizeof(char*)*iCvars);
					iCvars = parse_str((char*)s.c_str(), parts, iCvars, '|');
					iCvars = parse_str((char*)sval.c_str(), partsVal, iCvars, '|');

					StringW sValue;

					for(int j = 0; j < iCvars; ++j)
					{
						settingsWriter.setCVar(parts[j], partsVal[j]);
						Core_0ConsoleExecCmd("%s %s", parts[j], partsVal[j]);
						printf("New: '%s' = '%s'\n", parts[j], partsVal[j]);
					}
				}
			}

			settingsWriter.saveFile("../config_game_user_auto.cfg");

			GameData::m_pGUI->popDesktop();
		});
		m_pGUI->registerCallback("controls_commit", [](gui::IEvent * ev){

			CSettingsWriter settingsWriter;
			settingsWriter.loadFile("../config_game_user_auto.cfg");

			gui::IDesktop * pSettingsDesktop = GameData::m_pGUI->getActiveDesktop();
			gui::dom::IDOMdocument * doc = pSettingsDesktop->getDocument();
			auto pItems = doc->getElementsByClass(L"cctable_row");
			if(pItems)
			{
				const gui::dom::IDOMnodeCollection &items = *pItems;

				for(int i = 0, l = items.size(); i < l; ++i)
				{
					gui::dom::IDOMnode * pNode = items[i];

					const StringW &wsCmd = pNode->getAttribute(L"cmd");
					if(wsCmd.length())
					{
						auto pChilds = pNode->getChilds();
						for(int j = 0, jl = pChilds->size(); j < jl; ++j)
						{
							gui::dom::IDOMnode * pCell = (*pChilds)[j];
							const StringW &wsKey = pCell->getAttribute(L"key");
							if(wsKey.length())
							{
								String sCmd(wsCmd);
								String sKey(wsKey);

								settingsWriter.setBind(sKey.c_str(), sCmd.c_str());
								Core_0ConsoleExecCmd("bind %s %s", sKey.c_str(), sCmd.c_str());
								printf("Bind: '%s' = '%s'\n", sKey.c_str(), sCmd.c_str());
							}
							const StringW &wsOldKey = pCell->getAttribute(L"key_del");
							if(wsOldKey.length())
							{
								String sKey(wsOldKey);

								settingsWriter.unBind(sKey.c_str());
								Core_0ConsoleExecCmd("unbind %s", sKey.c_str());
								printf("Unbind: '%s'\n", sKey.c_str());
							}
						}
					}
				}
			}


			settingsWriter.saveFile("../config_game_user_auto.cfg");

			GameData::m_pGUI->popDesktop();
		});

		m_pGUI->registerCallback("dial_settings_controls", [](gui::IEvent * ev){
			static gui::IDesktop * pControlsDesktop = GameData::m_pGUI->createDesktopA("menu_settings_controls", "menu/settings_controls.html");
			GameData::m_pGUI->pushDesktop(pControlsDesktop);

			gui::dom::IDOMdocument * doc = pControlsDesktop->getDocument();



			gui::dom::IDOMnode * pNode = doc->getElementById(L"cctable_items");
			while(pNode->getChilds()->size() > 0)
			{
				pNode->removeChild((*(pNode->getChilds()))[0]);
			}

			char path[1024];
			sprintf(path, "%s/game/controls.cfg", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS));
			ISXConfig *pConfig = Core_OpConfig(path);

			const char *szSectionName;
			const char *szText;
			const char *szCmd;
			const char *szKey[2];
			const char *szBindKey, *szBindCmd;
			char szKeyName[64];
			wchar_t str[1024];

			StringW wsCmd, wsText, wsKey0, wsKey1;
			for(int i = 0, l = pConfig->getSectionCount(); i < l; ++i)
			{
				szSectionName = pConfig->getSectionName(i);
				szText = pConfig->getKey(szSectionName, "__name__");
				if(!szText)
				{
					szText = "";
				}

				wsprintfW(str, L"<div class=\"cctable_section\">%s</div>", StringW(String(szText)).c_str());
				pNode->appendHTML(str);

				for(int j = 0, jl = pConfig->getKeyCount(szSectionName); j < jl; ++j)
				{
					sprintf(szKeyName, "cmd_%d", j);
					szCmd = pConfig->getKey(szSectionName, szKeyName);
					sprintf(szKeyName, "txt_%d", j);
					szText = pConfig->getKey(szSectionName, szKeyName);

					if(!szCmd || !szText)
					{
						continue;
					}

					szKey[0] = szKey[1] = "";
					int iKeyIdx = 0;
					for(int k = 0, kl = SSInput_GetKeymapSize(); k < kl; ++k)
					{
						SSInput_GetBindEntry(k, &szBindKey, &szBindCmd);
						if(szBindCmd && !fstrcmp(szBindCmd, szCmd))
						{
							szKey[iKeyIdx++] = szBindKey;
							if(iKeyIdx >= 2)
							{
								break;
							}
						}
					}

					wsCmd = StringW(String(szCmd));
					wsText = StringW(String(szText));
					wsKey0 = StringW(String(szKey[0]));
					wsKey1 = StringW(String(szKey[1]));

					wsprintfW(str, L"<div class=\"cctable_row\" cmd=\"%s\">"
						L"<div class=\"cctable_left cctable_cell\">%s</div>"
						L"<div class=\"cctable_ctl0 cctable_cell\" key=\"%s\" onclick=\"settings_ctl_key\">%s</div>"
						L"<div class=\"cctable_ctl1 cctable_cell\" key=\"%s\" onclick=\"settings_ctl_key\">%s</div>"
						L"</div>", wsCmd.c_str(), wsText.c_str(), wsKey0.c_str(), wsKey0.c_str(), wsKey1.c_str(), wsKey1.c_str());
					pNode->appendHTML(str);
				}
			}

			mem_release(pConfig);
		});

		m_pGUI->registerCallback("bind_del", [](gui::IEvent * ev){
			if(ev->key == KEY_LBUTTON)
			{
				StringW wsKey;

				GameData::m_pCell->setText(wsKey, TRUE);
				StringW wsOldKey = GameData::m_pCell->getAttribute(L"key");
				GameData::m_pCell->setAttribute(L"key", wsKey);
				GameData::m_pCell->setAttribute(L"key_del", wsOldKey);

				GameData::m_pCell = NULL;
				GameData::m_pGUI->popDesktop();
			}
		});

		m_pGUI->registerCallback("bind_cancel", [](gui::IEvent * ev){
			if(ev->key == KEY_LBUTTON)
			{
				GameData::m_pCell = NULL;
				GameData::m_pGUI->popDesktop();
			}
		});
	
		m_pGUI->registerCallback("settings_ctl_key", [](gui::IEvent * ev){
			if(ev->key != KEY_LBUTTON)
			{
				return;
			}
			GameData::m_pGUI->messageBox(
				StringW(String("Переназначить")).c_str(), 
				StringW(String("Нажмите клавишу, которую вы хотите назначить для данного действия."
				"Для отмены нажмите ESC")).c_str(),
				StringW(String("Удалить")).c_str(),
				L"bind_del",
				StringW(String("Отмена")).c_str(),
				L"bind_cancel",
				NULL
			);

			GameData::m_pCell = ev->target;

			SSInput_OnNextKeyPress([](const char *szKey){
				if(!GameData::m_pCell || !fstrcmp(szKey, "escape"))
				{
					return;
				}
				printf("%s\n", szKey);
				GameData::m_pGUI->popDesktop();

				StringW wsNewKey = StringW(String(szKey));



				gui::IDesktop * pSettingsDesktop = GameData::m_pGUI->getActiveDesktop();
				gui::dom::IDOMdocument * doc = pSettingsDesktop->getDocument();
				auto pItems = doc->getElementsByClass(L"cctable_row");
				if(pItems)
				{
					const gui::dom::IDOMnodeCollection &items = *pItems;

					for(int i = 0, l = items.size(); i < l; ++i)
					{
						gui::dom::IDOMnode * pNode = items[i];
						const StringW &wsCmd = pNode->getAttribute(L"cmd");
						if(wsCmd.length())
						{
							auto pChilds = pNode->getChilds();
							for(int j = 0, jl = pChilds->size(); j < jl; ++j)
							{
								gui::dom::IDOMnode * pCell = (*pChilds)[j];
								const StringW &wsKey = pCell->getAttribute(L"key");
								if(wsKey.length() && wsKey == wsNewKey)
								{
									pCell->setText(L"", TRUE);;
									pCell->setAttribute(L"key", L"");
								}
								const StringW &wsOldKey = pCell->getAttribute(L"key_del");
								if(wsOldKey.length() && wsOldKey == wsNewKey)
								{
									pCell->setAttribute(L"key_del", L"");
								}
							}
						}
					}
				}




				GameData::m_pCell->setText(wsNewKey, TRUE);
				GameData::m_pCell->setAttribute(L"key", wsNewKey);
			});
		});
	}

	//gui::IDesktop * pDesk = m_pGUI->createDesktopA("ingame", "ingame.html");
	//gui::IDesktop * pDesk = m_pGUI->createDesktopA("ingame", "main_menu.html");
	//m_pGUI->setActiveDesktop(pDesk);

	//m_pPlayer->setModel("models/stalker_zombi/stalker_zombi_a.dse");
	//m_pPlayer->playAnimation("reload");

	//pl = SXAnim_CreatePlayer("models/stalker_zombi/stalker_zombi_a.dse");
	//pl->setPos(float3(0, 0, 0));
	//g_pRagdoll = new CRagdoll(pl);
	//pl->setRagdoll(g_pRagdoll);

	//m_pStatsUI = m_pGUI->createDesktopA("stats", "sys/stats.html");

	if(m_isGame)
	{
		CBaseTool *pTool = (CBaseTool*)CREATE_ENTITY("weapon_ak74", m_pMgr);
		pTool->setOwner(m_pPlayer);
		pTool->attachHands();
		pTool->playAnimation("idle");
		pTool->setPos(m_pPlayer->getHead()->getPos() + float3(1.0f, 0.0f, 1.0f));
		pTool->setOrient(m_pPlayer->getHead()->getOrient());
		pTool->setParent(m_pPlayer->getHead());

		CBaseAmmo *pAmmo = (CBaseAmmo*)CREATE_ENTITY("ammo_5.45x39ps", m_pMgr);
		pTool->chargeAmmo(pAmmo);

		m_pPlayer->getInventory()->putItems("ammo_5.45x39ps", 60);

		CBaseMag *pMag = (CBaseMag*)CREATE_ENTITY("mag_ak74_30", m_pMgr);
		pMag->load(30);
		((CBaseWeapon*)pTool)->attachMag(pMag);

		m_pPlayer->setActiveTool(pTool);
	}
	else
	{
		m_pMgr->setEditorMode(true);
	}
}
GameData::~GameData()
{
	//mem_delete(g_pRagdoll);

	mem_delete(m_pGameStateManager);
	mem_delete(m_pHUDcontroller);

	if(m_pGUI)
	{
		m_pGUI->release();
		m_pGUI = NULL;
	}

	mem_delete(g_pTracer);
	mem_delete(g_pTracer2);
	mem_delete(m_pMgr);

	for(int i = 0; i < MPT_COUNT; ++i)
	{
		// @TODO: SSCore_SndDelete3dInst()
		mem_delete_a(m_pidFootstepSound[i]);
	}
}

void GameData::update()
{
	if(m_pCrosshair)
	{
		m_pCrosshair->update();
	}
	if(m_pGUI)
	{
		m_pGUI->update();
	}

	if(m_pHUDcontroller)
	{
		static const bool * pbHudRangeFinder = GET_PCVAR_BOOL("hud_rangefinder");
		if(*pbHudRangeFinder)
		{
			float fRange = m_pPlayer->getAimRange();
			m_pHUDcontroller->setAimRange(fRange);
		}
		else
		{
			m_pHUDcontroller->setAimRange(-1.0f);
		}
		m_pHUDcontroller->update();
	}
	/*
	float3 start(-10.0f, 100.0f, -10.0f),
		end = start + float3(10.0f, -200.0f, 10.0f);
	
	DWORD t0 = GetTickCount();
	for(int i = 0; i < 60000; ++i)
	{
		btCollisionWorld::ClosestRayResultCallback cb(F3_BTVEC(start), F3_BTVEC(end));
		SPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);
	}
	DWORD t1 = GetTickCount();
	printf(COLOR_LRED "TIME: %.3fs\n" COLOR_RESET, (float)(t1 - t0) / 1000.0f);*/
}
void GameData::render()
{
	//g_pTracer->render();
	//g_pTracer2->render();
	const bool * pbHudDraw = GET_PCVAR_BOOL("hud_draw");
	if(*pbHudDraw && m_pGUI)
	{
		m_pGUI->render();
	}
	//m_pStatsUI->render(0.1f);
}
void GameData::renderHUD()
{
	static const bool * pbHudCrosshair = GET_PCVAR_BOOL("hud_crosshair");
	if(*pbHudCrosshair && m_pCrosshair)
	{
		m_pCrosshair->render();
	}
}
void GameData::sync()
{
	if(m_pCrosshair)
	{
		m_pCrosshair->onSync();
	}
	if(m_pGUI)
	{
		m_pGUI->syncronize();
	}
}

void GameData::playFootstepSound(MTLTYPE_PHYSIC mtl_type, const float3 &f3Pos)
{
	int iCount = m_iFootstepSoundCount[mtl_type];
	if(!iCount)
	{
		return;
	}
	ID idSound = m_pidFootstepSound[mtl_type][rand() % iCount];
	SSCore_SndInstancePlay3d(idSound, false, false, (float3*)&f3Pos);
}

void GameData::loadFoostepsSounds()
{
	Array<const char*> aSounds[MPT_COUNT];

	aSounds[MTLTYPE_PHYSIC_CONCRETE].push_back("actor/step/default1.ogg");
	aSounds[MTLTYPE_PHYSIC_CONCRETE].push_back("actor/step/default2.ogg");
	aSounds[MTLTYPE_PHYSIC_CONCRETE].push_back("actor/step/default3.ogg");
	aSounds[MTLTYPE_PHYSIC_CONCRETE].push_back("actor/step/default4.ogg");

	aSounds[MTLTYPE_PHYSIC_METAL].push_back("actor/step/metal_plate1.ogg");
	aSounds[MTLTYPE_PHYSIC_METAL].push_back("actor/step/metal_plate2.ogg");
	aSounds[MTLTYPE_PHYSIC_METAL].push_back("actor/step/metal_plate3.ogg");
	aSounds[MTLTYPE_PHYSIC_METAL].push_back("actor/step/metal_plate4.ogg");

	aSounds[MTLTYPE_PHYSIC_TREE].push_back("actor/step/new_wood1.ogg");
	aSounds[MTLTYPE_PHYSIC_TREE].push_back("actor/step/new_wood2.ogg");
	aSounds[MTLTYPE_PHYSIC_TREE].push_back("actor/step/new_wood3.ogg");
	aSounds[MTLTYPE_PHYSIC_TREE].push_back("actor/step/new_wood4.ogg");

	aSounds[MTLTYPE_PHYSIC_GROUD_SAND].push_back("actor/step/earth1.ogg");
	aSounds[MTLTYPE_PHYSIC_GROUD_SAND].push_back("actor/step/earth2.ogg");
	aSounds[MTLTYPE_PHYSIC_GROUD_SAND].push_back("actor/step/earth3.ogg");
	aSounds[MTLTYPE_PHYSIC_GROUD_SAND].push_back("actor/step/earth4.ogg");

	aSounds[MTLTYPE_PHYSIC_WATER].push_back("actor/step/t_water1.ogg");
	aSounds[MTLTYPE_PHYSIC_WATER].push_back("actor/step/t_water2.ogg");

	aSounds[MTLTYPE_PHYSIC_LEAF_GRASS].push_back("actor/step/grass1.ogg");
	aSounds[MTLTYPE_PHYSIC_LEAF_GRASS].push_back("actor/step/grass2.ogg");
	aSounds[MTLTYPE_PHYSIC_LEAF_GRASS].push_back("actor/step/grass3.ogg");
	aSounds[MTLTYPE_PHYSIC_LEAF_GRASS].push_back("actor/step/grass4.ogg");

	//aSounds[MTLTYPE_PHYSIC_GLASS].push_back("actor/step/.ogg");
	//aSounds[MTLTYPE_PHYSIC_PLASTIC].push_back("actor/step/.ogg");
	//aSounds[MTLTYPE_PHYSIC_FLESH].push_back("actor/step/.ogg");

	for(int i = 0; i < MPT_COUNT; ++i)
	{
		Array<const char*> *paSounds = &aSounds[i];
		int jl = paSounds->size();
		m_iFootstepSoundCount[i] = jl;
		m_pidFootstepSound[i] = jl ? new ID[jl] : NULL;
		for(int j = 0; j < jl; ++j)
		{
			m_pidFootstepSound[i][j] = SSCore_SndCreate3dInst(paSounds[0][j], SX_SOUND_CHANNEL_GAME, 100);
		}
	}
}

//###################################################################

void GameData::ccmd_forward_on()
{
	m_pPlayer->move(PM_FORWARD, TRUE);
}
void GameData::ccmd_forward_off()
{
	m_pPlayer->move(PM_FORWARD, FALSE);
}

void GameData::ccmd_backward_on()
{
	m_pPlayer->move(PM_BACKWARD, TRUE);
}
void GameData::ccmd_backward_off()
{
	m_pPlayer->move(PM_BACKWARD, FALSE);
}

void GameData::ccmd_left_on()
{
	m_pPlayer->move(PM_LEFT, TRUE);
}
void GameData::ccmd_left_off()
{
	m_pPlayer->move(PM_LEFT, FALSE);
}

void GameData::ccmd_right_on()
{
	m_pPlayer->move(PM_RIGHT, TRUE);
}
void GameData::ccmd_right_off()
{
	m_pPlayer->move(PM_RIGHT, FALSE);
}

void GameData::ccmd_boost_on()
{
	m_pPlayer->move(PM_RUN, TRUE);
}
void GameData::ccmd_boost_off()
{
	m_pPlayer->move(PM_RUN, FALSE);
}

void GameData::ccmd_crouch_on()
{
	m_pPlayer->move(PM_CROUCH, TRUE);
}
void GameData::ccmd_crouch_off()
{
	m_pPlayer->move(PM_CROUCH, FALSE);
}

void GameData::ccmd_crawl_on()
{
	m_pPlayer->move(PM_CRAWL, TRUE);
}
void GameData::ccmd_crawl_off()
{
	m_pPlayer->move(PM_CRAWL, FALSE);
}

void GameData::ccmd_jump_on()
{
	m_pPlayer->move(PM_JUMP, TRUE);
}
void GameData::ccmd_jump_off()
{
	m_pPlayer->move(PM_JUMP, FALSE);
}

void GameData::ccmd_attack_on()
{
	m_pPlayer->attack(TRUE);
}
void GameData::ccmd_attack_off()
{
	m_pPlayer->attack(FALSE);
}

void GameData::ccmd_attack2_on()
{
	m_pPlayer->attack2(TRUE);
}
void GameData::ccmd_attack2_off()
{
	m_pPlayer->attack2(FALSE);
}

void GameData::ccmd_spawn()
{
	Core_0ConsoleExecCmd("cl_mode_editor 0");
	m_pPlayer->spawn();
}
void GameData::ccmd_observe()
{
	m_pPlayer->observe();
}

void GameData::ccmd_reload()
{
	m_pPlayer->reload();
}

void GameData::ccmd_toggleflashlight()
{
	m_pPlayer->toggleFlashlight();
}

void GameData::ccmd_switch_firemode()
{
	m_pPlayer->nextFireMode();
}

void GameData::ccmd_game_menu()
{
	m_pGameStateManager->activate("ingame_menu");
}

void GameData::ccmd_use_on()
{
	m_pPlayer->use(TRUE);
}
void GameData::ccmd_use_off()
{
	m_pPlayer->use(FALSE);
}
