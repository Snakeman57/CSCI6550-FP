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
	bool disaster[5]; // 0=earthquake, 1=volcano, 2=tornado, 3=hurricane, 4=tsunami
};
UENUM()
enum Biomes {
	Tropical_Rainforest UMETA(DisplayName = "Tropical Rainforest"),
	Savannah UMETA(DisplayName = "Savannah"),
	Desert UMETA(DisplayName = "Desert"),
	Chaparral UMETA(DisplayName = "Chaparral"),
	Temperate_Steppe UMETA(DisplayName = "Temperate Steppe"),
	Temperate_Broadleaf_Forest UMETA(DisplayName = "Temperate Broadleaf Forest"),
	Coniferous_Forest UMETA(DisplayName = "Coniferous Forest"),
	Tundra UMETA(DisplayName = "Tundra"),
	Alpine_Tundra UMETA(DisplayName = "Alpine Tundra"),
	Polar_Ice UMETA(DisplayName = "Polar Ice"),
	MAX_BIOM UMETA(Hidden)
};
struct Biome {
	Biomes name;
	float supply; // base supply
	FRichCurve freq; // frequency by latitute -100 to 100
	float disaster[sizeof(Weather::disaster)]; // chances for natural disasters
};
struct Neighbor {
	int loc; // index of location being referenced
	int dist; // difficulty to get to location
};
struct Location {
	TArray<Neighbor> nbrs; // neighbors
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
	~TheWorld();
	void init(const int& lat1, const int& lat2, const int& size, const int& pop);
	void tick(); // does one tick of the sim
	void updtSupply(); // moves supply in each location towards base
	void updtWeather(); // changes the weather in each location
	WorldInfo getWstats() const;
	PopInfo getPstats() const;
private:
	Biome biomes[Biomes::MAX_BIOM]; // list of possible biomes
	const int bsRt; // base years to recover from ecological collapse (if supply is 0 in a lcation it will take bsRt years to get back to base)
	TArray<Location> l; // dynamically allocatable array
	WorldInfo info; // info from l
	Population p; // population of ppls
	void asgnBiomes();
	Biomes getBiome(int& lat) const; // returns a biome based on the probability of occurring at a given latitue
};
