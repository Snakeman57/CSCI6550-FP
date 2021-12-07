// Copyright 2021 Matthew Sentell

#include "TheWorld.h"

TheWorld::TheWorld() : safe(10000) { // default numbers, MUST CALL INIT BEFORE USING CLASS
}
TheWorld::~TheWorld() { // dxn
}
void TheWorld::init(const int& lat1, const int& lat2, const int& size, const int& pop) { // practical cxn, must be called seperately
	info.locations = size;
	info.lat1 = lat1;
	info.lat2 = lat2;
	info.coastalPpls = 0;
	for (int i = 0; i < Action::MAX_ACT; i++) {
		info.actions[i] = 0;
	}
	asgnBiomes();
	int* lats = new int[size]; // allocate temporary array
		std::default_random_engine rngsus;
		std::normal_distribution<double> bell(0, 20);
	for (int i = 0; i < size; i++) { // fills latitude list with a normal distribution within range
		lats[i] = (int)bell(rngsus);
		while (lats[i] < lat1 || lats[i] > lat2) // force  the latitude in range
			lats[i] = (int)bell(rngsus);
	}
	std::sort(lats, lats + size); // organize latitudes so neighboring locations have latitude similarity
	for (int i = 0; i < size; i++) { // generate the map
		Location loc;
		loc.biome = getBiome(lats[i]); // assign biome from latitude chances
		loc.coastal = (rand() % 10 < 3) ? false : true;
		info.biomes[loc.biome]++;
		loc.supply = biomes[loc.biome].supply; // assign base supply
		loc.lclPop = 0;
		int a = rand() % 6 + 1; // number of neighbors
		a = (a <= size) ? a : size;
		for (int j = 0; j < a; j++) { // generate one-way neighbor paths that are somewhat likely to overlap
			Neighbor nbr;
			int b = ((rand() % 2) > 0) ? ((i - j >= 0) ? i - j : i + j) : ((i + j < size) ? i + j : i - j); // generate neighbor index
			nbr.loc = b; // assign generated index
			nbr.dist = (abs(lats[b] - lats[i]) + 1) * (rand() % 365 + 1); // difficulty to reach location
			loc.nbrs.Add(nbr);
		}
		l.Add(loc);
	}
	updtWeather();
	updtSupply();
	delete lats; // deallocate no longer needed array
	for (int i = 0; i < pop; i++) { // populate the world
		p.addNew(*this);
	}
}
int TheWorld::isSafe() {
	return safe;
}
int TheWorld::setSafe(int s) {
	safe = s;
	return safe;
}
void TheWorld::tick() { // does one tick of the sim
	p.tick(*this);
	updtWeather();
	updtSupply();
}
void TheWorld::updtSupply() { // updates the supply value for each location
	const int bsRt = 5; // base years to recover from ecological collapse (if supply is 0 in a lcation it will take bsRt years to get back to base)
	for (int i = 0; i < info.locations; i++) {
		for (int j = 0; j < Disasters::MAX_DSTR; j++)
			if (l[i].weather.disaster[j])
				l[i].supply *= 0.8;
		if (l[i].supply != biomes[l[i].biome].supply) // moves supply toward biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt) * (((l[i].supply < biomes[l[i].biome].supply) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
		else // randomly adjusts supply for the year from biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt / 2) * (((rand() % 2 > 0) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
	}
}
void TheWorld::updtWeather() { // updates the weather for each location
	std::default_random_engine rngsus;
	std::normal_distribution<float> bell(0, 0.1);
	for (int i = 0; i < info.locations; i++) {
		l[i].weather.rain = bell(rngsus); // sets iedal weather modifier on a bell curve
		l[i].weather.temp = bell(rngsus); // sets iedal temperature modifier on a bell curve
		for (int j = 0; j < Disasters::MAX_DSTR; j++) // sets natural disasters T/F depending on likelihood in biome
			l[i].weather.disaster[j] = (static_cast<float>(rand() / static_cast<float>(RAND_MAX)) < biomes[l[i].biome].disaster[j]);
	}
}
void TheWorld::refreshStats() { // updates expensive-to-collect stat values
	info.avgSupply = 0;
	info.coastalPpls = 0;
	for (int i = 0; i < Biomes::MAX_BIOM; i++) { // reset
		info.pops[i] = 0.f;
		info.supplies[i] = 0.f;
		info.avgPopB[i] = 0;
		info.avgSupplyB[i] = 0.f;
	}
	for (auto& i : l) { // get totals
		info.avgSupply += i.supply;
		if (i.coastal)
			info.coastalPpls += i.pop.Num();
		info.pops[i.biome] += i.pop.Num();
		info.supplies[i.biome] += i.supply;
		info.avgPopB[i.biome] += i.lclPop;
		for (auto& j : i.pop) {
			info.avgSupplyB[i.biome] += p.get(j).getS();
		}
	}
	info.avgSupply /= l.Num();
	for (int i = 0; i < Biomes::MAX_BIOM; i++) { // get averages
		if (info.biomes[i] > 0) {
			if (info.pops[i] > 0) {
				info.avgPopB[i] /= info.pops[i];
				info.avgSupplyB[i] /= info.pops[i];
			}
			info.pops[i] /= p.length();
			info.supplies[i] /= static_cast<float>(info.biomes[i]);
		}
	}
}
WorldInfo TheWorld::getWstats() const { // returns stats about the world
	return info;
}
PopInfo TheWorld::getPstats() const { // returns stats about the population
	return p.stats();
}
Location TheWorld::getLoc(int& loc) { // public access to a Location
	return l[loc];
}
People TheWorld::getPpl(int& ppl) { // public access to a People
	return p.get(ppl);
}
int TheWorld::getPpls() { // public access to a People
	return p.length();
}
float TheWorld::getLocBase(int& loc) {
	return biomes[l[loc].biome].supply;
}
float TheWorld::getAP(int& loc) {
	return biomes[l[loc].biome].aniplant;
}
void TheWorld::getNeighbors(int& loc, TArray<Neighbor>& n) {
	for (int i = 0; i < l[loc].nbrs.Num(); i++)
		n.Add(l[loc].nbrs[i]);
}
void TheWorld::kill(int id) { // remove People from world
	p.del(id, *this);
}
void TheWorld::exploit(int loc, float s) { // adjust location supply value
	l[loc].supply -= s;
}
void TheWorld::move(int id, int loc1, int loc2) { // store People in a location
	if (loc1 >= 0) { // remove from location being left
		l[loc1].lclPop -= p.get(id).getP();
		l[loc1].pop.Remove(id);
	}
	if (loc2 >= 0) { // add to location being entered
		l[loc2].pop.Add(id);
		l[loc2].lclPop += p.get(id).getP();
	}
}
void TheWorld::clone(People& ppl) { // duplicate a People (used in split)
	p.clone(ppl, *this);
}
void TheWorld::interact(Action type) { // update interaction counts
	info.actions[type]++;
}
Biomes TheWorld::getBiome(int& lat) const { // returns a biome based on the probability of occurring at a given latitue
	int poslat = abs(lat);
	float max = 0;
	float chance[Biomes::MAX_BIOM];
	float biomeID = static_cast<float> (rand()) / static_cast<float>(RAND_MAX); // random value between 0 and 1
	Biomes biome = Biomes::MAX_BIOM;
	for (int i = 0; i < Biomes::MAX_BIOM; i++) // takes the total frequency of all biomes at lat
		max += biomes[i].freq.Eval((float)poslat);
	for (int i = 0; i < Biomes::MAX_BIOM; i++) // generates chances of any given biome occurring at lat
		chance[i] = biomes[i].freq.Eval((float)poslat) / max;
	for (int i = 0; biomeID > 0 && i < Biomes::MAX_BIOM; i++) { // sets biome according to generated chances from earlier random number
		biomeID -= chance[i];
		biome = biomes[i].name;
	}
	return biome; // return chosen biome
}
void TheWorld::asgnBiomes() { // look into importing and store biome data
	const float supplyBs = 839;
	FString dir = FPaths::GameSourceDir();
	std::ifstream biomeData(std::string(TCHAR_TO_UTF8(*dir)) + "CSCI6550_FP/CSCI6550-FP-BIOMES-CSV.csv"); // import data
	std::string input; // for readinig data
	for (int i = 0; i < Biomes::MAX_BIOM; i++) { // iterate through biomes
		biomes[i].name = static_cast<Biomes>(i); // set name to enum value
		std::getline(biomeData, input, ',');
		biomes[i].supply = stof(input) * supplyBs; // set supply from first value on line
		std::getline(biomeData, input, ',');
		biomes[i].aniplant = stof(input); // set animal plant ratio
		for (int j = 0; j < Disasters::MAX_DSTR; j++) { // set disaster chances from next four values on line
			std::getline(biomeData, input, ',');
			biomes[i].disaster[i] = stof(input);
		}
		biomes[i].freq = FRichCurve(); // initialize frequency curve
		std::getline(biomeData, input, ',');
		int keys = std::stoi(input); // sixth val on line is how many keys to iterate through before next line
		for (int j = 0; j < keys; j++) { // set keys of frequency curve
			std::getline(biomeData, input, ',');
			float lat = std::stof(input);
			std::getline(biomeData, input, ',');
			float freq = std::stof(input);
			biomes[i].freq.AddKey(lat, freq);
		}
		info.biomes[i] = 0;
	}
}
// FROM POPULATION.H REQUIRING THEWORLD
void Population::addNew(TheWorld& w) { // generate & insert
	People ppl = People(cID, w.getWstats().locations); // generate
	ppls.Add(cID, ppl);
	w.move(cID, -1, ppls[cID].getLoc());
	cID++; // update cID
}
void Population::clone(People& ppl, TheWorld& w) { // generate & insert
	People cloned = People(cID, ppl); // generate
	ppls.Add(cID, cloned);
	w.move(cID, -1, ppls[cID].getLoc());
	cID++; // update cID
}
void Population::del(int& id, TheWorld& w) { // delete
	w.move(id, ppls[id].getLoc(), -1);
	ppls.Remove(id);
}
void Population::tick(TheWorld& w) { // does one tick of the sim
	int* order = new int[length()]; // tmp array to make ppls go in random order
	TArray<int> keys;
	ppls.GetKeys(keys); // collect extant ppls
	for (int i = 0; i < length(); i++) { // fill order array
		int key = rand() % keys.Num();
		order[i] = keys[key];
		TArray<int> keysTmp = keys;
		keys.Remove(keysTmp[key]);
	}
	for (int i = 0; i < length(); i++) // call tick for each ppl from order array
		if (ppls.Contains(order[i]))
			ppls[order[i]].tick(w);
	delete order;
}
//FROM PEOPLE.H REQUIRING THEWORLD
void People::merge(People& other, bool merged, TheWorld& w) {
	w.move(id, loc, -1); // prep for pop change
	if (pop > other.getP()) {
		for (int i = 0; i < Traits::MAX_TRAIT; i++)
			traits[i] = (traits[i] * pop / (pop + other.getP())) + (other.getT(i) * other.getP() / (pop + other.getP()));
		pop += other.getP();
		supply += other.getS();
		if (!merged)
			other.merge(*this, true, w);
	}
	else {
		if (!merged)
			other.merge(*this, true, w);
		pop = 0;
	}
	w.move(id, -1, loc); // update pop
}
void People::move(TheWorld& w) {
	TArray<Neighbor> n;
	w.getNeighbors(loc, n); // collect neighbors of current location
	int dstn = loc, dist = 0;
	float h = (w.getLoc(loc).coastal ? (traits[Traits::fish] + 1.f) : 1.f) * (w.getLoc(loc).supply * (traits[Traits::glut] * 1.5 + 0.5) + w.getLoc(loc).lclPop * (traits[Traits::gregarious] * 2.f - 1.f)); // best location heuristic val
	for (int i = 0; i < n.Num(); i++) { // check utility of moving to each neighbor
		if ((w.getLoc(loc).coastal ? (traits[Traits::fish] + 1.f) : 1.f) * w.getLoc(n[i].loc).supply * (traits[Traits::glut] * 1.5 + 0.5) - n[i].dist * (traits[Traits::sedentary] * 1.5 + 0.5) * (traits[Traits::store] + 1.f) + w.getLoc(loc).lclPop * (traits[Traits::gregarious] * 2 - 1) > h) {
			h = (w.getLoc(loc).coastal ? (traits[Traits::fish] + 1.f) : 1.f) * (w.getLoc(loc).coastal ? (traits[Traits::fish] + 1.f) : 1.f) * w.getLoc(n[i].loc).supply * (traits[Traits::glut] * 1.5 + 0.5) - n[i].dist * (traits[Traits::sedentary] * 1.5 + 0.5) * (traits[Traits::store] + 1.f) + w.getLoc(loc).lclPop * (traits[Traits::gregarious] * 2 - 1);
			dstn = n[i].loc; // neighbor had better heuristic, update mvmt info (comment here b/c 2 lines above already long)
			dist = n[i].dist;
		}
	}
	if (dstn != loc) {
		supply -= dist * pop * (traits[Traits::store] + 1.f); // expend resources to move
		w.move(id, loc, dstn); // correct Location pops
		w.interact(Action::move);
	}
	loc = dstn; // update Location
}
void People::interact(TheWorld& w) { // if other ppls present, consider interacting with them
	if (w.getLoc(loc).pop.Num() > 1 && static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::gregarious]) { // decide to interact
		People tmp = w.getPpl(w.getLoc(loc).pop[rand() % w.getLoc(loc).pop.Num()]); // decide with whom to interact
		while (tmp == *this) // self doesn't count
			tmp = w.getPpl(w.getLoc(loc).pop[rand() % w.getLoc(loc).pop.Num()]);
		if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::trader] / (traits[Traits::trader] + traits[Traits::warrior])) // decide if interaction is friendly
			trade(tmp, w);
		else // unfriendly interaction
			war(tmp, w);
	}
}
void People::trade(People& other, TheWorld& w) {
	if (pop < other.getP() / 10 || other.getP() < pop / 10) { // is trade even practical?
		w.interact(Action::merge);
		merge(other, false, w);
	}
	else {
		w.interact(Action::trade);
		float s, t[Traits::MAX_TRAIT]; // temp vars to other to use later
		s = (other.getS() - supply) * (traits[Traits::trading] / (traits[Traits::trading] + other.getT(Traits::trading))) * traits[Traits::trading]; // amount of supply to exhange
		supply += s; // apply supply exchange
		for (int i = 0; i < Traits::MAX_TRAIT; i++) {
			t[i] = (traits[i] - other.getT(i)) * (other.getT(Traits::trading) / (traits[Traits::trading] + other.getT(Traits::trading))) * other.getT(Traits::delta); // amount for other to adopt
			if ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::adapt] * 0.9 + 0.1)) // chance for trait adoption
				traits[i] += (other.getT(i) - traits[i]) * (traits[Traits::trading] / (traits[Traits::trading] + other.getT(Traits::trading))) * traits[Traits::delta]; // amount to adopt
		}
		other.traded(s, t); // apply changes to other
	}
}
void People::war(People& other, TheWorld& w) {
	w.interact(Action::war);
	if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < (pop * supply * traits[Traits::warring]) / (pop * supply * traits[Traits::warring] + other.getP() * other.getS() * other.getT(Traits::warring))) {
		win(other, w); // decide winner (comment here b/c above line is already long)
		w.interact(Action::win);
	}
	else
		other.win(*this, w);
}
void People::win(People& other, TheWorld& w) {
	w.move(id, loc, -1);
	pop -= pop / 10;
	float tmp = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (1.f + traits[Traits::warrior] + traits[Traits::trader]);
	char type;
	if (tmp < 1) // pick win reaction
		type = '0';
	else {
		tmp -= 1;
		if (tmp < traits[Traits::trader])
			type = '1';
		else
			type = '2';
	}
	switch (type) {
	case'0': // release
		supply += other.getS() / 2;
		other.lose(type, w);
		w.move(id, -1, loc);
		break;
	case'1': // enslavement
		w.move(id, -1, loc);
		other.lose(type, w);
		merge(other, false, w);
		break;
	case'2': // genocide
		supply += other.getS();
		other.lose(type, w);
		w.move(id, -1, loc);
	}
}
void People::lose(char& type, TheWorld& w) {
	w.move(id, loc, -1); // prep tot adj pop
	pop /= 2;
	switch (type) {
	case'0': // release
		supply /= 2;
		break;
	case'2': // genocide
		pop = 0;
	}
	w.move(id, -1, loc);
}
void People::getSupply(TheWorld& w) { // aquires supply for the year
	if (supply < pop * (traits[Traits::store] + 1.f)) {
		float focus = traits[Traits::hunt] + traits[Traits::gather], s = 0; // how to divy up work
		if (w.getLoc(loc).coastal) // only assign fishers if coastal
			focus += traits[Traits::fish];
		if (focus > 0) {
			w.interact(Action::hunts);
			if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < 1.f - traits[Traits::game]) { // hunt success
				s += (pop * traits[Traits::hunt] / focus) * (traits[Traits::glut] * 1.5 + 0.5) * (sqrt(traits[Traits::game]) * w.getAP(loc) * 2);
				w.interact(Action::huntW);
			}
			s += (pop * traits[Traits::gather] / focus) * (traits[Traits::glut] * 1.5 + 0.5) * ((1.f - w.getAP(loc)) * 2); // gather
			if (w.getLoc(loc).coastal) // fish if coastal
				supply += (pop * traits[Traits::fish] / focus) * (traits[Traits::glut] * 1.5 + 0.5);
			s = (s > w.getLoc(loc).supply) ? w.getLoc(loc).supply : s; // cap at available local supply
			supply += s; // update supply
			w.exploit(loc, s); // update supply in loc
		}
	}
}
void People::eat(TheWorld& w) {
	w.move(id, loc, -1); // prep to adj lclPop
	supply -= pop * (traits[Traits::glut] * 1.5 + 0.5); // consume supply
	if (supply < 0) { // starvation
		pop += supply;
		supply = 0;
	}
	w.move(id, -1, loc);
}
void People::reproduce(TheWorld& w) { // adjusts the population
	if (pop > 0) {
		w.move(id, loc, -1); // prep to adj lclPop
		pop += (traits[Traits::growthRt] * 0.5764 * (pop / 2.f)) * (1 - pop / w.getLocBase(loc)); // rP combined into single term
		w.move(id, -1, loc); // adj lclPop
	}
	else // everyone is already dead, remove from world
		w.kill(id);
}
void People::split(TheWorld& w) { // splits the population if large
	if (pop > (traits[Traits::group] * 29.f + 152.f) * (traits[Traits::sedentary] + 1.f) && ((w.isSafe() > 0) ? (w.getPpls() < w.isSafe()) : true)) { // chance to split increases with pop to max at periodic aggregation size
		w.interact(Action::splits);
		w.move(id, loc, -1); // prep to account for int math
		float tmp = 1.f / pop;
		pop *= traits[Traits::split] * (0.5 - tmp) + tmp; // drop pop
		if (pop < 1)
			pop = 1;
		supply *= traits[Traits::split] * (0.5 - tmp) + tmp; // drop supply
		w.clone(*this); // split
		pop *= 1 / (traits[Traits::split] * (0.5 - tmp) + tmp); // restore pop
		pop -= pop * (traits[Traits::split] * (0.5 - tmp) + tmp);
		supply *= 1 / (traits[Traits::split] * (0.5 - tmp) + tmp); // restore supply
		supply -= supply * (traits[Traits::split] * (0.5 - tmp) + tmp);
		w.move(id, -1, loc); // account for int math
	}
}