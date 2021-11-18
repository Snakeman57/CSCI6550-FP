// copyright 2021

#include "People.h"

People::People(int inId, int locs) : id(inId), pop(200), supply(200), loc(rand() % locs) {
	for (auto& i : traits)
		i = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
People::People(int inId, People& ppl) : id(inId), pop(ppl.getP()), supply(ppl.getS()), loc(ppl.getLoc()) {
	for (int i = 0; i < Traits::MAX_TRAIT; i++)
		traits[i] = ppl.getT(i);
}
People::~People() {
}
void People::tick(TheWorld& w) { // does one tick of the sim
	move(w); // where to move // desire to move to or aviod other people, desire to stay with or leave other poeole, 
	//interact(w); // what interaction to make
	//getSupply(w); // what to focus on: hunt (big or small game), gather, fish
	eat(); // pop + trait mods
	reproduce(); // avg birth/death ratio
	//split(w); // 150 - 200
	adjTraits(); // small random adjustments
}
void People::eat() {
	supply -= pop * (traits[Traits::glut] + 0.5);
}
void People::reproduce() {
	pop += pop + traits[Traits::birthRt] - traits[Traits::deathRt];
}
void People::adjTraits() {
	for (auto& a : traits) {
		if (((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < Traits::adapt))
			a += ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5) * (Traits::delta + 0.5);
		a = (a > 1) ? 1 : ((a < 0) ? 0 : a);
	}
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
int People::getP() const {
	return pop;
}
bool People::operator == (People& other) const {
	return id == other.getID();
}
