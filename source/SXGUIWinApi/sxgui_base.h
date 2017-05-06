
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#ifndef SXGUI_BASE_H
#define SXGUI_BASE_H


extern class SXGUIButtonImg;

#include <SXGUIWinApi\SXGUI_hint.h>


//первый базовый класс, единственным родителем не используется
//определяет объект как контрол, то есть содержит в себе минимум данных для 
//управления объектом
class SXGUIControl : public virtual ISXGUIControl
{
public:
	SXGUIControl();
	SXGUIControl(HWND window_handle,HWND parent_handle,WNDPROC handler);
	~SXGUIControl();
	void Release(){ mem_del(this); }
	void Init(HWND window_handle, HWND parent_handle, WNDPROC handler);	//запоминаем дескрипторы, и устанавливаем новый обработчик

	HWND Parent();						//возвращает HWND родителя
	bool Parent(HWND parent_handle);	//устанавливает родителя

	HWND GetHWND();

	void Visible(bool bf);				//установка видимости контрола
	bool Visible();						//видим ли контрол?

	void SetFocus();					//установить фокус на контрол
	bool Focus();						//установлен ли фокус на контроле?

	void * GetUserPtr();
	void * SetUserPtr(void *);

private:
	void * UserPtr;

protected:
	HWND ParentHandle;
	HWND WindowHandle;
};

//второй в очереди родитель, часто наследование идет именно от него
//более расширенное управление большинством элементов
class SXGUIComponent : public SXGUIControl, public virtual ISXGUIComponent
{
public:
	SXGUIComponent();
	~SXGUIComponent();
	void Release(){ mem_del(this); }
	//устанавливает необходимые данные для управления элементом
	void InitComponent(); //необходимо вызывать после инициализации

	//действителен ли элемент
	bool Enable();
	void Enable(bool bf);

	//работа со шрифтом
	//если name == 0 то не изменияем название шрифта,
	//если name[0] == 0 то дефолтный шрифт gui
	//если остальные значения в -1 то не учитываются
	void SetFont(const char* name, int height, int width, int weight, int italic, int underline, int strike_out);
	void SetFont(HFONT hfont);
	HFONT GetFont();
	
	bool IsParentFont();	//наследуется ли шрифт родителя
	HFONT ParentFont();		//получает родительский шрифт

	//работа со стилями (в т.ч. специальным)
	//style_add - добавляемые стили
	//style_del - удаляемые
	bool ModifyStyle(long style_add, long style_del);
	bool ModifyExStyle(long style_add, long style_del);

	//устанавливает/возвращает область окна RECT элемента в глобальных координатах
	bool	SetWinRect(RECT* rect, bool alignment_screen_space);	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	RECT*	GetWinRect();
	void	GetWinRect(RECT* rect);

	//устанавливает/возвращает клиентскую область RECT элемента в глобальных координатах
	bool	SetClientRect(RECT* rect, bool alignment_screen_space);	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	void	GetClientRect(RECT* rect);

	//!!!первоначально нужно единожды включить видимость подсказки, ибо в этот момент она инициализируется
	//видимость подсказки
	void ShowHint(bool bf);
	bool ShowHint();

	//работа с текстом подсказки для элемента
	void SetHintText(const char* text);
	const char* GetHintText();
	void GetHintText(char* buf);

	//функции для цветов
	//set/get цвет текста
	void SetColorText(BYTE r, BYTE g, BYTE b);
	void SetColorText(DWORD color);
	DWORD GetColorText();

	//set/get прозрачность заднего фона текста
	void SetTransparentTextBk(bool bf);
	bool GetTransparentTextBk();

	//set/get цвет заднего фона
	void SetColorTextBk(BYTE r, BYTE g, BYTE b);
	void SetColorTextBk(DWORD color);
	DWORD GetColorTextBk();
	
	//set/get цвет заднего фона элемента
	bool SetColorBrush(BYTE r, BYTE g, BYTE b);
	bool SetColorBrush(DWORD color);
	DWORD GetColorBrush();
	HBRUSH GetBrush();

	//все что касается обработчиков функций
	///////////////////////////////////////

	//добавление обработчика
	bool AddHandler(HandlerMsg Handler, UINT Msg, WPARAM wParam, bool considerWparam, LPARAM lParam, bool considerLparam, bool isMain);
	bool AddHandler(HandlerMsg Handler, UINT Msg);	//все что необъявлено из функции выше не учитывается

	//key - ключ в массиве обработчиков
	HandlerMsg GetHandlerFunction(int key);	//возвращает функцию обработчик 
	int GetCountKeyArrHandler();			//количество ключей в массиве обработчиков

	//все что касается данных сообщения
	UINT GetMsgHandler(int key);
	WPARAM GetWParamHandler(int key);
	bool GetConsiderWParamHandler(int key);	//учитывать WParam?
	LPARAM GetLParamHandler(int key);
	bool GetConsiderLParamHandler(int key);	//учитывать LParam?

	bool IsMainFunction(int key);	//это главная функция?
	LRESULT ExecuteHandler(int key, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	//выполняет функцию обработчик, в аргументы отправляет свои аргументы
	///////////////////////////////////////

	void UpdateSize();
	void UpdateRect();


protected:

	//данные необходимые для UpdateSize
	RECT ParentRect;		//!< координаты родителя
	RECT OffsetParentRect;	//!< на сколько был изменен родитель
	RECT WinScreenRect;		//!< координаты текущего окна относительно рабочего стола

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
class SXGUITextual : public SXGUIComponent, public virtual ISXGUITextual
{
public:
	SXGUITextual();
	~SXGUITextual();
	void Release(){ mem_del(this); }
	bool	SetText(const char* text);
	void	GetText(char* buf, int size);
	int		GetTextLen();
};


#endif