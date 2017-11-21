
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Главный файл редактора уровней SXLevelEditor
*/

/*!
\page level_editor Редактор уровней SXLevelEditor
\tableofcontents
\section level_editor_main Общее
\b SXLevelEditor – редактор уровней движка SkyXEngine.\n
Редактор работает с файлом в текстовом формате .lvl который содержит всю информацию об уровне. 
Данный файл должен находится по относительному пути (относительно программы) /gamesource/level/name_level/name_level.lvl
при этом имя директории и имя файла .lvl должны быть идентичными\n

\image html level_editor/level_editor.png "Скриншот программы SXLevelEditor"

\section level_editor_window Структура окна
<b>Окно состоит из структурных элементов (сверху вниз):</b>
 - Заголовок окна, содержащий имя редактора и имя уровня.
 - Меню программы:
  - File:
   - New – очистить сцену и создать новый уроень
   - Open – открыть файл уровня
   - Save – сохранить уровень
   - Save as ... – сохранить уровень как
   - Exit – выход из программы
 - View (редакторные настройки вида):
  - Grid – сетка разметки (оси xz)
  - Axes – линии по направлению осей
 - Final image (выводимое изображение в окно рендера):
  - Color – цвет
  - Normals – нормали
  - Parameters – параметры освещения
  - Ambient - фоновое и диффузное освещение
  - Specular – отраженный свет
  - Lighting scene – освещенная сцена
 - Selection settings (настройки выделения)
  - Selection - надо ли выделять объект
  - Z Test - использовать ли z test при рендере выделения
  - Mesh - показывать сетку или модель целиком при выделении
  - Cull backfaces - показывать задние грани выделения
 - Weather (погода)
  - none - без погоды
  - ниже следует список доступных конфигов погоды
 - Панель инструментов, где расположены основные управляющие кнопки:
  - \image html level_editor/toolbar/new.bmp – очистить сцену и создать новый уроень
  - \image html level_editor/toolbar/open.bmp – открыть файл уровня
  - \image html level_editor/toolbar/save.bmp – сохранить уровень
  - \image html level_editor/toolbar/save_as.bmp – сохранить уровень как
  - \image html level_editor/toolbar/arrow.bmp - выделение/отметка объектов
  - \image html level_editor/toolbar/pos.bmp - перемещение объекты
  - \image html level_editor/toolbar/rot.bmp - вращение объектов
  - \image html level_editor/toolbar/scale.bmp - масштабирование объектов
  - \image html level_editor/toolbar/grid.bmp - показывать сетку 
  - \image html level_editor/toolbar/axes.bmp - показывать статичные оси в центре системы координат
  - \image html level_editor/toolbar/r_c.bmp - цвет (выводимое изображение в окно рендера)
  - \image html level_editor/toolbar/r_n.bmp - нормали (выводимое изображение в окно рендера)
  - \image html level_editor/toolbar/r_p.bmp - параметры освещения (выводимое изображение в окно рендера)
  - \image html level_editor/toolbar/r_ad.bmp - фоновое и диффузное освещение (выводимое изображение в окно рендера)
  - \image html level_editor/toolbar/r_s.bmp - отраженный свет (выводимое изображение в окно рендера)
  - \image html level_editor/toolbar/r_l.bmp - освещенная сцена (выводимое изображение в окно рендера)
  - \image html level_editor/toolbar/sel_s.bmp - надо ли выделять объект
  - \image html level_editor/toolbar/sel_z.bmp - использовать ли z test при рендере выделения
  - \image html level_editor/toolbar/sel_m.bmp - показывать сетку или модель целиком при выделении
  - \image html level_editor/toolbar/sel_cb.bmp - показывать задние грани выделения
  - \image html level_editor/toolbar/aigrid_bound.bmp - показывать ли ограничивающий объем ai сетки
  - \image html level_editor/toolbar/aigrid_quad.bmp - показывать ли квадраты ai сетки
  - \image html level_editor/toolbar/aigrid_graphpoint.bmp - показывать ил графпоинты ai сетки
  - \image html level_editor/toolbar/lvl_out.bmp - открытый (out) или закрытый (in) уровень
  - \image html level_editor/toolbar/sun.bmp - включить глобальный источник света

 - Содержимое окна – рабочая область по настройке уровня
 - Раздел объектов уровня
  - \image html level_editor/toolbar/list_level_objects.png - список объектов уровня в зависимости от типа выбора
  - \image html level_editor/toolbar/buttons_type_object.png - кнопки выбора типа объектов уровня
 - Строка состояния:
  - \image html level_editor/status_level_poly.png – общее количество полигонов на уровне
  - \image html level_editor/statusbar_geom_poly.png – количество полигонов геометрии
  - \image html level_editor/statusbar_green_poly.png – количество полигонов растительности
  - \image html level_editor/statusbar_count_game_object.png – количество игровых объектов


\section level_editor_camera Управление камерой
CTRL + w – движение вперед \n
CTRL + s – движение назад \n
CTRL + a – движение влево \n
CTRL + d – движение вправо \n
SHIFT - ускорение \n
CTRL + ПКМ – движение вверх/вниз \n
CTRL + ЛКМ – вращение \n


\subsection level_editor_gui_geom Static geometry – статическая геометрия
\image html level_editor/staticgeom_pre.png интерфейс статической геометрии до добавления модели
\n
\image html level_editor/staticgeom_post.png интерфейс статической геометрии после добавления модели
\n

 - \b Name - имя объекта
 - \b Model - путь до статической модели (относительно директории со статическими моделями)
 - \b Lod1 - путь до лода статической модели (относительно директории со статическими моделями)
 - \b Position - позиция объекта, при отметке одного из переключателей + RShift + ЛКМ будет трансформация по выбранной оси
 - \b Rotation - вращение объекта, при отметке одного из переключателей + RShift + ЛКМ будет трансформация по выбранной оси
 - \b Scale (не рекомендуется) - масштабирование объекта, при отметке одного из переключателей + RShift + ЛКМ будет трансформация по выбранной оси


\subsection level_editor_gui_green Green - растительность
\image html level_editor/green_pre.png интерфейс растительности до добавления растительности
\n
\image html level_editor/green_post.png интерфейс растительности после добавления растительности
\n

\note Если указать модель только для Model объект будет травой, иначе (должны быть заполнены все лоды) объект будет кустаник/дерево
\todo Про плотность надо расписать более подробно

 - \b Name - имя объекта
 - \b Model - путь до статической модели (относительно директории со статическими моделями)
 - \b Lod1 - путь до первого лода статической модели (относительно директории со статическими моделями)
 - \b Lod2 - путь до первого лода статической модели (относительно директории со статическими моделями)
 - \b Mask texture - имя текстуры маски, по которой будет сгенерирована растительность (маска покрывает весь уровень), необязательно в случае ручной расстановки
 - \b NavMesh - если растительность должна иметь физические параметры (к примеру для деревьев) то путь до статической модели (относительно директории со статическими моделями)
 - \b Density - плотность растительности

После добавления и выделения объекта растительности станвятся доступны новые элементы:
\image html level_editor/green_post_add.png интерфейс растительности после добавления растительности
 - новый элемент ComboBox содержит:
  - \b single select - выделение единицы объекта растительности по ЛКМ
  - \b single create - создание единицы объекта растительности по ЛКМ
  - \b multiple create - множественное создание единиц объекта растительности, ПКМ постановка бокса, ЛКМ постановка бокса и генерация в нем единицы объекта растительности
  - \b single delete - удаление единицы объекта растительности  по ЛКМ
 - \b PosX/PoxY/PosZ - установка значения позиции выделенной единицы объекта растительности, если multiple create то установка размеров бокса в котором будет сгенерирована растительность, при вкюченном переключатели на однйо из осей + нажатии LShift + ЛКМ (вверх/вниз) перемещение по выбранной оси (если multiple create то изменение размеров бокса по выбранной оси)


\subsection level_editor_gui_aigrid AI grid - ai сетка
\image html level_editor/aigrid_pre.png интерфейс ai сетки определения границ
\n
\image html level_editor/aigrid_post.png интерфейс ai сетки после определения границ
\n

Для создания ai сетки, необходимо определить ее границы:
 - \b Bound box dimensions - 
 - \b Bound box position - 
 - \b Create bound box - 

\note Для генерации AI сетки необходимо чтобы хотя бы один квадрат был создан, квадрат должен быть незамкнут уровнем.
\todo Уточнить про валидность сетки

После создания границ доступны следующие возможности:
 - \b AI quad add - добавить квадрат по ЛКМ
 - \b AI quad multi select - мультивыделение квадратов по ЛКМ, и единичное выделение по ПКМ
 - \b AI quads select->delete - удаление при выделении ЛКМ
 - \b AI quads delete selected - удалить выделенные
 - \b AI grid generation - сгенерировать сетку
 - \b AI grid clear - очистка сетки (удаление всех квадратов)
 - \b Graph point add - добавить графпоинт по ЛКМ
 - \b Graph point delete - удалить графпоинт по ЛКМ
 - \b Graph points generate - сгенерировать графпоинты
 - \b Graph points clear - очистить список графпоинтов
 - \b AI grid validation - проверить сетку на валидность
 - \b Marked splits - отмитить куски сетки разными цветами
 - \b Count quads - количество квадратов сетки
 - \b Count graph points - количество графпоинтов
 - \b Count splits - количество кусков сетки
 - \b Clear all - очистить все

Если есть хотя бы один выделенный квадрат, LCTRL + ЛКМ (вверх/вниз) будет перемещать квадрат/квадраты


\subsection level_editor_gui_gameobject Game object - игровой объект
\image html level_editor/gameobject_properties.png интерфейс свойств игрового объекта
\n

Таблица слева содержит все доступные свойства игрвоого объекта.
 - \b Class - класс объекта
 - \b Help - подсказка
 - \b Create - создание нового объекта
 - \b Connections - переход к соединениям объекта

\image html level_editor/gameobject_connections.png интерфейс соединений игрового объекта
\n
Таблица слева содержит все соединения игрового объекта
 - \b Event - событие (выход)
 - \b Name - имя игрового объекта с которым происходит связь
 - \b Action - действие (вход)
 - \b Delay - задержка в секундах (float)
 - \b Parameter - дополнительный параметр
 - \b Create - создание нового соединения
 - \b Properties - переход к свойствам объекта

*/

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>
#include "level_editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SXGUIRegClass::RegGroupBox();
	SXLevelEditor::InitAllElements();

	SkyXEngine_Init(SXLevelEditor::RenderWindow->GetHWND(), SXLevelEditor::JobWindow->GetHWND());
	SRender_GetCamera()->SetPosition(&float3(0, 0.5, -2));
	SXLevelEditor::MainMenu->InsertPopupItem(4, "Weather", 4, 0);
	SXLevelEditor::MainMenu->InsertItem(4, "none", SX_LE_MMENU_WEATHER_BEGIN_ID, 4);
	SXLevelEditor::MainMenu->CheckItem(SX_LE_MMENU_WEATHER_BEGIN_ID, true);

	SXLevelEditor::CheckBoxTBAIGBound->SetCheck(false);
	SXLevelEditor::AIGBound = false;
	SXLevelEditor::CheckBoxTBAIGQuad->SetCheck(true);
	SXLevelEditor::AIGQuad = true;
	SXLevelEditor::CheckBoxTBAIGGraphPoint->SetCheck(true);
	SXLevelEditor::AIGGraphPoint = true;

	SXLevelEditor::CheckBoxTBGrid->SetCheck(true);
	SXLevelEditor::CheckBoxTBAxes->SetCheck(true);
	SXLevelEditor::MainMenu->CheckItem(ID_VIEW_GRID, true);
	SXLevelEditor::MainMenu->CheckItem(ID_VIEW_AXES, true);

	SXLevelEditor::CheckBoxTBRColor->SetCheck(true);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, true);
	Core_0SetCVarInt("r_final_image", DS_RT_COLOR);

	SXLevelEditor::CheckBoxTBSelS->SetCheck(true);
	SXLevelEditor::SelSelection = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_SELECTION, true);

	SXLevelEditor::CheckBoxTBSelCullBack->SetCheck(true);
	SXLevelEditor::SelBackFacesCull = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, true);
	SXLevelEditor::SelZTest = false;

	SXLevelEditor::CheckBoxTBSelMesh->SetCheck(true);
	SXLevelEditor::SelMesh = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_MESH, true);

	SRender_EditorSetRenderGrid(true);
	SRender_EditorSetRenderAxesStatic(true);


	SXLevelEditor::LEcreateData();


	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	SXLevelEditor::MenuWeatherCount = 1;
	char tpath[1024];
	sprintf(tpath, "%sweather\\*.cfg", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS));
	hf = FindFirstFile(tpath, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE){
		do{
			SXLevelEditor::MainMenu->InsertItem(4, FindFileData.cFileName, SX_LE_MMENU_WEATHER_BEGIN_ID + SXLevelEditor::MenuWeatherCount, 4);
			++SXLevelEditor::MenuWeatherCount;
			SXLevelEditor::MenuWeatherArr.push_back(FindFileData.cFileName);
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

	

	int countgc = SXGame_EntGetClassListCount();
	const char** listgc = new const char*[countgc];

	SXGame_EntGetClassList(listgc, countgc);
	for (int i = 0; i < countgc; ++i)
	{
		SXLevelEditor::ComboBoxGameClass->AddItem(listgc[i]);
	}
	mem_delete_a(listgc);


	SkyXEngine_PreviewKill();
	SXLevelEditor::JobWindow->Visible(true);
	int result = SkyXEngine_CycleMain();

	SXLevelEditor::LEdeleteData();

	SkyXEngine_Kill();
	return result;
}