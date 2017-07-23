
#ifndef __level_ambient_sounds_h
#define __level_ambient_sounds_h

class LevelAmbientSounds
{
public:
	LevelAmbientSounds();
	~LevelAmbientSounds();

	void Add(const char* path);
	void Get(ID id, char* path);
	UINT GetCount();
	void Clear();

	void Play();
	void Update();
	void Pause();

	bool IsPlaying();

protected:
	Array<ID> ArrIDsSnds;
	bool playing;
	int PlayingLast;
};

#endif