
#ifndef __GAME_CALLBACK_H
#define __GAME_CALLBACK_H

#include "skyxengine.h"
#include "level_editor.h"

//! переключатель между свойствами и соединениями игрового объекта 
LRESULT SXLevelEditor_ButtonGameTab_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//**************************************************************************

//! клик по ListViewGameClass
LRESULT SXLevelEditor_ListViewGameClass_Click();

//! применение введенного значения для параметра игрового объекта
LRESULT SXLevelEditor_EditGameValue_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! вызов диалога выбора для смены параметра игрового объекта
LRESULT SXLevelEditor_ButtonGameValue_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! создать игровой объект
LRESULT SXLevelEditor_ButtonGameCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//**************************************************************************

//! склейка значений из таблицы значений
void SXLevelEditor_VeldStringGameConnections(
	proptable_t *pPropTable,	//!< таблица данных которая содержит pPropData (возможно надо убрать)
	propdata_t *pPropData,		//!< брать сроки из таблицы только если в юзердате записано это
	int iNumExclude,			//!< исключить строку таблицы с этим номером
	char *szStrOut,				//!< строка в которую записываеются все склееные данные
	int iSize					//!< размер szStrOut	(возможно надо убрать)
	);

//! клик по таблице соединений
LRESULT SXLevelEditor_ListViewGameConnections_Click();

//! обработка сообщений ввода в EditGameConnectionsName чтобы была автоподстановка
LRESULT SXLevelEditor_EditGameConnectionsName_IN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! применение значение введенного в EditGameConnectionsName
LRESULT SXLevelEditor_EditGameConnectionsName_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! применение всех параметров с редакторов к выделенному соединению
LRESULT SXLevelEditor_EditGameConnections_Enter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! создание нового соединения
LRESULT SXLevelEditor_ButtonGameConnectionsCreate_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//! удаление выделенного соединения
LRESULT SXLevelEditor_ButtonGameConnectionsDelete_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



#endif