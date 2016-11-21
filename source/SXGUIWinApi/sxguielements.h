
#include <gdefines.h>

#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <stdio.h>

//для начала нужно инициализировать как внутренние библиотеки так и новые компоненты зарегистрировать 
/*
//регистрируем новые элементы
SXGUIRegClass::RegButtonImg();
SXGUIRegClass::RegToolBar();

INITCOMMONCONTROLSEX icex;

icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
icex.dwICC  = ICC_BAR_CLASSES;
InitCommonControlsEx(&icex);
*/

//инициализировав элемент можно установить на него стандартные обработчики: \
	SXGUIBaseHandlers::InitHandlerMsg(HWND);
//а затем добавить необходимые:\
	Wnd->AddHandler(MouseWheel,WM_MOUSEWHEEL,0,0,0,0,true);

//!!! main функция-обработчик может быть только одна и должна быть вообще!
//!!! но если ее нет то для окон будет вызываться ::DefWindowProc(hwnd, msg, wParam, lParam)
//!!! а для остальных компонентов CallWindowProc(Component->OldProc,hwnd, msg, wParam, lParam)

#define SXGUI_HORZ_SCROLL	0
#define SXGUI_VERT_SCROLL	1

#define SXGUI_UP_SCROLL		0
#define SXGUI_DOWN_SCROLL	1

#define SXGUI_LEFT_SCROLL	0
#define SXGUI_RIGTH_SCROLL	1

//максимальнео количество обработчиков
//нужно подобрать оптимальное значение
#define SXGUI_COUNT_HANDLERS_MSG_IN_ARR 256

//тип обработчика
typedef LRESULT(*HandlerMsg) (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//структура данных для обработчика
struct SXHandlerMsgStruct
{
	HandlerMsg Handler;		//сама функция, которая будет вызываться
	UINT Msg;
	WPARAM WParam;
	bool BFCondiderWParam;	//проверять WParam?
	LPARAM LParam;
	bool BFCondiderLParam;	//проверять LParam
	bool IsMainFunction;	//главный ли это обработчик? true - значение возвращенное фунцией - возвращается WndProc, иначе нет 
};

//структура, логическая, для определения тянуться ли стороны элемента за сторонами родителя
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

//первый базовый класс, единственным родителем не используется
//определяет объект как контрол, то есть содержит в себе минимум данных для 
//управления объектом
struct ISXGUIControl : public IBaseObject
{
	virtual ~ISXGUIControl(){};
	virtual void Init(HWND window_handle, HWND parent_handle, WNDPROC handler) = 0;	//запоминаем дескрипторы, и устанавливаем новый обработчик

	virtual HWND Parent() = 0;						//возвращает HWND родителя
	virtual bool Parent(HWND parent_handle) = 0;	//устанавливает родителя

	virtual HWND GetHWND() = 0;

	virtual void Visible(bool bf) = 0;				//установка видимости контрола
	virtual bool Visible() = 0;						//видим ли контрол?

	virtual void SetFocus() = 0;					//установить фокус на контрол
	virtual bool Focus() = 0;						//установлен ли фокус на контроле?
	//virtual WNDPROC GetOldProc() = 0;

	WNDPROC OldProc;

protected:
	HWND ParentHandle;
	HWND WindowHandle;
};


//////
//размер текста подсказки
#define SX_HINT_COUNT_BUF_SIZE 1024

struct ISXGUIHint : public IBaseObject
{
	virtual ~ISXGUIHint(){}
	//autopop - время показа
	//init - сколько ждем появления подсказки после наведения курсора
	virtual void SetDelayTime(DWORD init, DWORD autopop) = 0;

	virtual bool Parent(HWND parent) = 0;
	virtual HWND Parent() = 0;

	virtual HWND GetHWND() = 0;

	virtual void Visible(bool bf) = 0;
	virtual bool Visible() = 0;

	virtual void SetText(const char* text) = 0;	//установка текста подсказки
	virtual char* GetText() = 0;				//возвращает указатель на текст подсказки, очищается при вызове деструктора
	virtual void GetText(char* buf) = 0;
};

SX_LIB_API ISXGUIHint* SXGUICrHint(HWND parent);

////

//второй в очереди родитель, часто наследование идет именно от него
//более расширенное управление большинством элементов
struct ISXGUIComponent : public virtual ISXGUIControl
{
	virtual ~ISXGUIComponent(){};
	//устанавливает необходимые данные для управления элементом
	virtual void InitComponent() = 0; //необходимо вызывать после инициализации

	//действителен ли элемент
	virtual bool Enable()=0;
	virtual void Enable(bool bf) = 0;

	//работа со шрифтом
	//если name == 0 то не изменияем название шрифта,
	//если name[0] == 0 то дефолтный шрифт gui
	//если остальные значения в -1 то не учитываются
	virtual void SetFont(const char* name, int height, int width, int weight, int italic, int underline, int strike_out) = 0;
	virtual void SetFont(HFONT* hfont) = 0;
	virtual HFONT GetFont() = 0;

	virtual bool IsParentFont() = 0;	//наследуется ли шрифт родителя
	virtual HFONT ParentFont() = 0;		//получает родительский шрифт

	//работа со стилями (в т.ч. специальным)
	//style_add - добавляемые стили
	//style_del - удаляемые
	virtual bool ModifyStyle(long style_add, long style_del) = 0;
	virtual bool ModifyExStyle(long style_add, long style_del) = 0;

	//устанавливает/возвращает область окна RECT элемента в глобальных координатах
	virtual bool	SetWinRect(RECT* rect, bool alignment_screen_space) = 0;	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	virtual RECT*	GetWinRect() = 0;
	virtual void	GetWinRect(RECT* rect) = 0;

	//устанавливает/возвращает клиентскую область RECT элемента в глобальных координатах
	virtual bool	SetClientRect(RECT* rect, bool alignment_screen_space) = 0;	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	virtual RECT*	GetClientRect() = 0;
	virtual void	GetClientRect(RECT* rect) = 0;

	//!!!первоначально нужно единожды включить видимость подсказки, ибо в этот момент она инициализируется
	//видимость подсказки
	virtual void ShowHint(bool bf) = 0;
	virtual bool ShowHint() = 0;

	//работа с текстом подсказки для элемента
	virtual void SetHintText(const char* text) = 0;
	virtual char* GetHintText() = 0;
	virtual void GetHintText(char* buf) = 0;

	//функции для цветов
	//set/get цвет текста
	virtual void SetColorText(BYTE r, BYTE g, BYTE b) = 0;
	virtual void SetColorText(DWORD color) = 0;
	virtual DWORD GetColorText() = 0;

	//set/get прозрачность заднего фона текста
	virtual void SetTransparentTextBk(bool bf) = 0;
	virtual bool GetTransparentTextBk() = 0;

	//set/get цвет заднего фона
	virtual void SetColorTextBk(BYTE r, BYTE g, BYTE b) = 0;
	virtual void SetColorTextBk(DWORD color) = 0;
	virtual DWORD GetColorTextBk() = 0;

	//set/get цвет заднего фона элемента
	virtual bool SetColorBrush(BYTE r, BYTE g, BYTE b) = 0;
	virtual bool SetColorBrush(DWORD color) = 0;
	virtual DWORD GetColorBrush() = 0;
	virtual HBRUSH GetBrush() = 0;

	//все что касается обработчиков функций
	///////////////////////////////////////

	//добавление обработчика
	virtual bool AddHandler(HandlerMsg Handler, UINT Msg, WPARAM wParam, bool considerWparam, LPARAM lParam, bool considerLparam, bool isMain) = 0;
	virtual bool AddHandler(HandlerMsg Handler, UINT Msg) = 0;	//все что необъявлено из функции выше не учитывается

	//key - ключ в массиве обработчиков
	virtual HandlerMsg GetHandlerFunction(int key) = 0;	//возвращает функцию обработчик 
	virtual WORD GetCountKeyArrHandler() = 0;			//количество ключей в массиве обработчиков

	//все что касается данных сообщения
	virtual UINT GetMsgHandler(int key) = 0;
	virtual WPARAM GetWParamHandler(int key) = 0;
	virtual bool GetConsiderWParamHandler(int key) = 0;	//учитывать WParam?
	virtual LPARAM GetLParamHandler(int key) = 0;
	virtual bool GetConsiderLParamHandler(int key) = 0;	//учитывать LParam?

	virtual bool IsMainFunction(int key) = 0;	//это главная функция?
	virtual LRESULT ExecuteHandler(int key, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;	//выполняет функцию обработчик, в аргументы отправляет свои аргументы
	///////////////////////////////////////

	virtual void UpdateSize() = 0;
	virtual void UpdateRect() = 0;


	SXRectBool GAlign;	//данные регулирующие размеры элемента при изменении размеров его родителя
	//данные необходимые для UpdateSize
	RECT ParentRect;		//параметры родителя
	RECT OffsetParentRect;	//на сколько был изменен родитель
	RECT WinScreenRect;		//параметры текущего окна относительно рабочего стола

	//данные регулирующие будет ли передвигаться определнная сторона
	bool BFSizingChangeTop;		//за верх окно расширять нельзя ... типа того
	bool BFSizingChangeBottom;
	bool BFSizingChangeRight;
	bool BFSizingChangeLeft;

	bool BFMinSize;	//установлен ли минимальный размер? если да то меньше чем MinSizeX и MinSizeY не будет уменьшаться
	WORD MinSizeX;
	WORD MinSizeY;

protected:
	ISXGUIHint *Hint;
	SXHandlerMsgStruct ArrHandler[SXGUI_COUNT_HANDLERS_MSG_IN_ARR];
	int CountKeyArrHandler;

	DWORD ColorBrush;	//цвет заливки формы
	HBRUSH HBrush;

	DWORD ColorText;	//цвет текста
	bool TransparenTextBf;	//используется прозрачность? если true то заднего фона у текста не будет
	DWORD ColorTextBk;	//цвет заднего фона у текста
};

//третий (если нужен текст) родитель, непосредтсвенно не используется
//характеризует элемент как содержащий единый текст (аля кэпшен) текстовый компонент
struct ISXGUITextual : public virtual ISXGUIComponent
{
	virtual ~ISXGUITextual(){};
	virtual bool SetText(const char* text)=0;
	//virtual const char*	GetText() = 0;
	virtual void GetText(char* buf, int size) = 0;
	virtual int GetTextLen() = 0;
};

///////

struct ISXGUIBaseWnd : public virtual ISXGUITextual
{
	virtual ~ISXGUIBaseWnd(){};
	virtual bool SetColorBrush(BYTE r, BYTE g, BYTE b) = 0;
	virtual bool SetColorBrush(DWORD color) = 0;
	virtual DWORD GetColorBrush() = 0;
	virtual HBRUSH GetBrush() = 0;

	virtual bool Alpha(WORD alpha) = 0;
	virtual WORD Alpha() = 0;
	virtual bool AlphaBlend(bool bf) = 0;
	virtual bool AlphaBlend() = 0;
};

SX_LIB_API ISXGUIBaseWnd* SXGUICrBaseWnd(
	const char* class_name, const char* caption, const char* menu,
	WORD id, WORD x, WORD y, WORD width, WORD heigth,
	HICON icon, HCURSOR cursor, HBRUSH brush,
	DWORD exstyle, DWORD wndstyle, DWORD style,
	HWND parent, WNDPROC handler=0
	);

/////

#define SXGUI_STATIC_ALIGN_LEFT		0
#define SXGUI_STATIC_ALIGN_RIGHT	1
#define SXGUI_STATIC_ALIGN_CENTER	2

struct ISXGUIStatic : public virtual ISXGUITextual
{
	virtual ~ISXGUIStatic(){}
	virtual WORD Align() = 0;			//возвращает флаг выравнивания текста
	virtual bool Align(WORD align) = 0;	//устанавливает выравнивание текста

	virtual bool WordWrap() = 0;		//установлен ли автоперенос строк
	virtual bool WordWrap(bool bf) = 0; //манипуляция со свойством автоперенос строк
};

SX_LIB_API ISXGUIStatic* SXGUICrStaticEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIStatic* SXGUICrStatic(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);
//создает линию
SX_LIB_API ISXGUIStatic* SXGUICrStaticLine(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool vertical);

//////

#define SXGUI_BUTTON_IMAGE_NONE		0
#define SXGUI_BUTTON_IMAGE_ICON		1
#define SXGUI_BUTTON_IMAGE_BITMAT	2


struct ISXGUIButton : public virtual ISXGUITextual
{
	virtual ~ISXGUIButton(){}
	virtual bool SetIconInFile(const char* path) = 0;
	virtual bool SetBmpInFile(const char* path) = 0;

	virtual bool SetIconInResourse(WORD id) = 0;
	virtual bool SetBmpInResourse(WORD id) = 0;
};

SX_LIB_API ISXGUIButton* SXGUICrButtonEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIButton* SXGUICrButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, WORD image, HWND parent, WNDPROC handler, DWORD id);

/////

SX_LIB_API LRESULT WndProcButtonImgDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define SXGUI_BI_POS_FREE			0
#define SXGUI_BI_POS_MOUSE_MOVE		1
#define SXGUI_BI_POS_MOUSE_CLICK	2

#define SXGUI_BI_SIZE_LINE			1


struct ISXGUIButtonImg : public virtual ISXGUIComponent
{
	virtual ~ISXGUIButtonImg(){}
	virtual void ReInitImage(const char* path) = 0;

	virtual void InitCallBack() = 0;

	DWORD AlphaColor;	//цвет который будет исключен
	DWORD BkColor;	//цвет фона кнопки
	DWORD FrameColor;	//цвет фона кнопки

	WORD Pos;			//позиция в которой состоит кнопка
	bool PosBf[3];		//нужна ли перерисовка состоянию

	bool EnableState;	//состояние кнопки (активна/пассивна)

	bool EnableBf;		//включено ли изменение активности кнопки
	bool EnableGroup;	//включена ли кнопка в группу всех дочерних элементов родителя, это необходимо чтобы деактивировать кнопку при нажатиии другой кнопки


	BITMAP Bitmap;
	HANDLE HBitMap;

	bool IsImages;

	virtual void GetPathForImg(char* buf) = 0;
};

SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgEx(const char* path, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImg(WORD button, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, DWORD id);

///////

#define SXGUI_EDIT_ALIGN_LEFT	0
#define SXGUI_EDIT_ALIGN_RIGHT	1
#define SXGUI_EDIT_ALIGN_CENTER	2

struct ISXGUIEdit : public virtual ISXGUITextual
{
	virtual ~ISXGUIEdit(){}
	virtual bool ReadOnly() = 0;		//установлено ли свойство "только для чтения"
	virtual bool ReadOnly(bool bf) = 0;	//манипулирует свойством "только для чтения", возвращает результат действия

	//дефолтово скрытый текст заменяется на *
	virtual bool Password() = 0;		//установлено ли свойств скрытия текста
	virtual bool Password(bool bf) = 0;	//манипулирует свойством скрытия текста

	virtual WORD Align() = 0;			//возвращает флаг выравнивания текста
	virtual bool Align(WORD align) = 0;	//устанавливает выравнивание
};

SX_LIB_API ISXGUIEdit* SXGUICrEditEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIEdit* SXGUICrEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

///////

struct ISXGUIUpDown : public virtual ISXGUIComponent
{
	virtual ~ISXGUIUpDown(){}
	virtual void SetMinMax(long min, long max) = 0;
	virtual long SetPos(long pos) = 0;

	virtual long GetMin() = 0;
	virtual long GetMax() = 0;
	virtual long GetPos() = 0;

	virtual HWND SetBuddy(HWND buddy) = 0;
	virtual HWND GetBuddy() = 0;
};

SX_LIB_API ISXGUIUpDown* SXGUICrUpDownEx(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id, HWND buddy);
SX_LIB_API ISXGUIUpDown* SXGUICrUpDown(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, HWND buddy, WORD left_right);

/////

struct ISXGUIMemo : public virtual ISXGUIEdit
{
	virtual ~ISXGUIMemo(){}
	virtual bool WordWrap() = 0;		//установлен ли автоперенос слов
	virtual bool WordWrap(bool bf) = 0;	//манипулирует свойством автоперенос слов

	//SXGUIFuctinon
	virtual bool ScrollBars(bool h, bool v) = 0;
	virtual bool ScrollBarV() = 0;
	virtual bool ScrollBarH() = 0;
	virtual bool ScrollLine(WORD scroll, WORD dir, int count) = 0;
};

SX_LIB_API ISXGUIMemo* SXGUICrMemoEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIMemo* SXGUICrMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

///////

struct ISXGUIComboBox : public virtual ISXGUIComponent
{
	virtual ~ISXGUIComboBox(){}
	virtual bool	InsertItem(int index, const char* text) = 0;		//вставка текста в строку
	virtual bool	DeleteItem(int index) = 0;						//удаление строки

	virtual int		GetCount() = 0;									//количество строк

	virtual bool	SetSel(int index) = 0;							//выделяет определенную строку
	virtual int		GetSel() = 0;									//номер выделенной строки

	virtual bool	SetItemData(int index, LPARAM data) = 0;			//установка для строки userdata (32 бита)
	virtual LPARAM	GetItemData(int index) = 0;						//получение userdata

	virtual bool	Clear() = 0;									//удаляет все элементы

	virtual bool	SetItemText(int index, const char* text) = 0;	//вставка текста в строку, подобие замены
	virtual char*	GetItemText(int index) = 0;						//получить текст из строки (происходит выделение памяти в куче, юзер сам должен ее освободить)
	virtual void	GetItemText(int index, char* buf) = 0;			//получить текст из строки 
	virtual int		GetItemTextLength(int index) = 0;				//получить длину текста из строки
	virtual bool	AddItem(const char* text) = 0;					//добавляет в конец новую строку
};

SX_LIB_API ISXGUIComboBox* SXGUICrComboBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);

//флаги: (parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS
SX_LIB_API ISXGUIComboBox* SXGUICrComboBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

/////

struct ISXGUIListBox : public virtual ISXGUIComponent
{
	virtual ~ISXGUIListBox(){}
	virtual bool	MultipleSel() = 0;	//установленно ли свойство множественного выделения строк

	virtual bool	InsertItem(int index, const char* text) = 0;	//вставить текст в строку с номером index
	virtual bool	AddItem(const char* text) = 0;				//добавить строку в конец списка
	virtual int		GetCountItem() = 0;							//возвращает количество строк

	virtual bool	DeleteItem(int index) = 0;					//удалить строку с номером index

	virtual bool	SetSel(int index) = 0;						//выделить строку с номером index
	virtual int		GetSel() = 0;

	virtual bool	SetItemData(int index, LPARAM data) = 0;		//установить userdata для строки с номером index
	virtual LPARAM	GetItemData(int index) = 0;					//возвращает userdata

	virtual bool	Clear() = 0;								//очистить

	virtual bool	SetTextItem(int index, const char* text) = 0;//установить текст в строке с номером index
	virtual char*	GetItemText(int index) = 0;					//возвращает текст из строки !!!память выделяется в куче
	virtual void	GetItemText(int index, char* buf) = 0;		//возвращает текст из строки

	virtual int		GetItemTextLength(int index) = 0;			//возвращает длину текста в строке

	//мультивыделение строк, если this->MultipleSel() == true
	virtual int		GetMultipleSelCount() = 0;					//возвращает количество выделенных строк

	virtual bool	SetMultipleSel(int index, bool sel) = 0;		//устанавливает выделена/не выделена строка
	virtual bool	GetMultipleSel(int index) = 0;				//возвращает выделена ли строка

	virtual int*	GetMultipleSelArr() = 0;					//возвращает массив с номерами выделенных строк

	//SXGUIFuctinon
	virtual bool ScrollBars(bool h, bool v) = 0;
	virtual bool ScrollBarV() = 0;
	virtual bool ScrollBarH() = 0;
	virtual bool ScrollLine(WORD scroll, WORD dir, int count) = 0;
};

SX_LIB_API ISXGUIListBox* SXGUICrListBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIListBox* SXGUICrListBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool miltiple_sel);

//////

struct ISXGUIGroupBox : public virtual ISXGUITextual
{
	virtual ~ISXGUIGroupBox(){}
};

SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

//////

struct ISXGUIProgressBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIProgressBar(){}
	//получить/установить позицию прогресса
	virtual int GetPos() = 0;				//возвращает текущую позицию
	virtual int SetPos(int pos) = 0;		//возвращает старую позицию

	//получить/установить размер шага прогресса
	/*virtual int GetStep() = 0;				//возвращает текущий размер шага
	virtual int SetStep(int step) = 0;		//возвращает старый размер шага

	//возвращают предшествующую позицию
	virtual int SetPosSteps(int steps) = 0;	//установить позицию по количеству шагов
	virtual int SetOneStep() = 0;			//увеличить позицию на один шаг

	//устанавливает позицию минимума и максимума прогресса
	//при удачном выполнении возвращает true, иначе false
	virtual bool SetMinMax(int min, int max) = 0;

	//возвращают минимальную/максимальную позицию прогресса
	virtual int GetMin() = 0;
	virtual int GetMax() = 0;

	//установить/получить цвет полоски (дефолтовый цвет определен вручную RGB(51,153,255))
	virtual bool SetBarColor(WORD r, WORD g, WORD b) = 0;	//при удачном выполнении возвращает true, иначе false
	virtual DWORD GetBarColor() = 0;

	//установить/получить цвет фона (дефолтовый цвет равен цвету фона родителя)
	virtual bool SetBkColor(WORD r, WORD g, WORD b) = 0; //при удачном выполнении возвращает true, иначе false
	virtual DWORD GetBkColor() = 0;*/
};

SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBarEx(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBar(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool vertical, bool smooth);

//////

#define SXGUI_RB_UNCHECKED		BST_UNCHECKED
#define SXGUI_RB_CHECKED		BST_CHECKED

struct ISXGUIRadioButton : public virtual ISXGUITextual
{
	virtual ~ISXGUIRadioButton(){}
	virtual void SetCheck(bool check) = 0;
	virtual bool GetCheck() = 0;
};

SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButtonEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

/////

#define SXGUI_CB_UNCHECKED		BST_UNCHECKED
#define SXGUI_CB_CHECKED		BST_CHECKED
#define SXGUI_CB_INDETERMINATE	BST_INDETERMINATE

struct ISXGUICheckBox : public virtual ISXGUITextual
{
	virtual ~ISXGUICheckBox(){}
	virtual void SetCheck(int check) = 0;
	virtual int GetCheck() = 0;
};

SX_LIB_API ISXGUICheckBox* SXGUICrCheckBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUICheckBox* SXGUICrCheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool cb_3_state);

///////

struct ISXGUITrackBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUITrackBar(){}
	virtual void SetPos(int pos) = 0;
	virtual int GetPos() = 0;

	virtual void SetMinMax(int min, int max) = 0;
	virtual int GetMin() = 0;
	virtual int GetMax() = 0;

	virtual bool SetTick(int pos) = 0;
	virtual int GetTick(int index) = 0;

	virtual void SetTickFrequency(int freq) = 0;

	virtual bool SetBuddies(HWND hwnd_left, HWND hwnd_right) = 0;
	virtual HWND GetLeftBuddy() = 0;
	virtual HWND GetRightBuddy() = 0;
};

SX_LIB_API ISXGUITrackBar* SXGUICrTrackBarEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUITrackBar* SXGUICrTrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id);

///////

#define SXGUI_SB_ALIGN_RS_NONE		0
#define SXGUI_SB_ALIGN_RS_PERCENT	1
#define SXGUI_SB_ALIGN_RS_PROP		2
//!!! проверить код, там чето не лады с выделением памяти
struct ISXGUIStatusBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIStatusBar(){}
	virtual bool SetCountParts(WORD count, int *arr) = 0;
	virtual WORD GetCountParts(int **arr) = 0;
	virtual bool SetTextParts(WORD pos, const char* text) = 0;
	virtual const char* GetTextParts(WORD pos) = 0;

	virtual void Update() = 0;
	virtual void UpdateSize() = 0;

	WORD AlignReSizing;
};

SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBarEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id);
SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBar(const char* caption, HWND parent, WNDPROC handler, DWORD id);

///////

#define SXGUI_MENU_ENABLED	0
#define SXGUI_MENU_DISABLED	1
#define SXGUI_MENU_GRAYED	2


struct ISXGUIMenu
{
	virtual ~ISXGUIMenu(){}
	virtual bool InsertItem(int index, const char* text, WORD id, WORD id_sub_menu) = 0;
	virtual bool InsertPopupItem(int index, const char* text, WORD id, WORD id_sub_menu) = 0;
	virtual bool InsertSeparator(int index, WORD id, WORD id_sub_menu) = 0;

	virtual bool DeleteItem(WORD id) = 0;

	virtual HMENU GetSubMenu(WORD id) = 0;

	virtual bool CheckItem(WORD id, bool check) = 0;
	virtual bool EnableItem(WORD id, WORD enable) = 0;

	virtual bool GetCheckedItem(WORD id) = 0;
	virtual WORD GetEnabledItem(WORD id) = 0;

	virtual HMENU GetHMENU() = 0;
	virtual bool SetToWindow(HWND hwnd) = 0;
};

SX_LIB_API ISXGUIMenu* SXGUICrMenu();
SX_LIB_API ISXGUIMenu* SXGUICrMenuEx(WORD menu);

/////////////////////////

struct ISXGUIPopupMenu
{
	virtual ~ISXGUIPopupMenu(){}
	virtual bool InsertItem(int index, const char* text, WORD id, WORD id_sub_menu) = 0;
	virtual bool InsertPopupItem(int index, const char* text, WORD id, WORD id_sub_menu) = 0;
	virtual bool InsertSeparator(int index, WORD id, WORD id_sub_menu) = 0;

	virtual bool DeleteItem(WORD id) = 0;

	virtual HMENU GetSubMenu(WORD id) = 0;

	virtual bool CheckItem(WORD id, bool check) = 0;
	virtual bool EnableItem(WORD id, WORD enable) = 0;

	virtual bool GetCheckedItem(WORD id) = 0;
	virtual WORD GetEnabledItem(WORD id) = 0;

	virtual bool Track(HWND hwnd, int x, int y) = 0;

	virtual HMENU GetHMENU() = 0;
};

SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenuEx(WORD menu);
SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenu();

//////

struct ISXGUIToolBar : public virtual ISXGUIComponent
{
	virtual ~ISXGUIToolBar(){}
	virtual void AddButton(WORD num, WORD id, const char* hint, WORD id_resource, DWORD mask) = 0;
	virtual void AddSeparator(WORD num) = 0;
};

SX_LIB_API ISXGUIToolBar* SXGUICrToolBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, WORD width_element, WORD heigth_element, HWND parent, WNDPROC handler, DWORD id);

//////

SX_LIB_API LRESULT CALLBACK WndProcToolBarDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct ISXGUIToolBarSX : public virtual ISXGUIComponent
{
	virtual ~ISXGUIToolBarSX(){}
	ISXGUIButtonImg *ImgButton;

	RECT MaxRect;
	RECT MinRect;

	const char* Caption;

	DWORD ColorTop;
	DWORD ColorClient;

	//SXGUIToolBarSX* ArrChildToolBar[1024];
	WORD CountChildToolBar;
};

SX_LIB_API ISXGUIToolBarSX* SXGUICrToolBarSXEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, const char* path_button_ctrl, DWORD id_ctrl_button);
SX_LIB_API ISXGUIToolBarSX* SXGUICrToolBarSX(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, WORD button_res, DWORD id_ctrl_button);

//////

//!!!
//на одно сообщение может быть назначен только один обработчик который будет возвращать значение
//седьмой аргумент AddHandler (isMain) должен быть true
//остальные не должны сего делать (седьмой аргумент AddHandler false)
//!!!

//стандартный обработчик событий для всех элементов
SX_LIB_API LRESULT CALLBACK WndProcAllDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

////////////////////////

namespace SXGUIFuctinon
{
	//!!!тут разобраться
	//!!!автоматизировать сроллбары
	SX_LIB_API bool	ScroolBars(ISXGUIControl *Control, bool h, bool v);
	SX_LIB_API bool	ScrollBarV(ISXGUIControl *Control);
	SX_LIB_API bool	ScrollBarH(ISXGUIControl *Control);

	SX_LIB_API bool	ScrollLine(ISXGUIControl *Control, WORD scroll, WORD dir, int count);

	//операции с текстом элемента
	SX_LIB_API bool	SetText(ISXGUIControl*const Control, const char* text);
	//!!!выделение памяти
	//SX_LIB_API char*GetTextOut(ISXGUIControl *Control);
	SX_LIB_API void	GetText(ISXGUIControl *Control, char* buf, int count);
	SX_LIB_API int	GetTextLen(ISXGUIControl *Control);
};

//функции обработки определенного события посылаемого родительскому элементу у всех его дочерних элементов
namespace SXGUIEnumChildWindow
{
	//для класса SXGUIButtonImg
	//обработка события активации кнопки и в последствии дезактивации остальных кнопкок
	SX_LIB_API BOOL CALLBACK EnumChildProcUpdateImgButton(HWND hwnd, LPARAM lParam);

	//событие WM_SIZE
	SX_LIB_API BOOL CALLBACK EnumChildProcUpdateSize(HWND hwnd, LPARAM lParam);

	//событие WM_MOVE
	SX_LIB_API BOOL CALLBACK EnumChildProcUpdateRect(HWND hwnd, LPARAM lParam);

	//событие WM_MOUSEMOVE
	SX_LIB_API BOOL CALLBACK EnumChildProcMouseMove(HWND hwnd, LPARAM lParam);
};

namespace SXGUIBaseHandlers
{
	//установка основных обработчиков, без которых SXGUIWinApi будет работать не стабильно, а возможно и не будет работать
	//если возвращает 0 значит все в норме, иначе возвращает номер того обработчика который не удалось устанвоить
	//данная функция вызывается только если не требуется каких либо дополнительных действий при обработке зарезервированных сообщений
	SX_LIB_API int InitHandlerMsg(ISXGUIComponent* Component);

	//обработчик события WM_SIZE
	SX_LIB_API LRESULT SizeChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик событий WM_CTLCOLORSTATIC WM_CTLCOLOREDIT WM_CTLCOLORBTN WM_CTLCOLORLISTBOX
	//то есть обработка цветов дочерних элементов окна (static, edit,button, listbox)
	SX_LIB_API LRESULT CtlColorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик события WM_SIZING
	SX_LIB_API LRESULT SizingChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик события WM_MOVE
	SX_LIB_API LRESULT MoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик события WM_MOUSEMOVE
	SX_LIB_API LRESULT MouseMoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик события WM_SETCURSOR
	SX_LIB_API LRESULT SetCursorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик события WM_COMMAND SC_MAXIMIZE
	SX_LIB_API LRESULT MaximuzeWinChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//обработчик события WM_DESTROY
	SX_LIB_API LRESULT WinDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

//классы созданные вручную и требующие регистрации
namespace SXGUIRegClass
{
	//класс кнопки
	SX_LIB_API bool RegButtonImg();
	//класс тулбара
	SX_LIB_API bool RegToolBar();
};