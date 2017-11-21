
#ifndef SXGUI_PROGRESS_BAR_H
#define SXGUI_PROGRESS_BAR_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIProgressBar : public  SXGUIComponent, public virtual ISXGUIProgressBar
{
public:
	SXGUIProgressBar();
	SXGUIProgressBar(WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIProgressBar(WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool vertical,bool smooth);

	//получить/установить позицию прогресса
	int GetPos();				//возвращает текущую позицию
	int SetPos(int pos);		//возвращает старую позицию

	//получить/установить размер шага прогресса
	int GetStep();				//возвращает текущий размер шага
	int SetStep(int step);		//возвращает старый размер шага

	//возвращают предшествующую позицию
	int SetPosSteps(int steps);	//установить позицию по количеству шагов
	int SetOneStep();			//увеличить позицию на один шаг

	//устанавливает позицию минимума и максимума прогресса
	//при удачном выполнении возвращает true, иначе false
	bool SetMinMax(int min,int max);	

	//возвращают минимальную/максимальную позицию прогресса
	int GetMin();
	int GetMax();

	//установить/получить цвет полоски (дефолтовый цвет определен вручную RGB(51,153,255))
	bool SetBarColor(BYTE r, BYTE g, BYTE b);	//при удачном выполнении возвращает true, иначе false
	DWORD GetBarColor();

	//установить/получить цвет фона (дефолтовый цвет равен цвету фона родителя)
	bool SetBkColor(BYTE r, BYTE g, BYTE b); //при удачном выполнении возвращает true, иначе false
	DWORD GetBkColor();
};

#endif