
#ifndef __ambient_sounds_h
#define __ambient_sounds_h

class AmbientSounds
{
public:
	AmbientSounds();
	~AmbientSounds();

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