// copyright 2021

#include "TheWorld.h"
#include <random>

UTheWorld::UTheWorld() { // default numbers, DO NOT CALL
	init(-100, 100, 100, 20);
}
UTheWorld::UTheWorld(int lat1, int lat2, int size, int pop) { // actual cxn
	init(lat1, lat2, size, pop);
}
UTheWorld::~UTheWorld() { // dxn
		delete l; // deallocate the map - check if need to do neighbors seperately or if this is enough
}
void UTheWorld::init(int lat1, int lat2, int size, int pop) {
	l = new Location[size]; // synamically allocate the map from user specs
	int * lats = new int[size]; // allocate temporary array
	std::default_random_engine rngsus;
	std::normal_distribution<double> bell((double)((lat1 + lat2) / 2), (double)((lat1 - lat2) / 6/*8*/));
	for (int i = 0; i < size; i++) { // fills latitude list with a normal distribution within range
		lats[i] = /*std::clamp(*/(int)bell(rngsus), lat1, lat2/*)*/; // FIX THIS
	}
	//std::sort(*lats, *lats + size); // FIX THIS
	for (int i = 0; i < size; i++) { // generate the map
		l[i].biome = getBiome(lats[i]); // assign biome from latitude chances
		l[i].supply = biomes[l[i].biome].supply; // assign base supply
		int a = rand() % 6 + 1; // number of neighbors
		l[i].nbrs = new Neighbor[(a <= size) ? a : size]; // contain if necessary
		for (int j = 0; j < sizeof(l[i].nbrs); j++) { // generate one-way neighbor paths that are somewhat likely to overlap
			int b = ((rand() % 2) > 0) ? ((i - j >= 0) ? i - j : i + j) : ((i + j < size) ? i + j : i - j); // generate neighbor index
			l[i].nbrs[j].loc = b; // assign generated index
			l[i].nbrs[j].dist = (abs(lats[b] - lats[i]) + 1) * (rand() % 366); // difficulty to reach location in days
		}
	}
	delete lats; // deallocate no longer needed array
}
Biomes UTheWorld::getBiome(int lat) {
	return MAX_BIOM; // FIX THIS
}