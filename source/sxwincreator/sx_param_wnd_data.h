
#ifndef __PARAM_WND_DATA_H
#define __PARAM_WND_DATA_H

#include <windows.h>
#include <SXGUIWinApi/sxgui.h>
#include "sxwincreator.h"

namespace SXUpdateParam
{
	//обноелние информации активного элемента
	void PosSize();	// позиция и габариты (x,y width,height)
	void Caption();
	void VarName();
	void Enable();
	void Visible();
	void VisibleHint();
	void HintText();
	void ColorText();
	void ColorTextBK();
	void ColorBK();
	void TransparentTextBk();
	void ParentFont();
	void Font();
	void MenuWindow();

	void UpdateAll();

	void SetPosX();
	void SetPosY();
	void SetWidth();
	void SetHeight();
	void SetVisible();
	void SetEnable();
	void SetCaption();
	void SetVarName();
	void SetVisibleHint();
	void SetHintText();
	void SetColorText();
	void SetColorTextBK();
	void SetColorBK();
	void SetTransparentTextBk();
	void SetParentFont();
	void SetFont();
	void SetImgButton();
	void SetMenuWindow();

	void LockPosSizeBlock();
	void LockFontBlock();
	void LockColorBlock();
	void LockColorTextBlock();
	void LockImgButton();

	void UnLockPosSizeBlock();
	void UnLockFontBlock();
	void UnLockColorBlock();
	void UnLockColorTextBlock();
	void UnlockAll();
	void UnLockImgButton();

	void SetDefaultValue();
}

void SaveFile(const char* path);

void LoadFile(const char* path);



void CreateNew(bool reestablish);

void CallCreateNew();

void OutputInFile(const char* path, const char* name_space, bool in_ns, bool constructors, bool destructors, bool comments, bool include);

#endif