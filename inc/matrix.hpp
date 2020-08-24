#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>

struct Coord{
    int raw = 5;
    int col = 4;
};

class Matrix{

    public:
    Matrix(int heigth, int width, Coord cd, int waterLvl = 20);
    ~Matrix();
    std::vector<std::vector<bool> > mat;

};

#endif