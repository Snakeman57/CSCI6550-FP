// copyright 2021

#include "TheWorld.h"
#include <random>

TheWorld::TheWorld() : cID(0) { // default numbers, DO NOT CALL
	init(-100, 100, 100, 20);
}
TheWorld::TheWorld(int lat1, int lat2, int size, int pop) : cID(0) { // actual cxn
	init(lat1, lat2, size, pop);
}
TheWorld::~TheWorld() { // dxn
		delete l; // deallocate the map - check if need to do neighbors seperately or if this is enough
}
void TheWorld::init(int lat1, int lat2, int size, int pop) {
	info.locations = size;
	info.lat1 = lat1;
	info.lat2 = lat2;
	l = new Location[size]; // dynamically allocate the map from user specs
	int * lats = new int[size]; // allocate temporary array
	std::default_random_engine rngsus;
	std::normal_distribution<double> bell((double)((lat1 + lat2) / 2), (double)((lat1 - lat2) / 6/*8*/));
	for (int i = 0; i < size; i++) { // fills latitude list with a normal distribution within range
		lats[i] = /*std::clamp(*/(int)bell(rngsus), lat1, lat2/*)*/; // FIX THIS
	}
	//std::sort(*lats, *lats + size); // FIX THIS
	for (int i = 0; i < size; i++) { // generate the map
		l[i].biome = getBiome(lats[i]); // assign biome from latitude chances
		info.biomes[l[i].biome]++;
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
	for (cID = 0; cID < pop; cID++) { // populate the world
		People a = People(cID, size);
		p.in(a);
	}
}
WorldInfo TheWorld::getWstats() {
	return info;
}
PopInfo TheWorld::getPstats() {
	PopInfo a;
	a.extant = p.length();
	a.extinct = cID - a.extant;
	a.avg = p.avgT();
	return a;
}
Biomes TheWorld::getBiome(int lat) {
	return MAX_BIOM; // FIX THIS
}