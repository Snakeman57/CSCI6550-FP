// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum Traits {
	trait1,
	trait2,
	MAX_TRAIT UMETA(Hidden)
};
class CSCI6550_FP_API People {
public:
	People(int inId, int locs);
	People(int inId, People& ppl);
	~People();
	void tick();
	int getID() const;
	int getLoc() const;
	float getT(Traits t) const;
	float getT(int t) const;
	float getS() const;
	bool operator == (People& other) const;
private:
	int id; // unique id for searches
	int pop; // integer representing population
	float traits[Traits::MAX_TRAIT]; // the traits for this People
	float supply; 
	int loc; // location index
	void move();
	void interact();
	void trade(People *&other);
	void war(People *&other); // what does the victor do: release, enslave, slaughter
	void merge(People*& other);
	void getSupply();
	void eat();
	void reproduce();
	void split();
	void adjTraits();
};
