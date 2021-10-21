// copyright 2021

#include "People.h"

Traits Traits::operator += (Traits& other) {
	return other;
}
Traits Traits::operator /= (int& div) {
	return *this;
}

People::People() : id(0) {
}
People::People(int inId, int locs) : id(inId), loc(rand() % locs) {
}
People::~People() {
}
void People::act() {
	//move();
	//interact();
	//getSupply();
	//eat();
	//reproduce();
	//split();
	//adjTraits();
}
int People::getID() const {
	return id;
}
int People::getLoc() const {
	return loc;
}
Traits People::getT() const {
	return traits;
}
bool People::operator == (People& other) const {
	return id == other.getID();
}
