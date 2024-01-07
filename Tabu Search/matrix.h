//
// Created by aleks on 27.11.2023.
//

#ifndef TABUSEARCH_MATRIX_H
#define TABUSEARCH_MATRIX_H
#include <string>
#include <vector>

using namespace std;

class Matrix {
private:
    vector<vector<int>> matrix;
public:
    int size=0;
    Matrix();
    ~Matrix();
    vector<vector<int>> return_matrix();
    void read_from_file(string filename);
};

#endif //TABUSEARCH_MATRIX_H
