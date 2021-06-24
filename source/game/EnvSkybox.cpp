#include "EnvSkybox.h"

/*! \skydocent env_skybox

*/

BEGIN_PROPTABLE(CEnvSkybox)
	//! Значение по умолчанию
	DEFINE_FIELD_STRINGFN(m_szSkyBox, PDFF_NONE, "skybox", "Skybox texture", setTexture, EDITOR_TEXTURE)

	//! Сброс на значение по умолчанию
	DEFINE_INPUT(reset, "reset", "Reset", PDF_NONE)
	//! Установка значения
	DEFINE_INPUT(setSkybox, "setSkybox", "Set skybox", PDF_STRING)

	//! Включить
	DEFINE_INPUT(turnOn, "turnOn", "Turn on", PDF_NONE)
	//! Выключить
	DEFINE_INPUT(turnOff, "turnOff", "Turn off", PDF_NONE)

	//! Изначально выключен
	DEFINE_FLAG(SKYBOX_DISABLED, "Initially dark")
END_PROPTABLE()

REGISTER_ENTITY(CEnvSkybox, env_skybox, REC_ICON("dev_env_skybox"));

CEnvSkybox::CEnvSkybox()
{
	m_pAmbient = (IXAmbient*)Core_GetIXCore()->getPluginManager()->getInterface(IXAMBIENT_GUID);
}

CEnvSkybox::~CEnvSkybox()
{
	//mem_release(m_pAmbient);
}

void CEnvSkybox::setTexture(const char *szValue)
{
	_setStrVal(&m_szSkyBox, szValue);

	if(m_pAmbient)
	{
		m_pAmbient->setSkybox(szValue[0] ? szValue : NULL);
	}
}

void CEnvSkybox::reset(inputdata_t *pInputdata)
{
	if(m_pAmbient)
	{
		m_pAmbient->setSkybox(m_szSkyBox);
	}
}
void CEnvSkybox::setSkybox(inputdata_t *pInputdata)
{
	assert(pInputdata->type == PDF_STRING);

	if(m_pAmbient)
	{
		m_pAmbient->setSkybox(pInputdata->parameter.str);
	}
}

void CEnvSkybox::turnOn(inputdata_t *pInputdata)
{
	if(m_pAmbient)
	{
		m_pAmbient->enableSkybox(true);
	}
}
void CEnvSkybox::turnOff(inputdata_t *pInputdata)
{
	if(m_pAmbient)
	{
		m_pAmbient->enableSkybox(false);
	}
}

void CEnvSkybox::updateFlags()
{
	BaseClass::updateFlags();

	if(m_pAmbient)
	{
		m_pAmbient->enableSkybox(!(getFlags() & SKYBOX_DISABLED));
	}
}
