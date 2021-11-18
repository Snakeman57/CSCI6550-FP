// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

class TheWorld;
struct Neighbor;
UENUM(BlueprintType)
enum Traits {
	glut UMETA(DisplayName = "Gluttony"),
	birthRt UMETA(DisplayName = "Birth Rate"),
	deathRt UMETA(DisplayName = "Death Rate"),
	adapt UMETA(DisplayName = "Adaptability"),
	delta UMETA(DisplayName = "Malleability"),
	MAX_TRAIT UMETA(Hidden)
};
class CSCI6550_FP_API People {
public:
	People(int inId, int locs);
	People(int inId, People& ppl);
	~People();
	void tick(TheWorld& w); // defined in TheWorld.cpp
	int getID() const;
	int getLoc() const;
	float getT(Traits t) const;
	float getT(int t) const;
	float getS() const;
	int getP() const;
	bool operator == (People& other) const;
private:
	int id; // unique id for searches
	int pop; // integer representing population
	float traits[Traits::MAX_TRAIT]; // the traits for this People
	float supply; 
	int loc; // location index
	void move(TheWorld& w); // defined in TheWorld.cpp
	void interact(TheWorld& w);
	void trade(People *&other);
	void war(People *&other); // what does the victor do: release, enslave, slaughter
	void merge(People*& other);
	void getSupply(TheWorld& w);
	void eat();
	void reproduce();
	void split(TheWorld& w);
	void adjTraits();
};
