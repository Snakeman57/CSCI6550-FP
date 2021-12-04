// Copyright 2021 Matthew Sentell

#include "People.h"

People::People(int inId, int locs) : id(inId), pop(165), supply(165), loc(locs) {
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
	move(w); // where to move // desire to move to or aviod other people, desire to stay with or leave other poeole, 
	interact(w); // what interaction to make
	getSupply(w); // what to focus on: hunt (big or small game), gather, fish
	eat(); // pop + trait mods
	reproduce(w); // avg birth/death ratio
	split(w); // 150 - 200
	adjTraits(); // small random adjustments
}
void People::eat() {
	supply -= pop * (traits[Traits::glut] * 1.5 + 0.5);
	if (supply < 0){
		pop += supply * (traits[Traits::glut] * 1.5 + 0.5); // starvation
		supply = 0;
	}
}
void People::adjTraits() {
	for (auto& a : traits) {
		if ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::adapt] * 0.9 + 0.1)) // chance to change a trait
			a += ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2 - 1) * (traits[Traits::delta] * 1.5 + 0.5); // amt to change a trait
		a = (a > 1) ? 1 : ((a < 0) ? 0 : a); // clamp trait 0 < x < 1
	}
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
void People::trade(People& other) {
	if (pop < other.getP() / 10 || other.getP() < pop / 10) {
		merge(other, false);
	}
	else {

	}
}
void People::merge(People& other, bool merged) {
	if (pop > other.getP()) {
		for (int i = 0; i < Traits::MAX_TRAIT; i++)
			traits[i] = traits[i] * pop / (pop + other.getP()) + other.getT(i) * other.getP() / (pop + other.getP());
		pop += other.getP();
		supply += other.getS();
		if (!merged)
			other.merge(*this, true);
	}
	else {
		if (!merged)
			other.merge(*this, true);
		pop = 0;
	}
}
void People::war(People& other) {
	if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < (pop * supply * traits[Traits::warring]) / (pop * supply * traits[Traits::warring] + other.getP() * other.getS() * other.getT(Traits::warring)))
		win(other);
	else
		other.win(*this);
}
void People::win(People& other) {
	pop -= pop / 10;
	float tmp = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (1.f + traits[Traits::warrior] + traits[Traits::trader]);
	char type;
	if (tmp < 1)
		type = '0';
	else {
		tmp -= 1;
		if (tmp < traits[Traits::trader])
			type = '1';
		else
			type = '2';
	}
	switch (type) {
	case'0':
		supply += other.getS() / 2;
		other.lose(type);
		break;
	case'1':
		merge(other, false);
		break;
	case'2':
		supply += other.getS();
		other.lose(type);
	}
}
void People::lose(char& type) {
	pop /= 2;
	switch (type) {
	case'0':
		supply /= 2;
		break;
	case'2':
		pop = 0;
	}
}