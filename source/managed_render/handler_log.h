
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*! 
\file
Файл логирования и обработки сообщений
*/

/*! \defgroup managed_render_out_log out_log - логирование и обработка сообщений
 \ingroup managed_render
@{*/

#ifndef __HANDLER_LOG_H
#define __HANDLER_LOG_H

#include <fstream>
#include <stdio.h>
#include <windows.h>

#include <gdefines.h>

//! перебор всех окон процесса для их сворачивания
BOOL CALLBACK HE_EnumWindowsProc(HWND hwnd, LPARAM lParam);

//! обработка ошибки
void HE_HandlerError(const char* format, ...);

//! инициализация потока ведения лога
void InitOutLog();

//! функция ведения лога и обработки сообщений
void PrintfLog(int level, const char* format, ...);

#endif

//!@} managed_render_out_log
