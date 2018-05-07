
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_IMG_BUTTON_H
#define __SXGUI_IMG_BUTTON_H

#include "base.h"


class CGUIButtonImg : public CGUIComponent, public virtual ISXGUIButtonImg
{
public:
	CGUIButtonImg();
	CGUIButtonImg(const char* path, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, ID idObj);
	CGUIButtonImg(WORD button, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, ID idObj);
	void Release(){ mem_del(this); }
	~CGUIButtonImg();

	void reInitImage(const char* path);

	void initCallBack();
	
	void getPathForImg(char* buf);

	void setColorFrame(DWORD color);			
	DWORD getColorFrame();						

	void setColorBk(DWORD color);			
	DWORD getColorBk();						

	void setColorAlpha(DWORD color);			
	DWORD getColorAlpha();						

	void setEnable(bool bf);	//!< установка состояния кнопки (активна/пассивна)
	bool getEnable();			//!< возвращает состояния кнопки (активна/пассивна)

	void setActive(bool bf);	//!< установка состояния активности кнопки (включена/выключена)
	bool getActive();			//!< включено ли изменение активности кнопки?

	void setParentGroup(bool bf);	//!< установить входит/не входит кнопка в группу остальных таких же кнопок своего родителя
	bool getParentGroup();

	BITMAP Bitmap;
	HANDLE HBitMap;

	bool IsImages;

	DWORD AlphaColor;	//!< цвет который будет исключен
	DWORD BkColor;		//!< цвет фона кнопки
	DWORD FrameColor;	//!< цвет фона кнопки

	WORD Pos;			//!< позиция в которой состоит кнопка
	bool PosBf[3];		//!< нужна ли перерисовка состоянию

	bool EnableState;	//!< состояние кнопки (активна/пассивна)

	bool EnableBf;		//!< включено ли изменение активности кнопки
	bool EnableGroup;	//!< включена ли кнопка в группу всех дочерних элементов родителя, это необходимо чтобы деактивировать кнопку при нажатиии другой кнопки


private:
	char PathForImg[1024];
};

#endif