
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Главный файл редактора материалов SXMaterialEditor
*/

/*!
\page material_editor Редактор материалов SXMaterialEditor
\tableofcontents
\section material_editor_main Общее
\b SXMaterialEditor – редактор материаловдля движка SkyXEngine. \n
<i>Основное окно (также как и окно рендера) не изменяемо в размерах.</i>

\image html material_editor/image1.png "Скришот программы SXMaterialEditor"

\section material_editor_window Структура окна
<b>Окно состоит из структурных элементов (сверху вниз): </b>
 - <b>Заголовок окна</b>, содержащий имя редактор и имя материала \n
 - Меню программы:
  - File:
  - Open – открыть материал
  - Save – сохранить материал
  - Exit – выход из программы
 - Final image (выводимое изображение в окно рендера):
  - Color – цвет
  - Normals – нормали
  - Parameters – параметры освещения
  - Ambient - фоновое и диффузное освещение
  - Specular – отраженный свет
  - Lighting scene – освещенная сцена
 - Панель инструментов, где расположены основные управляющие кнопки:
  - \image html material_editor/image2.png открыть материал
  - \image html material_editor/image3.png сохранить материал
  - \image html material_editor/image4.png возможность перемещения камеры
  - \image html material_editor/image5.png установка камере старновой позиции и поворота
  - \image html material_editor/image6.png вывод на рендер цвета
  - \image html material_editor/image7.png вывод на рендер нормалей
  - \image html material_editor/image8.png вывод на рендер параметров освещения
  - \image html material_editor/image9.png вывод на рендер фонового и диффузного освещения
  - \image html material_editor/image10.png вывод на рендер отраженного света
  - \image html material_editor/image11.png вывод на рендер освещенной сцены
 - Содержимое окна – рабочая область по настройке материала.


\section material_editor_camera Управление камерой
CTRL + w – движение вперед \n
CTRL + s – движение назад \n
CTRL + a – движение влево \n
CTRL + d – движение вправо \n
SHIFT - ускорение \n
CTRL + ПКМ – движение вверх/вниз \n
CTRL + ЛКМ – вращение \n


\section material_editor_gui Интерфейс
\subsection material_editor_gui_main Общее

\image html material_editor/image12.png кнопка выбора файлов, после нажатия на эту кнопку следует открытие диалога выбора файла
\image html material_editor/image13.png поле для ввода цифровой/текстовой информации, после ввода при нажатии на клавишу Enter введенные значения будут приняты. Также изменения вступят в силу после того как этот элемент потеряет фокус (к примеру щелкнув на пустое место/на другой элемент этого же окна)
Все текстуры (в том числе и текстуры скайбокса) должны находится по относительному пути (относительно запущенной программы) по адресу: \b /gamesource/textures/  \n
Все шейдеры должны находится по относительному пути (относительно запущенной программы) по адресу: \b /gamesource/ 


\subsection material_editor_gui_ed_set Редакторные настройки

\image html material_editor/image14.png вращение модели по оси Y, кнопка 0 установит угол вращения в 0
\image html material_editor/image15.png загрузка кубической текстуры скайбокса
\image html material_editor/image16.png выбор типа модели, от выбора зависит рендер и передаваемые данные (принимаемые шейдером), доступно:
 - geometry – статическая геометрия
 - grass - трава
 - tree - дерево
 - skin – анимационная модель.

\image html material_editor/image17.png тестовые модели:</p>
 - sphere
 - cube
 - plane.


\subsection material_editor_gui_base Базовые настройки

\image html material_editor/image18.png основная текстура материала
\image html material_editor/image19.png набор комбинаций шейдеров (для упрощения выбора шейдеров), если активна нулевая строка (пустая) значит, выбрана комбинация которой нет в списке
\image html material_editor/image20.png вершинный шейдер
\image html material_editor/image21.png пиксельный шейдер
\image html material_editor/image22.png физический материал:
 - concrete (бетон)
 - metal (металл)
 - glass (стекло)
 - plastic (пластик)
 - tree (дерево)
 - flesh (плоть)
 - ground/sand (земля/песок)
 - water (вода)
 - leaf/grass (листва/трава)

\image html material_editor/image23.png коэффициент пробиваемости [0,1]
\image html material_editor/image24.png нужно ли освещать материал
\image html material_editor/image25.png использовать ли альфа тест (отсечение пикселей которые не прошли минимальный порог прозрачности)

\image html material_editor/image26.png тип преломления:
 - none – нет преломления
 - transparency – освещаемый полупрозрачный материал

\image html material_editor/image27.png тип отражения:
 - none – нет отражения
 - reflect – плоское отражение
 - reflect cube dynamic – динамическое объемное/кубическое отражение
 - reflect cube static - статическое объемное/кубическое отражение, обновляется только в случае изменения данных самого отражения.


\subsection material_editor_gui_paraml Параметры освещения

\image html material_editor/image28.png набор параметров освещения (для упрощения настройки освещения), если активна нулевая строка (пустая) значит выбранных параметров нет в списке
\image html material_editor/image29.png просвечиваемость (для листвы и травы надо делать меньше 1) [0,1]
\image html material_editor/image30.png шероховатость поверхности, от этого зависит рассеиваемость света по поверхности, чем больше тем больше будет рассеивание света [0,1]
\image html material_editor/image31.png отражательная способность поверхности, чем больше тем меньше будет поглощено света и тем больше будет его отражение [0,1]
\image html material_editor/image32.png текстура с параметрами освещения (где можно настроить для каждого пикселя, каналы red – roughness, green - f0, blue</span – thickness), если стоит галочка на texture то будет использована загруженная текстура с параметрами освещения, если текстура не загружена будут использоваться нулевые параметры, если галочка не стоит – будут использоваться параметры из редактора


\subsection material_editor_gui_mr_d Микрорельеф и детальные текстуры

Данные понятия являются очень условными, лишь общая рекомендацией, но текстуры могут быть использованы как угодны в шейдерах, поэтому необходимо читать описание используемых шейдеров.

\image html material_editor/image33.png текстура маски (условно)
\image html material_editor/image34.png текстура микрорельефа (основная либо первая), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в red канале маски
\image html material_editor/image35.png текстура микрорельефа (вторая), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в green канале маски
\image html material_editor/image36.png текстура микрорельефа (третья), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в blue канале маски
\image html material_editor/image37.png текстура микрорельефа (четвертая), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в alpha канале маски

\image html material_editor/image38.png детальная текстура (первая), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в red канале маски
\image html material_editor/image39.png детальная текстура (вторая), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в green канале маски
\image html material_editor/image40.png детальная текстура (третья), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в blue канале маски
\image html material_editor/image41.png детальная текстура (четвертая), при использовании шейдером наложения по маске - коэффициент яркости данной текстуры в alpha канале маски


\subsection material_editor_gui_engine_data Движковые данные отправляемые в шейдеры

\image html material_editor/image42.png матрица мира
\image html material_editor/image43.png матрица вида
\image html material_editor/image44.png матрица проекции
\image html material_editor/image45.png комбинированная матрица мира вида
\image html material_editor/image46.png комбинированная матрица мира вида и проекции
\image html material_editor/image47.png позиция камеры
\image html material_editor/image48.png float2 вектор, x – общее время рендера в миллисекундах, y – текущее время рендера кадра в миллисекундах
\image html material_editor/image49.png float2 вектор размеров окна рендера в пикселях, x – ширина, y - высота


\subsection material_editor_gui_user_data Пользовательские данные (float4) отправляемые в шейдеры

vs - отправка в ввершинный шейдер \n
ps - отправка в пиксельный шейдер \n

\image html material_editor/image50.png отправка пользовательских данных в вершинный шейдер
\image html material_editor/image51.png отправить эти же данные в пиксельный шейдер

Компоненты вектора пользовательских данных [-1,1] вершинного
шейдера:

\image html material_editor/image52.png 
\image html material_editor/image53.png
\image html material_editor/image54.png
\image html material_editor/image55.png
\n
\image html material_editor/image56.png отправка пользовательских данных в пиксельный шейдер
\image html material_editor/image57.png отправить эти же данные в вершинный шейдер

Компоненты вектора пользовательских данных [-1,1] пиксельного
шейдера:

\image html material_editor/image52.png
\image html material_editor/image53.png
\image html material_editor/image58.png
\image html material_editor/image55.png


\section material_editor_shaders Шейдеры материалов
\subsection material_editor_shaders_geom Статическая геометрия

\subsubsection material_editor_shaders_geom_base Базовый рендер
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_base.ps
 - Движковые данные:
  - VS: WVP, W

\subsubsection material_editor_shaders_geom_mrbase Рендер с основным (повторяющим основную текстуру) микрорельефом
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_mrbase.ps
 - Движковые данные:
  - VS:  WVP, W
 - Пользовательские данные:
  - PS: z – множитель микрорельефа
 - Дополнительные текстуры: M R

\subsubsection material_editor_shaders_det1 Рендер с одной детальной текстурой
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_det1.ps
 - Движковые данные:
  - VS: WVP
 - Пользовательские данные:
  - PS: x – коэфициент масштабирования детальной текстуры
 - Дополнительные текстуры: D R

\subsubsection material_editor_shaders_geom_mrbase_det1 Рендер с основным микрорельефом и одной детальной текстурой
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_mrbase_det1.ps
  - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
  - PS: 
   - x – коэфициент масштабирования детальной текстуры, 
   - z – множитель микрорельефа
 - Дополнительные текстуры: M R, D R

\subsubsection material_editor_shaders_geom_mrbase_mr1_det1 Рендер с основным микрорельефом, одной детальной текстурой и микрорельефом для нее
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_mrbase_mr1_det1.ps
 - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
 - PS: 
  - x – коэфициент масштабирования детальной текстуры, 
  - z – множитель микрорельефа
 - Дополнительные текстуры: M R, M G, D R

\subsubsection material_editor_shaders_geom_land_det2 Рендер ландшафта, 2 детальных текстуры, определение местоположения по маске (каналы red, green)
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_land_det2.ps
 - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
 - PS: 
  - x – коэфициент масштабирования текстур, 
  - y – множитель цвета детальных текстур
 - Дополнительные текстуры: Mask, D R, D G

\subsubsection material_editor_shaders_geom_land_det3 Рендер ландшафта, 3 детальных текстуры, определение местоположения по маске (каналы red, green, blue)
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_land_det3.ps
 - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
 - PS: 
  - x – коэфициент масштабирования текстур, 
  - y – множитель цвета детальных текстур
 - Дополнительные текстуры: Mask, D R, D G, D B

\subsubsection material_editor_shaders_geom_land_det4 Рендер ландшафта, 4 детальных текстуры, определение местоположения по маске (каналы red, green, blue, alpha)
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_land_det4.ps
 - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
 - PS: 
  - x – коэфициент масштабирования текстур, 
  - y – множитель цвета детальных текстур, 
  - z – множитель микрорельефа
 - Дополнительные текстуры: Mask, D R, D G, D B, D A

\subsubsection material_editor_shaders_geom_land_mr2_det2 Рендер ландшафта, 2 детальных текстуры и 2 микрорельфа к ним, определение местоположения по маске (каналы red, green)
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_land_mr2_det2.ps
 - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
 - PS: 
  - x – коэфициент масштабирования текстур, 
  - y – множитель цвета детальных текстур, 
  - z – множитель микрорельефа
 - Дополнительные текстуры: Mask, M R, M G, D R, D G

\subsubsection material_editor_shaders_geom_land_mr3_det3 Рендер ландшафта, 3 детальных текстуры и 3 микрорельфа к ним, определение местоположения по маске (каналы red, green, blue)
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_land_mr3_det3.ps
 - Движковые данные:
 - VS:  WVP, W
 - Пользовательские данные:
 - PS: 
  - x – коэфициент масштабирования текстур, 
  - y – множитель цвета детальных текстур, 
  - z – множитель микрорельефа
 - Дополнительные текстуры: Mask, M R, M G, M B, D R, D G, D B

\subsubsection material_editor_shaders_geom_land_mr4_det4 Рендер ландшафта, 4 детальных текстуры и 4 микрорельфа к ним, определение местоположения по маске (каналы red, green, blue, alpha)
 - VS - mtrlgeom_base.vs
 - PS - mtrlgeom_land_mr4_det4.ps
 - Движковые данные:
  - VS:  WVP, W
 - Пользовательские данные:
  - PS: 
   - x – коэфициент масштабирования текстур, 
   - y – множитель цвета детальных текстур, 
   - z – множитель микрорельефа
 - Дополнительные текстуры: Mask, M R, M G, M B, M A, D R, D G, D B, D A

\subsubsection material_editor_shaders_geom_reflect_cube Рендер с объемным (кубическим) отражением
 - VS - mtrlgeom_reflect_cube.vs
 - PS - mtrlgeom_reflect_cube.ps
 - Движковые данные:
  - VS:  WVP, WV, W, View
 - Reflection: reflect cube dynamic или reflect cube static

\subsubsection material_editor_shaders_geom_reflect_cube_mrbase Рендер с объемным (кубическим) отражением и основным микрорельефом
 - VS - mtrlgeom_reflect_cube.vs
 - PS - mtrlgeom_reflect_cube_mrbase.ps
 - Движковые данные:
  - VS:  WVP, WV, W, View
 - Reflection: reflect cube dynamic или reflect cube static
 - Пользовательские данные:
  - PS: 
   - z – множитель микрорельефа
 - Дополнительные текстуры: M R

\subsubsection material_editor_shaders_geom_water Рендер воды
 - VS - mtrlgeom_water.vs
 - PS - mtrlgeom_water.ps
 - Движковые данные:
  - VS:  WVP, W, campos, count_td
  - PS: win size
 - Пользовательские данные:
  - VS:
   - x - амплитуда движения по оси X
   - y - амплитуда движения по оси Y
   - z – коэффициент масштаба текстурных координат
   - w – скорость движения волн
  - PS:
   - x - коэфициент смещения текстурных координат
   - y - коэфициент прозрачности
   - z - коэфициент микрорельефа
   - w - коэфициент смешивания между отражением и основной текстурой (чем больше тем больше будет преобладать цвет основной текстуры, чем меньше тем больше будет преобладать цвет
 - Дополнительные текстуры: M R

\subsubsection material_editor_shaders_geom_water_reflect Рендер воды с плоским отражением
 - VS - mtrlgeom_water.vs
 - PS - mtrlgeom_water_reflect.ps
 - Движковые данные:
  - VS:  WVP, W, campos, count_td
  - PS: win size
 - Пользовательские данные:
  - VS:
   - x - амплитуда движения по оси X
   - y - амплитуда движения по оси Y
   - z – коэффициент масштаба текстурных координат
   - w – скорость движения волн
  - PS:
   - x - коэфициент смещения текстурных координат
   - y - коэфициент прозрачности
   - z - коэфициент микрорельефа
   - w - коэфициент смешивания между отражением и основной текстурой (чем больше тем больше будет преобладать цвет основной текстуры, чем меньше тем больше будет преобладать цвет
 - Дополнительные текстуры: M R, D R (карта смещения координат)


\subsection material_editor_shaders_green Растительность

\subsubsection material_editor_shaders_green_grass_base Базовый рендер травы
 - VS - mtrlgreen_grass_base.vs
 - PS - mtrlgreen_base.ps
 - Движковые данные:
  - VS: WVP

\subsubsection material_editor_shaders_green_grass_mrbase Рендер травы с основным микрорельефом
 - VS - mtrlgreen_grass_base.vs
 - PS - mtrlgreen_mrbase.ps
 - Движковые данные:
  - VS: WVP
 - Пользовательские данные:
  - PS: 
   - z – множитель микрорельефа
 - Дополнительные текстуры: M R

\subsubsection material_editor_shaders_green_tree_base Базовый рендер деревьев
 - VS - mtrlgreen_tree_base.vs
 - PS - mtrlgreen_base.ps
 - Движковые данные:
  - VS: WVP

\subsubsection material_editor_shaders_green_tree_mrbase Рендер деревьев с основным микрорельефом
 - VS - mtrlgreen_tree_base.vs
 - PS - mtrlgreen_mrbase.ps
 - Движковые данные:
  - VS: WVP
 - Пользовательские данные:
  - PS: z – множитель микрорельефа
 - Дополнительные текстуры: M R


\subsection material_editor_shaders_skin Анимационные модели

\subsubsection material_editor_shaders_skin_base Базовый рендер
 - VS - mtrlskin_base.vs
 - PS - mtrlskin_base.ps
 - Движковые данные:
  - VS: WVP, W

\subsubsection material_editor_shaders_skin_mrbase Рендер с основным микрорельефом
 - VS - mtrlskin_base.vs
 - PS - mtrlskin_mrbase.ps
 - Движковые данные:
  - VS: WVP, W
  - PS: 
   - z – множитель микрорельефа
 - Дополнительные текстуры: M R


*/

#define SX_MATERIAL_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 

#include <SkyXEngine.h>
#include "material_editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SXGUIRegClass::RegGroupBox();
	srand((unsigned int)time(0));

	SXMaterialEditor::InitAllElements();
	SXMaterialEditor::Nulling();
	

	SkyXEngine_Init(SXMaterialEditor::WindowRender->GetHWND(), SXMaterialEditor::JobWindow->GetHWND());
	Core_0SetCVarInt("final_image", DS_RT_SCENELIGHT);

	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
	SXMaterialEditor::CheckBoxTBRLighting->SetCheck(true);

	SXMaterialEditor::CheckBoxTBCam->SetCheck(true);
	SRender_EditorCameraSetMove(SXMaterialEditor::CheckBoxTBCam->GetCheck());

	char shaderskitpath[1024];
	sprintf(shaderskitpath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_EXE), "\\shaders_kit.cfg");
	SXMaterialEditor::Shaders = new ShadersKit();
	SXMaterialEditor::Shaders->Load(shaderskitpath);

	for (int i = 0; i < SXMaterialEditor::Shaders->GetCount(); ++i)
	{
		SXMaterialEditor::ComboBoxShaders->AddItem(SXMaterialEditor::Shaders->GetName(i));
	}

	SXMaterialEditor::ComboBoxShaders->SetSel(0);


	char paramlpath[1024];
	sprintf(paramlpath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_EXE), "\\paraml_kit.cfg");
	SXMaterialEditor::ParamL = new ParamLKit();
	SXMaterialEditor::ParamL->Load(paramlpath);

	for (int i = 0; i < SXMaterialEditor::ParamL->GetCount(); ++i)
	{
		SXMaterialEditor::ComboBoxParamL->AddItem(SXMaterialEditor::ParamL->GetName(i));
	}

	SXMaterialEditor::ComboBoxParamL->SetSel(0);


	SGCore_SkyBoxLoadTex("sky/sky_2_cube.dds");
	SXMaterialEditor::EditSkyBox->SetText("sky/sky_2_cube.dds");
		
	SRender_GetCamera()->SetPosition(&float3(0, 0, -1.2 * 100));

	
	SRender_SimModelAdd("sphere.dse");
	SRender_SimModelAdd("cube.dse");
	SRender_SimModelAdd("plane.dse");

	SML_LigthsCreatePoint(
		&float3(110,110,-110),
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		true);
	SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
	SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");

	SGCore_LoadTexLoadTextures();

	SXMaterialEditor::InitMtl(SRender_SimModelGetIDMtl());
	SXMaterialEditor::IDMat = SRender_SimModelGetIDMtl();

	SkyXEngine_PreviewKill();
	SXMaterialEditor::JobWindow->Visible(true);
	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	SXMaterialEditor::DeleteAllElements();
	return result;
}