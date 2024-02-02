#pragma once
class Genetic
{
	struct individual {
		int roadLength;
		std::vector<int> road;
		
		bool operator() (individual i1, individual i2) {
			return (i1.roadLength < i2.roadLength);
		}
	} individualObject;

	std::vector<int> bestRoad;
	int bestRoadLength;
	int worstRoadLength;

	std::vector<individual> population;

	int Mutation(int numberOfVertices, int ** vertices, individual &ind);
	int OXCrossing(int numberOfVertices, int ** vertices, individual parent1, individual parent2, individual &child1, individual &child2);
	int CalculateRoad(int numberOfVertices, int **vertices, std::vector<int> &currentRoad);
	int GenerateRandomRoad(int numberOfVertices, int ** vertices, int populationIndex);
	int GenerateFirstPopulation(int numberOfVertices, int ** vertices);

public:
    int populationSize;
    float mutationProbability;
    float reproductionProbability;
    double endTime;
    long int rouletteSum;
    int iterations;
    int badIterations;

	Genetic();
	~Genetic();

	int RunGenetic(int numberOfVertices, int ** vertices);
    int getBestCost();
};

