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
	avgSupplyW = other.avgSupply;
	return *this;
}
FSimStats FSimStats::operator =(PopInfo& other) {
	for (int i = 0; i < Traits::MAX_TRAIT; i++) {
		traits.Add(other.avg[i]);
	}
	extant = other.extant;
	extinct = other.extinct;
	avgSupplyP = other.avgSupply;
	return *this;
}
UDisplayMainC::UDisplayMainC(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	FString tmp = FPaths::GameSourceDir() + " #";
}
UDisplayMainC::~UDisplayMainC() {
	delete world;
}
void UDisplayMainC::newSim(const int& lat1, const int& lat2, const int& size, const int& pop) {
	delete world;
	world = new TheWorld;
	world->init(lat1, lat2, size, pop);
}
void UDisplayMainC::runSim() {
	world->tick();
}
FSimStats UDisplayMainC::getStats() {
	FSimStats a;
	WorldInfo w = world->getWstats();
	PopInfo p = world->getPstats();
	a = w;
	a = p;
	return a;
}