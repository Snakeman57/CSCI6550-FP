// copyright 2021

#include "People.h"

Traits Traits::operator += (Traits& other) {
	return other;
}
Traits Traits::operator /= (const int& div) {
	return *this;
}

People::People() : id(0) {
}
People::People(int inId, int locs) : id(inId), loc(rand() % locs) {
}
People::People(int inId, People& ppl) : id(inId), loc(ppl.getLoc()) {
	traits = ppl.getT();
}
People::~People() {
}
void People::tick() { // does one tick of the sim
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
float People::getS() const {
	return supply;
}
bool People::operator == (People& other) const {
	return id == other.getID();
}
