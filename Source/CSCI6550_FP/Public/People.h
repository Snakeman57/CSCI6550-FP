// Copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

class TheWorld;
struct Neighbor;
UENUM(BlueprintType)
enum Traits {
	glut UMETA(DisplayName = "Energy Use"), // 1 = more food aquired and eaten
	growthRt UMETA(DisplayName = "Growth Rate"), // 1 = faster growth
	adapt UMETA(DisplayName = "Adaptability"), // 1 = more likely to change traits
	delta UMETA(DisplayName = "Malleability"), // 1 = greater amt of change to traits
	hunt UMETA(DisplayName = "Hunting Focus"), // 1 = more pop sent hunting
	game UMETA(DisplayName = "Preferred Game Size"), // 1 = more likely to attempt big game
	gather UMETA(DisplayName = "Gathering Focus"), // 1 = more pop sent gathering
	fish UMETA(DisplayName = "Fishing Focus"), // 1 = more pop sent fishing
	sedentary UMETA(DisplayName = "Sedentariness"), // 1 = more weight to distance in movement heuristic
	gregarious UMETA(DisplayName = "Interpersonal Interaction"), // 1 = more weight to location pop in mvmt heuristic & more likely to interact
	trader UMETA(DisplayName = "Trade Intent"), // 1 = more likely to do friendly interaction
	trading UMETA(DisplayName = "Trade Efficacy"), // 1 = better trade outcomes
	warrior UMETA(DisplayName = "War Intent"), // 1 = more likely to do unfriendly interaction
	warring UMETA(DisplayName = "War Efficacy"), // 1 = better war outcomes
	group UMETA(DisplayName = "Desired Group Size"),
	MAX_TRAIT UMETA(Hidden)
};
class CSCI6550_FP_API People {
public:
	People(int inId, int locs);
	People(int inId, People& ppl);
	~People();
	void tick(TheWorld& w); // defined in TheWorld.cpp
	void merge(People& other, bool merged);
	void win(People& other);
	void lose(char& type);
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
	void trade(People& other);
	void war(People& other); // what does the victor do: release, enslave, slaughter
	void getSupply(TheWorld& w);
	void eat();
	void reproduce(TheWorld& w);
	void split(TheWorld& w);
	void adjTraits();
};
