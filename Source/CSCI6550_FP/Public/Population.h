// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "People.h"


struct PopInfo {
	int extant;
	int extinct;
	float avg[Traits::MAX_TRAIT];
	float avgSupply;
	int avgPop;
};
class CSCI6550_FP_API Population {
public:
	Population();
	~Population();
	void del(int id, TheWorld& w); // delete
	void addNew(TheWorld& w); // generate & insert
	void clone(People& ppl, TheWorld& w); // copy & insert
	int length() const; // public access to length
	void tick(TheWorld& w); // defined in TheWorld.cpp
	PopInfo stats() const; // average trait vaules across population
	People get(int &ppl); // return a specific person
private:
	TMap<int, People> ppls;
	int cID; // current uniquie id value to apply to People creation
};
