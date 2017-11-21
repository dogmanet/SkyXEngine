
#ifndef SXGUI_IMG_BUTTON_H
#define SXGUI_IMG_BUTTON_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIButtonImg : public SXGUIComponent, public virtual ISXGUIButtonImg
{
public:
	SXGUIButtonImg();
	SXGUIButtonImg(const char* path,WORD x,WORD y,WORD width,WORD heigth,DWORD alpha_color,DWORD bk_color,HWND parent,WNDPROC handler,DWORD id);
	SXGUIButtonImg(WORD button,WORD x,WORD y,WORD width,WORD heigth,DWORD alpha_color,DWORD bk_color,HWND parent,WNDPROC handler,DWORD id);
	void Release(){ mem_del(this); }
	~SXGUIButtonImg();

	void ReInitImage(const char* path);

	void InitCallBack();
	
	void GetPathForImg(char* buf);

	void SetColorFrame(BYTE r, BYTE g, BYTE b);	
	void SetColorFrame(DWORD color);			
	DWORD GetColorFrame();						

	void SetColorBk(BYTE r, BYTE g, BYTE b);	
	void SetColorBk(DWORD color);			
	DWORD GetColorBk();						

	void SetColorAlpha(BYTE r, BYTE g, BYTE b);	
	void SetColorAlpha(DWORD color);			
	DWORD GetColorAlpha();						

	void SetEnable(bool bf);	//!< установка состояния кнопки (активна/пассивна)
	bool GetEnable();			//!< возвращает состояния кнопки (активна/пассивна)

	void SetEnableActive(bool bf);	//!< установка состояния активности кнопки (включена/выключена)
	bool GetEnableActive();			//!< включено ли изменение активности кнопки?

	void SetParentGroup(bool bf);	//!< установить входит/не входит кнопка в группу остальных таких же кнопок своего родителя
	bool GetParentGroup();

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