
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл библиотеки графического пользовательского интерфейса посредством WinApi
*/

/*! \defgroup sxguiwinapi sxguiwinapi - библиотека графического пользовательского интерфейса реализованного на WinApi
 \note Для начала нужно инициализировать вызвать функции инициализации #SXGUIinit
@{*/

#ifndef __SXGUIWINAPI_H
#define __SXGUIWINAPI_H

#include <gdefines.h>

#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <stdio.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Comctl32.lib")

#ifdef SX_DLL
#define SX_LIB_API extern "C" __declspec (dllexport)
#else
#define SX_LIB_API extern "C" __declspec (dllimport)
#endif

//##########################################################################

SX_LIB_API void SXGUIinit();

//##########################################################################

//! тип обработчика
typedef LRESULT(*HandlerFunc) (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//! структура данных для обработчика
struct CHandlerData
{
	HandlerFunc m_fnHandler;//!< сама функция, которая будет вызываться
	UINT m_uMsg;			//!< код сообщения
	WPARAM m_wParam;		//!< WParam сообщения
	bool m_needAllowW;		//!< проверять WParam?
	LPARAM m_lParam;		//!< LParam сообщения
	bool m_needAllowL;		//!< проверять LParam
	bool m_isMainFunction;	//!< главный ли это обработчик? true - значение возвращенное фунцией - возвращается WndProc, иначе нет 
};

//!Выравнивание текста в элементе
enum SXGUI_TEXT_ALIGN
{
	SXGUI_TEXT_ALIGN_LEFT = 0,	/*!< по левому краю */
	SXGUI_TEXT_ALIGN_RIGHT,		/*!< по правому краю */
	SXGUI_TEXT_ALIGN_CENTER,	/*!< по центру */
};

//! Стороны элемента
enum SXGUI_SIDE
{
	SXGUI_SIDE_TOP = 0,	/*!< верх */
	SXGUI_SIDE_BOTTOM,	/*!< низ */
	SXGUI_SIDE_RIGHT,	/*!< правая сторона */
	SXGUI_SIDE_LEFT,	/*!< левая сторона */

	SXGUI_SIDE_COUNT
};

//#############################################################################

/*! Первый базовый класс, единственным родителем не используется, 
определяет объект как control, то есть содержит в себе минимум данных для управления объектом
*/
struct ISXGUIControl : public IBaseObject
{
	virtual ~ISXGUIControl(){};

	//! запоминаем дескрипторы, и устанавливаем новый обработчик
	//virtual void init(HWND hWindow, HWND hParent, WNDPROC lpfnWndProc) = 0;	


	//! возвращает HWND родителя
	virtual HWND getParent() = 0;						

	//! устанавливает родителя
	virtual bool setParent(HWND hParent) = 0;	

	//! возвращает свой HWND
	virtual HWND getHWND() = 0;	


	//! установка видимости контрола
	virtual void setVisible(bool isVisible) = 0;

	//! возвращает видим ли элемент
	virtual bool getVisible() = 0;


	//! установить фокус
	virtual void setFocus() = 0;

	//! установлен ли фокус?
	virtual bool getFocus() = 0;						

	//! возвращает предыдущий обработчик
	virtual WNDPROC getPrevWndProc() = 0;


	//! получить пользовательские данные
	virtual void* getUserPtr() = 0;

	//! установить пользовательские данные, возвращает предыдущее значение
	virtual void* setUserPtr(void *ptr) = 0;
};

//#############################################################################

//! см. #ISXGUIHint
struct ISXGUIHint;

/*! Компонент - второй в очереди родитель, часто наследование идет именно от него,
более расширенное управление большинством возможностей 
*/
struct ISXGUIComponent : public virtual ISXGUIControl
{
	virtual ~ISXGUIComponent(){};

	/*! устанавливает необходимые данные для управления элементом
	 \note Необходимо вызывать после инициализации, то есть когда HWND уже получен
	*/
	virtual void initComponent() = 0;

	//! действителен ли элемент
	virtual bool getEnable() = 0;

	//! установка действительности элемента
	virtual void setEnable(bool isEnable) = 0;


	//! установка шрифта (с указанием значений)
	virtual void setFont(
		const char *szName,	//!< имя шрифта, если name == 0 то не изменияем название шрифта, если name[0] == 0 то дефолтный шрифт gui
		int iHeight,		//!< высота, если -1 то не учитываются
		int iWidth,			//!< ширина, если -1 то не учитываются
		int iWeight = -1,	//!< толщина от 1 до 1000 (400 - нормальный, 700 - полужирный), если -1 то не учитываются, 0 - толщина по умолчанию
		int iItalic = -1,	//!< использовать ли стиль "наклонный", 1 - да, 0 - нет, -1 не учитываются
		int iUnderline = -1,//!< использовать ли стиль "подчеркнутый", 1 - да, 0 - нет, -1 не учитываются
		int iStrikeOut = -1	//!< использовать ли стиль "зачеркнутый", 1 - да, 0 - нет, -1 не учитываются
		) = 0;

	//! установка шрифта из дескриптора
	virtual void setHFont(HFONT hFont) = 0;

	//! возвращает дексриптор шрифта
	virtual HFONT getHFont() = 0;


	//! наследуется ли шрифт родителя
	virtual bool hasParentFont() = 0;

	//! возвращает дескриптор родительскиго шрифта
	virtual HFONT getParentHFont() = 0;


	//! модификация стилей
	virtual bool modifyStyle(
		long lStyleAdd,	//!< добавляемые стили, иначе 0
		long lStyleDel	//!< удаляемые стили, иначе 0
		) = 0;

	//! миодицикация расширенных стилей
	virtual bool modifyExStyle(
		long lStyleAdd,	//!< добавляемые стили, иначе 0
		long lStyleDel	//!< удаляемые стили, иначе 0
		) = 0;

	//! устанавливает область окна RECT элемента в глобальных координатах
	virtual bool setWinRect(
		const RECT *pRect,	//!< устанавливаемый RECT
		bool isBoundScreen	//!< использовать ли только рабочую область (рабочий стол кроме панели задач)
		) = 0;

	//! возвращает область окна RECT элемента в глобальных координатах
	virtual void getWinRect(RECT *pRect) = 0;

	//! устанавливает клиентскую область RECT элемента в глобальных координатах
	virtual bool setClientRect(
		const RECT *pRect,	//!< устанавливаемый RECT
		bool isBoundScreen	//!< использовать ли только рабочую область (рабочий стол кроме панели задач)
		) = 0;	

	//! возвращает клиентскую область RECT элемента в глобальных координатах
	virtual void getClientRect(RECT *pRect) = 0;


	/*! \name Всплывающая подсказка
	 \note Первая установка видимости ShowHint(true) либо первая устновка текста #SetHintText инициализируют объект подсказки, до первого вызова функции #GetHintText записывают/возвращают 0
	@{*/

	//! установка состояния видимости подсказки
	virtual void setShowHint(bool isShowHint) = 0;

	//! возвращает: видима ли подсказка?
	virtual bool getShowHint() = 0;		


	//! установить текст подсказки
	virtual void setHintText(const char *szText) = 0;

	//! возвращает текст текущей подсказки
	virtual const char* getHintText() = 0;

	//!@}

	
	/*! \name Методы для цветов элемента
	@{*/

	//! установка цвета текста
	virtual void setColorText(DWORD dwColor) = 0;				

	//! возвращает текущее значение цвета текста
	virtual DWORD getColorText() = 0;						


	//! установка прозрачности заднего фона
	virtual void setTransparentTextBk(bool isTransparent) = 0;			

	//! установлена ли прозрачность заднего фона?
	virtual bool getTransparentTextBk() = 0;				


	//! установка цвета заднего фона текста
	virtual void setColorTextBk(DWORD dwColor) = 0;			

	//! возвращает текущее значение цвета заднего фона текста
	virtual DWORD getColorTextBk() = 0;						


	//! установка цвета фона
	virtual bool setColorBrush(DWORD dwColor) = 0;			

	//! возвращает текущее значение цвета фона
	virtual DWORD getColorBrush() = 0;						

	//! возвращает дескриптор "кисти" фона
	virtual HBRUSH getBrush() = 0;							

	//!@}


	/*! \name Функции для работы с обработчиками
	 \note функция обработчик должна соответствовать стандартной функции обработчику WinApi
	 \note main функция-обработчик может быть только одна, 
	но если ее нет то в качестве возвращаемого значения будет использовано значение вызова функции: 
	 - для окон DefWindowProc(hwnd, msg, wParam, lParam),
	 - для остальных компонентов CallWindowProc(Component->OldProc,hwnd, msg, wParam, lParam)
	@{*/

	//! добавление обработчика
	virtual void addHandler(
		HandlerFunc fnHandler, //!< функция обработчик
		UINT uMsg,			//!< сообщение реакция на которое обрабатывает функция обработчик
		WPARAM wParam,		//!< wParam функции обработчика при котором следует производить вызов функции обработчика
		bool needAllowW,	//!< учитывать ли wParam? если true то прежде вызова функции обработчика будет сравнение на равенство поступившего аргумента wParam с тем что указано было при вызове этой функции
		LPARAM lParam,		//!< lParam функции обработчика при котором следует производить вызов функции обработчика
		bool needAllowL,	//!< учитывать ли lParam? если true то прежде вызова функции обработчика будет сравнение на равенство поступившего аргумента lParam с тем что указано было при вызове этой функции 
		bool isMain			//!< главная ли эта функция обработчик? если true то ее возвращаемое значение будет послано как результат обработки сообщения
		) = 0;

	/*! добавление второстепенного обработчика
	 \note Все что необъявлено из функции #AddHandler не учитывается
	*/
	virtual void addHandler(
		HandlerFunc fnHandler,	//!< функция обработчик
		UINT uMsg				//!< сообщение реакция на которое обрабатывает функция обработчик
		) = 0;	

	//! возвращает обработчик по id
	virtual CHandlerData* getHandler(ID idHandler) = 0;

	//! количество ключей в массиве обработчиков
	virtual int getCountHandlers() = 0;

	//! вызвать функцию обработчик
	virtual LRESULT executeHandler(ID idHandler, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	
	//!@}


	/*! \name Обновление позиций относительно родителя
	@{*/

	//! обновление координат и размеров элемента относительно его родителя, обычно вызывается установленными обработчиками
	virtual void updateSize() = 0;

	//! обновление внутрненних данных для #UpdateSize
	virtual void updateRect() = 0;


	//! закрепление сторон элемента за позициями его родителя, если сторона закреплена, то будет реагировать на изменения размеров родителя, по умолчанию все false
	virtual void setFollowParentSides(bool canTop, bool canBottom, bool canRight, bool canLeft) = 0;
	
	//! закрепление стороны элемента за позициями его родителя
	virtual void setFollowParentSide(SXGUI_SIDE side, bool can) = 0;

	//! закреплена ли сторона за родителем?
	virtual bool getFollowParentSide(SXGUI_SIDE side) = 0;

	//!@}


	/*! \name Данные регулирующие будет ли передвигаться определнная сторона при изменении размеров с этой стороны
	 \note true - можно тянуть за сторону изменяя размер, false - тянуть за сторону не получится
	@{*/

	//! установка возможности растягивать элемент за стороны, по умолчанию все true
	virtual void setStretchSides(bool canTop, bool canBottom, bool canRight, bool canLeft) = 0;

	//! установка возможности растягивать элемент за сторонe
	virtual void setStretchSide(SXGUI_SIDE side, bool can) = 0;

	//! возвращает можно ли растягивать элемент за сторону
	virtual bool getStretchSide(SXGUI_SIDE side) = 0;

	//!@}


	//! установить минимальные размеры окна, установливать надо оба, один работать не будет, по умолчанию 0 - нет минмума
	virtual void setMixSize(UINT uMinWidth, UINT uMinHeght) = 0;

	//! возвращает минимальную ширину
	virtual UINT getMinWidth() = 0;

	//! возвращает минимальную высоту
	virtual UINT getMinHeight() = 0;
};

//##########################################################################

/*! Элемент содержащий текст, название */
struct ISXGUITextual : public virtual ISXGUIComponent
{
	virtual ~ISXGUITextual(){};

	//! установка текста
	virtual bool setText(const char *szStr) = 0;

	//! записывает в buf текущий текст элемента, не больше size
	virtual void getText(char *szBuf, int iSize) = 0;

	//! возвращает размер строки
	virtual int getTextLen() = 0;
};

//#############################################################################

/*! \defgroup sxguiwinapi_elements Элементы интерфейса
 \note Инициализировав элемент можно установить на него стандартные обработчики:
\code
gui_func::base_handlers::InitHandlerMsg(HWND);
\endcode
 \note Во многих функциях создания элементов используются одни и теже именования аргументов:
 - const char *szCaption - название окна
 - WORD x,				- позиция по оси x
 - WORD y,				- позиция по оси y
 - WORD width,			- ширина
 - WORD heigth,			- высота
 - HCURSOR cursor,		- дескриптор курсора, 0 - в случае отсутствия
 - HBRUSH brush,		- дескриптор кисти окна (цвета фона), 0 - в случае отсутствия
 - DWORD exstyle,		- расширенные стили WS_EX_
 - DWORD wndstyle,		- стили окна CS_
 - DWORD style,			- стили элемента WS_
 - HWND hParent,			- дескриптор родителя
 - WNDPROC lpfnHandler = 0	- функция обработчик, если 0 то будет назначен DefWindowProc
@{*/

//! размер текста подсказки #ISXGUIHint
#define SX_HINT_COUNT_BUF_SIZE 1024

//! элемент "Hint" - всплывающая подсказка при наведении на элемент
struct ISXGUIHint : public IBaseObject
{
	virtual ~ISXGUIHint(){}

	//! установка времени ожидания и показа (время в милисекундах)
	virtual void setDelayTime(
		UINT uInit,		//!< сколько ждать до появления подсказки после наведения курсора
		UINT uAutopop	//!< время показа
		) = 0;

	//! устанавливает родителя (при наведении на родителя всплывает эта подсказка)
	virtual bool setParent(HWND hParent) = 0;

	//! возвращает родителя
	virtual HWND getParent() = 0;

	//! возвращает свой HWND
	virtual HWND getHWND() = 0;				

	//! установка видимости
	virtual void setVisible(bool isVisible) = 0;		

	//! видно ли?
	virtual bool getVisible() = 0;				


	//! установка текста подсказки
	virtual void setText(const char *szStr) = 0;		

	//! возвращает указатель на текущий текст подсказки, очищается при вызове деструктора
	virtual const char* getText() = 0;				

	//! записывает в buf текущий текст подсказки
	virtual void getText(char *szBuf) = 0;			
};

//! создание объекта "Hint" для элемента parent
SX_LIB_API ISXGUIHint* SXGUICrHint(HWND hParent);

//! создание объекта "Hint" для элемента parent
SX_LIB_API ISXGUIHint* SXGUICrHintEx(HWND hParent, const char *szText, UINT uInit = 1000, UINT uAutopop = 10000);

//#############################################################################

//! стиль обычного окна, имеющего название, кнопки свернуть/развернуть/закрыть и возможность растягивать
#define SXGUI_WND_FULL_OPT	WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION

//! стиль окна только с рабочей областью
#define SXGUI_WND_AREA	WS_CHILD

//! стиль окна с рабочей областью и рамкой
#define SXGUI_WND_AREA_BORDER	WS_CHILD | WS_BORDER

//! стиль диалогового окна, с названием и кнопкой закрыть
#define SXGUI_WND_DIALOG	WS_DLGFRAME | WS_SYSMENU | WS_CAPTION

//! стиль диалогового окна, с названием и кнопками свернуть/закрыть
#define SXGUI_WND_DIALOG_MINIMIZE	WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION

//! стиль диалогового окна с меньшим по высоте заголовоком, с названием и кнопкой закрыть
#define SXGUI_WND_MINDIALOG	WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED


/*! элемент "Window" - окно. 
По дефолту обработчиком назначается DefWindowProc, 
чтобы функционировали свойства ISXGUIComponent необходимо указать стандартный для всех элементов обработчик WndProcAllDefault
*/
struct ISXGUIBaseWnd : public virtual ISXGUITextual
{
	virtual ~ISXGUIBaseWnd(){};

	/*! \name Прозрачность окна
	 \note Коэфициент прозрачности лежит в пределах [0,1] где 0 - полностью прозрачное, 1 - полностью непрозрачное
	@{*/

	//! установить коэфициент прозрачности
	virtual bool setAlpha(float fAlpha) = 0;

	//! возвращает коэфициент прозрачности
	virtual float getAlpha() = 0;

	//! установка прозрачного стиля (true - использовать коэфициент прозрачности, false - не использовать)
	virtual bool setAlphaBlend(bool isBlend) = 0;

	//! установлен ли прозрачный стиль?
	virtual bool getAlphaBlend() = 0;
	
	//!@}
};

//! создает экземплляр объекта "Window", и возвращает указатель
SX_LIB_API ISXGUIBaseWnd* SXGUICrBaseWnd(
	const char *szCaption,	//!< название окна
	int iX,					//!< позиция по оси x
	int iY,					//!< позиция по оси y
	int width,				//!< ширина
	int heigth,				//!< высота
	DWORD dwExStyle,		//!< расширенные стили WS_EX_
	DWORD dwStyle,			//!< стили элемента WS_
	HWND hParent,			//!< дескриптор родителя
	WNDPROC lpfnHandler = 0,//!< функция обработчик, если 0 то будет назначен общий WndProcAllDefault
	ID idObj = -1
	);

//! создает экземплляр объекта "Window", и возвращает указатель
SX_LIB_API ISXGUIBaseWnd* SXGUICrBaseWndEx(
	const char *szClassName,//!< имя класса окна
	const char *szCaption,	//!< название окна
	int iX,					//!< позиция по оси x
	int iY,					//!< позиция по оси y
	int width,				//!< ширина
	int heigth,				//!< высота
	HICON hIcon,			//!< дескриптор иконки, 0 - в случае отсутствия
	HCURSOR hCursor,		//!< дескриптор курсора, 0 - в случае отсутствия
	HBRUSH hBbrush,			//!< дескриптор кисти окна (цвета фона), 0 - в случае отсутствия
	DWORD dwExStyle,		//!< расширенные стили WS_EX_
	DWORD dwWndStyle,		//!< стили окна CS_
	DWORD dwStyle,			//!< стили элемента WS_
	HWND hParent,			//!< дескриптор родителя
	WNDPROC lpfnHandler = 0,//!< функция обработчик, если 0 то будет назначен общий WndProcAllDefault
	ID idObj = -1
	);

//#############################################################################

#define SXGUI_DEF_STATIC "STATIC"

//! элемент "Static" простое текстовое поле без возможности пользовательского редактирования, также можно создать при помощи него линию горизонтальную/вертикальную
struct ISXGUIStatic : public virtual ISXGUITextual
{
	virtual ~ISXGUIStatic(){}

	//! возвращает тип выравнивания текста #SXGUI_TEXT_ALIGN
	virtual SXGUI_TEXT_ALIGN getAlign() = 0;

	//! устанавливает выравнивание текста #SXGUI_TEXT_ALIGN
	virtual bool setAlign(SXGUI_TEXT_ALIGN align) = 0;


	//! установлен ли автоперенос строк
	virtual bool getWordWrap() = 0;

	//! манипуляция со свойством автоперенос строк
	virtual bool setWordWrap(bool isWordWrap) = 0;
};

//! создает экземплляр объекта "Static", и возвращает указатель
SX_LIB_API ISXGUIStatic* SXGUICrStaticEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExStyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler = 0, ID idObj = -1);

//! создает экземплляр объекта "Static", и возвращает указатель
SX_LIB_API ISXGUIStatic* SXGUICrStatic(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler = 0, ID idObj = -1);

/*! создает экземплляр объекта "Static" который представлен линией, и возвращает указатель
 \param isVertical - вертикальная линия (true), горизонтальная (false)
*/
SX_LIB_API ISXGUIStatic* SXGUICrStaticLine(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler = 0, ID idObj = -1, bool isVertical = false);

//#############################################################################

//! Типы загружаемого фона элемента #ISXGUIButton
enum SXGUI_BUTTON_IMAGE
{
	SXGUI_BUTTON_IMAGE_NONE = 0,	/*!< не использовать загружаемый фон */
	SXGUI_BUTTON_IMAGE_ICON = 1,	/*!< загружать иконку (.ico) */
	SXGUI_BUTTON_IMAGE_BITMAT = 2,	/*!< загружать изображение (.bmp) */
};

//! элемент "button" - кнопка
struct ISXGUIButton : public virtual ISXGUITextual
{
	virtual ~ISXGUIButton(){}

	//! загрузка на фон иконки (.ico)
	virtual bool setIconFromFile(const char *szPath) = 0;	

	//! загрузка на фон изображения (.bmp)
	virtual bool setBmpFromFile(const char *szPath) = 0;

	//! подгрузка на фон иконки (.ico) из ресурсов
	virtual bool setIconFromResourse(UINT idRes) = 0;		

	//! подгрузка на фон изображения (.bmp) из ресурсов
	virtual bool setBmpFromResourse(UINT idRes) = 0;
};

//! создает экземплляр объекта "button", и возвращает указатель
SX_LIB_API ISXGUIButton* SXGUICrButtonEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

/*! создает экземплляр объекта "button", и возвращает указатель
 \param image тип загружаемого фона из возможных вариантов SXGUI_BUTTON_IMAGE_*/
SX_LIB_API ISXGUIButton* SXGUICrButton(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, SXGUI_BUTTON_IMAGE type_image, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

#define SXGUI_DEF_BUTTONIMG "SXGUIBUTTONIMG"

//! обработчик для элемента #ISXGUIButtonImg
SX_LIB_API LRESULT WndProcButtonImgDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define SXGUI_BI_SIZE_LINE			1	/*!< толщина рамки элемента #ISXGUIButtonImg */

/*! \name Состояния выделения элемента #ISXGUIButtonImg
@{*/

#define SXGUI_BUTTONIMG_POS_FREE		0	/*!< не выделен*/
#define SXGUI_BUTTONIMG_POS_HALF_ALLOC	1	/*!< полувыделение при наведении мыши*/
#define SXGUI_BUTTONIMG_POS_ALLOC		2	/*!< выделен*/

//!@}

/*! элемент "ButtonImg" - кнопка с картинкой внутри, ничего общего со станартным элементом "Button" не имеет.
Создавалось для совмещения возможностей "Button" и "RadioButton", то есть несколько "ButtonImg" могут быть у одного родителя, 
при этом только один "ButtonImg" может быть выделен как активный/текущий. Однако остальные элементы могут принимать состояния как "наведенный курсор" и слегка выдяляться. 
*/
struct ISXGUIButtonImg : public virtual ISXGUIComponent
{
	virtual ~ISXGUIButtonImg(){}

	//! перезагрузка изображения (.bmp) path - путь до изображения
	virtual void reInitImage(const char *szPath) = 0;

	//! в szBuf записывает путь до текущего загруженного изображения
	virtual void getPathForImg(char *szBuf) = 0;

	//! установка необходимых обработчиков, вызывать сразу после создания
	virtual void initCallBack() = 0;


	//! установка цвета рамки
	virtual void setColorFrame(DWORD dwColor) = 0;

	//! возвращает текущее значение цвета рамки
	virtual DWORD getColorFrame() = 0;


	//! установка цвета заднего фона
	virtual void setColorBk(DWORD dwColor) = 0;

	//! возвращает текущее значение цвета заднего фона
	virtual DWORD getColorBk() = 0;


	//! установка отсекаемого цвета в изображении
	virtual void setColorAlpha(DWORD dwColor) = 0;

	//! возвращает текущее значение отсекаемого цвета в изображении
	virtual DWORD getColorAlpha() = 0;
	

	//! установка состояния активности кнопки (включена/выключена)
	virtual void setActive(bool isActive) = 0;

	//! включено ли изменение активности кнопки?
	virtual bool getActive() = 0;


	//! установить входит/не входит кнопка в группу остальных таких же кнопок своего родителя
	virtual void setParentGroup(bool isInParentGroup) = 0;

	//! входит ли кнопка в группу остальных таких же кнопок своего родителя?
	virtual bool getParentGroup() = 0;
};

/*! создает экземплляр объекта "ButtonImg", и возвращает указатель
 \param szPath путь до загружаемого изображения (.bmp)
 \param dwAlphaColor исключаемый цвет 
 \param bkColor цвет фона кнопки
*/
SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgLoad(const char *szPath, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwAlphaColor, DWORD bkColor, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

/*! создает экземплляр объекта "ButtonImg", и возвращает указатель
 \param uButton идентификатор ресурса подгружаемого изображения (.bmp)
 \param dwAlphaColor исключаемый цвет
 \param bkColor цвет фона кнопки
*/
SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgRes(UINT uButton, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwAlphaColor, DWORD bkColor, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! элемент "Edit" - текстовове поле с возможностью пользовательского редактирования
struct ISXGUIEdit : public virtual ISXGUITextual
{
	virtual ~ISXGUIEdit(){}

	//! установлено ли свойство "только для чтения"
	virtual bool getReadOnly() = 0;

	//! манипулирует свойством "только для чтения", возвращает результат действия
	virtual bool setReadOnly(bool isReadOnly) = 0;


	//! установлено ли свойств скрытия текста, текст заменяется на *
	virtual bool getStatePassword() = 0;

	//! манипулирует свойством скрытия текста, текст заменяется на *
	virtual bool setStatePassword(bool IsStatePassword) = 0;


	//! возвращает флаг выравнивания текста #SXGUI_TEXT_ALIGN
	virtual int getAlign() = 0;

	//! устанавливает выравнивание #SXGUI_TEXT_ALIGN
	virtual bool setAlign(int align) = 0;
};

//! создает экземплляр объекта "Edit", и возвращает указатель
SX_LIB_API ISXGUIEdit* SXGUICrEditEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "Edit", и возвращает указатель
SX_LIB_API ISXGUIEdit* SXGUICrEdit(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! элемент "UpDown" - кнопки увеличения/уменьшения, обычно используется совместно с элементом "Edit" (#ISXGUIEdit)
struct ISXGUIUpDown : public virtual ISXGUIComponent
{
	virtual ~ISXGUIUpDown(){}

	//! установка минимального и максимального значений
	virtual void setMinMax(int iMin, int iMax) = 0;

	//! устанавливает текущую позицию (число)
	virtual long setPos(int iPos) = 0;


	//! возвращает минимальное число
	virtual long getMin() = 0;

	//! возвращает максимальное число
	virtual long getMax() = 0;

	//! возвращает текущую позицию (число)
	virtual long getPos() = 0;


	//! устанавливает контроль/управление над значением в дескприторе buddy элемента
	virtual HWND setBuddy(HWND hBuddy) = 0;

	//! возвращает HWND элемента над которым осуществляется контроль/управление
	virtual HWND getBuddy() = 0;
};

//! создает экземплляр объекта "UpDown", и возвращает указатель
SX_LIB_API ISXGUIUpDown* SXGUICrUpDownEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1, HWND hBuddy = 0);

/*! создает экземплляр объекта "UpDown", и возвращает указатель
 \param hBuddy HWND элемента (обычно "Edit" #ISXGUIEdit) над которым устанавливается контроль и управление
 \param isAlignLeft true - выравнивание по левому краю, false - выравнивание по правому краю
*/
SX_LIB_API ISXGUIUpDown* SXGUICrUpDown(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1, HWND hBuddy = 0, bool isAlignLeft = false);

//#############################################################################

//! элемент "Memo" - текстовое поле с возможностью создания новых строк
struct ISXGUIMemo : public virtual ISXGUIEdit
{
	virtual ~ISXGUIMemo(){}

	//! установлен ли автоперенос слов
	virtual bool getWordWrap() = 0;

	//! установка свойства автопереноса слов
	virtual bool setWordWrap(bool isWordWrap) = 0;
};

//! создает экземплляр объекта "Memo", и возвращает указатель
SX_LIB_API ISXGUIMemo* SXGUICrMemoEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "Memo", и возвращает указатель
SX_LIB_API ISXGUIMemo* SXGUICrMemo(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! элемент "ComboBox" - раскрывающийся список с возможностью едиинчного выбора
struct ISXGUIComboBox : public virtual ISXGUIComponent
{
	virtual ~ISXGUIComboBox(){}

	//! вставка текста в строку
	virtual bool insertItem(ID idItem, const char *szStr) = 0;

	//! удаление строки
	virtual bool deleteItem(ID idItem) = 0;

	//! количество строк
	virtual int getCount() = 0;									


	//! выделяет определенную строку
	virtual bool setSel(ID idItem) = 0;

	//! номер выделенной строки
	virtual ID getSel() = 0;									


	//! установка для строки userdata
	virtual bool setItemData(ID idItem, LPARAM lData) = 0;

	//! получение userdata
	virtual LPARAM getItemData(ID idItem) = 0;


	//! удаляет все элементы
	virtual bool clear() = 0;									


	//! вставка текста в строку, подобие замены
	virtual bool setItemText(ID idItem, const char *szStr) = 0;

	//! получить текст из строки 
	virtual void getItemText(ID idItem, char *szBuf) = 0;

	//! получить длину текста из строки
	virtual int getItemTextLength(ID idItem) = 0;

	//! добавляет в конец новую строку
	virtual bool addItem(const char *szStr) = 0;					
};

//! создает экземплляр объекта "ComboBox", и возвращает указатель
SX_LIB_API ISXGUIComboBox* SXGUICrComboBoxEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

/*! создает экземплляр объекта "ComboBox", и возвращает указатель
 \note Параметр style: (parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS
*/
SX_LIB_API ISXGUIComboBox* SXGUICrComboBox(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! элемент "ListBox" - список с возможностью выбора (как единичного так и множественного)
struct ISXGUIListBox : public virtual ISXGUIComponent
{
	virtual ~ISXGUIListBox(){}

	//! установленно ли свойство множественного выделения строк
	virtual bool getStateMultiSel() = 0;
	
	/*! установка состояния мультивыделения
	 \todo протестировать изменение состояния мультивыделения
	*/
	virtual void setStateMultiSel(bool isMultiSel) = 0;


	//! вставить текст в строку с номером index
	virtual bool insertItem(ID idString, const char *szText) = 0;

	//! добавить строку в конец списка
	virtual bool addItem(const char *szText) = 0;

	//! возвращает количество строк
	virtual int getItemCount() = 0;


	//! удалить строку с номером index
	virtual bool deleteItem(ID idString) = 0;


	//! выделить строку с номером index
	virtual bool setSel(ID idString) = 0;
	virtual int	getSel() = 0;


	//! установить userdata для строки с номером index
	virtual bool setItemData(ID idString, LPARAM lData) = 0;

	//! возвращает userdata
	virtual LPARAM getItemData(ID idString) = 0;


	//! очистить 
	virtual bool clear() = 0;								


	//! установить текст в строке с номером index
	virtual bool setItemText(ID idString, const char *szText) = 0;

	//! возвращает текст из строки
	virtual void getItemText(ID idString, char *szBuf) = 0;


	//! возвращает длину текста в строке
	virtual int getItemTextLength(ID idString) = 0;


	//! возвращает количество выделенных строк, если #GetMultipleSel() == true
	virtual int	getMultiSelCount() = 0;					

	//! устанавливает выделена/не выделена строка
	virtual bool setMultiSel(ID idString, bool sel) = 0;

	//! возвращает выделена ли строка
	virtual bool getMultiSel(ID idString) = 0;

	/*! инициализирует ppArr и записывает массив с номерами выделенных строк, возвращает размер массива
	 \todo придумать как упростить, и желательно перевести на String
	*/
	virtual int getMultiSelArr(int **ppArr) = 0;			
};

//! создает экземплляр объекта "ListBox", и возвращает указатель
SX_LIB_API ISXGUIListBox* SXGUICrListBoxEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "ListBox", и возвращает указатель
SX_LIB_API ISXGUIListBox* SXGUICrListBox(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1, bool isMiltipleSel = false);

//#############################################################################

//! элемент "ListView" - таблица с возможностью выбора
struct ISXGUIListView : public virtual ISXGUIComponent
{
	virtual ~ISXGUIListView(){}

	/*! вставить столбец по индексу
	 \todo надо заюзать выделение на стеке через alloca или calloc, а сейчас new
	*/
	virtual int insertColumn(const char *szText, int iColumn, int iWidth) = 0;	

	//! добавить (последним) столбец
	virtual int addColumn(const char *szText, int iWidth) = 0;

	//! удалить столбец по индексу
	virtual void deleteColumn(int iColumn) = 0;

	//! количество столбцов
	virtual int getColumnsCount() = 0;


	//! добавить строку
	virtual int addString(LPARAM lData = -1) = 0;

	//! удалить строку
	virtual void deleteString(int iString) = 0;

	//! количество строк
	virtual int getStringCount() = 0;


	//! установить текст в ячейку
	virtual void setItemText(char *szText, int iColumn, int iString) = 0;

	//! получить текст из ячейки
	virtual void getItemText(char *szText, int iColumn, int iString, int iSizeBuff) = 0;


	//! установить userdata на строку
	virtual void setItemData(int iString, LPARAM lData) = 0;

	//! возвращает userdata строки iString
	virtual LPARAM getItemData(int iString) = 0;


	//! возвращает индекс первого найденного выделения после start (-1 - значит сначала искать)
	virtual int getSelString(int iStart = -1) = 0;	

	//! выделение строки по индексу
	virtual void setSelString(int iString) = 0;

	//! возвращает индекс выделеного столбца
	virtual int getSelColumn() = 0;					

	//! выделение столбца по индексу
	virtual void setSelColumn(int iColumn) = 0;


	//! очистка всей таблицы
	virtual void clear() = 0;						

	//! очистка только строк
	virtual void clearStrings() = 0;				
};

//! создает экземплляр объекта "ListView", и возвращает указатель
SX_LIB_API ISXGUIListView* SXGUICrListViewEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "ListView", и возвращает указатель
SX_LIB_API ISXGUIListView* SXGUICrListView(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

#define SXGUI_DEF_GROUPBOX "SXGUIGROUPBOX"

//! обработчик WM_PAINT для GroupBox
SX_LIB_API LRESULT WndProcGroupBoxPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//! элемент "GroupBox" - контейнер для других элементов, с рамкой и необязательным заголовком, класс переопределен, вся занимаемая область данным элементом принадлжеит ему, и как следствие InvalidateRect приведет к обновлению всей области
struct ISXGUIGroupBox : public virtual ISXGUITextual
{
	virtual ~ISXGUIGroupBox(){}
};

//! создает экземплляр объекта "GroupBox", и возвращает указатель
SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBoxEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "GroupBox", и возвращает указатель
SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBox(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! элемент "ProgressBar" - полоса состояния/прогресса выполнения чего либо
struct ISXGUIProgressBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIProgressBar(){}

	//! возвращает текущую позицию прогресса
	virtual int getPos() = 0;				

	//! устанавливает новую и возвращает старую позицию прогресса
	virtual int setPos(int pos) = 0;		


	//! возвращает текущий размер шага
	virtual int getStep() = 0;				

	//! устанавливает новый и возвращает старый размер шага
	virtual int setStep(int step) = 0;		


	//! установить позицию по количеству шагов, возвращает предшествующую позицию
	virtual int setPosSteps(int steps) = 0;	

	//! увеличить позицию на один шаг, возвращает предшествующую позицию
	virtual int setOneStep() = 0;			


	/*! устанавливает позицию минимума и максимума прогресса
	 \return При удачном выполнении возвращает true, иначе false 
	 */
	virtual bool setMinMax(int min, int max) = 0;


	//! возвращают минимальную позицию прогресса
	virtual int getMin() = 0;	

	//! возвращают максимальную позицию прогресса
	virtual int getMax() = 0;	

	
	//! установить цвет полоски? при удачном выполнении возвращает true, иначе false
	virtual bool setColorBar(DWORD dwColor) = 0;

	//! получить цвет полоски
	virtual DWORD getColorBar() = 0;						


	//! установить цвет фона, при удачном выполнении возвращает true, иначе false
	virtual bool setColorBk(DWORD dwColor) = 0;

	//! возвращает цвет фона
	virtual DWORD getColorBk() = 0;							
};

//! создает экземплляр объекта "GroupBox", и возвращает указатель
SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBarEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

/*! создает экземплляр объекта "GroupBox", и возвращает указатель
 \param vertical true - вертикальное размещение, false - горизонтальное
 \param smooth true - сплошная заливка полосы, false - заливка полосы одинаковыми секциями/прямоугольниками
*/
SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBar(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1, bool isVertical = false, bool isSmooth = true);

//#############################################################################

//! элемент "RadioButton" - переключа́тель (с необязательной подписью), позволяет выбрать одну опцию  из предопределенного набора (таких же RadioButton'ов у одного родителя)
struct ISXGUIRadioButton : public virtual ISXGUIButton
{
	virtual ~ISXGUIRadioButton(){}

	//! установка отметки
	virtual void setCheck(bool check) = 0;	

	//! возвращает отмечен ли
	virtual bool getCheck() = 0;			
};

//! создает экземплляр объекта "RadioButton", и возвращает указатель
SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButtonEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "RadioButton", и возвращает указатель
SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButton(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! Состояния выделения элемента #ISXGUICheckBox
enum SXGUI_CHECKBOX_STATE
{
	SXGUI_CHECKBOX_STATE_UNCHECKED = BST_UNCHECKED,			/*!< не отмечен */
	SXGUI_CHECKBOX_STATE_CHECKED = BST_CHECKED,				/*!< отмечен */
	SXGUI_CHECKBOX_STATE_INDETERMINATE = BST_INDETERMINATE,	/*!< неопределен */
};

//! элемент "CheckBox" - "галочка" (с необязательной подписью) с возможностью выбора состояния отметки
struct ISXGUICheckBox : public virtual ISXGUIButton
{
	virtual ~ISXGUICheckBox(){}

	//! установка состояния отметки #SXGUI_CHECKBOX_STATE
	virtual void setCheckEx(SXGUI_CHECKBOX_STATE check) = 0;

	//! установка состояния отметки
	virtual void setCheck(bool isCheck) = 0;

	//! возвращает состояние отметки SXGUI_CHECKBOX_STATE
	virtual SXGUI_CHECKBOX_STATE getCheck() = 0;
};

//! создает экземплляр объекта "CheckBox", и возвращает указатель
SX_LIB_API ISXGUICheckBox* SXGUICrCheckBoxEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

/*! создает экземплляр объекта "CheckBox", и возвращает указатель
 \param is3State - может ли элемент иметь третье состояние "неопределенный"?
*/
SX_LIB_API ISXGUICheckBox* SXGUICrCheckBox(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1, bool is3State = false);

//#############################################################################

//! элемент "TrackBar" - ползунок
struct ISXGUITrackBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUITrackBar(){}

	//! устанавливает позицию
	virtual void setPos(int iPos) = 0;	

	//! возвращает текущую позицию
	virtual int getPos() = 0;			


	//! устанавливает минимальное (левое) и максимальное (правое) значения позиции
	virtual void setMinMax(int iMin, int iMax) = 0;	

	//! возвращает текущее минимальное (левое) значение позиции
	virtual int getMin() = 0;			

	//! возвращает текущее максимальное (правое) значение позиции
	virtual int getMax() = 0;			


	/*! установка риски в позиции (между минимумом и максимумом)
	 \note Элемент trackbar создает две крайние риски сам (если пределы от минуса (-100 к примеру) до плюса (100 к примеру) то тогда будет создана третья риска в нуле)
	*/
	virtual bool setTick(int iPos) = 0;	

	//! возвращает позициюю риски (между минимумом и максимумом) по индексу риски
	virtual int getTick(int iIndex) = 0;	

	//! установка промежутков между рисками, значения указываются в интервалах приращения (пунктах)
	virtual void setTickFrequency(int iFreq) = 0;	
};

//! создает экземплляр объекта "TrackBar", и возвращает указатель
SX_LIB_API ISXGUITrackBar* SXGUICrTrackBarEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "TrackBar", и возвращает указатель
SX_LIB_API ISXGUITrackBar* SXGUICrTrackBar(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! Выравнивание составных частей внутри #ISXGUIStatusBar при изменении размеров
enum SXGUI_STATUSBAR_ALIGN_RESIZE
{
	SXGUI_STATUSBAR_ALIGN_RESIZE_NONE = 0,		/*!< без изменений */
	SXGUI_STATUSBAR_ALIGN_RESIZE_PERCENT = 1,	/*!< в процентном соотношениям к текущему состоянию */
	SXGUI_STATUSBAR_ALIGN_RESIZE_EQUALLY = 2,	/*!< равномерно одинаково */
};

//! элемент "StatusBar" - горизонтальная полоса в нижней части родительского окна, в ней может отображать различные виды информации о состоянии
struct ISXGUIStatusBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIStatusBar(){}

	//! установка количества частей и их размеров
	virtual bool setPartsCount(
		int iCount,	//!< количество частей, размер pArr
		int *pArr	//!< массив с размерами в пикселях по ширине каждой части
		) = 0;

	//! инициализирует arr (если arr != 0) и записывает в него позиции (правая сторона клиентской области) каждой части, возвращает количество частей 
	virtual int getPartsCount(int **ppArr) = 0;
	
	//! устанавливает в часть текст
	virtual bool setPartText(
		int iPos,			//!< номер части
		const char *szText	//!< текст
		) = 0;

	//! в szBuf записывает текст части iPos, iLenBuf - размер szBuf
	virtual bool getPartText(int iPos, char *szBuf, int iLenBuf) = 0;

	//! обновление с учетом установленного выравнивания, вызывать при изменении размеров
	virtual void update() = 0;		

	//! аналогично #Update
	virtual void updateSize() = 0;	


	//! устанавливает выравнивание при изменении размеров SXGUI_STATUSBAR_ALIGNRS_
	virtual void setAlignReSize(SXGUI_STATUSBAR_ALIGN_RESIZE align_resize) = 0;

	//! возвращает выравнивание при изменении размеров SXGUI_STATUSBAR_ALIGNRS_
	virtual SXGUI_STATUSBAR_ALIGN_RESIZE getAlignReSize() = 0;
};

//! создает экземплляр объекта "StatusBar", и возвращает указатель
SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBarEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//! создает экземплляр объекта "StatusBar", и возвращает указатель
SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBar(const char *szCaption, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//#############################################################################

//! элемент "ToolBar" - панель инструментов
struct ISXGUIToolBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIToolBar(){}
};

//! создает экземплляр объекта "ToolBar", и возвращает указатель
SX_LIB_API ISXGUIToolBar* SXGUICrToolBar(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj = -1);

//**************************************************************************

/*! \name Состояния элементов меню
@{*/

#define SXGUI_MENU_PART_STATE_ENABLED	0	/*!< действительно */
#define SXGUI_MENU_PART_STATE_DISABLED	1	/*!< недействительно */
#define SXGUI_MENU_PART_STATE_GRAYED	2	/*!< недействительно */

//!@}

struct ISXGUIMenuBase : public IBaseObject
{
	virtual ~ISXGUIMenuBase(){}

	//! вставить элемент меню
	virtual bool insertItem(
		int iIndex,			//!< индекс позиции
		const char* text,	//!< текст элемента
		ID idUnit,			//!< идентификатор элемента меню
		ID idSubUnit		//!< идентификатор подменю
		) = 0;

	//! вставить контекстное меню
	virtual bool insertPopupItem(
		int iIndex,			//!< индекс позиции
		const char* text,	//!< текст элемента
		ID idUnit,			//!< идентификатор элемента меню
		ID idSubUnit		//!< идентификатор подменю
		) = 0;

	//! вставить разделитель
	virtual bool insertSeparator(
		int iIndex,			//!< индекс позиции
		ID idUnit,			//!< идентификатор элемента меню
		ID idSubUnit		//!< идентификатор подменю
		) = 0;

	//! удалить элемент меню по его id
	virtual bool deleteItem(ID idUnit) = 0;

	//! возвращает дескриптор подменю по его id
	virtual HMENU getSubMenu(ID idUnit) = 0;


	//! выделить элемент меню
	virtual bool setCheckItem(ID idUnit, bool isCheck) = 0;

	//! установить действительность элементу меню
	virtual bool setEnableItem(ID idUnit, int isEnable) = 0;


	//! выделен ли элемент меню idUnit
	virtual bool getCheckedItem(ID idUnit) = 0;

	//! действителен ли элемент меню idUnit
	virtual int getEnabledItem(ID idUnit) = 0;


	//! возвращает дескриптор меню
	virtual HMENU getHMENU() = 0;
};

//! элемент "Menu" - строка меню окна
struct ISXGUIMenuWindow : public virtual ISXGUIMenuBase
{
	virtual ~ISXGUIMenuWindow(){}

	//! установить меню окну hwnd
	virtual bool setToWindow(HWND hWnd) = 0;
};

//! создает экземплляр объекта "Menu", и возвращает указатель
SX_LIB_API ISXGUIMenuWindow* SXGUICrMenuWindow();

//! создает экземплляр объекта "Menu" из ресурсов по идентификатору uResMenu, и возвращает указатель
SX_LIB_API ISXGUIMenuWindow* SXGUICrMenuWindowEx(UINT uResMenu);

//**************************************************************************

//! элемент "PopupMenu" - контекстное меню
struct ISXGUIPopupMenu : public virtual ISXGUIMenuBase
{
	virtual ~ISXGUIPopupMenu(){}

	//! показать меню
	virtual bool track(
		HWND hWnd,	//!< дескриптор элемента для которого вызывается меню
		int iPosX,	//!< позиция по оси x (screen space)
		int iPosY	//!< позиция по оси y (screen space)
		) = 0;
};

//! создает экземплляр объекта "PopupMenu", и возвращает указатель
SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenu();

//! создает экземплляр объекта "PopupMenu" из ресурсов по идентификатору uResMenu, и возвращает указатель
SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenuEx(UINT uResMenu);

//!@} sxguiwinapi_elements

//#############################################################################

//! Стандартный обработчик событий для всех элементов, исполняет всю очередь событий
SX_LIB_API LRESULT CALLBACK WndProcAllDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//#############################################################################

/*! \name Типы диалогов выбора файлов
@{*/

#define SXGUI_DIALOG_FILE_OPEN 0	/*!< открыватие файла */
#define SXGUI_DIALOG_FILE_SAVE 1	/*!< сохранение файла */

//!@}

//! типы scrollbars
enum SXGUI_SCROLL_TYPE
{
	SXGUI_SCROLL_TYPE_H = 0,	/*!< горизонтальный тип */
	SXGUI_SCROLL_TYPE_V = 1,	/*!< вертикальный тип */
};

//! направления прокрутки scrollbars
enum SXGUI_SCROLL_DIR
{
	SXGUI_SCROLL_DIR_UP = 0,	/*!< направление вверх */
	SXGUI_SCROLL_DIR_DOWN = 1,	/*!< направление вниз */

	SXGUI_SCROLL_DIR_LEFT = 3,	/*!< направление влево */
	SXGUI_SCROLL_DIR_RIGTH = 4,	/*!< направление вправо */
};

//! тип обработчика для диалогов с превью, szPath - целевой путь, szBuf в который будет записан путь до превью, возвращает true если удалось
typedef bool (*HandlerDialogOwndGetPreview) (const char *szPath, char *szBuf);

typedef bool(*HandlerDialogOwndGetPreview2) (const char *szPath, void **pBuf, int *pSizeBuf, int *pWidth, int *pHeight);

//! тип обработчика для диалогов с информацией для выделенного объекта, szPath - целевой путь, szBuf в который будет записана информация, возвращает true если удалось
typedef bool (*HandlerDialogOwndGetInfo) (const char *szPath, char *szBuf);

//! функции для управления gui
namespace gui_func
{
	//! Операции с полосами прокрутки (Scroll bar) элемента
	namespace scrollbar
	{
		//! установка прокрутки
		SX_LIB_API bool	InitScroolBars(
			ISXGUIControl *pControl,	//!< элемент которому устанавливается
			bool canH,					//!< устанавливать ли горизонтальную полосу прокрутки?
			bool canV					//!< устанавливать ли вертикальную полосу прокрутки?
			);

		//! устанвлена ли scrollbar
		SX_LIB_API bool	existsScrollBar(ISXGUIControl *pControl, SXGUI_SCROLL_TYPE scroll_type);

		//! прокрутка содержимого элемента
		SX_LIB_API bool	ScrollLine(
			ISXGUIControl *pControl,		//!< элемент
			SXGUI_SCROLL_TYPE scroll_type,	//!< какую полосу прокручивать? SXGUI_SCROLL_TYPE_
			SXGUI_SCROLL_DIR dir,			//!< в какую сторону прокручивать, SXGUI_SCROLL_DIR_
			int iCountString				//!< на сколько строк прокрутить
			);
	};

	//! Операции с текстом (caption) элемента
	namespace text
	{
		//! установка текста контролу
		SX_LIB_API bool	SetText(ISXGUIControl*const Control, const char* text);

		//! в buf записывает текст контрола, count - длина buf
		SX_LIB_API void	GetText(ISXGUIControl *Control, char* buf, int count);

		//! возвращает длину текста контрола
		SX_LIB_API int	GetTextLen(ISXGUIControl *Control);
	};

	//! функции обработки определенного события посылаемого родительскому элементу у всех его дочерних элементов
	namespace enum_child
	{
		//! для класса SXGUIButtonImg, обработка события активации кнопки и в последствии дезактивации остальных кнопкок
		SX_LIB_API BOOL CALLBACK EnumChildProcUpdateImgButton(HWND hwnd, LPARAM lParam);

		//! событие WM_SIZE
		SX_LIB_API BOOL CALLBACK EnumChildProcUpdateSize(HWND hwnd, LPARAM lParam);

		//! событие WM_MOVE
		SX_LIB_API BOOL CALLBACK EnumChildProcUpdateRect(HWND hwnd, LPARAM lParam);

		//! событие WM_MOUSEMOVE
		SX_LIB_API BOOL CALLBACK EnumChildProcMouseMove(HWND hwnd, LPARAM lParam);
	};

	//! пространство имен с базовыми функциями обработчиками сообщений, используется #WndProcAllDefault
	namespace base_handlers
	{
		/*! установка основных обработчиков, без которых SXGUIWinApi будет работать не стабильно, а возможно и не будет работать,
		если возвращает 0 значит все в норме, иначе возвращает номер того обработчика который не удалось установить,
		данная функция вызывается только если не требуется каких либо дополнительных действий при обработке зарезервированных сообщений
		*/
		SX_LIB_API void InitHandlerMsg(ISXGUIComponent* Component);

		//! обработчик события WM_SIZE
		SX_LIB_API LRESULT SizeChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*! обработчик событий WM_CTLCOLORSTATIC WM_CTLCOLOREDIT WM_CTLCOLORBTN WM_CTLCOLORLISTBOX,
		то есть обработка цветов дочерних элементов окна (static, edit,button, listbox)
		*/
		SX_LIB_API LRESULT CtlColorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//! обработчик события WM_SIZING
		SX_LIB_API LRESULT SizingChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//! обработчик события WM_MOVE
		SX_LIB_API LRESULT MoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//! обработчик события WM_MOUSEMOVE
		SX_LIB_API LRESULT MouseMoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//! обработчик события WM_SETCURSOR
		SX_LIB_API LRESULT SetCursorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//! обработчик события WM_COMMAND SC_MAXIMIZE
		SX_LIB_API LRESULT MaximuzeWinChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//! обработчик события WM_DESTROY
		SX_LIB_API LRESULT WinDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

	//! функции диалогов
	namespace dialogs
	{
		/*! диалог выбора файла
		\note path или name должен быть не 0, иначе результат вызова диалога некуда будет записать и как следствие диалог не будет вызван
		*/
		SX_LIB_API void SelectFileStd(
			int type,				//!< тип диалога SXGUI_DIALOG_FILE_
			char* path,				//!< если не 0 то запишется путь до файла
			char* name,				//!< если не 0 то запишется имя файла
			const char* stdpath,	//!< путь относительно которого открывать диалог
			const char* filter		//!< фильтр расширений
			);

		/*! свой диалог выбора директории 
		 \note szOutName должен быть не 0, иначе результат вызова диалога некуда будет записать
		*/
		SX_LIB_API bool SelectDirOwn(
			char *szOutName,			//!< имя выбранной директории
			char *szOutPath,			//!< полный путь вместе с выбранной директорией
			const char *szStartPath,	//!< начальный путь
			const char *szDialogName = 0,//!< название окна, если надо
			bool canExplore = true,		//!< возможно ли перемещаться по директориям, если нужен выбор только из одной директории то false
			bool canCreateNew = true,	//!< возможно ли создание новой директории
			const char *szDownPath = 0,	//!< путь ниже которого нельзя заходить
			HandlerDialogOwndGetPreview lpfnHandlerPreview = 0 //!< если требуется предпросмотр тогда нужно указать обработчик #HandlerDialogOwndSelDir, который получит аргумент с полным текущим путем, а должен вернуть полный путь на bmp файл
			);

		/*! свой диалог выбора директории
		\note szOutName должен быть не 0, иначе результат вызова диалога некуда будет записать
		*/
		SX_LIB_API bool SelectFileOwn(
			char *szOutName,			//!< имя выбранной директории
			char *szOutPath,			//!< полный путь вместе с выбранной директорией
			const char *szStartPath,	//!< начальный путь
			const char *szFilterExt,	//!< расширение файлов, которые надо отобразить, либо 0 или "" если нужны все файлы
			const char *szDialogName = 0,//!< название окна, если надо
			bool canExplore = true,		//!< возможно ли перемещаться по директориям, если нужен выбор только из одной директории то false
			const char *szDownPath = 0,	//!< путь ниже которого нельзя заходить,
			HWND hWndLock = 0,			//!< окно для блокировки во время работы диалога
			HandlerDialogOwndGetPreview2 lpfnHandlerPreview = 0, //!< если требуется предпросмотр тогда нужно указать обработчик #HandlerDialogOwndSelDir, который получит аргумент с полным текущим путем, а должен вернуть полный путь на bmp файл
			HandlerDialogOwndGetInfo lpfnHandlerInfo = 0
			);
	};
};

#endif

//!@} sxguiwinapi
