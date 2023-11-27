#ifndef PEA_MATRIX_H
#define PEA_MATRIX_H
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
    void print_matrix();

    void set_size(int s);
};

#endif //PEA_MATRIX_H
