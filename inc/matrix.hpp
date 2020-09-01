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

struct F{
    float x;
    float y;
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
        int move(std::vector<bool> b);//Where do I go ?
        void reverseGive(char typ, float coeff = 0.8);
        void bye();
        float strenght();
        void hello(Matter m);
        int weight;
        int giveDir;
        std::vector<float> receive;//eight neighbours
        std::vector<float> give;//give others drop its forces
        F force;
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
        void updateReceive();
        void updatePositions(bool sens);
        void updateGives();
        void resetMoved();
        void resetReceive();
        void resetGive();
        int height;
        int width;
};

#endif