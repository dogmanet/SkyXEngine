
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

//#############################################################################

/*! \page engine_cvar Движковые квары
\tableofcontents
\section managed_render Managed render
 \subsection pp Post process
 - \b pp_ssao установка качества ssao (0 - не рисовать, 1 - среднее, 2 - хорошее, 3 - высокое)
 - \b pp_bloom рисовать ли эффект bloom (true/false)
 - \b pp_lensflare true рисовать ли эффект lens flare (true/false)
 - \b pp_lensflare_usebloom при рендере эффекта lens flare использовать ли данные от прохода эффекта bloom (true/false)
 - \b pp_dlaa рисовать ли эффект dlaa (true/false)
 - \b pp_nfaa рисовать ли эффект nfaa (true/false)
 - \b pp_motionblur рисовать ли эффект motion blur (true/false)
 - \b pp_motionblur_coef коэфициент размытия эффекта motion blur [0.0, 1.0]

 \subsection light Light
 - \b pssm_4or3 для рисования глобальных теней использовать 4 сплита (true) или 3 (false)
 - \b pssm_q коэфициент увеличения карты глубины для глобальных теней относительно размеров окна рендера [0.5, 2]
 - \b lsm_q коэфициент увеличения карты глубины для локальных теней относительно размеров окна рендера [0.5, 2]
 - \b shadow_soft качество сглаживания теней (0 - без сглаживания, 1 - один проход, 2 - двойное сглаживание)
 - \b hdr_adapted_coef коэфициент адаптации глаза к освещению (0,1] (медленно,быстро]

 \subsection green Растительность
 - \b grass_frec плотность рендера травы (0, 100]
 - \b green_lod0 дистанция от наблюдателя на которой заканчивается нулевой лод растительности (он же лод травы), то есть дальность отрисовки травы (0,100)
 - \b green_lod1 дистанция от наблюдателя на которой заканчивается первый лод растительности (50,150)
 - \b green_less дистанция от наблюдателя c которой начнет уменьшаться трава (0,green_lod0)

 \subsection other Прочие 
 - \b r_s_filter фильтрация текстур (0 - point, 1 - linear, 2 - anisotropy)
 - \b r_s_max_anisotropy значение анизитропной фильтрации [0,16]
 - \b r_s_max_miplevel максимально возможный mip уровень текстур [0, #SXGC_LOADTEX_COUNT_MIPMAP]
 - \b p_far дальность видимости (отрисовки мира)

\page editors Редакторы
\link material_editor SXMaterialEditor - редактор материалов \endlink \n 
\link particles_editor SXParticlesEditor - редактор партиклов \endlink \n
*/

//#############################################################################

#include <windows.h>
#include <ctime>
#include <gdefines.h>
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
#pragma comment(lib, "sxaigrid_d.lib")
#else
#pragma comment(lib, "sxaigrid.lib")
#endif
#include <aigrid/sxaigrid.h>

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

//#############################################################################

#include <managed_render\\handler_out_log.cpp>
#include <managed_render\\gdata.h>
#include <managed_render\\camera_update.h>
#include <managed_render\\render_func.h>
#include <managed_render\\level.h>

#if defined(SX_GAME)
#include <SkyXEngine_Build/resource.h>
#endif

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

#include <managed_render\\render_func.h>
#include <managed_render\\gdata.cpp>
#include <managed_render\\camera_update.cpp>
#include <managed_render\\render_func.cpp>
#include <managed_render\\level.cpp>


//#############################################################################

//! инициализация движка
void SkyXEngine_Init();
void SkyXEngine_InitPaths();

//! рендер
void SkyXEngine_Render(DWORD timeDelta);

//! запуск основного цикла обработки
int SkyXEngine_CycleMain();

//! уничтожение данных движка, освобождение памяти
void SkyXEngine_Kill();

//! обработчик для превьюокна
LRESULT CALLBACK SkyXEngine_PreviewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//! создание превью окна
void SkyXEngine_PreviewCreate();

//! уничтожение превью окна
void SkyXEngine_PreviewKill();