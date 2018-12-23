

#ifndef __CALLBACKS_H
#define __CALLBACKS_H

#include <windows.h>
#include <SXGUIWinApi/sxgui.h>
#include "sxwincreator.h"
#include "sx_param_wnd_data.h"

LRESULT CALLBACK WndProcChildJob(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WndProcChildJob2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//найден ли хотя бы один groupbox или static
//bool IsFindGroupBox = false;

BOOL CALLBACK GroupBoxLButtonDown(HWND hwnd, LPARAM lParam);

//сообщение клика левой кнопкой мыши по рабочему окну
LRESULT AddElement(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK GroupBoxMouseMove(HWND hwnd, LPARAM lParam);

BOOL CALLBACK GroupBoxRButtonUp(HWND hwnd, LPARAM lParam);



LRESULT MouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT RButtonUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ListBoxAllElementsDBLClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT InitColorText(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT InputInfoEdit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT InputInfoComboBox(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT InputToEditColor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT InitFont(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT InitImageForButtonImg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CallWmCommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SaveResult();



void LoadResult();

void CallLoadResult();

void CallOutputWnd();

void CallAboutSXWinCreator();

LRESULT ComMenuId(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT MinimuzeWinInsteadClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT TrueExit(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CallImgNew(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CallImgSave(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CallImgLoad(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CallImgBuild(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif