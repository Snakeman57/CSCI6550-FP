// Copyright 2021 Matthew Sentell

#include "People.h"

People::People(int inId, int locs) : id(inId), pop(165), supply(165), loc(rand() % locs) {
	for (auto& i : traits)
		i = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
People::People(int inId, People& ppl) : id(inId), pop(ppl.getP()), supply(ppl.getS()), loc(ppl.getLoc()) {
	for (int i = 0; i < Traits::MAX_TRAIT; i++)
		traits[i] = ppl.getT(i);
}
People::~People() {
}
void People::tick(TheWorld& w) { // does one tick of the sim
	if(pop > 0)
		move(w); // where to move // desire to move to or aviod other people, desire to stay with or leave other peole, 
	if(pop > 0)
		interact(w); // what interaction to make
	if(pop > 0)
		getSupply(w); // what to focus on: hunt hig risk high reward or low risk low reward, gather, fish
	if(pop > 0)
		eat(w); // pop * energy use
	reproduce(w); // logistic fxn: rP(1-(P/K)); r = possible pregnancies per year * proportion of pop who can give birth * (1 - IMR);
	if(pop > 0)
		split(w); // 152 - 181 size is normal
	if (pop > 0)
		adjTraits(); // small random adjustments
}
void People::traded(float s, float t[Traits::MAX_TRAIT]) {
	supply -= s; // supply exchange
	for (int i = 0; i < Traits::MAX_TRAIT; i++)
		if ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::adapt] * 0.9 + 0.1)) // chance to adopt trait
			traits[i] += t[i]; // adopt by provided amount
}
int People::getID() const { // public access to id
	return id;
}
int People::getLoc() const { // public access to location
	return loc;
}
float People::getT(Traits t) const { // public access to trait from trait
	return traits[t];
}
float People::getT(int t) const { // public access to trait from int
	return traits[t];
}
float People::getS() const { // public access to supply
	return supply;
}
int People::getP() const { // public access to population
	return pop;
}
bool People::operator == (People& other) const {
	return id == other.getID();
}
void People::adjTraits() {
	for (auto& a : traits) {
		if ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::adapt] * 0.9 + 0.1)) // chance to change a trait
			a += ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1) * (traits[Traits::delta] * 1.5 + 0.5); // amt to change a trait
		a = (a > 1) ? 1 : ((a < 0) ? 0 : a); // clamp trait 0 < x < 1
	}
}