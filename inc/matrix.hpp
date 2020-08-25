#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <cstdlib>
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
        bool moved = false;
        double getReceive(int i){this->receive[i];};
        void addReceive(int i, double v){this->receive[i] += v;};
        void resultant();
        int move(std::vector<bool> b);
        void bye();
        void hello(Matter);
        std::vector<double> receive;//eight neighbours
        std::vector<double> give;//give others drop its forces

    private:
        
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