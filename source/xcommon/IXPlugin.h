#ifndef __IXPLUGIN_H
#define __IXPLUGIN_H

#include "IXCore.h"

#define X_PLUGIN_ENTRYPOINT XPluginMain
#define X_PLUGIN_API extern "C" __declspec(dllexport)
#define IXPLUGIN_VERSION 1
#define DECLARE_XPLUGIN(cls) class cls##Imp:public cls{                                                   \
public:                                                                                                   \
	cls##Imp(ID id):m_id(id){}                                                                            \
	ID XMETHODCALLTYPE getID()override{return(m_id);}                                                     \
	UINT XMETHODCALLTYPE getVersion()override{return(IXPLUGIN_VERSION);}                                  \
	void XMETHODCALLTYPE startup(IXCore *pCore)override{INIT_OUTPUT_STREAM(pCore); cls::startup(pCore);}  \
protected:                                                                                                \
	ID m_id;                                                                                              \
}; X_PLUGIN_API IXPlugin *X_PLUGIN_ENTRYPOINT(ID id){return(new cls##Imp(id));}

class IXPlugin: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getVersion() = 0;
	virtual ID XMETHODCALLTYPE getID() = 0;

	virtual UINT XMETHODCALLTYPE getInterfaceCount() = 0;
	virtual const XGUID * XMETHODCALLTYPE getInterfaceGUID(UINT id) = 0;
	virtual IXUnknown * XMETHODCALLTYPE getInterface(const XGUID &guid) = 0;

	virtual void XMETHODCALLTYPE startup(IXCore *pCore) = 0;
	virtual void XMETHODCALLTYPE shutdown() = 0;
};

typedef IXPlugin*(*PFNXPLUGINMAIN)(ID id);

#endif
