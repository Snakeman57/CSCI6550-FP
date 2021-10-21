// copyright 2021

#include "People.h"

UPeople::UPeople() : id(0) {
}
UPeople::UPeople(int inId, int locs) : id(inId), loc(rand() % locs) {
}
UPeople::~UPeople() {
}
void UPeople::set(UPeople& p ) {
	id = p.getID();
	loc = p.getLoc();
}
void UPeople::act() {
	//move();
	//interact();
	//getSupply();
	//eat();
	//reproduce();
	//split();
	//adjTraits();
}
int UPeople::getID() const {
	return id;
}
int UPeople::getLoc() const {
	return loc;
}
bool UPeople::operator == (UPeople& other) const {
	return id == other.getID();
}
