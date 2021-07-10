#ifndef _PERLIN_NOISE_H_
#define _PERLIN_NOISE_H_

class CPerlinNoise final
{
public:
	// Constructor
	CPerlinNoise() = default;
	CPerlinNoise(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

	// Get Height
	double getHeight(double x, double y) const;

	// Get
	double getPersistence() const
	{
		return persistence;
	}
	double getFrequency() const
	{
		return frequency;
	}
	double getAmplitude() const
	{
		return amplitude;
	}
	int getOctaves() const
	{
		return octaves;
	}
	int getRandomSeed() const
	{
		return randomseed;
	}

	// Set
	void set(double _persistence, double _frequency, double _amplitude, int _octaves, int _randomseed);

	void setPersistence(double _persistence)
	{
		persistence = _persistence;
	}
	void setFrequency(double _frequency)
	{
		frequency = _frequency;
	}
	void setAmplitude(double _amplitude)
	{
		amplitude = _amplitude;
	}
	void setOctaves(int _octaves)
	{
		octaves = _octaves;
	}
	void setRandomSeed(int _randomseed)
	{
		randomseed = _randomseed;
	}

private:

	double getTotal(double i, double j) const;
	double getValue(double x, double y) const;
	double interpolate(double x, double y, double a) const;
	double getNoise(int x, int y) const;

	double persistence = 0.0;
	double frequency = 0.0; 
	double amplitude = 0.0;
	int octaves = 0;
	int randomseed = 0;
};

#endif
