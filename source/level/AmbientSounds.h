
#ifndef __AMBIENTSOUNDS_H
#define __AMBIENTSOUNDS_H

#include <gdefines.h>
#include <common/Array.h>
#include <level/sxlevel.h>

class CAmbientSounds
{
public:
	CAmbientSounds();
	~CAmbientSounds();

	void add(const char *szPath);
	void get(ID id, char *szPath);
	UINT getCount();
	void clear();

	void play();
	void update();
	void pause();

	bool getPlaying();

protected:
	Array<ID> m_aIDSnds;
	bool m_isPlaying;
	int m_iPlayingLast;
};

#endif