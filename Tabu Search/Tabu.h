//
// Created by aleks on 27.11.2023.
//

#include <vector>
#ifndef TABUSEARCH_TABU_H
#define TABUSEARCH_TABU_H
using namespace std;

class Tabu {
public:
    int stop=5;
    int max_iterations;
    int the_best_cost;
    Tabu();
    ~Tabu();
    vector<vector<int>> tabu_list;
    void TSP(vector<vector<int>> matrix);

    bool Swap(int rozmiar, vector<vector<int>> &m, vector<int> &curr_sol, int best_sol);

    void Update_tabulist();

    void Add_to_tabulist(int x, int y, int rozmiar);

    void Create_tabuList(int rozmiar);

    int M_val(vector<int> &path, vector<vector<int>> &m);

    long long int read_QPC();

    int getBestCost() const;
};

#endif //TABUSEARCH_TABU_H
