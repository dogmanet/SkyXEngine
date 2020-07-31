#ifndef __ENV_SKYBOX_H
#define __ENV_SKYBOX_H

#include "PointEntity.h"
#include <xcommon/IXAmbient.h>

#define SKYBOX_DISABLED ENT_FLAG_0

class CEnvSkybox: public CPointEntity
{
	DECLARE_CLASS(CEnvSkybox, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CEnvSkybox();
	
protected:
	void reset(inputdata_t *pInputdata);
	void setSkybox(inputdata_t *pInputdata);

	void turnOn(inputdata_t *pInputdata);
	void turnOff(inputdata_t *pInputdata);

	void setTexture(const char *szValue);

	void updateFlags() override;

	const char *m_szSkyBox = NULL;

	IXAmbient *m_pAmbient = NULL;
};

#endif
