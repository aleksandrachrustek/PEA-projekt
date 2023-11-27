#include "matrix.h"
#include <fstream>
#include <iostream>

using namespace std;

Matrix::Matrix()
{
}

Matrix::~Matrix()
= default;

vector<vector<int>> Matrix::return_matrix()
{
    return matrix;
}

void Matrix::read_from_file(string filename)
{
    matrix.clear();
    int val;
    fstream file;
    file.open("C:\\Users\\aleks\\OneDrive\\Pulpit\\uczelnia\\pea\\BranchAndBound\\"+ filename);
    if(file.is_open())
    {
        file >> size;
        matrix.resize(size);
        if(file.fail())
            cout << "File error - READ SIZE" << endl;
        else
            for(int i = 0; i < size; i++)
            {
                for(int j=0; j<size; j++)
                {
                    file >> val;
                    if(file.fail())
                    {
                        cout << "File error - READ DATA" << endl;
                        break;
                    }
                    else
                    {
                        matrix[i].push_back(val);
                    }

                }
            }
        file.close();
    }
    else
        cout << "File error - OPEN" << endl;
}

