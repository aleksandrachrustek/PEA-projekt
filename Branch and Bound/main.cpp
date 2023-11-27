#include <iostream>
#include <fstream>
#include "matrix.h"
#include "Branch_Bound.h"
#include <windows.h>
#include <string>

using namespace std;

int size_of_m=0;

//-------------------------------------------------------------------------
long long int read_QPC()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}
//-------------------------------------------------------------------------


void choose_option(Matrix macierz)
{
    long long int frequency, start, elapsed;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
    Branch_Bound bb;

    fstream file_in;
    file_in.open("C:\\Users\\aleks\\OneDrive\\Pulpit\\uczelnia\\pea\\BranchAndBound\\conf.ini");                                              // otwarcie pliku "conf.ini"
    if(!file_in.is_open()){
        cout << " File OPEN error - FILE_IN" << endl;
    }
    ofstream file_out("wyniki.csv");                               // otwarcie pliku "result.csv" - tymczasowa nazwa dopóki nie zczytamy ostatniej lini z "conf.ini"
    string instance_info;

    while(getline(file_in, instance_info) ) {

        std::string instance_name, reps_s;       // wczytanie nazwy instancji, ilości powtórzeń, kosztu najlepszej ścieżki, najlepszej ścieżki
        // elementy powinny być rozdzielone POJEDYNCZĄ spacją
        // za ostatnim elementem w pliku "conf.ini" może być jedna spacja lub nic
        int reps;                                            // w pliku "conf.ini" optymalna scieżka powinna zaczynać się i konczyć węzłem "0"

        bool read_name, read_reps;
        read_name = true;
        read_reps = true;

        for (int i = 0; i < instance_info.size(); i++) {

            for (char c : instance_info) {
                if (!isspace(c)) {
                    if (read_name) {instance_name.push_back(c);}
                    else if (read_reps) reps_s.push_back(c);
                }
                else if (read_name) read_name = false;
                else if (read_reps) { read_reps = false; reps = stoi(reps_s);}
            }
        }                                                               // dane instancji lub nazwa pliku csv zostały wczytane
        if (instance_name[instance_name.size() - 1] =='v') {               // ostatnia linia z "conf.ini" czyli nazwa pliku csv jest wpisywana na końcu tego pliku
            file_out << instance_name << endl;                     // ostatnia linia jest wtedy gdy pierwsze słowo z lini konczy się na 'v'
            file_out.close();
            if (rename("wyniki.csv", instance_name.c_str()))
                cout << "ERROR renaming" << endl;
        } else {                                                         // jesli to nie ostatnia linia to mamy instancje dla której należy znaleźć optymalną ścieżkę

            file_out << instance_info<< endl;                     // wpisanie do pliku csv danych instancji w celu określenia dla jakiej instancji będą podane czasy
            macierz.read_from_file(instance_name);
            int rep = 1;
            while (rep++ <= reps) {
                bb.setSize(size_of_m);
                start = read_QPC();
                bb.TSP(macierz.return_matrix());
                elapsed = read_QPC() - start;
                file_out << (1000000.0 * elapsed) / frequency << endl;
            }
        }
    }
}

int main()
{
    Matrix macierz;
    choose_option(macierz);
}
