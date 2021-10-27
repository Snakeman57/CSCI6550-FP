// copyright 2021

#include "People.h"

People::People(int inId, int locs) : id(inId), loc(rand() % locs), supply(0.f) {
	for(auto& i : traits)
		i = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
People::People(int inId, People& ppl) : id(inId), loc(ppl.getLoc()), supply(ppl.getS()) {
	for (int i = 0; i < Traits::MAX_TRAIT; i++)
		traits[i] = ppl.getT(i);
}
People::~People() {
}
void People::tick() { // does one tick of the sim
	//move(); // where to move // desire to move to or aviod other people, desire to stay with or leave other poeole, 
	//interact(); // what interaction to make
	//getSupply(); // what to focus on: hunt (big or small game), gather, fish
	//eat(); // pop + trait mods
	//reproduce(); // avg birth/death ratio
	//split(); // 150 - 200
	//adjTraits(); // small random adjustments
}
int People::getID() const {
	return id;
}
int People::getLoc() const {
	return loc;
}
float People::getT(Traits t) const {
	return traits[t];
}
float People::getT(int t) const {
	return traits[t];
}
float People::getS() const {
	return supply;
}
bool People::operator == (People& other) const {
	return id == other.getID();
}
