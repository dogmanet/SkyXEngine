#include "keymap.h"
#include <memory>


namespace gui
{
	bool CKeyMap::m_keys[KEYMAP_SIZE];

	void CKeyMap::init()
	{
		releaseKeys();
	}
	void CKeyMap::releaseKeys()
	{
		memset(m_keys, 0, sizeof(bool)* KEYMAP_SIZE);
	}
	bool CKeyMap::keyDown(int key)
	{
		if(!m_keys[key])
		{
			m_keys[key] = true;
			return(true);
		}
		return(false);
	}
	bool CKeyMap::keyUp(int key)
	{
		if(m_keys[key])
		{
			m_keys[key] = false;
			return(true);
		}
		return(false);
	}

	bool CKeyMap::keyState(int key)
	{
		return(m_keys[key]);
	}

	const bool * CKeyMap::getKeys()
	{
		return(m_keys);
	}
};