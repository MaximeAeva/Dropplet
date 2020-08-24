#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>

struct Coord{
    int raw = 5;
    int col = 4;
};

class Matter{
    public:
        Matter(bool d);
        ~Matter();
        bool drop;//is it a water drop?
        std::vector<double> receive;//eight neighbours
        std::vector<double> give;//give others drop its forces
};

class Matrix{

    public:
        Matrix(int heigth, int width, Coord cd, int waterLvl = 20);
        ~Matrix();
       std::vector<std::vector<Matter*> > mat;

};

#endif