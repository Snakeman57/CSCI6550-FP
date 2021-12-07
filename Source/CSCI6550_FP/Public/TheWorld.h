// Copyright 2021 Matthew Sentell

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
enum Disasters { // 
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
	float disaster[Disasters::MAX_DSTR]; // chances for natural disasters
	float aniplant; // ainmal to plant ratio
};
struct Neighbor {
	int loc; // index of location being referenced
	int dist; // difficulty to get to location
};
struct Location {
	TArray<Neighbor> nbrs; // neighbors
	Biomes biome; // biome
	bool coastal; // whether or not this biome has access to large bodies of water
	Weather weather; // weather
	float supply; // supply
	TArray<int> pop; // local population
	int64 lclPop; // collected local pop of all Peoples
};
struct WorldInfo {
	int locations; // number of locations
	int lat1, lat2; // lattitude range
	int biomes[Biomes::MAX_BIOM]; // number of each biomme
	float pops[Biomes::MAX_BIOM]; // percentage population per biome
	float supplies[Biomes::MAX_BIOM]; // average supply per biome
	float avgSupply; // average supply value
	int64 avgPopB[Biomes::MAX_BIOM]; // average People population value per biome
	float avgSupplyB[Biomes::MAX_BIOM]; // average People supply value per biome
	int actions[Action::MAX_ACT];
	int coastalPpls;
};
class CSCI6550_FP_API TheWorld {
public:
	TheWorld();
	~TheWorld();
	void init(const int& lat1, const int& lat2, const int& size, const int& pop);
	int isSafe();
	int setSafe(int s = 10000);
	void tick(); // does one tick of the sim
	void updtSupply(); // moves supply in each location towards base
	void updtWeather(); // changes the weather in each location
	void refreshStats();
	WorldInfo getWstats() const;
	PopInfo getPstats() const;
	Location getLoc(int& loc);
	People getPpl(int& ppl);
	int getPpls();
	float getLocBase(int& loc);
	float getAP(int& loc);
	void getNeighbors(int& loc, TArray<Neighbor>& n);
	void kill(int id);
	void exploit(int loc, float s);
	void move(int id, int loc1, int loc2);
	void clone(People& ppl);
	void interact(Action type);
private:
	Biome biomes[Biomes::MAX_BIOM]; // list of possible biomes
	TArray<Location> l; // dynamically allocatable array
	WorldInfo info; // info from l
	Population p; // population of ppls
	void asgnBiomes();
	Biomes getBiome(int& lat) const; // returns a biome based on the probability of occurring at a given latitue
	int safe;
};
