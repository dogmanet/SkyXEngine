
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_BASE_H
#define __SXGUI_BASE_H

#include "hint.h"
#include "SXGUI.h"
#include <common/array.h>


//первый базовый класс, единственным родителем не используется
//определяет объект как контрол, то есть содержит в себе минимум данных для 
//управления объектом
class CGUIControl : public virtual ISXGUIControl
{
public:
	CGUIControl();
	CGUIControl(HWND hWindow, HWND hParent, WNDPROC lpfnHandler);
	~CGUIControl();

	void Release(){ mem_del(this); }


	void init(HWND hWindow, HWND hParent, WNDPROC lpfnHandler, ID idObj);	//запоминаем дескрипторы, и устанавливаем новый обработчик

	HWND getParent();						//возвращает HWND родителя
	bool setParent(HWND hParent);	//устанавливает родителя

	HWND getHWND();

	void setVisible(bool isVisible);				//установка видимости контрола
	bool getVisible();						//видим ли контрол?

	void setFocus();					//установить фокус на контрол
	bool getFocus();						//установлен ли фокус на контроле?

	WNDPROC getPrevWndProc();

	void* getUserPtr();
	void* setUserPtr(void *prt);

private:
	void *m_pUserPtr;
	ID m_id;

protected:
	HWND m_hParent;
	HWND m_hWindow;
	WNDPROC m_lpfnPrevProc;								//!< старый обработчик
	
};

//##########################################################################

#define COMPONENT_PRECOD_HANDLER_ID(id, retval){if!(id >= 0 && m_aHandlers.size() > id)return retval;}

//второй в очереди родитель, часто наследование идет именно от него
//более расширенное управление большинством элементов
class CGUIComponent : public CGUIControl, public virtual ISXGUIComponent
{
public:
	CGUIComponent();
	~CGUIComponent();

	void Release(){ mem_del(this); }
	//устанавливает необходимые данные для управления элементом
	void initComponent(); //необходимо вызывать после инициализации

	//действителен ли элемент
	bool getEnable();
	void setEnable(bool isEnable);

	//работа со шрифтом
	//если name == 0 то не изменияем название шрифта,
	//если name[0] == 0 то дефолтный шрифт gui
	//если остальные значения в -1 то не учитываются
	void setFont(const char *szName, int iHeight, int iWidth, int iWeight, int iItalic, int iUnderline, int iStrikeOut);
	void setHFont(HFONT hFont);
	HFONT getHFont();
	
	bool hasParentFont();	//наследуется ли шрифт родителя
	HFONT getParentHFont();		//получает родительский шрифт

	//работа со стилями (в т.ч. специальным)
	//style_add - добавляемые стили
	//style_del - удаляемые
	bool modifyStyle(long lStyleAdd, long lStyleDel);
	bool modifyExStyle(long lStyleAdd, long lStyleDel);

	//устанавливает/возвращает область окна RECT элемента в глобальных координатах
	bool setWinRect(const RECT *pRect, bool alignment_screen_space);	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	RECT* getWinRect();
	void getWinRect(RECT *pRect);

	//устанавливает/возвращает клиентскую область RECT элемента в глобальных координатах
	bool setClientRect(const RECT *pRect, bool isBoundScreen);	//alignment_screen_space - использовать ли только работчую область (рабочий стол кроме панели задач)
	void getClientRect(RECT *pRect);

	//!!!первоначально нужно единожды включить видимость подсказки, ибо в этот момент она инициализируется
	//видимость подсказки
	void setShowHint(bool isShowHint);
	bool getShowHint();

	//работа с текстом подсказки для элемента
	void setHintText(const char *szStr);
	const char* getHintText();
	void getHintText(char *szBuf);

	//функции для цветов
	//set/get цвет текста
	void setColorText(DWORD dwColor);
	DWORD getColorText();

	//set/get прозрачность заднего фона текста
	void setTransparentTextBk(bool isTransparent);
	bool getTransparentTextBk();

	//set/get цвет заднего фона
	void setColorTextBk(DWORD dwColor);
	DWORD getColorTextBk();
	
	//set/get цвет заднего фона элемента
	bool setColorBrush(DWORD dwColor);
	DWORD getColorBrush();
	HBRUSH getBrush();

	//все что касается обработчиков функций
	///////////////////////////////////////

	//добавление обработчика
	void addHandler(HandlerFunc Handler, UINT uMsg, WPARAM wParam, bool considerWparam, LPARAM lParam, bool considerLparam, bool isMain);
	void addHandler(HandlerFunc Handler, UINT uMsg);	//все что необъявлено из функции выше не учитывается

	CHandlerData* getHandler(ID idHandler);	//возвращает обработчик 
	int getCountHandlers();			//количество ключей в массиве обработчиков

	LRESULT executeHandler(ID idHandler, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//выполняет функцию обработчик, в аргументы отправляет свои аргументы
	///////////////////////////////////////

	void updateSize();
	void updateRect();

	void setMixSize(UINT uMinWidth, UINT uMinHeght);
	UINT getMinWidth();
	UINT getMinHeight();

	void setStretchSides(bool canTop, bool canBottom, bool canRight, bool canLeft);
	void setStretchSide(SXGUI_SIDE side, bool can);
	bool getStretchSide(SXGUI_SIDE side);

	void setFollowParentSides(bool canTop, bool canBottom, bool canRight, bool canLeft);
	void setFollowParentSide(SXGUI_SIDE side, bool can);
	bool getFollowParentSide(SXGUI_SIDE side);


protected:

	//данные необходимые для UpdateSize
	RECT m_rcParentScreen;		//!< координаты родителя
	RECT m_rcOffsetParent;	//!< на сколько был изменен родитель
	RECT m_rcWinScreen;		//!< координаты текущего окна относительно рабочего стола
	
	//! всплывающая подсказка
	ISXGUIHint *m_pHint;

	//! массив обработчиков сообщений
	Array<CHandlerData> m_aHandlers;

	DWORD m_dwColorBrush;	//цвет заливки формы
	HBRUSH m_hBrush;

	DWORD m_dwColorText;	//цвет текста
	bool m_isTransparentTextBk;	//используется прозрачность? если true то заднего фона у текста не будет
	DWORD m_dwColorTextBk;	//цвет заднего фона у текста

	int m_iMinSizeWidth;	//!< минимальный размер окна по ширине
	int m_iMinSizeHeight;	//!< минимальный размер окна по высоте

	bool m_aStrethSide[SXGUI_SIDE_COUNT];
	bool m_aFollowParentSide[SXGUI_SIDE_COUNT];
};

//##########################################################################

//третий (если нужен текст) родитель, непосредтсвенно не используется
//характеризует элемент как содержащий единый текст (caption), текстовый компонент
class CGUITextual : public CGUIComponent, public virtual ISXGUITextual
{
public:
	CGUITextual();
	~CGUITextual();

	void Release(){ mem_del(this); }

	bool setText(const char* text);
	void getText(char* buf, int size);
	int getTextLen();
};


#endif