// copyright 2021

#include "TheWorld.h"
#include <random>

TheWorld::TheWorld() : bsRt(50) { // default numbers, DO NOT CALL
	init(-100, 100, 100, 20);
}
TheWorld::TheWorld(int lat1, int lat2, int size, int pop) : bsRt(50) { // actual cxn
	init(lat1, lat2, size, pop);
}
TheWorld::~TheWorld() { // dxn
	for (int i = 0; i < info.locations; i++) // deallocate neighbors in map
		delete[] l[i].nbrs;
	delete[] l; // deallocate the map
	for (int i = 0; i < Biomes::MAX_BIOM; i++) // deallocate biome frequency curves
		delete biomes[i].freq;
}
void TheWorld::init(int lat1, int lat2, int size, int pop) {
	info.locations = size;
	info.lat1 = lat1;
	info.lat2 = lat2;
	asgnBiomes();
	l = new Location[size]; // dynamically allocate the map from user specs
	int * lats = new int[size]; // allocate temporary array
	std::default_random_engine rngsus;
	std::normal_distribution<double> bell((double)((lat1 + lat2) / 2), (double)((lat1 - lat2) / 6/*8*/));
	for (int i = 0; i < size; i++) { // fills latitude list with a normal distribution within range
		lats[i] = /*std::clamp(*/(int)bell(rngsus)/*, lat1, lat2)*/; // FIX THIS
	}
	//std::sort(*lats, *lats + size); // FIX THIS
	for (int i = 0; i < size; i++) { // generate the map
		l[i].biome = getBiome(lats[i]); // assign biome from latitude chances
		info.biomes[l[i].biome]++;
		l[i].supply = biomes[l[i].biome].supply; // assign base supply
		int a = rand() % 6 + 1; // number of neighbors
		l[i].nbrs = new Neighbor[(a <= size) ? a : size]; // contain if necessary
		for (int j = 0; j < sizeof(l[i].nbrs); j++) { // generate one-way neighbor paths that are somewhat likely to overlap
			int b = ((rand() % 2) > 0) ? ((i - j >= 0) ? i - j : i + j) : ((i + j < size) ? i + j : i - j); // generate neighbor index
			l[i].nbrs[j].loc = b; // assign generated index
			l[i].nbrs[j].dist = (abs(lats[b] - lats[i]) + 1) * (rand() % 366); // difficulty to reach location in days
		}
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
		for (int j = 0; j < sizeof(Weather::disaster); j++)
			if (l[i].weather.disaster[j])
				l[i].supply *= .8;
		if (l[i].supply != biomes[l[i].biome].supply) // moves supply toward biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt) * (((l[i].supply < biomes[l[i].biome].supply) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
		else // randomly adjusts supply for the year from biome base
			l[i].supply += ((biomes[l[i].biome].supply / bsRt) * ((((rand() % 2) > 0) ? 1.f : -1.f) + l[i].weather.rain + l[i].weather.temp));
	}
}
void TheWorld::updtWeather() { // updates the weather for each location
	std::default_random_engine rngsus;
	std::normal_distribution<float> bell(0, .1);
	for (int i = 0; i < info.locations; i++) {
		l[i].weather.rain = bell(rngsus); // sets iedal weather modifier on a bell curve
		l[i].weather.temp = bell(rngsus); // sets iedal temperature modifier on a bell curve
		for (int j = 0;j < 5;j++) // sets natural disasters T/F depending on likelihood in biome
			l[i].weather.disaster[j] = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) < biomes[l[i].biome].disaster[j]) ? true : false;
	}
}
WorldInfo TheWorld::getWstats() const { // returns stats about the world
	return info;
}
PopInfo TheWorld::getPstats() const { // returns stats about the population
	return p.stats();
}
Biomes TheWorld::getBiome(int lat) { // returns a biome based on the probability of occurring at a given latitue
	float max = 0;
	float chance[Biomes::MAX_BIOM];
	float biomeID = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); // random value between 0 and 1
	Biomes biome = Biomes::MAX_BIOM;
	for (int i = 0; i < Biomes::MAX_BIOM; i++) // takes the total frequency of all biomes at lat
		max += biomes[i].freq->GetFloatValue((float)lat);
	for (int i = 0; i < Biomes::MAX_BIOM; i++) // generates chances of any given biome occurring at lat
		chance[i] = biomes[i].freq->GetFloatValue((float)lat) / max;
	for (int i = 0; biomeID > 0 && i < Biomes::MAX_BIOM; i++) { // sets biome according to generated chances from earlier random number
		biomeID -= chance[i];
		biome = biomes[i].name;
	}
	return biome; // return chosen biome
}
void TheWorld::asgnBiomes() { // currently using hard coded values, look into importing
	float supplies[] = { // base supplies
		0,
		0
	};
	float disasters[][sizeof(Weather::disaster)] = { // disaster chances
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	FRichCurve* freqsData[Biomes::MAX_BIOM]; // frequency curves
	FKeyHandle key;
	for (int i = 0; i < Biomes::MAX_BIOM; i++) {
		freqsData[i] = new FRichCurve();
	}
	freqsData[0]->AddKey(-100.f, 0.f);
	freqsData[0]->AddKey(100.f, 1.f);
	//freqsData[0]->SetKeyInterpMode(key, ERichCurveInterpMode::RCIM_Linear);
	freqsData[1]->AddKey(-100.f, 0.f);
	//freqsData[1]->AddKey(100.f, 1.f);
	//freqsData[1]->SetKeyInterpMode(key, ERichCurveInterpMode::RCIM_Linear);

	for (int i = 0; i < Biomes::MAX_BIOM; i++) { // actually set the values
		biomes[i].name = static_cast<Biomes>(i);
		biomes[i].supply = supplies[i];
		biomes[i].freq = NewObject<UCurveFloat>();
		biomes[i].freq->FloatCurve = *freqsData[i];
		for(int j = 0; j < sizeof(Weather::disaster); j++)
			biomes[i].disaster[i] = disasters[i][j];
	}
	for (auto& i : freqsData) 
		delete i;
}