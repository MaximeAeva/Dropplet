#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <matter.hpp>

struct Coord{
    int raw;
    int col;
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
       inline int njMax(int dropPound, int wtrLvl, int drop8) {return dropPound*(drop8-wtrLvl);};
       float totalStrenght();

    private:
        void Transmission(float transmission, float loss);
        void Reaction(float transmission, float loss);
        void Tension(float fluidTension);
        void Gravity(float force);
        void updatePositions(bool sens);
        void updateGives(float wallLoss, float timeLoss);
        void resetMoved();
        void resetReceive();
        void resetGive();
        int height;
        int width;
};

#endif