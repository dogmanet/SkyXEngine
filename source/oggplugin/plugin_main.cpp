#include <xcommon/IXPlugin.h>
#include "AudioCodecOgg.h"

#if defined(_DEBUG)
#pragma comment(lib, "ogg-vorbis_d.lib")
#else
#pragma comment(lib, "ogg-vorbis.lib")
#endif

//##########################################################################

class CPlugin_AudioCodecOgg: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
	}

	void XMETHODCALLTYPE shutdown() override
	{
	}

	UINT XMETHODCALLTYPE getInterfaceCount() override
	{
		return(1);
	}
	const XGUID * XMETHODCALLTYPE getInterfaceGUID(UINT id) override
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXAUDIOCODEC_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown * XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if (guid == IXAUDIOCODEC_GUID)
		{
			return(new CAudioCodecOgg());
		}
		return(NULL);
	}
};

DECLARE_XPLUGIN(CPlugin_AudioCodecOgg);
