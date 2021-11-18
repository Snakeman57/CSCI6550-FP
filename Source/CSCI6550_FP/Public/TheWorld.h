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
#include <iostream>
#include <sstream>

class LStream : public std::stringbuf { // for writing to console
protected:
	int sync() {
		UE_LOG(LogTemp, Log, TEXT("%s"), *FString(str().c_str()));
		str("");
		return std::stringbuf::sync();
	}
};
//LStream Stream;
//std::cout.rdbuf(&Stream);

UENUM(BlueprintType)
enum Disasters {
	quake UMETA(DisplayName = "Earthquake"),
	volcano UMETA(DisplayName = "Volcanic Eruption"),
	tornado UMETA(DisplayName = "Tornado"),
	hurricane UMETA(DisplayName = "Hurricane"),
	tsunami UMETA(DisplayName = "Tsunami"),
	MAX_DSTR UMETA(Hidden)
};
struct Weather {
	float rain = 0; // ideal rainfall modifier
	float temp = 0; // ideal tmp modifier
	bool disaster[Disasters::MAX_DSTR]; // did a disaster occur
};
UENUM(BlueprintType)
enum Biomes {
	tforest UMETA(DisplayName = "Tropical Rainforest"),
	savannah UMETA(DisplayName = "Savannah"),
	desert UMETA(DisplayName = "Desert"),
	chapparral UMETA(DisplayName = "Chaparral"),
	dforest UMETA(DisplayName = "Temperate Broadleaf Forest"),
	cforest UMETA(DisplayName = "Coniferous Forest"),
	tundra UMETA(DisplayName = "Tundra"),
	mountain UMETA(DisplayName = "Alpine Tundra"),
	polar UMETA(DisplayName = "Polar Ice"),
	steppe UMETA(DisplayName = "Temperate Grassland"),
	MAX_BIOM UMETA(Hidden)
};
struct Biome {
	Biomes name;
	float supply; // base supply
	FRichCurve freq; // frequency by latitute -100 to 100
	bool coastal; // whether or not this biome has access to large bodies of water
	float disaster[Disasters::MAX_DSTR]; // chances for natural disasters
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
	//TArray<People> p;
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
	Location getLoc(int &loc);
	void getNeighbors(int &loc, TArray<Neighbor>& n);
private:
	Biome biomes[Biomes::MAX_BIOM]; // list of possible biomes
	const int bsRt; // base years to recover from ecological collapse (if supply is 0 in a lcation it will take bsRt years to get back to base)
	TArray<Location> l; // dynamically allocatable array
	WorldInfo info; // info from l
	Population p; // population of ppls
	void asgnBiomes();
	Biomes getBiome(int& lat) const; // returns a biome based on the probability of occurring at a given latitue
};
