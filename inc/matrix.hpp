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
        int move(std::vector<bool> b);//Where to go ?
        void bye();
        void hello(Matter);
        void vertLimit(){this->speed.x *= -1;}
        void horzLimit(){this->speed.y *= -1;}
        std::vector<float> receive;//eight neighbours
        std::vector<float> give;//give others drop its forces
        float mass = 5*10e-5;//kg
        F speed;
        F acceleration;
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
       void animate(int time);

    private:
        void updateReceive();
        void updatePositions();
        void updateGives();
        void resetMoved();
        int height;
        int width;
};

#endif