// Copyright 2021 Matthew Sentell

#include "DisplayMainC.h"

FSimStats FSimStats::operator =(WorldInfo& other) { // assign world stats from struct
	locations = other.locations;
	lat1 = other.lat1;
	lat2 = other.lat2;
	biomes.Empty();
	for (int i = 0; i < Biomes::MAX_BIOM; i++) {
		biomes.Add(other.biomes[i]);
		pops.Add(other.pops[i]);
		supplies.Add(other.supplies[i]);
		avgPopB.Add(other.avgPopB[i]);
		avgSupplyB.Add(other.avgSupplyB[i]);
	}
	avgSupplyW = other.avgSupply;
	coastPpls = other.coastalPpls;
	for (int i = 0; i < Action::MAX_ACT; i++)
		actions.Add(other.actions[i]);
	return *this;
}
FSimStats FSimStats::operator =(PopInfo& other) { // assign population stats from struct
	for (int i = 0; i < Traits::MAX_TRAIT; i++) {
		traits.Add(other.avg[i]);
	}
	extant = other.extant;
	extinct = other.extinct;
	avgSupplyP = other.avgSupply;
	avgPop = other.avgPop;
	return *this;
}
UDisplayMainC::UDisplayMainC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), safety(10000) {
}
UDisplayMainC::~UDisplayMainC() {
	delete world;
}
void UDisplayMainC::newSim(const int& lat1, const int& lat2, const int& size, const int& pop) { // create new instance of the simulation
	delete world;
	world = new TheWorld;
	world->init(lat1, lat2, size, pop);
	world->setSafe(safety);
}
void UDisplayMainC::runSim() { // call from tick to run one cycle of the simulation
	world->tick();
}
FSimStats UDisplayMainC::getStats() { // collect and return current simulation stats
	FSimStats a;
	world->refreshStats();
	WorldInfo w = world->getWstats();
	PopInfo p = world->getPstats();
	a = w;
	a = p;
	return a;
}
void UDisplayMainC::safe(int s) {
	safety = s;
}