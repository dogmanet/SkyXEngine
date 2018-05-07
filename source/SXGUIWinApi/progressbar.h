
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_PROGRESS_BAR_H
#define __SXGUI_PROGRESS_BAR_H

#include "base.h"


class CGUIProgressBar : public CGUIComponent, public virtual ISXGUIProgressBar
{
public:
	CGUIProgressBar();
	CGUIProgressBar(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIProgressBar(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, bool vertical, bool smooth);

	//получить/установить позицию прогресса
	int getPos();				//возвращает текущую позицию
	int setPos(int pos);		//возвращает старую позицию

	//получить/установить размер шага прогресса
	int getStep();				//возвращает текущий размер шага
	int setStep(int step);		//возвращает старый размер шага

	//возвращают предшествующую позицию
	int setPosSteps(int steps);	//установить позицию по количеству шагов
	int setOneStep();			//увеличить позицию на один шаг

	//устанавливает позицию минимума и максимума прогресса
	//при удачном выполнении возвращает true, иначе false
	bool setMinMax(int min,int max);	

	//возвращают минимальную/максимальную позицию прогресса
	int getMin();
	int getMax();

	//установить/получить цвет полоски (дефолтовый цвет определен вручную RGB(51,153,255))
	bool setColorBar(DWORD dwColor);	//при удачном выполнении возвращает true, иначе false
	DWORD getColorBar();

	//установить/получить цвет фона (дефолтовый цвет равен цвету фона родителя)
	bool setColorBk(DWORD dwColor); //при удачном выполнении возвращает true, иначе false
	DWORD getColorBk();
};

#endif