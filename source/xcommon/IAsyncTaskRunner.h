#ifndef __IASYNCTASKRUNNER_H
#define __IASYNCTASKRUNNER_H

#include <gdefines.h>

class IAsyncTask: public IXUnknown
{
public:
	//! Выполняйте здесь обработку задачи
	virtual void XMETHODCALLTYPE exec() = 0;
	//! Вызывается при необходимости отменить задачу (должен быть потокобезопасный)
	virtual void XMETHODCALLTYPE abort() = 0;
	//! Вызывается для завершенной задачи по IAsyncTaskRunner::runCallbacks()
	virtual void XMETHODCALLTYPE onFinished() = 0;

	//! Имя задачи для отображения в очереди выполнения
	virtual const char* XMETHODCALLTYPE getName() = 0;

	// Прогресс выполнения задачи, в диапазоне [0..100], либо -1, если подсчет прогресса не применим применимо
	virtual int XMETHODCALLTYPE getProgress() = 0;
};

class IAsyncTaskRunner: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE runTask(IAsyncTask *pTask) = 0;
	virtual void XMETHODCALLTYPE runCallbacks() = 0;
};

#endif
