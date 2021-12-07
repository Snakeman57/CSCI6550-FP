// Copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

class TheWorld;
struct Neighbor;
UENUM(BlueprintType)
enum Traits {
	adapt UMETA(DisplayName = "Adaptability"), // 1 = more likely to change traits
	delta UMETA(DisplayName = "Malleability"), // 1 = greater amt of change to traits
	glut UMETA(DisplayName = "Energy Use"), // 1 = more food aquired and eaten
	growthRt UMETA(DisplayName = "Growth Rate"), // 1 = faster growth
	sedentary UMETA(DisplayName = "Sedentariness"), // 1 = more weight to distance in movement heuristic
	group UMETA(DisplayName = "Desired Group Size"), // 1 = larger group before splitting
	split UMETA(DisplayName = "Desired Colonist Size"), // 1 = how muc to split
	store UMETA(DisplayName = "Food Storage"), // 1 = store more food on hand
	hunt UMETA(DisplayName = "Hunting Focus"), // 1 = more pop sent hunting
	game UMETA(DisplayName = "Preferred Game Size"), // 1 = more likely to attempt big game
	gather UMETA(DisplayName = "Gathering Focus"), // 1 = more pop sent gathering
	fish UMETA(DisplayName = "Fishing Focus"), // 1 = more pop sent fishing
	gregarious UMETA(DisplayName = "Interpersonal Interaction"), // 1 = more weight to location pop in mvmt heuristic & more likely to interact
	trader UMETA(DisplayName = "Trade Intent"), // 1 = more likely to do friendly interaction
	trading UMETA(DisplayName = "Trade Efficacy"), // 1 = better trade outcomes
	warrior UMETA(DisplayName = "War Intent"), // 1 = more likely to do unfriendly interaction
	warring UMETA(DisplayName = "War Efficacy"), // 1 = better war outcomes
	MAX_TRAIT UMETA(Hidden)
};
UENUM(BlueprintType)
enum Action {
	war UMETA(DisplayName = "Wars"),
	win UMETA(DisplayName = "Victorious Agressors"),
	trade UMETA(DisplayName = "Trades"),
	merge UMETA(DisplayName = "Merges"),
	move UMETA(DisplayName = "Moves"),
	splits UMETA(DisplayName = "Splits"),
	hunts UMETA(DisplayName = "Hunting Attempts"),
	huntW UMETA(DisplayName = "Hunting Successes"),
	MAX_ACT UMETA(Hidden)
};
class CSCI6550_FP_API People {
public:
	People(int inId, int locs);
	People(int inId, People& ppl);
	~People();
	void tick(TheWorld& w); // defined in TheWorld.cpp
	void merge(People& other, bool merged, TheWorld& w);
	void traded(float s, float t[Traits::MAX_TRAIT]);
	void win(People& other, TheWorld& w);
	void lose(char& type, TheWorld& w);
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
	void trade(People& other, TheWorld& w);
	void war(People& other, TheWorld& w); // what does the victor do: release, enslave, slaughter
	void getSupply(TheWorld& w);
	void eat(TheWorld& w);
	void reproduce(TheWorld& w);
	void split(TheWorld& w);
	void adjTraits();
};
