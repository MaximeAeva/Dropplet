#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <matter.hpp>

/**
 * @brief A matrix of matter
 * 
 */
class Matrix{

    public:
        Matrix(int heigth, int width, Coord cd, int waterLvl = 20, int matterKindDiversity = 1);
        ~Matrix();
       std::vector<std::vector<Matter> > mat;
       void animate(int time, bool t);
       inline int njMax(int dropPound, int wtrLvl, int drop8) {return dropPound*(drop8-wtrLvl);};
       float totalStrenght();

    private:
        void Transmission(float transmission, float loss, std::vector<int> applicationVector = {0});
        void Reaction(float transmission, float loss, std::vector<int> applicationVector = {0});
        void Tension(float fluidTension, std::vector<int> applicationVector = {0});
        void Gravity(float force, std::vector<int> applicationVector = {0});
        void updatePosition(bool sens);
        void updateSpeed(float wallLoss, float timeLoss);
        void resetAcceleration();
        int height;
        int width;
};

#endif