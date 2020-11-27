#ifndef __IXCONFIGLOADER_H
#define __IXCONFIGLOADER_H

#include <gdefines.h>

/*! интерфейс для работы с файлами конфигурации (ini)
\warning секции и ключи хранятся в виде дерева, и нет гарантии что может быть доступ по порядковому номеру,
можно получить общее количество секций/ключей, дальше плясать */
class IXConfig: public IXUnknown
{
public:
	// virtual void XMETHODCALLTYPE create(const char *szPath) = 0; //!< новый файл
	virtual bool XMETHODCALLTYPE open(const char *szPath) = 0; //!< открыть файл
	virtual bool XMETHODCALLTYPE save() = 0; //!< сохранить файл

	virtual const char* XMETHODCALLTYPE getKey(const char *szSection, const char *szKey) = 0; //!< получить значения ключа key который в секции section
	virtual const char* XMETHODCALLTYPE getKeyName(const char *szSection, UINT uIndex) = 0; //!< получить имя ключа по номеру
	virtual const char* XMETHODCALLTYPE getSectionName(UINT uIndex) = 0; //!< получить имя секции по номеру
	virtual void XMETHODCALLTYPE set(const char *szSection, const char *szKey, const char *szValue) = 0; //!< установить значение val ключа key котор в секции section
	virtual UINT XMETHODCALLTYPE getSectionCount() = 0; //!< количество секций в файле
	virtual UINT XMETHODCALLTYPE getKeyCount() = 0; //!< общее количество ключей
	virtual UINT XMETHODCALLTYPE getKeyCount(const char *szSection) = 0; //!< общее количество ключей в секции
	virtual bool XMETHODCALLTYPE sectionExists(const char *szSection) = 0; //!< существует ли секция section
	virtual bool XMETHODCALLTYPE keyExists(const char *szSection, const char *szKey) = 0; //!< существует ли ключ key в секции section

	virtual void XMETHODCALLTYPE clear() = 0; //!< очистить
};

#endif
