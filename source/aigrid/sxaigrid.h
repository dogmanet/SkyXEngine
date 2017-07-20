
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxaigrid - библиотека AI сетки (навигационной)
*/

/*! \defgroup sxaigrid sxaigrid - регулярная навигационная сетка (AI сетка)
AI сетка состоит из квадратов (квад), квады имеют соседей (приелгающие квады по xz), некоторые квады могут быть графпонитами\n
 \warning Функции #SAIG_QuadAdd #SAIG_QuadDelete #SAIG_GridGenerate могу нарушать целостность сетки, либо наоборот собирать сетку воедино, поэтому вызов этих функций изменяет внутреннее состояние целостности,
 если состояние целостности изменено его нужно просчитать вызовом функции #SAIG_GridTestValidation после чего сосотяние целостности будет четко определено и можно искать путь.
 \warning Ограничивающий объем (ббокс) создается первым, далее следует его настрока (если необходимо) после того как ббокс полностью настроен необходимо вызвать #SAIG_BBCreateFinish, эта функция произведен первичную сегментацию пространства, после этого никакие манипуляции с ббоксом недоступны

@{
*/

#ifndef __sxaigrid
#define __sxaigrid

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>

/*! \name Базовые функции библиотеки
@{*/

SX_LIB_API long SAIG_0GetVersion();				//!< версия подсистемы
SX_LIB_API void SAIG_Dbg_Set(report_func rf);	//!< установить функцию обработки сообщений

//! инициализация подсистемы
SX_LIB_API void SAIG_0Create(
	const char* name,			//!< присваиваемое имя
	bool use_graphics = false,	//!< использовать ли графическое отображение (для редакторов к прмиеру) или нет (игровой режим)
	bool is_unic = true			//!< должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SAIG_AKill();	//!< уничтожить подсистему

//!@}

#define AIGRID_QUADS_CENTERS_MAXHEIGHT	0.6f	/*!< максимальная высота между центрами соседних квадов */

#define AIGRID_BBOXSIZE					5.f		/*!< размеры ограничивающих боксов */

#define LAIGRID_QUADS_CENTERS_MAXHEIGHT	60		/*!< int*100 максимальная высота между центрами соседних квадов */
#define LAIGRID_QUAD_SIZE				70		/*!< int*100 размеры квада */
#define LAIGRID_QUAD_SIZEDIV2			35		/*!< int*100 половинный размер квада */

//направления
#define AIGRID_QUAD_DIR_ALL				0		/*!< квад имеет переходы в положительное и отрицательное направления */
#define AIGRID_QUAD_DIR_POSITIVE		1		/*!< квад имеет только положительный переход */
#define AIGRID_QUAD_DIR_NEGATIVE		-1		/*!< квад имеет только отрицательный переход */
#define AIGRID_QUAD_DIR_NONE			2		/*!< квад не имеет переходов */

#define AIGRID_QUAD_SIZE				0.7f	/*!< размеры квада */
#define AIGRID_QUAD_SIZEDIV2			0.35f	/*!< половинный размер квада */
#define AIGRID_QUAD_ADDYPOS				0.01f;	/*!< на сколько метров будет приподнят квад */

//цена перемещения
#define AIGRID_COST_DIRECT				10		/*!< цена прямого перемещения */
#define AIGRID_COST_DIAGONAL			14		/*!< цена перемещения по диагонали */

#define AIGRID_COST_GPATH				500		/*!< минимально необходимая дистанция (int(dist * 100)) для нахождения ближайшего графпоинта */

#define AIGRID_COSTADD_TEMPBUSY			50		/*!< добавочна стоимость для прохода через квад который временно занят */
#define AIGRID_COSTADD_BUSY				250		/*!< добавочна стоимость для прохода через квад который занят */

#define AIGRID_QUADSCOUNT_BUSY			50		/*!< дистанция (ходы по горизонтали и вертикали, но не по диагонали) в пределах которой будет учитываться занятость квада */

#define AIGRID_ENTITY_MAX_HEIGHT		1.5f								/*!< максимальная высота сущности, которая будет передвигаться по сетке */
#define AIGRID_ENTITY_MAX_HEIGHTDIV2	(AIGRID_ENTITY_MAX_HEIGHT * 0.5f)	/*!< половинная максимальная высота сущности */

#define AIGRID_COLOR_GBBOX				D3DCOLOR_ARGB(128, 0, 255, 0)		/*!< цвет главного ограничивающего объема */
#define AIGRID_COLOR_SEL_MAIN			D3DCOLOR_ARGB(220, 255, 255, 255)	/*!< цвет выделенного квада */
#define AIGRID_COLOR_SEL_ADD			D3DCOLOR_ARGB(100, 255, 255, 255)	/*!< цвет соседей (прямых) выделенного квада */

//! состояния занятости квада
enum AIQUAD_STATE
{
	AIQUAD_STATE_FREE = 0,		//!< свободен
	AIQUAD_STATE_TEMPBUSY = 1,	//!< временно занят
	AIQUAD_STATE_BUSY = 2,		//!< занят
};

/*! тип функции для навигации квада по уровню, 
на вход идет текущая позиция квада, 
возвращает false в случае удачи (изъянов не найдено) и true в случае неудачи(есть боковые пересечения или нет персечения с уровнем вообще, либо по высоте не вмещается) 
*/
typedef bool(*g_aiquad_phy_navigate) (float3_t * pos);

/*! переназначение g_aiquad_phy_navigate, обязательное действие для работы с сеткой */
SX_LIB_API void SAIG_SetFunc_QuadPhyNavigate(g_aiquad_phy_navigate func);

SX_LIB_API void SAIG_Clear();						//!< очистка всех данных

//сохранение/загрузка
SX_LIB_API void SAIG_GridSave(const char* path);	//!< сохранение сетки
SX_LIB_API void SAIG_GridLoad(const char* path);	//!< загрузка сетки

/*! \name Ограничивающий объем 
@{*/

SX_LIB_API void SAIG_BBCreate(const float3* center, const float3* param);	//!< создать ограничивающий объем по параметрам
SX_LIB_API bool SAIG_BBIsCreated();											//!< создан ли ограничивающий объем

//установка/получение габаритов
SX_LIB_API void SAIG_BBSetDimensions(const float3* dim);	//!< установка габаритов ббокса для каждой оси
SX_LIB_API void SAIG_BBGetDimensions(float3* dim);			//!< записывает габариты ббокса

//установка/получение позиции
SX_LIB_API void SAIG_BBSetPos(const float3* pos);			//!< установак позиции ббокса
SX_LIB_API void SAIG_BBGetPos(float3* pos);					//!< записывает позицию ббокса

SX_LIB_API void SAIG_BBCreateFinish();						//!< завершение создания ограничивающего объема, после которого изменить какие-либо его данные будет невозможно
SX_LIB_API bool SAIG_BBIsCreatedFinish();					//!< завершено ли создание ббокса

//!@}

/*! \name Управление AI сеткой
@{*/

SX_LIB_API void SAIG_GridClear();							//!< очистка данных сетки
SX_LIB_API ID SAIG_GridTraceBeam(const float3* start, const float3* dir);	//!< трассировка луча и проверка его пересечения с каким либо квадом сетки, возвращает id квада если было пересечение
SX_LIB_API void SAIG_GridTestValidation();									//!< тест действительности сетки и устранение возможных дефектов
SX_LIB_API UINT SAIG_GridGetCountSplits();
SX_LIB_API void SAIG_GridSetMarkSplits(bool mark);			//!< выделение сплитов
SX_LIB_API bool SAIG_GridGetMarkSplits();					//!< возвращает состояние выделения сплитов

SX_LIB_API void SAIG_GridGenerate();						//!< функция просчетов, ее нужно вызывать чтобы просчитать всю аи сетку
SX_LIB_API UINT SAIG_GridGetCountQuads();					//!< возвращает количество квадов в сетке
SX_LIB_API int SAIG_GridFindPath(ID beginq, ID endq);		//!< поиск пути, (beginq,beginq]
SX_LIB_API UINT SAIG_GridGetSizePath();						//!< размер найденного пути в количестве квадратов
SX_LIB_API bool SAIG_GridGetPath(ID * pmem, UINT count);	//!< запись найденного пути в уже выделенную память

SX_LIB_API void SAIG_GridSetColorArr(const ID * pmem, DWORD color, UINT count);	//!< установка цвета массиву квадов
SX_LIB_API void SAIG_GridSetNullColor();					//!< обнуление увета у всех квадов

//!@}

/*! \name Управление квадратами сетки
@{*/

SX_LIB_API void SAIG_QuadSetState(ID id, AIQUAD_STATE state);	//!< устанавливает состояние для квада
SX_LIB_API AIQUAD_STATE SAIG_QuadGetState(ID id);				//!< возвращает текущее состояние квада

SX_LIB_API ID SAIG_QuadGet(const float3* pos, bool isnear_or_permissible);		//!< получить id квада по позиции, isnear_or_permissible - самый ближний квад (true), или самый ближний в пределах допустимой разницы начальной точки (false)?

SX_LIB_API void SAIG_QuadSetPosY(ID id, float posy);			//!< установка позиции по оси Y для квада
SX_LIB_API float SAIG_QuadGetPosY(ID id);						//!< возвращает позицию по оси Y квада

SX_LIB_API ID SAIG_QuadAdd(const float3* pos);					//!< добавление квада в позицию
SX_LIB_API bool SAIG_QuadDelete(ID id);							//!< удаление квада по его id

SX_LIB_API void SAIG_QuadSelect(ID id, bool consider_prev);		//!< добавление квада к списку выделения, id - идентификатор квада, если -1 то очищает список, consider_prev - учитывать ли предыдущие записанные, если нет то очищает массив и записывает туда, если да то записывает сверху
SX_LIB_API void SAIG_QuadSelectedAddPosY(float posy);			//!< добавление к позиции по оси Y для выделенных квадов
SX_LIB_API void SAIG_QuadSelectedDelete();						//!< удаление выделенных квадов

SX_LIB_API bool SAIG_QuadIsFree(ID id, int radius);				//!< свободен ли квад id в радиусе radius (radius - количество квадов вокруг указанного в id, 1 - значит только указанный, 2 - значит все соседние и так далее)
SX_LIB_API ID SAIG_QuadGetNear(const float3* pos, bool isfree = false, int raius = 1);	//!< возвращает id ближайшего квада (если isfree == true то ищет только свободные) с радиусом свободности radius

#define SAIG_QuadSetColor(id, color) SAIG_GridSetColorArr(&id, color, 1)

//!@}

/*! \name Graph points
 \note Graph points (графпоинты) - промежуточные квадраты на AI сетке через которые возможно быстрое построение длинного пути. Эти квадраты являются обычными квадратами сетки, только занесены в массив графпоинтов
 \note Графпоинтов не должно быть много, они могут быть расставлены вручную, либо сгенерированы автоматически (по центру каждого ббокса имеющего квады)
@{*/

SX_LIB_API void SAIG_GraphPointGenerate();					//!< автоматическая генерация графпоинтов в центрах ограничивающих боксов
SX_LIB_API UINT SAIG_GraphPointGetCount();					//!< возвращает количество графпоинтов
SX_LIB_API void SAIG_GraphPointClear();						//!< очистка списка графпоинтов
SX_LIB_API void SAIG_GraphPointAdd(ID id);					//!< добавление графпоинта (id - идентификатор квада сетки)
SX_LIB_API void SAIG_GraphPointDelete(ID id);				//!< удаление графпоинта (id - идентификатор квада сетки)
SX_LIB_API ID SAIG_GraphPointGetNear(ID beginq, ID endq);	//!< поиск наиболее близкого графпоинта между стартовым и конечным квадами следования

//!@}

/*! \name Рендер
@{*/

SX_LIB_API void SAIG_RenderQuads(const ISXFrustum * frustum, const float3 * viewpos, float dist);	//!< отрисовка сетки
SX_LIB_API void SAIG_RenderGraphPoints(const float3 * viewpos, float dist);							//!< отрисовка графпоинтов
SX_LIB_API void SAIG_RenderBB();			//!< отрисовка ограничивающих объемов

//!@}

#endif

//!@} sxaigrid