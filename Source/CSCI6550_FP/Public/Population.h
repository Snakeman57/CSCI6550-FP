// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "People.h"


struct PopInfo {
	int extant;
	int extinct;
	float avg[Traits::MAX_TRAIT];
	float avgSupply;
};
class CSCI6550_FP_API Population {
public:
	Population();
	~Population();
	void del(int id); // delete
	void addNew(int size); // generate & insert
	void clone(People& ppl); // copy & insert
	int length() const; // public access to length
	void tick();
	PopInfo stats() const; // average trait vaules across population
private:
	TMap<int, People> ppls;
	int cID; // current uniquie id value to apply to People creation
};
