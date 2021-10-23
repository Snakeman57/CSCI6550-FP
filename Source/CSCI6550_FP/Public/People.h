// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

struct Traits {
	float a; // 
	Traits operator += (Traits& other);
	Traits operator /= (const int& div);
};
class CSCI6550_FP_API People {
public:
	People();
	People(int inId, int locs);
	People(int inId, People& ppl);
	~People();
	void tick();
	int getID() const;
	int getLoc() const;
	Traits getT() const;
	float getS() const;
	bool operator == (People& other) const;
private:
	int id; // unique id for searches
	int pop; // integer representing population
	Traits traits; // the traits for this People (see struct above)
	float supply; 
	int loc; // location index
	void move();
	void interact();
	void trade(People *&other);
	void war(People *&other);
	void merge(People*& other);
	void getSupply();
	void eat();
	void reproduce();
	void split();
	void adjTraits();
};
