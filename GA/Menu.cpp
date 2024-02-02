#include "stdafx.h"
#include "Menu.h"
#include "Genetic.h"
#include <windows.h>
using namespace std;

//-------------------------------------------------------------------------
long long int read_QPC()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}
//-------------------------------------------------------------------------

Menu::Menu()
{
	numberOfVertices = 0;
	filename = "Brak wczytanego pliku";
}

Menu::~Menu()
{
	if (numberOfVertices != 0) {
		for (int i = 0; i < numberOfVertices; i++)
			delete vertices[i];
		delete[] vertices;
		vertices = NULL;
		numberOfVertices = 0;
	}
}

void Menu::ShowMenu(){

    Genetic ga;
    long long int frequency, start, elapsed;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

    fstream file_in;
    file_in.open("C:\\Users\\aleks\\OneDrive\\Pulpit\\uczelnia\\pea\\Genetyczny - Nowy\\conf.ini");  // otwarcie pliku "conf.ini"
    if (!file_in.is_open()) {
        cout << " File OPEN error - FILE_IN" << endl;
    }
    ofstream file_out("wyniki.csv");          // otwarcie pliku "wyniki.csv" - tymczasowa nazwa dopóki nie zczytamy ostatniej lini z "conf.ini"
    string instance_info;
    while (getline(file_in, instance_info)) {
        std::string instance_name, reps_s;      // wczytanie nazwy instancji, ilości powtórzeń
        int reps;

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
        }
        // Wczytanie nazwy instancji oraz liczby powtórzeń
        if (instance_name[instance_name.size() - 1] =='v') {// ostatnia linia z "conf.ini" czyli nazwa pliku csv jest wpisywana na końcu tego pliku
            file_out << instance_name << endl;              // ostatnia linia jest wtedy gdy pierwsze słowo z lini konczy się na 'v'
            file_out.close();
            if (rename("wyniki.csv", instance_name.c_str()))
                cout << "ERROR renaming" << endl;
        }
        else
        {                                            // jesli to nie ostatnia linia to mamy instancje dla której należy znaleźć optymalną ścieżkę
            cout << "Czas wykonywania algorytmu genetycznego w sekundach: \n";
            cin >> ga.endTime;
            cout<<endl<<"Wprowadz rozmiar populacji startowej:\n";
            cin>>ga.populationSize;
            cout<<endl<<"Wprowadz prawdopodobienstwo reprodukcji (od 0.1 do 1):\n";
            cin>>ga.reproductionProbability;
            cout<<endl<<"Prawdopodobienstwo wystapienia mutacji (od 0 do 1):\n";
            cin>>ga.mutationProbability;
            file_out << instance_info<< " " << ga.endTime << " " << ga.populationSize<< " " << ga.reproductionProbability<< " " << ga.mutationProbability<< endl; // wpisanie do pliku csv danych instancji
            LoadFile(instance_name); // Wczytanie macierzy z pliku
            int rep = 1;
            while (rep++ <= reps) {
                start = read_QPC();
                ga.RunGenetic(numberOfVertices, vertices);
                elapsed = read_QPC() - start;
                file_out << (1000*elapsed) / frequency<<";"<<ga.getBestCost()<<endl;
                cout<<"Czas wykonania: "<<(1000*elapsed) / frequency<<" milisekund"<<endl;
            }
        }
    }
	return;
}

int Menu::LoadFile(string &filename) {
    int verticesff = 0;
    if (numberOfVertices != 0) {
        for (int i = 0; i < numberOfVertices; i++)
            delete vertices[i];
        delete[] vertices;
        vertices = NULL;
        numberOfVertices = 0;
    }
    ifstream File;
    File.open("C:\\Users\\aleks\\OneDrive\\Pulpit\\uczelnia\\pea\\Genetyczny - Nowy\\" + filename);
    if (!File.good()) {
        this->filename = "Brak wczytanego pliku";
        cout << "\nNie mozna wczytac pliku!\n";
        return 0;
    }
    this->filename = filename;

    string line;
    while (!File.eof()) {
        getline(File, line);
        if (verticesff == 0) {
            numberOfVertices = stoi(line);
            verticesff++;
        }
        vertices = new int *[numberOfVertices];
        for (int i = 0; i < numberOfVertices; i++)
            vertices[i] = new int[numberOfVertices];

        for (int j = 0; j < numberOfVertices; j++) {
            for (int k = 0; k < numberOfVertices; k++) {
                File >> vertices[j][k];
            }
        }return 0;
    }
}

