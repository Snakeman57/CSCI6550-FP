// Copyright 2021 Matthew Sentell


#include "Population.h"

Population::Population() : cID(0) { // cxn
}
Population::~Population() { // dxn
}
void Population::addNew(int size) { // generate & insert
	People ppl = People(cID, size); // generate
	ppls.Add(cID, ppl);
	cID++; // update cID
}
void Population::clone(People& ppl) { // generate & insert
	People cloned = People(cID, ppl); // generate
	ppls.Add(cID, cloned);
	cID++; // update cID
}
void Population::del(int id) { // delete
	ppls.Remove(id);

}
int Population::length() const { // public access to ppls.Num()
	return ppls.Num();
}
void Population::tick() { // does one tick of the sim
	int* order = new int[length()];
	TArray<int> keys;
	ppls.GetKeys(keys);
	for (int i = 0; i < length(); i++) {
		int key = rand() % keys.Num();
		order[i] = keys[key];
		keys.Remove(keys[key]);
	}
	for (int i = 0; i < length(); i++) {
		ppls[i].tick();
	}
}
PopInfo Population::stats() const {
	PopInfo tmp;
	tmp.extant = length();
	tmp.extinct = cID - length();
	Traits a;
	float s = 0;
	for (auto& i : ppls) {
		Traits b = i.Value.getT();
		a += b;
		s += i.Value.getS();
	}
	a /= length();
	tmp.avg = a;
	return tmp;
}