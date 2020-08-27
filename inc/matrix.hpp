#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <iostream>

struct Coord{
    int raw;
    int col;
};

/**
 * @brief A way to give matter its own behaviour
 * 
 */
class Matter{
    public:
        Matter(bool d);
        ~Matter();
        bool drop = false;//is it a water drop?
        bool moved = false;//Has it moved ?
        void pfd();//Dynamic fundamental principe
        int move(std::vector<bool> b);//Where to go ?
        void reverseGive(bool vert, float coeff = 0.8);
        void bye();
        float strenght();
        void hello(Matter);
        int weight;
        std::vector<float> receive;//eight neighbours
        std::vector<float> give;//give others drop its forces
};

/**
 * @brief A matrix of matter
 * 
 */
class Matrix{

    public:
        Matrix(int heigth, int width, Coord cd, int waterLvl = 20);
        ~Matrix();
       std::vector<std::vector<Matter> > mat;
       void animate(int time, bool t);

    private:
        void updateReceive(bool sens);
        void updatePositions(bool sens);
        void updateGives();
        std::vector<Matter*> prioritize();
        void resetMoved();
        int height;
        int width;
};

#endif