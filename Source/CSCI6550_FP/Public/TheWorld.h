// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "Population.h"
#include "Curves/CurveFloat.h"
#include <Runtime\Core\Public\Misc\Paths.h>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>

struct Weather {
	float rain = 0; // ideal rainfall modifier
	float temp = 0; // ideal tmp modifier
	bool disaster[4]; // 0=earthquake, 1=volcano, 2=tornado, 3=hurricane, 4=tsunami
};
enum Biomes {
	Tropical_Rainforest,
	Savannah,
	Desert,
	Chaparral,
	Temperate_Steppe,
	Temperate_Broadleaf_Forest,
	Coniferous_Forest,
	Tundra,
	Alpine_Tundra,
	Polar_Ice,
	MAX_BIOM
};
struct Biome {
	Biomes name;
	float supply; // base supply
	FRealCurve freq; // frequency by latitute -100 to 100
	float disaster[sizeof(Weather::disaster)]; // chances for natural disasters
};
struct Neighbor {
	int loc; // index of location being referenced
	int dist; // difficulty to get to location
};
struct Location {
	Neighbor* nbrs; // neighbors
	Biomes biome; // biome
	Weather weather; // weather
	float supply; // supply
};
struct WorldInfo {
	int locations; // number of locations
	int lat1, lat2; // lattitude range
	int biomes[Biomes::MAX_BIOM]; // number of each biomme
	float avgSupply; // average supply value
};
class CSCI6550_FP_API TheWorld {
public:
	TheWorld();
	TheWorld(int lat1, int lat2, int size, int pop);
	~TheWorld();
	void init(int lat1, int lat2, int size, int pop);
	void tick(); // does one tick of the sim
	void updtSupply(); // moves supply in each location towards base
	void updtWeather(); // changes the weather in each location
	WorldInfo getWstats() const;
	PopInfo getPstats() const;
private:
	Biome biomes[Biomes::MAX_BIOM]; // list of possible biomes
	const int bsRt; // base years to recover from ecological collapse (if supply is 0 in a lcation it will take bsRt years to get back to base)
	Location* l; // dynamically allocatable array
	WorldInfo info; // info from l
	Population p; // population of ppls
	void asgnBiomes();
	Biomes getBiome(int lat); // returns a biome based on the probability of occurring at a given latitue
};
