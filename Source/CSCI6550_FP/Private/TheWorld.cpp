// Copyright 2021 Matthew Sentell

#include "TheWorld.h"

TheWorld::TheWorld() : bsRt(5){ // default numbers, MUST CALL INIT BEFORE USING CLASS
}
TheWorld::~TheWorld() { // dxn
}
void TheWorld::init(const int& lat1, const int& lat2, const int& size, const int& pop) { // practical cxn, must be called seperately
	info.locations = size;
	info.lat1 = lat1;
	info.lat2 = lat2;
	asgnBiomes();
	int* lats = new int[size]; // allocate temporary array
		std::default_random_engine rngsus;
		std::normal_distribution<double> bell((double)((lat1 + lat2) / 2), (double)((lat1 - lat2) / 6/*8*/));
	for (int i = 0; i < size; i++) { // fills latitude list with a normal distribution within range
		int a = (int)bell(rngsus);
		lats[i] = (a > lat2) ? lat2 : ((a < lat1) ? lat1 : a) ; // clamp the latitude in range
	}
	std::sort(lats, lats + size - 1); // organize latitudes so neighboring locations have latitude similarity
	for (int i = 0; i < size; i++) { // generate the map
		Location loc;
		loc.biome = getBiome(lats[i]); // assign biome from latitude chances
		loc.coastal = ((rand() % 10) > 0) ? false : true;
		info.biomes[loc.biome]++;
		loc.supply = biomes[loc.biome].supply; // assign base supply
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
void TheWorld::tick() { // does one tick of the sim
	p.tick(*this);
	updtWeather();
	updtSupply();
}
void TheWorld::updtSupply() { // updates the supply value for each location
	for (int i = 0; i < info.locations; i++) {
		for (int j = 0; j < Disasters::MAX_DSTR; j++)
			if (l[i].weather.disaster[j])
				l[i].supply *= 0.8;
		if (l[i].supply != biomes[l[i].biome].supply) // moves supply toward biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt) * (((l[i].supply < biomes[l[i].biome].supply) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
		else // randomly adjusts supply for the year from biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt / 2) * (((rand() % 2 > 0) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
	}
	info.avgSupply = 0;
	for (int i = 0; i < l.Num(); i++)
		info.avgSupply += l[i].supply;
	info.avgSupply /= l.Num();
}
void TheWorld::updtWeather() { // updates the weather for each location
	std::default_random_engine rngsus;
	std::normal_distribution<float> bell(0, .1);
	for (int i = 0; i < info.locations; i++) {
		l[i].weather.rain = bell(rngsus); // sets iedal weather modifier on a bell curve
		l[i].weather.temp = bell(rngsus); // sets iedal temperature modifier on a bell curve
		for (int j = 0; j < Disasters::MAX_DSTR; j++) // sets natural disasters T/F depending on likelihood in biome
			l[i].weather.disaster[j] = (static_cast<float>(rand() / static_cast<float>(RAND_MAX)) < biomes[l[i].biome].disaster[j]);
	}
}
void TheWorld::refreshStats() { // updates expensive-to-collect stat values
	for (int i = 0; i < Biomes::MAX_BIOM; i++) { // reset
		info.pops[i] = 0.f;
		info.supplies[i] = 0.f;
		info.avgPopB[i] = 0;
		info.avgSupplyB[i] = 0.f;
	}
	for (auto& i : l) { // get totals
		info.pops[i.biome] += i.pop.Num();
		info.supplies[i.biome] += i.supply;
		for (auto& j : i.pop) {
			info.avgPopB[i.biome] += p.get(j).getP();
			info.avgSupplyB[i.biome] += p.get(j).getS();
		}
	}
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
float TheWorld::getLocBase(int& loc) {
	return biomes[l[loc].biome].supply;
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
	if (loc1 >= 0)
		l[loc1].pop.Remove(id);
	if (loc2 >= 0)
		l[loc2].pop.Add(id);
}
void TheWorld::clone(People& ppl) { // duplicate a People (used in split)
	p.clone(ppl, *this);
}
void TheWorld::interact(bool war) { // update interaction counts
	if (war)
		info.wars++;
	else
		info.trades++;
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
void Population::del(int id, TheWorld& w) { // delete
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
void People::move(TheWorld& w) {
	TArray<Neighbor> n;
	w.getNeighbors(loc, n); // collect neighbors of current location
	int dstn = loc, dist = 0, lclPop = 0;
	for (int i = 0; i < w.getLoc(loc).pop.Num(); i++) // get total pop of loc
		lclPop += w.getPpl(w.getLoc(loc).pop[i]).getP();
	float h = w.getLoc(loc).supply * (traits[Traits::glut] * 1.5 + 0.5) + lclPop * (traits[Traits::gregarious] * 2 - 1); // best location heuristic val
	for (int i = 0; i < n.Num(); i++) { // check utility of moving to each neighbor
		lclPop = 0;
		for (int j = 0; j < w.getLoc(n[i].loc).pop.Num(); j++) // get total pop of current neighbor
			lclPop += w.getPpl(w.getLoc(n[i].loc).pop[j]).getP();
		if (w.getLoc(n[i].loc).supply * (traits[Traits::glut] * 1.5 + 0.5) - n[i].dist * (traits[Traits::sedentary] * 1.5 + 0.5) + lclPop * (traits[Traits::gregarious] * 2 - 1) > h) {
			h = w.getLoc(n[i].loc).supply * (traits[Traits::glut] * 1.5 + 0.5) - n[i].dist * (traits[Traits::sedentary] * 1.5 + 0.5) + lclPop * (traits[Traits::gregarious] * 2 - 1);
			dstn = n[i].loc; // neighbor had better heuristic, update mvmt info (comment here b/c 2 lines above already long)
			dist = n[i].dist;
		}
	}
	if (dstn != loc) {
		supply -= dist * pop; // expend resources to move
		w.move(id, loc, dstn); // correct Location pops
	}
	loc = dstn; // update Location
}
void People::interact(TheWorld& w) { // if other ppls present, consider interacting with them
	if (w.getLoc(loc).pop.Num() > 1 && static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::gregarious]) { // decide to interact
		People tmp = w.getPpl(w.getLoc(loc).pop[rand() % w.getLoc(loc).pop.Num()]); // decide with whom to interact
		if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::trader] / (traits[Traits::trader] + traits[Traits::warrior])) { // decide if interaction is friendly
			trade(tmp);
			w.interact(false);
		}
		else { // unfriendly interaction
			war(tmp);
			w.interact(true);
		}
	}
}
void People::getSupply(TheWorld& w) { // aquires supply for the year
	float focus = traits[Traits::hunt] + traits[Traits::gather], s = 0; // how to divy up work
	if (w.getLoc(loc).coastal) // only assign fishers if coastal
		focus += traits[Traits::fish];
	if (focus > 0) {
		if (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < traits[Traits::game]) // hunt 
			if (rand() % 100 < 48) // chance of successful hunt
				s += pop * (traits[Traits::hunt] / focus) * (traits[Traits::glut] * 1.5 + 0.5) * 10; // big game
			else
				s += pop * (traits[Traits::hunt] / focus) * (traits[Traits::glut] * 1.5 + 0.5); // small game (traps)
		s += pop * (traits[Traits::gather] / focus) * (traits[Traits::glut] * 1.5 + 0.5); // gather
		if (w.getLoc(loc).coastal) // fish if coastal
			s += pop * (traits[Traits::fish] / focus) * (traits[Traits::glut] * 1.5 + 0.5);
		s = (s > w.getLoc(loc).supply) ? w.getLoc(loc).supply : s; // cap at available local supply
		supply += s; // update supply
		w.exploit(loc, s); // update supply in loc
	}
}
void People::reproduce(TheWorld& w) { // adjusts the population
	pop += (1 - pop / w.getLocBase(loc)) * (traits[Traits::growthRt] * (pop / 2.f) * 0.75); // logistic fxn
	if (pop <= 0) // everyone is dead, remove from world
		w.kill(id);
}
void People::split(TheWorld& w) { // splits the population if large
	if (pop > 152 && static_cast<float>(rand()) / static_cast<float>(RAND_MAX) < (pop - 152.f) / (traits[Traits::group] * 29)) { // chance to split increases with pop to max at periodic aggregation size
		pop /= 2;
		supply /= 2;
		w.clone(*this);
	}
}