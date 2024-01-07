//
// Created by aleks on 27.11.2023.
//

#include "Tabu.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <windows.h>

using namespace std;

//-------------------------------------------------------------------------
long long int Tabu::read_QPC()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}
//-------------------------------------------------------------------------

Tabu::Tabu() = default;

Tabu::~Tabu()= default;

void Tabu::Create_tabuList(int rozmiar)
{
    tabu_list.resize(rozmiar);
    for(int i=0; i<rozmiar; i++)
        tabu_list[i].resize(rozmiar);
}

int Tabu::M_val(vector<int> &path, vector<vector<int>> &m)
{
    int j = 0;
    int koszt = 0;
    for(int city=0; city<path.size(); city++)
    {
        koszt += m[j][path[city]];
        j=path[city];
    }
    koszt+=m[j][0];
    return koszt;
}

void Tabu::Add_to_tabulist(int x, int y, int rozmiar)
{
    if(x<y)
        tabu_list[x][y]= rozmiar;
    else
        tabu_list[y][x] = rozmiar;
}

void Tabu::Update_tabulist()
{
    for(int i=0; i<tabu_list.size(); i++)
        for(int j=0; j<tabu_list[i].size(); j++)
            if(tabu_list[i][j]>0)
                tabu_list[i][j]--;
}

bool Tabu::Swap(int rozmiar, vector<vector<int>> &m, vector<int> &curr_sol, int best_cost)
{
    int initial_best = best_cost;
    for (int i = 0; i < curr_sol.size(); i++)
        for (int j = i+1; j < curr_sol.size(); j++)
        {
            if (i == j)
                continue;

            int buff=curr_sol[i];
            curr_sol[i]=curr_sol[j];
            curr_sol[j] = buff;

            if (tabu_list[j][i] == 0) {
                int cost = M_val(curr_sol, m);
                if (best_cost > cost) {
                    Add_to_tabulist(i, j, rozmiar);
                    best_cost = cost;
                } else {
                    buff = curr_sol[i];
                    curr_sol[i] = curr_sol[j];
                    curr_sol[j] = buff;
                }
            }
        }
    if(initial_best>best_cost)              // jezeli znaleziono nizszy koszt zwracamy true, inaczej false
        return true;
    else
        return false;
}

void Tabu::TSP(vector<vector<int>> matrix)
{
        // deklaracja list przechowujacych miasta
        /* curr_sol -> aktualnie badania sciezka
        * best_sol -> najkorzystniejsza znalezniona sciezka
        * cities -> posortowana lista miast (kolejnosc wg indeksow w macierzy) */
    vector<int> curr_sol, cities, best_sol;
    int rozmiar = matrix.size();
    Create_tabuList(rozmiar);

    for(int i=1; i<rozmiar; i++)
        cities.push_back(i);

    best_sol.resize(rozmiar);
    curr_sol.resize(rozmiar);
    int best_cost, curr_cost;               // best -> koszt najkorzystniejszej sciezki, curr -> koszt sciezki badanej
                                            // zapisujemy miasta w kolejnosci do sciezki, a potem losujemy miasta w sciezce
    curr_sol = cities;
    shuffle(curr_sol.begin(), curr_sol.end(), std::mt19937(std::random_device()()));
    best_cost = M_val(curr_sol, matrix);
    best_sol = curr_sol;
    // Zmienne do pomiaru czasu
    long long int frequency, start, elapsed_temp, elapsed;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

    // kryterium stopu (czas w sekundach), jezeli kryterium nie zostalo wprowadzone w menu, to domyslna wartosc wynosi 5 sekund
    start = read_QPC();
    int iterations_without_improvement = 0;

    while(iterations_without_improvement < max_iterations)
    {
        Update_tabulist();
        bool is_changed = Swap(rozmiar, matrix, curr_sol, best_cost);
        curr_cost = M_val(curr_sol, matrix);
            // jezeli znaleziono lepsza sciezke, to zapisujemy ją
        if(is_changed)
        {
            best_sol = curr_sol;
            best_cost = M_val(best_sol, matrix);
            iterations_without_improvement = 0; // Zresetuj liczbę iteracji bez poprawy
        }
        else
        { 
            iterations_without_improvement++;
        }
        // Porownanie czasu wykonania z kryterium stopu
        elapsed_temp = read_QPC() - start;
        if(float(elapsed_temp/frequency) >= stop)
            break;
    }

    elapsed = read_QPC() - start;
    cout<<"Sciezka: \n";
    cout<<"0 ";
    for(int city : best_sol)
        cout<<" -> "<<city<<" ";
    the_best_cost=best_cost;
    cout<<endl<<"Koszt: "<<best_cost<<endl;

}

int Tabu::getBestCost() const {
    return the_best_cost;
}
