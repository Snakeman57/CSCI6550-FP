// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "Population.h"
#include "TheWorld.generated.h"

enum Biomes {
	biome1,
	MAX_BIOM
};
struct Biome {
	Biomes name;
	float supply; // base supply
	int numPt; // amount of array below to use for evaluation the bezier curve
	FVector chance[10]; // chance (0-1) of occurring at a given |latitude| (0-100) // vectors make up bezier curve with no more than 10 points
};
enum Weathers {
	wthr1,
	MAX_WTHR
};
struct Weather {
	float chance[Biomes::MAX_BIOM]; // chance of occurring in each biome
	float effect; // effect on supply
};
struct Neighbor {
	int loc; // index of location being referenced
	int dist; // difficulty to get to location
};
struct Location {
	Neighbor* nbrs; // neighbors
	Biomes biome; // biome
	Weathers weather; // weather
	float supply; // supply
};
UCLASS()
class CSCI6550_FP_API UTheWorld : public UObject {
	GENERATED_BODY()
public:
	UTheWorld();
	UTheWorld(int lat1, int lat2, int size, int pop);
	~UTheWorld();
	void init(int lat1, int lat2, int size, int pop);
	void updtSupply(); // moves supply in each location towards base
	void updtWeather(); // changes the weather in each location based on Weather.chance
private:
	Biome biomes[Biomes::MAX_BIOM];
	Weather weathers[Weathers::MAX_WTHR];
	Location* l; // dynamically allocate an array
	UPopulation p; // population of ppls
	Biomes getBiome(int lat);
};
