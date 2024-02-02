#include "stdafx.h"
#include "Genetic.h"

using namespace std;

Genetic::Genetic()
{
	this->bestRoadLength = INT_MAX;
	this->worstRoadLength = 0;
}

Genetic::~Genetic(){}

int Genetic::getBestCost()
{
    return bestRoadLength;
}

int Genetic::RunGenetic(int numberOfVertices, int ** vertices)
{
	if (numberOfVertices <= 0) {
		cout << "\nNie wczytano danych\n";
		return 0;
	}

	cout << "\nParametry algorytmu genetycznego:\n";
	cout << " Czas dzialania algorytmu: " << this->endTime << "(s)" << endl;
	cout << " Rozmiar populacji: " << this->populationSize << endl;
	cout << " Prawdopodobienstwo krzyzowania: " << this->reproductionProbability << endl;
	cout << " Prawdopodobienstwo mutacji: " << this->mutationProbability << endl;
	cout << "\nZostal uruchomiony algorytm genetyczny\n";

	this->iterations = 0;
	this->badIterations = 0;

	//Generowanie pierwszej populacji, losowo
	GenerateFirstPopulation(numberOfVertices, vertices);

	if (population.size() <= 0) {
		cout << "Za maly rozmiar populacji, algorytm przerwano.\n";
		return 0;
	}

	std::chrono::time_point<std::chrono::system_clock> start, end; // start pomiaru czasu
	start = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = start - start;

	float mutateRand;
	int generationCount = 0;
	vector <individual> nextGeneration;

	while (elapsed_seconds.count() < endTime) {
		nextGeneration.clear();
		sort(population.begin(), population.end(), individualObject);

		int indexInPopulation = 0;
		while (nextGeneration.size() < populationSize*0.2) {
			if ((nextGeneration.size() == 0) || (population[indexInPopulation].roadLength != nextGeneration.back().roadLength))
				nextGeneration.push_back(population[indexInPopulation]);
			indexInPopulation++;
		}

		rouletteSum = 0;
		for (int i = 0; i < population.size(); i++) {

			int x = this->population[i].roadLength;
			int y = this->worstRoadLength;
			int a = y - x;
			int b = a / (long int)numberOfVertices;
			rouletteSum += b;
		}

		while (nextGeneration.size() < populationSize) { // dopóki nie bêdzie nowej generacji - wektor nie bêdzie pe³ny
			vector <int> reproductiveIDsVec;

			while (reproductiveIDsVec.size() == 2) { // warunek - kiedy koñczy siê ruletkowanie
				long int tempSum = rouletteSum;
				long int random = rand() % rouletteSum;
				int sumIndex = 0;
				while (tempSum > random) { //sprawdzanie indexu wyruletkowanej wartoœci
					tempSum -= (this->worstRoadLength - this->population[sumIndex].roadLength) / (long int)numberOfVertices;
					sumIndex++;
				}
				reproductiveIDsVec.push_back(sumIndex - 1);
			}

			float reproductionRand;
			float tempProb1 = rand() % 100000;
			reproductionRand = tempProb1 / 100000;

            //cross ox
			if (reproductionRand <= reproductionProbability) {
				individual child1, child2;
				OXCrossing(numberOfVertices, vertices, population.front(), population.back(), child1, child2);

				// mutation
				float tempProb = rand() % 100000; // mo¿liwa mutacja dla pierwszego dziecka
				mutateRand = tempProb / 100000;
				if (mutateRand <= mutationProbability) {
					Mutation(numberOfVertices, vertices, child1);
				}

				tempProb = rand() % 100000;
				mutateRand = tempProb / 100000; // mo¿liwa mutacja dla pierwszego dziecka
				if (mutateRand <= mutationProbability) {
					Mutation(numberOfVertices, vertices, child2);
				}

				// podmiana najlepszej i najgorszej drogi z nowych rozwi¹zañ
				if (child1.roadLength < bestRoadLength) {
					bestRoad = child1.road;
					bestRoadLength = child1.roadLength;
				}
				if (child2.roadLength < bestRoadLength) {
					bestRoad = child2.road;
					bestRoadLength = child2.roadLength;
				}
				if (child1.roadLength > worstRoadLength) {
					worstRoadLength = child1.roadLength;
				}
				if (child2.roadLength > worstRoadLength) {
					worstRoadLength = child2.roadLength;
				}
				nextGeneration.push_back(child1);
				nextGeneration.push_back(child2);
			}
			reproductiveIDsVec.clear();
		}

		while (nextGeneration.size() > populationSize) {
			nextGeneration.pop_back();
		}
		population = nextGeneration;

		end = std::chrono::system_clock::now(); // koniec pomiaru czasu
		elapsed_seconds = end - start;
		iterations++;
		badIterations++;
		generationCount++;

	}
	cout << "\nDlugosc najlepszej drogi: " << bestRoadLength << endl;
	return 0;
}

int Genetic::GenerateRandomRoad(int numberOfVertices, int ** vertices, int index) {

	individual temp;

	bool * isInCurrentRoad = new bool[numberOfVertices];
	for (int i = 0; i < numberOfVertices; i++)
		isInCurrentRoad[i] = false;

	while (temp.road.size() < numberOfVertices) {
		int randomVerticle = (rand() % (numberOfVertices));
		if (!isInCurrentRoad[randomVerticle]) {
			temp.road.push_back(randomVerticle);
			isInCurrentRoad[randomVerticle] = true;
		}
	}
	temp.roadLength = CalculateRoad(numberOfVertices, vertices, temp.road);
	if (temp.roadLength < this->bestRoadLength) {
		this->bestRoadLength = temp.roadLength;
		this->bestRoad = temp.road;
	}
	else if (temp.roadLength > this->worstRoadLength) {
		this->worstRoadLength = temp.roadLength;
	}

	this->population.push_back(temp);
	return 0;
}

int Genetic::GenerateFirstPopulation(int numberOfVertices, int ** vertices)
{
	for (int i = 0; i < this->populationSize; i++) {
		GenerateRandomRoad(numberOfVertices, vertices, i);
	}
	return 0;
}

int Genetic::Mutation(int numberOfVertices, int ** vertices, individual &ind) {
	// mutacja typu invert, losowanie krawedzi
	int randomVer1 = (rand() % numberOfVertices);
	int randomVer2 = (rand() % numberOfVertices);
	while (randomVer2 == randomVer1) {
		randomVer2 = (rand() % numberOfVertices);
	}
	if (randomVer2 < randomVer1)
		swap(randomVer2, randomVer1);

	int invSize = randomVer2 - randomVer1;
	vector <int> citesToInvert;

	for (int i = randomVer1; i < randomVer2; i++) {
		citesToInvert.push_back(ind.road[i]);
	}
	for (int i = randomVer1; i < randomVer2; i++) {
		ind.road[i] = citesToInvert.back();
		citesToInvert.pop_back();
	}
	ind.roadLength = CalculateRoad(numberOfVertices, vertices, ind.road); //obliczenie drogi dla danego elementu

	return 0;
}

int Genetic::OXCrossing(int numberOfVertices, int ** vertices, individual parent1, individual parent2, individual &child1, individual &child2) {

    // cross ox
	int size = parent1.road.size();

	bool *childUsed1 = new bool[size]; // tablica booli przechowujaca które wierzcho³ki s¹ w przekopiowanym fragmencie
	bool *childUsed2 = new bool[size];

	individual childAlt1, childAlt2;
	childAlt1.road.resize(size);
	childAlt2.road.resize(size);

	for (int i = 0; i < size; i++) {
		childUsed1[i] = false;
		childUsed2[i] = false;
	}

	int startPos = rand() % size;
	int endPos = -1;
	while (endPos < 0 || endPos == startPos) {
		endPos = rand() % size;
	}
	if (startPos > endPos) {
		swap(startPos, endPos);
	}

	vector <int> childCpy1, childCpy2; // wektory tego co jest kopiowane
	childCpy1.resize(endPos - startPos);
	childCpy2.resize(endPos - startPos);

	int tempCounter = 0;
	for (int i = startPos; i < endPos; i++) {
		childAlt2.road[i] = parent1.road[i];
		childAlt1.road[i] = parent2.road[i];
		childCpy1[tempCounter] = parent2.road[i];
		childCpy2[tempCounter] = parent1.road[i];

		childUsed1[parent2.road[i]] = true;
		childUsed2[parent1.road[i]] = true; // zape³nienie tablicy booli

		tempCounter++;
	}

	int c1Counter;
	int c2Counter;

	// wrzucenie tych co siê nie powtarzaj¹
	if (endPos < size) {
		c1Counter = endPos;
		c2Counter = endPos;
	}
	else {
		c1Counter = 0;
		c2Counter = 0;
	}

	if (endPos < size)
		for (int i = endPos; i < size; i++) { // wrzucenie koñca ³añcucha
			if (childUsed1[parent1.road[i]] == false) {
				childAlt1.road[c1Counter] = parent1.road[i];
				c1Counter++;
				if (c1Counter >= size)
					c1Counter = 0;
			}
			if (childUsed2[parent2.road[i]] == false) {
				childAlt2.road[c2Counter] = parent2.road[i];
				c2Counter++;
				if (c2Counter >= size)
					c2Counter = 0;
			}
		}
	for (int i = 0; i < startPos; i++) { // wrzucenie pocz¹tku ³añcucha
		if (childUsed1[parent1.road[i]] == false) {
			childAlt1.road[c1Counter] = parent1.road[i];
			c1Counter++;
			if (c1Counter >= size)
				c1Counter = 0;
		}
		if (childUsed2[parent2.road[i]] == false) {
			childAlt2.road[c2Counter] = parent2.road[i];
			c2Counter++;
			if (c2Counter >= size)
				c2Counter = 0;
		}
	}
	for (int i = 0; i < childCpy1.size(); i++) { // wrzucenie tych co s¹ skopiowane do drugiego dziecka
		if (childUsed1[childCpy2[i]] == false) {
			childAlt1.road[c1Counter] = childCpy2[i];
			c1Counter++;
			if (c1Counter >= size)
				c1Counter = 0;
		}
		if (childUsed2[childCpy1[i]] == false) {
			childAlt2.road[c2Counter] = childCpy1[i];
			c2Counter++;
			if (c2Counter >= size)
				c2Counter = 0;
		}
	}

	childAlt1.roadLength = CalculateRoad(numberOfVertices, vertices, childAlt1.road);
	childAlt2.roadLength = CalculateRoad(numberOfVertices, vertices, childAlt2.road);

	child1 = childAlt1;
	child2 = childAlt2;

	delete childUsed1;
	delete childUsed2;
	return 0;
}

int Genetic::CalculateRoad(int numberOfVertices, int **vertices, vector<int> &currentRoad) {

	int temp = currentRoad.size();
	if (temp > 1) {
		int length = 0;
		for (int i = 0; i < temp - 1; i++)
			length += vertices[currentRoad[i]][currentRoad[i + 1]];

		if (temp == numberOfVertices)
			length += vertices[currentRoad[temp - 1]][currentRoad[0]];

		return length;
	}
	return 0;
}



