// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayMainC.h"

FSimStats FSimStats::operator =(WorldInfo& other) {
	locations = other.locations;
	lat1 = other.lat1;
	lat2 = other.lat2;
	biomes.Empty();
	for (int i = 0; i < Biomes::MAX_BIOM; i++) {
		biomes.Add(other.biomes[i]);
	}
	avgSupply = other.avgSupply;
	return *this;
}
FSimStats FSimStats::operator =(PopInfo& other) {
	//avg = other.avg;
	extant = other.extant;
	extinct = other.extinct;
	return *this;
}
UDisplayMainC::~UDisplayMainC() {
	if (world != nullptr) delete world;
}
void UDisplayMainC::newSim(int lat1, int lat2, int size, int pop) {
	world = new TheWorld(lat1, lat2, size, pop);
}
FSimStats UDisplayMainC::getStats() {
	FSimStats a;
	WorldInfo w = world->getWstats();
	PopInfo p = world->getPstats();
	a = w;
	a = p;
	return a;
}