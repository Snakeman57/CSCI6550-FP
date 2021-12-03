// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

class TheWorld;
struct Neighbor;
UENUM(BlueprintType)
enum Traits {
	glut UMETA(DisplayName = "Energy Use"), // 1 = more food aquired and eaten
	birthRt UMETA(DisplayName = "Birth Rate"),
	deathRt UMETA(DisplayName = "Death Rate"),
	adapt UMETA(DisplayName = "Adaptability"),
	delta UMETA(DisplayName = "Malleability"),
	hunt UMETA(DisplayName = "Hunting Focus"),
	game UMETA(DisplayName = "Preferred Game Size"),
	gather UMETA(DisplayName = "Gathering Focus"),
	fish UMETA(DisplayName = "Fishing Focus"),
	sedentary UMETA(DisplayName = "Sedentariness"),
	gregarious UMETA(DisplayName = "Preference to be around other peoples"),
	trader UMETA(DisplayName = "Trade Intent"),
	trading UMETA(DisplayName = "Trade Efficacy"),
	warrior UMETA(DisplayName = "War Intent"),
	warring UMETA(DisplayName = "War Efficacy"),
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
