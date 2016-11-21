#include "keymap.h"
#include <memory>


namespace GUI
{
	bool KeyMap::keys[KEYMAP_SIZE];

	void KeyMap::Init()
	{
		ReleaseKeys();
	}
	void KeyMap::ReleaseKeys()
	{
		memset(keys, 0, sizeof(bool) * KEYMAP_SIZE);
	}
	bool KeyMap::KeyDown(int key)
	{
		if(!keys[key])
		{
			keys[key] = true;
			return(true);
		}
		return(false);
	}
	bool KeyMap::KeyUp(int key)
	{
		if(keys[key])
		{
			keys[key] = false;
			return(true);
		}
		return(false);
	}

	bool KeyMap::KeyState(int key)
	{
		return(keys[key]);
	}

	const bool * KeyMap::GetKeys()
	{
		return(keys);
	}
};