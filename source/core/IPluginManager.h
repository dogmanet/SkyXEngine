#ifndef __IPLUGINMANAGER_H
#define __IPLUGINMANAGER_H

#include <gdefines.h>
#include <xcommon/IXPlugin.h>

// Интерфейс для работы с плагинами движка


class IPluginManager: public IXUnknown
{
public:
	//! загружает .dll/.so плагина, возвращает интерфейс, в случае неудачи - возвращает NULL
	virtual IXPlugin *loadPlugin(const char *szPath) = 0;

	/*! получает интерфейс по его UUID, так как интерфейсов одного типа может быть много 
	    (в зависимости от загруженных плагинов), 
	    для перебора всех - увеличиваем uStartFrom до тех пор, пока вызов не вернет NULL
	*/
	virtual IXUnknown *getInterface(const XGUID &guid, UINT uStartFrom = 0) = 0;

	/*! Позволяет зарегистрировать сторонний интерфейс (в качестве параметров передается GUID 
	    и указатель на экземпляр интерфейса. getInterface вернет именно этот экземпляр)
	*/
	virtual void registerInterface(const XGUID &guid, IXUnknown *pIFace) = 0;
};

#endif
