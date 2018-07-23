
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __AIGRID_H
#define __AIGRID_H

#include <gdefines.h>
#include <common/array.h>
#include <common/memalloc.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>
#include "sxaigrid.h"
#include <math.h>

extern g_aiquad_phy_navigate AIQuadPhyNavigate;

#define AIGRID_QUAD_PRECOND(id,retval)  if (id < 0 || m_aQuads.size() <= id) {LibReport(REPORT_MSG_LEVEL_ERROR, " %s - unresolved index '%d' of quad, sxaigrid", GEN_MSG_LOCATION, id); return retval;}

//! int значения позиций полученные путем AIGRID_TOINT
struct int3
{
	int3()
	{ 
		x = y = z = 0; 
	}

	int3(int32_t iX, int32_t iY, int32_t iZ)
	{ 
		x = iX; y = iY; z = iZ; 
	}

	int32_t x, y, z;
};

//! структура описывающая квадрат на аи сетке
struct CAIquad
{
	CAIquad()
	{
		m_chAxisX = m_chAxisY = 0;
		m_isClose = false;
		m_id = -1;

		for(int i=0;i<8;++i)
			m_aNeighbors[i] = -1;

		m_iSplit = 0;
	}

	//установка соседа для квада
	void setNeighbor(int iKey,ID idQuad)
	{
		m_aNeighbors[iKey] = idQuad;

		if (iKey == 1)
		{
			if (m_chAxisY == AIGRID_QUAD_DIR_NEGATIVE)
				m_chAxisY = AIGRID_QUAD_DIR_ALL;
			else if (m_chAxisY == AIGRID_QUAD_DIR_NONE)
				m_chAxisY = AIGRID_QUAD_DIR_POSITIVE;
		}

		if (iKey == 6)
		{
			if (m_chAxisY == AIGRID_QUAD_DIR_POSITIVE)
				m_chAxisY = AIGRID_QUAD_DIR_ALL;
			else if (m_chAxisY == AIGRID_QUAD_DIR_NONE)
				m_chAxisY = AIGRID_QUAD_DIR_NEGATIVE;
		}

		if (iKey == 3)
		{
			if (m_chAxisX == AIGRID_QUAD_DIR_POSITIVE)
				m_chAxisX = AIGRID_QUAD_DIR_ALL;
			else if (m_chAxisX == AIGRID_QUAD_DIR_NONE)
				m_chAxisX = AIGRID_QUAD_DIR_NEGATIVE;
		}

		if (iKey == 4)
		{
			if (m_chAxisX == AIGRID_QUAD_DIR_NEGATIVE)
				m_chAxisX = AIGRID_QUAD_DIR_ALL;
			else if (m_chAxisX == AIGRID_QUAD_DIR_NONE)
				m_chAxisX = AIGRID_QUAD_DIR_POSITIVE;
		}
	}


	//! направления по осям, значения из AIGRID_QUAD_DIR_
	char m_chAxisX, m_chAxisY;

	//! позиция квада
	float3_t m_vPos;		

	//! закрытый ли квад
	bool m_isClose;		

	//! id соседей
	ID m_aNeighbors[8];

	//! идентификатор-ключ для обращения к массиву
	ID m_id;				

	//! идентификатор сплита к которому отсносится данный квад (в случае если сетка не цельная)
	uint16_t m_iSplit;	
};

//! ограничивающий объем для квадов
struct CBoundAIquad
{
	CBoundAIquad(){}
	~CBoundAIquad(){}

	//! float экстремумы
	float3_t m_f3Min, m_f3Max;

	//! int (#AIGRID_TOINT) экстремумы 
	int3 m_i3Min, m_i3Max;

	//! массив идентификаторов квадов
	Array<ID> m_aIdsQuads;
};

//! нод дерева боксов
struct CTreeBoundAIquad
{
	CTreeBoundAIquad()
	{
		m_aSplits[0] = 0;
		m_aSplits[1] = 0;
		m_aSplits[2] = 0;
		m_aSplits[3] = 0;
		m_pBoundBox = 0;
	}

	~CTreeBoundAIquad()
	{
		mem_delete(m_aSplits[0]);
		mem_delete(m_aSplits[1]);
		mem_delete(m_aSplits[2]);
		mem_delete(m_aSplits[3]);
		mem_release(m_pBoundBox);
	}

	//! входящие ноды
	CTreeBoundAIquad *m_aSplits[4];

	//! ограничивающий объем
	ISXBound *m_pBoundBox;

	//! int данные ограничивающего объема
	int3 m_i3Min, m_i3Max;

	//! входящие боксы
	Array<CBoundAIquad*, 64> m_pBounds;
};

//!структура стоимости перехода по кваду
struct CCostTransit
{
	CCostTransit()
	{
		F=G=H=0;
	}

	uint32_t F,G,H;
};

//! вершина квада AI сетки
struct CAIQuadVertexData
{
	CAIQuadVertexData(){}
	CAIQuadVertexData(float fX, float fY, float fZ, float fTCX, float fTCY)
	{
		m_vPos.x = fX;
		m_vPos.y = fY;
		m_vPos.z = fZ;

		m_vTC.x = fTCX;
		m_vTC.y = fTCY;
	}

	//! позиция
	float3_t m_vPos;

	//! текстурные координаты
	float2_t m_vTC;
};

//! вершина трансформации для hardware instancing
struct CAIQuadVertexDataInst
{
	//! позиция
	float3_t m_vPos;

	//! текстурные координаты
	float4_t m_vTC;

	//! цвет
	uint32_t m_uiColor;
};

//##########################################################################

//! класс AI сетки
class CAIGrid
{
public:
	CAIGrid();
	~CAIGrid();

	//!инициализация графических данных, если надо
	void initGraphics();

	//! очистка всех данных
	void clear();		

	//! есть ли квады в сетке?
	bool existsQuads() const;

	//! сохранение данных о сетке, путь абсолютный
	void save(const char *szPath);

	//! загрузка данных о сетке, путь абсолютный
	void load(const char *szPath);
	
	//ограничивающий объем
	//{

	//! создать ограничивающий объем по параметрам
	void bbCreate(const float3 *pCenter, const float3 *pParam);	

	//! создан ли ограничивающий объем
	bool bbIsCreated() const;									
	
	//! установка габаритов
	void bbSetDimensions(const float3 *pDimensions);

	//! получение габаритов
	void bbGetDimensions(float3 *pDimensions) const;

	//! установка позиции
	void bbSetPos(const float3 *pPos);

	//! получение позиции
	void bbGetPos(float3 *pPos) const;

	//! завершение создания ограничивающего объема, после которого изменить какие-либо его данные будет невозможно
	void bbCreateFinish();

	//! окончательно ли создан ограничивающий объем сетки?
	bool bbIsCreatedFinish() const;
	//}

	//квады
	//{
	//установка/возвращение состояния квада
	void quadSetState(ID id, AIQUAD_STATE state);

	//! возвращает состояние квада
	AIQUAD_STATE quadGetState(ID id) const;

	//! установка занятости квада, idWho - идентификатор того кто занял квад
	void quadSetStateWho(ID id, ID idWho);

	//! возвращает идентификатор того кто занял квад
	ID quadGetStateWho(ID id) const;

	//! проверка квада: является ли квад id соседом и idn1 кваду и idn2 кваду
	bool quadIs2Neighbors(ID id, ID idn1, ID idn2) const;
	
	//! установка/возвращение позиции по оси Y для квада
	void quadSetPosY(ID id, float fPosY);

	//! возвращение позиции по оси Y для квада
	float quadGetPosY(ID id) const;


	//! добавление квада в позицию
	ID quadAdd(const float3 *pPos);					

	//! удаление квада по его id
	bool quadDelete(ID id);							
	

	//! добавление квада к списку выделения, id - идентификатор квада, если -1 то очищает список, isConsiderPrev - учитывать ли предыдущие записанные, если нет то очищает массив и записывает туда, если да то записывает сверху
	void quadSelect(ID id, bool isConsiderPrev);		

	//! добавление к позиции по оси Y для выделенных квадов
	void quadSelectedAddPosY(float fPosY);	

	//! удаление выделенных квадов
	void quadSelectedDelete();						
	
	//! свободен ли квад id в радиусе radius (iRadius - количество квадов вокруг указанного в id, 1 - значит только указанный, 2 - значит все соседние и т.д.)
	bool quadIsFree(ID id, int iRadius);		

	//! возвращает id ближайшего квада в радиусе fRadius от pPos (если isFree == true то ищет только свободные) c радиусом свободности iQuadRadius
	ID quadGetNear(const float3 *pPos, float fRadius, bool isFree = false, int iQuadRadius = 1);	

	//! получить id квада по позиции, isNearOrPermissible - самый ближний квад, или самый ближний в пределах допустимой разницы по оси Y
	ID quadGet(const float3 *pPos, bool isNearOrPermissible) const;

	//! возвращает позицию квада по его id
	bool quadGetPos(ID id, float3 *pPos) const;
	//}

	//сетка
	//{

	//! функция генерации сетки, должен быть хотя бы один квад для начала генерации
	void gridGenerate();	

	//! очистка сетки, полное удаление данных
	void gridClear();

	//! трассировка луча и проверка его пересечения с каким либо квадом сетки
	ID gridTraceBeam(const float3 *pStart, const float3 *pDir) const;	

	//! тест действительности сетки и устранение возможных дефектов
	void gridTestValidation();

	//! возвращает количество сплитов сетки
	UINT gridGetCountSplits() const;

	//! выделение сплитов
	void gridSetMarkSplits(bool isMark);						

	//! возвращает состояние выделения сплитов
	bool gridGetMarkSplits() const;

	//! возвращает количество квадов в сетке
	UINT gridGetCountQuads() const;								
	
	//! возвращает размер найденного пути в количестве квадратов, idQueueObject - идентификатор запроса
	int gridGetSizePath(ID idQueueObject) const;

	//! запись найденного пути в уже выделенную память
	bool gridGetPath(ID idQueueObject, ID *pMemory, UINT uiCount, bool canReverse);	

	//! запрос на поиск пути, возвращает идентификатор запроса
	ID gridQueryFindPath(ID idStart, ID idFinish);

	//! отменить запрос поиска пути по идентификатору запроса idQuery
	bool gridCancelQueryFindPath(ID idQuery);

	//! обновление запросов поиска пути, uiLimitMls количество млсекунд выделяемых на работу функции, но минимум один поиск
	void gridQueryFindPathUpdate(UINT uiLimitMls);

	//! установить квадам сетки цвета
	void gridSetColorArr(const ID *pMem, DWORD dwColor, UINT uiCount);

	//! обнулить все цвета сетки
	void gridSetNullColor();

	//}

	//графпоинты
	//{

	//! автоматическая генерация графпоинтов в центрах ограничивающих боксов
	void graphPointGenerate();

	//! возвращает количество графпоинтов
	UINT graphPointGetCount() const;

	//! очистка списка графпоинтов
	void graphPointClear();

	//! добавление графпоинта (id - идентификатор квада сетки)
	void graphPointAdd(ID id);

	//! удаление графпоинта (id - идентификатор квада сетки)
	void graphPointDelete(ID id);

	//! поиск наиболее близкого графпоинта между стартовой и конецной точкой следования
	ID graphPointGetNear(ID idStart, ID idFinish);

	//}
	
	//рендер
	//{

	//! отрисовка сетки
	void renderQuads(const IFrustum *pFrustum, const float3 *pViewPos, float fDist);

	//! отрисовка графпоинтов
	void renderGraphPoints(const float3 *pViewPos, float fDist);

	//! отрисовка ограничивающих объемов
	void renderBB();

	//}
	
protected:

	//**********************************************************************

	//! состояния элемента очереди запросов на поиск пути
	enum QUEUE_OBJ_STATE
	{
		//! простой
		QUEUE_OBJ_STATE_IDLE,

		//! ожидает поиска
		QUEUE_OBJ_STATE_WAIT,

		//! путь найден
		QUEUE_OBJ_STATE_COMPLITE,

		//! при поиске возникли проблемы
		QUEUE_OBJ_STATE_ERROR,
	};

	//! объект очереди
	struct CQueueObject
	{
		CQueueObject()
		{
			m_state = QUEUE_OBJ_STATE_IDLE;
			m_idStart = m_idFinish = -1;
		}

		//! ожидает ли текущий объект поиска?
		QUEUE_OBJ_STATE m_state;

		//! стартовый квад
		ID m_idStart;

		//! финальный квад
		ID m_idFinish;

		//! массив id квадов найденного пути
		Array<ID> m_aQuads;
	};

	//**********************************************************************

	//! использовать ли графику, точнее: будет ли осуществляться рендер
	bool m_useGraphics;

	//! dx9 устройство
	IDirect3DDevice9 *m_pDXDevice;

	//! декларация вершин сетки
	IDirect3DVertexDeclaration9 *m_pDecl;

	//! текстура сетки
	ID m_idTex;

	//! вершинный шейдер
	ID m_idVS;

	//! пиксельный шейдер
	ID m_idPS;

	//! текстура для ограничивающих объемов
	ID m_idTexBB;

	//! выделенная память для квадов
	MemAlloc<CAIquad> m_oAllocQuad;			

	//! выделенная память для баундов
	MemAlloc<CBoundAIquad> m_oAllocBound;

	//! массив цвета для каждого квада сетки (по id квада)
	Array<uint32_t, 1000> m_aQuadColors;		

	//! массив цветов сплитов
	Array<uint32_t> m_aSplitsColor;		

	//! массив int значений координат для каждого квада (по id квада)
	Array<int3, 1000> m_aLongCoordQuads;

	//! массив готовых квадов (порядковый номер и есть id)
	Array<CAIquad*, 1000> m_aQuads;		

	//! массив состояний для каждого квада (по id квада)
	Array<AIQUAD_STATE> m_aQuadState;		

	//! массив квадов на проверку (используется при генерации)
	Array<CAIquad*, 1000> m_aQuadsCheck;	
	
	//! массив идентификаторов объектов которые занимают квады (по id квада)
	Array<ID> m_aStateWho;				

	//! массив состояния проверка занятости для каждого квада (по id квада)
	Array<bool> m_aPreCondFreeState;	

	//! дерево (quad) боксов
	CTreeBoundAIquad *m_pTreeBound;
	
	//! массив ограничивающих объемов для разбиения сетки
	Array<CBoundAIquad*> m_aBound;		

	//! массив навигационных квадов, они такие же как и обычные квады, длинные пути удобнее строить через них
	Array<ID> m_aGraphPointsIDs;		

	//! массив со стоимостями перемещения по навигационным квадам
	Array<int32_t> m_aCostGraphPointsIDs;


	//! массив id квадов которые мы проверяем при тесте целостности сетки
	Array<ID, 1000> m_aTestSplitsIDs;	

	//! закрытый список (по id квада)
	Array<bool> m_aCloseIDs;			

	//! открытый список, ArrIDsInOpen[id квада]
	Array<ID> m_aOpenIDs;

	//! значение квада в открытом списке, по id квада
	Array<ID> m_aIDsInOpen;				

	//! список родителей (по id квада)
	Array<ID> m_aParentIDs;				

	//! список стоимостей проходов по каждому кваду (по id квада)
	Array<CCostTransit> m_aCostQuads;

	//! массив выделенных в данный момент квадов
	Array<ID> m_aSelectedQuads;

	//! выделить ли цветом сплиты сетки
	bool m_isMarkSplits;

	//! примерный размер открытого списка, бывает чуть больше чем надо, но не выходит за пределы массива
	int NumLastKeyOpenList;

	//! номер последнего занесенного квада в открытый список
	ID CountInitOpen2;

	//! количество сплитов сетки
	uint16_t m_uiCountSplits;

	//! вершинный буфер квада
	IDirect3DVertexBuffer9 *m_pVertexQuad;

	//! индексный буфер квада
	IDirect3DIndexBuffer9 *m_pIndexQuad;

	//! вершинный буфер с данными трансформаций
	IDirect3DVertexBuffer9 *m_pTransVertBuf;

	//! меш главного ограничивающего объема
	ID3DXMesh *m_pBoundBox;

	//! значения ограничивающего объема
	float3_t m_vMax, m_vMin;

	//! массив очереди запросов на поиск пути
	Array<CQueueObject> m_aQueueFind;

	//! номер последнего ожидающего запроса на поиск пути
	int m_iLastWait;
	
	//**********************************************************************

	//! генерация дерева боксов
	void createTreeBound();

	//! возвращает найденный ожидающий запрос на поиск пути
	ID getQueueWaiting();


	//! возвращает найденный простаивающий запрос на поиск пути
	ID getQueueIdle();


	//! поиск пути
	bool gridFindPath(ID idQueueObject);							

	//! является ил квад с id графпоинтом
	ID graphPointGetKey(ID id);			

	//! рекурсивная функция проверки свободности квада
	bool isFreeQuad2(ID id, int iRadius);	

	//! выделен ли квад с id
	bool quadSelectedExists(ID id);					

	//! есть ли у квада с id сосед по прямому направлению который сейчас выделен
	bool quadSelectedExistsNeightborDirect(ID id);	


	//! коррекция позиции под центр ближайшего квада (по xz)
	void correctPosXZ(float3 *pPos);				

	//! валидный ли квад с указанным id
	bool isValidIdInArrQuads(ID id);		

	//! удалить все невалидные квады (это те которые по каким-то причинам оказались закрытыми или не имеют прямого направления)
	void quadDeleteInvalidAll();				

	//! удаление информации о кваде из всех возможных источников (с изменением всех идентификаторов которые идут после него)
	void quadDelete2(ID id);					


	//! физическая навигация  квада
	void phyNavigate(CAIquad *pQuad);	

	//! находится ли разница между высотами центров квадов в допустимом пределе
	bool gridCompareHeight(ID id1, ID id2);				

	//! пересоздание буферов
	void reCreateBuffs();											

	//! установка значений по умолчанию для квада id
	void defInitBuffs(ID id);										


	//! добавление квада в подходящий ббокс и запись его позиции в int3
	void addUniqQuadArrag(const CAIquad *pQuad);					

	//! добавление квада в указанный ббокс и запись его позиции в int3
	void addUniqQuadArrag2(const CAIquad *pQuad, CBoundAIquad *pBB);		

	//! генерация соседей квада по прямым направлениям и полный просчет квада
	void genQuad(CAIquad *pQuad);										

	//! просчет и назначение соседей для квада
	void computeNeighbor(ID id);									

	//! является ли квад уникальным
	bool isUniqQuadArrag(CAIquad *pQuad, ID *pIdQuad, CBoundAIquad **ppBB);

	//! деление пространства ограничивающего объема на более мелкие части
	void splitBB(int iX,int iY,int iZ);								

	//! находится ли квад в открытом списке
	bool IsOpen(ID id);				

	//! просчет H стоимости, диагональные шаги не в счет
	int CalcHCost(ID id1, ID id2);	

	//! просчет H стоимости, просчет расстояния между квадами * 100
	int CalcH2Cost(ID id1, ID id2);	

	//! добавить квад в открытый список
	ID AddInOpenList(ID id);			
};

#endif