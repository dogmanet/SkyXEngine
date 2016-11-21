//-----------------------------------------------------
// Файл: SXGUI_img_button.h
// Краткое описание: кнопка в "своем стиле"
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

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
	~SXGUIButtonImg();

	void ReInitImage(const char* path);

	void InitCallBack();

	/*DWORD AlphaColor;	//цвет который будет исключен
	DWORD BkColor;	//цвет фона кнопки
	DWORD FrameColor;	//цвет фона кнопки

	WORD Pos;			//позиция в которой состоит кнопка
	bool PosBf[3];		//нужна ли перерисовка состоянию

	bool EnableState;	//состояние кнопки (активна/пассивна)
	bool EnableBf;		//включено ли изменение активности кнопки
	bool EnableGroup;	//включена ли кнопка в группу всех дочерних элементов родителя, это необходимо чтобы деактивировать кнопку при нажатиии другой кнопки

	BITMAP Bitmap;
	HANDLE HBitMap;

	bool IsImages;*/
	
	void GetPathForImg(char* buf);

private:
	char PathForImg[1024];
};

#endif