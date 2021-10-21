// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"

struct Traits {
	float a; // 
	Traits operator += (Traits& other);
	Traits operator /= (int& div);
};
class CSCI6550_FP_API People {
public:
	People();
	People(int inId, int locs);
	~People();
	void act();
	int getID() const;
	int getLoc() const;
	Traits getT() const;
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
