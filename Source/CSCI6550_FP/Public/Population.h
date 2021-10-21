// copyright 2021 Matthew Sentell

#pragma once

#include "CoreMinimal.h"
#include "People.h"

struct Node {
	People ppl;
	Node* nxt;
};
class CSCI6550_FP_API Population {
public:
	Population();
	~Population();
	void in(People& item); // insert
	void del(People& item); // delete
	int length() const; // public access to length
	void randomOrder(); // randomizes the order of the list so ppl don't have bis for being early/late in it
	Traits avgT(); // average trait vaules across population
private:
	Node* head; // first pop
	int ln; // length
	void findItem(Node*& ploc, Node*& loc, bool& found, People &item);
	void swap(Node*& a, Node*& b);
};
