
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл библиотеки графического пользовательского интерфейса посредством WinApi
*/

/*! \defgroup sxguiwinapi sxguiwinapi - библиотека графического пользовательского интерфейса реализованного на WinApi
 \note Для начала нужно инициализировать как внутренние библиотеки так и новые компоненты зарегистрировать:
\code

SXGUIRegClass::RegButtonImg();
SXGUIRegClass::RegGroupBox();

INITCOMMONCONTROLSEX icex;

icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
icex.dwICC  = ICC_BAR_CLASSES;
InitCommonControlsEx(&icex);
\endcode
@{
*/

#ifndef __sxguiwinapi
#define __sxguiwinapi

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

/*! \name Определения для полос прокрутки (Scrool bar)
@{*/

#define SXGUI_SCROLL_TYPE_HORZ	0	/*!< горизонтальный тип */
#define SXGUI_SCROLL_TYPE_VERT	1	/*!< вертикальный тип */

#define SXGUI_SCROLL_DIR_UP		0	/*!< направление вверх */
#define SXGUI_SCROLL_DIR_DOWN	1	/*!< направление вниз */

#define SXGUI_SCROLL_DIR_LEFT	0	/*!< направление влево */
#define SXGUI_SCROLL_DIR_RIGTH	1	/*!< направление вправо */

//!@}

//! максимальное количество обработчиков
#define SXGUI_COUNT_HANDLERS_MSG_IN_ARR 256

//! тип обработчика
typedef LRESULT(*HandlerMsg) (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! структура данных для обработчика
struct SXHandlerMsgStruct
{
	HandlerMsg Handler;		//!< сама функция, которая будет вызываться
	UINT Msg;				//!< код сообщения
	WPARAM WParam;			//!< WParam сообщения
	bool BFCondiderWParam;	//!< проверять WParam?
	LPARAM LParam;			//!< LParam сообщения
	bool BFCondiderLParam;	//!< проверять LParam
	bool IsMainFunction;	//!< главный ли это обработчик? true - значение возвращенное фунцией - возвращается WndProc, иначе нет 
};

//! структура, логическая, для определения тянуться ли стороны элемента за сторонами родителя
struct SXRectBool
{
	SXRectBool()
	{
		this->top = false;
		this->left = false;
		this->bottom = false;
		this->right = false;
	}
	SXRectBool(bool _top, bool _left, bool _bottom, bool _right)
	{
		this->top = _top;
		this->left = _left;
		this->bottom = _bottom;
		this->right = _right;
	}
	bool top;
	bool left;
	bool bottom;
	bool right;
};

/*! \name Выравнивание текста в элементе
@{*/

#define SXGUI_TEXT_ALIGN_LEFT	0	/*!< по левому краю */
#define SXGUI_TEXT_ALIGN_RIGHT	1	/*!< по правому краю */
#define SXGUI_TEXT_ALIGN_CENTER	2	/*!< по центру */

//!@}

//#############################################################################

/*! 
первый базовый класс, единственным родителем не используется, 
определяет объект как контрол, то есть содержит в себе минимум данных для управления объектом
*/
struct ISXGUIControl : public IBaseObject
{
	virtual ~ISXGUIControl(){};
	virtual void Init(HWND window_handle, HWND parent_handle, WNDPROC handler) = 0;	//!< запоминаем дескрипторы, и устанавливаем новый обработчик

	virtual HWND Parent() = 0;						//!< возвращает HWND родителя
	virtual bool Parent(HWND parent_handle) = 0;	//!< устанавливает родителя

	virtual HWND GetHWND() = 0;						//!< возвращает свой HWND

	virtual void Visible(bool bf) = 0;				//!< установка видимости контрола
	virtual bool Visible() = 0;						//!< видим ли?

	virtual void SetFocus() = 0;					//!< установить фокус
	virtual bool Focus() = 0;						//!< установлен ли фокус?
	
	WNDPROC OldProc;								//!< старый обработчик

	/*! \name Пользовательские данные
	@{*/

	virtual void * GetUserPtr() = 0;				//!< получить пользовательские данные
	virtual void * SetUserPtr(void *) = 0;			//!< установить пользовательские данные

	//!@}
};


//! смотреть #ISXGUIHint
struct ISXGUIHint;

/*! Компонент - второй в очереди родитель, часто наследование идет именно от него,
более расширенное управление большинством элементов*/
struct ISXGUIComponent : public virtual ISXGUIControl
{
	virtual ~ISXGUIComponent(){};

	/*! устанавливает необходимые данные для управления элементом
	 \note Необходимо вызывать после инициализации, то есть когда HWND уже получен*/
	virtual void InitComponent() = 0; 

	virtual bool Enable() = 0;			//!< действителен ли элемент
	virtual void Enable(bool bf) = 0;	//!< установка действительности элемента

	//! установка шрифта (с указанием значений)
	virtual void SetFont(
		const char* name,	//!< имя шрифта, если name == 0 то не изменияем название шрифта, если name[0] == 0 то дефолтный шрифт gui
		int height,			//!< высота, если -1 то не учитываются
		int width,			//!< ширина, если -1 то не учитываются
		int weight,			//!< толщина от 1 до 1000 (400 - нормальный, 700 - полужирный), если -1 то не учитываются, 0 - толщина по умолчанию
		int italic,			//!< использовать ли стиль "наклонный", 1 - да, 0 - нет, -1 не учитываются
		int underline,		//!< использовать ли стиль "подчеркнутый", 1 - да, 0 - нет, -1 не учитываются
		int strike_out		//!< использовать ли стиль "зачеркнутый", 1 - да, 0 - нет, -1 не учитываются
		) = 0;

	
	virtual void SetFont(HFONT hfont) = 0;	//!< установка шрифта из дескриптора
	virtual HFONT GetFont() = 0;			//!< возвращает дексриптор шрифта

	virtual bool IsParentFont() = 0;	//!< наследуется ли шрифт родителя
	virtual HFONT ParentFont() = 0;		//!< получает родительский шрифт


	//! модификация стилей
	virtual bool ModifyStyle(
		long style_add,	//!< добавляемые стили, иначе 0
		long style_del	//!< удаляемые стили, иначе 0
		) = 0;

	//! миодицикация расширенных стилей
	virtual bool ModifyExStyle(
		long style_add,	//!< добавляемые стили, иначе 0
		long style_del	//!< удаляемые стили, иначе 0
		) = 0;

	//! устанавливает область окна RECT элемента в глобальных координатах
	virtual bool	SetWinRect(
		RECT* rect, //!< устанавливаемый RECT
		bool alignment_screen_space	//!< использовать ли только работчую область (рабочий стол кроме панели задач)
		) = 0;
	//! возвращает область окна RECT элемента в глобальных координатах
	virtual void	GetWinRect(RECT* rect) = 0;

	//! устанавливает клиентскую область RECT элемента в глобальных координатах
	virtual bool	SetClientRect(
		RECT* rect,	//!< устанавливаемый RECT
		bool alignment_screen_space	//!< использовать ли только рабочую область (рабочий стол кроме панели задач)
		) = 0;	
	//! возвращает клиентскую область RECT элемента в глобальных координатах
	virtual void	GetClientRect(RECT* rect) = 0;

	/*! \name Всплывающая подсказка
	 \note Первая установка видимости ShowHint(true) либо первая устновка текста #SetHintText инициализируют объект подсказки, до первого вызова функции #GetHintText записывают/возвращают 0
	@{*/

	virtual void ShowHint(bool bf) = 0;	//!< установка состояния видимости подсказки
	virtual bool ShowHint() = 0;		//!< возвращает: видима ли подсказка?

	virtual void SetHintText(const char* text) = 0;	//!< установить текст подсказки
	virtual const char* GetHintText() = 0;			//!< возвращает текст текущей подсказки
	virtual void GetHintText(char* buf) = 0;		//!< в buf записывает текст текущей подсказки

	//!@}

	
	/*! \name Методы для цветов элемента
	@{*/

	//set/get цвет текста
	virtual void SetColorText(BYTE r, BYTE g, BYTE b) = 0;	//!< покомпонентная установка цвета текста
	virtual void SetColorText(DWORD color) = 0;				//!< установка цвета текста
	virtual DWORD GetColorText() = 0;						//!< возвращает текущее значение цвета текста

	//set/get прозрачность заднего фона текста
	virtual void SetTransparentTextBk(bool bf) = 0;			//!< установка прозрачности заднего фона
	virtual bool GetTransparentTextBk() = 0;				//!< установлена ли прозрачность заднего фона?

	//set/get цвет заднего фона текста
	virtual void SetColorTextBk(BYTE r, BYTE g, BYTE b) = 0;//!< покомпонентная установка цвета заднего фона текста
	virtual void SetColorTextBk(DWORD color) = 0;			//!< установка цвета заднего фона текста
	virtual DWORD GetColorTextBk() = 0;						//!< возвращает текущее значение цвета заднего фона текста

	//set/get цвет фона элемента
	virtual bool SetColorBrush(BYTE r, BYTE g, BYTE b) = 0;	//!< покомпонентная установка цвета фона
	virtual bool SetColorBrush(DWORD color) = 0;			//!< установка цвета фона
	virtual DWORD GetColorBrush() = 0;						//!< возвращает текущее значение цвета фона
	virtual HBRUSH GetBrush() = 0;							//!< возвращает дескриптор "кисти" фона

	//!@}

	/*! \name Функции для работы с обработчиками
	Пример добавления основного обработчика: Wnd->AddHandler(MouseWheel,WM_MOUSEWHEEL,0,0,0,0,true);\n
	Пример добавления второстепенного обработчика: Wnd->AddHandler(MouseWheel,WM_MOUSEWHEEL);
	 \note функция обработчик должна соответствовать стандартной функции обработчику WinApi
	 \note main функция-обработчик может быть только одна и должна быть вообще, 
	но если ее нет то в качестве возвращаемого значения будет использовано значение вызова функции: 
	 - для окон DefWindowProc(hwnd, msg, wParam, lParam),
	 - для остальных компонентов CallWindowProc(Component->OldProc,hwnd, msg, wParam, lParam)
	@{*/

	//! добавление обработчика
	virtual bool AddHandler(
		HandlerMsg Handler, //!< функция обработчик
		UINT Msg,			//!< сообщение реакция на которое обрабатывает функция обработчик
		WPARAM wParam,		//!< wParam функции обработчика при котором следует производить вызов функции обработчика
		bool considerWparam,//!< учитывать ли wParam? если true то прежде вызова функции обработчика будет сравнение на равенство поступившего аргумента wParam с тем что указано было при вызове этой функции
		LPARAM lParam,		//!< lParam функции обработчика при котором следует производить вызов функции обработчика
		bool considerLparam,//!< учитывать ли lParam? если true то прежде вызова функции обработчика будет сравнение на равенство поступившего аргумента lParam с тем что указано было при вызове этой функции 
		bool isMain			//!< главная ли эта функция обработчик? если true то ее возвращаемое значение будет послано как результат обработки сообщения
		) = 0;

	/*! добавление второстепенного обработчика
	 \note Все что необъявлено из функции #AddHandler не учитывается
	*/
	virtual bool AddHandler(
		HandlerMsg Handler, //!< функция обработчик
		UINT Msg			//!< сообщение реакция на которое обрабатывает функция обработчик
		) = 0;	

	//key - ключ в массиве обработчиков
	virtual HandlerMsg GetHandlerFunction(int key) = 0;	//!< возвращает функцию обработчик по порядковому номеру
	virtual int GetCountKeyArrHandler() = 0;			//!< количество ключей в массиве обработчиков

	//все что касается данных сообщения
	virtual UINT GetMsgHandler(int key) = 0;			//!< код сообщения обработчика, по порядковому номеру key в общем массиве обработчиков для данного элемента
	virtual WPARAM GetWParamHandler(int key) = 0;		//!< wParam обработчика, по порядковому номеру key в общем массиве обработчиков для данного элемента
	virtual bool GetConsiderWParamHandler(int key) = 0;	//!< учитывать WParam? по порядковому номеру key в общем массиве обработчиков для данного элемента
	virtual LPARAM GetLParamHandler(int key) = 0;		//!< lParam обработчика, по порядковому номеру key в общем массиве обработчиков для данного элемента
	virtual bool GetConsiderLParamHandler(int key) = 0;	//!< учитывать LParam? по порядковому номеру key в общем массиве обработчиков для данного элемента

	virtual bool IsMainFunction(int key) = 0;	//!< это главная функция? по порядковому номеру key в общем массиве обработчиков для данного элемента
	virtual LRESULT ExecuteHandler(int key, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;	//!< вызвать функцию обработчик
	
	//!@}

	/*! \name Обновление позиций относительно родителя
	@{*/

	virtual void UpdateSize() = 0;	//!< обновление координат и размеров элемента относительно его родителя, обычно вызывается установленными обработчиками
	virtual void UpdateRect() = 0;	//!< обновление внутрненних данных для #UpdateSize

	SXRectBool GAlign;	//!< данные регулирующие размеры элемента при изменении размеров его родителя, каждый компонент SXRectBool установленный в true привязывает сторону элемента к его родителю изменяя ее при манипуляциях с изменением стороны с родителем

	//!@}

	/*! \name Данные регулирующие будет ли передвигаться определнная сторона при изменении размеров с этой стороны
	 \note true - можно тянуть за сторону изменяя размер, false - тянуть за сторону не получится
	@{*/

	bool BFSizingChangeTop;		//!< верх
	bool BFSizingChangeBottom;	//!< низ
	bool BFSizingChangeRight;	//!< право
	bool BFSizingChangeLeft;	//!< лево

	//!@}

	//bool BFMinSize;	//!< установлен ли минимальный размер? если да то меньше чем #MinSizeX и #MinSizeY не будет уменьшаться
	WORD MinSizeX;	//!< минимальный размер окна по ширине
	WORD MinSizeY;	//!< минимальный размер окна по высоте

};

/*! третий (если нужен текст) родитель, непосредтсвенно не используется,
характеризует элемент как содержащий единый текст (аля кэпшен) текстовый компонент
*/
struct ISXGUITextual : public virtual ISXGUIComponent
{
	virtual ~ISXGUITextual(){};
	virtual bool SetText(const char* text) = 0;		//!< установка текста
	virtual void GetText(char* buf, int size) = 0;	//!< записывает в buf текущий текст элемента, не больше size
	virtual int GetTextLen() = 0;					//!< возвращает размер строки
};

//#############################################################################

/*! \defgroup sxguiwinapi_elements Элементы интерфейса
 \note Инициализировав элемент можно установить на него стандартные обработчики:
\code
SXGUIBaseHandlers::InitHandlerMsg(HWND);
\endcode
 \note Во многих функциях создания элементов используются одни и теже именования аргументов:
 - const char* caption - название окна
 - WORD x,				- позиция по оси x
 - WORD y,				- позиция по оси y
 - WORD width,			- ширина
 - WORD heigth,			- высота
 - HCURSOR cursor,		- дескриптор курсора, 0 - в случае отсутствия
 - HBRUSH brush,		- дескриптор кисти окна (цвета фона), 0 - в случае отсутствия
 - DWORD exstyle,		- расширенные стили WS_EX_
 - DWORD wndstyle,		- стили окна CS_
 - DWORD style,			- стили элемента WS_
 - HWND parent,			- дескриптор родителя
 - WNDPROC handler = 0	- функция обработчик, если 0 то будет назначен DefWindowProc
@{*/

//! размер текста подсказки #ISXGUIHint
#define SX_HINT_COUNT_BUF_SIZE 1024

//! элемент "Hint" - всплывающая подсказка при наведении на элемент
struct ISXGUIHint : public IBaseObject
{
	virtual ~ISXGUIHint(){}

	//! установка времени ожидания и показа
	virtual void SetDelayTime(
		DWORD init,		//!< сколько ждать до появления подсказки после наведения курсора
		DWORD autopop	//!< время показа
		) = 0;

	virtual bool Parent(HWND parent) = 0;	//!< устанавливает родителя (при наведении на родителя всплывает эта подсказка)
	virtual HWND Parent() = 0;				//!< возвращает родителя

	virtual HWND GetHWND() = 0;				//!< возвращает свой HWND

	virtual void Visible(bool bf) = 0;		//!< установка видимости
	virtual bool Visible() = 0;				//!< видно ли?

	virtual void SetText(const char* text) = 0;		//!< установка текста подсказки
	virtual const char* GetText() = 0;				//!< возвращает указатель на текущий текст подсказки, очищается при вызове деструктора
	virtual void GetText(char* buf) = 0;			//!< записывает в buf текущий текст подсказки
};

//! создание объекта "Hint" для элемента parent
SX_LIB_API ISXGUIHint* SXGUICrHint(HWND parent);

////////////////////////////////////////////////////////////////////////////

/*! элемент "Window" - окно. 
По дефолту обработчиком назначается DefWindowProc, 
чтобы функционировали свойства SXGUIComponent необходимо указать стандартный для всех элементов обработчик WndProcAllDefault
*/
struct ISXGUIBaseWnd : public virtual ISXGUITextual
{
	virtual ~ISXGUIBaseWnd(){};
	
	//set/get цвет фона элемента
	virtual bool SetColorBrush(BYTE r, BYTE g, BYTE b) = 0;	//!< покомпонентная установка цвета фона
	virtual bool SetColorBrush(DWORD color) = 0;			//!< установка цвета фона
	virtual DWORD GetColorBrush() = 0;						//!< возвращает текущее значение цвета фона
	virtual HBRUSH GetBrush() = 0;							//!< возвращает дескриптор "кисти" фона

	/*! \name Прозрачность окна
	 \note Коэфициент прозрачности лежит в пределах [0,255] где 0 - полностью прозрачное, 255 - полностью непрозрачное
	@{*/

	virtual bool Alpha(WORD alpha) = 0;		//!< установить коэфициент прозрачности
	virtual WORD Alpha() = 0;				//!< возвращает коэфициент прозрачности
	virtual bool AlphaBlend(bool bf) = 0;	//!< установка прозрачного стиля (true - использовать коэфициент прозрачности, false - не использовать)
	virtual bool AlphaBlend() = 0;			//!< установлен ли прозрачный стиль?
	
	//!@}
};

//! создает экземплляр объекта "Window", и возвращает указатель
SX_LIB_API ISXGUIBaseWnd* SXGUICrBaseWnd(
	const char* class_name, //!< имя класса окна
	const char* caption,	//!< название окна
	const char* menu,		//!< имя ресурса меню класса, которое как имя показывается в файле ресурса, 0 - в случае отсутствия
	WORD menu_id,			//!< идентификатор меню в ресурсах, 0 - в случае отсутствия
	WORD x,					//!< позиция по оси x
	WORD y,					//!< позиция по оси y
	WORD width,				//!< ширина
	WORD heigth,			//!< высота
	HICON icon,				//!< дескриптор иконки, 0 - в случае отсутствия
	HCURSOR cursor,			//!< дескриптор курсора, 0 - в случае отсутствия
	HBRUSH brush,			//!< дескриптор кисти окна (цвета фона), 0 - в случае отсутствия
	DWORD exstyle,			//!< расширенные стили WS_EX_
	DWORD wndstyle,			//!< стили окна CS_
	DWORD style,			//!< стили элемента WS_
	HWND parent,			//!< дескриптор родителя
	WNDPROC handler = 0		//!< функция обработчик, если 0 то будет назначен DefWindowProc
	);

////////////////////////////////////////////////////////////////////////////

//! элемент "Static" простое текстовое поле без возможности пользовательского редактирования, также можно создать при помощи него линию горизонтальную/вертикальную
struct ISXGUIStatic : public virtual ISXGUITextual
{
	virtual ~ISXGUIStatic(){}
	virtual int Align() = 0;			//!< возвращает тип выравнивания текста SXGUI_TEXT_ALIGN_
	virtual bool Align(int align) = 0;	//!< устанавливает выравнивание текста SXGUI_TEXT_ALIGN_

	virtual bool WordWrap() = 0;		//!< установлен ли автоперенос строк
	virtual bool WordWrap(bool bf) = 0; //!< манипуляция со свойством автоперенос строк
};

//! создает экземплляр объекта "Static", и возвращает указатель
SX_LIB_API ISXGUIStatic* SXGUICrStaticEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "Static", и возвращает указатель
SX_LIB_API ISXGUIStatic* SXGUICrStatic(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

/*! создает экземплляр объекта "Static" который представлен линией, и возвращает указатель
 \param vertical - вертикальная линия (true) или горизонтальная (false)
*/
SX_LIB_API ISXGUIStatic* SXGUICrStaticLine(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool vertical);

////////////////////////////////////////////////////////////////////////////

/*! \name Типы загружаемого фона элемента #ISXGUIButton
@{*/

#define SXGUI_BUTTON_IMAGE_NONE		0	/*!< не использовать загружаемый фон */
#define SXGUI_BUTTON_IMAGE_ICON		1	/*!< загружать иконку (.ico) */
#define SXGUI_BUTTON_IMAGE_BITMAT	2	/*!< загружать изображение (.bmp) */

//!@}

//! элемент "button" - кнопка
struct ISXGUIButton : public virtual ISXGUITextual
{
	virtual ~ISXGUIButton(){}
	virtual bool SetIconInFile(const char* path) = 0;	//!< загрузка на фон иконки (.ico)
	virtual bool SetBmpInFile(const char* path) = 0;	//!< загрузка на фон изображения (.bmp)

	virtual bool SetIconInResourse(WORD id) = 0;		//!< подгрузка на фон иконки (.ico) из ресурсов
	virtual bool SetBmpInResourse(WORD id) = 0;			//!< подгрузка на фон изображения (.bmp) из ресурсов
};

//! создает экземплляр объекта "button", и возвращает указатель
SX_LIB_API ISXGUIButton* SXGUICrButtonEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

/*! создает экземплляр объекта "button", и возвращает указатель
 \param image тип загружаемого фона из возможных вариантов SXGUI_BUTTON_IMAGE_*/
SX_LIB_API ISXGUIButton* SXGUICrButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, int image, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! обработчик для элемента #ISXGUIButtonImg
SX_LIB_API LRESULT WndProcButtonImgDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	virtual void ReInitImage(const char* path) = 0;

	//! в buf записывает путь до текущего загруженного изображения
	virtual void GetPathForImg(char* buf) = 0;

	//! установка необходимых обработчиков, вызывать сразу после создания
	virtual void InitCallBack() = 0;

	virtual void SetColorFrame(BYTE r, BYTE g, BYTE b) = 0;	//!< покомпонентная установка цвета рамки
	virtual void SetColorFrame(DWORD color) = 0;			//!< установка цвета рамки
	virtual DWORD GetColorFrame() = 0;						//!< возвращает текущее значение цвета рамки

	virtual void SetColorBk(BYTE r, BYTE g, BYTE b) = 0;	//!< покомпонентная установка цвета заднего фона
	virtual void SetColorBk(DWORD color) = 0;				//!< установка цвета заднего фона
	virtual DWORD GetColorBk() = 0;							//!< возвращает текущее значение цвета заднего фона

	virtual void SetColorAlpha(BYTE r, BYTE g, BYTE b) = 0;	//!< покомпонентная установка отсекаемого цвета в изображении
	virtual void SetColorAlpha(DWORD color) = 0;			//!< установка отсекаемого цвета в изображении
	virtual DWORD GetColorAlpha() = 0;						//!< возвращает текущее значение отсекаемого цвета в изображении
	

	virtual void SetEnable(bool bf) = 0;	//!< установка состояния кнопки (активна/пассивна)
	virtual bool GetEnable() = 0;			//!< возвращает состояния кнопки (активна/пассивна)
	
	virtual void SetEnableActive(bool bf) = 0;	//!< установка состояния активности кнопки (включена/выключена)
	virtual bool GetEnableActive() = 0;			//!< включено ли изменение активности кнопки?

	virtual void SetParentGroup(bool bf) = 0;	//!< установить входит/не входит кнопка в группу остальных таких же кнопок своего родителя
	virtual bool GetParentGroup() = 0;			//!< входит ли кнопка в группу остальных таких же кнопок своего родителя?
};

/*! создает экземплляр объекта "ButtonImg", и возвращает указатель
 \param path путь до загружаемого изображения (.bmp)
 \param alpha_color исключаемый цвет 
 \param bk_color цвет фона кнопки
*/
SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgLoad(const char* path, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, DWORD id);

/*! создает экземплляр объекта "ButtonImg", и возвращает указатель
 \param button идентификатор ресурса подгружаемого изображения (.bmp)
 \param alpha_color исключаемый цвет
 \param bk_color цвет фона кнопки
*/
SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgRes(WORD button, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! элемент "Edit" - текстовове поле с возможностью пользовательского редактирования
struct ISXGUIEdit : public virtual ISXGUITextual
{
	virtual ~ISXGUIEdit(){}
	virtual bool ReadOnly() = 0;		//!< установлено ли свойство "только для чтения"
	virtual bool ReadOnly(bool bf) = 0;	//!< манипулирует свойством "только для чтения", возвращает результат действия

	//дефолтово скрытый текст заменяется на *
	virtual bool Password() = 0;		//!< установлено ли свойств скрытия текста
	virtual bool Password(bool bf) = 0;	//!< манипулирует свойством скрытия текста

	virtual int Align() = 0;			//!< возвращает флаг выравнивания текста SXGUI_TEXT_ALIGN_
	virtual bool Align(int align) = 0;	//!< устанавливает выравнивание SXGUI_TEXT_ALIGN_
};

//! создает экземплляр объекта "Edit", и возвращает указатель
SX_LIB_API ISXGUIEdit* SXGUICrEditEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "Edit", и возвращает указатель
SX_LIB_API ISXGUIEdit* SXGUICrEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! элемент "UpDown" - кнопки увеличения/уменьшения, обычно используется совместно с элементом "Edit" (#ISXGUIEdit)
struct ISXGUIUpDown : public virtual ISXGUIComponent
{
	virtual ~ISXGUIUpDown(){}

	virtual void SetMinMax(long min, long max) = 0; //!< установка минимального и максимального значений
	virtual long SetPos(long pos) = 0;				//!< устанавливает текущую позицию (число)

	virtual long GetMin() = 0;	//!< возвращает минимальное число
	virtual long GetMax() = 0;	//!< возвращает максимальное число
	virtual long GetPos() = 0;	//!< возвращает текущую позицию (число)

	virtual HWND SetBuddy(HWND buddy) = 0;	//!< устанавливает контроль/управление над значением в дескприторе buddy элемента
	virtual HWND GetBuddy() = 0;			//!< возвращает HWND элемента над которым осуществляется контроль/управление
};

//! создает экземплляр объекта "UpDown", и возвращает указатель
SX_LIB_API ISXGUIUpDown* SXGUICrUpDownEx(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id, HWND buddy);

/*! создает экземплляр объекта "UpDown", и возвращает указатель
 \param buddy HWND элемента (обычно "Edit" #ISXGUIEdit) над которым устанавливается контроль и управление
 \param align_left true - выравнивание по левому краю, false - выравнивание по правому краю
*/
SX_LIB_API ISXGUIUpDown* SXGUICrUpDown(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, HWND buddy, bool align_left);

////////////////////////////////////////////////////////////////////////////

//! элемент "Memo" - текстовое поле с возможностью создания новых строк
struct ISXGUIMemo : public virtual ISXGUIEdit
{
	virtual ~ISXGUIMemo(){}
	virtual bool WordWrap() = 0;		//!< установлен ли автоперенос слов
	virtual bool WordWrap(bool bf) = 0;	//!< установка свойства автопереноса слов

	//SXGUIFuctinon
	virtual bool ScrollBars(bool h, bool v) = 0;					//!< 
	virtual bool ScrollBarV() = 0;									//!< 
	virtual bool ScrollBarH() = 0;									//!< 
	virtual bool ScrollLine(WORD scroll, WORD dir, int count) = 0;	//!< 
};

//! создает экземплляр объекта "Memo", и возвращает указатель
SX_LIB_API ISXGUIMemo* SXGUICrMemoEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "Memo", и возвращает указатель
SX_LIB_API ISXGUIMemo* SXGUICrMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! элемент "ComboBox" - раскрывающийся список с возможностью едиинчного выбора
struct ISXGUIComboBox : public virtual ISXGUIComponent
{
	virtual ~ISXGUIComboBox(){}
	virtual bool	InsertItem(int index, const char* text) = 0;	//!< вставка текста в строку
	virtual bool	DeleteItem(int index) = 0;						//!< удаление строки

	virtual int		GetCount() = 0;									//!< количество строк

	virtual bool	SetSel(int index) = 0;							//!< выделяет определенную строку
	virtual int		GetSel() = 0;									//!< номер выделенной строки

	virtual bool	SetItemData(int index, LPARAM data) = 0;		//!< установка для строки userdata (32 бита)
	virtual LPARAM	GetItemData(int index) = 0;						//!< получение userdata

	virtual bool	Clear() = 0;									//!< удаляет все элементы

	virtual bool	SetItemText(int index, const char* text) = 0;	//!< вставка текста в строку, подобие замены
	virtual void	GetItemText(int index, char* buf) = 0;			//!< получить текст из строки 
	virtual int		GetItemTextLength(int index) = 0;				//!< получить длину текста из строки
	virtual bool	AddItem(const char* text) = 0;					//!< добавляет в конец новую строку
};

//! создает экземплляр объекта "ComboBox", и возвращает указатель
SX_LIB_API ISXGUIComboBox* SXGUICrComboBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

/*! создает экземплляр объекта "ComboBox", и возвращает указатель
 \note Параметр style: (parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS
*/
SX_LIB_API ISXGUIComboBox* SXGUICrComboBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! элемент "ListBox" - список с возможностью выбора (как единичного так и множественного)
struct ISXGUIListBox : public virtual ISXGUIComponent
{
	virtual ~ISXGUIListBox(){}
	virtual bool	MultipleSel() = 0;							//!< установленно ли свойство множественного выделения строк

	virtual bool	InsertItem(int index, const char* text) = 0;//!< вставить текст в строку с номером index
	virtual bool	AddItem(const char* text) = 0;				//!< добавить строку в конец списка
	virtual int		GetCountItem() = 0;							//!< возвращает количество строк

	virtual bool	DeleteItem(int index) = 0;					//!< удалить строку с номером index

	virtual bool	SetSel(int index) = 0;						//!< выделить строку с номером index
	virtual int		GetSel() = 0;

	virtual bool	SetItemData(int index, LPARAM data) = 0;	//!< установить userdata для строки с номером index
	virtual LPARAM	GetItemData(int index) = 0;					//!< возвращает userdata

	virtual bool	Clear() = 0;								//!< очистить

	virtual bool	SetTextItem(int index, const char* text) = 0;//!< установить текст в строке с номером index
	virtual void	GetItemText(int index, char* buf) = 0;		//!< возвращает текст из строки

	virtual int		GetItemTextLength(int index) = 0;			//!< возвращает длину текста в строке

	virtual int		GetMultipleSelCount() = 0;					//!< возвращает количество выделенных строк, если #GetMultipleSel() == true

	virtual bool	SetMultipleSel(int index, bool sel) = 0;	//!< устанавливает выделена/не выделена строка
	virtual bool	GetMultipleSel(int index) = 0;				//!< возвращает выделена ли строка

	virtual void	GetMultipleSelArr(int** arr) = 0;			//!< возвращает массив с номерами выделенных строк

	//SXGUIFuctinon
	virtual bool ScrollBars(bool h, bool v) = 0;
	virtual bool ScrollBarV() = 0;
	virtual bool ScrollBarH() = 0;
	virtual bool ScrollLine(WORD scroll, WORD dir, int count) = 0;
};

//! создает экземплляр объекта "ListBox", и возвращает указатель
SX_LIB_API ISXGUIListBox* SXGUICrListBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "ListBox", и возвращает указатель
SX_LIB_API ISXGUIListBox* SXGUICrListBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool miltiple_sel);

////////////////////////////////////////////////////////////////////////////

//! элемент "ListView" - таблица с возможностью выбора
struct ISXGUIListView : public virtual ISXGUIComponent
{
	virtual ~ISXGUIListView(){}

	virtual int InsertColumn(char* text, int col, int width) = 0;	//!< вставить столбец по индексу
	virtual int AddColumn(char* text, int width) = 0;				//!< добавить (последним) столбец
	virtual void DeleteColumn(int col) = 0;							//!< удалить столбец по индексу
	virtual int GetCountColumns() = 0;								//!< количество столбцов

	virtual int AddString(long data = -1) = 0;				//!< добавить строку
	virtual void DeleteString(int str) = 0;		//!< удалить строку
	virtual int GetCountString() = 0;			//!< количество строк

	virtual void SetTextItem(char* text, int col, int str) = 0;					//!< установить текст в ячейку
	virtual void GetTextItem(char* text, int col, int str, int sizebuff) = 0;	//!< получить текст из ячейки

	virtual void SetDataItem(int str, long data) = 0;
	virtual long GetDataItem(int str) = 0;

	virtual int GetSelString(int start = -1) = 0;	//!< возвращает индекс первого найденного выделения после start (-1 - значит сначала искать)
	virtual void SetSelString(int str) = 0;			//!< выделение строки по индексу
	virtual int GetSelColumn() = 0;					//!< возвращает индекс выделеного столбца
	virtual void SetSelColumn(int col) = 0;			//!< выделение столбца по индексу

	virtual void Clear() = 0;						//!< очистка всей таблицы
	virtual void ClearStrings() = 0;				//!< очистка только строк
};

//! создает экземплляр объекта "istView", и возвращает указатель
SX_LIB_API ISXGUIListView* SXGUICrListViewEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "istView", и возвращает указатель
SX_LIB_API ISXGUIListView* SXGUICrListView(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! обработчик для элемента #ISXGUIGroupBox
SX_LIB_API LRESULT WndProcGroupBoxPaint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! элемент "GroupBox" - контейнер для других элементов, с рамкой и необязательным заголовком, класс переопределен, вся занимаемая область данным элементом принадлжеит ему, и как следствие InvalidateRect приведет к обновлению всей области
struct ISXGUIGroupBox : public virtual ISXGUITextual
{
	virtual ~ISXGUIGroupBox(){}
};

//! создает экземплляр объекта "GroupBox", и возвращает указатель
SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "GroupBox", и возвращает указатель
SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! элемент "ProgressBar" - полоса состояния/прогресса выполнения чего либо
struct ISXGUIProgressBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIProgressBar(){}
	//получить/установить позицию прогресса
	virtual int GetPos() = 0;				//!< возвращает текущую позицию
	virtual int SetPos(int pos) = 0;		//!< возвращает старую позицию

	//получить/установить размер шага прогресса
	virtual int GetStep() = 0;				//!< возвращает текущий размер шага
	virtual int SetStep(int step) = 0;		//!< возвращает старый размер шага

	//возвращают предшествующую позицию
	virtual int SetPosSteps(int steps) = 0;	//!< установить позицию по количеству шагов
	virtual int SetOneStep() = 0;			//!< увеличить позицию на один шаг

	/*! устанавливает позицию минимума и максимума прогресса
	 \return При удачном выполнении возвращает true, иначе false */
	virtual bool SetMinMax(int min, int max) = 0;

	virtual int GetMin() = 0;	//!< возвращают минимальную позицию прогресса
	virtual int GetMax() = 0;	//!< возвращают максимальную позицию прогресса

	
	virtual bool SetBarColor(BYTE r, BYTE g, BYTE b) = 0;	//!< установить цвет полоски? при удачном выполнении возвращает true, иначе false
	virtual DWORD GetBarColor() = 0;						//!< получить цвет полоски

	//установить/получить цвет фона
	virtual bool SetBkColor(BYTE r, BYTE g, BYTE b) = 0;	//установить цвет фона, при удачном выполнении возвращает true, иначе false
	virtual DWORD GetBkColor() = 0;							//!< получить цвет фона
};

//! создает экземплляр объекта "GroupBox", и возвращает указатель
SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBarEx(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

/*! создает экземплляр объекта "GroupBox", и возвращает указатель
 \param vertical true - вертикальное размещение, false - горизонтальное
 \param smooth true - сплошная заливка полосы, false - заливка полосы одинаковыми секциями/прямоугольниками
*/
SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBar(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool vertical, bool smooth);

////////////////////////////////////////////////////////////////////////////

//! элемент "RadioButton" - переключа́тель (с необязательной подписью), позволяет выбрать одну опцию  из предопределенного набора (таких же RadioButton'ов у одного родителя)
struct ISXGUIRadioButton : public virtual ISXGUIButton
{
	virtual ~ISXGUIRadioButton(){}
	virtual void SetCheck(bool check) = 0;	//!< установка отметки
	virtual bool GetCheck() = 0;			//!< отмечен ли?
};

//! создает экземплляр объекта "RadioButton", и возвращает указатель
SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButtonEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "RadioButton", и возвращает указатель
SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

/*! \name Состояния выделения элемента #ISXGUICheckBox
@{*/
#define SXGUI_CHECKBOX_UNCHECKED		BST_UNCHECKED		/*!< не отмечен */
#define SXGUI_CHECKBOX_CHECKED			BST_CHECKED			/*!< отмечен */
#define SXGUI_CHECKBOX_INDETERMINATE	BST_INDETERMINATE	/*!< неопределен */

//!@}

//! элемент "CheckBox" - "галочка" (с необязательной подписью) с возможностью выбора состояния отметки
struct ISXGUICheckBox : public virtual ISXGUIButton
{
	virtual ~ISXGUICheckBox(){}
	virtual void SetCheck(int check) = 0;	//!< установка состояния отметки SXGUI_CHECKBOX_
	virtual int GetCheck() = 0;				//!< возвращает состояние отметки SXGUI_CHECKBOX_
};

//! создает экземплляр объекта "CheckBox", и возвращает указатель
SX_LIB_API ISXGUICheckBox* SXGUICrCheckBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

/*! создает экземплляр объекта "CheckBox", и возвращает указатель
 \param cb_3_state - может ли элемент иметь третье состояние "неопределенный"?
*/
SX_LIB_API ISXGUICheckBox* SXGUICrCheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool cb_3_state);

////////////////////////////////////////////////////////////////////////////

//! элемент "TrackBar" - ползунок
struct ISXGUITrackBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUITrackBar(){}
	virtual void SetPos(int pos) = 0;	//!< устанавливает позицию
	virtual int GetPos() = 0;			//!< возвращает текущую позицию

	virtual void SetMinMax(int min, int max) = 0;	//!< устанавливает минимальное (левое) и максимальное (правое) значения позиции
	virtual int GetMin() = 0;			//!< возвращает текущее минимальное (левое) значение позиции
	virtual int GetMax() = 0;			//!< возвращает текущее максимальное (правое) значение позиции

	/*! установка риски в позиции (между минимумом и максимумом)
	 \note Элемент trackbar создает две крайние риски сам (если пределы от минуса (-100 к примеру) до плюса (100 к примеру) то тогда будет создана третья риска в нуле)
	*/
	virtual bool SetTick(int pos) = 0;	
	virtual int GetTick(int index) = 0;	//!< возвращает позициюю риски (между минимумом и максимумом) по индексу риски

	virtual void SetTickFrequency(int freq) = 0;	//!< установка промежутков между рисками, значения указываются в интервалах приращения (пунктах)
};

//! создает экземплляр объекта "TrackBar", и возвращает указатель
SX_LIB_API ISXGUITrackBar* SXGUICrTrackBarEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "TrackBar", и возвращает указатель
SX_LIB_API ISXGUITrackBar* SXGUICrTrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

/*! \name Выравнивание составных частей внутри #ISXGUIStatusBar при изменении размеров
@{*/

#define SXGUI_STATUSBAR_ALIGNRS_NONE	0	/*!< без изменений */
#define SXGUI_STATUSBAR_ALIGNRS_PERCENT	1	/*!< в процентном соотношениям к текущему состоянию */
#define SXGUI_STATUSBAR_ALIGNRS_EQUALLY	2	/*!< равномерно одинаково */

//!@}

//! элемент "StatusBar" - горизонтальная полоса в нижней части родительского окна, в ней может отображать различные виды информации о состоянии
struct ISXGUIStatusBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIStatusBar(){}

	//! установка количества частей и их размеров
	virtual bool SetCountParts(
		int count,	//!< количество частей, оно же размер arr
		int* arr	//!< массив с размерами в пикселях по ширине каждой части
		) = 0;

	virtual int GetCountParts(int **arr) = 0;	//!< инициализирует arr (если arr != 0) и записывает в него позиции (правая сторона клиентской области) каждой части, возвращает количество частей 
	
	//! устанавливает в часть текст
	virtual bool SetTextParts(
		int pos,			//!< номер части
		const char* text	//!< текст
		) = 0;

	//!< в buf записывает текст части pos, len - размер buf
	virtual bool GetTextParts(int pos, char* buf, int len) = 0;	

	virtual void Update() = 0;		//!< обновление с учетом установленного выравнивания, вызывать при изменении размеров
	virtual void UpdateSize() = 0;	//!< аналогично #Update

	virtual void SetAlignRS(int alignrs) = 0;	//!< устанавливает выравнивание при изменении размеров SXGUI_STATUSBAR_ALIGNRS_
	virtual int GetAlignRS() = 0;				//!< возвращает выравнивание при изменении размеров SXGUI_STATUSBAR_ALIGNRS_
};

//! создает экземплляр объекта "StatusBar", и возвращает указатель
SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBarEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//! создает экземплляр объекта "StatusBar", и возвращает указатель
SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBar(const char* caption, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

//! элемент "ToolBar" - панель инструментов
struct ISXGUIToolBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIToolBar(){}
};

//! создает экземплляр объекта "ToolBar", и возвращает указатель
SX_LIB_API ISXGUIToolBar* SXGUICrToolBar(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

////////////////////////////////////////////////////////////////////////////

/*! \name Состояния элементов меню #ISXGUIMenu и #ISXGUIPopupMenu
@{*/

#define SXGUI_MENU_PART_STATE_ENABLED	0	/*!< действительно */
#define SXGUI_MENU_PART_STATE_DISABLED	1	/*!< недействительно */
#define SXGUI_MENU_PART_STATE_GRAYED	2	/*!< недействительно */

//!@}

//! элемент "Menu" - строка меню окна
struct ISXGUIMenu : public IBaseObject
{
	virtual ~ISXGUIMenu(){}

	//! вставить элемент меню
	virtual bool InsertItem(
		int index,			//!< индекс позиции
		const char* text,	//!< текст элемента
		WORD id,			//!< идентификатор элемента меню
		WORD id_sub_menu	//!< идентификатор подменю
		) = 0;

	//! вставить контекстное меню
	virtual bool InsertPopupItem(
		int index,			//!< индекс позиции
		const char* text,	//!< текст элемента
		WORD id,			//!< идентификатор элемента меню
		WORD id_sub_menu	//!< идентификатор подменю
		) = 0;

	//! вставить разделитель
	virtual bool InsertSeparator(
		int index,			//!< индекс позиции
		WORD id,			//!< идентификатор элемента меню
		WORD id_sub_menu	//!< идентификатор подменю
		) = 0;

	virtual bool DeleteItem(WORD id) = 0;	//!< удалить элемент меню по его id

	virtual HMENU GetSubMenu(WORD id) = 0;	//!< возвращает дескриптор подменю по его id

	virtual bool CheckItem(WORD id, bool check) = 0;	//!< выделить элемент меню
	virtual bool EnableItem(WORD id, int enable) = 0;	//!< установить действительность элементу меню

	virtual bool GetCheckedItem(WORD id) = 0;	//!< выделен ли элемент меню id
	virtual int GetEnabledItem(WORD id) = 0;	//!< действителен ли элемент меню id

	//! возвращает дескриптор меню
	virtual HMENU GetHMENU() = 0;

	//! установить меню окну hwnd
	virtual bool SetToWindow(HWND hwnd) = 0;
};

//! создает экземплляр объекта "Menu", и возвращает указатель
SX_LIB_API ISXGUIMenu* SXGUICrMenu();

//! создает экземплляр объекта "Menu" из ресурсов по идентификатору menu, и возвращает указатель
SX_LIB_API ISXGUIMenu* SXGUICrMenuEx(WORD menu);

////////////////////////////////////////////////////////////////////////////

//! элемент "PopupMenu" - контекстное меню
struct ISXGUIPopupMenu : public IBaseObject
{
	virtual ~ISXGUIPopupMenu(){}
	
	//! вставить элемент меню
	virtual bool InsertItem(
		int index,			//!< индекс позиции
		const char* text,	//!< текст элемента
		WORD id,			//!< идентификатор элемента меню
		WORD id_sub_menu	//!< идентификатор подменю
		) = 0;

	//! вставить контекстное меню
	virtual bool InsertPopupItem(
		int index,			//!< индекс позиции
		const char* text,	//!< текст элемента
		WORD id,			//!< идентификатор элемента меню
		WORD id_sub_menu	//!< идентификатор подменю
		) = 0;
	
	//! вставить разделитель
	virtual bool InsertSeparator(
		int index,			//!< индекс позиции
		WORD id,			//!< идентификатор элемента меню
		WORD id_sub_menu	//!< идентификатор подменю
		) = 0;

	virtual bool DeleteItem(WORD id) = 0;	//!< удалить элемент меню по его id

	virtual HMENU GetSubMenu(WORD id) = 0;	//!< возвращает дескриптор подменю по его id

	virtual bool CheckItem(WORD id, bool check) = 0;	//!< выделить элемент меню
	virtual bool EnableItem(WORD id, int enable) = 0;	//!< установить действительность элементу меню

	virtual bool GetCheckedItem(WORD id) = 0;	//!< выделен ли элемент меню id
	virtual int GetEnabledItem(WORD id) = 0;	//!< действителен ли элемент меню id

	//! возвращает дескриптор меню
	virtual HMENU GetHMENU() = 0;

	//! показать меню
	virtual bool Track(
		HWND hwnd,	//!< дескриптор элемента для которого вызывается меню
		int x,		//!< позиция по оси x (screen space)
		int y		//!< позиция по оси y (screen space)
		) = 0;
};

//! создает экземплляр объекта "PopupMenu", и возвращает указатель
SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenu();

//! создает экземплляр объекта "PopupMenu" из ресурсов по идентификатору menu, и возвращает указатель
SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenuEx(WORD menu);

//!@} sxguiwinapi_elements

//#############################################################################

//! Стандартный обработчик событий для всех элементов
SX_LIB_API LRESULT CALLBACK WndProcAllDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//#############################################################################

//! функции управения элементами
namespace SXGUIFuctinon
{
	/*! \name Операции с полосами прокрутки (Scroll bar) элемента
	@{*/

	//! установка прокрутки
	SX_LIB_API bool	ScroolBars(
		ISXGUIControl *Control, //!< элемент которому устанавливается
		bool h,					//!< устанавливать ли горизонтальную полосу прокрутки?
		bool v					//!< устанавливать ли вертикальную полосу прокрутки?
		);

	SX_LIB_API bool	ScrollBarV(ISXGUIControl *Control);	//!< устанвлена ли вертикальная полоса прокрутки?
	SX_LIB_API bool	ScrollBarH(ISXGUIControl *Control);	//!< устанвлена ли горизонтальная полоса прокрутки?

	SX_LIB_API bool	ScrollLine(
		ISXGUIControl *Control, //!< элемент
		int scroll,	//!< какую полосу прокручивать? SXGUI_SCROLL_TYPE_
		int dir,	//!< в какую сторону прокручивать, SXGUI_SCROLL_DIR_
		int count	//!< на сколько строк прокрутить
		);

	//!@}

	/*! \name Операции с текстом (caption) элемента
	@{*/

	SX_LIB_API bool	SetText(ISXGUIControl*const Control, const char* text);	//!< установка текста контролу
	SX_LIB_API void	GetText(ISXGUIControl *Control, char* buf, int count);	//!< в buf записывает текст контрола, count - длина buf
	SX_LIB_API int	GetTextLen(ISXGUIControl *Control);						//!< возвращает длину текста контрола

	//!@}
};

//#############################################################################

//! функции обработки определенного события посылаемого родительскому элементу у всех его дочерних элементов
namespace SXGUIEnumChildWindow
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
namespace SXGUIBaseHandlers
{
	/*! установка основных обработчиков, без которых SXGUIWinApi будет работать не стабильно, а возможно и не будет работать, 
	если возвращает 0 значит все в норме, иначе возвращает номер того обработчика который не удалось установить, 
	данная функция вызывается только если не требуется каких либо дополнительных действий при обработке зарезервированных сообщений
	*/
	SX_LIB_API int InitHandlerMsg(ISXGUIComponent* Component);

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

//#############################################################################

//! пространство имен с функциями регистрации классов созданные вручную
namespace SXGUIRegClass
{
	//! регистрация класса "ButtonImg" #ISXGUIButtonImg
	SX_LIB_API bool RegButtonImg();
	//! регистрация класса "GroupBox" #ISXGUIGroupBox
	SX_LIB_API bool RegGroupBox();
};

//#############################################################################

/*! \name Типы диалогов выбора файлов
@{*/

#define SXGUI_DIALOG_FILE_OPEN 0	/*!< открыватие файла */
#define SXGUI_DIALOG_FILE_SAVE 1	/*!< сохранение файла */

//!@}

//! функции диалогов
namespace SXGUIDialogs
{
	/*! диалог выбора файла
	 \note path или name должен быть не 0, иначе результат вызова диалога некуда будет записать и как следствие диалог не будет вызван
	*/
	SX_LIB_API void SelectFile(
		int type,				//!< тип диалога SXGUI_DIALOG_FILE_
		char* path,				//!< если не 0 то запишется путь до файла
		char* name,				//!< если не 0 то запишется имя файла
		const char* stdpath,	//!< путь относительно которого открывать диалог
		const char* filter		//!< фильтр расширений
		);
};

#endif

//!@} sxguiwinapi
