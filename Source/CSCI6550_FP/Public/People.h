// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "People.generated.h"

struct Traits {
	float a; // 
};
UCLASS()
class CSCI6550_FP_API UPeople : public UObject {
	GENERATED_BODY()
public:
	UPeople();
	UPeople(int inId, int locs);
	~UPeople();
	void set(UPeople& p); // set the UPeople b/c compiler bitched about "UPopulation.head->ppl = item;"
	void act();
	int getID() const;
	int getLoc() const;
	bool operator == (UPeople& other) const;
private:
	int id; // unique id for searches
	int pop; // integer representing population
	Traits traits; // the traits for this UPeople (see struct above)
	float supply; 
	int loc; // location index
	void move();
	void interact();
	void trade(UPeople *&other);
	void war(UPeople *&other);
	void merge(UPeople*& other);
	void getSuUPeopley();
	void eat();
	void reproduce();
	void split();
	void adjTraits();
};
