
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Главный файл редактора партиклов SXParticlesEditor
*/

/*!
\page particles_editor Редактор партиклов SXParticlesEditor
\tableofcontents
\section particles_editor_main Общее
\b SXParticlesEditor – редактор эффектов состоящих из эмиттеров, которые свою очедерь состоят из частиц (партиклов).\n
Редактор работает только с файлом eff.eff который содержит всю информацию об эффектах и эмиттерах. Данный файл должен находится по относительному пути (относительно программы) /gamesource/eff.eff\n

\image html particles_editor/image1.png "Скриншот программы SXMaterialEditor"


\section particles_editor_window Структура окна
<b>Окно состоит из структурных элементов (сверху вниз):</b>
 - Заголовок окна, содержащий имя редактора.
 - Меню программы:
  - File:
   - ReOpen – переоткрыть файл эффектов eff.eff
   - Save – сохранить файл эффектов eff.eff
   - Exit – выход из программы
  - View (редакторные настройки вида):
   - Grid – сетка разметки (оси xz)
   - Axes – линии по направлению осей
   - Bound – показывать или нет ограничивающий объем для эмиттера (если задан объем)
 - Панель инструментов, где расположены основные управляющие кнопки:
  - \image html particles_editor/image2.png  переоткрыть файл эффектов eff.eff
  - \image html particles_editor/image3.png  сохранить файл эффектов eff.eff
  - \image html particles_editor/image4.png  сетка разметки (оси xz)
  - \image html particles_editor/image5.png  линии по направлению осей
  - \image html particles_editor/image6.png  показывать или нет ограничивающий объем для эмиттера (если задан объем)
  - \image html particles_editor/image7.png  воспроизвести (клик при воспроизведении приведет к паузе)
  - \image html particles_editor/image8.png  пауза (если идет воспроизведение)
  - \image html particles_editor/image9.png  остановить воспроизведение (если идет воспроизведение)
 - Содержимое окна – рабочая область по настройке материала
 - Строка состояния:
  - \image html particles_editor/image10.png  общее количество эмиттеров в эффекте / количество проигрываемых эмиттеров в эффекте
  - \image html particles_editor/image11.png  общее количество частиц в эмиттере / количество живых частиц в эмиттере.


\section particles_editor_camera Управление камерой
CTRL + w – движение вперед \n
CTRL + s – движение назад \n
CTRL + a – движение влево \n
CTRL + d – движение вправо \n
SHIFT - ускорение \n
CTRL + ПКМ – движение вверх/вниз \n
CTRL + ЛКМ – вращение \n


\section particles_editor_gui Интерфейс
\subsection particles_editor_gui_main Общее
\image html particles_editor/image12.png кнопка выбора файлов, после нажатия на эту кнопку следует открытие диалога выбора файла
\image html particles_editor/image13.png поле для ввода цифровой/текстовой информации, после ввода при нажатии на клавишу Enter введенные значения будут приняты. Также изменения вступят в силу после того как этот элемент потеряет фокус (к примеру щелкнув на пустое место/на другой элемент этого же окна)\n
Все текстуры должны находится по относительному пути (относительно запущенной программы) по адресу: /gamesource/textures/ \n
Максимальная длина имен 63 символа. \n

Весь интерфейс создания/изменения эффектов/эмиттеров находится в нижней части окна.\n
В правой части окна находятся списки.\n
\image html particles_editor/image14.png "первый список эффектов"
\image html particles_editor/image15.png "количество эффектов"
\image html particles_editor/image16.png "сам список"
\image html particles_editor/image17.png "кнопка вывода интерфейса создания"
\image html particles_editor/image18.png "кнопка удаления выбранного из списка эффекта".
\n
\image html particles_editor/image19.png второй список эмиттеров в эффекте
\image html particles_editor/image20.png количество эмиттеров в эффекте
\image html particles_editor/image21.png сам список
\image html particles_editor/image22.png кнопка вывода интерфейса создания эмиттера
\image html particles_editor/image23.png кнопка вывода интерфейса создания эмиттера но основе выделенного эмиттера
\image html particles_editor/image24.png кнопка удаления эмиттера

\n
\image html particles_editor/image25.png интерфейс создания/редактирования эффекта
Состоит из поля для ввода имени, при создании эффекта кнопки Create видима, при нажатии на нее будет создан эффект (если имя введено корректно, то есть не пустое).

\image html particles_editor/image26.png интерфейс создания/редактирования эмиттера
Интерфейс создания от интерфейса редактирования отличается наличием кнопки CREATE, при нажатии на которую произойдет создание эмиттера (если данные введены корректно).
Интерфейс содержит множество элементов, и все они поделены логически, для отображения определенной группы элементов существуют переключатели:
\image html particles_editor/image27.png 
\n
\b Base – базовые настройки эмиттера \n
\b TextureAnimTex – настройка текстуры или анимационной текстуры \n
\b Spawn – управление спавном \n
\b Bound – управление ограничивающим объемом \n
\b Characters – установка характеров поведения \n
\b VelocityAcceleration – настройка скорости и ускорения \n


\subsection particles_editor_gui_base Base – базовые настройки эмиттера
\image html particles_editor/image28.png 
\n
\image html particles_editor/image29.png имя эмиттера
\image html particles_editor/image30.png общее количество частиц в эмиттере
\image html particles_editor/image31.png коэфициент на который будет домножен цвет
\image html particles_editor/image32.png количество создваваемых/пересоздаваемых, 0< - пересоздание в случае нехватки, 0> - единственное создание при запуске
\image html particles_editor/image33.png коэфициент для смягчения партиклов [0-1]
\image html particles_editor/image34.png коэфициент для преломления партиклов [0-1]
\image html particles_editor/image35.png коэфициент прозрачности [0-1], компонента альфа канала будет домножена на это значение
\image html particles_editor/image36.png освещаемые ли частицы
\image html particles_editor/collision_delete.png удалять ли частицы при столкновении с миром

\image html particles_editor/image37.png тип фигуры партикла, поддерживаются:
 - quad – квадрат (прямоугольник)
 - billboard – билборд (прямоугольник всегда направленый на камеру)
 - quad_composite – композитная модель, состоящая из нескольких прямоугольников
\image html particles_editor/image38.png количество квадаратов в случае если используется композитная модель частиц
\image html particles_editor/image39.png если отмечено - случайно генерировать углы поворотов, если не отмечено - равномерно поворачивать
\image html particles_editor/image40.png поворачивать ли по оси X
\image html particles_editor/image41.png поворачивать ли по оси Y
\image html particles_editor/image42.png поворачивать ли по оси Z
\image html particles_editor/image43.png тип смешивания, поддерживаемые:
 - pabt_alpha – альфа смешивание на основе альфа канала текстуры
 - pabt_add – аддитивное смешивание, то есть цвет текстуры будет прибавлен к текущему цвету сцены
\image html particles_editor/color_rgba.png цвет частицы [0-1], rgb - цвет, a - коэфициент интерполяции между этим цветом и цветом в текстуре

\image html particles_editor/image44.png время жизни частиц, если 0 тогда частицы живут все время, иначе это время в миллисекундах
\image html particles_editor/image45.png дисперсия для времени жизни
\image html particles_editor/image46.png зависит ли альфа компонента частицы от ее возраста, возможные значения:
 - none - нет
 - direct – прямая зависимость, чем  больше возраст тем меньше альфа компонента
 - inverse – обратная зависимость, чем больше возраст тем больше альфа компонента
\image html particles_editor/image47.png размеры частицы в метрах
\image html particles_editor/image48.png дисперсия для размера частиц
\image html particles_editor/image49.png зависит ли размер частицы от ее возраста (от нулевого размера до установленного), возможные значения:
 - none - нет
 - direct – прямая зависимость, чем старше тем больше размер
 - inverse – обратная зависимость, чем старше  тем меньше размер
\image html particles_editor/track.png след от частиц при столкновении с миром (только для эмиттеров у которых установлен Collision delete), Size - размер в метрах, Time - время показа с плавным затуханием


\subsection particles_editor_gui_tex TextureAnimTex - настройка текстуры или анимационной текстуры
\image html particles_editor/image51.png
\n
\image html particles_editor/image52.png имя текстуры
\image html particles_editor/image53.png количество кадров по оси X
\image html particles_editor/image54.png количество кадров по оси Y
\image html particles_editor/image55.png стартовый кадр
\image html particles_editor/image56.png разброс для стартового кадра
\image html particles_editor/image57.png частота обновления в млсек
\image html particles_editor/image58.png разброс для частоты обновления в млсек


\subsection particles_editor_gui_spawn Spawn - управление спавном
\image html particles_editor/image59.png
\n
\image html particles_editor/image60.png тип спавна партиклов, возможные значения:
 - strictly - строгий в указанном месте
 - bound - по ограничивающему объему
\image html particles_editor/image61.png точка, из которой идут частицы (обязательно должна быть точка, откуда идет спавн частиц, если нет ограничивающего объема), чекбоксы по каждой компонентой позиции означают пределы спавна в случае рандомного спавна по ограничивающему объему, - (минус) от минимума, + (плюс) до максимума
\image html particles_editor/image62.png дисперсия для SpawnOrigin
\image html particles_editor/image63.png промежуток времени, через который будет спавниться следующая частица, если 0 - спавним все сразу
\image html particles_editor/image64.png дисперсия для SpawnNextTime


\subsection particles_editor_gui_bound Bound – управление ограничивающим объемом
\image html particles_editor/image65.png
\n
\image html particles_editor/image66.pngтип ограничивающего объема, для уничтожения партикла при выходе за пределы, возможные значения:
 - none – нет объема
 - sphere - сфера
 - box - параллелепипед
 - cone – конус

Для указания параметров ограничивающего объема используется 2 float4 вектора:
\image html particles_editor/image67.png
\image html particles_editor/image68.png
\n
Box: Vector1 x y z – минимум, Vector2 x y z – максимум \n
Sphere: Vector1 x y z – центр сферы, w – радиус \n
Cone: Vector1 x y z – нижняя точка конуса, w – радиус нижней точки, Vector2 y координата Y верхней точки конуса, w – радиус верхней точки \n


\subsection particles_editor_gui_characters Characters - установка характеров поведения
\image html particles_editor/image69.png 
\n
\subsubsection particles_editor_gui_characters_circle Характер: круговое движение
\image html particles_editor/image70.png использовать ли круговое движение
\image html particles_editor/image71.png ось для кругового движения
\image html particles_editor/image72.png угол (в радианах)
\image html particles_editor/image73.png дисперсия для CharacterCircleAngle
\image html particles_editor/image74.png разрешено ли использовать отрицательную дисперсию

\subsubsection particles_editor_gui_characters_rotate Характер: вращение
\image html particles_editor/image75.png испольовать ли вращение
\image html particles_editor/image76.png угол (в радианах)
\image html particles_editor/image77.png дисперсия для CharacterRotateAngle
\image html particles_editor/image78.png разрешено ли использовать отрицательную дисперсию

\subsubsection particles_editor_gui_characters_deviation Характер: отклонения при движении
\image html particles_editor/image79.png использовать ли отклонения при движении
\image html particles_editor/image80.png тип смещения, возможные значения:
 - rnd - случайное
 - along - вдоль оси
 - wave - волновое
\image html particles_editor/image81.png амплитуда, может быть отрицательна
\image html particles_editor/image82.png коэфициент на который будет умножен угол, только для DeviationType == wave
\image html particles_editor/image83.png коэфициент дисперсии, для DeviationCoefAngle если DeviationType == wave, для DeviationAmplitude в других случаях
\image html particles_editor/image84.png возможна ли отрицательная дисперсия
\image html particles_editor/image85.png ось на основании которой будет вычислен угол поворота, только для DeviationType == wave
\image html particles_editor/image86.png время обновления для DeviationType == rnd и DeviationType == along

\image html particles_editor/image87.png задействовать ли координату X при просчете смещения
\image html particles_editor/image88.png задействовать ли координату Y при просчете смещения
\image html particles_editor/image89.png задействовать ли координату Z при просчете смещения


\subsection particles_editor_gui_vel_acc VelocityAcceleration – настройка скорости и ускорения
\image html particles_editor/image90.png 
\n
\b Скорость:
\image html particles_editor/image91.png
Задается float3 вектором, нормализация этого вектора есть направление взгляда, в котором будет следовать частица. \n
\b Dispersion – дисперсия скорости. \n
\b XDispNeg, \b YDispNeg, \b ZDispNeg – возможна ли отрицательная дисперсия для компоненты вектора (в случае если вообще дисперсия установлена), если да, то для каждой компоненты итоговая скорость будет считать как: rand(-компонента, компонента), если отрицательная дисперсия для компонента запрещена то: rand(0, компонента). \n
\n
\b Ускорение (ускорение нарастает с каждым обновлением, и как поэтому скорость тоже нарастает):
\image html particles_editor/image92.png
Задается float3 вектором, где каждая копонента есть ускорение для определнной оси. \n
\b Dispersion – дисперсия ускорения. \n
\b XDispNeg, \b YDispNeg, \b ZDispNeg – возможна ли отрицательная дисперсия для компоненты вектора (в случае если вообще дисперсия установлена), если да, то для каждой компоненты итоговая ускорение будет считаться как: rand(-компонента, компонента), если отрицательная дисперсия дляял компонента запрещена то: rand(0, компонента).

*/

#define _CRT_SECURE_NO_WARNINGS

#include <SkyXEngine.h>
#include "particles_editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SXGUIinit();
	srand((unsigned int)time(0));

	SXParticlesEditor::InitAllElements();

	SXParticlesEditor::TabsVisible(false);
	SXParticlesEditor::AllInTabsVisible(false);
	
	/*RECT winrndrect;
	SXParticlesEditor::WindowRender->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;*/

	SkyXEngine_Init(SXParticlesEditor::WindowRender->getHWND(), SXParticlesEditor::JobWindow->getHWND());
	Core_0SetCVarInt("r_final_image",DS_RT_COLOR);
	SRender_GetCamera()->setPosition(&float3(0, 0.5, -2));
	SXParticlesEditor::MainMenu->setCheckItem(ID_VIEW_GRID, true);
	SXParticlesEditor::MainMenu->setCheckItem(ID_VIEW_AXES, true);
	SXParticlesEditor::MainMenu->setCheckItem(ID_VIEW_BOUND, true);
	SXParticlesEditor::CheckBoxTBGrid->setCheck(true);
	SXParticlesEditor::CheckBoxTBAxes->setCheck(true);
	SXParticlesEditor::CheckBoxTBBound->setCheck(true);
	SRender_EditorSetRenderGrid(true);
	SRender_EditorSetRenderAxesStatic(true);
	SXParticlesEditor::canRenderBound = true;


	SXParticlesEditor::PEcreateData();

	SGCore_LoadTexAllLoad();

	SXParticlesEditor::EffInitList();

	SkyXEngine_PreviewKill();
	SXParticlesEditor::JobWindow->setVisible(true);
	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();

	SXParticlesEditor::PEdeleteData();

	SXParticlesEditor::DeleteAllElements();
	return result;
}