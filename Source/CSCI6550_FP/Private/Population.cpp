// Copyright 2021 Matthew Sentell


#include "Population.h"

Population::Population() : cID(0) { // cxn
}
Population::~Population() { // dxn
}
int Population::length() const { // public access to ppls.Num()
	return ppls.Num();
}
PopInfo Population::stats() const { // statisitcal info
	PopInfo tmp;
	tmp.extant = length(); // assign number of extant Peoples
	tmp.extinct = cID - length(); // assign number of extinct Peoples
	float a[Traits::MAX_TRAIT]; // temp var for trait averages
	for (auto& i : a) // initialize all values to 0
		i = 0;
	float s = 0; // temp var for supply average
	int p = 0;
	for (auto& i : ppls) {
		for (int j = 0; j < Traits::MAX_TRAIT; j++) { // get total values for each trait
			float b = i.Value.getT(j);
			a[j] += b;
		}
		s += i.Value.getS(); // get total supply
		p += i.Value.getP();
	}
	if (length() > 0) {
		for (auto& i : a) // average trait totals
			i /= length();
		s /= length(); // average supply total
		p /= length();
	}
	for (int i = 0; i < Traits::MAX_TRAIT; i++) // assign trait averages
		tmp.avg[i] = a[i];
	tmp.avgSupply = s; // assign average supply
	tmp.avgPop = p; // assign average population
	return tmp;
}
People Population::get(int &ppl) {
	return ppls[ppl];
}