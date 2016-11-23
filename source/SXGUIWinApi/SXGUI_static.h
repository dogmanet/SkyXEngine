//-----------------------------------------------------
// ����: SXGUI_static.h
// ������� ��������: ����������� ������
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_STATIC_H
#define SXGUI_STATIC_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIStatic : public SXGUITextual, public virtual ISXGUIStatic
{
public:
	SXGUIStatic();
	SXGUIStatic(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIStatic(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	//������� �����
	SXGUIStatic(WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool vertical);

	WORD Align();			//���������� ���� ������������ ������
	bool Align(WORD align);	//������������� ������������ ������

	bool WordWrap();		//���������� �� ����������� �����
	bool WordWrap(bool bf); //����������� �� ��������� ����������� �����
};

#endif