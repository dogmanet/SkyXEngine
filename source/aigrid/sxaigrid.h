
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

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

#ifndef __SXAIGRID_H
#define __SXAIGRID_H

#if 1

#define SAIG_QuadGetNear(...) (-1)
#define SAIG_QuadSetState(...)
#define SAIG_QuadGetState(...) (AIQUAD_STATE_FREE)
#define SAIG_QuadSetStateWho(...)
#define SAIG_QuadGetStateWho(...) (-1)
#define SAIG_QuadGetPos(...)
#define SAIG_QuadGet(...) (-1)
#define SAIG_GridQueryFindPath(...) (-1)
#define SAIG_GridGetSizePath(...) (-1)
#define SAIG_GridSetColorArr(...)
#define SAIG_GridGetPath(...)

#else

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

/*! \name Базовые функции библиотеки
@{*/

//! версия подсистемы
SX_LIB_API long SAIG_0GetVersion();				

//! установить функцию обработки сообщений
SX_LIB_API void SAIG_Dbg_Set(report_func fnReport);	

//! инициализация подсистемы
SX_LIB_API void SAIG_0Create(
	const char *szName,			//!< присваиваемое имя
	bool useGraphics = false,	//!< использовать ли графическое отображение (для редакторов к прмиеру) или нет (игровой режим)
	bool isUnic = true			//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожить подсистему
SX_LIB_API void SAIG_AKill();	

//!@}
#endif

/*! преобразование float в int
 \warning осторожно, изменения  могут повлечь баги в генерации и некоторых иных функция. Когда было *100 то в некоторых местах генерировались дубликаты, и поиск квада по позиции не всегда работал корректно
*/
#define AIGRID_TOINT(fNum) (int32_t)(roundf(fNum * 10.f))

/*! максимальная высота между центрами соседних квадов */
#define AIGRID_QUADS_CENTERS_MAXHEIGHT	0.6f	

/*! размеры ограничивающих боксов */
#define AIGRID_BBOXSIZE					5.f		

/*! минимально допустимый размер дял деления сплита дерева боксов */
#define AIGRID_SPLIT_MIN_SIZE AIGRID_BBOXSIZE*5.f		

/*! #AIGRID_TOINT максимальная высота между центрами соседних квадов */
#define LAIGRID_QUADS_CENTERS_MAXHEIGHT	6		

/*! #AIGRID_TOINT размеры квада */
#define LAIGRID_QUAD_SIZE				7		

//направления
/*! квад имеет переходы в положительное и отрицательное направления */
#define AIGRID_QUAD_DIR_ALL				0		

/*! квад имеет только положительный переход */
#define AIGRID_QUAD_DIR_POSITIVE		1		

/*! квад имеет только отрицательный переход */
#define AIGRID_QUAD_DIR_NEGATIVE		-1		

/*! квад не имеет переходов */
#define AIGRID_QUAD_DIR_NONE			2		


/*! размеры квада */
#define AIGRID_QUAD_SIZE				0.7f	

/*! половинный размер квада */
#define AIGRID_QUAD_SIZEDIV2			0.35f	

/*! на сколько метров будет приподнят квад */
#define AIGRID_QUAD_ADDYPOS				0.01f;	


//цена перемещения
/*! цена прямого перемещения */
#define AIGRID_COST_DIRECT				10		

/*! цена перемещения по диагонали */
#define AIGRID_COST_DIAGONAL			14		


/*! минимально необходимая дистанция (int(dist * 100)) для нахождения ближайшего графпоинта */
#define AIGRID_COST_GPATH				500		


/*! добавочная стоимость для прохода через квад который временно занят */
#define AIGRID_COSTADD_TEMPBUSY			50		

/*! добавочная стоимость для прохода через квад который занят */
#define AIGRID_COSTADD_BUSY				250		


/*! дистанция (ходы по горизонтали и вертикали, но не по диагонали) в пределах которой будет учитываться занятость квада */
#define AIGRID_QUADSCOUNT_BUSY			50		


/*! максимальная высота сущности, которая будет передвигаться по сетке */
#define AIGRID_ENTITY_MAX_HEIGHT		1.5f								

/*! половинная максимальная высота сущности */
#define AIGRID_ENTITY_MAX_HEIGHTDIV2	(AIGRID_ENTITY_MAX_HEIGHT * 0.5f)	


/*! цвет главного ограничивающего объема */
#define AIGRID_COLOR_GBBOX				GX_COLOR_ARGB(128, 0, 255, 0)		

/*! цвет выделенного квада */
#define AIGRID_COLOR_SEL_MAIN			GX_COLOR_ARGB(220, 255, 255, 255)	

/*! цвет соседей (прямых) выделенного квада */
#define AIGRID_COLOR_SEL_ADD			GX_COLOR_ARGB(100, 255, 255, 255)	

//! состояния занятости квада
enum AIQUAD_STATE
{

	//! свободен
	AIQUAD_STATE_FREE = 0,		

	//! временно занят
	AIQUAD_STATE_TEMPBUSY = 1,	

	//! занят
	AIQUAD_STATE_BUSY = 2,		
};

/*! тип функции для навигации квада по уровню, 
на вход идет текущая позиция квада, 
возвращает false в случае удачи (изъянов не найдено) и true в случае неудачи(есть боковые пересечения или нет персечения с уровнем вообще, либо по высоте не вмещается) 
*/
typedef bool(*g_aiquad_phy_navigate) (float3_t *pPos);

#if 0
//##########################################################################

/*! переназначение g_aiquad_phy_navigate, обязательное действие для работы с сеткой */
SX_LIB_API void SAIG_SetFunc_QuadPhyNavigate(g_aiquad_phy_navigate func);

//! очистка всех данных
SX_LIB_API void SAIG_Clear();


//! есть ли валидные квады на аи сетке
SX_LIB_API bool SAIG_ExistsQuads();


//! сохранение сетки
SX_LIB_API void SAIG_GridSave(const char *szPath);	

//! загрузка сетки
SX_LIB_API void SAIG_GridLoad(const char *szPath);	

/*! \name Ограничивающий объем 
@{*/

//! создать ограничивающий объем по параметрам
SX_LIB_API void SAIG_BBCreate(const float3 *pCenter, const float3 *pParam);	

//! создан ли ограничивающий объем
SX_LIB_API bool SAIG_BBIsCreated();											


//! установка габаритов ббокса для каждой оси
SX_LIB_API void SAIG_BBSetDimensions(const float3 *pDimensions);	

//! записывает габариты ббокса
SX_LIB_API void SAIG_BBGetDimensions(float3 *pDimensions);			


//! установак позиции ббокса
SX_LIB_API void SAIG_BBSetPos(const float3 *pPos);			

//! записывает позицию ббокса
SX_LIB_API void SAIG_BBGetPos(float3 *pPos);		


//! завершение создания ограничивающего объема, после которого изменить какие-либо его данные будет невозможно
SX_LIB_API void SAIG_BBCreateFinish();						

//! завершено ли создание ббокса
SX_LIB_API bool SAIG_BBIsCreatedFinish();					

//!@}

/*! \name Управление AI сеткой
@{*/

//! очистка данных сетки
SX_LIB_API void SAIG_GridClear();	

//! трассировка луча и проверка его пересечения с каким либо квадом сетки, возвращает id квада если было пересечение
SX_LIB_API ID SAIG_GridTraceBeam(const float3 *pStart, const float3 *pDir);	

//! тест действительности сетки и устранение возможных дефектов
SX_LIB_API void SAIG_GridTestValidation();									

SX_LIB_API UINT SAIG_GridGetCountSplits();

//! выделение сплитов
SX_LIB_API void SAIG_GridSetMarkSplits(bool isMark);			

//! возвращает состояние выделения сплитов
SX_LIB_API bool SAIG_GridGetMarkSplits();					


//! функция просчетов, ее нужно вызывать чтобы просчитать всю аи сетку
SX_LIB_API void SAIG_GridGenerate();						

//! возвращает количество квадов в сетке
SX_LIB_API UINT SAIG_GridGetCountQuads();

SX_LIB_API void SAIG_GridQueryFindPathUpdate(UINT uiLimitMls);

//! поиск пути, (beginq,beginq]
SX_LIB_API ID SAIG_GridQueryFindPath(ID idBegin, ID idEnd);

//! отмена запроса поиска пути, idQuery - идентификатор запроса, возвращает удалось ли отменить
SX_LIB_API bool SAIG_GridCancelQueryFindPath(ID idQuery);

//! размер найденного пути в количестве квадратов
SX_LIB_API int SAIG_GridGetSizePath(ID idQueueObject);


//! запись найденного пути в уже выделенную память, если reverse == true то будет записано от начала до конца пути, иначе от конца до начала пути
SX_LIB_API bool SAIG_GridGetPath(ID idQueueObject, ID *pMem, UINT uiCount, bool isReverse);

//! установка цвета массиву квадов
SX_LIB_API void SAIG_GridSetColorArr(const ID *pMem, DWORD dwColor, UINT uiCount);	

//! обнуление увета у всех квадов
SX_LIB_API void SAIG_GridSetNullColor();					

//!@}

/*! \name Управление квадратами сетки
@{*/

//! устанавливает состояние для квада
SX_LIB_API void SAIG_QuadSetState(ID idQuad, AIQUAD_STATE state);

//! возвращает текущее состояние квада
SX_LIB_API AIQUAD_STATE SAIG_QuadGetState(ID idQuad);

//! устанавливает idQuad объекта который занял квад
SX_LIB_API void SAIG_QuadSetStateWho(ID idQuad, ID idWho);

//! возвращает idQuad объекта который занял квад
SX_LIB_API ID SAIG_QuadGetStateWho(ID idQuad);

//! проверка квада: является ли квад idQuad соседом и idQuad1 кваду и idQuad2 кваду
SX_LIB_API bool SAIG_QuadIs2Neighbors(ID idQuad, ID idQuad1, ID idQuad2);


//! получить idQuad квада по позиции, isNearOrPermissible - самый ближний квад (true), или самый ближний в пределах допустимой разницы начальной точки (false)?
SX_LIB_API ID SAIG_QuadGet(const float3 *pPos, bool isNearOrPermissible);	

SX_LIB_API bool SAIG_QuadGetPos(ID idQuad, float3 *pPos);

//! установка позиции по оси Y для квада
SX_LIB_API void SAIG_QuadSetPosY(ID idQuad, float fPosY);

//! возвращает позицию по оси Y квада
SX_LIB_API float SAIG_QuadGetPosY(ID idQuad);


//! добавление квада в позицию
SX_LIB_API ID SAIG_QuadAdd(const float3 *pPos);					

//! удаление квада по его idQuad
SX_LIB_API bool SAIG_QuadDelete(ID idQuad);


//! добавление квада к списку выделения, idQuad - идентификатор квада, если -1 то очищает список, useConsiderPrev - учитывать ли предыдущие записанные, если нет то очищает массив и записывает туда, если да то записывает сверху
SX_LIB_API void SAIG_QuadSelect(ID idQuad, bool useConsiderPrev);

//! добавление к позиции по оси Y для выделенных квадов
SX_LIB_API void SAIG_QuadSelectedAddPosY(float fPosY);			

//! удаление выделенных квадов
SX_LIB_API void SAIG_QuadSelectedDelete();						


//! свободен ли квад idQuad в радиусе iRadius (iRadius - количество квадов вокруг указанного в idQuad, 1 - значит только указанный, 2 - значит все соседние и так далее)
SX_LIB_API bool SAIG_QuadIsFree(ID idQuad, int iRadius);

//! возвращает id ближайшего квада (если isFree == true то ищет только свободные) с радиусом свободности iQuadRadius
SX_LIB_API ID SAIG_QuadGetNear(const float3 *pPos, float fRadius, bool isFree = false, int iQuadRadius = 1);

#define SAIG_QuadSetColor(id, color) SAIG_GridSetColorArr(&id, color, 1)

//!@}

/*! \name Graph points
 \note Graph points (графпоинты) - промежуточные квадраты на AI сетке через которые возможно быстрое построение длинного пути. Эти квадраты являются обычными квадратами сетки, только занесены в массив графпоинтов
 \note Графпоинтов не должно быть много, они могут быть расставлены вручную, либо сгенерированы автоматически (по центру каждого ббокса имеющего квады)
@{*/

//! автоматическая генерация графпоинтов в центрах ограничивающих боксов
SX_LIB_API void SAIG_GraphPointGenerate();					

//! возвращает количество графпоинтов
SX_LIB_API UINT SAIG_GraphPointGetCount();					

//! очистка списка графпоинтов
SX_LIB_API void SAIG_GraphPointClear();						

//! добавление графпоинта (idQuad - идентификатор квада сетки)
SX_LIB_API void SAIG_GraphPointAdd(ID idQuad);

//! удаление графпоинта (idQuad - идентификатор квада сетки)
SX_LIB_API void SAIG_GraphPointDelete(ID idQuad);				

//! поиск наиболее близкого графпоинта между стартовым и конечным квадами следования
SX_LIB_API ID SAIG_GraphPointGetNear(ID idBegin, ID idEnd);	

//!@}

/*! \name Рендер
@{*/

//! отрисовка сетки
SX_LIB_API void SAIG_RenderQuads(const IFrustum *pFrustum, const float3 *pViewPos, float fDist);	

//! отрисовка графпоинтов
SX_LIB_API void SAIG_RenderGraphPoints(const float3 *pViewPos, float fDist);			

//! отрисовка ограничивающих объемов
SX_LIB_API void SAIG_RenderBB();			

//!@}
#endif
#endif

//!@} sxaigrid