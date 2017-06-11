
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*! 
\file
Заголовочный файл движка SkyXEngine, подключает все необходимые библиотеки
*/

/*!
\mainpage Документация SkyXEngine
Это документация API (интерфейса программирования приложения) 3D движка real-time рендера SkyXEngine. \n
Для того чтобы понять что такое SkyXEngine рекомендуем Вам ознакомится с \ref info_engine "кратким описанием движка". \n
Также было бы крайне полезным узнать \ref general_info_libs "об организации библиотек". \n
Дальнейшее ознакомление с движком SkyXEngine можно по <a href="./modules.html">документации, которая представлена модулями/подмодулями</a>.
*/

/*! \page info_engine Информация о движке SkyXEngine
\tableofcontents
\section ie_common Общее
SkyXEngine - графический 3D движок с real-time рендером, использует технологии DirectX 9. \n
<small>Сразу уточнение, DirectX 9 мы используем в связи с личными предпочтениями, так как на наш взгляд эта технология является одной из лучших, хоть и считается устаревшей. 
Все новое это забытое старое)) И как нам кажется ... нет ничего такого, чего нельзя было бы сделать на DirectX 9, но что можно сделать на другом GAPI, для создания игр любого жанра с real-time рендером.
Но это только точка зрения нашей команды и она может быть ошибочная. Однако в планах есть расширение поддерживаемых GAPI.</small>

<b>Формирование уровня</b> осуществляется посредством:
 - загрузки статических моделей формата dse, с их автоматическим разбиением на "куски рендера" как quad или octo дерево, возможны сохранение/загрузка в бинарный файл всей геометрии уровня
 - генерации растительности по маске, возможны 2 вида: трава (чем дальше тем меньше размер объекта) и деревья (с возможностью загрузки лодов), возможны возможны сохранение/загрузка в бинарный файл всей растительности уровня.

<b>Освещение</b> представлено 3 видами источников света (опционально поддерживается генерация теней от каждого исчтоника света):
 - глобальный (солнце), тени - PSSM
 - локальный - точечный, тени - Cube Shadow Mapping
 - локальный - направленный? тени - Shadow Mapping. 

HDR эффект присутсвует.

<b>Система материалов</b> pbr, то есть построенная на физичеки корретных, но апроксимированных вычислениях. \n
Для формирования данных для освещения используется техника Deferred shading, что позволяет обрабатывать неограниченное количество источников света. \n
Поддерживаются отражения 2 видов:
 - плоские (планарные)
 - объемные (кубические). 

Поддерживается до 3 полупрозрачных поверхностей в кадре (пока только тестовый режим, в планах улучшения качества). \n
Поддерживаемые эффекты поверхностей:
 - микрорельеф
 - микрорельеф по маске (до 4 текстур)
 - детальность
 - детальность по маске (до 4 текстур)
 - комбинированный микрорельеф с детальностью по маске (до 4 текстур)
 - альфа тест.

 Вся система материалов является открытой и практически любой функционал системы доступен для собственной интерпретации, посредством шейдеров. Имеется встроенный набор данных.\n
 Поддерживаются различные пользовательские данные которые интерпретируются только создающим материалы.

<b>Постпроцесс</b> состоит из эффектов:
 - черно-белое изображение
 - эффект сепия
 - коррекция изображения
 - рендер солнца
 - bloom
 - lens flare, эффект восприятия яркого света линзами
 - depth of field, глубина резкости
 - linear fog, линейный туман
 - space screen ambient occulusion, глобальное освещение (точнее затенение) в пространстве экрана
 - motion blur, размытие в движении
 - nfaa
 - dlaa.


\section dogma_engine Идеология движка
Основной технической идеей при разработке движка была идея о том что программист должен иметь контроль над объектами которыми он оперирует, НО этот контроль должен быть в меру. \n
Основной идеей предназначения служила и служит идея о том чтобы предоставляемый инструментарий мог полностью удовлетворять потребностям разработчика, без необходимости со стороны разработчика вникать в детали реализации, но чтобы этот инструментарий имел прозрачную, открытую, свободную, бесплатную лицензию, и разработчик мог спокойно податься в стихию сотворения своего мира. \n
Также главенствовал принцип разделения функционала на логические блоки (библиотеки) и после окончания разработки очередного блока он бы выносился в dll (с глаз долой). 
Однако существенным дополнением являлась идея о том что библиотеки не должны ничего значть о равнозначных себе библиотеках в общей иерархии.
К примеру \ref sxgeom "библиотека статической геометрии и растительности" ничего не знает о \ref sxmtllight "библиотеке материалов", и последняя ничего не знает о первой, однако первая использует функции из второй посредством графического ядра, а вторая осуществляется настройку материалов перед рендером первой.\n
Немаловажным моментом являлся ориентир на гибкость. К примеру  \ref sxgcore_redefinition_func "переопредляемые функции" в графическом ядре, позволяют настроить на свой лад рендер. \n
Ну и пожалуй самая главная идея заключается в полноценной возможности разработки игр с видом от первого лица, с настройкой/перестройкой "под себя" всего рендера. Также к приправе к этому мы руководствовались мыслью о том что скриптеры (коих не мало) могли бы вести разработку игры на своем (доступном им) уровне, со всеми возможностями движка.

\section struct_engine Структура движка
Весь движок состоит из \ref general_info_libs "подсистем", которые в некоторых случаях являются ядрами. \n
Иерархия подсистем:\n
\- \ref sxcore\n
\-- \ref sxgcore\n
\--- \ref sxgeom\n
\--- \ref sxanim\n
\--- \ref sxmtllight\n
\--- \ref sxpp\n
\- \ref sxinput\n
Все библиотеки представлены файлами dll, что позволяет разделить функционал и реализацию. Также в большинстве случаев библиотеки могут быть вычленены из самого движка и могут использоваться автономно.\n
В большинстве случаев весь функционал библиотек предоставляется интерфейсом функций, а идентификация объектов числовая, по типу #ID.
Это позволяет не беспокоится о типах и о существовании объектов - об этом заботятся сами библиотеки.
*/

/*! \page std_doc Стандарт документирования
\tableofcontents
\section std_doc_common Общее
Дабы создать удобочитаемые комментрии, которые легко бы трансформировались в документацию был создан данный стандарт документирования, следование которому является обязательным.
Так как для документирования был выбран Doxygen, то все что касается документирования будет в контексте именно этой программы. \n
Для более чектого структурирования  необходимо разделять весь код на логические звенья, которые могут быть представлены модулями/подмодулями и группами как в модулях так и вне их. 

\section style_doc Стиль документирования
QT стиль документирования (!) и QT_AUTOBRIEF - короткая документация до точки в любом комментарии, а после точки идет подробное документирование если надо.\n
Возможные виды:
<pre>
//! документирование того что идет за этой строкой 
//!< документирование того что предшествовало данному комментарию
/ *! документирование того что идет за этой строкой * /
/ *!< документирование того что предшествовало данному комментарию * /
</pre>
<b>Примечание:</b> здесь и далее / * и * / комментарий без пробелов!

\section style_desc_file Описание файла (обязательно для каждого документируемого файла)
<pre>
/ *!
\\file
Описание файла
\* /
</pre>

\section style_modules Модули/подмодули
<pre>
/ *! \\defgroup имя_модуля описание модуля
\@{
\* /
... код модуля
//!\@} имя_модуля
</pre>
В случае если модуль является подмодулем другого модуля то после описания модуля необходимо добавить:
<pre>
 \\ingroup имя_модуля_которому_будет_принадлежать_данный_модуль
</pre>
Окончание модуля может быть любым удобным, но только окончание модуля должно включать в себя имя модуля для которого написан конец.

\section style_group Группа (вложенные группы не поддерживаются)
<pre>
/ *! \\name название группы, оно также будет размещено в документации
а здесь пишем описание к группе если надо
\* /
//! \@{
... все кроме классов
//! \@}
</pre>

\section style_ref Ссылки
Для лучшей навигации необходимо использовать ссылки, которые могут указывать на структурные единицы (модули/подмодули, страницы, секции) так и на код.
Пример ссылки на структурные единицы:
<pre>
Весь движок состоит из \\ref general_info_libs "подсистем".
</pre>
Или без указания имени ссылки:
<pre>
\\ref sxcore
</pre>

Пример ссылки на код:
<pre>
\#SCore_Create
</pre>

\section style_page Страницы
<pre>
/ *! \\page идентификатор_только_латиница Заголовок страницы
\\tableofcontents
Для того чтобы разделить на разделы можно использовать секции:
\\section style_page Создание страниц
\* /
</pre>

\section style_list Списки
<pre>
 \- первый элемент
 \- второй элемент
</pre>

\section style_extra Дополнительно
\\n - для новой строки \n
Также можно использовать html теги. \n
\\note - заметка  \n
\\warning - предупреждение  \n
\\todo - пометка о том что надо сделать  \n
*/

#include <windows.h>
#include <ctime>
#include <gdefines.h>
#include <cdata.h>
#include <common\\array.h>
#include <common\\string.cpp>

//ЗАГРУЗКА БИБЛИОТЕК
//{
#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxinput_d.lib")
#else
#pragma comment(lib, "sxinput.lib")
#endif
#include <input\\sxinput.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxscore_d.lib")
#else
#pragma comment(lib, "sxscore.lib")
#endif
#include <score/sxscore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgeom_d.lib")
#else
#pragma comment(lib, "sxgeom.lib")
#endif
#include <geom\\sxgeom.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxmtllight_d.lib")
#else
#pragma comment(lib, "sxmtllight.lib")
#endif
#include <mtllight\\sxmtllight.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxparticles_d.lib")
#else
#pragma comment(lib, "sxparticles.lib")
#endif
#include <particles\\sxparticles.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxpp_d.lib")
#else
#pragma comment(lib, "sxpp.lib")
#endif
#include <pp\\sxpp.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxanim_d.lib")
#else
#pragma comment(lib, "sxanim.lib")
#endif
#include <anim/sxanim.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxphysics_d.lib")
#else
#pragma comment(lib, "sxphysics.lib")
#endif
#include <physics/sxphysics.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxdecals_d.lib")
#else
#pragma comment(lib, "sxdecals.lib")
#endif
#include <decals/sxdecals.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgame_d.lib")
#else
#pragma comment(lib, "sxgame.lib")
#endif
#include <game/sxgame.h>

#if defined(SX_LEVEL_EDITOR) || defined(SX_MATERIAL_EDITOR) || defined(SX_PARTICLES_EDITOR)
#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <sxguiwinapi\\sxgui.h>
#endif

//}

#include <managed_render\\gdata.h>
#include <common\\string_api.cpp>
#include <managed_render\\camera_update.h>
#include <managed_render\\render_func.h>
#include <managed_render\\level.h>

#if defined(SX_LEVEL_EDITOR)
#include <SXLevelEditor/resource.h>
#include <SXLevelEditor\\level_editor.cpp>
#endif

#if defined(SX_MATERIAL_EDITOR)
#include <sxmaterialeditor/resource.h>
#include <sxmaterialeditor\\material_editor.cpp>
#endif

#if defined(SX_PARTICLES_EDITOR)
#include <sxparticleseditor/resource.h>
#include <sxparticleseditor\\particles_editor.cpp>
#endif

#include <managed_render\\handler_out_log.cpp>
#include <managed_render\\gdata.cpp>
#include <managed_render\\camera_update.cpp>
#include <managed_render\\render_func.cpp>
#include <managed_render\\level.cpp>


//!< инициализация движка
void SkyXEngine_Init()
{
	srand((UINT)time(0));
	InitOutLog();
	GData::Pathes::InitAllPathes();
	if (!Core_0IsProcessRun("sxconsole.exe"))
		ShellExecute(NULL, "open", "sxconsole.exe", NULL, GData::Pathes::ForExe, SW_SHOWNORMAL);

	SetConsoleTitle("sxconsole");

#if defined(SX_GAME)
	GData::InitWin("SkyXEngine", "SkyXEngine");
#endif

	SSInput_0Create("sxinput", GData::Handle3D, true);
	SSInput_Dbg_Set(printflog);

	Core_0Create("sxcore", true);
	Core_SetOutPtr();

	G_Timer_Render_Scene = Core_TimeAdd();
	G_Timer_Game = Core_TimeAdd();
	Core_RIntSet(G_RI_INT_TIMER_RENDER, G_Timer_Render_Scene);
	Core_RIntSet(G_RI_INT_TIMER_GAME, G_Timer_Game);

	tm ct = {0,0,9,27,5,2030-1900,0,0,0};
	Core_TimeUnixStartSet(G_Timer_Game, mktime(&ct));
	
	Core_TimeWorkingSet(G_Timer_Render_Scene, true);
	Core_TimeWorkingSet(G_Timer_Game, true);

	SSCore_0Create("sxsound", GData::Handle3D, GData::Pathes::Sounds,false);
	SSCore_Dbg_Set(printflog);

	SGCore_0Create("sxgcore", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_ShaderSetStdPath(GData::Pathes::Shaders);

	SGCore_SetFunc_MtlSet(SXRenderFunc::RFuncMtlSet);
	SGCore_SetFunc_MtlLoad(SXRenderFunc::RFuncMtlLoad);
	SGCore_SetFunc_MtlGetSort((g_func_mtl_get_sort)SML_MtlGetTypeTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SML_MtlGetTypeReflection);
	SGCore_SetFunc_MtlGetPhysicType((g_func_mtl_get_physic_type)SML_MtlGetPhysicMaterial);

	SGCore_SkyBoxCr();
	SGCore_SkyBoxSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	SGCore_SkyCloudsCr();
	SGCore_SkyCloudsSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#ifndef SX_GAME
	GData::ObjCamera = SGCore_CrCamera();
#endif

	SGeom_0Create("sxgeom", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);

	SML_0Create("sxml", SGCore_GetDXDevice(), GData::Pathes::Materials, GData::Pathes::Meshes, &GData::WinSize, GData::ProjFov, false);
	SML_Dbg_Set(printflog);

	SPE_0Create("sxparticles", SGCore_GetDXDevice(), false);
	SPE_Dbg_Set(printflog);
	Level::LoadParticles();

	SPP_0Create("sxpp", SGCore_GetDXDevice(), &GData::WinSize, false);
	SPP_Dbg_Set(printflog);

#if defined(SX_GAME)
	SPP_ChangeTexSun("fx_sun.dds");
#endif
	SPP_RTSetInput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com));
	SPP_RTSetOutput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com2));
	SPP_RTSetDepth0(SML_DSGetRT_ID(DS_RT::ds_rt_depth0));
	SPP_RTSetDepth1(SML_DSGetRT_ID(DS_RT::ds_rt_depth1));
	SPP_RTSetNormal(SML_DSGetRT_ID(DS_RT::ds_rt_normal));

	SXAnim_0Create();
	SXAnim_Dbg_Set(printflog);

	SXPhysics_0Create();
	SXPhysics_Dbg_Set(printflog);

	SXDecals_0Create();
	SXDecals_Dbg_Set(printflog);

	SXGame_0Create();
	SXGame_Dbg_Set(printflog);
#ifdef SX_GAME
	GData::ObjCamera = SXGame_GetActiveCamera();
#endif

#if defined(SX_LEVEL_EDITOR) || defined(SX_PARTICLES_EDITOR)
	GData::Editors::ObjGrid = new Grid();
	GData::Editors::ObjGrid->Create(100, 100, D3DCOLOR_ARGB(255, 200, 200, 200));

	GData::Editors::ObjAxesStatic = new AxesStatic();
	GData::Editors::ObjAxesStatic->Create(1);

	GData::ObjCamera->SetPosition(&float3(0, 0.5, -2));

	GData::Editors::RenderGrid = GData::Editors::RenderAxesStatic = true;
#endif

#if defined(SX_PARTICLES_EDITOR)
	GData::Editors::RenderBound = true;

	D3DXCreateBox(GData::DXDevice, 1, 1, 1, &GData::FigureBox, 0);
	D3DXCreateSphere(GData::DXDevice, 1, 20, 20, &GData::FigureSphere, 0);

	GData::FigureConeParam.x = 1;
	GData::FigureConeParam.y = 0.1;
	GData::FigureConeParam.z = 1;

	SGCore_FCreateCone(GData::FigureConeParam.x, GData::FigureConeParam.y, GData::FigureConeParam.z, &GData::FigureCone, 20);

#endif

	GData::InitAllMatrix();

	GData::IDsShaders::InitAllShaders();

	Core_0RegisterConcmd("screenshot", SXRenderFunc::SaveScreenShot);
	Core_0RegisterConcmd("save_worktex", SXRenderFunc::SaveWorkTex);
	Core_0RegisterConcmd("shader_reload", SGCore_ShaderReloadAll);

#if defined(SX_GAME)
	Core_0RegisterConcmd("change_mode_window", SXRenderFunc::ChangeModeWindow);
#endif

	Core_0ConsoleExecCmd("exec ../sysconfig.cfg");
	Core_0ConsoleExecCmd("exec ../userconfig.cfg");

#if !defined(SX_GAME)
	Core_0ConsoleExecCmd("exec ../editor.cfg");
#endif
}

//!< рендер
void SkyXEngine_Render(DWORD timeDelta)
{
	int64_t ttime;
	//потеряно ли устройство или произошло изменение размеров?
	if (GData::DXDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET || GData::ReSize)
	{
		//если не свернуто окно
		if (!IsIconic(GData::Handle3D) && ((GData::HandleParent3D != 0 && !IsIconic(GData::HandleParent3D)) || GData::HandleParent3D == 0))
			SXRenderFunc::ComDeviceLost();	//пытаемся восстановить
		return;
	}

#ifndef SX_GAME
	CameraUpdate::UpdateEditorial(timeDelta);
#endif

	SXAnim_Update();
	SXGame_Update();
	SXPhysics_Update();

	SXAnim_Sync();
	SXPhysics_Sync();
	SXGame_Sync();

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SGeom_ModelsMSortGroups(&GData::ConstCurrCamPos, 2);
	SXRenderFunc::Delay::GeomSortGroup += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	if (GData::DefaultGeomIDArr < 0)
		GData::DefaultGeomIDArr = SGeom_ModelsAddArrForCom();

	if (GData::DefaultGreenIDArr < 0)
		GData::DefaultGreenIDArr = SGeom_GreenAddArrForCom();

	if (GData::DefaultAnimIDArr < 0)
		GData::DefaultAnimIDArr = SXAnim_ModelsAddArrForCom();

	/**/
	if (SGeom_GreenGetOccurencessLeafGrass(&float3(GData::ConstCurrCamPos - float3(0.25, 1.8, 0.25)), &float3(GData::ConstCurrCamPos + float3(0.25, 0, 0.25)), MtlPhysicType::mpt_leaf_grass))
		SXRenderFunc::Delay::FreeVal = 1;
	else
		SXRenderFunc::Delay::FreeVal = 0;
	/**/

	SXRenderFunc::UpdateView();
	SML_Update(timeDelta, &GData::WinSize, &GData::NearFar, &GData::ConstCurrCamPos, &GData::MCamView, GData::ProjFov);

	GData::DXDevice->BeginScene();

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::UpdateReflection(timeDelta);
	SXRenderFunc::Delay::ComReflection += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	if (GData::FinalImage == DS_RT::ds_rt_ambient_diff || GData::FinalImage == DS_RT::ds_rt_specular || GData::FinalImage == DS_RT::ds_rt_scene_light_com)
	{
		//рендерим глубину от света
		ttime = TimeGetMcsU(G_Timer_Render_Scene);
		SXRenderFunc::UpdateShadow(timeDelta);
		SXRenderFunc::Delay::UpdateShadow += TimeGetMcsU(G_Timer_Render_Scene) - ttime;
	}

	//рисуем сцену и заполняем mrt данными
	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::RenderInMRT(timeDelta);
	SXRenderFunc::Delay::RenderMRT += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	if (GData::FinalImage == DS_RT::ds_rt_ambient_diff || GData::FinalImage == DS_RT::ds_rt_specular || GData::FinalImage == DS_RT::ds_rt_scene_light_com)
	{
		//освещаем сцену
		ttime = TimeGetMcsU(G_Timer_Render_Scene);
		SXRenderFunc::ComLighting(timeDelta, true);
		SXRenderFunc::Delay::ComLighting += TimeGetMcsU(G_Timer_Render_Scene) - ttime;
	}

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

#if defined(SX_GAME)
	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::RenderPostProcess(timeDelta);
	SXRenderFunc::Delay::PostProcess += TimeGetMcsU(G_Timer_Render_Scene) - ttime;
#endif

	SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if !defined(SX_GAME)
	GData::DXDevice->SetTexture(0, SML_DSGetRT(GData::FinalImage));
#else
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(SPP_RTGetCurrSend()));
#endif

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	SXRenderFunc::RenderParticles(timeDelta);

	SXRenderFunc::RenderEditorMain();
	SXRenderFunc::RenderEditorLE(timeDelta);

	SXRenderFunc::OutputDebugInfo(timeDelta);

	SXPhysics_DebugRender();
	//SXGame_EditorRender();

	SXRenderFunc::ShaderRegisterData();

	GData::DXDevice->EndScene();

	//@@@
	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::ComVisibleForCamera();
	SXRenderFunc::Delay::UpdateVisibleForCamera += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::ComVisibleReflection();
	SXRenderFunc::Delay::UpdateVisibleForReflection += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::ComVisibleForLight();
	SXRenderFunc::Delay::UpdateVisibleForLight += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SPE_EffectVisibleComAll(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	SPE_EffectComputeAll();
	SPE_EffectComputeLightingAll();
	SXRenderFunc::Delay::UpdateParticles += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	GData::DXDevice->Present(0, 0, 0, 0);
	SXRenderFunc::Delay::Present += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

#if defined(SX_LEVEL_EDITOR)
	GData::Editors::LevelEditorUpdateStatusBar();
#endif

#if defined(SX_PARTICLES_EDITOR)
	GData::Editors::ParticlesEditorUpdateStatusBar();
#endif
}

//!< запуск основного цикла обработки
int SkyXEngine_CycleMain()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static DWORD lastTime = TimeGetMls(G_Timer_Render_Scene);

	while (msg.message != WM_QUIT && IsWindow(GData::Handle3D))
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);

#if !defined(SX_GAME)
			IMSG imsg;
			imsg.lParam = msg.lParam;
			imsg.wParam = msg.wParam;
			imsg.message = msg.message;

			SSInput_AddMsg(imsg);
#endif

			::DispatchMessage(&msg);
		}
		else
		{
			Core_TimesUpdate();
			Core_0ConsoleUpdate();
			SSInput_Update();
			SSCore_Update(&GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
			SGCore_LoadTexLoadTextures();

			if (SSInput_GetKeyEvents(SIK_T) == InputEvents::iv_k_first)
			{
				Core_TimeWorkingSet(G_Timer_Render_Scene, !Core_TimeWorkingGet(G_Timer_Render_Scene));
				/*for (int i = 0; i < 1; ++i)
				{
				ID tmpid = SPE_EffectIdOfKey(i);
				//SPE_EffectPosSet(tmpid, &float3(i, 0, k));
				SPE_EffectAlifeSet(tmpid, !SPE_EffectAlifeGet(tmpid));
				}*/
			}

			DWORD currTime = TimeGetMls(G_Timer_Render_Scene);
			DWORD timeDelta = (currTime - lastTime);

#ifdef SX_GAME
			GData::ObjCamera = SXGame_GetActiveCamera();
#endif

			if (Core_TimeWorkingGet(G_Timer_Render_Scene) && (GetForegroundWindow() == GData::Handle3D || GetForegroundWindow() == FindWindow(NULL, "sxconsole")))
			{

#if defined(SX_LEVEL_EDITOR)
				SXLevelEditor_Transform(10);
#endif

				SkyXEngine_Render(timeDelta);
			}

			lastTime = currTime;
		}
	}

	return msg.wParam;
}

//!< уничтожение данных движка, освобождение памяти
void SkyXEngine_Kill()
{
#if defined(SX_MATERIAL_EDITOR)
	mem_delete(GData::Editors::SimModel);
#endif

#if !defined(SX_GAME)
	mem_delete(GData::Editors::ObjGrid);
	mem_delete(GData::Editors::ObjAxesStatic);
	mem_release(GData::Editors::FigureBox);
	mem_release(GData::Editors::FigureSphere);
	mem_release(GData::Editors::FigureCone);
#endif

	SXGame_0Kill();
	SXDecals_AKill();
	SXPhysics_AKill();
	SXAnim_AKill();
	mem_release(GData::ObjCamera);
	SGeom_AKill();
	SML_AKill();
	SSCore_AKill();
	SGCore_AKill();
	Core_AKill();
}

