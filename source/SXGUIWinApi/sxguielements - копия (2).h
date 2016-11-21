


//первый базовый класс, единственным родителем не используется
//определяет объект как контрол, то есть содержит в себе минимум данных для 
//управления объектом
struct ISXGUIControl
{
	virtual void Init(HWND window_handle, HWND parent_handle, WNDPROC handler);	//запоминаем дескрипторы, и устанавливаем новый обработчик
	virtual void Release();
	virtual HWND& Parent();						//возвращает HWND родителя
	virtual bool Parent(HWND parent_handle);	//устанавливает родителя

	virtual HWND& GetHWND();

	virtual void Visible(bool bf);				//установка видимости контрола
	virtual bool Visible();						//видим ли контрол?

	virtual void SetFocus();					//установить фокус на контрол
	virtual bool Focus();						//установлен ли фокус на контроле?
	virtual WNDPROC& GetOldProc();
};


//второй в очереди родитель, часто наследование идет именно от него
//более расширенное управление большинством элементов
struct ISXGUIComponent : public ISXGUIControl
{
	//устанавливает необходимые данные для управления элементом
	virtual void InitComponent(); //необходимо вызывать после инициализации

	//действителен ли элемент
	virtual bool Enable();
	virtual void Enable(bool bf);

	//работа со шрифтом
	//если name == 0 то не изменияем название шрифта,
	//если name[0] == 0 то дефолтный шрифт gui
	//если остальные значения в -1 то не учитываются
	virtual void SetFont(const char* name, int height, int width, int weight, int italic, int underline, int strike_out);
	virtual void SetFont(HFONT* hfont);
	virtual HFONT GetFont();

	virtual bool IsParentFont();	//наследуется ли шрифт родителя
	virtual HFONT ParentFont();		//получает родительский шрифт

	//работа со стилями (в т.ч. специальным)
	//style_add - добавляемые стили
	//style_del - удаляемые
	virtual bool ModifyStyle(long style_add, long style_del);
	virtual bool ModifyExStyle(long style_add, long style_del);

	//устанавливает/возвращает область окна RECT элемента в глобальных координатах
	virtual bool	SetWinRect(RECT* rect, bool alignment_screen_space);	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	virtual RECT*	GetWinRect();
	virtual void	GetWinRect(RECT* rect);

	//устанавливает/возвращает клиентскую область RECT элемента в глобальных координатах
	virtual bool	SetClientRect(RECT* rect, bool alignment_screen_space);	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	virtual RECT*	GetClientRect();
	virtual void	GetClientRect(RECT* rect);

	//!!!первоначально нужно единожды включить видимость подсказки, ибо в этот момент она инициализируется
	//видимость подсказки
	virtual void ShowHint(bool bf);
	virtual bool ShowHint();

	//работа с текстом подсказки для элемента
	virtual void SetHintText(const char* text);
	virtual char* GetHintText();
	virtual void GetHintText(char* buf);

	//функции для цветов
	//set/get цвет текста
	virtual void SetColorText(BYTE r, BYTE g, BYTE b);
	virtual DWORD GetColorText();

	//set/get прозрачность заднего фона текста
	virtual void SetTransparentTextBk(bool bf);
	virtual bool GetTransparentTextBk();

	//set/get цвет заднего фона
	virtual void SetColorTextBk(BYTE r, BYTE g, BYTE b);
	virtual DWORD GetColorTextBk();

	//set/get цвет заднего фона элемента
	virtual bool SetColorBrush(BYTE r, BYTE g, BYTE b);
	virtual DWORD GetColorBrush();
	virtual HBRUSH GetBrush();

	//все что касается обработчиков функций
	///////////////////////////////////////

	//добавление обработчика
	virtual bool AddHandler(HandlerMsg Handler, UINT Msg, WPARAM wParam, bool considerWparam, LPARAM lParam, bool considerLparam, bool isMain);
	virtual bool AddHandler(HandlerMsg Handler, UINT Msg);	//все что необъявлено из функции выше не учитывается

	//key - ключ в массиве обработчиков
	virtual HandlerMsg GetHandlerFunction(int key);	//возвращает функцию обработчик 
	virtual WORD GetCountKeyArrHandler();			//количество ключей в массиве обработчиков

	//все что касается данных сообщения
	virtual UINT GetMsgHandler(int key);
	virtual WPARAM GetWParamHandler(int key);
	virtual bool GetConsiderWParamHandler(int key);	//учитывать WParam?
	virtual LPARAM GetLParamHandler(int key);
	virtual bool GetConsiderLParamHandler(int key);	//учитывать LParam?

	virtual bool IsMainFunction(int key);	//это главная функция?
	virtual LRESULT ExecuteHandler(int key, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	//выполняет функцию обработчик, в аргументы отправляет свои аргументы
	///////////////////////////////////////

	virtual void UpdateSize();
	virtual void UpdateRect();
};

//третий (если нужен текст) родитель, непосредтсвенно не используется
//характеризует элемент как содержащий единый текст (аля кэпшен) текстовый компонент
struct ISXGUITextual : public ISXGUIComponent
{
	virtual bool SetText(const char* text);
	virtual const char*	GetText();
	virtual void GetText(char* buf, int size);
	virtual int GetTextLen();
};

////

struct ISXGUIBaseWnd : public ISXGUITextual
{
	virtual bool SetColorBrush(BYTE r, BYTE g, BYTE b);
	virtual void GetColorBrush(DWORD color);
	virtual HBRUSH GetBrush();

	virtual bool Alpha(WORD alpha);
	virtual WORD Alpha();
	virtual bool AlphaBlend(bool bf);
	virtual bool AlphaBlend();
};

SX_LIB_API ISXGUIBaseWnd* SXCreateBaseWnd(
	const char* class_name, const char* caption, const char* menu,
	WORD id, WORD x, WORD y, WORD width, WORD heigth,
	HICON icon, HCURSOR cursor, HBRUSH brush,
	DWORD exstyle, DWORD wndstyle, DWORD style,
	HWND parent, WNDPROC handler
	);