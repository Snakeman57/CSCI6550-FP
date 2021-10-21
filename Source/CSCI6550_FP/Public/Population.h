// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "People.h"
#include "Population.generated.h"

struct FPopNode {
	UPeople ppl;
	FPopNode* nxt;
};
UCLASS()
class CSCI6550_FP_API UPopulation : public UObject {
	GENERATED_BODY()
public:
	UPopulation();
	~UPopulation();
	void in(UPeople& item); // insert
	void del(UPeople& item); // delete
	int length() const; // public access to length
	void randomOrder(); // randomizes the order of the list so ppl don't have bis for being early/late in it
private:
	FPopNode* head; // first pop
	int ln; // length
	void findItem(FPopNode*& ploc, FPopNode*& loc, bool& found, UPeople &item);
	void swap(FPopNode*& a, FPopNode*& b);
};
