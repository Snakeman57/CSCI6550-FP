// copyright 2021

#include "TheWorld.h"

TheWorld::TheWorld() : bsRt(50) { // default numbers, MUST CALL INIT BEFORE USING CLASS
}
TheWorld::~TheWorld() { // dxn
	//for (int i = 0; i < info.locations; i++) // deallocate neighbors in map
	//	delete[] l[i].nbrs;
	//delete[] l; // deallocate the map
}
void TheWorld::init(const int& lat1, const int& lat2, const int& size, const int& pop) { // practical cxn, must be called seperately
	info.locations = size;
	info.lat1 = lat1;
	info.lat2 = lat2;
	asgnBiomes();
	int *lats = new int[size]; // allocate temporary array
	std::default_random_engine rngsus;
	std::normal_distribution<double> bell((double)((lat1 + lat2) / 2), (double)((lat1 - lat2) / 6/*8*/));
	for (int i = 0; i < size; i++) { // fills latitude list with a normal distribution within range
		int a = (int)bell(rngsus);
		lats[i] = (a > lat2) ? lat2 : ((a < lat1) ? lat1 : a) ; // clamp the latitude in range
	}
	std::sort(lats, lats + size); // organize latitudes so neighboring locations have latitude similarity
	for (int i = 0; i < size; i++) { // generate the map
		Location loc;
		loc.biome = getBiome(lats[i]); // assign biome from latitude chances
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
	delete lats; // deallocate no longer needed array
	for (int i = 0; i < pop; i++) { // populate the world
		p.addNew(size);
	}
}
void TheWorld::tick() { // does one tick of the sim
	updtSupply();
	updtWeather();
	p.tick();
}
void TheWorld::updtSupply() { // updates the supply value for each location
	for (int i = 0; i < info.locations; i++) {
		for (int j = 0; j < Disasters::MAX_DSTR; j++)
			if (l[i].weather.disaster[j])
				l[i].supply *= 0.8;
		if (l[i].supply != biomes[l[i].biome].supply) // moves supply toward biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt) * (((l[i].supply < biomes[l[i].biome].supply) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
		else // randomly adjusts supply for the year from biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt / 2) * ((((rand() % 2) > 0) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
	}
}
void TheWorld::updtWeather() { // updates the weather for each location
	std::default_random_engine rngsus;
	std::normal_distribution<float> bell(0, .1);
	for (int i = 0; i < info.locations; i++) {
		l[i].weather.rain = bell(rngsus); // sets iedal weather modifier on a bell curve
		l[i].weather.temp = bell(rngsus); // sets iedal temperature modifier on a bell curve
		for (int j = 0; j < Disasters::MAX_DSTR; j++) // sets natural disasters T/F depending on likelihood in biome
			l[i].weather.disaster[j] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < biomes[l[i].biome].disaster[j]);
	}
}
WorldInfo TheWorld::getWstats() const { // returns stats about the world
	return info;
}
PopInfo TheWorld::getPstats() const { // returns stats about the population
	return p.stats();
}
Biomes TheWorld::getBiome(int& lat) const { // returns a biome based on the probability of occurring at a given latitue
	int poslat = abs(lat);
	float max = 0;
	float chance[Biomes::MAX_BIOM];
	float biomeID = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random value between 0 and 1
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
	FString dir = FPaths::GameSourceDir();
	std::ifstream biomeData(std::string(TCHAR_TO_UTF8(*dir)) + "CSCI6550_FP/CSCI6550-FP-BIOMES-CSV.csv"); // import data
	std::string input; // for readinig data
	for (int i = 0; i < Biomes::MAX_BIOM; i++) { // iterate through biomes
		biomes[i].name = static_cast<Biomes>(i); // set name to enum value
		std::getline(biomeData, input, ',');
		biomes[i].supply = stof(input); // set supply from first value on line
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